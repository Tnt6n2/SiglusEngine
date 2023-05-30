#pragma		once

#include	"db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F���[�U�R�[���t���O
// ================================================================
class C_db_wnd_info_user_call_flag : public C_db_wnd_info_listview
{
public:
	void		init();			// ������
	void		frame();		// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		struct ITEM
		{
			int		prop_id;
			int		form;
			int		size;
			TSTR	name;
			TSTR	value;
		};

		ARRAY<ITEM>		item_list;

	}	m_ld;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm

	// ���X�g�r���[���_�u���N���b�N����
	void	on_dblclk_lv();

	// ���X�g�r���[�̒l��ύX����
	void	on_change_lv_value(int item_no, int column_no, CTSTR& value);
};

