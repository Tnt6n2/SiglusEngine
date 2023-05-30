#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dir.h"
#include	"element/elm_sound.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_bgm_table.h"
#include	"utility/util_jitan_cnv.h"

// ****************************************************************
// ovk �w�b�_
// ================================================================
struct OVKPACKHEADER
{
	DWORD size;						// �t�@�C���T�C�Y
	DWORD offset;					// �I�t�Z�b�g
	int no;							// �ԍ�
	long smp_cnt;					// �T���v����
};

// ****************************************************************
// �v���C���[�F������
// ================================================================
void C_tnm_player::init()
{
	// �v���C���[��������
	m_player.init();

	// �v���C���[���}�l�[�W���ɓo�^
	G_pm.regist_player(&m_player);
}

// ****************************************************************
// �v���C���[�F���
// ================================================================
void C_tnm_player::free()
{
	// �v���C�}�l�[�W������v���C���[�𖕏�
	G_pm.delete_player(&m_player);

	// �v���C���[�����
	m_player.release();
}

// ****************************************************************
// �v���C���[�F�ď�����
// ================================================================
void C_tnm_player::reinit()
{
	// �T�E���h���������
	m_player.release_sound();

	// ����������
	m_sound_type = TNM_VOLUME_TYPE_NONE;
	m_bgm_no = -1;
	m_koe_no = -1;
	m_file_name.clear();
	m_se_no = -1;

	// ���p�N�{�����[��
	m_mouth_volume_table.clear();
}

// ****************************************************************
// �v���C���[�Fwav ���Đ��Fprivate �Ȃ̂ŏ������s�v
// ================================================================
bool C_tnm_player::__play_wav(TSTR base_dir, TSTR sub_dir, TSTR file_name, bool loop_flag, int fade_time, bool ready_only, bool check_only)
{
	return __play_wav(base_dir, sub_dir, file_name, loop_flag, 0, -1, 0, fade_time, ready_only, check_only);	// �ŏ�����Ō�܂�
}

bool C_tnm_player::__play_wav(TSTR base_dir, TSTR sub_dir, TSTR file_name, bool loop_flag, int start_pos, int end_pos, int repeat_pos, int fade_time, bool ready_only, bool check_only)
{
	// �t�@�C�������󂾂����琳��I��
	if (file_name.empty())
		return true;

	int sound_type = 0;	// 1: wav, 2: nwa, 3: ogg, 4: owp

	// �t�@�C��������
	TSTR file_path = tnm_find_wav(base_dir, sub_dir, file_name, &sound_type);
	if (file_path.empty())	{
		tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, _T("�T�E���h�t�@�C�� ") + file_name + _T(" ��������܂���ł����B"));
		return false;
	}

	// �`�F�b�N�݂̂Ȃ炱���Ő���I��
	if (check_only)
		return true;

	// �X�g���[������������
	switch (sound_type)	{
		case 1:
			{
				// wav �X�g���[������������
				BSP<C_wave_stream> wav_stream(new C_wave_stream);
				if (!wav_stream->open(file_path))	{
					tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, _T("�T�E���h�t�@�C�� ") + file_name + _T(" ���J���܂���ł����B\n") + get_last_error_msg());
					return false;
				}

				// �v���C���[�ɃX�g���[����ݒ�
				if (!m_player.set_stream(wav_stream))	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T�E���h�t�@�C�� ") + file_name + _T(" �̏������ɃG���[���������܂����B"));
					return false;
				}
			}
			break;
		case 2:
			{
				// nwa �X�g���[������������
				BSP<C_nwa_stream> nwa_stream(new C_nwa_stream);
				if (!nwa_stream->open(file_path))
					return tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, _T("�T�E���h�t�@�C�� ") + file_name + _T(" ���J���܂���ł����B"));

				// �v���C���[�ɃX�g���[����ݒ�
				if (!m_player.set_stream(nwa_stream))
					return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T�E���h�t�@�C�� ") + file_name + _T(" �̏������ɃG���[���������܂����B"));
			}
			break;
		case 3:
			{
				// ogg �X�g���[������������
				BSP<C_ogg_stream> ogg_stream(new C_ogg_stream(0));
				if (!ogg_stream->open(file_path))
					return tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, _T("�T�E���h�t�@�C�� ") + file_name + _T(" ���J���܂���ł����B"));

				// �v���C���[�ɃX�g���[����ݒ�
				if (!m_player.set_stream(ogg_stream))
					return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T�E���h�t�@�C�� ") + file_name + _T(" �̏������ɃG���[���������܂����B"));
			}
			break;
		case 4:
			{
				// owp �X�g���[������������
				BSP<C_ogg_stream> ogg_stream(new C_ogg_stream(0x39));
				if (!ogg_stream->open(file_path))
					return tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, _T("�T�E���h�t�@�C�� ") + file_name + _T(" ���J���܂���ł����B"));

				// �v���C���[�ɃX�g���[����ݒ�
				if (!m_player.set_stream(ogg_stream))
					return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T�E���h�t�@�C�� ") + file_name + _T(" �̏������ɃG���[���������܂����B"));
			}
			break;
	}

	// �p�����[�^��ݒ�
	m_player.set_loop(loop_flag);
	m_player.set_range(start_pos, end_pos, repeat_pos);

	// �Đ�
	if (ready_only)	{
		m_player.ready();
	}
	else	{
		if (fade_time <= 0)		m_player.play();
		else					m_player.fade_in(fade_time);
	}

	return true;
}

// ****************************************************************
// �v���C���[�F�a�f�l���Đ��Fpublic �Ȃ̂ŏ��������K�v
// ================================================================
bool C_tnm_player::play_bgm(int bgm_no, bool loop_flag, int fade_in_time, int start_pos, bool ready_only)
{
	reinit();

	if (bgm_no < 0 || Gp_ini->bgm_cnt <= bgm_no)
		return false;

	// �X�^�[�g�ʒu���w�肳��Ă��Ȃ��ꍇ�� ini �ɏ]��
	if (start_pos == TNM_BGM_START_POS_INI)
		start_pos = Gp_ini->bgm[bgm_no].start_pos;

	// �Đ�
	if (!__play_wav(Gp_dir->base_dir, _T("bgm"), Gp_ini->bgm[bgm_no].file_name, loop_flag, start_pos, Gp_ini->bgm[bgm_no].end_pos, Gp_ini->bgm[bgm_no].repeat_pos, fade_in_time, ready_only, false))
		return false;

	// �p�����[�^��ݒ�
	m_sound_type = TNM_VOLUME_TYPE_BGM;
	m_bgm_no = bgm_no;

	// �a�f�l�e�[�u���ɓo�^
	tnm_bgm_table_set_listened(bgm_no, true);

	return true;
}

#include <boost/algorithm/string/classification.hpp> // is_any_of
#include <boost/algorithm/string/split.hpp>

// ****************************************************************
// �v���C���[�F�����Đ��Fpublic �Ȃ̂ŏ��������K�v
// ================================================================
//		���Z�Đ����[�g�F100%�`400%
// ================================================================
bool C_tnm_player::play_koe(int koe_no, bool loop_flag, int jitan_rate, int play_pos_mills, bool ready_only, bool check_only)
{
	reinit();

	// �����[�X���[�h�Ń`�F�b�N�݂̂̏ꍇ�̓`�F�b�N���Ȃ�
	if (!Gp_global->debug_flag && check_only)
		return true;

	// ���ԍ����s����������I��
	if (koe_no < 0)
		return true;

	// ���ԍ��𕪊�����
	int scn_no = koe_no / 100000;
	int line_no = koe_no % 100000;

	// �t�@�C��������
	int sound_type = 0;
	TSTR base_dir = Gp_dir->base_dir;
	TSTR file_path = tnm_find_koe(base_dir, _T("koe"), koe_no, &sound_type);
	if (file_path.empty())	{
		tnm_set_error(TNM_ERROR_TYPE_KOE_NOT_FOUND, _T("���t�@�C�� ") + tostr(koe_no) + _T(" ��������܂���ł����B"));
		return false;
	}

	// �`�F�b�N�݂̂Ȃ炱���Ő���I���iovk �͂���������ƏI�����Ȃ��j
	if (sound_type != 3)	{
		if (check_only)
			return true;
	}

	// �I���������W�J�p�̃T�E���h
	BSP<C_sound> memory_sound(new C_sound);
	BSP<C_sound_stream> memory_stream(new C_sound_stream);

	if (false);
	else if (sound_type == 1)	{	// wav

		// wav �X�g���[������������
		BSP<C_wave_stream> wav_stream(new C_wave_stream);
		if (!wav_stream->open(file_path))
			return tnm_set_error(TNM_ERROR_TYPE_KOE_NOT_FOUND, _T("���t�@�C�� ") + tostr(koe_no) + _T(" ���J���܂���ł����B"));

		// �I���������ɃT�E���h�����[�h
		if (!memory_sound->load_sound(wav_stream.get()))
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���t�@�C�� ") + tostr(koe_no) + _T(" �̓W�J�Ɏ��s���܂����B"));
	}
	else if (sound_type == 2)	{	// nwa

		// nwa �X�g���[������������
		BSP<C_nwa_stream> nwa_stream(new C_nwa_stream);
		if (!nwa_stream->open(file_path))
			return tnm_set_error(TNM_ERROR_TYPE_KOE_NOT_FOUND, _T("���t�@�C�� ") + tostr(koe_no) + _T(" ���J���܂���ł����B"));

		// �I���������ɃT�E���h�����[�h
		if (!memory_sound->load_sound(nwa_stream.get()))
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���t�@�C�� ") + tostr(koe_no) + _T(" �̓W�J�Ɏ��s���܂����B"));
	}
	else if (sound_type == 3)	{	// ovk

		// �t�@�C�����J��
		C_file file;
		if (!file.open(file_path, _T("rb")))
			return tnm_set_error(TNM_ERROR_TYPE_KOE_NOT_FOUND, _T("���t�@�C�� ") + tostr(koe_no) + _T(" ���J���܂���ł����B"));

		// �I�t�Z�b�g�����߂�
		DWORD offset = -1;
		DWORD size = 0;
		DWORD koe_cnt = 0;
		file.read(&koe_cnt, 4);
		if (koe_cnt > 0)	{
			ARRAY<BYTE> read_buf;
			read_buf.resize(sizeof(OVKPACKHEADER) * koe_cnt);
			file.read(read_buf.get(), sizeof(OVKPACKHEADER) * koe_cnt);
			OVKPACKHEADER *oph = (OVKPACKHEADER *)read_buf.get();

			// ���ԍ���T��
			for (int k = 0; k < (int)koe_cnt; k++)	{
				if (oph->no == line_no)	{
					offset = oph->offset;
					size = oph->size;
					break;
				}
				oph ++;
			}
			if (offset == -1)	{
				return tnm_set_error(TNM_ERROR_TYPE_KOE_NOT_FOUND, _T("���t�@�C�� ") + tostr(koe_no) + _T(" ��������܂���ł����B"));
			}
			read_buf.clear();
		}

		// �`�F�b�N�݂̂Ȃ炱���Ő���I��
		if (check_only)
			return true;

		// ogg �X�g���[������������
		BSP<C_ogg_stream> ogg_stream(new C_ogg_stream(0));
		if (!ogg_stream->open(file_path, offset, size))
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���t�@�C�� ") + tostr(koe_no) + _T(" ���J���܂���ł����B"));

		// �I���������ɃT�E���h�����[�h
		if (!memory_sound->load_sound(ogg_stream.get()))
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���t�@�C�� ") + tostr(koe_no) + _T(" �̓W�J�Ɏ��s���܂����B"));
	}

	// ���Z�Đ�������ꍇ
	if (jitan_rate != 100)	{

		// ���m�����Ȃ畁�ʂɂ���
		if (memory_sound->get_channel_cnt() == 1)	{

			// ���Z�Đ��p�̃T�E���h���쐬
			BSP<C_sound> jitan_sound(new C_sound);
			jitan_sound->create(memory_sound->get_channel_cnt(), memory_sound->get_bit_cnt_per_sample(), memory_sound->get_sample_cnt_per_sec(), memory_sound->get_data_size());

			// ���Z�Đ����s��
			C_jitan_cnv jitan_converter;
			jitan_converter.convert(jitan_sound->get_buffer(), memory_sound->get_ptr(), memory_sound->get_data_size(), memory_sound->get_sample_cnt_per_sec(), memory_sound->get_channel_cnt(), memory_sound->get_bit_cnt_per_sample(), jitan_rate);
			jitan_sound->update_data_size();

			// �I������������T�E���h��ǂݍ��ރX�g���[�����쐬
			memory_stream->set_sound(jitan_sound);
		}
		// �X�e���I�Ȃ玞�Z�Đ��ɑΉ����Ă��Ȃ��̂Ń��m���������Ă݂�I
		else if (memory_sound->get_channel_cnt() == 2)	{

			// 16bit �O��ł��c

			// ���m�������p�̃o�b�t�@���쐬�i���m�������Ȃ̂Ńf�[�^�T�C�Y�͔����I�j
			BSP<C_sound> mono_sound(new C_sound);
			mono_sound->create(1, memory_sound->get_bit_cnt_per_sample(), memory_sound->get_sample_cnt_per_sec(), memory_sound->get_data_size() / 2);

			// ���m���������Ă݂�i���͑����Z���Ă邾���j
			int sample_cnt = memory_sound->get_sample_cnt();
			short* p_src = (short *)memory_sound->get_ptr();
			short* p_dst = (short *)mono_sound->get_ptr();
			short* p_dst_end = p_dst + sample_cnt;
			int value;
			while (p_dst != p_dst_end)	{
				 value = *p_src++;		// 1ch�ڂ���
				 value += *p_src++;		// 2ch�ڂ����Z
				 *p_dst++ = limit<int>(-30000, value, 30000);	// �O�̂���30000�ŕ␳
			}

			// ���Z�Đ��p�̃T�E���h���쐬�i���m����������̂Ńf�[�^�T�C�Y�͔����I�j
			BSP<C_sound> jitan_sound(new C_sound);
			jitan_sound->create(1, mono_sound->get_bit_cnt_per_sample(), mono_sound->get_sample_cnt_per_sec(), mono_sound->get_data_size());

			// ���Z�Đ����s��
			C_jitan_cnv jitan_converter;
			jitan_converter.convert(jitan_sound->get_buffer(), mono_sound->get_ptr(), mono_sound->get_data_size(), mono_sound->get_sample_cnt_per_sec(), mono_sound->get_channel_cnt(), mono_sound->get_bit_cnt_per_sample(), jitan_rate);
			jitan_sound->update_data_size();

			// �I������������T�E���h��ǂݍ��ރX�g���[�����쐬
			memory_stream->set_sound(jitan_sound);
		}
	}
	// ���Z�Đ������Ȃ��ꍇ
	else	{

		// �I������������T�E���h��ǂݍ��ރX�g���[�����쐬
		memory_stream->set_sound(memory_sound);
	}

	// �v���C���[�ɃI���������X�g���[����ݒ�
	m_player.set_stream(memory_stream);

	// �v���C���[�̃p�����[�^��ݒ�
	m_player.set_loop(loop_flag);
	m_player.set_range(0, -1, 0);
	m_player.set_play_pos_by_millsecond(play_pos_mills);

	// ���p�N�{�����[������͂���
	TSTR mouse_volume_file_path = tnm_find_dat(str_format(_T("z%09d.vol.csv"), koe_no));
	if (!mouse_volume_file_path.empty())
	{
		ARRAY<TSTR> mouse_volume_file_text;
		if (!C_file::read_full_text_UTF16(mouse_volume_file_path, mouse_volume_file_text))
		{
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���p�N�f�[�^ ") + tostr(koe_no) + _T(" �̓W�J�Ɏ��s���܂����B"));
		}
		if (mouse_volume_file_text.empty())
		{
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���p�N�f�[�^ ") + tostr(koe_no) + _T(" ����ł��B"));
		}

		std::vector<std::string> mouse_volume_table_mbstr;
		boost::algorithm::split(mouse_volume_table_mbstr, TSTR_to_MBSTR(mouse_volume_file_text[0]), boost::is_any_of(","));
		for (int i = 0; i < (int)mouse_volume_table_mbstr.size(); i++)
		{
			float volume = std::stof(mouse_volume_table_mbstr[i]);
			m_mouth_volume_table.push_back(volume);
		}
	}

	// �Đ�
	if (ready_only)	{
		m_player.ready();
	}
	else	{
		m_player.play();
	}

	// �p�����[�^��ݒ�
	m_sound_type = TNM_SOUND_TYPE_KOE;
	m_koe_no = koe_no;

	return true;
}

float C_tnm_player::get_current_mouth_volume()
{
	if (m_player.is_playing())
	{
		int play_pos_mill = m_player.get_play_pos_by_millsecond();
		int play_pos_frame = play_pos_mill * 60 / 1000;
		if (0 <= play_pos_frame && play_pos_frame < (int)m_mouth_volume_table.size())
		{
			return m_mouth_volume_table[play_pos_frame];
		}
	}

	return 0.0f;
}

// ****************************************************************
// �v���C���[�F���ʉ����Đ��Fpublic �Ȃ̂ŏ������K�v
// ================================================================
bool C_tnm_player::play_pcm(TSTR file_name, bool loop_flag, int fade_in_time, bool ready_only)
{
	reinit();

	// �Đ�
	if (!__play_wav(Gp_dir->base_dir, _T("wav"), file_name, loop_flag, fade_in_time, ready_only, false))
		return false;

	// �p�����[�^��ݒ�
	m_sound_type = TNM_SOUND_TYPE_PCM;
	m_file_name = file_name;

	return true;
}

// ****************************************************************
// �v���C���[�F�V�X�e�������Đ��Fpublic �Ȃ̂ŏ������K�v
// ================================================================
bool C_tnm_player::play_se(int se_no, bool ready_only)
{
	reinit();

	if (se_no < 0 || Gp_ini->se_cnt <= se_no)
		return false;

	// SE �ԍ�����t�@�C���p�X�����߂�
	TSTR file_name = Gp_ini->se[se_no].file_name;

	// �Đ�
	if (!__play_wav(Gp_dir->base_dir, _T("wav"), file_name, false, 0, ready_only, false))
		return false;

	// �p�����[�^��ݒ�
	m_sound_type = TNM_SOUND_TYPE_SE;
	m_se_no = se_no;

	return true;
}

// ****************************************************************
// �v���C���[�F��~
// ================================================================
void C_tnm_player::stop(int fade_time)
{
	// ��~
	if (fade_time <= 0)		m_player.release_sound();					// �T�E���h���Ɖ������i����Đ����� set_stream �������̂ł���łn�j�j
	else					m_player.fade_out_and_release(fade_time);	// �t�F�[�h�A�E�g���I����Ă��T�E���h�͎c����ςȂ������d���Ȃ�
}

// ****************************************************************
// �v���C���[�F�ꎞ��~
// ================================================================
void C_tnm_player::pause(int fade_time)
{
	if (fade_time <= 0)		m_player.pause();
	else					m_player.fade_out_and_pause(fade_time);
}

// ****************************************************************
// �v���C���[�F�ĊJ
// ================================================================
void C_tnm_player::resume(int fade_time)
{
	if (fade_time <= 0)		m_player.play();
	else					m_player.fade_in(fade_time);
}

// ****************************************************************
// �v���C���[�F�T�E���h�����
// ================================================================
void C_tnm_player::release_sound()
{
	m_player.release_sound();
}

// ****************************************************************
// �v���C���[�F�{�����[����ݒ�
// ================================================================
void C_tnm_player::__set_volume(int volume_channel_no, int volume, int fade_time)
{
	// �͈͐���
	volume = limit(TNM_VOLUME_MIN, volume, TNM_VOLUME_MAX);

	// �{�����[����ݒ肷��
	m_player.set_volume(volume_channel_no, volume, fade_time);
}

// ****************************************************************
// �v���C���[�F�{�����[�����擾
// ================================================================
int C_tnm_player::__get_volume(int volume_channel_no)
{
	return m_player.get_volume(volume_channel_no);
}

// ****************************************************************
// �v���C���[�F�V�X�e���{�����[����ݒ�
// ================================================================
void C_tnm_player::set_system_volume(int volume)
{
	// �{�����[����ݒ肷��
	__set_volume(TNM_PLAYER_VOLUME_CHANNEL_SYSTEM, volume, 0);
}

// ****************************************************************
// �v���C���[�F�V�X�e���{�����[�����擾
// ================================================================
int C_tnm_player::get_system_volume()
{
	return __get_volume(TNM_PLAYER_VOLUME_CHANNEL_SYSTEM);
}

// ****************************************************************
// �v���C���[�F�Q�[���{�����[����ݒ�
// ================================================================
void C_tnm_player::set_game_volume(int volume, int fade_time)
{
	// �{�����[����ݒ肷��
	__set_volume(TNM_PLAYER_VOLUME_CHANNEL_GAME, volume, fade_time);
}

// ****************************************************************
// �v���C���[�F�Q�[���{�����[�����擾
// ================================================================
int C_tnm_player::get_game_volume()
{
	return __get_volume(TNM_PLAYER_VOLUME_CHANNEL_GAME);
}

// ****************************************************************
// �v���C���[�F�v���C�ʒu���擾
// ================================================================
int C_tnm_player::get_play_pos()
{
	return m_player.get_play_pos_by_sample();
}
int C_tnm_player::get_play_pos_by_millsecond()
{
	return m_player.get_play_pos_by_millsecond();
}

// ****************************************************************
// �v���C���[�F�v���C�ʒu��ݒ�
// ================================================================
void C_tnm_player::set_play_pos_by_sample(int sample)
{
	m_player.set_play_pos_by_sample(sample);
}
void C_tnm_player::set_play_pos_by_millsecond(int mills)
{
	m_player.set_play_pos_by_millsecond(mills);
}

// ****************************************************************
// �v���C���[�F�v���C�T�C�Y���擾
// ================================================================
int C_tnm_player::get_length()
{
	return m_player.get_sample_cnt();
}
int C_tnm_player::get_length_by_millsecond()
{
	return m_player.get_length_by_millsecond();
}
