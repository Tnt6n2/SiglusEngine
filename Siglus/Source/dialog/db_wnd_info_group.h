#pragma		once

#include	"db_wnd_info_listview.h"

class	C_elm_group_list;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�O���[�v
// ================================================================
class C_db_wnd_info_group : public C_db_wnd_info_listview
{
public:
	void		init();				// ������
	void		frame(C_elm_group_list** p_group_list, int group_list_cnt);				// �t���[��

private:
	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

