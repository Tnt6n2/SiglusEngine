#include	"pch.h"
#include	"dialog/cfg_wnd_solo_volume_bgmfade.h"

#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�\���F���ʁi���ʁA�a�f�l�t�F�[�h�j
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_solo_volume_bgmfade")

// ������
void Ccfg_wnd_solo_volume_bgmfade::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_CFG_WIN_SOLO_COMMON);
}

// ������
bool Ccfg_wnd_solo_volume_bgmfade::on_init_func()
{
	cfg_wnd_func_volume.on_init_func(this, CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE, false, false, false);
	cfg_wnd_func_bgmfade.on_init_func(this, CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE, false, false, false);

	return true;
}

// ����
bool Ccfg_wnd_solo_volume_bgmfade::on_close_func()
{
	cfg_wnd_func_volume.on_close_func();
	cfg_wnd_func_bgmfade.on_close_func();
	return true;
}

// �R�}���h
bool Ccfg_wnd_solo_volume_bgmfade::on_command(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_command_func(wp, lp))	{	return true;	}
	if (cfg_wnd_func_bgmfade.on_command_func(wp, lp))	{	return true;	}
	return true;
}

// �ʒm
bool Ccfg_wnd_solo_volume_bgmfade::on_notify(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_notify_func(wp, lp))		{	return true;	}
	if (cfg_wnd_func_bgmfade.on_notify_func(wp, lp))	{	return true;	}
	return true;
}

// �}�E�X�z�C�[��
bool Ccfg_wnd_solo_volume_bgmfade::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_mousewheel_func(wp, lp))		{	return false;	}
	if (cfg_wnd_func_bgmfade.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// �g�X�N���[��
bool Ccfg_wnd_solo_volume_bgmfade::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_h_scroll_func(wp, lp))	{	return true;	}
	if (cfg_wnd_func_bgmfade.on_h_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// �u�X�N���[��
bool Ccfg_wnd_solo_volume_bgmfade::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_v_scroll_func(wp, lp))	{	return true;	}
	if (cfg_wnd_func_bgmfade.on_v_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT Ccfg_wnd_solo_volume_bgmfade::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = cfg_wnd_func_volume.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	if (res = cfg_wnd_func_bgmfade.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	return NULL;
}

// �t���[��
void Ccfg_wnd_solo_volume_bgmfade::frame_func()
{
	if (!m_h_wnd)	{	return;	}
	cfg_wnd_func_volume.frame_func();
	cfg_wnd_func_bgmfade.frame_func();
}

