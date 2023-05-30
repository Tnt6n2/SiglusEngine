#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"dialog/sys_wnd_solo_tweet.h"
#include	"engine/eng_twitter.h"
#include	"localize/localize.h"
#include	"../resource.h"

// ****************************************************************
// システムウィンドウ：ソロ：ツイート
// ================================================================

#define		REGIST_WINDOW_NAME		_T("sys_wnd_solo_tweet")

// 初期化
void C_sys_wnd_solo_tweet::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_TWEET);
}

// 初期化
bool C_sys_wnd_solo_tweet::on_init_func()
{
	bind_control(&m_stc_bitmap, IDC_STC_BITMAP, _T("stc_bitmap"), 5);
	bind_control(&m_stc_user_name, IDC_STC_USER_NAME, _T("stc_user_name"), 2);
	bind_control(&m_edt_tweet_text, IDC_EDT_TWEET_TEXT, _T("edt_tweet_text"), 2);
	bind_control(&m_btn_authorize, IDC_BTN_AUTHORIZE, _T("btn_authorize"), 3);
	bind_control(&m_btn_tweet, IDC_BTN_TWEET, _T("btn_tweet"), 3);

	m_stc_bitmap.m_dib.load_png(Gp_dir->get_tweet_image_file_path());
	m_edt_tweet_text.set_text(Gp_ini->twitter_initial_tweet_text);

	update_state();

	return true;
}

// 閉じる
bool C_sys_wnd_solo_tweet::on_close_func()
{
	return true;
}

// コマンド
bool C_sys_wnd_solo_tweet::on_command(WPARAM wp, LPARAM lp)
{
	if (LOWORD(wp) == IDC_BTN_AUTHORIZE)
	{
		twitter_authorize();
	}
	else if (LOWORD(wp) == IDC_BTN_TWEET)
	{
		TSTR text = m_edt_tweet_text.get_text();
		if (text.empty())
		{
			int res = error_box(_T("メッセージが空です。投稿してよろしいですか？"), MB_OKCANCEL);
			if (res == IDOK)
			{
				twitter_tweet(text, Gp_dir->get_tweet_image_file_path());
			}
		}
		else
		{
			twitter_tweet(text, Gp_dir->get_tweet_image_file_path());
		}
	}

	return true;
}

void C_sys_wnd_solo_tweet::on_success_tweet()
{
	m_edt_tweet_text.clear_text();

	update_state();

	close();
}

void C_sys_wnd_solo_tweet::update_state()
{
	// 認証済み
	if (twitter_is_authorized())
	{
		m_stc_user_name.set_text(G_twitter_user_name + _T(" (@") + G_twitter_screen_name + _T(")"));
		m_edt_tweet_text.set_enable(true);
		m_btn_tweet.set_enable(true);
	}
	// 認証まだ
	else
	{
		m_stc_user_name.set_text(LCL_STR_TWITTER_DIALOG_STR_AUTH_NEED);
		m_edt_tweet_text.set_enable(false);
		m_btn_tweet.set_enable(false);
	}
}




C_sys_wnd_solo_tweet_bitmap::C_sys_wnd_solo_tweet_bitmap() : m_dib(true)
{

}

LRESULT C_sys_wnd_solo_tweet_bitmap::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	if (msg == WM_PAINT) {
		if (!on_paint()) { return FALSE; }
	}

	return Cc_picture::window_proc(msg, wp, lp);
}

bool C_sys_wnd_solo_tweet_bitmap::on_paint()
{
	if (m_dib.get_handle())
	{
		PAINTSTRUCT ps;
		HDC h_dc = BeginPaint(get_handle(), &ps);
		SetStretchBltMode(h_dc, HALFTONE);

		// 矩形を求める
		int src_w = m_dib.get_width();
		int src_h = m_dib.get_height();
		int canvas_w = get_client_rect().width();
		int canvas_h = get_client_rect().height();
		int dst_x = 0, dst_y = 0, dst_w = 0, dst_h = 0;

		// キャンバスが横長の場合
		if (canvas_w * src_h > src_w * canvas_h)
		{
			dst_h = canvas_h;
			dst_w = dst_h * src_w / src_h;
			dst_x = (canvas_w - dst_w) / 2;
			dst_y = 0;
		}
		// キャンバスが縦長の場合
		else
		{
			dst_w = canvas_w;
			dst_h = dst_w * src_h / src_w;
			dst_x = 0;
			dst_y = (canvas_h - dst_h) / 2;
		}

		// キャンバスを塗りつぶす
		FillRect(h_dc, &C_rect(0, 0, canvas_w, canvas_h), (HBRUSH)GetStockObject(GRAY_BRUSH));
		// ビットマップを描画
		m_h_src_dc = CreateCompatibleDC(h_dc);
		HGDIOBJ h_old_obj = SelectObject(m_h_src_dc, m_dib.get_handle());
		StretchBlt(h_dc, dst_x, dst_y, dst_w, dst_h, m_h_src_dc, 0, 0, src_w, src_h, SRCCOPY),
		SelectObject(m_h_src_dc, h_old_obj);

		EndPaint(get_handle(), &ps);

		return false;
	}

	return true;
}

