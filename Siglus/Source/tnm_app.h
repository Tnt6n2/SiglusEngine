#pragma		once

#include	"tnm_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_reg_ini.h"
#include	"data/tnm_cmd_line.h"

// ****************************************************************
// となまアプリケーション
// ================================================================
class C_tnm_app
{
public:
	bool	main();
	bool	message_loop();

private:
	C_tnm_reg_ini			m_reg_ini;		// reg.ini
	C_tnm_select_ini		m_select_ini;	// Select.ini
	C_tnm_ini				m_ini;			// Gameexe.ini
	C_tnm_cmd_line			m_cmd_line;		// コマンドライン
	C_tnm_eng				m_eng;			// エンジン
};




