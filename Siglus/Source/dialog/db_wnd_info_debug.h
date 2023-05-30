#pragma		once

#include	"dialog/db_wnd_info.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�f�o�b�O�ݒ�
// ================================================================
class C_db_wnd_info_debug : public C_db_wnd_info
{

public:

	void		init();						// ������
	void		load_state();				// �X�e�[�^�X�ǂݍ���
	void		save_state();				// �X�e�[�^�X�ۑ�
	void		frame();					// �t���[��

private:

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_static	stc_ss;
		Cc_edit		edt_ss;
		Cc_static	stc_editor;
		Cc_edit		edt_editor;
		Cc_static	stc_koe;
		Cc_edit		edt_koe;
		Cc_static	stc_koe_tool;
		Cc_edit		edt_koe_tool;
		Cc_button	btn_open;
	}	ci;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init();										// ������
	bool		on_close();										// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h

	bool		on_edit_ss_path();
	bool		on_edit_editor_path();
	bool		on_edit_koe_path();
	bool		on_edit_koe_tool_path();
	bool		on_btn_open();
};


