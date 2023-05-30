#include	"pch.h"
#include	"ifc_eng.h"
#include	"element/elm_stage.h"
#include	"element/elm_excall.h"
#include	"dialog/db_element_info.h"
#include	"dialog/db_wnd_info_object.h"
#include	"dialog/db_wnd_info_element_lv.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�I�u�W�F�N�g
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_object")

// ������
void C_db_wnd_info_object::init()
{
}

// �������t�@���N�V����
void C_db_wnd_info_object::on_init_func()
{
	// ���X�g�r���[����
	tnm_init_lv_element(FM_OBJECTLIST, true, &lv);
}

// ����t�@���N�V����
void C_db_wnd_info_object::on_close_func()
{
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_object::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}

	return C_db_wnd_info_listview::window_proc(msg, wp, lp);
}

// �ʒm
bool C_db_wnd_info_object::on_notify(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);
	NMHDR* hdr = (NMHDR *)lp;

	if (ctrl_id == lv.get_id())	{
		if (hdr->code == NM_DBLCLK)	{
			on_dblclk_lv();
		}
		else if (hdr->code == LVN_CHANGE_TEXT)	{
			on_change_lv_value(((NMLVCHANGE *)lp)->item_no, ((NMLVCHANGE *)lp)->column_no, ((NMLVCHANGE *)lp)->new_text);
		}
	}

	return true;
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_object::frame()
{
	if (!m_h_wnd)
		return;

	// �I�u�W�F�N�g���X�g���擾����
	C_elm_object_list* p_object_list = NULL;
	get_page_object(&p_object_list);

	// ���X�g�r���[���X�V����
	if (p_object_list)	{
		int item_no = 0;
		tnm_update_lv_list_table(p_object_list, &lv, &item_no, true);
	}
}

// ****************************************************************
// ���X�g�r���[���_�u���N���b�N����
// ================================================================
void C_db_wnd_info_object::on_dblclk_lv()
{
	// �I����Ԏ擾
	int select_cnt = lv.get_selected_item_cnt();
	int select_no = lv.get_selected_item_no();

	// �}�E�X�J�[�\���̏���Ă���Z���ԍ�
	int item_no, column_no;
	C_point mp = lv.screen_to_client(get_mouse_pos());
	lv.get_cell_by_point(mp.x, mp.y, &item_no, &column_no);

	// �A�C�e����ύX����
	if (select_cnt == 1 && select_no == item_no)	{

		// �I�u�W�F�N�g���擾����
		C_elm_object_list* p_object_list = NULL;
		get_page_object(&p_object_list);
		if (p_object_list)	{
			tnm_on_dblclk_lv_object_list_table(&lv, item_no, column_no);
		}
	}
}

// ****************************************************************
// ���X�g�r���[�̒l��ύX����
// ================================================================
void C_db_wnd_info_object::on_change_lv_value(int item_no, int column_no, CTSTR& value)
{
	// �I�u�W�F�N�g���擾����
	C_elm_object_list* p_object_list = NULL;
	get_page_object(&p_object_list);

	// ���X�g�r���[�̒l���X�V����
	if (p_object_list)	{
		tnm_on_change_lv_value_object_list_table(&lv, item_no, column_no, value, p_object_list);
	}
}

// ****************************************************************
// �I�u�W�F�N�g���擾����
// ================================================================
void C_db_wnd_info_object::get_page_object(C_elm_object_list** pp_object_list)
{
	*pp_object_list = NULL;
	switch (m_page_no)	{
		case 0:		*pp_object_list = &(*Gp_stage_list)[TNM_STAGE_BACK].m_object_list;			break;
		case 1:		*pp_object_list = &(*Gp_stage_list)[TNM_STAGE_FRONT].m_object_list;			break;
		case 2:		*pp_object_list = &(*Gp_stage_list)[TNM_STAGE_NEXT].m_object_list;			break;
		case 3:		if (Gp_excall->is_ready())	{	*pp_object_list = &Gp_excall->m_stage_list[TNM_STAGE_BACK].m_object_list;	}	break;
		case 4:		if (Gp_excall->is_ready())	{	*pp_object_list = &Gp_excall->m_stage_list[TNM_STAGE_FRONT].m_object_list;	}	break;
		case 5:		if (Gp_excall->is_ready())	{	*pp_object_list = &Gp_excall->m_stage_list[TNM_STAGE_NEXT].m_object_list;	}	break;
	}
}

