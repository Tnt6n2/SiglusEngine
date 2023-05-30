#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_form_code.h"
#include	"data/tnm_local_data.h"
#include	"dialog/db_wnd_info_proc_stack.h"


#define PROC(def, id, key)	_T(#def),

const TCHAR* G_proc_name[] = 
{
#include	"data/tnm_proc_def_00.h"
};

#undef PROC


// ****************************************************************
// �f�o�b�O���E�B���h�E�F�v���Z�X�X�^�b�N
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_proc_stack")

// ������
void C_db_wnd_info_proc_stack::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�v���Z�X�X�^�b�N���"));

	// ���X�g�r���[����
	listview_add_column(_T("proc"), _T("�v���Z�X"), 200);
}

// �������t�@���N�V����
void C_db_wnd_info_proc_stack::on_init_func()
{
}

// ����t�@���N�V����
void C_db_wnd_info_proc_stack::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_proc_stack::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// ���݂̃f�[�^�����
	cd.item_list.resize(Gp_local->proc_stack.size() + 1);
	for (int i = 0; i < (int)Gp_local->proc_stack.size(); i++)	{
		cd.item_list[i].proc = Gp_local->proc_stack[i].type;
	}
	cd.item_list[Gp_local->proc_stack.size()].proc = Gp_local->cur_proc.type;

	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// �������������͕ύX���ꂽ���Ɋւ��Ēl��\��
	for (int i = 0; i < (int)cd.item_list.size(); i++)	{
		int col_no = -1;
		bool change = frame_first_flag || i >= (int)m_ld.item_list.size();

		// �v���Z�X
		col_no ++;
		TSTR proc_name;
		if (change || cd.item_list[i].proc != m_ld.item_list[i].proc)	{
			if (0 <= cd.item_list[i].proc && cd.item_list[i].proc < sizeof_array(G_proc_name))	{
				proc_name = G_proc_name[cd.item_list[i].proc];
				lv.set_cell_text(i, col_no, proc_name);
			}
		}
	}

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}

