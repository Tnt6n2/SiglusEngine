#include	"pch.h"

#include	"data/tnm_def.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_sound.h"


// ****************************************************************
// BGM�F������
// ================================================================
void C_elm_bgm::init(S_element element, int form, CTSTR& name)
{
	// ���̏�����
	C_tnm_element::init(element, form, name, NULL);

	// �v���C���[���쐬
	m_player_list.resize(TNM_BGM_PLAYER_CNT);

	// �v���C���[��������
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].init();

	// �p�����[�^��������
	m_volume = TNM_VOLUME_MAX;
	m_delay_time = 0;
	m_fade_in_time = 0;
	m_loop_flag = false;
	m_pause_flag = false;

	// ��ƕϐ���������
	m_cur_player_id = -1;
}

// ****************************************************************
// BGM�F���
// ================================================================
void C_elm_bgm::free()
{
	// �v���C���[�����
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].free();

	// �v���C���[���폜
	m_player_list.clear();
}

// ****************************************************************
// BGM�F�ď�����
// ================================================================
void C_elm_bgm::reinit()
{
	// �p�����[�^��������
	m_regist_name.clear();
	m_volume = TNM_VOLUME_MAX;
	m_delay_time = 0;
	m_fade_in_time = 0;
	m_loop_flag = false;
	m_pause_flag = false;

	// �v���C���[���ď�����
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].reinit();

	// �v���C���[�̃{�����[����ݒ�
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].set_game_volume(m_volume, 0);
}

// ****************************************************************
// BGM�F�Z�[�u
// ================================================================
void C_elm_bgm::save(C_tnm_save_stream& stream)
{
	// �p�����[�^���Z�[�u
	stream.save(m_regist_name);
	stream.save(m_volume);
	stream.save(m_delay_time);
	stream.save(m_loop_flag);
	stream.save(m_pause_flag);
}

// ****************************************************************
// BGM�F���[�h
// ================================================================
void C_elm_bgm::load(C_tnm_save_stream& stream)
{
	TSTR regist_name;
	int volume;
	int delay_time;
	bool loop_flag;
	bool ready_flag;

	// �ꎞ�ϐ��Ƀp�����[�^�����[�h
	stream.load(regist_name);
	stream.load(volume);
	stream.load(delay_time);
	stream.load(loop_flag);
	stream.load(ready_flag);

	// �p�����[�^���v���C���[�ɔ��f������
	set_volume(volume);

	// ���[�v�Đ��܂��͏������Ă����ꍇ�́A�Đ��𕜌�����
	if (loop_flag || ready_flag || delay_time > 0)
		play(regist_name, loop_flag, false, 0, 0, TNM_BGM_START_POS_INI, ready_flag, 0);
}

// ****************************************************************
// BGM�F���Ԃ�i�߂�
// ================================================================
void C_elm_bgm::update_time(int past_real_time)
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
// BGM�F�Đ��i�����p�j
// ================================================================
void C_elm_bgm::play(TSTR regist_name, bool loop_flag, bool wait_flag, int fade_in_time, int fade_out_time, int start_pos, bool ready_only, int delay_time)
{
	bool total_ready_only = ready_only;

	// �o�^�����������ɒ���
	str_to_lower(regist_name);

	// �����o�^���̏ꍇ
	if (regist_name == m_regist_name)	{
		// ���[�v�������[�v�w��Ȃ�Đ����s��Ȃ�
		if (m_loop_flag && loop_flag)
			return;
	}

	// ���݂̃v���C���[���t�F�[�h�A�E�g������
	stop(fade_out_time);

	// �҂����Ԃ�����ꍇ�͏����݂̂ɂȂ�
	if (delay_time > 0)	{
		total_ready_only = true;
	}

	// BGM �o�^������ BGM �ԍ����擾
	int bgm_no = tnm_get_bgm_no_by_regist_name(regist_name);

	// �Đ�
	int player_id = (m_cur_player_id + 1) % TNM_BGM_PLAYER_CNT;
	if (!m_player_list[player_id].play_bgm(bgm_no, loop_flag, fade_in_time, start_pos, total_ready_only))
		return;

	// �p�����[�^��ݒ�
	m_cur_player_id = player_id;
	m_regist_name = regist_name;
	m_delay_time = delay_time;
	m_fade_in_time = fade_in_time;
	m_loop_flag = loop_flag;
	m_pause_flag = ready_only;

	// �I���҂��H
	if (wait_flag)
		wait(false, false);		// �I���҂�
}

// ****************************************************************
// BGM�F�Đ�
// ================================================================
void C_elm_bgm::play(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool ready_only, int delay_time)
{
	play(regist_name, true, false, fade_in_time, fade_out_time, start_pos, ready_only, delay_time);
}

// ****************************************************************
// BGM�F�Đ��i�����V���b�g�j
// ================================================================
void C_elm_bgm::play_oneshot(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool ready_only, int delay_time)
{
	play(regist_name, false, false, fade_in_time, fade_out_time, start_pos, ready_only, delay_time);
}

// ****************************************************************
// BGM�F�Đ��i�I���҂��j
// ================================================================
void C_elm_bgm::play_wait(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool ready_only, int delay_time)
{
	play(regist_name, false, true, fade_in_time, fade_out_time, start_pos, ready_only, delay_time);
}

// ****************************************************************
// BGM�F��~
// ================================================================
void C_elm_bgm::stop(int fade_out_time)
{
	// ��~
	if (m_cur_player_id >= 0)
		m_player_list[m_cur_player_id].stop(fade_out_time);

	// �t���O���������i2013/7/2 �o�^�������������܂����Bget_regist_name �΍�B���v���ȁc�H�j
	m_regist_name.clear();
	m_loop_flag = false;
}

// ****************************************************************
// BGM�F�ꎞ��~
// ================================================================
void C_elm_bgm::pause(int fade_out_time)
{
	// ��~
	if (m_cur_player_id >= 0)
		m_player_list[m_cur_player_id].pause(fade_out_time);

	// �|�[�Y�t���O�𗧂Ă�
	m_pause_flag = true;

	// �f�B���C���L�����Z��
	m_delay_time = 0;
}

// ****************************************************************
// BGM�F�ĊJ
// ================================================================
void C_elm_bgm::resume(bool wait_flag, int fade_in_time, int delay_time)
{
	if (delay_time < 0)
		delay_time = 0;

	// �|�[�Y�t���O���~�낷
	m_pause_flag = false;

	// �x���ĊJ�łȂ���΍ĊJ����
	if (delay_time == 0)	{
		if (m_cur_player_id >= 0)
			m_player_list[m_cur_player_id].resume(fade_in_time);

		// �I���҂�
		if (wait_flag)
			wait(false, false);
	}

	// �p�����[�^��ݒ�
	m_delay_time = delay_time;
	m_fade_in_time = fade_in_time;
}

// ****************************************************************
// BGM�F�Đ��I���҂�
// ================================================================
void C_elm_bgm::wait(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_BGM_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// BGM�F�t�F�[�h�A�E�g�I���҂�
// ================================================================
void C_elm_bgm::wait_fade(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_BGM_FADE_OUT_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// BGM�F�Đ����𔻒�
// ================================================================
int C_elm_bgm::check()
{
	if (m_cur_player_id >= 0)
		return m_player_list[m_cur_player_id].get_play_state();

	return TNM_PLAYER_STATE_FREE;
}

// ****************************************************************
// BGM�F�Đ������ǂ����𔻒�
// ================================================================
bool C_elm_bgm::is_playing()
{
	if (m_cur_player_id >= 0)	{
		if (m_player_list[m_cur_player_id].is_playing())
			return true;
	}

	return false;
}

// ****************************************************************
// BGM�F�t�F�[�h�A�E�g�����ǂ����𔻒�
// ================================================================
bool C_elm_bgm::is_fade_out_doing()
{
	if (m_cur_player_id >= 0)	{
		if (m_player_list[m_cur_player_id].is_fade_out())
			return true;
	}

	return false;
}

// ****************************************************************
// BGM�F�{�����[����ݒ�^�擾
// ================================================================
void C_elm_bgm::set_volume(int volume, int fade_time)
{
	// �v���C���[�̃Q�[���{�����[����ݒ�
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].set_game_volume(volume, fade_time);

	// �p�����[�^��ݒ�
	m_volume = volume;
}

void C_elm_bgm::set_volume_max(int fade_time)
{
	set_volume(TNM_VOLUME_MAX, fade_time);
}

void C_elm_bgm::set_volume_min(int fade_time)
{
	set_volume(TNM_VOLUME_MIN, fade_time);
}

// ****************************************************************
// BGM�F�V�X�e���{�����[����ݒ�
// ================================================================
void C_elm_bgm::set_system_volume(int volume)
{
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		m_player_list[i].set_system_volume(volume);
	}
}

// ****************************************************************
// BGM�F�Đ��ʒu���擾
// ================================================================
int C_elm_bgm::get_play_pos()
{
	if (m_cur_player_id >= 0)	{
		return m_player_list[m_cur_player_id].get_play_pos();
	}
	else	{
		return 0;
	}
}

// ****************************************************************
// BGM�F�v���C���[���擾
// ================================================================
C_tnm_player* C_elm_bgm::get_player(int player_no)
{
	if (player_no < 0 || (int)m_player_list.size() <= player_no)
		return NULL;

	// �p�����[�^���擾
	return &m_player_list[player_no];
}
