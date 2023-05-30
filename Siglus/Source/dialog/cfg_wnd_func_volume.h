#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：音量
// ================================================================
class Ccfg_wnd_func_volume : public C_cfg_wnd_func
{
public:
	void		on_init_funcfunc();										// 初期化
	void		on_close_funcfunc();									// 閉じる
	bool		on_command_funcfunc(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify_funcfunc(WPARAM wp, LPARAM lp);				// 通知
	bool		on_mousewheel_funcfunc(WPARAM wp, LPARAM lp);			// マウスホイール
	bool		on_h_scroll_funcfunc(WPARAM wp, LPARAM lp);				// Ｈスクロール
	bool		on_v_scroll_funcfunc(WPARAM wp, LPARAM lp);				// Ｖスクロール
	LRESULT		on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp);		// スタティックコントロールのカラー変更
	void		frame_funcfunc();										// フレーム

	void		init_state();				// 初期状態に戻す

	void		rep_window_size_func(int x_rep);		// 「ウィンドウサイズ」と「初期化ボタン」と「閉じるボタン」を調整する

	int			bgmfade_item_x_rep;						// ＢＧＭフェードの座標調整用に記憶

private:

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_group	grp_volume;
		Cc_group	grp_volume_all;
		Cc_slider	sld_volume_all;
		Cc_check	chk_volume_all;
		Cc_group	grp_volume_bgm;
		Cc_slider	sld_volume_bgm;
		Cc_check	chk_volume_bgm;
		Cc_group	grp_volume_koe;
		Cc_slider	sld_volume_koe;
		Cc_check	chk_volume_koe;
		Cc_group	grp_volume_pcm;
		Cc_slider	sld_volume_pcm;
		Cc_check	chk_volume_pcm;
		Cc_group	grp_volume_se;
		Cc_slider	sld_volume_se;
		Cc_check	chk_volume_se;
		Cc_group	grp_volume_mov;
		Cc_slider	sld_volume_mov;
		Cc_check	chk_volume_mov;
		Cc_button	btn_volume_init;
		Cc_button	btn_volume_close;
	}	ci;

	// 作業変数
	struct WORK
	{
	}	work;

	void		setup_dialog();				// ダイアログを構築
	void		update_dialog();			// ダイアログを更新
};
