#pragma		once

#include	"cfg_wnd_base.h"
#include	"cfg_wnd_func_jitan.h"

// ****************************************************************
// コンフィグウィンドウ：ベース：時短再生
// ================================================================
class Ccfg_wnd_base_jitan : public C_cfg_wnd_base
{
public:
	void		init();						// 初期化

	Ccfg_wnd_func_jitan			cfg_wnd_func_jitan;

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
