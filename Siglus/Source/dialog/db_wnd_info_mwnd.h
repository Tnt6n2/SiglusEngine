#pragma		once

#include	"db_wnd_info.h"

class	C_elm_mwnd_list;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F���b�Z�[�W�E�B���h�E
// ================================================================
class C_db_wnd_info_mwnd : public C_db_wnd_info
{
public:
	void		init();					// ������
	void		load_state();			// �X�e�[�^�X�ǂݍ���
	void		save_state();			// �X�e�[�^�X�ۑ�
	void		frame(C_elm_mwnd_list** mwnd_list_list, int list_cnt);		// �t���[��

private:

	enum
	{
		PAGE_CNT = 2,
	};

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_tab				tab;
		C_tnm_dbg_listview	lv[PAGE_CNT];
	}	ci;

	// �X�e�[�^�X
	struct STATE
	{
		struct SUB
		{
			int		mwnd_no;
			int		world;
			int		layer;
			bool	appear_flag;
			TSTR	msg;
		};

		ARRAY<SUB>	mwnd_list[PAGE_CNT];

	}	m_ld;

	// ��ƕϐ�
	int			m_page_no;
	bool		m_first[PAGE_CNT];

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init();										// ������
	bool		on_close();										// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm

	void		on_change_page(int page_no);					// �y�[�W���ύX���ꂽ

	void		change_page(int page_no);						// �y�[�W��ύX����
	void		update_one_item(int item_no, bool change);		// �P�̃A�C�e�����X�V����
};

