#include	"pch.h"

#include	"data/tnm_def.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// PCM�F������
// ================================================================
void C_elm_pcm::init(S_element element, int form, CTSTR& name, int player_type)
{
	// ���̏�����
	C_tnm_element::init(element, form, name, NULL);

	// �Œ�p�����[�^�̏�����
	m_player_type = player_type;

	// �v���C���[���쐬
	switch (player_type)	{
		//case TNM_PLAYER_TYPE_BGM:	m_player_list.resize(TNM_BGM_PLAYER_CNT);	break;
		//case TNM_PLAYER_TYPE_KOE:	m_player_list.resize(TNM_KOE_PLAYER_CNT);	break;
		case TNM_PLAYER_TYPE_PCM:	m_player_list.resize(TNM_PCM_PLAYER_CNT);	break;
		case TNM_PLAYER_TYPE_SE:	m_player_list.resize(TNM_SE_PLAYER_CNT);	break;
	}

	// �v���C���[��������
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].init();

	// �p�����[�^��������
	m_next_player_id = 0;
}

// ****************************************************************
// PCM�F���
// ================================================================
void C_elm_pcm::free()
{
	// �v���C���[�����
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].free();

	// �v���C���[���폜
	m_player_list.clear();
}

// ****************************************************************
// PCM�F�ď�����
// ================================================================
void C_elm_pcm::reinit()
{
	// �p�����[�^��������
	m_volume = TNM_VOLUME_MAX;

	// �v���C���[���ď�����
	if (m_player_type == TNM_PLAYER_TYPE_SE)	{
		// SE �̏ꍇ�͏��������Ȃ��I
	}
	else	{
		for (int i = 0; i < (int)m_player_list.size(); i++)
			m_player_list[i].reinit();
	}

	// �v���C���[�̃{�����[����ݒ�
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].set_game_volume(m_volume, 0);
}

// ****************************************************************
// PCM�F�Z�[�u
// ================================================================
void C_elm_pcm::save(C_tnm_save_stream& stream)
{
	// �p�����[�^���Z�[�u
	stream.save(m_volume);
}

// ****************************************************************
// PCM�F���[�h
// ================================================================
void C_elm_pcm::load(C_tnm_save_stream& stream)
{
	// �p�����[�^�����[�h
	stream.load(m_volume);
}

// ****************************************************************
// PCM�F�����Đ�
// ================================================================
void C_elm_pcm::play_koe(int koe_no, int* p_used_player_no)
{
	// �󂢂Ă���v���C���[��T��
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		if (!m_player_list[m_next_player_id].is_using())
			break;	// �Đ����łȂ��v���C���[�����������̂Ŕ�����I

		// ���̃v���C���[��
		m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
	}

	// �Đ�
	m_player_list[m_next_player_id].play_koe(koe_no, false, TNM_JITAN_RATE_NORMAL, 0, false, false);
	if (p_used_player_no)	{
		*p_used_player_no = m_next_player_id;
	}

	// ���̃v���C���[��
	m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
}

// ****************************************************************
// PCM�F���ʉ����Đ�
// ================================================================
void C_elm_pcm::play_pcm(TSTR file_name, int* p_used_player_no)
{
	// �󂢂Ă���v���C���[��T��
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		if (!m_player_list[m_next_player_id].is_using())
			break;	// �Đ����łȂ��v���C���[�����������̂Ŕ�����I

		// ���̃v���C���[��
		m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
	}

	// �Đ�
	m_player_list[m_next_player_id].play_pcm(file_name, false, 0, false);
	if (p_used_player_no)	{
		*p_used_player_no = m_next_player_id;
	}

	// ���̃v���C���[��
	m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
}

// ****************************************************************
// PCM�F�V�X�e�������Đ�
// ================================================================
void C_elm_pcm::play_se(int se_no, int* p_used_player_no)
{
	// �󂢂Ă���v���C���[��T��
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		if (!m_player_list[m_next_player_id].is_using())
			break;	// �Đ����łȂ��v���C���[�����������̂Ŕ�����I

		// ���̃v���C���[��
		m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
	}

	// �Đ�
	m_player_list[m_next_player_id].play_se(se_no, false);
	if (p_used_player_no)	{
		*p_used_player_no = m_next_player_id;
	}

	// ���̃v���C���[��
	m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
}

// ****************************************************************
// PCM�F��~
// ================================================================
void C_elm_pcm::stop(int fade_out_time)
{
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].stop(fade_out_time);
}

// ****************************************************************
// PCM�F�Đ��I���҂�
// ================================================================
void C_elm_pcm::wait(bool is_key_wait)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_PCM_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = is_key_wait;
	tnm_push_proc(proc);
}

// ****************************************************************
// PCM�F�Đ�������
// ================================================================
int C_elm_pcm::check()
{
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		if (m_player_list[i].get_play_state() == TNM_PLAYER_STATE_PLAY)
			return TNM_PLAYER_STATE_PLAY;
	}
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		if (m_player_list[i].get_play_state() == TNM_PLAYER_STATE_FADE_OUT)
			return TNM_PLAYER_STATE_FADE_OUT;
	}

	return TNM_PLAYER_STATE_FREE;
}

// ****************************************************************
// PCM�F�{�����[����ݒ�^�擾
// ================================================================
void C_elm_pcm::set_volume(int volume, int fade_time)
{
	// �e�v���C���[�̃Q�[���{�����[����ݒ�
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].set_game_volume(volume, fade_time);

	// �p�����[�^��ݒ�
	m_volume = volume;
}

void C_elm_pcm::set_volume_max(int fade_time)
{
	set_volume(TNM_VOLUME_MAX, fade_time);
}

void C_elm_pcm::set_volume_min(int fade_time)
{
	set_volume(TNM_VOLUME_MIN, fade_time);
}

// ****************************************************************
// PCM�F�V�X�e���{�����[����ݒ�
// ================================================================
void C_elm_pcm::set_system_volume(int volume)
{
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		m_player_list[i].set_system_volume(volume);
	}
}

// ****************************************************************
// PCM�F�v���C���[���擾
// ================================================================
C_tnm_player* C_elm_pcm::get_player(int player_no)
{
	if (player_no < 0 || (int)m_player_list.size() <= player_no)
		return NULL;

	// �p�����[�^���擾
	return &m_player_list[player_no];
}
