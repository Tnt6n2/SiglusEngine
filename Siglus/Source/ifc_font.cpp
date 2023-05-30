#include	"pch.h"
#include	"ifc_eng.h"
#include	"engine/eng_etc.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_moji_manager.h"
#include	"element/elm_excall.h"

// �t�H���g���X�V
void tnm_update_font(bool change_force)
{
	TSTR font_name;
	bool font_futoku;
	int font_shadow;

	// �f�t�H���g�͊��ݒ�
	font_name = Gp_config->font_name;
	font_futoku = Gp_config->font_futoku;
	font_shadow = Gp_config->font_shadow;

	// �u�d�w�R�[���v�u���b�Z�[�W�o�b�N�v�łȂ��Ȃ烍�[�J���̒l�𔽉f
	if (!tnm_excall_is_excall() && !tnm_excall_is_msg_back())
	{
		if (!Gp_local->font_name.empty())
		{
			font_name = Gp_local->font_name;
		}
		if (Gp_local->pod.font_futoku >= 0)
		{
			font_futoku = Gp_local->pod.font_futoku ? true : false;
		}
		if (Gp_local->pod.font_shadow >= 0)
		{
			font_shadow = Gp_local->pod.font_shadow;
		}
	}

	// �u�d�w�R�[���v�Ȃ炻�̒l�𔽉f
	if (tnm_excall_is_excall() && !tnm_excall_is_msg_back())
	{
		if (!Gp_excall->m_font_name.empty())
		{
			font_name = Gp_excall->m_font_name;
		}
		if (Gp_excall->m_pod.font_futoku >= 0)
		{
			font_futoku = Gp_excall->m_pod.font_futoku ? true : false;
		}
		if (Gp_excall->m_pod.font_shadow >= 0)
		{
			font_shadow = Gp_excall->m_pod.font_shadow;
		}
	}

	// �O��Ɣ�r���ĕς���Ă���Ε����}�l�[�W���ɔ��f
	if (change_force || font_name != Gp_global->total_font_name || font_futoku != Gp_global->total_font_futoku || font_shadow != Gp_global->total_font_shadow)
	{
		G_moji_manager.clear();
	}

	// ��Ԃ�ۑ�
	Gp_global->total_font_name = font_name;
	Gp_global->total_font_futoku = font_futoku;
	Gp_global->total_font_shadow = font_shadow;
}
