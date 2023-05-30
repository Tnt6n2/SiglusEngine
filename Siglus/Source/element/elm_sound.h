#pragma		once

class		C_tnm_save_stream;

#include	"data/tnm_element.h"

#include	"element/elm_list.h"
#include	"element/elm_sound_player.h"
#include	"element/elm_mov.h"

// ****************************************************************
// BGM
// ================================================================
//		�Q�̃v���C���[���g���ăN���X�t�F�[�h���s�����Ƃ��o���܂��B
// ================================================================
class C_elm_bgm : public C_tnm_element
{
public:
	void	init(S_element element, int form, CTSTR& name);		// ������
	void	reinit();											// �ď�����
	void	free();												// ���
	void	save(C_tnm_save_stream& stream);					// �Z�[�u
	void	load(C_tnm_save_stream& stream);					// ���[�h
	void	update_time(int past_real_time);					// ���Ԃ�i�߂�
	
	// �R�}���h
	void	play(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool pause_flag, int delay_time);				// �Đ�
	void	play_oneshot(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool pause_flag, int delay_time);		// �Đ�
	void	play_wait(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool pause_flag, int delay_time);			// �Đ�
	void	stop(int fade_out_time = 100);								// ��~
	void	pause(int fade_out_time = 100);								// �ꎞ��~
	void	resume(bool wait_flag, int fade_in_time, int delay_time);	// �ĊJ
	void	wait(bool key_skip_flag, bool return_value_flag);			// �E�F�C�g
	void	wait_fade(bool key_skip_flag, bool return_value_flag);		// �t�F�[�h�E�F�C�g
	int		check();
	void	set_volume(int volume, int fade_time = 0);					// �{�����[���ݒ�
	void	set_volume_max(int fade_time = 0);							// �{�����[���ݒ�i�ő�j
	void	set_volume_min(int fade_time = 0);							// �{�����[���ݒ�i�ŏ��j
	void	set_system_volume(int volume);								// �V�X�e���{�����[����ݒ�
	int		get_play_pos();												// �v���C�ʒu���擾

	// ���擾
	int		get_player_cnt()	{ return (int)m_player_list.size(); }
	TSTR	get_regist_name()	{ return m_regist_name; }
	int		get_volume()		{ return m_volume; }
	bool	is_loop()			{ return m_loop_flag; }
	bool	is_playing();
	bool	is_fade_out_doing();
	bool	is_pause()			{ return m_pause_flag; }
	int		get_delay_time()	{ return m_delay_time; }

	C_tnm_player*		get_player(int player_no);

private:
	void	play(TSTR regist_name, bool loop_flag, bool wait_flag, int fade_in_time, int fade_out_time, int start_pos, bool pause_flag, int delay_time);		// �Đ�

	// �p�����[�^
	TSTR				m_regist_name;		// BGM �̓o�^��
	int					m_volume;			// �{�����[��
	int					m_delay_time;		// �x������
	int					m_fade_in_time;		// �t�F�[�h�C������
	bool				m_loop_flag;		// ���[�v�t���O
	bool				m_pause_flag;		// �|�[�Y�t���O

	// ��ƕϐ�
	ARRAY<C_tnm_player>	m_player_list;		// �v���C���[�i�N���X�t�F�[�h�̂��߂ɕ����K�v�ł��j
	int					m_cur_player_id;	// ���ݎg�p���̃v���C���[�h�c
};

// ****************************************************************
// KOE
// ================================================================
class C_elm_koe : public C_tnm_element
{
public:
	void	init(S_element element, int form, CTSTR& name);		// ������
	void	reinit();											// �ď�����
	void	free();												// ���
	void	save(C_tnm_save_stream& stream);					// �Z�[�u
	void	load(C_tnm_save_stream& stream);					// ���[�h
	
	// �R�}���h
	void	play(int koe_no, int chara_no, int jitan_rate, int play_pos_mills, bool wait_flag, bool key_skip_flag, bool return_value_flag, bool ex_koe_flag, bool check_only);			// �Đ�
	void	stop(int fade_time = 100);							// ��~
	void	wait(bool key_wait_flag, bool return_value_flag);	// �E�F�C�g
	int		check();											// �`�F�b�N
	void	set_volume(int volume, int fade_time = 0);			// �{�����[���ݒ�
	void	set_volume_max(int fade_time = 0);					// �{�����[���ݒ�i�ő�j
	void	set_volume_min(int fade_time = 0);					// �{�����[���ݒ�i�ŏ��j
	void	set_system_volume(int volume);						// �V�X�e���{�����[����ݒ�

	// ���擾
	bool	is_playing()		{	return check() == TNM_PLAYER_STATE_PLAY;											}
	bool	is_using()			{	return check() == TNM_PLAYER_STATE_PLAY || check() == TNM_PLAYER_STATE_FADE_OUT;	}
	int		get_player_cnt()	{	return (int)m_player_list.size();	}
	int		get_koe_no()		{	return m_koe_no;					}
	int		get_chara_no()		{	return m_chara_no;					}
	int		get_volume()		{	return m_volume;					}
	bool	get_ex_koe_flag()	{	return m_ex_koe_flag;				}

	int		get_length();
	int		get_length_by_millsecond();
	int		get_play_pos();
	int		get_play_pos_by_millsecond();
	float	get_current_mouth_volume();

	C_tnm_player*				get_player(int player_no);
	C_sound_stream_base*		get_stream(int player_no);


private:

	ARRAY<C_tnm_player>	m_player_list;		// �v���C���[
	int					m_koe_no;			// ���ԍ�
	int					m_chara_no;			// �L�����ԍ�
	int					m_volume;			// �{�����[��
	bool				m_ex_koe_flag;		// exkoe �t���O
};

// ****************************************************************
// PCM
// ================================================================
//		PCM ���Ǘ�����N���X�ł��B
//		�ėp�v���C���[���g���� PCM ���Đ����܂��B
// ================================================================
class C_elm_pcm : public C_tnm_element
{
public:
	// �V�X�e��
	void	init(S_element element, int form, CTSTR& name, int player_type);	// ������
	void	reinit();							// �ď�����
	void	free();								// ���
	void	save(C_tnm_save_stream& stream);	// �Z�[�u
	void	load(C_tnm_save_stream& stream);	// ���[�h

	// �R�}���h
	void	play_koe(int koe_no, int* p_used_player_no);		// �����Đ�
	void	play_pcm(TSTR file_name, int* p_used_player_no);	// ���ʉ����Đ�
	void	play_se(int se_no, int* p_used_player_no);			// �V�X�e�������Đ�
	void	stop(int fade_out_time = 100);		// ��~
	void	wait(bool is_key_wait);				// �Đ��I���҂�
	int		check();
	void	set_volume(int volume, int fade_time = 0);
	void	set_volume_max(int fade_time = 0);
	void	set_volume_min(int fade_time = 0);
	void	set_system_volume(int volume);

	// ���擾
	bool	is_playing()		{	return check() == TNM_PLAYER_STATE_PLAY;											}
	bool	is_pausing()		{	return check() == TNM_PLAYER_STATE_PAUSE;											}
	bool	is_using()			{	return check() == TNM_PLAYER_STATE_PLAY || check() == TNM_PLAYER_STATE_FADE_OUT;	}
	int		get_player_cnt()	{	return (int)m_player_list.size();		}
	int		get_volume()		{	return m_volume;						}

	C_tnm_player*		get_player(int player_no);

private:

	// �Œ�p�����[�^
	int					m_player_type;

	// �ʏ�p�����[�^
	int					m_volume;

	// �ꎞ�p�����[�^
	ARRAY<C_tnm_player>	m_player_list;
	int					m_next_player_id;
};

// ****************************************************************
// PCMCH
// ================================================================
//		PCMCH �𗘗p���� PCM �Đ����s���܂��B
// ================================================================
class C_elm_pcmch : public C_tnm_element
{
public:
	// �V�X�e��
	void	init(S_element element, int form, CTSTR& name);	// ������
	void	reinit();										// �ď�����
	void	free();											// ���
	void	save(C_tnm_save_stream& stream);				// �Z�[�u
	void	load(C_tnm_save_stream& stream);				// ���[�h
	void	update_time(int past_real_time);				// ���Ԃ�i�߂�

	// �R�}���h
	void	play_pcm(TSTR file_name, bool loop_flag, bool wait_flag, int fade_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only);
	void	play_bgm(TSTR regist_name, bool loop_flag, bool wait_flag, int fade_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only);
	void	play_koe(int koe_no, bool loop_flag, bool wait_flag, int fade_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only);
	void	play_se(int se_no, bool loop_flag, bool wait_flag, int fade_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only);
	void	stop(int fade_time = 100);
	void	pause(int fade_time = 100);
	void	resume(bool wait_flag, int fade_time, int delay_time);
	void	wait(bool key_skip_flag, bool return_value_flag);
	void	wait_fade(bool key_skip_flag, bool return_value_flag);
	int		check()				{	return m_player.get_play_state();	}
	void	set_volume(int volume, int fade_time = 0);
	void	set_volume_max(int fade_time = 0);
	void	set_volume_min(int fade_time = 0);
	void	set_system_volume(int volume);			// �V�X�e���{�����[����ݒ�
	int		get_system_volume();					// �V�X�e���{�����[�����擾

	// ���擾
	TSTR	get_pcm_name()			{	return m_pcm_name;					}
	TSTR	get_bgm_name()			{	return m_bgm_name;					}
	int		get_koe_no()			{	return m_koe_no;					}
	int		get_se_no()				{	return m_se_no;						}
	int		get_volume_type()		{	return m_volume_type;				}
	int		get_volume()			{	return m_volume;					}
	int		get_chara_no()			{	return m_chara_no;					}
	bool	is_loop()				{	return m_loop_flag;					}
	bool	is_bgm_fade_target()	{	return m_bgm_fade_target_flag;		}
	bool	is_bgm_fade2_target()	{	return m_bgm_fade2_target_flag;		}
	bool	is_bgm_fade_source()	{	return m_bgm_fade_source_flag;		}
	bool	is_playing();
	bool	is_fade_out_doing();

	// �v���C���[�����擾
	int		get_length_by_millsecond()	{ return m_player.get_length_by_millsecond(); }
	int		get_play_state_2()			{ return m_player.get_play_state_2(); }

private:

	// �ʏ�p�����[�^
	TSTR			m_pcm_name;					// ���ʉ���
	TSTR			m_bgm_name;					// �a�f�l��
	int				m_koe_no;					// ���ԍ�
	int				m_se_no;					// �V�X�e�����ԍ�
	int				m_volume_type;				// �{�����[���^�C�v
	int				m_chara_no;					// �L�����N�^����
	int				m_volume;					// �{�����[��
	int				m_delay_time;				// �x������
	int				m_fade_in_time;				// �t�F�[�h�C������
	bool			m_loop_flag;				// ���[�v�t���O
	bool			m_bgm_fade_target_flag;		// �a�f�l�t�F�[�h�̃^�[�Q�b�g�ƂȂ�
	bool			m_bgm_fade2_target_flag;	// �a�f�l�t�F�[�h�̃^�[�Q�b�g�ƂȂ�
	bool			m_bgm_fade_source_flag;		// �a�f�l�t�F�[�h�̃\�[�X�ƂȂ�
	bool			m_ready_flag;				// �����t���O

	// �ꎞ�p�����[�^
	C_tnm_player		m_player;			// �v���C���[
};

// ****************************************************************
// PCMCH ���X�g
// ================================================================
class C_elm_pcmch_list : public C_elm_list<C_elm_pcmch>
{

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

// ****************************************************************
// SE
// ================================================================
//		SE �̍Đ����Ǘ����܂��B
//		�ėp�� SE �v���C���[���g���� SE ���Đ����܂��B
// ================================================================
class C_elm_se : public C_elm_pcm
{
};

// ****************************************************************
// �T�E���h
// ================================================================
class C_elm_sound
{
public:
	void	init();								// ������
	void	reinit();							// �ď�����
	void	free();								// ���
	void	save(C_tnm_save_stream& stream);	// �Z�[�u
	void	load(C_tnm_save_stream& stream);	// ���[�h
	void	frame();							// �t���[������

public:

	// �q�G�������g
	C_elm_bgm			m_bgm;				// BGM
	C_elm_koe			m_koe;				// KOE
	C_elm_pcm			m_pcm;				// PCM
	C_elm_pcmch_list	m_pcmch_list;		// PCMCH
	C_elm_se			m_se;				// SE
	C_elm_mov			m_mov;				// MOV

	// �T�|�[�g�p�̖����T�E���h
	BSP<C_sound>					m_muon_sound;
	BSP<C_sound_stream>				m_muon_stream;
	BSP<C_player>					m_muon_player;
};


