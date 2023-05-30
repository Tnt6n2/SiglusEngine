#pragma		once

#include	"cfg_wnd_modal.h"
#include	"cfg_wnd_func_screen.h"

// ****************************************************************
// コンフィグウィンドウ：モーダル：画面モード
// ================================================================
class C_cfg_wnd_modal_screen : public C_cfg_wnd_modal
{
public:
	C_cfg_wnd_func_screen		cfg_wnd_func_screen;

private:

	void		open_modal_func(HWND parent_h_wnd);				// オープンモーダルファンクション

	bool		on_init_func();									// 初期化
	bool		on_close_func();								// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_timer_func(WPARAM wp, LPARAM lp);			// タイマー
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知
	bool		on_mousewheel(WPARAM wp, LPARAM lp);			// マウスホイール
	bool		on_h_scroll(WPARAM wp, LPARAM lp);				// Ｈスクロール
	bool		on_v_scroll(WPARAM wp, LPARAM lp);				// Ｖスクロール
	LRESULT		on_ctlcolorstatic(WPARAM wp, LPARAM lp);		// スタティックコントロールのカラー変更
};
