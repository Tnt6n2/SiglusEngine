#pragma		once

// ****************************************************************
// ‚f‚t‚h‚cƒ}ƒl[ƒWƒƒ
// ================================================================
//		‚ ‚ç‚ä‚éƒIƒuƒWƒFƒNƒg‚ÉŒÂ•Ê‚h‚c‚ğU‚è‚Ü‚·B
// ================================================================


// ****************************************************************
// ‚f‚t‚h‚cƒNƒ‰ƒX‚Ì‘O•ûéŒ¾
// ================================================================
class	C_tnm_guid;

// ****************************************************************
// ‚f‚t‚h‚cƒ}ƒl[ƒWƒƒ
// ================================================================
class C_tnm_guid_manager
{
	friend	class	C_tnm_guid;

public:

	C_tnm_guid_manager();

private:

	void	create_guid(C_tnm_guid* guid);			// GUID ‚ğì‚é
	void	create_guid(C_tnm_guid* guid, int id);	// GUID ‚ğì‚éiIDw’èj
	void	release_guid(C_tnm_guid* guid);

	ARRAY<int>	m_enable_guid_list;		// g—p‚Å‚«‚é GUID ƒŠƒXƒg
	ARRAY<int>	m_ref_count_list;		// QÆƒJƒEƒ“ƒg
	int			m_guid_cnt;				// ì¬‚µ‚½ GUID ‚ÌŒÂ”
};

// ****************************************************************
// ‚f‚t‚h‚c
// ================================================================
class C_tnm_guid
{
	friend	class	C_tnm_guid_manager;

public:
	C_tnm_guid();
	~C_tnm_guid();
	C_tnm_guid(const C_tnm_guid& rhs);
	C_tnm_guid& operator = (const C_tnm_guid& rhs);

	void	init();								// ‰Šú‰»‚·‚é
	void	create_guid();						// GUID ‚ğì¬‚·‚é
	void	release_guid();						// GUID ‚ğ‰ğ•ú‚·‚é
	int		get_guid()	{	return m_guid;	}	// GUID ‚ğæ“¾‚·‚é

private:

	int							m_guid;
	static	C_tnm_guid_manager	m_guid_manager;
};




