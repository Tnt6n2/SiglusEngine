#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_timer.h"
#include	"element/elm_sound.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_config.h"
#include	"engine/eng_etc.h"

#include	"engine/ifc_sound.h"

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�FBGM �̌����擾
// ================================================================
int tnm_get_bgm_cnt()
{
	return Gp_ini->bgm_cnt;
}

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�FBGM �ԍ����� BGM �o�^�����擾
// ================================================================
TSTR tnm_get_regist_name_by_bgm_no(int bgm_no)
{
	if (bgm_no < 0 || Gp_ini->bgm_cnt <= bgm_no)
		return _T("");

	return Gp_ini->bgm[bgm_no].regist_name;
}

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�FBGM �o�^������ BGM �ԍ����擾
// ================================================================
int tnm_get_bgm_no_by_regist_name(CTSTR& bgm_regist_name)
{
	int bgm_no = -1;

	// Gameexe.ini ����o�^����T��
	for (int i = 0; i < Gp_ini->bgm_cnt; i++)	{
		if (str_to_lower_ret(bgm_regist_name) == str_to_lower_ret(Gp_ini->bgm[i].regist_name))	{
			bgm_no = i;
			break;
		}
	}
	if (bgm_no == -1)	{
		TSTR error_str;
		error_str += _T("BGM �̓o�^ ") + bgm_regist_name + _T(" ��������܂���B");
		error_str += _T("\nBGM ���Đ�����ɂ́AGameexe.ini �� BGM ��o�^����K�v������܂��B");
		error_str += _T("\n\n��悭����ԈႢ��");
		error_str += _T("\nBGM �̓o�^���ƃt�@�C�������t�ɂȂ��Ă��܂��񂩁H");
		tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, error_str);
		return -1;
	}

	return bgm_no;
}

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�FBGM ���Ȃ��Ă��邩�𔻒�
// ================================================================
bool tnm_check_bgm_playing()
{
	int state = Gp_sound->m_bgm.check();

	if (state == TNM_PLAYER_STATE_PLAY)
	{
		return true;
	}

	return false;
}

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�F�����Đ�����
// ================================================================
void tnm_play_koe(int koe_no, int chara_no, int jitan_rate, int play_pos_mills, bool wait_flag, bool key_skip_flag, bool return_value_flag, bool ex_koe_flag, bool check_only_flag)
{
	Gp_sound->m_koe.play(koe_no, chara_no, jitan_rate, play_pos_mills, wait_flag, key_skip_flag, return_value_flag, ex_koe_flag, check_only_flag);
}

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�F�����~����
// ================================================================
void tnm_stop_koe()
{
	Gp_sound->m_koe.stop();
}

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�FSE ���Đ�����
// ================================================================
void tnm_play_se(int se_no)
{
	Gp_sound->m_se.play_se(se_no, NULL);
}

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�F���[�r�[�����������𔻒�
// ================================================================
bool tnm_is_movie_readying()
{
	return Gp_sound->m_mov.is_readying();
}

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�F���[�r�[���Đ������𔻒�
// ================================================================
bool tnm_is_movie_playing()
{
	return Gp_sound->m_mov.is_playing();
}

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�Fomv ���ǂ݂���i�f�B�X�N�L���b�V���ɏ悹�č��������v��I�j
// ================================================================
bool tnm_preload_omv(CTSTR& file_name)
{
	TSTR file_path = tnm_find_omv(file_name);
	if (file_path.empty())	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, file_name + _T(".omv ��������܂���ł����B"));
		return false;
	}

	C_file file;
	if (!file.open(file_path, _T("rb")))	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, file_name + _T(".omv ���J���܂���ł����B\n") + get_last_error_msg());
		return false;
	}

	const int buf_size = 1024 * 1024;	// 1MB
	BUFFER buf(buf_size);
	file.read(buf.get(), buf_size);	// ��ǂ݂��Ă݂�
	file.close();

	return true;
}

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X�F�T�E���h�̃{�����[�����X�V����
// ================================================================

// static�F�{�����[��
int G_all_total_volume = 255;
int G_bgm_total_volume = 255;
int G_koe_total_volume = 255;
int G_exkoe_total_volume = 255;
int G_pcm_total_volume = 255;
int G_se_total_volume = 255;
int G_mov_total_volume = 255;

// static�F�a�f�l�t�F�[�h���g���H
static bool tnm_is_bgmfade_use(int koe_buf_total_volume, bool is_pcmch_bgm_fade_use)
{
	if (!Gp_config->bgmfade_use_check)	{
		return false;	// ���ݒ�ŃI�t�Ȃ�g��Ȃ�
	}
	else if (Gp_local->pod.bgmfade_flag)	{
		return true;	// ���[�J���̂a�f�l�t�F�[�h�t���O�������Ă���Ύg��
	}
	else if (koe_buf_total_volume > 0 && Gp_sound->m_koe.is_playing())	{
		return true;	// ���̃{�����[�����O�ȏ�Ő����Đ����Ȃ�g��
	}
	else if (is_pcmch_bgm_fade_use)	{
		return true;	// pcmch �Ŏg���Ɣ��肳�ꂽ�ꍇ�͎g��
	}
	else	{
		return false;	// ����ȊO�Ȃ�g��Ȃ�
	}
}

// static�F�a�f�l�t�F�[�h�Q���g���H
static bool tnm_is_bgmfade2_use(int koe_buf_total_volume, bool is_pcmch_bgm_fade_use)
{
	if (koe_buf_total_volume > 0 && Gp_sound->m_koe.is_playing())	{
		return true;	// ���̃{�����[�����O�ȏ�Ő����Đ����Ȃ�g��
	}
	else if (is_pcmch_bgm_fade_use)	{
		return true;	// pcmch �Ŏg���Ɣ��肳�ꂽ�ꍇ�͎g��
	}
	else	{
		return false;	// ����ȊO�Ȃ�g��Ȃ�
	}
}

// static�Fpcmch �̃{�����[�����X�V
static void tnm_update_sound_volume_pcmch(int step, bool change_force, bool* p_is_bgm_fade_use)
{
	// step 0: bgmfade �̃\�[�X�ƂȂ� pcmch
	// step 1: bgmfade �̃\�[�X�ƂȂ�Ȃ� pcmch�i�ʏ�j

	for (int i = 0; i < (int)Gp_sound->m_pcmch_list.get_size(); i++)	{
		if ((step == 0 && Gp_sound->m_pcmch_list[i].is_bgm_fade_source()) || (step == 1 && !Gp_sound->m_pcmch_list[i].is_bgm_fade_source()))	{

			int new_volume = 255;
			int volume_type = Gp_sound->m_pcmch_list[i].get_volume_type();
			switch (volume_type)	{
				case TNM_VOLUME_TYPE_NONE:		new_volume = G_all_total_volume;	break;
				case TNM_VOLUME_TYPE_BGM:		new_volume = G_bgm_total_volume;	break;
				case TNM_VOLUME_TYPE_KOE:		new_volume = G_koe_total_volume;	break;
				case TNM_VOLUME_TYPE_PCM:		new_volume = G_pcm_total_volume;	break;
				case TNM_VOLUME_TYPE_SE:		new_volume = G_se_total_volume;		break;
				case TNM_VOLUME_TYPE_MOV:		new_volume = G_mov_total_volume;	break;
				default:
					// �ėp�{�����[��
					if (16 <= volume_type && volume_type < TNM_VOLUME_TYPE_MAX)	{
						if (Gp_config->play_sound_check[volume_type])	{
							new_volume = G_all_total_volume * Gp_config->sound_user_volume[volume_type] / 255;
						}
						else	{
							new_volume = 0;
						}
					}
					break;
			}

			// �a�f�l�t�F�[�h
			if (Gp_sound->m_pcmch_list[i].is_bgm_fade_target())	{
				new_volume = new_volume * Gp_global->bgmfade_total_volume / 255;
			}
			if (Gp_sound->m_pcmch_list[i].is_bgm_fade2_target())	{
				new_volume = new_volume * Gp_global->bgmfade2_total_volume / 255;
			}

			// �L�����N�^����
			int pcmch_chara_no = Gp_sound->m_pcmch_list[i].get_chara_no();
			new_volume = new_volume * tnm_get_chrkoe_volume(pcmch_chara_no) / 255;	// �{�����[��
			new_volume = tnm_get_chrkoe_onoff(pcmch_chara_no) ? new_volume : 0;		// �I���I�t

			// �{�����[����ύX����
			if (change_force || new_volume != Gp_sound->m_pcmch_list[i].get_system_volume())	{
				Gp_sound->m_pcmch_list[i].set_system_volume(new_volume);
			}

			// �a�f�l�t�F�[�h���g�����𔻒�
			if (p_is_bgm_fade_use && step == 0 && new_volume > 0 && Gp_sound->m_pcmch_list[i].is_playing())	{
				*p_is_bgm_fade_use = true;
			}
		}
	}
}


// �{�����[�����X�V�F�{��
bool tnm_update_sound_volume(bool change_force)
{
	bool is_pcmch_bgm_fade_use = false;

	// ���̓��ꏈ���ł��B
	// ���̍Đ��Ƀ`�F�b�N�����Ă��Ȃ��ꍇ�Akoe �͎~�߂܂��Bexkoe �͒ʏ�ʂ�Đ����܂��B
	// �S�̂̍Đ��Ƀ`�F�b�N�����Ă��Ȃ��ꍇ�Akoe �͎~�߂܂��Bexkoe �̓{�����[���O���ɂ��܂��B
	// �������[�v������Ȃ炱�̎d�l�_�����ȁc

	// ����̐����t���[��������
	G_all_total_volume = 255;
	G_bgm_total_volume = 255;
	G_koe_total_volume = 255;
	G_exkoe_total_volume = 255;
	G_pcm_total_volume = 255;
	G_se_total_volume = 255;
	G_mov_total_volume = 255;

	// �ʏ�̐��̏ꍇ�A�����Đ����Ȃ��ꍇ�͐����~�߂�
	if (!Gp_sound->m_koe.get_ex_koe_flag())	{
		int koe_chara_no = Gp_sound->m_koe.get_chara_no();
		bool koe_play_flag = tnm_is_koe_play(koe_chara_no);

		// �����Đ����Ȃ��Ȃ琺���~�߂�
		if (!koe_play_flag && Gp_sound->m_koe.is_playing())
			Gp_sound->m_koe.stop();
	}

	// �S�̃{�����[�����v�Z
	G_all_total_volume = G_all_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;	// �S�̃{�����[���ɂ�钲��

	// koe �̃{�����[�����v�Z

	// �ʏ�̐��̏ꍇ�͐��I�t�ɉe�������
	// �����Ōv�Z�����̂́Apcmch �́u���`�����l���̉��ʂōĐ��v�Ŏg���鉹��
	G_koe_total_volume = G_koe_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;										// �S�̃{�����[���ɂ�钲��
	G_koe_total_volume = G_koe_total_volume * (Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE] ? Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE] : 0) / 255;		// ���{�����[���ɂ�钲��

	// �d�w���̏ꍇ�͐��I�t�𖳎�����
	// �����Ōv�Z�����̂́Apcmch �́u�d�w���`�����l���̉��ʂōĐ��v�Ŏg���鉹��
	G_exkoe_total_volume = G_exkoe_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;									// �S�̃{�����[���ɂ�钲��
	G_exkoe_total_volume = G_exkoe_total_volume * Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE] / 255;															// ���{�����[���ɂ�钲��

	// ���ۂ̐��o�b�t�@�͐����d�w�����ɂ���ĕς��
	int koe_buf_total_volume = 255;
	if (Gp_sound->m_koe.get_ex_koe_flag())	{
		koe_buf_total_volume = G_exkoe_total_volume;
	}
	else	{
		koe_buf_total_volume = G_koe_total_volume;
	}

	// ���o�b�t�@�ɃL�����N�^������K�p����
	int koe_chara_no = Gp_sound->m_koe.get_chara_no();
	koe_buf_total_volume = koe_buf_total_volume * tnm_get_chrkoe_volume(koe_chara_no) / 255;	// �L�����N�^�����ɂ��{�����[������

	// �a�f�l
	G_bgm_total_volume = G_bgm_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;									// �S�̃{�����[���ɂ�钲��
	G_bgm_total_volume = G_bgm_total_volume * (Gp_config->play_sound_check[TNM_VOLUME_TYPE_BGM] ? Gp_config->sound_user_volume[TNM_VOLUME_TYPE_BGM] : 0) / 255;	// �a�f�l�{�����[���ɂ�钲��

	// ���ʉ�
	G_pcm_total_volume = G_pcm_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;									// �S�̃{�����[���ɂ�钲��
	G_pcm_total_volume = G_pcm_total_volume * (Gp_config->play_sound_check[TNM_VOLUME_TYPE_PCM] ? Gp_config->sound_user_volume[TNM_VOLUME_TYPE_PCM] : 0) / 255;	// �o�b�l�{�����[���ɂ�钲��

	// �V�X�e����
	G_se_total_volume = G_se_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;										// �S�̃{�����[���ɂ�钲��
	G_se_total_volume = G_se_total_volume * (Gp_config->play_sound_check[TNM_VOLUME_TYPE_SE] ? Gp_config->sound_user_volume[TNM_VOLUME_TYPE_SE] : 0) / 255;		// �r�d�{�����[���ɂ�钲��

	// ���[�r�[
	G_mov_total_volume = G_mov_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;									// �S�̃{�����[���ɂ�钲��
	G_mov_total_volume = G_mov_total_volume * (Gp_config->play_sound_check[TNM_VOLUME_TYPE_MOV] ? Gp_config->sound_user_volume[TNM_VOLUME_TYPE_MOV] : 0) / 255;	// �l�n�u�{�����[���ɂ�钲��

	// pcmch ���a�f�l�t�F�[�h���g�����ǂ����𔻒肵�A�����Ƀ{�����[�����X�V����
	tnm_update_sound_volume_pcmch(0, change_force, &is_pcmch_bgm_fade_use);

	// �����a�f�l�t�F�[�h���g�����ǂ����𔻒肷��
	// koe_buf�i���ۂɗ���Ă鉹�ʁj���g�����Ƃɒ���
	bool is_bgm_fade_use = tnm_is_bgmfade_use(koe_buf_total_volume, is_pcmch_bgm_fade_use);
	bool is_bgm_fade2_use = tnm_is_bgmfade2_use(koe_buf_total_volume, is_pcmch_bgm_fade_use);
	
	// �a�f�l�t�F�[�h�̒l���v�Z

	// �a�f�l�t�F�[�h�̎��Ԃ�i�߂�
	Gp_global->bgmfade_cur_time += Gp_timer->real_time_past;
	Gp_global->bgmfade2_cur_time += Gp_timer->real_time_past;

	// �a�f�l�t�F�[�h�J�n�����i���̍��v�{�����[�����O�̂Ƃ��͏������Ȃ��j

	if (!Gp_global->bgmfade_flag)	{
		if (is_bgm_fade_use)	{

			// �a�f�l�t�F�[�h�J�n�����J�n�I
			Gp_global->bgmfade_cur_time = 0;
			Gp_global->bgmfade_start_value = Gp_global->bgmfade_total_volume;	// ���݂̂a�f�l�t�F�[�h�̃{�����[������J�n
			Gp_global->bgmfade_flag = true;
		}
	}
	if (Gp_global->bgmfade_flag)	{
		if (!is_bgm_fade_use)	{

			// �a�f�l�t�F�[�h�I�������J�n�I
			Gp_global->bgmfade_cur_time = 0;
			Gp_global->bgmfade_start_value = Gp_global->bgmfade_total_volume;	// ���݂̂a�f�l�t�F�[�h�̃{�����[������J�n
			Gp_global->bgmfade_flag = false;
		}
	}
	if (!Gp_global->bgmfade2_flag)	{
		if (is_bgm_fade2_use)	{

			// �a�f�l�t�F�[�h�Q�J�n�����J�n�I
			Gp_global->bgmfade2_cur_time = 0;
			Gp_global->bgmfade2_start_value = Gp_global->bgmfade2_total_volume;	// ���݂̂a�f�l�t�F�[�h�̃{�����[������J�n
			Gp_global->bgmfade2_flag = true;
		}
	}
	if (Gp_global->bgmfade2_flag)	{
		if (!is_bgm_fade2_use)	{

			// �a�f�l�t�F�[�h�Q�I�������J�n�I
			Gp_global->bgmfade2_cur_time = 0;
			Gp_global->bgmfade2_start_value = Gp_global->bgmfade2_total_volume;	// ���݂̂a�f�l�t�F�[�h�̃{�����[������J�n
			Gp_global->bgmfade2_flag = false;
		}
	}

	// �a�f�l�t�F�[�h�̒l���v�Z
	Gp_global->bgmfade_total_volume = 255;
	Gp_global->bgmfade2_total_volume = 255;
	if (Gp_global->bgmfade_flag)	{
		// 0�`1000 �ؕb�̊Ԃɉ��ʂ� bgmfade �l�܂Ŏ����Ă���
		Gp_global->bgmfade_total_volume = linear_limit(Gp_global->bgmfade_cur_time, 0, Gp_global->bgmfade_start_value, 1000, Gp_config->bgmfade_volume);
	}
	else	{
		// 2000�`3000 �ؕb�̊Ԃɉ��ʂ� 255 �܂Ŏ����Ă���
		Gp_global->bgmfade_total_volume = linear_limit(Gp_global->bgmfade_cur_time, 2000, Gp_global->bgmfade_start_value, 3000, 255);
	}
	if (Gp_global->bgmfade2_flag)	{
		// 0�`500 �ؕb�̊Ԃɉ��ʂ� 0 �܂Ŏ����Ă���
		Gp_global->bgmfade2_total_volume = linear_limit(Gp_global->bgmfade2_cur_time, Gp_ini->bgmfade2_in_start_time, Gp_global->bgmfade2_start_value, Gp_ini->bgmfade2_in_start_time + Gp_ini->bgmfade2_in_time_len, Gp_ini->bgmfade2_volume);
	}
	else	{
		if (Gp_global->bgmfade2_need_flag)	{
			// 0�`500 �ؕb�̊Ԃɉ��ʂ� 255 �܂Ŏ����Ă���
			Gp_global->bgmfade2_total_volume = linear_limit(Gp_global->bgmfade2_cur_time, Gp_ini->bgmfade2_out_start_time, Gp_global->bgmfade2_start_value, Gp_ini->bgmfade2_out_start_time + Gp_ini->bgmfade2_out_time_len, 255);
		}
		else	{	// �������P�^�P�΍�A���̂Ȃ����͂̏ꍇ�͂����ɕ��A�i�ł�500ms�͑҂Ƃ��B���Ɛ��̋��ň�u�a�f�u���Ȃ�̂�h�����߁j
			// 0�`500 �ؕb�̊Ԃɉ��ʂ� 255 �܂Ŏ����Ă���
			Gp_global->bgmfade2_total_volume = linear_limit(Gp_global->bgmfade2_cur_time, 500, Gp_global->bgmfade2_start_value, 500 + Gp_ini->bgmfade2_out_time_len, 255);
		}
	}

	// �a�f�l�t�F�[�h��K�p
	G_bgm_total_volume = G_bgm_total_volume * Gp_global->bgmfade_total_volume / 255;

	// �e�o�b�t�@�̃{�����[�������߂�
	int bgm_buf_total_volume = G_bgm_total_volume;
	int pcm_buf_total_volume = G_pcm_total_volume;
	int se_buf_total_volume = G_se_total_volume;
	int mov_buf_total_volume = G_mov_total_volume;

	// �{�����[�����X�V����
	if (change_force || bgm_buf_total_volume != Gp_global->bgm_buf_total_volume)	{
		Gp_sound->m_bgm.set_system_volume(bgm_buf_total_volume);
	}
	if (change_force || koe_buf_total_volume != Gp_global->koe_buf_total_volume)	{
		Gp_sound->m_koe.set_system_volume(koe_buf_total_volume);
	}
	if (change_force || pcm_buf_total_volume != Gp_global->pcm_buf_total_volume)	{
		Gp_sound->m_pcm.set_system_volume(pcm_buf_total_volume);
	}
	if (change_force || se_buf_total_volume != Gp_global->se_buf_total_volume)	{
		Gp_sound->m_se.set_system_volume(se_buf_total_volume);
	}
	if (change_force || mov_buf_total_volume != Gp_global->mov_buf_total_volume)	{
		Gp_sound->m_mov.set_volume(mov_buf_total_volume);
	}

	// ���ʉ��`�����l�����{�����[�����X�V����
	tnm_update_sound_volume_pcmch(1, change_force, NULL);

	// �l��ۑ�����
	Gp_global->all_total_volume = G_all_total_volume;
	Gp_global->bgm_buf_total_volume = bgm_buf_total_volume;
	Gp_global->koe_buf_total_volume = koe_buf_total_volume;
	Gp_global->pcm_buf_total_volume = pcm_buf_total_volume;
	Gp_global->se_buf_total_volume = se_buf_total_volume;
	Gp_global->mov_buf_total_volume = mov_buf_total_volume;

	// ��p�`�����l���Ŗ�����炵������
	{
		bool play_silent_sound = Gp_system_config->ds_play_silent_sound_flag;

		// �J�n
		if (play_silent_sound && !Gp_global->play_silent_sound)	{
			Gp_sound->m_muon_sound = BSP<C_sound>(new C_sound);
			Gp_sound->m_muon_sound->create(1, 16, 44100, 1024);
			Gp_sound->m_muon_stream = BSP<C_sound_stream>(new C_sound_stream);
			Gp_sound->m_muon_stream->set_sound(Gp_sound->m_muon_sound);
			Gp_sound->m_muon_player = BSP<C_player>(new C_player);
			Gp_sound->m_muon_player->set_stream(Gp_sound->m_muon_stream);
			Gp_sound->m_muon_player->play();
		}
		// ��~
		else if (!play_silent_sound && Gp_global->play_silent_sound)	{
			if (Gp_sound->m_muon_player)
				Gp_sound->m_muon_player.reset();
			if (Gp_sound->m_muon_stream)
				Gp_sound->m_muon_stream.reset();
			if (Gp_sound->m_muon_sound)
				Gp_sound->m_muon_sound.reset();
		}

		// �l��ۑ�����
		Gp_global->play_silent_sound = play_silent_sound;
	}

	return true;
}

