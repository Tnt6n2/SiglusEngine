#pragma		once

#include	"db_wnd_info_listview.h"

class	C_tnm_input;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F����
// ================================================================
class C_db_wnd_info_input : public C_db_wnd_info_listview
{
public:

	void		init();		// ������
	void		frame();	// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		C_point	mouse_pos;
		int		mouse_left_down;
		int		mouse_left_up;
		int		mouse_left_down_up;
		int		mouse_left_state;
		int		mouse_right_down;
		int		mouse_right_up;
		int		mouse_right_down_up;
		int		mouse_right_state;
		C_point	script_mouse_pos;
		int		script_mouse_left_down;
		int		script_mouse_left_up;
		int		script_mouse_left_down_up;
		int		script_mouse_left_state;
		int		script_mouse_right_down;
		int		script_mouse_right_up;
		int		script_mouse_right_down_up;
		int		script_mouse_right_state;

	}	m_ld;

	// ��ƕϐ�
	struct WORK
	{
	}	work;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

