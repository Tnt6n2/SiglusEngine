#pragma		once

#include	"db_wnd_info.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�b�f�e�[�u��
// ================================================================
class C_db_wnd_info_cg_table : public C_db_wnd_info
{
public:
	void		init();					// ������
	void		load_state();			// �X�e�[�^�X�ǂݍ���
	void		save_state();			// �X�e�[�^�X�ۑ�
	void		frame();				// �t���[��

private:

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_tab				tab;
		C_tnm_dbg_listview	lst_original;
		C_tnm_dbg_listview	lst_group;
	}	ci;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init();										// ������
	bool		on_close();										// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm
	void		on_dblclk_lv(int ctrl_id);						// ���X�g�r���[���_�u���N���b�N����

	void		change_page(int page_no);							// �y�[�W��ύX����
	void		change_flag(int ctrl_id, int item_no, int value);	// �t���O���X�V����

	C_tnm_dbg_listview*	get_lv_by_ctrl_id(int ctrl_id);			// �R���g���[���h�c���烊�X�g�r���[���擾����
};

