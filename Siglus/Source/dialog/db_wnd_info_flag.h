#pragma		once

#include	"dialog/db_wnd_info_listview.h"

class	C_elm_int_list;
class	C_elm_str_list;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�t���O
// ================================================================
class C_db_wnd_info_flag : public C_db_wnd_info_listview
{
public:
	void	init();		// ������
	void	frame();	// �t���[��

	void	set_page_no(int page_no)	{	m_page_no = page_no;	}

private:

	// �X�e�[�^�X
	struct STATE
	{
		ARRAY<int>	int_flag;
		ARRAY<TSTR>	str_flag;
	}	m_ld;

	void	on_init_func();		// �������t�@���N�V����
	void	on_close_func();	// ����t�@���N�V����

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm

	// ���X�g�r���[���_�u���N���b�N����
	void	on_dblclk_lv();

	// ���X�g�r���[�̒l��ύX����
	void	on_change_lv_value(int item_no, int column_no, CTSTR& value);

	// �t���O���擾����
	void	get_page_flag(C_elm_int_list** pp_int_list, C_elm_str_list** pp_str_list);

	// �p�����[�^
	int		m_page_no;
};
