#pragma		once

// ****************************************************************
// �v���C���[��
// ================================================================
const int	TNM_BGM_PLAYER_CNT = 2;		// �N���X�t�F�[�h�p
const int	TNM_KOE_PLAYER_CNT = 1;		// �P��v���C���[
const int	TNM_PCM_PLAYER_CNT = 16;	// �ėp�v���C���[
const int	TNM_SE_PLAYER_CNT = 16;		// �ėp�v���C���[

// ****************************************************************
// �v���C���[�^�C�v
// ================================================================
const int	TNM_PLAYER_TYPE_NONE = -1;
const int	TNM_PLAYER_TYPE_BGM = 0;
const int	TNM_PLAYER_TYPE_KOE = 1;
const int	TNM_PLAYER_TYPE_PCM = 2;
const int	TNM_PLAYER_TYPE_SE = 3;

// ****************************************************************
// �v���C���[�̏��
// ================================================================
const int	TNM_PLAYER_STATE_FREE = 0;
const int	TNM_PLAYER_STATE_PLAY = 1;
const int	TNM_PLAYER_STATE_FADE_OUT = 2;
const int	TNM_PLAYER_STATE_PAUSE = 3;

// ****************************************************************
// �T�E���h�^�C�v
// ================================================================
const int	TNM_SOUND_TYPE_NONE = -1;
const int	TNM_SOUND_TYPE_BGM = 0;
const int	TNM_SOUND_TYPE_KOE = 1;
const int	TNM_SOUND_TYPE_PCM = 2;
const int	TNM_SOUND_TYPE_SE = 3;

// ****************************************************************
// �{�����[���`�����l��
// ================================================================
const int	TNM_PLAYER_VOLUME_CHANNEL_SYSTEM = 0;	// �V�X�e���Őݒ肷��{�����[��
const int	TNM_PLAYER_VOLUME_CHANNEL_GAME = 1;		// �X�N���v�g�Őݒ肷��{�����[��

// ****************************************************************
// �{�����[��
// ================================================================
const int	TNM_VOLUME_MAX = 255;
const int	TNM_VOLUME_MIN = 0;

// ****************************************************************
// �v���C���[
// ================================================================
//		�ƂȂܐ�p�̃v���C���[�B
//		BGM, KOE, PCM, SE �Ȃǂ͂��ׂāA���̃v���C���[�ŉ����Đ����܂��B
// ================================================================
class C_tnm_player
{
public:
	void	init();					// ������
	void	reinit();				// �ď�����
	void	free();					// ���

	bool	play_bgm(int bgm_no, bool loop_flag, int fade_in_time, int start_pos, bool ready_only);
	bool	play_koe(int koe_no, bool loop_flag, int jitan_rate, int play_pos_mills, bool ready_only, bool check_only);
	bool	play_pcm(TSTR file_name, bool loop_flag, int fade_in_time, bool ready_only);
	bool	play_se(int se_no, bool ready_only);
	void	stop(int fade_time = 100);
	void	pause(int fade_time = 100);
	void	resume(int fade_time = 100);
	void	release_sound();

	void	set_system_volume(int volume);
	int		get_system_volume();
	void	set_game_volume(int volume, int fade_time);
	int		get_game_volume();

	bool	is_playing()		{	return m_player.is_playing() && !m_player.is_fade_out() && !m_player.is_pausing();	}	// �Đ���
	bool	is_fade_out()		{	return m_player.is_fade_out();			}	// �t�F�[�h�A�E�g��
	bool	is_pausing()		{	return m_player.is_pausing();			}	// �ꎞ��~��
	bool	is_using()			{	return m_player.is_playing();			}	// �Đ����܂��̓t�F�[�h�A�E�g��

	int		get_length();					// �T���v�����Ŏ擾
	int		get_length_by_millsecond();		// �ؕb�Ŏ擾

	int		get_play_pos();								// �v���C�ʒu���擾�i�T���v���ʒu�Ŏw��j
	void	set_play_pos_by_sample(int sample_no);		// �v���C�ʒu��ݒ�i�T���v���ʒu�Ŏw��j
	int		get_play_pos_by_millsecond();				// �v���C�ʒu���擾�i�ؕb�Ŏw��j
	void	set_play_pos_by_millsecond(int mills);		// �v���C�ʒu��ݒ�i�ؕb�Ŏw��j

	int		get_play_state()
	{
		if (!m_player.is_playing())		return TNM_PLAYER_STATE_FREE;
		if (m_player.is_fade_out())		return TNM_PLAYER_STATE_FADE_OUT;
		return TNM_PLAYER_STATE_PLAY;
	}

	int		get_play_state_2()
	{
		if (!m_player.is_playing())		return TNM_PLAYER_STATE_FREE;
		if (m_player.is_fade_out())		return TNM_PLAYER_STATE_FADE_OUT;
		if (m_player.is_pausing())		return TNM_PLAYER_STATE_PAUSE;
		return TNM_PLAYER_STATE_PLAY;
	}

	int		get_sound_type()	{	return m_sound_type;	}
	int		get_bgm_no()		{	return m_bgm_no;		}
	int		get_koe_no()		{	return m_koe_no;		}
	TSTR	get_file_name()		{	return m_file_name;		}
	int		get_se_no()			{	return m_se_no;			}

	C_sound_stream_base* get_stream()
	{
		return m_player.get_stream();
	}
	float	get_current_mouth_volume();

private:
	bool	__play_wav(TSTR base_dir, TSTR sub_dir, TSTR file_name, bool loop, int fade_time, bool ready_only, bool check_only);
	bool	__play_wav(TSTR base_dir, TSTR sub_dir, TSTR file_name, bool loop, int start_pos, int end_pos, int repeat_pos, int fade_time, bool ready_only, bool check_only);
	void	__set_volume(int volume_channel_no, int volume, int fade_time);
	int		__get_volume(int volume_channel_no);

	C_player	m_player;		// �v���C���[

	int		m_sound_type;	// �T�E���h�^�C�v
	int		m_bgm_no;		// �Đ����� BGM �̓o�^�ԍ�
	int		m_koe_no;		// �Đ����� KOE �̐��ԍ�
	TSTR	m_file_name;	// �Đ����� PCM �̃t�@�C����
	int		m_se_no;		// �Đ����� SE �̓o�^�ԍ�

	ARRAY<float>	m_mouth_volume_table;
};

