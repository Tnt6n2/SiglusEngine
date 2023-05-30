#pragma		once

#include	"data/tnm_def.h"

// ****************************************************************
// システムコマンドメニュー
// ================================================================
//		ローカルにセーブされます。
// ================================================================
struct S_tnm_syscommenu
{
	// 存在・許可／禁止
	struct S_existenable
	{
		bool	exist;		// 存在の有無
		bool	enable;		// 許可／禁止
	};

	// 存在・許可／禁止・ON/OFF状態
	struct S_existenableonoff
	{
		bool	exist;		// 存在の有無
		bool	enable;		// 許可／禁止
		bool	onoff;		// ON/OFF状態
	};

	// 存在・許可／禁止・モード値
	struct S_existenablemode
	{
		bool	exist;		// 存在の有無
		bool	enable;		// 許可／禁止
		int		mode;		// モード値
	};

	S_existenable		read_skip;		// 一度読んだ文章の早送りを開始します。
	S_existenable		unread_skip;	// 未読の文章でも早送りを開始します。
	S_existenable		auto_skip;		// 自動早送り機能をＯＮ／ＯＦＦします。
	S_existenable		auto_mode;		// オートモードを開始します。
	S_existenable		hide_mwnd;		// メッセージウィンドウを隠します。
	S_existenable		msg_back;		// メッセージバックを表示します。
	S_existenable		save;			// セーブを呼び出します。
	S_existenable		load;			// ロードを呼び出します。
	S_existenable		return_to_sel;	// 前の選択肢に戻ります。
	S_existenable		config;			// 環境設定を呼び出します。
	S_existenable		manual;			// マニュアルを表示します。
	S_existenable		version;		// バージョン情報を表示します。
	S_existenable		return_to_menu;	// タイトルメニューに戻ります。
	S_existenable		game_end;		// ゲームを終了します。
	S_existenable		cancel;			// システムコマンドメニューをキャンセルします。

	S_existenableonoff	local_extra_switch[TNM_LOCAL_EXTRA_SWITCH_CNT];		// ローカル汎用スイッチ
	S_existenablemode	local_extra_mode[TNM_LOCAL_EXTRA_MODE_CNT];			// ローカル汎用モード
};

// ****************************************************************
// システムコマンド
// ================================================================
class C_tnm_syscom
{
public:
	bool				read_skip_flag;		// 既読早送り
	bool				auto_skip_flag;		// 自動早送り
	bool				hide_mwnd_flag;		// メッセージウィンドウを隠す
	bool				msg_back_flag;		// メッセージバックを開く
	S_tnm_syscommenu	syscommenu;			// システムコマンドメニュー
};
