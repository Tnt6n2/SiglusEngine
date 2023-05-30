#pragma		once

#include	"cfg_wnd_solo.h"

// ****************************************************************
// システムウィンドウ：ソロ：ツイート
// ================================================================

class C_sys_wnd_solo_tweet_bitmap : public Cc_picture
{
public:
	C_sys_wnd_solo_tweet_bitmap();

	C_dib		m_dib;
	HDC			m_h_src_dc;
protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
private:
	bool		on_paint();					// 描画
};


class C_sys_wnd_solo_tweet : public C_cfg_wnd_solo
{
public:

	C_sys_wnd_solo_tweet_bitmap	m_stc_bitmap;

	Cc_static	m_stc_user_name;
	Cc_edit		m_edt_tweet_text;
	Cc_button	m_btn_authorize;
	Cc_button	m_btn_tweet;

	void		init();						// 初期化
	void		update_state();				// ステートを初期化
	void		on_success_tweet();			// ツイートに成功

private:

	bool		on_init_func();									// 初期化
	bool		on_close_func();								// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド
};
