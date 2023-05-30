#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_sound.h"
#include	"dialog/db_wnd_info_sound.h"


// ****************************************************************
// �f�o�b�O���E�B���h�E�F�T�E���h
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_sound")

// ������
void C_db_wnd_info_sound::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�T�E���h���"));

	// ���X�g�r���[����
	listview_add_column(_T("type"), _T("����"), 100);
	listview_add_column(_T("id"), _T("ID"), 50);
	listview_add_column(_T("file"), _T("̧��"), 100);
	listview_add_column(_T("volume"), _T("��ح��"), 50);
	listview_add_column(_T("ready"), _T("����"), 50);
	listview_add_column(_T("delay"), _T("�x��"), 50);
	listview_add_column(_T("state"), _T("���"), 50);
	listview_add_column(_T("state2"), _T("���2"), 50);
	listview_add_column(_T("option"), _T("��߼��"), 50);
	listview_add_column(_T("bgmfade"), _T("bgmfade"), 50);
	listview_add_column(_T("bgmfade2"), _T("bgmfade2"), 50);
}

// �������t�@���N�V����
void C_db_wnd_info_sound::on_init_func()
{
}

// ����t�@���N�V����
void C_db_wnd_info_sound::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_sound::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// ���݂̃f�[�^�����
	cd.item_list.resize(
		Gp_sound->m_bgm.get_player_cnt()
		+ Gp_sound->m_koe.get_player_cnt()
		+ Gp_sound->m_pcm.get_player_cnt()
		+ (int)Gp_sound->m_pcmch_list.get_size()
		+ Gp_sound->m_se.get_player_cnt()
		+ 1);

	int item_no = 0;

	// BGM
	for (int i = 0; i < (int)Gp_sound->m_bgm.get_player_cnt(); i++, item_no++)	{
		cd.item_list[item_no].type = 0;
		cd.item_list[item_no].player_no = i;
		cd.item_list[item_no].file_name = tnm_get_regist_name_by_bgm_no(Gp_sound->m_bgm.get_player(i)->get_bgm_no());
		cd.item_list[item_no].volume = Gp_sound->m_bgm.get_volume();
		cd.item_list[item_no].pause_flag = Gp_sound->m_bgm.is_pause();
		cd.item_list[item_no].delay_time = Gp_sound->m_bgm.get_delay_time();
		cd.item_list[item_no].state = Gp_sound->m_bgm.get_player(i)->get_play_state();
		cd.item_list[item_no].state2 = 0;
		cd.item_list[item_no].option = 0;
		cd.item_list[item_no].bgmfade_target = false;
		cd.item_list[item_no].bgmfade2_target = false;

		if (cd.item_list[item_no].state == TNM_PLAYER_STATE_PLAY)	{
			if (false);
			else if (Gp_sound->m_bgm.is_loop())			cd.item_list[item_no].option = 1;
		}
	}

	// KOE
	for (int i = 0; i < (int)Gp_sound->m_koe.get_player_cnt(); i++, item_no++)	{
		cd.item_list[item_no].type = 1;
		cd.item_list[item_no].player_no = i;
		int koe_no = Gp_sound->m_koe.get_player(i)->get_koe_no();
		cd.item_list[item_no].file_name = (koe_no == -1) ? _T("") : str_format(_T("%04d %05d"), koe_no / 10000, koe_no % 10000);
		cd.item_list[item_no].volume = Gp_sound->m_koe.get_volume();
		cd.item_list[item_no].pause_flag = false;
		cd.item_list[item_no].delay_time = 0;
		cd.item_list[item_no].state = Gp_sound->m_koe.get_player(i)->get_play_state_2();
		cd.item_list[item_no].state2 = 0;
		cd.item_list[item_no].option = 0;
		cd.item_list[item_no].bgmfade_target = false;
		cd.item_list[item_no].bgmfade2_target = false;
	}

	// PCM
	for (int i = 0; i < (int)Gp_sound->m_pcm.get_player_cnt(); i++, item_no++)	{
		cd.item_list[item_no].type = 2;
		cd.item_list[item_no].player_no = i;
		cd.item_list[item_no].file_name = Gp_sound->m_pcm.get_player(i)->get_file_name();
		cd.item_list[item_no].volume = Gp_sound->m_pcm.get_volume();
		cd.item_list[item_no].pause_flag = false;
		cd.item_list[item_no].delay_time = 0;
		cd.item_list[item_no].state = Gp_sound->m_pcm.get_player(i)->get_play_state_2();
		cd.item_list[item_no].state2 = 0;
		cd.item_list[item_no].option = 0;
		cd.item_list[item_no].bgmfade_target = false;
		cd.item_list[item_no].bgmfade2_target = false;
	}

	// PCMCH
	int pcmch_cnt = Gp_sound->m_pcmch_list.get_size();
	for (int i = 0; i < pcmch_cnt; i++, item_no++)	{
		C_elm_pcmch* p_pcmch = &Gp_sound->m_pcmch_list[i];

		cd.item_list[item_no].type = 3;
		cd.item_list[item_no].player_no = i;
		if (!p_pcmch->get_pcm_name().empty())	{
			cd.item_list[item_no].file_name = p_pcmch->get_pcm_name();
		}
		else if (!p_pcmch->get_bgm_name().empty())	{
			cd.item_list[item_no].file_name = _T("bgm; ") + p_pcmch->get_bgm_name();
		}
		else if (p_pcmch->get_koe_no() >= 0)	{
			if (p_pcmch->get_chara_no() >= 0)	{
				cd.item_list[item_no].file_name = str_format(_T("KOE(%09d,%03d)"), p_pcmch->get_koe_no(), p_pcmch->get_chara_no());
			}
			else	{
				cd.item_list[item_no].file_name = str_format(_T("KOE(%09d)"), p_pcmch->get_koe_no());
			}
		}
		else if (p_pcmch->get_se_no() >= 0)	{
			cd.item_list[item_no].file_name = _T("se; ") + tostr(p_pcmch->get_se_no()) + _T("��");
		}
		cd.item_list[item_no].volume = p_pcmch->get_volume();
		cd.item_list[item_no].pause_flag = false;
		cd.item_list[item_no].delay_time = 0;
		cd.item_list[item_no].state = p_pcmch->get_play_state_2();
		cd.item_list[item_no].state2 = 0;
		cd.item_list[item_no].option = 0;
		cd.item_list[item_no].bgmfade_target = p_pcmch->is_bgm_fade_target();
		cd.item_list[item_no].bgmfade2_target = p_pcmch->is_bgm_fade2_target();

		if (cd.item_list[item_no].state == TNM_PLAYER_STATE_PLAY || cd.item_list[item_no].state == TNM_PLAYER_STATE_PAUSE)	{
			if (false);
			else if (p_pcmch->is_loop())	cd.item_list[item_no].option = 1;
		}
	}

	// SE
	for (int i = 0; i < (int)Gp_sound->m_se.get_player_cnt(); i++, item_no++)	{
		int sound_type = Gp_sound->m_se.get_player(i)->get_sound_type();
		int koe_no = Gp_sound->m_se.get_player(i)->get_koe_no();
		int se_no = Gp_sound->m_se.get_player(i)->get_se_no();
		cd.item_list[item_no].type = 4;
		cd.item_list[item_no].player_no = i;
		switch (sound_type)	{
			case TNM_SOUND_TYPE_BGM:	cd.item_list[item_no].file_name = tnm_get_regist_name_by_bgm_no(Gp_sound->m_se.get_player(i)->get_bgm_no());			break;
			case TNM_SOUND_TYPE_KOE:	cd.item_list[item_no].file_name = (koe_no == -1) ? _T("") : str_format(_T("%04d %05d"), koe_no / 10000, koe_no % 10000);	break;
			case TNM_SOUND_TYPE_PCM:	cd.item_list[item_no].file_name = Gp_sound->m_se.get_player(i)->get_file_name();										break;
			case TNM_SOUND_TYPE_SE:		cd.item_list[item_no].file_name = (0 <= se_no && se_no < Gp_ini->se_cnt) ? Gp_ini->se[se_no].file_name : _T("");			break;
		}
		cd.item_list[item_no].volume = Gp_sound->m_se.get_volume();
		cd.item_list[item_no].pause_flag = false;
		cd.item_list[item_no].delay_time = 0;
		cd.item_list[item_no].state = Gp_sound->m_se.get_player(i)->get_play_state();
		cd.item_list[item_no].state2 = 0;
		cd.item_list[item_no].option = 0;
		cd.item_list[item_no].bgmfade_target = false;
		cd.item_list[item_no].bgmfade2_target = false;
	}

	// MOV
	cd.item_list[item_no].type = 5;
	cd.item_list[item_no].player_no = 0;
	cd.item_list[item_no].file_name = Gp_sound->m_mov.get_file_name();
	cd.item_list[item_no].volume = 255;
	cd.item_list[item_no].pause_flag = false;
	cd.item_list[item_no].delay_time = 0;
	int mov_play_state = Gp_sound->m_mov.get_state();
	switch (mov_play_state)	{
		case TNM_MOVIE_PLAY_STATE_PLAY:		cd.item_list[item_no].state = TNM_PLAYER_STATE_PLAY;	break;
		default:							cd.item_list[item_no].state = TNM_PLAYER_STATE_FREE;	break;
	}
	cd.item_list[item_no].state2 = Gp_sound->m_mov.get_wmp_state_detail();
	cd.item_list[item_no].option = 0;
	cd.item_list[item_no].bgmfade_target = false;
	cd.item_list[item_no].bgmfade2_target = false;

	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// �������������͕ύX���ꂽ���Ɋւ��Ēl��\��
	for (int i = 0; i < (int)cd.item_list.size(); i++)	{
		bool change = frame_first_flag || i >= (int)m_ld.item_list.size();

		// �^�C�v
		if (change || cd.item_list[i].type != m_ld.item_list[i].type)	{
			TSTR str;
			switch (cd.item_list[i].type)	{
				case 0:		str = _T("BGM");		break;
				case 1:		str = _T("KOE");		break;
				case 2:		str = _T("�ėpPCM");	break;
				case 3:		str = _T("��pPCM");	break;
				case 4:		str = _T("SE");			break;
				case 5:		str = _T("MOV");		break;
			}
			lv.set_cell_text(i, 0, str);
		}

		// ID
		if (change || cd.item_list[i].player_no != m_ld.item_list[i].player_no)	{
			lv.set_cell_text(i, 1, tostr(cd.item_list[i].player_no));
		}

		// �t�@�C����
		if (change || cd.item_list[i].file_name != m_ld.item_list[i].file_name)	{
			lv.set_cell_text(i, 2, cd.item_list[i].file_name);
		}

		// �{�����[��
		if (change || cd.item_list[i].volume != m_ld.item_list[i].volume)	{
			lv.set_cell_text(i, 3, tostr(cd.item_list[i].volume));
		}

		// �|�[�Y�t���O
		if (change || cd.item_list[i].pause_flag != m_ld.item_list[i].pause_flag)	{
			lv.set_cell_text(i, 4, cd.item_list[i].pause_flag ? _T("�|�[�Y��") : _T(""));
		}

		// �҂�����
		if (change || cd.item_list[i].delay_time != m_ld.item_list[i].delay_time)	{
			lv.set_cell_text(i, 5, tostr(cd.item_list[i].delay_time));
		}

		// ���
		if (change || cd.item_list[i].state != m_ld.item_list[i].state)	{
			TSTR str;
			switch (cd.item_list[i].state)	{
				case TNM_PLAYER_STATE_FREE:				str = _T("");			break;
				case TNM_PLAYER_STATE_PLAY:				str = _T("�Đ���");		break;
				case TNM_PLAYER_STATE_FADE_OUT:			str = _T("̪��ޱ�Ē�");	break;
				case TNM_PLAYER_STATE_PAUSE:			str = _T("�ꎞ��~��");		break;
			}
			lv.set_cell_text(i, 6, str);
		}

		// ��߼��
		if (change || cd.item_list[i].option != m_ld.item_list[i].option)	{
			TSTR str;
			switch (cd.item_list[i].option)	{
				case 0:	str = _T("");						break;
				case 1:	str = _T("���[�v");					break;
				case 2:	str = _T("�Đ��I���҂�");			break;
				case 3:	str = _T("�t�F�[�h�A�E�g�I���҂�");	break;
			}
			lv.set_cell_text(i, 8, str);
		}

		// bgmfade
		if (change || cd.item_list[i].bgmfade_target != m_ld.item_list[i].bgmfade_target)	{
			lv.set_cell_text(i, 9, tostr(cd.item_list[i].bgmfade_target));
		}
		if (change || cd.item_list[i].bgmfade2_target != m_ld.item_list[i].bgmfade2_target)	{
			lv.set_cell_text(i, 10, tostr(cd.item_list[i].bgmfade2_target));
		}
	}

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}
