#include	"pch.h"

#if __USE_STEAM

#include	"data/tnm_steam.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// スチームインターフェイス
// ================================================================

void steam_first_proc()
{
	if (SteamUserStats())
	{
		if (!SteamUserStats()->RequestCurrentStats())
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Steam：ユーザーデータの取得に失敗しました。(1)"));
		}
	}
}

void steam_frame_proc()
{
	SteamAPI_RunCallbacks();
}

void steam_set_achievement(CTSTR& achievement_id)
{
	if (SteamUserStats())
	{
		if (!SteamUserStats()->SetAchievement(TSTR_to_MBSTR(achievement_id).c_str()))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Steam：実績の設定に失敗しました。(1)"));
		}
		if (!SteamUserStats()->StoreStats())
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Steam：ユーザーデータの保存に失敗しました。(1)"));
		}
	}
}

void steam_reset_all_status()
{
	if (SteamUserStats())
	{
		if (!SteamUserStats()->ResetAllStats(true))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Steam：情報の初期化に失敗しました。(1)"));
		}
	}
}

// ****************************************************************
// スチームクラス本体
// ================================================================

C_tnm_steam::C_tnm_steam()
	: m_CallbackUserStatsReceived(this, &C_tnm_steam::OnUserStatsReceived)
	, m_CallbackUserStatsStored(this, &C_tnm_steam::OnUserStatsStored)
	, m_CallbackAchievementStored(this, &C_tnm_steam::OnAchievementStored)
{

}

void C_tnm_steam::OnUserStatsReceived(UserStatsReceived_t *callback)
{
	SteamUtils()->GetAppID();

	if (callback->m_nGameID == CGameID(SteamUtils()->GetAppID()).ToUint64())
	{
		if (callback->m_eResult == k_EResultOK)
		{
			// OK
		}
		else
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Steam：ユーザーデータの取得に失敗しました。(2)"));
		}
	}
}

void C_tnm_steam::OnUserStatsStored(UserStatsStored_t *callback)
{
}

void C_tnm_steam::OnAchievementStored(UserAchievementStored_t *callback)
{
}


#endif
