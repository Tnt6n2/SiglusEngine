#pragma		once

#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�a�f�l�e�[�u��
// ================================================================
class C_db_wnd_info_bgm_table : public C_db_wnd_info_listview
{
public:
	void		init();		// ������
	void		frame();	// �t���[��

	// �X�e�[�^�X
	struct STATE
	{
		ARRAY<int>	flag_list;
	}	m_ld;

private:
	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

