#pragma		once

#include	"dialog/cfg_wnd_func_mod.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�^�C�}�[��`
// ================================================================
#define		CFG_WND_MODAL_TIMER_ID					12345
#define		CFG_WND_MODAL_TIMER_TIME				50
#define		CFG_WND_MODAL_TIMER_TIME_FOR_MSGSPD		10

// ****************************************************************
// �R���t�B�O�E�B���h�E�F���[�_��
// ================================================================
class C_cfg_wnd_modal : public C_modal_dialog
{
public:
	void		open_modal(HWND parent_h_wnd = G_app.h_wnd);					// �I�[�v�����[�_��
	bool		open_modal_result_bool(HWND parent_h_wnd = G_app.h_wnd);		// �I�[�v�����[�_��
	int			open_modal_result_int(HWND parent_h_wnd = G_app.h_wnd);			// �I�[�v�����[�_��

protected:

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);					// �E�B���h�E�v���V�[�W��

private:

	bool		on_init();														// ������
	bool		on_close();														// ����
	bool		on_timer(WPARAM wp, LPARAM lp);									// �^�C�}�[

	// ���z�֐�
	virtual		void		open_modal_func(HWND parent_h_wnd);					// �I�[�v�����[�_���t�@���N�V����
	virtual		bool		open_modal_result_bool_func(HWND parent_h_wnd);		// �I�[�v�����[�_���t�@���N�V����
	virtual		int			open_modal_result_int_func(HWND parent_h_wnd);		// �I�[�v�����[�_���t�@���N�V����

	// �������z�֐�
	virtual		bool		on_init_func() = 0;									// ������
	virtual		bool		on_close_func() = 0;								// ����
	virtual		bool		on_command(WPARAM wp, LPARAM lp) = 0;				// �R�}���h
	virtual		bool		on_timer_func(WPARAM wp, LPARAM lp) = 0;			// �^�C�}�[
	virtual		bool		on_notify(WPARAM wp, LPARAM lp) = 0;				// �ʒm
	virtual		bool		on_mousewheel(WPARAM wp, LPARAM lp) = 0;			// �}�E�X�z�C�[��
	virtual		bool		on_h_scroll(WPARAM wp, LPARAM lp) = 0;				// �g�X�N���[��
	virtual		bool		on_v_scroll(WPARAM wp, LPARAM lp) = 0;				// �u�X�N���[��
	virtual		LRESULT		on_ctlcolorstatic(WPARAM wp, LPARAM lp) = 0;		// �X�^�e�B�b�N�R���g���[���̃J���[�ύX

};

