#pragma		once

#include	"db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�v���Z�X�X�^�b�N
// ================================================================
class C_db_wnd_info_proc_stack : public C_db_wnd_info_listview
{
public:
	void		init();		// ������
	void		frame();	// �t���[��

private:

	// �A�C�e��
	struct	ITEM
	{
		TNM_PROC_TYPE		proc;
	};

	// �X�e�[�^�X
	struct STATE
	{
		ARRAY<ITEM>	item_list;
	}	m_ld;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

