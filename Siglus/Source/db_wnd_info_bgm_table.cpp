#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"engine/eng_bgm_table.h"
#include	"dialog/db_wnd_info_bgm_table.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�a�f�l�e�[�u��
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_bgm_table")

// ������
void C_db_wnd_info_bgm_table::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�a�f�l�e�[�u��"));

	// ���X�g�r���[����
	listview_add_column(_T("name"), _T("�o�^��"), 100);
	listview_add_column(_T("flag"), _T("�׸�"), 50);
}

// �������t�@���N�V����
void C_db_wnd_info_bgm_table::on_init_func()
{
}

// ����t�@���N�V����
void C_db_wnd_info_bgm_table::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_bgm_table::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// ���݂̃f�[�^�����
	int flag_cnt = tnm_bgm_table_get_bgm_cnt();
	cd.flag_list.resize(flag_cnt);
	for (int i = 0; i < flag_cnt; i++)	{
		cd.flag_list[i] = tnm_bgm_table_get_listened(i) ? 1 : 0;
	}

	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (frame_first_flag || cd.flag_list.size() != m_ld.flag_list.size())
		lv.set_item_cnt((int)cd.flag_list.size());

	// ���������Ɋւ��ēo�^����\��
	for (int i = frame_first_flag ? 0 : (int)m_ld.flag_list.size(); i < (int)cd.flag_list.size(); i++)
		lv.set_cell_text(i, 0, Gp_ini->bgm[i].regist_name);

	// �������������͕ύX���ꂽ���Ɋւ��Ēl��\��
	for (int i = 0; i < (int)cd.flag_list.size(); i++)	{
		int column_no = 0;

		// �J�E���^
		column_no ++;
		if (frame_first_flag || i >= (int)m_ld.flag_list.size() || cd.flag_list[i] != m_ld.flag_list[i])
			lv.set_cell_text(i, column_no, cd.flag_list[i] ? _T("��") : _T("�~"));
	}

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}

