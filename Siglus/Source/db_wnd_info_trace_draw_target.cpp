#include	"pch.h"

#include	"dialog/db_wnd_info_trace_draw_target.h"

#include	"../resource.h"

// ****************************************************************
// コンストラクタ
// ================================================================
C_db_wnd_info_trace_draw_target::C_db_wnd_info_trace_draw_target() : text_bmp(true)
{
	init_text_bmp();
}

// ****************************************************************
// デストラクタ
// ================================================================
C_db_wnd_info_trace_draw_target::~C_db_wnd_info_trace_draw_target()
{
	free_text_bmp();
}

// ****************************************************************
// テキストＢＭＰ：初期化
// ================================================================
void C_db_wnd_info_trace_draw_target::init_text_bmp()
{
	text_mdc = 0;
}

// ****************************************************************
// テキストＢＭＰ：開放
// ================================================================
void C_db_wnd_info_trace_draw_target::free_text_bmp()
{
	if (text_mdc)	{
		DeleteDC(text_mdc);
	}
	text_bmp.destroy();
	init_text_bmp();
}

// ****************************************************************
// テキストＢＭＰ：確保
// ================================================================
bool C_db_wnd_info_trace_draw_target::alloc_text_bmp()
{
	HWND h_wnd = get_handle();
	if (h_wnd == 0)	{	return false;	}

	C_rect rect = get_client_rect();
	if (text_bmp.get_handle() && rect.width() == text_bmp.get_width() && rect.height() == text_bmp.get_height())	{	return false;	}

	free_text_bmp();

	text_bmp.create(rect.width(), rect.height(), 32);

	HDC hdc = GetDC(h_wnd);
	text_mdc = CreateCompatibleDC(hdc);
	SelectObject(text_mdc, text_bmp.get_handle());
	ReleaseDC(h_wnd, hdc);

	return true;
}

// ****************************************************************
// 矩形描画
// ================================================================

void C_db_wnd_info_trace_draw_target::draw_fill_rect(int x1, int y1, int x2, int y2, HBRUSH brush)
{
	if (x1 >= x2 || y1 >= y2)	{	return;	}
	C_rect rect(x1, y1, x2, y2);
	FillRect(text_mdc, &rect, brush);
	invalidate_rect(&rect, FALSE);
}

void C_db_wnd_info_trace_draw_target::draw_fill_rect(int x1, int y1, int x2, int y2, BYTE r, BYTE g, BYTE b)
{
	HBRUSH brush = CreateSolidBrush(RGB(r, g, b));
	draw_fill_rect(x1, y1, x2, y2, brush);
	DeleteObject(brush);
}

// ****************************************************************
// 矩形コピー
// ================================================================
void C_db_wnd_info_trace_draw_target::copy_rect(int x1, int y1, int x2, int y2, int dst_x, int dst_y)
{
	if (x1 >= x2 || y1 >= y2)	{	return;	}
	int width = x2 - x1;
	int height = y2 - y1;
	BitBlt(text_mdc, dst_x, dst_y, width, height, text_mdc, x1, y1, SRCCOPY);
	C_rect rect = C_rect::by_size(dst_x, dst_y, width, height);
	invalidate_rect(&rect, FALSE);
}

// ****************************************************************
// 文字描画（更新処理は行いません）
// ================================================================
void C_db_wnd_info_trace_draw_target::draw_moji(TCHAR moji, int x, int y, HFONT hfont, COLORREF rgb)
{
	SelectObject(text_mdc, hfont);
	SetTextColor(text_mdc, rgb);
	SetBkMode(text_mdc, TRANSPARENT);
	TCHAR str[] = {moji, _T('\0')};
	TextOut(text_mdc, x, y, str, 1);
}

void C_db_wnd_info_trace_draw_target::draw_moji(TCHAR moji, int x, int y, HFONT hfont, BYTE r, BYTE g, BYTE b)
{
	draw_moji(moji, x, y, hfont, RGB(r, g, b));
}

// ****************************************************************
// プロシージャ
// ================================================================
LRESULT C_db_wnd_info_trace_draw_target::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_PAINT:
			if(text_mdc){
				PAINTSTRUCT pc;
				HDC hdc = BeginPaint(get_handle(), &pc);
				BitBlt(hdc,
					pc.rcPaint.left, pc.rcPaint.top,
					pc.rcPaint.right, pc.rcPaint.bottom,
					text_mdc,
					pc.rcPaint.left, pc.rcPaint.top,
					SRCCOPY);
				EndPaint(get_handle(), &pc);
			}
			break;
	}
	return Cc_picture::window_proc(msg, wp, lp);
}

