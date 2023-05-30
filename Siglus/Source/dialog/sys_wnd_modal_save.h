#pragma		once

#include	"cfg_wnd_modal.h"
#include	"sys_wnd_func_saveload.h"

// ****************************************************************
// システムウィンドウ：モーダル：セーブ
// ================================================================
class C_sys_wnd_modal_save : public C_cfg_wnd_modal
{
public:
	C_sys_wnd_func_saveload			sys_wnd_func_saveload;

private:

	int		open_modal_result_int_func(HWND parent_h_wnd);		// オープンモーダルファンクション

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
