#pragma		once

#include	"db_wnd_info.h"

class	C_elm_flag;
class	C_elm_str_list;
class	C_tnm_dbg_listview;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F���O�t���O
// ================================================================
class C_db_wnd_info_namae_flag : public C_db_wnd_info
{
public:
	void		init();						// ������
	void		load_state();				// �X�e�[�^�X�ǂݍ���
	void		save_state();				// �X�e�[�^�X�ۑ�
	void		frame();					// �t���[��

private:

	enum	{	PAGE_CNT = 2,	};

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_tab				tab;
		C_tnm_dbg_listview	lv_flag[PAGE_CNT];
	}	ci;

	// �X�e�[�^�X
	struct STATE
	{
		ARRAY<TSTR>	str_flag[PAGE_CNT];
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
	void		on_dblclk_lv(int ctrl_id, int form);			// ���X�g�r���[���_�u���N���b�N����

	void		change_page(int page_no);									// �y�[�W��ύX����
	void		change_str_flag(int ctrl_id, int flag_no, CTSTR& value);	// ������t���O���X�V����

	C_tnm_dbg_listview*	get_lv_by_ctrl_id(int ctrl_id);			// �R���g���[���h�c���烊�X�g�r���[���擾����
	C_elm_str_list*		get_str_list_by_ctrl_id(int ctrl_id);	// �R���g���[���h�c���當���񃊃X�g���擾����
};

