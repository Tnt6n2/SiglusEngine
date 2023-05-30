#pragma		once

#include	"cfg_wnd_solo.h"
#include	"cfg_wnd_func_koemode.h"
#include	"cfg_wnd_func_chrkoe.h"

// ****************************************************************
// コンフィグウィンドウ：ソロ：音声（音声モード、キャラクター音声）
// ================================================================
class Ccfg_wnd_solo_koemode_chrkoe : public C_cfg_wnd_solo
{
public:
	void		init();						// 初期化

	Ccfg_wnd_func_koemode	cfg_wnd_func_koemode;
	Ccfg_wnd_func_chrkoe	cfg_wnd_func_chrkoe;

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
