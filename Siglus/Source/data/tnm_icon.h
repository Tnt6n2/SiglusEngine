#pragma		once

// ****************************************************************
// アイコン
// ================================================================
class C_tnm_icon
{
public:
	C_tnm_icon();
	~C_tnm_icon();

	void		create_icon();
	void		destroy_icon();
	HICON		get_large_icon()	{	return m_h_large_icon;	}
	HICON		get_small_icon()	{	return m_h_small_icon;	}

private:
	HICON		m_h_large_icon;					// 大きいアイコン
	HICON		m_h_small_icon;					// 小さいアイコン

	// コピー禁止！
	C_tnm_icon(C_tnm_icon&);
	C_tnm_icon& operator = (C_tnm_icon&);
};

