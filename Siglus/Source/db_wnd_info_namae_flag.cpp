#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_form_code.h"
#include	"element/elm_flag.h"
#include	"engine/eng_flag.h"
#include	"dialog/db_wnd_info_namae_flag.h"
#include	"../resource.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F���O�t���O
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_namae_flag")

// ������
void C_db_wnd_info_namae_flag::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_NAMAE_FLAG);
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_namae_flag::window_proc(UINT msg, WPARAM wp, LPARAM lp)
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
bool C_db_wnd_info_namae_flag::on_init()
{
	// ���X�g�r���[��ID�Ɩ��O�A�^�u�̃y�[�W��
	int lv_id[PAGE_CNT] = {IDC_LV_GLOBAL, IDC_LV_LOCAL};
	TCHAR* lv_name[PAGE_CNT] = {_T("lv_global"), _T("lv_local")};
	TCHAR* tab_caption[PAGE_CNT] = {_T("��"), _T("��")};

	// ������
	for (int i = 0; i < PAGE_CNT; i++)
		m_first[i] = true;

	// �A�C�e���o�^
	bind_control(&ci.tab, IDC_TAB, _T("tab"), 5);
	for (int i = 0; i < PAGE_CNT; i++)
		bind_control(&ci.lv_flag[i], lv_id[i], lv_name[i], 5, 1);

	// �Z�p���[�^�o�^

	// �^�u�̐ݒ�
	for (int i = 0; i < PAGE_CNT; i++)
		ci.tab.add_item(tab_caption[i]);

	C_rect tab_rect = screen_to_client(ci.tab.get_display_rect());	// �^�u�̕\���̈�����߂�
	tab_rect.left -= 2;		// ������

	// �A�C�e���ݒ胍�[�h
	for (int i = 0; i < PAGE_CNT; i++)	{
		ci.lv_flag[i].easy_setting(true, true, false, true);
		ci.lv_flag[i].add_column(_T("flag"), _T("�t���O"), 100);
		ci.lv_flag[i].add_column(_T("value"), _T("�l"), 100);
		ci.lv_flag[i].load_ini();

		// ���X�g�r���[�̏����ʒu��ݒ�
		ci.lv_flag[i].set_def_rltv_rect(tab_rect);
	}

	// �Z�p���[�^�ݒ胍�[�h

	// �E�B���h�E��`�̓ǂݍ���
	load_ini_window_rect();
	
	// �A�C�e���t�H�[�J�X
	change_page(m_page_no);

	// �T�C�Y�̍X�V
	send_wm_size();

	// �E�B���h�E�\��
	update();	set_show(true);

	return true;
}

// ����
bool C_db_wnd_info_namae_flag::on_close()
{
	// �X�e�[�^�X�̎擾�ƕۑ�
	for (int i = 0; i < PAGE_CNT; i++)
		ci.lv_flag[i].save_ini();

	// �Z�p���[�^�ݒ�Z�[�u

	// �E�B���h�E��`�̕ۑ�
	save_ini_window_rect();

	return true;
}

// �R�}���h
bool C_db_wnd_info_namae_flag::on_command(WPARAM wp, LPARAM lp)
{
//	switch (LOWORD(wp))	{
//	}
	return true;
}

// �ʒm
bool C_db_wnd_info_namae_flag::on_notify(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);
	NMHDR* hdr = (NMHDR *)lp;

	switch (ctrl_id)	{
		case IDC_TAB:
			switch (hdr->code)	{
				case TCN_SELCHANGE:
					on_change_page(ci.tab.get_selected_item_no());
					break;
			}
			break;
		case IDC_LV_GLOBAL:
		case IDC_LV_LOCAL:
			switch (hdr->code)	{
				case NM_DBLCLK:
					on_dblclk_lv(ctrl_id, FM_STR);
					break;
				case LVN_CHANGE_TEXT:
					change_str_flag(ctrl_id, ((NMLVCHANGE *)lp)->item_no, ((NMLVCHANGE *)lp)->new_text);
					break;
			}
			break;
	}

	return true;
}

// ****************************************************************
// �X�e�[�^�X�ǂݍ���
// ================================================================
void C_db_wnd_info_namae_flag::load_state()
{
	m_page_no = C_ini_file::body().load_int(_T("page_no"), 0, regist_state.window_name);
}

// ****************************************************************
// �X�e�[�^�X�ۑ�
// ================================================================
void C_db_wnd_info_namae_flag::save_state()
{
	C_ini_file::body().save_int(_T("page_no"), m_page_no, regist_state.window_name);
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_namae_flag::frame()
{
	if (!is_open())
		return;

	STATE cd;

	// �y�[�W������
	if (m_page_no < 0 || PAGE_CNT <= m_page_no)
		return;

	// �A���t�@�x�b�g�S�p
	static TCHAR* g_alpha = _T("�`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y");

	// ������t���O
	TSTR flag_name;		// ���O�t���O��
	switch (m_page_no)	{
		case 0:		flag_name = _T("��");	cd.str_flag[m_page_no] = Gp_flag->global_namae.get_sub();	break;
		case 1:		flag_name = _T("��");	cd.str_flag[m_page_no] = Gp_flag->local_namae.get_sub();	break;
	}

	// ���X�g�r���[�̃A�C�e������ݒ�
	if (m_first[m_page_no] || cd.str_flag[m_page_no].size() != m_ld.str_flag[m_page_no].size())	{
		ci.lv_flag[m_page_no].set_item_cnt((int)cd.str_flag[m_page_no].size());
	}
	// ���������Ɋւ��ăt���O����\��
	for (int i = m_first[m_page_no] ? 0 : (int)m_ld.str_flag[m_page_no].size(); i < (int)cd.str_flag[m_page_no].size(); i++)	{
		TSTR this_flag_name;

		if (0 <= i && i < 26)	{
			this_flag_name = flag_name + g_alpha[i];
		}
		else	{
			int m1 = i / 26 - 1;
			int m2 = i % 26;
			this_flag_name = flag_name + g_alpha[m1] + g_alpha[m2];
		}

		ci.lv_flag[m_page_no].set_cell_text(i, 0, this_flag_name);
	}
	// �������������͕ύX���ꂽ���Ɋւ��Ēl��\��
	for (int i = 0; i < (int)cd.str_flag[m_page_no].size(); i++)	{
		if (m_first[m_page_no] || i >= (int)m_ld.str_flag[m_page_no].size() || cd.str_flag[m_page_no][i] != m_ld.str_flag[m_page_no][i])	{
			ci.lv_flag[m_page_no].set_cell_text(i, 1, cd.str_flag[m_page_no][i]);
		}
	}

	// �O��̒l��ۑ��i���݂̃y�[�W�̂݁j
	m_ld.str_flag[m_page_no] = cd.str_flag[m_page_no];

	// ����t���O
	m_first[m_page_no] = false;
}

// ****************************************************************
// �R���g���[���h�c���烊�X�g�r���[���擾����
// ================================================================
C_tnm_dbg_listview* C_db_wnd_info_namae_flag::get_lv_by_ctrl_id(int ctrl_id)
{
	C_tnm_dbg_listview* p_lv = NULL;
	switch (ctrl_id)	{
		case IDC_LV_GLOBAL:	p_lv = &ci.lv_flag[0];		break;
		case IDC_LV_LOCAL:	p_lv = &ci.lv_flag[1];		break;
	}
	return p_lv;
}

// ****************************************************************
// �R���g���[���h�c���當���񃊃X�g���擾����
// ================================================================
C_elm_str_list* C_db_wnd_info_namae_flag::get_str_list_by_ctrl_id(int ctrl_id)
{
	C_elm_str_list* p_str_list = NULL;
	switch (ctrl_id)	{
		case IDC_LV_GLOBAL:	p_str_list = &Gp_flag->global_namae;	break;
		case IDC_LV_LOCAL:	p_str_list = &Gp_flag->local_namae;		break;
	}
	return p_str_list;
}

// ****************************************************************
// ���X�g�r���[���_�u���N���b�N����
// ================================================================
void C_db_wnd_info_namae_flag::on_dblclk_lv(int ctrl_id, int form)
{
	C_tnm_dbg_listview* p_lv = get_lv_by_ctrl_id(ctrl_id);
	if (p_lv)	{

		// �I����Ԏ擾
		int select_cnt = p_lv->get_selected_item_cnt();
		int select_no = p_lv->get_selected_item_no();

		// �}�E�X�J�[�\���̏���Ă���Z���ԍ�
		int onmouse_item_no, onmouse_column_no;
		C_point mp = p_lv->screen_to_client(get_mouse_pos());
		p_lv->get_cell_by_point(mp.x, mp.y, &onmouse_item_no, &onmouse_column_no);

		// �A�C�e����ύX����
		if (select_cnt == 1 && select_no == onmouse_item_no && onmouse_column_no == 1)	{
			if (false);
			else if (form == FM_STR)	{
				TSTR value = p_lv->get_cell_text(onmouse_item_no, onmouse_column_no);
				p_lv->edit_item_text_edit(onmouse_item_no, onmouse_column_no);
			}
		}
	}
}

// ****************************************************************
// ������t���O��ύX����
// ================================================================
void C_db_wnd_info_namae_flag::change_str_flag(int ctrl_id, int flag_no, CTSTR& value)
{
	C_elm_str_list* p_str_list = get_str_list_by_ctrl_id(ctrl_id);
	if (p_str_list)	{
		if (0 <= flag_no && flag_no < p_str_list->get_size())
			(*p_str_list)[flag_no] = value;
	}
}

// ****************************************************************
// �y�[�W���ύX���ꂽ
// ================================================================
void C_db_wnd_info_namae_flag::on_change_page(int page_no)
{
	// ���݂̃y�[�W���\���ɂ���
	ci.lv_flag[m_page_no].set_show(false);

	// �y�[�W�ԍ����X�V
	m_page_no = page_no;

	// �V�����y�[�W��\������
	ci.lv_flag[m_page_no].set_show(true);
}

// ****************************************************************
// �y�[�W��ύX����
// ================================================================
void C_db_wnd_info_namae_flag::change_page(int page_no)
{
	// �^�u�R���g���[�����ړ�
	ci.tab.set_selected_item_no(page_no);

	// �y�[�W���ύX���ꂽ�Ƃ��̏������s��
	on_change_page(page_no);
}

