#pragma		once

#include	"cfg_wnd_solo.h"
#include	"cfg_wnd_func_font.h"
#include	"cfg_wnd_func_msgspd.h"

// ****************************************************************
// コンフィグウィンドウ：ベース：メッセージ（フォント、文字速度）
// ================================================================
class C_cfg_wnd_solo_font_msgspd : public C_cfg_wnd_solo
{

public:

	void		init();						// 初期化

	C_cfg_wnd_func_font		cfg_wnd_func_font;
	C_cfg_wnd_func_msgspd	cfg_wnd_func_msgspd;

private:

	bool		on_init_func();									// 初期化
	bool		on_close_func();								// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知
	bool		on_mousewheel(WPARAM wp, LPARAM lp);			// マウスホイール
	bool		on_h_scroll(WPARAM wp, LPARAM lp);				// Ｈスクロール
	bool		on_v_scroll(WPARAM wp, LPARAM lp);				// Ｖスクロール
	LRESULT		on_ctlcolorstatic(WPARAM wp, LPARAM lp);		// スタティックコントロールのカラー変更

	void		frame_func();									// フレーム
};
