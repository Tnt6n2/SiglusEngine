#pragma		once
#include	"data/tnm_dlg.h"
#include	"dialog/cfg_wnd_func_mod.h"

// ****************************************************************
// コンフィグウィンドウ：ソロ
// ================================================================
class C_cfg_wnd_solo : public C_tnm_dlg
{

public:

	C_cfg_wnd_solo()	{	cfg_wnd_full_mode_flag = false;	}		// コンストラクタ

	void		refresh_dialog_func(int dialog_width, int dialog_height);

	bool		cfg_wnd_full_mode_flag;

	// 仮想関数
	virtual		void		frame();											// フレーム

protected:

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);					// ウィンドウプロシージャ

	// 仮想関数
	virtual		bool		on_init();											// 初期化
	virtual		bool		on_close();											// 閉じる

	// 純粋仮想関数
	virtual		bool		on_init_func() = 0;									// 初期化
	virtual		bool		on_close_func() = 0;								// 閉じる
	virtual		bool		on_command(WPARAM wp, LPARAM lp);					// コマンド
	virtual		bool		on_notify(WPARAM wp, LPARAM lp);					// 通知
	virtual		bool		on_mousewheel(WPARAM wp, LPARAM lp);				// マウスホイール
	virtual		bool		on_h_scroll(WPARAM wp, LPARAM lp);					// Ｈスクロール
	virtual		bool		on_v_scroll(WPARAM wp, LPARAM lp);					// Ｖスクロール
	virtual		LRESULT		on_ctlcolorstatic(WPARAM wp, LPARAM lp);			// スタティックコントロールのカラー変更
	virtual		void		frame_func();										// フレーム
};

