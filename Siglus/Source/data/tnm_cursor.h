#pragma		once


// ****************************************************************
// マウスカーソル
// ================================================================
//		カラーにするため、カーソルだけどアイコンを使うのです。
// ================================================================
struct S_cursor
{
	S_cursor();
	~S_cursor();

	HICON		h_icon;
	HBITMAP		h_mask;

private:
	// コピー禁止
	S_cursor(S_cursor&);
	S_cursor& operator = (S_cursor&);
};

class C_tnm_cursor
{
public:
	C_tnm_cursor();

	void	create_cursor(int cursor_no);

public:
	BSA<S_cursor>	m_cursor_pat_list;
	int				m_cursor_pat_cnt;
};

class C_tnm_cursor_list
{
public:
	ARRAY<C_tnm_cursor>		sub;
};

// ****************************************************************
// マウスカーソル：フレーム処理
// ================================================================
void	tnm_cursor_frame(int cur_time);
