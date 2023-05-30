#pragma		once

#include	"cfg_wnd_modal.h"
#include	"cfg_wnd_func_system.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F���[�_���F�V�X�e��
// ================================================================
class Ccfg_wnd_modal_system : public C_cfg_wnd_modal
{
public:
	C_cfg_wnd_func_system		cfg_wnd_func_system;

private:

	void		open_modal_func(HWND parent_h_wnd);				// �I�[�v�����[�_���t�@���N�V����

	bool		on_init_func();									// ������
	bool		on_close_func();								// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h
	bool		on_timer_func(WPARAM wp, LPARAM lp);			// �^�C�}�[
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm
	bool		on_mousewheel(WPARAM wp, LPARAM lp);			// �}�E�X�z�C�[��
	bool		on_h_scroll(WPARAM wp, LPARAM lp);				// �g�X�N���[��
	bool		on_v_scroll(WPARAM wp, LPARAM lp);				// �u�X�N���[��
	LRESULT		on_ctlcolorstatic(WPARAM wp, LPARAM lp);		// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
};
