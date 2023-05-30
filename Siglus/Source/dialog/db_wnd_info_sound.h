#pragma		once

#include	"db_wnd_info_listview.h"

class	C_tnm_ini;
class	C_elm_sound;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�T�E���h
// ================================================================
class C_db_wnd_info_sound : public C_db_wnd_info_listview
{
public:
	void		init();		// ������
	void		frame();	// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		struct ITEM
		{
			int		type;
			int		player_no;
			TSTR	file_name;
			int		volume;
			int		delay_time;
			int		state;
			int		state2;
			int		option;
			bool	pause_flag;
			bool	bgmfade_target;
			bool	bgmfade2_target;
		};

		ARRAY<ITEM>	item_list;
		
	}	m_ld;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

