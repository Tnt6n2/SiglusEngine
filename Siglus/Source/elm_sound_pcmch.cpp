#include	"pch.h"
#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// PCMCH�F������
// ================================================================
void C_elm_pcmch::init(S_element element, int form, CTSTR& name)
{
	// ���̏�����
	C_tnm_element::init(element, form, name, NULL);

	// �v���C���[��������
	m_player.init();
}

// ****************************************************************
// PCMCH�F���
// ================================================================
void C_elm_pcmch::free()
{
	// �v���C���[�����
	m_player.free();
}

// ****************************************************************
// PCMCH�F�ď�����
// ================================================================
void C_elm_pcmch::reinit()
{
	// �p�����[�^��������
	m_pcm_name.clear();
	m_bgm_name.clear();
	m_koe_no = -1;
	m_se_no = -1;
	m_volume_type = TNM_VOLUME_TYPE_PCM;
	m_chara_no = -1;
	m_volume = TNM_VOLUME_MAX;
	m_delay_time = 0;
	m_fade_in_time = 0;
	m_loop_flag = false;
	m_bgm_fade_target_flag = false;
	m_bgm_fade2_target_flag = false;
	m_bgm_fade_source_flag = false;
	m_ready_flag = false;

	// �v���C���[���ď�����
	m_player.reinit();

	// �v���C���[�̃{�����[����ݒ�
	m_player.set_game_volume(m_volume, 0);
}

// ****************************************************************
// PCMCH�F�Z�[�u
// ================================================================
void C_elm_pcmch::save(C_tnm_save_stream& stream)
{
	// �p�����[�^���Z�[�u
	stream.save(m_pcm_name);
	stream.save(m_bgm_name);
	stream.save(m_koe_no);
	stream.save(m_se_no);
	stream.save(m_volume_type);
	stream.save(m_chara_no);
	stream.save(m_volume);
	stream.save(m_delay_time);
	stream.save(m_loop_flag);
	stream.save(m_bgm_fade_target_flag);
	stream.save(m_bgm_fade2_target_flag);
	stream.save(m_bgm_fade_source_flag);
	stream.save(m_ready_flag);
}

// ****************************************************************
// PCMCH�F���[�h
// ================================================================
void C_elm_pcmch::load(C_tnm_save_stream& stream)
{
	TSTR pcm_name, bgm_name;
	int koe_no, se_no, volume_type, chara_no, volume, delay_time;
	bool loop_flag, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, ready_flag;

	// �ꎞ�ϐ��Ƀp�����[�^�����[�h
	stream.load(pcm_name);
	stream.load(bgm_name);
	stream.load(koe_no);
	stream.load(se_no);
	stream.load(volume_type);
	stream.load(chara_no);
	stream.load(volume);
	stream.load(delay_time);
	stream.load(loop_flag);
	stream.load(bgm_fade_target_flag);
	stream.load(bgm_fade2_target_flag);
	stream.load(bgm_fade_source_flag);
	stream.load(ready_flag);

	// �p�����[�^���v���C���[�ɔ��f������
	set_volume(volume);

	// ���[�v�Đ��܂��͏������Ă����ꍇ�́A�Đ��𕜌�����
	if (loop_flag || ready_flag || delay_time > 0)	{
		if (!pcm_name.empty())	{
			play_pcm(pcm_name, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, ready_flag);
		}
		if (!bgm_name.empty())	{
			play_bgm(bgm_name, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, ready_flag);
		}
		else if (koe_no >= 0)	{
			play_koe(koe_no, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, ready_flag);
		}
		else if (se_no >= 0)	{
			play_se(se_no, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, ready_flag);
		}
	}
}

// ****************************************************************
// PCMCH�F���Ԃ�i�߂�
// ================================================================
void C_elm_pcmch::update_time(int past_real_time)
{
	// �x���Đ��̏ꍇ�A���Ԃ��o������ĊJ����
	if (m_delay_time > 0)	{
		m_delay_time -= past_real_time;
		if (m_delay_time <= 0)	{

			// �ĊJ
			resume(false, m_fade_in_time, 0); 
		}
	}
}

// ****************************************************************
// PCMCH�F�Đ��i�t���p�����[�^�j
// ================================================================
void C_elm_pcmch::play_pcm(TSTR pcm_name, bool loop_flag, bool wait_flag, int fade_in_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only)
{
	// ���݂̃T�E���h���������
	m_player.release_sound();

	// �{�����[���^�C�v���ς��Ȃ�{�����[�����Đݒ肷��
	tnm_update_sound_volume(false);

	// �v���C���[�ōĐ�
	if (!m_player.play_pcm(pcm_name, loop_flag, fade_in_time, ready_only))
		return;

	// �p�����[�^��ݒ�
	m_pcm_name = pcm_name;
	m_bgm_name.clear();
	m_koe_no = -1;
	m_se_no = -1;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_delay_time = 0;
	m_fade_in_time = fade_in_time;
	m_loop_flag = loop_flag;
	m_bgm_fade_target_flag = bgm_fade_target_flag;
	m_bgm_fade2_target_flag = bgm_fade2_target_flag;
	m_bgm_fade_source_flag = bgm_fade_source_flag;
	m_ready_flag = ready_only;

	// �I���҂�
	if (wait_flag)
		wait(false, false);
}

// ****************************************************************
// PCMCH�FBGM ���Đ��i�t���p�����[�^�j
// ================================================================
void C_elm_pcmch::play_bgm(TSTR bgm_name, bool loop_flag, bool wait_flag, int fade_in_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only)
{
	// ���݂̃T�E���h���������
	m_player.release_sound();

	// �{�����[���^�C�v���ς��Ȃ�{�����[�����Đݒ肷��
	tnm_update_sound_volume(false);

	// BGM �o�^������ BGM �ԍ����擾
	int bgm_no = tnm_get_bgm_no_by_regist_name(bgm_name);
	if (bgm_no == -1)
		return;

	// �v���C���[�ōĐ�
	if (!m_player.play_bgm(bgm_no, loop_flag, fade_in_time, TNM_BGM_START_POS_INI, ready_only))
		return;

	// �p�����[�^��ݒ�
	m_pcm_name.clear();
	m_bgm_name = bgm_name;
	m_koe_no = -1;
	m_se_no = -1;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_delay_time = 0;
	m_fade_in_time = fade_in_time;
	m_loop_flag = loop_flag;
	m_bgm_fade_target_flag = bgm_fade_target_flag;
	m_bgm_fade2_target_flag = bgm_fade2_target_flag;
	m_bgm_fade_source_flag = bgm_fade_source_flag;
	m_ready_flag = ready_only;

	// �I���҂�
	if (wait_flag)
		wait(false, false);
}

// ****************************************************************
// PCMCH�F�����Đ��i�t���p�����[�^�j
// ================================================================
void C_elm_pcmch::play_koe(int koe_no, bool loop_flag, bool wait_flag, int fade_in_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only)
{
	// ���݂̃T�E���h���������
	m_player.release_sound();

	// �{�����[���^�C�v���ς��Ȃ�{�����[�����Đݒ肷��
	tnm_update_sound_volume(false);

	// �v���C���[�ōĐ�
	if (!m_player.play_koe(koe_no, loop_flag, TNM_JITAN_RATE_NORMAL, 0, ready_only, false))
		return;

	// �p�����[�^��ݒ�
	m_pcm_name.clear();
	m_bgm_name.clear();
	m_koe_no = koe_no;
	m_se_no = -1;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_delay_time = 0;
	m_fade_in_time = fade_in_time;
	m_loop_flag = loop_flag;
	m_bgm_fade_target_flag = bgm_fade_target_flag;
	m_bgm_fade2_target_flag = bgm_fade2_target_flag;
	m_bgm_fade_source_flag = bgm_fade_source_flag;
	m_ready_flag = ready_only;

	// �I���҂�
	if (wait_flag)
		wait(false, false);
}

// ****************************************************************
// PCMCH�F�V�X�e�������Đ��i�t���p�����[�^�j
// ================================================================
void C_elm_pcmch::play_se(int se_no, bool loop_flag, bool wait_flag, int fade_in_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only)
{
	// ���݂̃T�E���h���������
	m_player.release_sound();

	// �{�����[���^�C�v���ς��Ȃ�{�����[�����Đݒ肷��
	tnm_update_sound_volume(false);

	// �v���C���[�ōĐ�
	if (!m_player.play_se(se_no, ready_only))
		return;

	// �p�����[�^��ݒ�
	m_pcm_name.clear();
	m_bgm_name.clear();
	m_koe_no = -1;
	m_se_no = se_no;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_delay_time = 0;
	m_fade_in_time = fade_in_time;
	m_loop_flag = loop_flag;
	m_bgm_fade_target_flag = bgm_fade_target_flag;
	m_bgm_fade2_target_flag = bgm_fade2_target_flag;
	m_bgm_fade_source_flag = bgm_fade_source_flag;
	m_ready_flag = ready_only;

	// �I���҂�
	if (wait_flag)
		wait(false, false);
}

// ****************************************************************
// PCMCH�F��~
// ================================================================
void C_elm_pcmch::stop(int fade_out_time)
{
	m_player.stop(fade_out_time);

	// ���[�v�t���O�����͏���������
	m_loop_flag = false;
}

// ****************************************************************
// PCMCH�F�ꎞ��~
// ================================================================
void C_elm_pcmch::pause(int fade_out_time)
{
	m_player.pause(fade_out_time);
}

// ****************************************************************
// PCMCH�F�ĊJ
// ================================================================
void C_elm_pcmch::resume(bool wait_flag, int fade_in_time, int delay_time)
{
	if (delay_time < 0)
		delay_time = 0;

	// �������t���O���~�낷
	m_ready_flag = false;

	// �x���ĊJ�łȂ���΍ĊJ����
	if (delay_time == 0)	{
		m_player.resume(fade_in_time);

		// �I���҂�
		if (wait_flag)
			wait(false, false);
	}

	// �p�����[�^��ݒ�
	m_delay_time = delay_time;
	m_fade_in_time = fade_in_time;
}

// ****************************************************************
// PCMCH�F�Đ��I���҂�
// ================================================================
void C_elm_pcmch::wait(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_PCMCH_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// PCMCH�F�t�F�[�h�A�E�g�I���҂�
// ================================================================
void C_elm_pcmch::wait_fade(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_PCMCH_FADE_OUT_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// PCMCH�F�Đ������ǂ����𔻒�
// ================================================================
bool C_elm_pcmch::is_playing()
{
	return m_player.is_playing();
}

// ****************************************************************
// PCMCH�F�t�F�[�h�A�E�g�����ǂ����𔻒�
// ================================================================
bool C_elm_pcmch::is_fade_out_doing()
{
	return m_player.is_fade_out();
}

// ****************************************************************
// PCMCH�F�{�����[����ݒ�^�擾
// ================================================================
void C_elm_pcmch::set_volume(int volume, int fade_time)
{
	// �v���C���[�̃Q�[���{�����[����ݒ�
	m_player.set_game_volume(volume, fade_time);

	// �p�����[�^��ݒ�
	m_volume = volume;
}

void C_elm_pcmch::set_volume_max(int fade_time)
{
	set_volume(TNM_VOLUME_MAX, fade_time);
}

void C_elm_pcmch::set_volume_min(int fade_time)
{
	set_volume(TNM_VOLUME_MIN, fade_time);
}

// ****************************************************************
// PCMCH�F�V�X�e���{�����[����ݒ�
// ================================================================
void C_elm_pcmch::set_system_volume(int volume)
{
	m_player.set_system_volume(volume);
}

// ****************************************************************
// PCMCH�F�V�X�e���{�����[�����擾
// ================================================================
int C_elm_pcmch::get_system_volume()
{
	return m_player.get_system_volume();
}

// ****************************************************************
// PCMCH ���X�g�F�T�u�̏�����
// ================================================================
void C_elm_pcmch_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), FM_PCMCH, get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// PCMCH ���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_pcmch_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// PCMCH ���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_pcmch_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// PCMCH ���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_pcmch_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
