#pragma		once

#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�Fflag.ini
// ================================================================
class C_db_wnd_info_flag_ini : public C_db_wnd_info_listview
{
public:
	void	init();		// ������
	void	frame();	// �t���[��

	void	set_page_no(int page_no)	{	m_page_no = page_no;	}

private:

	// �X�e�[�^�X

	void	on_init_func();		// �������t�@���N�V����
	void	on_close_func();	// ����t�@���N�V����

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm

	// ���X�g�r���[���_�u���N���b�N����
	void	on_dblclk_lv();

	// ���X�g�r���[�̒l��ύX����
	void	on_change_lv_value(int item_no, int column_no, CTSTR& value);

	// �p�����[�^
	int		m_page_no;

};
