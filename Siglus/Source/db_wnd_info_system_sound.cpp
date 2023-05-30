#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_config_data.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_sound.h"
#include	"dialog/db_wnd_info_system_sound.h"


// ****************************************************************
// �f�o�b�O���E�B���h�E�F�V�X�e���F�T�E���h
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_system_sound")

// ������
void C_db_wnd_info_system_sound::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�{�����[���`�����l��"));

	// ���X�g�r���[����
	listview_add_column(_T("type"), _T("����"), 100);
	listview_add_column(_T("volume"), _T("��ح��"), 100);
	listview_add_column(_T("check"), _T("����"), 50);
}

// �������t�@���N�V����
void C_db_wnd_info_system_sound::on_init_func()
{
}

// ����t�@���N�V����
void C_db_wnd_info_system_sound::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_system_sound::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// �T�C�Y�͌Œ�
	cd.item_list.resize(22);

	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// �A�C�e������ݒ肷��
	if (frame_first_flag)	{
		lv.set_cell_text(0, 0, _T("-1�i�S�́j"));
		lv.set_cell_text(1, 0, _T("0�iBGM�j"));
		lv.set_cell_text(2, 0, _T("1�iKOE�j"));
		lv.set_cell_text(3, 0, _T("2�iPCM�j"));
		lv.set_cell_text(4, 0, _T("3�iSE�j"));
		lv.set_cell_text(5, 0, _T("4�iMOV�j"));
		lv.set_cell_text(6, 0, _T("16�i�ėp�j"));
		lv.set_cell_text(7, 0, _T("17�i�ėp�j"));
		lv.set_cell_text(8, 0, _T("18�i�ėp�j"));
		lv.set_cell_text(9, 0, _T("19�i�ėp�j"));
		lv.set_cell_text(10, 0, _T("20�i�ėp�j"));
		lv.set_cell_text(11, 0, _T("21�i�ėp�j"));
		lv.set_cell_text(12, 0, _T("22�i�ėp�j"));
		lv.set_cell_text(13, 0, _T("23�i�ėp�j"));
		lv.set_cell_text(14, 0, _T("24�i�ėp�j"));
		lv.set_cell_text(15, 0, _T("25�i�ėp�j"));
		lv.set_cell_text(16, 0, _T("26�i�ėp�j"));
		lv.set_cell_text(17, 0, _T("27�i�ėp�j"));
		lv.set_cell_text(18, 0, _T("28�i�ėp�j"));
		lv.set_cell_text(19, 0, _T("29�i�ėp�j"));
		lv.set_cell_text(20, 0, _T("30�i�ėp�j"));
		lv.set_cell_text(21, 0, _T("31�i�ėp�j"));
	}

	// ���݂̃f�[�^�����
	for (int i = 0; i < 22; i++)	{
		if (i == 0)	{
			cd.item_list[0].volume = Gp_config->all_sound_user_volume;
			cd.item_list[0].check = Gp_config->play_all_sound_check;
		}
		else if (1 <= i && i <= 5)	{
			cd.item_list[i].volume = Gp_config->sound_user_volume[i - 1];
			cd.item_list[i].check = Gp_config->play_sound_check[i - 1];
		}
		else if (6 <= i && i <= 21)	{
			cd.item_list[i].volume = Gp_config->sound_user_volume[i + 10];
			cd.item_list[i].check = Gp_config->play_sound_check[i + 10];
		}
	}

	// �������������͕ύX���ꂽ���Ɋւ��Ēl��\��
	for (int i = 0; i < (int)cd.item_list.size(); i++)	{
		bool change = frame_first_flag || i >= (int)m_ld.item_list.size();

		// �{�����[��
		if (change || cd.item_list[i].volume != m_ld.item_list[i].volume)	{
			lv.set_cell_text(i, 1, tostr(cd.item_list[i].volume));
		}
		// �Đ�
		if (change || cd.item_list[i].check != m_ld.item_list[i].check)	{
			lv.set_cell_text(i, 2, cd.item_list[i].check ? _T("�I��") : _T("�I�t"));
		}
	}

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}
