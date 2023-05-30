#pragma		once

#include	"dialog/db_wnd_info_listview.h"

class	C_elm_object_list;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�I�u�W�F�N�g
// ================================================================
class C_db_wnd_info_object : public C_db_wnd_info_listview
{
public:
	void	init();		// ������
	void	frame();	// �t���[��

	// �y�[�W�ԍ���ݒ�
	void	set_page_no(int page_no)	{	m_page_no = page_no;	}

private:

	void	on_init_func();		// �������t�@���N�V����
	void	on_close_func();	// ����t�@���N�V����

	// �E�B���h�E�v���V�[�W��
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm

	// ���X�g�r���[���_�u���N���b�N����
	void	on_dblclk_lv();

	// ���X�g�r���[�̒l��ύX����
	void	on_change_lv_value(int item_no, int column_no, CTSTR& value);

	// �I�u�W�F�N�g���擾����
	void	get_page_object(C_elm_object_list** pp_int_list);

	// �p�����[�^
	int		m_page_no;
};
