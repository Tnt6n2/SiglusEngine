#pragma		once

// ****************************************************************
// カラーテーブル
// ================================================================
class C_tnm_color_table
{
public:
	void	init();

	C_argb	get_color(int color_no)
	{
		color_no = limit(0, color_no, (int)m_color.size() - 1);
		return m_color[color_no];
	}

private:
	ARRAY<C_argb>	m_color;
};



