#pragma		once
#include	"data/tnm_dlg.h"
#include	"dialog/cfg_wnd_func_mod.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�\��
// ================================================================
class C_cfg_wnd_solo : public C_tnm_dlg
{

public:

	C_cfg_wnd_solo()	{	cfg_wnd_full_mode_flag = false;	}		// �R���X�g���N�^

	void		refresh_dialog_func(int dialog_width, int dialog_height);

	bool		cfg_wnd_full_mode_flag;

	// ���z�֐�
	virtual		void		frame();											// �t���[��

protected:

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);					// �E�B���h�E�v���V�[�W��

	// ���z�֐�
	virtual		bool		on_init();											// ������
	virtual		bool		on_close();											// ����

	// �������z�֐�
	virtual		bool		on_init_func() = 0;									// ������
	virtual		bool		on_close_func() = 0;								// ����
	virtual		bool		on_command(WPARAM wp, LPARAM lp);					// �R�}���h
	virtual		bool		on_notify(WPARAM wp, LPARAM lp);					// �ʒm
	virtual		bool		on_mousewheel(WPARAM wp, LPARAM lp);				// �}�E�X�z�C�[��
	virtual		bool		on_h_scroll(WPARAM wp, LPARAM lp);					// �g�X�N���[��
	virtual		bool		on_v_scroll(WPARAM wp, LPARAM lp);					// �u�X�N���[��
	virtual		LRESULT		on_ctlcolorstatic(WPARAM wp, LPARAM lp);			// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
	virtual		void		frame_func();										// �t���[��
};

