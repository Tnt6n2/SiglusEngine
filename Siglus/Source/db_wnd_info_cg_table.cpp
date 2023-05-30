#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_cg_table.h"
#include	"dialog/db_wnd_info_cg_table.h"

#include	"../resource.h"

// ���I�[�v�����A�E�B���h�E�T�C�Y�����T�C�Y���Ȃ������ꍇ�A���X�g���\������Ȃ��i�T�C�Y��ύX����Ε\������܂��j

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�b�f�e�[�u��
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_cg_table")

// ���X�g�����X�g�̍���
const int	LST_ORIGINAL_COLMUN_CNT = 9;
static TCHAR*	lst_original_colmun_id[LST_ORIGINAL_COLMUN_CNT] = {_T("ind"), _T("name"), _T("look"), _T("flag"), _T("c0"), _T("c1"), _T("c2"), _T("c3"), _T("c4"),};
static TCHAR*	lst_original_colmun_str[LST_ORIGINAL_COLMUN_CNT] = {_T("Ind"), _T("Name"), _T("Look"), _T("Flag"), _T("C0"), _T("C1"), _T("C2"), _T("C3"), _T("C4"),};
static int		lst_original_colmun_width[LST_ORIGINAL_COLMUN_CNT] = {35, 100, 40, 60, 30, 30, 30, 30, 30,};

// �O���[�v���X�g�̍���
const int	LST_GROUP_COLMUN_CNT = 14;
static TCHAR*	lst_group_colmun_id[LST_GROUP_COLMUN_CNT] = {_T("ind"), _T("name"), _T("look"), _T("g0"), _T("g1"), _T("g2"), _T("g3"), _T("g4"), _T("flag"), _T("c0"), _T("c1"), _T("c2"), _T("c3"), _T("c4"),};
static TCHAR*	lst_group_colmun_str[LST_GROUP_COLMUN_CNT] = {_T("Ind (ListNo)"), _T("Name"), _T("Look"), _T("G0"), _T("G1"), _T("G2"), _T("G3"), _T("G4"), _T("Flag"), _T("C0"), _T("C1"), _T("C2"), _T("C3"), _T("C4"),};
static int		lst_group_colmun_width[LST_GROUP_COLMUN_CNT] = {75, 100, 40, 30, 30, 30, 30, 30, 60, 30, 30, 30, 30, 30,};

// ������
void C_db_wnd_info_cg_table::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_CG_TABLE);
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_cg_table::window_proc(UINT msg, WPARAM wp, LPARAM lp)
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
bool C_db_wnd_info_cg_table::on_init()
{
	// �A�C�e���o�^
	bind_control(&ci.tab, IDC_TAB, _T("tab"), 5);
	bind_control(&ci.lst_original, IDC_LST_ORIGINAL, _T("lst_original"), 5);
	bind_control(&ci.lst_group, IDC_LST_GROUP, _T("lst_group"), 5);

	// �^�u�̐ݒ�
	ci.tab.add_item(_T("���X�g��"));
	ci.tab.add_item(_T("�O���[�v"));
	C_rect tab_rect = screen_to_client(ci.tab.get_display_rect());	// �^�u�̕\���̈�����߂�
	tab_rect.left -= 2;		// ������
	int tab_no = ci.tab.load_ini_int(_T("TAB_SELECT_NO"), 0);
	ci.tab.set_selected_item_no(tab_no);

	// ���X�g�����X�g�̐ݒ�
	ci.lst_original.easy_setting(true, true, false, false);
	ci.lst_original.set_def_rltv_rect(tab_rect);
	for (int i = 0; i < LST_ORIGINAL_COLMUN_CNT; i++)	{
		ci.lst_original.add_column(lst_original_colmun_id[i], lst_original_colmun_str[i], lst_original_colmun_width[i]);
	}
	ci.lst_original.load_ini();

	// �A�C�e����ǉ�
	C_cg_table_data* cg_table = Gp_cg_table->get_ptr_to_cg_table_data();
	if (!cg_table->cg_table_data_list.empty())	{
		TSTR str;
		S_cg_table_data_sub *sub =  cg_table->cg_table_data_list.get();
		for (int i = 0; i < (int)cg_table->cg_table_data_list.size(); i++ )	{
			ci.lst_original.add_item(0, str_format(_T("%03d"), i));
			ci.lst_original.set_cell_text(i, 1, sub->name);
			ci.lst_original.set_cell_text(i, 3, str_format(_T("flag[%04d]"), sub->flag_no));
			for (int j = 0; j < CG_TABLE_DATA_CODE_MAX; j++ )	{
				ci.lst_original.set_cell_text(i, 4 + j, str_format(_T("%03d"), sub->code[j]));
			}
			sub++;
		}
	}

	// �O���[�v���X�g�̐ݒ�
	ci.lst_group.easy_setting(true, true, false, false);
	ci.lst_group.set_def_rltv_rect(tab_rect);
	for (int i = 0; i < LST_GROUP_COLMUN_CNT; i++)	{
		ci.lst_group.add_column(lst_group_colmun_id[i], lst_group_colmun_str[i], lst_group_colmun_width[i]);
	}
	ci.lst_group.load_ini();

	// �A�C�e����ǉ�
	if (!cg_table->cg_table_sort_list.empty())	{
		TSTR str;
		S_cg_table_data_sub *sub;
		for (int i = 0; i < (int)cg_table->cg_table_sort_list.size(); i++ )	{
			sub = cg_table->cg_table_sort_list[i];
			ci.lst_group.add_item(0, str_format(_T("%03d"), i));
			ci.lst_group.set_cell_text(i, 1, sub->name);
			ci.lst_group.set_cell_text(i, 8, str_format(_T("flag[%04d]"), sub->flag_no));
			for (int j = 0; j < CG_TABLE_DATA_CODE_MAX; j++ )	{
				ci.lst_group.set_cell_text(i, 3 + j, str_format(_T("%03d"), sub->group[j]));
				ci.lst_group.set_cell_text(i, 9 + j, str_format(_T("%03d"), sub->code[j]));
			}
		}
	}

	// �E�B���h�E��`�̓ǂݍ���
	load_ini_window_rect();

	// �y�[�W�ύX
	change_page(ci.tab.get_selected_item_no());

	// �T�C�Y�̍X�V
	send_wm_size();

	// �E�B���h�E�\��
	update();
	set_show(true);

	return true;
}

// ����
bool C_db_wnd_info_cg_table::on_close()
{
	// �A�C�e���ݒ�Z�[�u
	ci.tab.save_ini_int(_T("TAB_SELECT_NO"), ci.tab.get_selected_item_no());
	ci.lst_original.save_ini();
	ci.lst_group.save_ini();

	// �E�B���h�E��`�̕ۑ�
	save_ini_window_rect();

	return true;
}

// �R�}���h
bool C_db_wnd_info_cg_table::on_command(WPARAM wp, LPARAM lp)
{
//	switch (LOWORD(wp))	{
//	}
	return true;
}

// �ʒm
bool C_db_wnd_info_cg_table::on_notify(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);
	NMHDR* hdr = (NMHDR *)lp;

	switch (ctrl_id)	{
		case IDC_TAB:
			switch (hdr->code)	{
				case TCN_SELCHANGE:
					change_page(ci.tab.get_selected_item_no());
					break;
			}
			break;
		case IDC_LST_ORIGINAL:
		case IDC_LST_GROUP:
			switch (hdr->code)	{
				case NM_DBLCLK:
					on_dblclk_lv(ctrl_id);
					break;
				case LVN_CHANGE_TEXT:
					change_flag(ctrl_id, ((NMLVCHANGE *)lp)->item_no, str_to_int(((NMLVCHANGE *)lp)->new_text, 0));
					break;
			}
			break;
	}

	return true;
}

// ****************************************************************
// �X�e�[�^�X�ǂݍ���
// ================================================================
void C_db_wnd_info_cg_table::load_state()
{
}

// ****************************************************************
// �X�e�[�^�X�ۑ�
// ================================================================
void C_db_wnd_info_cg_table::save_state()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_cg_table::frame()
{
	if (!m_h_wnd)
		return;

	C_cg_table_data* cg_table = Gp_cg_table->get_ptr_to_cg_table_data();

	if (!cg_table->cg_table_data_list.empty())	{
		TSTR str;
		S_cg_table_data_sub *sub = cg_table->cg_table_data_list.get();
		for (int i = 0; i < (int)cg_table->cg_table_data_list.size(); i++ )	{
			ci.lst_original.set_cell_text(i, 1, sub->name);

			// �b�f�e�[�u���F�t���O�̒l����u�����^���ĂȂ��v��ݒ�i�� or �~�j
			int flag_value = 0;
			Gp_cg_table->get_flag_value_from_flag_no(sub->flag_no, &flag_value);
			ci.lst_original.set_cell_text(i, 2, tostr(flag_value));
			sub++;
		}
	}

	if (!cg_table->cg_table_sort_list.empty())	{
		TSTR str;
		S_cg_table_data_sub *sub;
		for (int i = 0; i < (int)cg_table->cg_table_sort_list.size(); i++ )	{
			sub = cg_table->cg_table_sort_list[i];

			// �b�f�e�[�u���F�t���O�̒l����u�����^���ĂȂ��v��ݒ�i�� or �~�j
			int flag_value = 0;
			Gp_cg_table->get_flag_value_from_flag_no(sub->flag_no, &flag_value);
			ci.lst_group.set_cell_text(i, 2, tostr(flag_value));
		}
	}
}

// ****************************************************************
// �R���g���[���h�c���烊�X�g�r���[���擾����
// ================================================================
C_tnm_dbg_listview* C_db_wnd_info_cg_table::get_lv_by_ctrl_id(int ctrl_id)
{
	C_tnm_dbg_listview* p_lv = NULL;
	switch (ctrl_id)	{
		case IDC_LST_ORIGINAL:	p_lv = &ci.lst_original;	break;
		case IDC_LST_GROUP:		p_lv = &ci.lst_group;		break;
	}
	return p_lv;
}

// ****************************************************************
// ���X�g�r���[���_�u���N���b�N����
// ================================================================
void C_db_wnd_info_cg_table::on_dblclk_lv(int ctrl_id)
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
		if (select_cnt == 1 && select_no == onmouse_item_no && onmouse_column_no == 2)	{
			p_lv->edit_item_text_edit(onmouse_item_no, onmouse_column_no);
		}
	}
}

// ****************************************************************
// �t���O��ύX����
// ================================================================
void C_db_wnd_info_cg_table::change_flag(int ctrl_id, int item_no, int value)
{
	C_tnm_dbg_listview* p_lv = get_lv_by_ctrl_id(ctrl_id);
	if (p_lv)	{
		
		C_cg_table_data* cg_table = Gp_cg_table->get_ptr_to_cg_table_data();
		if (!cg_table->cg_table_data_list.empty())	{

			// �ʏ�ł̏���
			if (ctrl_id == IDC_LST_ORIGINAL)	{
				if (0 <= item_no && item_no < (int)cg_table->cg_table_data_list.size())	{
					S_cg_table_data_sub *sub = &cg_table->cg_table_data_list[item_no];
					Gp_cg_table->set_flag_value_from_flag_no(sub->flag_no, value);
				}
			}
			// ���X�g�ł̏���
			else if (ctrl_id == IDC_LST_GROUP)	{
				if (0 <= item_no && item_no < (int)cg_table->cg_table_sort_list.size())	{
					S_cg_table_data_sub *sub = cg_table->cg_table_sort_list[item_no];
					Gp_cg_table->set_flag_value_from_flag_no(sub->flag_no, value);
				}
			}
		}
	}
}

// ****************************************************************
// �y�[�W��ύX����
// ================================================================
void C_db_wnd_info_cg_table::change_page(int page_no)
{
	int tab_no = ci.tab.get_selected_item_no();

	if (tab_no == 0)	{
		ci.lst_original.set_show(true);
		ci.lst_group.set_show(false);
	}
	else	{
		ci.lst_original.set_show(false);
		ci.lst_group.set_show(true);
	}
}

