#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_form_code.h"
#include	"engine/ifc_stack.h"
#include	"dialog/db_wnd_info_stack.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�X�^�b�N
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_stack")

// ������
void C_db_wnd_info_stack::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�X�^�b�N���"));

	// ���X�g�r���[����
	listview_add_column(_T("id"), _T("ID"), 50);
	listview_add_column(_T("form"), _T("�^"), 50);
	listview_add_column(_T("value"), _T("�l"), 100);
}

// �������t�@���N�V����
void C_db_wnd_info_stack::on_init_func()
{
}

// ����t�@���N�V����
void C_db_wnd_info_stack::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_stack::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

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

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}

