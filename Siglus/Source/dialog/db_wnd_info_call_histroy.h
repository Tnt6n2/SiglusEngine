#pragma		once

#include	"db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�Ăяo������
// ================================================================
class C_db_wnd_info_call_histroy : public C_db_wnd_info_listview
{
public:
	void		init();		// ������
	void		frame();	// �t���[��

private:

	// �A�C�e��
	struct	ITEM
	{
		int		call_type;
		bool	excall_flag;
		bool	frame_action_flag;
		int		src_scn;
		int		dst_scn;
		int		dst_label;
	};

	// �X�e�[�^�X
	struct STATE
	{
		ARRAY<ITEM>	item_list;
	}	m_ld;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

