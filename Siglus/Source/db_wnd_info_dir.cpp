#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dir.h"
#include	"engine/eng_dir.h"
#include	"dialog/db_wnd_info_dir.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�V�X�e��
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_dir")

// ������
void C_db_wnd_info_dir::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�f�B���N�g�����"));

	// ���X�g�r���[����
	listview_add_column(_T("state"), _T("�X�e�[�g"), 100);
	listview_add_column(_T("value"), _T("�l"), 100);
}

// �������t�@���N�V����
void C_db_wnd_info_dir::on_init_func()
{
	// ���ڂ�ǉ�
	lv.add_item(0, _T("�Z�b�g�A�b�v"));
	lv.add_item(0, _T("���s�t�H���_"));
	lv.add_item(0, _T("�f�[�^�t�H���_"));
	lv.add_item(0, _T("��ƃt�H���_"));
	lv.add_item(0, _T("�Z�[�u�t�H���_"));
}

// ����t�@���N�V����
void C_db_wnd_info_dir::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_dir::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// ���݂̒l���擾����
	cd.setup_type = Gp_global->setup_type;
	cd.exe_dir = Gp_dir->exe_dir;
	cd.base_dir = Gp_dir->base_dir;
	cd.work_dir = Gp_dir->work_dir;
	cd.save_dir = Gp_dir->save_dir;

	// ���ڂ����ɐݒ肷��
	int item_no = -1;

	// �Z�b�g�A�b�v�^�C�v
	item_no ++;
	if (frame_first_flag || cd.setup_type != m_ld.setup_type)	{
		switch (cd.setup_type)	{
			case TNM_SETUP_TYPE_FULL:	lv.set_cell_text(item_no, 1, _T("���S�Z�b�g�A�b�v"));			break;
			case TNM_SETUP_TYPE_MIN:	lv.set_cell_text(item_no, 1, _T("�ŏ��Z�b�g�A�b�v"));			break;
			default:					lv.set_cell_text(item_no, 1, _T("�Z�b�g�A�b�v����Ă��܂���"));	break;
		}
	}

	item_no ++;
	if (frame_first_flag || cd.exe_dir != m_ld.exe_dir)
		lv.set_cell_text(item_no, 1, cd.exe_dir);
	item_no ++;
	if (frame_first_flag || cd.base_dir != m_ld.base_dir)
		lv.set_cell_text(item_no, 1, cd.base_dir);
	item_no ++;
	if (frame_first_flag || cd.work_dir != m_ld.work_dir)
		lv.set_cell_text(item_no, 1, cd.work_dir);
	item_no ++;
	if (frame_first_flag || cd.save_dir != m_ld.save_dir)
		lv.set_cell_text(item_no, 1, cd.save_dir);

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}
