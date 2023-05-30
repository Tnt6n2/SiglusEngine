#pragma		once

// ****************************************************************
// デバッグ情報ウィンドウ：描画先
// ================================================================
class C_db_wnd_info_trace_draw_target : public Cc_picture
{
public:
	C_db_wnd_info_trace_draw_target();						// コンストラクタ
	~C_db_wnd_info_trace_draw_target();						// デストラクタ
	bool		alloc_text_bmp();							// テキストＢＭＰ：確保
	void		draw_fill_rect(int x1, int y1, int x2, int y2, HBRUSH brush);				// 矩形描画
	void		draw_fill_rect(int x1, int y1, int x2, int y2, BYTE r, BYTE g, BYTE b);		// 矩形描画
	void		copy_rect(int x1, int y1, int x2, int y2, int dst_x, int dst_y);			// 矩形コピー
	void		draw_moji(TCHAR moji, int x, int y, HFONT hfont, COLORREF rgb);				// 文字描画（更新処理は行いません）
	void		draw_moji(TCHAR moji, int x, int y, HFONT hfont, BYTE r, BYTE g, BYTE b);	// 文字描画（更新処理は行いません）

	C_dib		text_bmp;
	HDC			text_mdc;

private:
	void		init_text_bmp();							// テキストＢＭＰ：初期化
	void		free_text_bmp();							// テキストＢＭＰ：開放

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
};


