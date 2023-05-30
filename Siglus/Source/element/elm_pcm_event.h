#pragma		once

#include	"data/tnm_element.h"
#include	"element/elm_list.h"

class	C_tnm_save_stream;

// ****************************************************************
// ���ʉ��C�x���g
// ================================================================
const int	TNM_PCM_EVENT_TYPE_NONE = -1;
const int	TNM_PCM_EVENT_TYPE_ONESHOT = 0;
const int	TNM_PCM_EVENT_TYPE_LOOP = 1;
const int	TNM_PCM_EVENT_TYPE_RANDOM = 2;

// ****************************************************************
// ���ʉ��C�x���g�F���C���f�[�^
// ================================================================
struct S_tnm_pcm_event_line
{
	TSTR	file_name;
	int		min_time;
	int		max_time;
	int		probability;
};

// ****************************************************************
// ���ʉ��C�x���g
// ================================================================
class C_elm_pcm_event : public C_tnm_element
{
public:
	void	init(S_element element, int form, CTSTR& name);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);
	void	frame(int past_game_time, int past_real_time);

	// �R�}���h
	void	start_oneshot(int pcm_buf_no, int volume_type, int chara_no, bool bgm_fade_target, bool bgm_fade2_target, bool bgm_fade2_source, bool real_flag, bool time_type);
	void	start_loop(int pcm_buf_no, int volume_type, int chara_no, bool bgm_fade_target, bool bgm_fade2_target, bool bgm_fade2_source, bool real_flag, bool time_type);
	void	start_random(int pcm_buf_no, int volume_type, int chara_no, bool bgm_fade_target, bool bgm_fade2_target, bool bgm_fade2_source, bool real_flag, bool time_type);
	void	stop(bool pcm_stop_flag);
	bool	check();
	void	wait();
	void	wait_key();

	// �p�����[�^
	int		get_type()			{	return m_type;			}
	int		get_pcm_buf_no()	{	return m_pcm_buf_no;	}
	int		get_cur_time()		{	return m_cur_time;		}
	int		get_cur_line_no()	{	return m_cur_line_no;	}
	int		get_next_time()		{	return m_next_time;		}

	ARRAY<S_tnm_pcm_event_line>&	line_list()		{	return m_line_list;	}

private:

	// �p�����[�^
	int							m_type;
	int							m_pcm_buf_no;
	int							m_volume_type;
	int							m_chara_no;
	bool						m_bgm_fade_target;
	bool						m_bgm_fade2_target;
	bool						m_bgm_fade_source;
	bool						m_real_flag;
	bool						m_time_type;
	ARRAY<S_tnm_pcm_event_line>	m_line_list;

	// ��ƕϐ�
	int							m_cur_time;
	int							m_cur_line_no;
	int							m_next_time;
	int							m_last_line_no;
};

// ****************************************************************
// ���ʉ��C�x���g���X�g
// ================================================================
class C_elm_pcm_event_list : public C_elm_list<C_elm_pcm_event>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};
