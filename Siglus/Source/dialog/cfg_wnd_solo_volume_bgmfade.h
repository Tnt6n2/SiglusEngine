#pragma		once

#include	"cfg_wnd_solo.h"
#include	"cfg_wnd_func_volume.h"
#include	"cfg_wnd_func_bgmfade.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�x�[�X�F���ʁi���ʁA�a�f�l�t�F�[�h�j
// ================================================================
class Ccfg_wnd_solo_volume_bgmfade : public C_cfg_wnd_solo
{
public:
	void		init();						// ������

	Ccfg_wnd_func_volume	cfg_wnd_func_volume;
	Ccfg_wnd_func_bgmfade	cfg_wnd_func_bgmfade;

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
