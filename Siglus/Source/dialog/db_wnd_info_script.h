#pragma		once

#include	"data/tnm_proc_def.h"
#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：スクリプト情報
// ================================================================
class C_db_wnd_info_script : public C_db_wnd_info_listview
{
public:
	void		init();		// 初期化
	void		frame();	// フレーム

private:

	// ステータス
	struct STATE
	{
		int				cur_scn_no;							// 現在のシーン番号
		int				cur_line_no;						// 現在の行番号
		TNM_PROC_TYPE	proc_type;							// 現在のプロセス
		bool			syscom_menu_disable;				// システムコマンドメニュー禁止
		bool			hide_mwnd_disable;					// メッセージウィンドウを隠す禁止
		bool			msg_back_disable;					// メッセージバック禁止
		bool			shortcut_disable;					// ショートカット禁止
		bool			mwnd_btn_disable_all;				// ウィンドウボタン禁止
		bool			mwnd_btn_touch_disable;				// ウィンドウボタン触れない
		bool			skip_disable;						// スキップ禁止
		bool			ctrl_disable;						// Ctrl スキップ禁止
		bool			not_stop_skip_by_click;				// クリックしてもスキップを止めない
		bool			not_skip_msg_by_click;				// クリックしてもメッセージを一括表示しない
		bool			skip_unread_message;				// Ctrl スキップ禁止
		bool			msg_back_off;						// メッセージバックを使わない
		bool			msg_back_disp_off;					// メッセージバックを非表示にする
		bool			auto_mode_flag;						// オートモード
		int				msg_speed;							// メッセージスピード
		bool			msg_nowait;							// メッセージノーウェイト
		bool			async_msg_mode;						// 非同期メッセージモード
		bool			multi_msg_mode;						// マルチメッセージモード
		bool			skip_trigger;						// スキップトリガー
		bool			cursor_disp_off;					// カーソルを消す
		bool			cursor_move_by_key_disable;			// キーボードでカーソルを動かさない
		bool			mwnd_anime_on_flag;					// メッセージウィンドウのアニメを行うフラグ
		bool			mwnd_anime_off_flag;				// メッセージウィンドウのアニメを行わないフラグ
		bool			mwnd_disp_off_flag;					// メッセージウィンドウを消すフラグ
		bool			koe_dont_stop_on_flag;				// 声を止めないオンフラグ
		bool			koe_dont_stop_off_flag;				// 声を止めないオフフラグ
		bool			quake_stop_flag;					// クエイクを止めるフラグ
		bool			cg_table_off_flag;					// ＣＧテーブルを使わないフラグ
		bool			bgmfade_flag;						// ＢＧＭフェード中
		bool			dont_set_save_point;				// セーブポイントを設定しない
		bool			wait_display_vsync_off_flag;		// ディスプレイの垂直同期を待つをオフにするフラグ

		bool			syscom_read_skip;					// （ｼｽﾃﾑｺﾏﾝﾄﾞ）既読早送り
		bool			syscom_auto_skip;					// （ｼｽﾃﾑｺﾏﾝﾄﾞ）自動早送り
		bool			syscom_auto_mode;					// （ｼｽﾃﾑｺﾏﾝﾄﾞ）オートモード
		bool			syscom_hide_mwnd;					// （ｼｽﾃﾑｺﾏﾝﾄﾞ）メッセージウィンドウを隠す
		bool			syscom_msg_back;					// （ｼｽﾃﾑｺﾏﾝﾄﾞ）メッセージバック

		bool			syscom_read_skip_disable;			// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）既読早送り禁止
		bool			syscom_auto_skip_disable;			// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）自動早送り禁止
		bool			syscom_auto_mode_disable;			// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）オートモード禁止
		bool			syscom_hide_mwnd_disable;			// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）メッセージウィンドウを隠す禁止
		bool			syscom_msg_back_disable;			// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）メッセージバック禁止
		bool			syscom_save_disable;				// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）セーブ禁止
		bool			syscom_load_disable;				// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）ロード禁止
		bool			syscom_return_to_sel_disable;		// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）前の選択肢に戻る禁止

		bool			syscom_read_skip_off;				// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）既読早送り禁止
		bool			syscom_auto_skip_off;				// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）自動早送り禁止
		bool			syscom_auto_mode_off;				// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）オートモード禁止
		bool			syscom_hide_mwnd_off;				// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）メッセージウィンドウを消す禁止
		bool			syscom_msg_back_off;				// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）メッセージバック禁止
		bool			syscom_save_off;					// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）セーブ非表示
		bool			syscom_load_off;					// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）ロード非表示
		bool			syscom_return_to_sel_off;			// （ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ）前の選択肢に戻る禁止

	}	m_ld;

	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

