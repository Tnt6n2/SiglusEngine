#pragma		once

#include	"db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�f�B���N�g�����
// ================================================================
class C_db_wnd_info_dir : public C_db_wnd_info_listview
{
public:
	void		init();		// ������
	void		frame();	// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		int		setup_type;
		TSTR	exe_dir;
		TSTR	base_dir;
		TSTR	work_dir;
		TSTR	save_dir;
	}	m_ld;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

