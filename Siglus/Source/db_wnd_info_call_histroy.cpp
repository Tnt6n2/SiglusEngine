#include	"pch.h"
#include	"tnm_form_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_lexer.h"
#include	"element/elm_call.h"
#include	"dialog/db_wnd_info_call_histroy.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�Ăяo������
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_call_histroy")

// ������
void C_db_wnd_info_call_histroy::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�Ăяo������"));

	// ���X�g�r���[����
	listview_add_column(_T("call_type"), _T("����"), 100);
	listview_add_column(_T("src_scn"), _T("�Ăяo����"), 100);
	listview_add_column(_T("dst_scn"), _T("�Ăяo����"), 100);
}

// �������t�@���N�V����
void C_db_wnd_info_call_histroy::on_init_func()
{
}

// ����t�@���N�V����
void C_db_wnd_info_call_histroy::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_call_histroy::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// ���݂̃f�[�^�����
	cd.item_list.resize(Gp_call_list->get_call_cnt() - 1);
	for (int i = 1; i < Gp_call_list->get_call_cnt(); i++)	{		// 0 �Ԃ̓X�^�[�g�V�[���Ȃ̂ŕ\�����Ȃ�
		cd.item_list[i - 1].call_type = (*Gp_call_list)[i].call_type;
		cd.item_list[i - 1].excall_flag = (*Gp_call_list)[i].excall_flag;
		cd.item_list[i - 1].frame_action_flag = (*Gp_call_list)[i].frame_action_flag;
		cd.item_list[i - 1].src_scn = (*Gp_call_list)[i].src_scn_no;
		cd.item_list[i - 1].dst_scn = (*Gp_call_list)[i].dst_scn_no;
		cd.item_list[i - 1].dst_label = (*Gp_call_list)[i].dst_label_no;
	}

	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// �������������͕ύX���ꂽ���Ɋւ��Ēl��\��
	for (int i = 0; i < (int)cd.item_list.size(); i++)	{
		int col_no = -1;
		bool change = frame_first_flag || i >= (int)m_ld.item_list.size();

		// �^�C�v
		col_no ++;
		if (change || cd.item_list[i].call_type != m_ld.item_list[i].call_type || cd.item_list[i].excall_flag != m_ld.item_list[i].excall_flag)	{
			TSTR str;
			switch (cd.item_list[i].call_type)	{
				case E_tnm_call_type_gosub:		str += _T("gosub");		break;
				case E_tnm_call_type_farcall:	str += _T("farcall");	break;
				case E_tnm_call_type_user_cmd:	str += _T("հ�޺����");	break;
			}
			if (cd.item_list[i].excall_flag)	{
				str += _T("(excall)");
			}
			if (cd.item_list[i].frame_action_flag)	{
				str += _T("(�ڰѱ����)");
			}
			lv.set_cell_text(i, col_no, str);
		}

		// �Ăяo����
		col_no ++;
		if (change || cd.item_list[i].src_scn != m_ld.item_list[i].src_scn)	{
			TSTR str;
			int scn_no = cd.item_list[i].src_scn;
			if (0 <= scn_no && scn_no < Gp_lexer->get_scn_cnt())	{
				TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
				str += scn_name;
			}
			lv.set_cell_text(i, col_no, str);
		}

		// �Ăяo����
		col_no ++;
		if (change || cd.item_list[i].dst_scn != m_ld.item_list[i].dst_scn)	{
			TSTR str;
			int scn_no = cd.item_list[i].dst_scn;
			if (0 <= scn_no && scn_no < Gp_lexer->get_scn_cnt())	{
				TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
				str += scn_name;
			}
			switch (cd.item_list[i].call_type)	{
				case E_tnm_call_type_gosub:
					str += str_format(_T("(����%d)"), cd.item_list[i].dst_label);
					break;
				case E_tnm_call_type_farcall:
					str += str_format(_T("(#z%03d)"), cd.item_list[i].dst_label);
					break;
				case E_tnm_call_type_user_cmd:
				{
					TSTR cmd_name = Gp_lexer->get_user_cmd_name(cd.item_list[i].dst_scn, cd.item_list[i].dst_label);
					str += _T("(") + cmd_name + _T(")");
					break;
				}
			}
			lv.set_cell_text(i, col_no, str);
		}
	}
	/*
	

	// ���݂̃f�[�^�����
	cd.item_list.resize(Gp_stack->int_now_cnt);
	for (int i = 0; i < (int)Gp_stack->int_now_cnt; i++)	{
		cd.item_list[i].id = i;
		cd.item_list[i].form = FM_INT;
		cd.item_list[i].Int = *(Gp_stack->int_p + i);
	}

	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// �������������͕ύX���ꂽ���Ɋւ��Ēl��\��
	for (int i = 0; i < (int)cd.item_list.size(); i++)	{
		int col_no = -1;
		bool change = frame_first_flag || i >= (int)m_ld.item_list.size();

		// ID
		col_no ++;
		if (change || cd.item_list[i].id != m_ld.item_list[i].id)
			lv.set_cell_text(i, col_no, tostr(cd.item_list[i].id));

		// �^
		col_no ++;
		if (change || cd.item_list[i].form != m_ld.item_list[i].form)	{
			switch (cd.item_list[i].form)	{
				case FM_VOID:	lv.set_cell_text(i, col_no, _T("FM_VOID"));	break;
				case FM_INT:	lv.set_cell_text(i, col_no, _T("FM_INT"));	break;
				case FM_STR:	lv.set_cell_text(i, col_no, _T("FM_STR"));	break;
				default:		lv.set_cell_text(i, col_no, _T("???"));		break;
			}
		}

		// �l
		col_no ++;
		if (change || cd.item_list[i].Int != m_ld.item_list[i].Int)
			lv.set_cell_text(i, col_no, tostr(cd.item_list[i].Int));
	}
	*/

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}

