
#include	"steam/steam_api.h"

#pragma		comment(lib, "steam_api.lib")

class CSteam
{
public:
	CSteam();
	
private:

	STEAM_CALLBACK(CSteam, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
};

CSteam::CSteam()
	: m_CallbackUserStatsReceived(this, &CSteam::OnUserStatsReceived)
{
}

CSteam g_steam;

void CSteam::OnUserStatsReceived(UserStatsReceived_t *callback)
{
	if (callback->m_eResult == k_EResultFail)
	{
		printf("k_EResultFail...\n");
	}
	else
	{
		printf("NOT k_EResultFail!!\n");
	}
}

int main() {

	if (!SteamAPI_Init())
		return 1;

	if (!SteamUserStats()->RequestCurrentStats())
		return 1;

	while (1)
	{
		SteamAPI_RunCallbacks();
	}

	return 0;
}

