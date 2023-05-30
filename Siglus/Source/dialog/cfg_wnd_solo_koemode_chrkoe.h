#pragma		once

#include	"cfg_wnd_solo.h"
#include	"cfg_wnd_func_koemode.h"
#include	"cfg_wnd_func_chrkoe.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�\���F�����i�������[�h�A�L�����N�^�[�����j
// ================================================================
class Ccfg_wnd_solo_koemode_chrkoe : public C_cfg_wnd_solo
{
public:
	void		init();						// ������

	Ccfg_wnd_func_koemode	cfg_wnd_func_koemode;
	Ccfg_wnd_func_chrkoe	cfg_wnd_func_chrkoe;

private:

	bool		on_init_func();									// ������
	bool		on_close_func();								// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm
	bool		on_mousewheel(WPARAM wp, LPARAM lp);			// �}�E�X�z�C�[��
	bool		on_h_scroll(WPARAM wp, LPARAM lp);				// �g�X�N���[��
	bool		on_v_scroll(WPARAM wp, LPARAM lp);				// �u�X�N���[��
	LRESULT		on_ctlcolorstatic(WPARAM wp, LPARAM lp);		// �X�^�e�B�b�N�R���g���[���̃J���[�ύX

	void		frame_func();									// �t���[��
};
