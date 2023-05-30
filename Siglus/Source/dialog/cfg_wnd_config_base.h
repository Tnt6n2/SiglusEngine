#pragma		once

#include	"dialog/cfg_wnd_solo.h"

#include	"dialog/cfg_wnd_base_screen.h"
#include	"dialog/cfg_wnd_base_font_msgspd.h"
#include	"dialog/cfg_wnd_base_volume_bgmfade.h"
#include	"dialog/cfg_wnd_base_volume.h"
#include	"dialog/cfg_wnd_base_koemode_chrkoe.h"
#include	"dialog/cfg_wnd_base_koemode.h"
#include	"dialog/cfg_wnd_base_chrkoe.h"
#include	"dialog/cfg_wnd_base_mwndbk.h"
#include	"dialog/cfg_wnd_base_automode.h"
#include	"dialog/cfg_wnd_base_jitan.h"
#include	"dialog/cfg_wnd_base_else.h"
#include	"dialog/cfg_wnd_base_system.h"

// ****************************************************************
// コンフィグウィンドウ：ベース
// ================================================================
class C_cfg_wnd_config_base : public C_tnm_dlg
{
public:
	void		init();						// 初期化
	void		frame();					// フレーム

	ARRAY<C_cfg_wnd_solo *>::iterator	get_child_window_list_begin()		{	return child_window_list.begin();		}
	ARRAY<C_cfg_wnd_solo *>::iterator	get_child_window_list_end()			{	return child_window_list.end();			}
	ARRAY<C_cfg_wnd_solo *>::iterator	get_all_child_window_list_begin()	{	return all_child_window_list.begin();	}
	ARRAY<C_cfg_wnd_solo *>::iterator	get_all_child_window_list_end()		{	return all_child_window_list.end();		}

protected:

	bool		cfg_wnd_full_mode_flag;

	void		init_child_window();		// 子ウィンドウを初期化
	
	

private:

	void		regist_child_window();		// 子ウィンドウを登録
	void		open_child_window();		// 子ウィンドウを開く
	void		close_all_child_window(C_cfg_wnd_solo* no_close_window = NULL);	// 全ての子ウィンドウを閉じる
	void		adjust_child_window();		// 子ウィンドウを調整する

	// テキストタブ
	class C_tab_base : public Cc_tab
	{
	public:
		C_cfg_wnd_config_base *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		C_tab_base	tab_base;
		Cc_button	btn_all_config_init;
		Cc_button	btn_close;
		Cc_button	stc_game_version;
	}	ci;

	// 子ウィンドウリスト
	ARRAY<C_cfg_wnd_solo *>	all_child_window_list;
	ARRAY<C_cfg_wnd_solo *>	child_window_list;

	// 子ウィンドウ
	C_cfg_wnd_base_screen			cfg_wnd_base_screen;			// ベース：画面モード
	C_cfg_wnd_base_font_msgspd		cfg_wnd_base_font_msgspd;		// ベース：メッセージ（フォント、文字速度）
	Ccfg_wnd_base_volume_bgmfade	cfg_wnd_base_volume_bgmfade;	// ベース：音量（音量、ＢＧＭフェード）
	Ccfg_wnd_base_volume			cfg_wnd_base_volume;			// ベース：音量
	Ccfg_wnd_base_koemode_chrkoe	cfg_wnd_base_koemode_chrkoe;	// ベース：音声（音声モード、キャラクター音声）
	Ccfg_wnd_base_koemode			cfg_wnd_base_koemode;			// ベース：音声モード
	Ccfg_wnd_base_chrkoe			cfg_wnd_base_chrkoe;			// ベース：キャラクター音声
	Ccfg_wnd_base_mwndbk			cfg_wnd_base_mwndbk;			// ベース：ウィンドウ背景色
	Ccfg_wnd_base_automode			cfg_wnd_base_automode;			// ベース：オートモード
	Ccfg_wnd_base_jitan				cfg_wnd_base_jitan;				// ベース：時短再生
	Ccfg_wnd_base_else				cfg_wnd_base_else;				// ベース：その他
	Ccfg_wnd_base_system			cfg_wnd_base_system;			// ベース：システム

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知
	bool		on_mousewheel(WPARAM wp, LPARAM lp);			// マウスホイール
};
