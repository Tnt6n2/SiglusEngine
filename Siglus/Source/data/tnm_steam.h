#pragma		once

#if __USE_STEAM

enum EAchievements
{
	ACH_TEST = 0,
};

struct STEAM_Achievement_t
{
	EAchievements m_eAchievementID;
	const char *m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};

class C_tnm_steam
{
public:
	C_tnm_steam();


private:

	// ユーザーデータを取得
	STEAM_CALLBACK(C_tnm_steam, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);

	// ユーザーデータを保存
	STEAM_CALLBACK(C_tnm_steam, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);

	// アチーブメントを保存
	STEAM_CALLBACK(C_tnm_steam, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
};

#endif
