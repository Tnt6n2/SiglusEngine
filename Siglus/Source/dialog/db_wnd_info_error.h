#pragma		once

#include	"data/tnm_def.h"
#include	"dialog/db_wnd_info.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�G���[
// ================================================================
class C_db_wnd_info_error : public C_db_wnd_info
{
public:

	void		init();		// ������
	void		frame();	// �t���[��

private:

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		C_tnm_dbg_listview	lv;
		Cc_check			chk_only_error;
	}	ci;

	// �X�e�[�^�X
	struct STATE
	{
		int		error_cnt;
	}	m_ld;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init();										// ������
	bool		on_close();										// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h

	void		clear_lv();
	void		add_lv_item(TNM_ERROR_TYPE error_type, CTSTR& error_str);
};

