#pragma		once

// ****************************************************************
// クリック禁止矩形
// ================================================================
class C_tnm_click_disable
{
public:
	C_rect		m_rect;

	void init()
	{
		m_rect = C_rect(0, 0, 0, 0);
	}
	void set_rect(const C_rect& rect)
	{
		m_rect = rect;
	}
	void clear_rect()
	{
		m_rect = C_rect(0, 0, 0, 0);
	}
};

