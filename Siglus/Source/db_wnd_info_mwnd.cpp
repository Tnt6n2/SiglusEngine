#include	"pch.h"
#include	"element\elm_mwnd.h"
#include	"dialog\db_wnd_info_mwnd.h"

#include	"..\resource.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F���b�Z�[�W�E�B���h�E
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_mwnd")

// ������
void C_db_wnd_info_mwnd::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_MWND);
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_mwnd::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// ������
bool C_db_wnd_info_mwnd::on_init()
{
	// ���X�g�r���[��ID�Ɩ��O�A�^�u�̃y�[�W��
	int lv_id[PAGE_CNT] = {IDC_LV_BACK, IDC_LV_FRONT};
	TCHAR* lv_name[PAGE_CNT] = {_T("lv_back"), _T("lv_front")};
	TCHAR* tab_caption[PAGE_CNT] = {_T("back"), _T("front")};

	// ������
	for (int i = 0; i < PAGE_CNT; i++)
		m_first[i] = true;

	// �A�C�e���o�^
	bind_control(&ci.tab, IDC_TAB, _T("tab"), 5);
	for (int i = 0; i < PAGE_CNT; i++)
		bind_control(&ci.lv[i], lv_id[i], lv_name[1], 5, 1);

	// �Z�p���[�^�o�^

	// �^�u�̐ݒ�
	for (int i = 0; i < PAGE_CNT; i++)	{
		ci.tab.add_item(tab_caption[i]);
	}
	C_rect tab_rect = screen_to_client(ci.tab.get_display_rect());	// �^�u�̕\���̈�����߂�
	tab_rect.left -= 2;		// ������

	// �A�C�e���ݒ胍�[�h
	for (int i = 0; i < PAGE_CNT; i++)	{
		ci.lv[i].easy_setting(true, true, false, true);

		int column_width = 20;
		ci.lv[i].add_column(_T("id"), _T("ID"), column_width);
		ci.lv[i].add_column(_T("world"), _T("ܰ���"), column_width);
		ci.lv[i].add_column(_T("layer"), _T("ڲ԰"), column_width);
		ci.lv[i].add_column(_T("appear"), _T("�o��"), column_width);
		ci.lv[i].add_column(_T("msg"), _T("ү����"), column_width);
		ci.lv[i].load_ini();

		// ���X�g�r���[�̏����ʒu��ݒ�
		ci.lv[i].set_def_rltv_rect(tab_rect);
	}

	// �Z�p���[�^�ݒ胍�[�h

	// �E�B���h�E��`�̓ǂݍ���
	load_ini_window_rect();
	
	// �A�C�e���t�H�[�J�X
	change_page(m_page_no);

	// �E�B���h�E�\��
	update();	set_show(true);

	return true;
}

// ����
bool C_db_wnd_info_mwnd::on_close()
{
	// �X�e�[�^�X�̎擾�ƕۑ�
	for (int i = 0; i < PAGE_CNT; i++)	{
		ci.lv[i].save_ini();
	}

	// �Z�p���[�^�ݒ�Z�[�u

	// �E�B���h�E��`�̕ۑ�
	save_ini_window_rect();

	return true;
}

// �R�}���h
bool C_db_wnd_info_mwnd::on_command(WPARAM wp, LPARAM lp)
{
//	switch (LOWORD(wp))	{
//	}
	return true;
}

// �ʒm
bool C_db_wnd_info_mwnd::on_notify(WPARAM wp, LPARAM lp)
{
	NMHDR* hdr = (NMHDR *)lp;

	switch (hdr->code)	{
		case TCN_SELCHANGE:
			on_change_page(ci.tab.get_selected_item_no());
			break;
	}

	return true;
}

// ****************************************************************
// �X�e�[�^�X�ǂݍ���
// ================================================================
void C_db_wnd_info_mwnd::load_state()
{
	m_page_no = C_ini_file::body().load_int(_T("page_no"), 1, regist_state.window_name);
		// �f�t�H���g�̃y�[�W�ԍ��F1 = front
}

// ****************************************************************
// �X�e�[�^�X�ۑ�
// ================================================================
void C_db_wnd_info_mwnd::save_state()
{
	C_ini_file::body().save_int(_T("page_no"), m_page_no, regist_state.window_name);
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_mwnd::frame(C_elm_mwnd_list** mwnd_list_list, int list_cnt)
{
	if (!is_open())
		return;

	STATE cd;

	int lv_no;			// �\������Ă��郊�X�g�r���[
	int stage_no;		// �\������t���O���X�g�ԍ�

	switch (m_page_no)	{
		case 0:		lv_no = 0;	stage_no = 0;	break;
		case 1:		lv_no = 1;	stage_no = 1;	break;
	}

	// ================================================================
	// ���b�Z�[�W�E�B���h�E�����擾����
	// ----------------------------------------------------------------
	//		appear_flag �������Ă�����̂̂݃��X�g�r���[�ɕ\������
	//		���X�g�r���[�ɕ\��������̂������擾���Ă���
	// ----------------------------------------------------------------

	cd.mwnd_list[stage_no].clear();
	int mwnd_cnt = mwnd_list_list[stage_no]->get_size();
	for (int i = 0; i < mwnd_cnt; i++)	{
		C_elm_mwnd* src = &(*mwnd_list_list[stage_no])[i];

		// �E�B���h�E���o�����Ă�����̂ɂ��Ă̂�
		if (src->get_window_appear_flag())	{
			int now_size = (int)cd.mwnd_list[stage_no].size();
			cd.mwnd_list[stage_no].resize(now_size + 1);
			STATE::SUB* dst = &cd.mwnd_list[stage_no][now_size];
	
			// �I�u�W�F�N�g�̏����擾����
			dst->mwnd_no = i;
			dst->world = src->get_world();
			dst->layer = src->get_layer();
			dst->appear_flag = src->get_window_appear_flag();
			dst->msg = src->get_debug_msg();
				// �����̑��̃p�����[�^�F������
		}
	}

	// ���X�g�r���[�̃A�C�e������ݒ�
	if (m_first[m_page_no] || cd.mwnd_list[stage_no].size() != m_ld.mwnd_list[stage_no].size())
		ci.lv[lv_no].set_item_cnt((int)cd.mwnd_list[stage_no].size());

	// �������������͕ύX���ꂽ���Ɋւ��Ēl��\��
	for (int i = 0; i < (int)cd.mwnd_list[stage_no].size(); i++)	{
		int column_no = -1;

		// ����������͑��������Ɋւ��Ă͋����\��
		bool change = m_first[m_page_no] || i >= (int)m_ld.mwnd_list[stage_no].size();

		// �킩��₷�����邽�߂ɏ��ւ̃|�C���^���擾���Ă���
		STATE::SUB* now = &cd.mwnd_list[stage_no][i];
		STATE::SUB* last = change ? NULL : &m_ld.mwnd_list[stage_no][i];

		// ���b�Z�[�W�E�B���h�E�ԍ�
		column_no ++;
		if (change || now->mwnd_no != last->mwnd_no)	{
			ci.lv[lv_no].set_cell_text(i, column_no, tostr(now->mwnd_no));
		}
	
		// ���[���h
		column_no ++;
		if (change || now->world != last->world)	{
			ci.lv[lv_no].set_cell_text(i, column_no, tostr(now->world));
		}

		// ���C���[
		column_no ++;
		if (change || now->layer != last->layer)	{
			ci.lv[lv_no].set_cell_text(i, column_no, tostr(now->layer));
		}

		// �o��
		column_no ++;
		if (change || now->appear_flag != last->appear_flag)	{
			ci.lv[lv_no].set_cell_text(i, column_no, now->appear_flag ? _T("1") : _T("0"));
		}

		// �t�@�C����
		column_no ++;
		if (change || now->msg != last->msg)		{
			ci.lv[lv_no].set_cell_text(i, column_no, now->msg);
		}
	}

	// �O��̒l��ۑ��i���݂̃y�[�W�̂݁j
	m_ld.mwnd_list[stage_no] = cd.mwnd_list[stage_no];

	// ����t���O
	m_first[m_page_no] = false;
}

void C_db_wnd_info_mwnd::on_change_page(int page_no)
{
	// ���݂̃y�[�W���\���ɂ���
	ci.lv[m_page_no].set_show(false);

	// �y�[�W�ԍ����X�V
	m_page_no = page_no;

	// �V�����y�[�W��\������
	ci.lv[m_page_no].set_show(true);
}

void C_db_wnd_info_mwnd::change_page(int page_no)
{
	// �^�u�R���g���[�����ړ�
	ci.tab.set_selected_item_no(page_no);

	// �y�[�W���ύX���ꂽ�Ƃ��̏������s��
	on_change_page(page_no);
}

