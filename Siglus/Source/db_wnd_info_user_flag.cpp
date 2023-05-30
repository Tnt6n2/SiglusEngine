#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_form_code.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"element/elm_flag.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_flag.h"
#include	"dialog/db_wnd_info_user_flag.h"

const int	ITEM_FM_INT = 0;
const int	ITEM_FM_INTLIST = 1;
const int	ITEM_FM_INTLIST_SUB = 2;
const int	ITEM_FM_STR = 3;
const int	ITEM_FM_STRLIST = 4;
const int	ITEM_FM_STRLIST_SUB = 5;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F���[�U�v���p�e�B
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_user_flag")

// ������
void C_db_wnd_info_user_flag::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("���[�U�[�t���O���"));

	// ���X�g�r���[����
	listview_add_column(_T("id"), _T("���"), 100);
	listview_add_column(_T("form"), _T("�^"), 50);
	listview_add_column(_T("name"), _T("���O"), 100);
	listview_add_column(_T("value"), _T("�l"), 100);
}

// �������t�@���N�V����
void C_db_wnd_info_user_flag::on_init_func()
{
}

// ����t�@���N�V����
void C_db_wnd_info_user_flag::on_close_func()
{
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_user_flag::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}

	return C_db_wnd_info_listview::window_proc(msg, wp, lp);
}

// �ʒm
bool C_db_wnd_info_user_flag::on_notify(WPARAM wp, LPARAM lp)
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
void C_db_wnd_info_user_flag::frame()
{
	if (!m_h_wnd)
		return;

	int item_no = -1;

	STATE cd;

	// �C���N�v���p�e�B
	for (int i = 0; i < (int)Gp_user_inc_prop->prop_list.get_size(); i++)	{
		C_tnm_prop* p_prop = &Gp_user_inc_prop->prop_list[i];
		item_no ++;

		cd.item_list.resize(item_no + 1);
		cd.item_list[item_no].scn_no = -1;	// �V�[���ԍ��i-1 �� inc�j
		cd.item_list[item_no].prop_id = i;	// �v���p�e�B�h�c
		switch (Gp_lexer->get_user_inc_prop_form(i))	{
			case FM_INT:		cd.item_list[item_no].form = ITEM_FM_INT;		break;	// �^
			case FM_STR:		cd.item_list[item_no].form = ITEM_FM_STR;		break;	// �^
			case FM_INTLIST:	cd.item_list[item_no].form = ITEM_FM_INTLIST;	break;	// �^
			case FM_STRLIST:	cd.item_list[item_no].form = ITEM_FM_STRLIST;	break;	// �^
		}
		cd.item_list[item_no].size = 0;										// �T�C�Y
		cd.item_list[item_no].name = Gp_lexer->get_user_inc_prop_name(i);	// ���O

		switch (cd.item_list[item_no].form)	{
			case ITEM_FM_INT:
				cd.item_list[item_no].value = tostr(p_prop->Int);
				break;
			case ITEM_FM_STR:
				cd.item_list[item_no].value = p_prop->str;
				break;
			case ITEM_FM_INTLIST:
			{
				C_elm_int_list* p_int_list = boost::any_cast< BSP<C_elm_int_list> >(p_prop->any).get();
				for (int j = 0; j < (int)p_int_list->get_size(); j++)	{
					item_no ++;
					cd.item_list.resize(item_no + 1);
					cd.item_list[item_no].scn_no = -1;	// �V�[���ԍ��i-1 �� inc�j
					cd.item_list[item_no].prop_id = i;	// �v���p�e�B�h�c
					cd.item_list[item_no].index = j;	// �C���f�b�N�X
					cd.item_list[item_no].form = ITEM_FM_INTLIST_SUB;
					cd.item_list[item_no].value = tostr(p_int_list->get_value(j));
					cd.item_list[item_no].name = p_int_list->get_element_name() + _T("[") + tostr(j) + _T("]");	// ���O
				}
				break;
			}
			case ITEM_FM_STRLIST:
			{
				C_elm_str_list* p_str_list = boost::any_cast< BSP<C_elm_str_list> >(p_prop->any).get();
				for (int j = 0; j < (int)p_str_list->get_size(); j++)	{
					item_no ++;
					cd.item_list.resize(item_no + 1);
					cd.item_list[item_no].scn_no = -1;	// �V�[���ԍ��i-1 �� inc�j
					cd.item_list[item_no].prop_id = i;	// �v���p�e�B�h�c
					cd.item_list[item_no].index = j;	// �C���f�b�N�X
					cd.item_list[item_no].form = ITEM_FM_STRLIST_SUB;
					cd.item_list[item_no].value = p_str_list->get_value(j);
					cd.item_list[item_no].name = p_str_list->get_element_name() + _T("[") + tostr(j) + _T("]");	// ���O
				}
				break;
			}
		}
	}

	// �V�[���v���p�e�B
	int scn_no = Gp_lexer->get_cur_scn_no();
	for (int i = 0; i < (int)(*Gp_user_scn_prop_list)[scn_no].prop_list.get_size(); i++)	{
		C_tnm_prop* p_prop = &(*Gp_user_scn_prop_list)[scn_no].prop_list[i];
		item_no ++;

		cd.item_list.resize(item_no + 1);
		cd.item_list[item_no].scn_no = scn_no;	// �V�[���ԍ�
		cd.item_list[item_no].prop_id = i;		// �v���p�e�B�h�c
		switch (Gp_lexer->get_user_scn_prop_form(scn_no, i))	{
			case FM_INT:		cd.item_list[item_no].form = ITEM_FM_INT;		break;	// �^
			case FM_STR:		cd.item_list[item_no].form = ITEM_FM_STR;		break;	// �^
			case FM_INTLIST:	cd.item_list[item_no].form = ITEM_FM_INTLIST;	break;	// �^
			case FM_STRLIST:	cd.item_list[item_no].form = ITEM_FM_STRLIST;	break;	// �^
		}
		cd.item_list[item_no].size = 0;													// �T�C�Y
		cd.item_list[item_no].name = Gp_lexer->get_user_scn_prop_name(scn_no, i);		// ���O

		// �^�ʂ̃��X�g�����擾
		switch (cd.item_list[item_no].form)	{
			case ITEM_FM_INT:
				cd.item_list[item_no].value = tostr(p_prop->Int);
				break;
			case ITEM_FM_STR:
				cd.item_list[item_no].value = p_prop->str;
				break;
			case ITEM_FM_INTLIST:
			{
				cd.item_list[item_no].value = _T("");
				C_elm_int_list* p_int_list = boost::any_cast< BSP<C_elm_int_list> >(p_prop->any).get();
				for (int j = 0; j < (int)p_int_list->get_size(); j++)	{
					item_no ++;
					cd.item_list.resize(item_no + 1);
					cd.item_list[item_no].scn_no = scn_no;	// �V�[���ԍ�
					cd.item_list[item_no].prop_id = i;		// �v���p�e�B�h�c
					cd.item_list[item_no].index = j;		// �C���f�b�N�X
					cd.item_list[item_no].form = ITEM_FM_INTLIST_SUB;
					cd.item_list[item_no].value = tostr(p_int_list->get_value(j));
					cd.item_list[item_no].name = p_int_list->get_element_name() + _T("[") + tostr(j) + _T("]");	// ���O
				}
				break;
			}
			case ITEM_FM_STRLIST:
			{
				cd.item_list[item_no].value = _T("");
				C_elm_str_list* p_str_list = boost::any_cast< BSP<C_elm_str_list> >(p_prop->any).get();
				for (int j = 0; j < (int)p_str_list->get_size(); j++)	{
					item_no ++;
					cd.item_list.resize(item_no + 1);
					cd.item_list[item_no].scn_no = scn_no;	// �V�[���ԍ��i-2 �� ��\���j
					cd.item_list[item_no].prop_id = i;		// �v���p�e�B�h�c
					cd.item_list[item_no].index = j;		// �C���f�b�N�X
					cd.item_list[item_no].form = ITEM_FM_STRLIST_SUB;
					cd.item_list[item_no].value = p_str_list->get_value(j);
					cd.item_list[item_no].name = p_str_list->get_element_name() + _T("[") + tostr(j) + _T("]");	// ���O
				}
				break;
			}
		}
	}

	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// �A�C�e��
	for (int i = 0; i < (int)lv.get_item_cnt(); i++)	{
		bool change = frame_first_flag || (i >= (int)m_ld.item_list.size());

		// �V�[���ԍ��A�v���p�e�B�h�c
		if (change || cd.item_list[i].scn_no != m_ld.item_list[i].scn_no || cd.item_list[i].prop_id != m_ld.item_list[i].prop_id)	{
			if (cd.item_list[i].form == ITEM_FM_INTLIST_SUB || cd.item_list[i].form == ITEM_FM_STRLIST_SUB)
				lv.set_cell_text(i, 0, _T(""));
			else if (cd.item_list[i].scn_no == -1)	// inc
				lv.set_cell_text(i, 0, _T("(inc)"));
			else
				lv.set_cell_text(i, 0, Gp_lexer->get_scn_name(cd.item_list[i].scn_no));
		}

		// �^
		if (change || cd.item_list[i].form != m_ld.item_list[i].form)	{
			switch (cd.item_list[i].form)	{
				case ITEM_FM_INT:		lv.set_cell_text(i, 1, _T("int"));		break;
				case ITEM_FM_STR:		lv.set_cell_text(i, 1, _T("str"));		break;
				case ITEM_FM_INTLIST:	lv.set_cell_text(i, 1, _T("intlist"));	break;
				case ITEM_FM_STRLIST:	lv.set_cell_text(i, 1, _T("strlist"));	break;
				default:				lv.set_cell_text(i, 1, _T(""));			break;
			}
		}

		// ���O
		if (change || cd.item_list[i].name != m_ld.item_list[i].name)
			lv.set_cell_text(i, 2, cd.item_list[i].name);

		// �l
		if (change || cd.item_list[i].value != m_ld.item_list[i].value)	{
			lv.set_cell_text(i, 3, cd.item_list[i].value);
		}
	}

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}

// ****************************************************************
// ���X�g�r���[���_�u���N���b�N����
// ================================================================
void C_db_wnd_info_user_flag::on_dblclk_lv()
{
	// �I����Ԏ擾
	int select_cnt = lv.get_selected_item_cnt();
	int select_no = lv.get_selected_item_no();

	// �}�E�X�J�[�\���̏���Ă���Z���ԍ�
	int item_no, column_no;
	C_point mp = lv.screen_to_client(get_mouse_pos());
	lv.get_cell_by_point(mp.x, mp.y, &item_no, &column_no);

	// �A�C�e����ύX����
	if (select_cnt == 1 && select_no == item_no && column_no == 3)	{
		if (0 <= item_no && item_no < (int)m_ld.item_list.size())	{
			int form = m_ld.item_list[item_no].form;

			// �^�ɂ���ĕ���
			if (form == ITEM_FM_INT || form == ITEM_FM_INTLIST_SUB)	{
				int value = str_to_int(lv.get_cell_text(item_no, column_no), 0);
				lv.edit_item_text_edit_for_num(item_no, column_no, INT_MIN, INT_MAX, value);
			}
			else if (form == ITEM_FM_STR || form == ITEM_FM_STRLIST_SUB)	{
				TSTR value = lv.get_cell_text(item_no, column_no);
				lv.edit_item_text_edit_with_str(item_no, column_no, value);
			}
		}
	}
}

// ****************************************************************
// ���X�g�r���[�̒l��ύX����
// ================================================================
void C_db_wnd_info_user_flag::on_change_lv_value(int item_no, int column_no, CTSTR& value)
{
	if (column_no == 3)	{
		if (0 <= item_no && item_no < (int)m_ld.item_list.size())	{
			int scn_no = m_ld.item_list[item_no].scn_no;
			int form = m_ld.item_list[item_no].form;
			int index = m_ld.item_list[item_no].index;
			C_tnm_prop* p_prop = NULL;

			// �C���N�t���O
			if (scn_no == -1)	{

				// �^�ɂ���ĕ���
				if (form == ITEM_FM_INT)	{
					int user_prop_id = m_ld.item_list[item_no].prop_id;
					p_prop = &Gp_user_inc_prop->prop_list[user_prop_id];
					p_prop->Int = str_to_int(value, 0);
				}
				else if (form == ITEM_FM_STR)	{
					int user_prop_id = m_ld.item_list[item_no].prop_id;
					p_prop = &Gp_user_inc_prop->prop_list[user_prop_id];
					p_prop->str = value;
				}
				else if (form == ITEM_FM_INTLIST_SUB)	{
					int user_prop_id = m_ld.item_list[item_no].prop_id;
					p_prop = &Gp_user_inc_prop->prop_list[user_prop_id];
					C_elm_int_list* p_int_list = boost::any_cast< BSP<C_elm_int_list> >(p_prop->any).get();
					if (p_int_list)	{
						if (0 <= index && index < p_int_list->get_size())	{
							p_int_list->set_value(index, str_to_int(value, 0));
						}
					}
				}
				else if (form == ITEM_FM_STRLIST_SUB)	{
					int user_prop_id = m_ld.item_list[item_no].prop_id;
					p_prop = &Gp_user_inc_prop->prop_list[user_prop_id];
					C_elm_str_list* p_str_list = boost::any_cast< BSP<C_elm_str_list> >(p_prop->any).get();
					if (p_str_list)	{
						if (0 <= index && index < p_str_list->get_size())	{
							p_str_list->set_value(index, value);
						}
					}
				}
			}

			// �V�[���t���O
			else	{

				// �^�ɂ���ĕ���
				if (form == ITEM_FM_INT)	{
					int user_prop_id = m_ld.item_list[item_no].prop_id;
					p_prop = &(*Gp_user_scn_prop_list)[scn_no].prop_list[user_prop_id];
					p_prop->Int = str_to_int(value, 0);
				}
				else if (form == ITEM_FM_STR)	{
					int user_prop_id = m_ld.item_list[item_no].prop_id;
					p_prop = &(*Gp_user_scn_prop_list)[scn_no].prop_list[user_prop_id];
					p_prop->str = value;
				}
				else if (form == ITEM_FM_INTLIST_SUB)	{
					int user_prop_id = m_ld.item_list[item_no].prop_id;
					p_prop = &(*Gp_user_scn_prop_list)[scn_no].prop_list[user_prop_id];
					C_elm_int_list* p_int_list = boost::any_cast< BSP<C_elm_int_list> >(p_prop->any).get();
					if (p_int_list)	{
						if (0 <= index && index < p_int_list->get_size())	{
							p_int_list->set_value(index, str_to_int(value, 0));
						}
					}
				}
				else if (form == ITEM_FM_STRLIST_SUB)	{
					int user_prop_id = m_ld.item_list[item_no].prop_id;
					p_prop = &(*Gp_user_scn_prop_list)[scn_no].prop_list[user_prop_id];
					C_elm_str_list* p_str_list = boost::any_cast< BSP<C_elm_str_list> >(p_prop->any).get();
					if (p_str_list)	{
						if (0 <= index && index < p_str_list->get_size())	{
							p_str_list->set_value(index, value);
						}
					}
				}
			}
		}
	}
}

