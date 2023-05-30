#include	"pch.h"
#include	"ifc_eng.h"
#include	"engine/eng_etc.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_moji_manager.h"
#include	"element/elm_excall.h"

// フォントを更新
void tnm_update_font(bool change_force)
{
	TSTR font_name;
	bool font_futoku;
	int font_shadow;

	// デフォルトは環境設定
	font_name = Gp_config->font_name;
	font_futoku = Gp_config->font_futoku;
	font_shadow = Gp_config->font_shadow;

	// 「ＥＸコール」「メッセージバック」でないならローカルの値を反映
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

	// 「ＥＸコール」ならその値を反映
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

	// 前回と比較して変わっていれば文字マネージャに反映
	if (change_force || font_name != Gp_global->total_font_name || font_futoku != Gp_global->total_font_futoku || font_shadow != Gp_global->total_font_shadow)
	{
		G_moji_manager.clear();
	}

	// 状態を保存
	Gp_global->total_font_name = font_name;
	Gp_global->total_font_futoku = font_futoku;
	Gp_global->total_font_shadow = font_shadow;
}
