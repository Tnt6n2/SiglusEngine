#pragma		once

#include	"dialog/cfg_wnd_func_mod.h"

// ****************************************************************
// コンフィグウィンドウ：タイマー定義
// ================================================================
#define		CFG_WND_MODAL_TIMER_ID					12345
#define		CFG_WND_MODAL_TIMER_TIME				50
#define		CFG_WND_MODAL_TIMER_TIME_FOR_MSGSPD		10

// ****************************************************************
// コンフィグウィンドウ：モーダル
// ================================================================
class C_cfg_wnd_modal : public C_modal_dialog
{
public:
	void		open_modal(HWND parent_h_wnd = G_app.h_wnd);					// オープンモーダル
	bool		open_modal_result_bool(HWND parent_h_wnd = G_app.h_wnd);		// オープンモーダル
	int			open_modal_result_int(HWND parent_h_wnd = G_app.h_wnd);			// オープンモーダル

protected:

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);					// ウィンドウプロシージャ

private:

	bool		on_init();														// 初期化
	bool		on_close();														// 閉じる
	bool		on_timer(WPARAM wp, LPARAM lp);									// タイマー

	// 仮想関数
	virtual		void		open_modal_func(HWND parent_h_wnd);					// オープンモーダルファンクション
	virtual		bool		open_modal_result_bool_func(HWND parent_h_wnd);		// オープンモーダルファンクション
	virtual		int			open_modal_result_int_func(HWND parent_h_wnd);		// オープンモーダルファンクション

	// 純粋仮想関数
	virtual		bool		on_init_func() = 0;									// 初期化
	virtual		bool		on_close_func() = 0;								// 閉じる
	virtual		bool		on_command(WPARAM wp, LPARAM lp) = 0;				// コマンド
	virtual		bool		on_timer_func(WPARAM wp, LPARAM lp) = 0;			// タイマー
	virtual		bool		on_notify(WPARAM wp, LPARAM lp) = 0;				// 通知
	virtual		bool		on_mousewheel(WPARAM wp, LPARAM lp) = 0;			// マウスホイール
	virtual		bool		on_h_scroll(WPARAM wp, LPARAM lp) = 0;				// Ｈスクロール
	virtual		bool		on_v_scroll(WPARAM wp, LPARAM lp) = 0;				// Ｖスクロール
	virtual		LRESULT		on_ctlcolorstatic(WPARAM wp, LPARAM lp) = 0;		// スタティックコントロールのカラー変更

};

