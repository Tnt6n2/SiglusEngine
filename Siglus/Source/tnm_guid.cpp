#include	"pch.h"
#include	"data/tnm_guid.h"

// ****************************************************************
// ‚f‚t‚h‚cƒNƒ‰ƒX‚ÌƒXƒ^ƒeƒBƒbƒNƒIƒuƒWƒFƒNƒg
// ================================================================
C_tnm_guid_manager	C_tnm_guid::m_guid_manager;

// ****************************************************************
// ‚f‚t‚h‚cFƒfƒtƒHƒ‹ƒgƒRƒ“ƒXƒgƒ‰ƒNƒ^
// ================================================================
C_tnm_guid::C_tnm_guid()
{
	// ƒtƒH[ƒ}ƒbƒg
	m_guid = -1;

	// ‰Šú‰»
	init();
}

// ****************************************************************
// ‚f‚t‚h‚cFƒRƒs[ƒRƒ“ƒXƒgƒ‰ƒNƒ^
// ================================================================
C_tnm_guid::C_tnm_guid(const C_tnm_guid& rhs)
{
	// ƒtƒH[ƒ}ƒbƒg
	m_guid = -1;

	// ‰Šú‰»
	init();

	// GUID ‚ðƒRƒs[‚·‚é
	m_guid_manager.create_guid(this, rhs.m_guid);
}

// ****************************************************************
// ‚f‚t‚h‚cF‘ã“ü‰‰ŽZŽq
// ================================================================
C_tnm_guid& C_tnm_guid::operator = (const C_tnm_guid& rhs)
{
	// ‰Šú‰»
	init();

	// GUID ‚ðƒRƒs[‚·‚é
	m_guid_manager.create_guid(this, rhs.m_guid);

	return *this;
}

// ****************************************************************
// ‚f‚t‚h‚cFƒfƒXƒgƒ‰ƒNƒ^
// ================================================================
C_tnm_guid::~C_tnm_guid()
{
	release_guid();
}


// ****************************************************************
// ‚f‚t‚h‚cF‰Šú‰»
// ================================================================
void C_tnm_guid::init()
{
	release_guid();
}

// ****************************************************************
// ‚f‚t‚h‚cF‚f‚t‚h‚c‚ðì¬‚·‚é
// ================================================================
void C_tnm_guid::create_guid()
{
	// GUID ‚ªì‚ç‚ê‚Ä‚È‚©‚Á‚½‚çì‚é
	if (m_guid == -1)
		m_guid_manager.create_guid(this);
}

// ****************************************************************
// ‚f‚t‚h‚cF‚f‚t‚h‚c‚ð‰ð•ú‚·‚é
// ================================================================
void C_tnm_guid::release_guid()
{
	// GUID ‚ªì‚ç‚ê‚Ä‚¢‚ê‚Î‰ð•ú‚·‚é
	if (m_guid >= 0)
		m_guid_manager.release_guid(this);
}


// ****************************************************************
// ‚f‚t‚h‚cƒ}ƒl[ƒWƒƒFƒRƒ“ƒXƒgƒ‰ƒNƒ^
// ================================================================
C_tnm_guid_manager::C_tnm_guid_manager()
{
	m_guid_cnt = 0;
}

// ****************************************************************
// ‚f‚t‚h‚cƒ}ƒl[ƒWƒƒF‚f‚t‚h‚c‚ðì¬
// ================================================================
void C_tnm_guid_manager::create_guid(C_tnm_guid* guid)
{
	// ‚h‚c‚ð—\–ñ‚·‚é
	if (m_enable_guid_list.empty())	{
		guid->m_guid = m_guid_cnt;					// V‚µ‚¢ GUID ‚ðŠ„‚è“–‚Ä‚é
		m_ref_count_list.resize(m_guid_cnt + 1);	// ŽQÆƒJƒEƒ“ƒgƒŠƒXƒg‚ðŠg’£
		m_ref_count_list[guid->m_guid] = 1;			// ŽQÆƒJƒEƒ“ƒg‚ðÝ’è‚·‚é
		m_guid_cnt ++;								// Š„‚è“–‚Ä‚½ GUID ”‚ð‘‚â‚·
	}
	else	{
		guid->m_guid = m_enable_guid_list.back();	// ‹ó‚¢‚Ä‚¢‚é GUID ‚ðŠ„‚è“–‚Ä‚é
		m_ref_count_list[guid->m_guid] ++;			// ŽQÆƒJƒEƒ“ƒ^‚ðã‚°‚é
		m_enable_guid_list.pop_back();				// Žg—p‚Å‚«‚é GUID ƒŠƒXƒg‚©‚çíœ
	}
}

// ****************************************************************
// ‚f‚t‚h‚cƒ}ƒl[ƒWƒƒF‚f‚t‚h‚c‚ðŠ„‚è“–‚Ä‚é
// ================================================================
void C_tnm_guid_manager::create_guid(C_tnm_guid* guid, int id)
{
	if (id >= 0)	{
		guid->m_guid = id;							// GUID ‚ðŠ„‚è“–‚Ä‚é
		m_ref_count_list[guid->m_guid] ++;			// ŽQÆƒJƒEƒ“ƒg‚ðã‚°‚é
	}
}

// ****************************************************************
// ‚f‚t‚h‚cƒ}ƒl[ƒWƒƒF‚f‚t‚h‚c‚ð‰ð•ú
// ================================================================
void C_tnm_guid_manager::release_guid(C_tnm_guid* guid)
{
	// ‚h‚c‚ð‰ð•ú‚·‚é
	if (guid->m_guid >= 0)	{
		m_ref_count_list[guid->m_guid] --;			// ŽQÆƒJƒEƒ“ƒg‚ð‰º‚°‚é

		// ŽQÆƒJƒEƒ“ƒ^‚ª‚O‚É‚È‚Á‚½‚ç‰ð•úƒŠƒXƒg‚É’Ç‰Á
		if (m_ref_count_list[guid->m_guid] == 0)	{
			m_enable_guid_list.push_back(guid->m_guid);
		}

		guid->m_guid = -1;							// GUID ‚ð‰ð•ú‚·‚é
	}
}

