#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_icon.h"

// ****************************************************************
// �A�C�R��
// ================================================================
C_tnm_icon::C_tnm_icon()
{
	m_h_large_icon = NULL;
	m_h_small_icon = NULL;
}

C_tnm_icon::~C_tnm_icon()
{
	destroy_icon();
}

// ****************************************************************
// �A�C�R���F�A�C�R�����č\�z����
// ================================================================
void C_tnm_icon::create_icon()
{
	// exe �Ɠ����f�B���N�g���ɂ��� ico �t�@�C����T���o���A�E�B���h�E�A�C�R���ɐݒ肵�܂��B
	C_file_find ff;
	ff.find(Gp_dir->exe_dir, _T("*"), _T("ico"), false, true, false);
	if (ff.get_file_cnt() > 0)	{

		// �傫���A�C�R���Ə������A�C�R����ʁX�ɓǂݍ���
		m_h_large_icon = (HICON)LoadImage(NULL, (Gp_dir->exe_dir + _T("\\") + ff[0].rltv_path).c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		m_h_small_icon = (HICON)LoadImage(NULL, (Gp_dir->exe_dir + _T("\\") + ff[0].rltv_path).c_str(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	}
}

// ****************************************************************
// �A�C�R���F�A�C�R����j������
// ================================================================
void C_tnm_icon::destroy_icon()
{
	if (m_h_large_icon)	{
		DestroyIcon(m_h_large_icon);
		m_h_large_icon = NULL;
	}
	if (m_h_small_icon)	{
		DestroyIcon(m_h_small_icon);
		m_h_small_icon = NULL;
	}
}

