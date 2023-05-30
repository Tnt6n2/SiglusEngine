#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_btn.h"
#include	"dialog/db_wnd_info_button.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�{�^��
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_button")

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�{�^��
// ================================================================

// ������
void C_db_wnd_info_btn::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�{�^���}�l�[�W�����"));

	// ���X�g�r���[����
	listview_add_column(_T("state"), _T("�X�e�[�g"), 200);
	listview_add_column(_T("value"), _T("�l"), 100);
}

// �������t�@���N�V����
void C_db_wnd_info_btn::on_init_func()
{
	// ���ڂ�ǉ�
	lv.add_item(0, _T("�{�^���̌�"));
	lv.add_item(0, _T("�A�N�e�B�u�ȃ{�^����GUID"));
	lv.add_item(0, _T("�A�N�e�B�u�ȃ{�^���̃I�[�_�["));
	lv.add_item(0, _T("�A�N�e�B�u�ȃ{�^���̃��C���["));
	lv.add_item(0, _T("�A�N�e�B�u�ȃ{�^���̃X�e�[�g"));
	lv.add_item(0, _T("����������"));
	lv.add_item(0, _T("�������ꂽ"));
}

// ����t�@���N�V����
void C_db_wnd_info_btn::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_btn::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;
	int item_no = -1;

	// ���݂̒l���擾
	cd.btn_cnt = Gp_btn_mng->get_btn_cnt();
	cd.active_btn_guid = Gp_btn_mng->get_active_btn_guid();
	cd.active_btn_sorter = Gp_btn_mng->get_active_btn_sorter();
	cd.active_btn_state = Gp_btn_mng->get_active_btn_state();
	cd.hit_now = Gp_btn_mng->is_hit_this_frame();
	cd.pushed_now = Gp_btn_mng->is_pushed_this_frame();

	// �{�^���̌�
	item_no ++;
	if (frame_first_flag || cd.btn_cnt != m_ld.btn_cnt)
		lv.set_cell_text(item_no, 1, tostr(cd.btn_cnt));

	// �A�N�e�B�u�ȃ{�^����GUID
	item_no ++;
	if (frame_first_flag || cd.active_btn_guid != m_ld.active_btn_guid)
		lv.set_cell_text(item_no, 1, tostr(cd.active_btn_guid));

	// �A�N�e�B�u�ȃ{�^���̃I�[�_�[
	item_no ++;
	if (frame_first_flag || cd.active_btn_sorter.order != m_ld.active_btn_sorter.order)
		lv.set_cell_text(item_no, 1, tostr(cd.active_btn_sorter.order));

	// �A�N�e�B�u�ȃ{�^���̃��C���[
	item_no ++;
	if (frame_first_flag || cd.active_btn_sorter.layer != m_ld.active_btn_sorter.layer)
		lv.set_cell_text(item_no, 1, tostr(cd.active_btn_sorter.layer));

	// �A�N�e�B�u�ȃ{�^���̃X�e�[�g
	item_no ++;
	if (frame_first_flag || cd.active_btn_state != m_ld.active_btn_state)	{
		switch (cd.active_btn_state)	{
			case TNM_BTN_STATE_NORMAL:	lv.set_cell_text(item_no, 1, _T("�ʏ�"));			break;
			case TNM_BTN_STATE_HIT:		lv.set_cell_text(item_no, 1, _T("�������Ă���"));	break;
			case TNM_BTN_STATE_PUSH:	lv.set_cell_text(item_no, 1, _T("�����Ă���"));		break;
			default:					lv.set_cell_text(item_no, 1, _T("???"));			break;
		}
	}

	// ����
	item_no ++;
	if (frame_first_flag || cd.hit_now != m_ld.hit_now)
		lv.set_cell_text(item_no, 1, cd.hit_now ? _T("��������") : _T(""));

	item_no ++;
	if (frame_first_flag || cd.pushed_now != m_ld.pushed_now)
		lv.set_cell_text(item_no, 1, cd.pushed_now ? _T("������") : _T(""));

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}

