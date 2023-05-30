#pragma		once

#include	"data/tnm_element.h"

class	C_tnm_save_stream;

// ****************************************************************
// ���[�r�[�F���
// ================================================================
enum TNM_MOVIE_PLAY_STATE
{
	TNM_MOVIE_PLAY_STATE_NONE = -1,
	TNM_MOVIE_PLAY_STATE_NOT_READY,
	TNM_MOVIE_PLAY_STATE_STOP,
	TNM_MOVIE_PLAY_STATE_PLAY,
	TNM_MOVIE_PLAY_STATE_PAUSE,
};

// ****************************************************************
// ���[�r�[�F�p�����[�^
// ================================================================
struct S_tnm_mov_param
{
	TSTR	file_name;
};

class C_elm_mov_param
{
public:
	static const C_size DEF_MOV_SIZE;

	C_elm_mov_param();
	C_elm_mov_param(const C_point& pos, const C_size& size);

	void	add_pos(const C_point& add);
	void	add_pos(int x, int y);
	void	add_size(const C_size& add);
	void	add_size(int x, int y);

	// �A�N�Z�T���\�b�h
	void	set_pos(const C_point& pos);
	void	set_pos(int x, int y);
	void	set_size(const C_size& size);
	void	set_size(int x, int y);
	void	set_pos_and_size(const C_point& pos, const C_size& size);
	void	set_pos_and_size(int px, int py, int sx, int sy);
	void	set_size_rate(float rate_x, float rate_y);
	void	set_total_game_screen_pos(const C_point& pos);

	C_point	get_pos();
	C_size	get_size();
	D3DXVECTOR2	get_size_rate();
	C_point	get_calc_pos();
	C_size	get_calc_size();

private:
	C_point		m_pos;
	C_size		m_size;

	D3DXVECTOR2	m_size_rate;
	C_point		m_total_game_screen_pos;
};

// ****************************************************************
// ���[�r�[
// ================================================================
class C_elm_mov : public C_tnm_element
{
public:

	void	init(S_element element, int form, CTSTR& name);		// ������
	void	reinit();							// �ď�����
	void	free();								// ���
	void	save(C_tnm_save_stream& stream);	// �Z�[�u
	void	load(C_tnm_save_stream& stream);	// ���[�h
	void	frame();							// �t���[������

	void	play(CTSTR& file_name);
	void	play(CTSTR& file_name, int px, int py, int sx, int sy);
	void	close();
//	void	update_rect();					// ��`���X�V
	void	update_rect_org();
	void	change_rect(const C_point& pos, const C_size& size);	// ��`��ύX
	void	add_rect(const C_point& pos, const C_size& size);		// ��`��ύX
	void	set_size_rate(float x, float y);
	void	set_total_game_screen_pos(const C_point& pos);
	void	set_volume(int volume);
	bool	is_playing();
	bool	is_readying();

	int		get_wmp_state_detail();
	int		get_volume();
	int		get_volume_real();


	void	pause();		// ���f
	void	resume();		// �ĊJ

	void	create_file_playing();
	void	delete_file_playing();
	bool	check_file_playing();

	TSTR	get_file_name()	{	return m_param.file_name;	}
	int		get_state();

private:

	int				m_method;
	int				m_volume;
	bool			m_wmp_ready;
	S_tnm_mov_param	m_param;
	C_elm_mov_param	m_emp;

	void	restruct();
	int		convert_play_state(int state);
	int		convert_play_state(CTSTR& state);
};

// ****************************************************************
// ���[�r�[�E�B���h�E
// ================================================================
class C_tnm_movie_wnd : public C_window
{
public:
	bool		create();

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);

private:
	bool		on_paint();

};
