#pragma		once

#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�J�E���^
// ================================================================
class C_db_wnd_info_counter : public C_db_wnd_info_listview
{
public:
	void		init();		// ������
	void		frame();	// �t���[��

private:
	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

