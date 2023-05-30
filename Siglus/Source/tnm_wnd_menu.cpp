#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_rmenu.h"
#include	"data/tnm_wnd_menu.h"
#include	"data/tnm_dlg_manager.h"
#include	"data/tnm_flag_ini.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_dialog.h"

// ****************************************************************
// ＩＤ定義
// ================================================================

// システム
const int	IDM_SYSTEM									= 1000;
const int	IDM_SYSTEM_SYSCOMMENU						= 1001;
const int	IDM_SYSTEM_CONFIG							= 1002;
const int	IDM_SYSTEM_SAVE_MODAL_DIALOG				= 1003;
const int	IDM_SYSTEM_LOAD_MODAL_DIALOG				= 1004;
const int	IDM_SYSTEM_CONFIG_SOLO						= 1005;
const int	IDM_SYSTEM_CONFIG_MODAL						= 1006;
const int	IDM_SYSTEM_CONFIG_SUPPORT					= 1007;
const int	IDM_SYSTEM_DEBUG							= 1008;
const int	IDM_SYSTEM_TWEET_SOLO_DIALOG				= 1009;

const int	IDM_SYSTEM_CONFIG_SOLO_SCREEN				= 1200;
const int	IDM_SYSTEM_CONFIG_SOLO_FONT_MSGSPD			= 1201;
const int	IDM_SYSTEM_CONFIG_SOLO_FONT					= 1202;
const int	IDM_SYSTEM_CONFIG_SOLO_MSGSPD				= 1203;
const int	IDM_SYSTEM_CONFIG_SOLO_VOLUME_BGMFADE		= 1204;
const int	IDM_SYSTEM_CONFIG_SOLO_VOLUME				= 1205;
const int	IDM_SYSTEM_CONFIG_SOLO_BGMFADE				= 1206;
const int	IDM_SYSTEM_CONFIG_SOLO_KOEMODE_CHRKOE		= 1207;
const int	IDM_SYSTEM_CONFIG_SOLO_KOEMODE				= 1208;
const int	IDM_SYSTEM_CONFIG_SOLO_CHRKOE				= 1209;
const int	IDM_SYSTEM_CONFIG_SOLO_MWNDBK				= 1210;
const int	IDM_SYSTEM_CONFIG_SOLO_AUTOMODE				= 1211;
const int	IDM_SYSTEM_CONFIG_SOLO_JITAN				= 1212;
const int	IDM_SYSTEM_CONFIG_SOLO_ELSE					= 1213;
const int	IDM_SYSTEM_CONFIG_SOLO_SYSTEM				= 1214;
const int	IDM_SYSTEM_CONFIG_SOLO_MOVTYPE				= 1215;

const int	IDM_SYSTEM_CONFIG_SOLO_ALL_OPEN				= 1250;
const int	IDM_SYSTEM_CONFIG_SOLO_ALL_CLOSE			= 1251;

const int	IDM_SYSTEM_CONFIG_MODAL_SCREEN				= 1300;
const int	IDM_SYSTEM_CONFIG_MODAL_FONT_MSGSPD			= 1301;
const int	IDM_SYSTEM_CONFIG_MODAL_FONT				= 1302;
const int	IDM_SYSTEM_CONFIG_MODAL_MSGSPD				= 1303;
const int	IDM_SYSTEM_CONFIG_MODAL_VOLUME_BGMFADE		= 1304;
const int	IDM_SYSTEM_CONFIG_MODAL_VOLUME				= 1305;
const int	IDM_SYSTEM_CONFIG_MODAL_BGMFADE				= 1306;
const int	IDM_SYSTEM_CONFIG_MODAL_KOEMODE_CHRKOE		= 1307;
const int	IDM_SYSTEM_CONFIG_MODAL_KOEMODE				= 1308;
const int	IDM_SYSTEM_CONFIG_MODAL_CHRKOE				= 1309;
const int	IDM_SYSTEM_CONFIG_MODAL_MWNDBK				= 1310;
const int	IDM_SYSTEM_CONFIG_MODAL_AUTOMODE			= 1311;
const int	IDM_SYSTEM_CONFIG_MODAL_JITAN				= 1312;
const int	IDM_SYSTEM_CONFIG_MODAL_ELSE				= 1313;
const int	IDM_SYSTEM_CONFIG_MODAL_SYSTEM				= 1314;
const int	IDM_SYSTEM_CONFIG_MODAL_MOVTYPE				= 1315;

const int	IDM_SYSTEM_DEBUG_SYSCOMMENU_FULL_MODE		= 1400;
const int	IDM_SYSTEM_DEBUG_CONFIG_FULL_MODE			= 1401;
const int	IDM_SYSTEM_DEBUG_SAVE_SOLO_DIALOG			= 1402;
const int	IDM_SYSTEM_DEBUG_LOAD_SOLO_DIALOG			= 1403;

// デバッグ
const int	IDM_DEBUG									= 2000;
const int	IDM_DEBUG_RESTART							= 2001;		// 最初から始める
const int	IDM_DEBUG_TRACE								= 2002;		// トレース
const int	IDM_DEBUG_DRAW_OBJECT_HIT_RECT				= 2003;		// オブジェクト当たり矩形の表示

// ウィンドウ
const int	IDM_WINDOW									= 3000;
const int	IDM_WINDOW_DB_WND_INFO_DEBUG				= 3001;		// デバッグ設定
const int	IDM_WINDOW_DB_WND_INFO_SWITCH				= 3002;		// スイッチ
const int	IDM_WINDOW_DB_WND_INFO_SCN_CONTROL			= 3003;		// シーンコントロール
const int	IDM_WINDOW_DB_WND_INFO_CALL_HISTORY			= 3004;		// 呼び出し履歴
const int	IDM_WINDOW_DB_WND_INFO_SAVE_HISTORY			= 3005;		// セーブポイント履歴
const int	IDM_WINDOW_DB_WND_INFO_TRACE				= 3006;		// トレース
const int	IDM_WINDOW_DB_WND_INFO_ERROR				= 3007;		// エラー情報
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM				= 3008;		// システム情報
const int	IDM_WINDOW_DB_WND_INFO_INPUT				= 3009;		// 入力情報
const int	IDM_WINDOW_DB_WND_INFO_SCRIPT				= 3010;		// スクリプト情報
const int	IDM_WINDOW_DB_WND_INFO_NAMAE				= 3011;		// 【名前】情報
const int	IDM_WINDOW_DB_WND_INFO_READ_FLAG			= 3012;		// 既読フラグ情報
const int	IDM_WINDOW_DB_WND_INFO_CG_TABLE				= 3013;		// ＣＧテーブル情報
const int	IDM_WINDOW_DB_WND_INFO_BGM_TABLE			= 3014;		// ＢＧＭテーブル情報
const int	IDM_WINDOW_DB_WND_INFO_ELEMENT				= 3015;		// エレメント情報
const int	IDM_WINDOW_DB_WND_INFO_FLAG					= 3016;		// フラグ情報
const int	IDM_WINDOW_DB_WND_INFO_FLAG_INI				= 3017;		// Flag.ini 情報
const int	IDM_WINDOW_DB_WND_INFO_USER_FLAG			= 3018;		// ユーザフラグ情報
const int	IDM_WINDOW_DB_WND_INFO_USER_CALL_FLAG		= 3019;		// ユーザコールフラグ情報
const int	IDM_WINDOW_DB_WND_INFO_NAMAE_FLAG			= 3020;		// 名前フラグ情報
const int	IDM_WINDOW_DB_WND_INFO_COUNTER				= 3021;		// カウンタ情報
const int	IDM_WINDOW_DB_WND_INFO_BUTTON				= 3022;		// ボタン情報
const int	IDM_WINDOW_DB_WND_INFO_GROUP				= 3023;		// グループ情報
const int	IDM_WINDOW_DB_WND_INFO_WORLD				= 3024;		// ワールド情報
const int	IDM_WINDOW_DB_WND_INFO_OBJECT				= 3025;		// オブジェクト情報
const int	IDM_WINDOW_DB_WND_INFO_MWND					= 3026;		// メッセージウィンドウ情報
const int	IDM_WINDOW_DB_WND_INFO_SOUND				= 3027;		// サウンド情報
const int	IDM_WINDOW_DB_WND_INFO_VOLUME_CHANNEL		= 3028;		// ボリュームチャンネル情報
const int	IDM_WINDOW_DB_WND_INFO_KOE                  = 3029;		// 声ウォッチ情報
const int	IDM_WINDOW_DB_WND_INFO_TEST                 = 3030;		// テスト
const int	IDM_WINDOW_DB_WND_INFO_TEST_TREEVIEW		= 3031;		// テストツリービュー

// ウィンドウ：Direct3D 情報
const int	IDM_WINDOW_DB_WND_INFO_D3D_DEVICE			= 3100;		// Direct3D デバイス情報
const int	IDM_WINDOW_DB_WND_INFO_D3D_RESOURCE			= 3101;		// Direct3D リソース情報

// ウィンドウ：システム情報
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM_D3D			= 3200;		// Direct3D情報
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM		= 3201;		// システム情報
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM_DIR			= 3202;		// ディレクトリ情報
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM_STACK			= 3203;		// スタック情報
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM_PROC_STACK	= 3204;		// プロセススタック情報

// ウィンドウ：フラグ
const int	IDM_WINDOW_DB_WND_INFO_FLAG_TOP				= 3300;		// フラグ

// ウィンドウ：Flag.ini
const int	IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP			= 3320;		// Flag.ini

// ウィンドウ：オブジェクト
const int	IDM_WINDOW_DB_WND_INFO_OBJECT_TOP			= 3350;		// オブジェクト

// コマンド
const int	IDM_COMMAND									= 4000;

// システムコマンドメニュー
const int	IDM_SYSTEM_COMMAND_MENU						= 5000;


// ****************************************************************
// ウィンドウメニュー
// ================================================================

// メニュー作成
void C_tnm_wnd_menu::create()
{
	// システム：システムコマンドメニュー
	sub_menu_system_syscommenu.create();

	// システム：個別の環境設定
	sub_menu_system_config_solo.create();
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_SCREEN, _T("画面の設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_VOLUME_BGMFADE, _T("音量の設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_VOLUME, _T(" ├ 音量の設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_BGMFADE, _T(" └ ＢＧＭフェードの設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_FONT_MSGSPD, _T("文章の設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_FONT, _T(" ├ フォントの設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_MSGSPD, _T(" └ 文字速度の設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_MWNDBK, _T("ウィンドウの背景色の設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_KOEMODE_CHRKOE, _T("音声の設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_KOEMODE, _T(" ├ 音声モードの設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_CHRKOE, _T(" └ キャラクター音声の設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_AUTOMODE, _T("オートモードの設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_JITAN, _T("時短再生の設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_ELSE, _T("その他の設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_SYSTEM, _T("システムの設定"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_MOVTYPE, _T("ムービーの再生方法の設定"), true);

	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_ALL_OPEN, _T("★全て開く"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_ALL_CLOSE, _T("★全て閉じる"), true);

	// システム：個別の環境設定（モーダル版）
	sub_menu_system_config_modal.create();
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_SCREEN, _T("画面の設定（▲設定を変更するとバグる→対応無理）"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_VOLUME_BGMFADE, _T("音量の設定（▲音量がリアルタイムで変わらない→何とかしたい）"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_VOLUME, _T(" ├ 音量の設定（▲音量がリアルタイムで変わらない→何とかしたい）"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_BGMFADE, _T(" └ ＢＧＭフェードの設定（▲音量がリアルタイムで変わらない→何とかしたい）"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_FONT_MSGSPD, _T("文章の設定（▲フォントがリアルタイムで変わらない→対応無理）"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_FONT, _T(" ├ フォントの設定（▲フォントがリアルタイムで変わらない→対応無理）"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_MSGSPD, _T(" └ 文字速度の設定"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_MWNDBK, _T("ウィンドウの背景色の設定（▲背景色がリアルタイムで変わらない→調整用サンプル表示）"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_KOEMODE_CHRKOE, _T("音声の設定"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_KOEMODE, _T(" ├ 音声モードの設定"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_CHRKOE, _T(" └ キャラクター音声の設定"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_AUTOMODE, _T("オートモードの設定"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_JITAN, _T("時短再生の設定"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_ELSE, _T("その他の設定"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_SYSTEM, _T("システムの設定"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_MOVTYPE, _T("ムービーの再生方法の設定（★サポート用）"), true);

	// システム：デバッグ用：全システムコマンドメニュー
	sub_menu_system_debug_syscommenu_full_mode.create();

	// システム：デバッグ用
	sub_menu_system_debug.create();
	sub_menu_system_debug.add_sub_menu(IDM_SYSTEM_SYSCOMMENU, _T("全システムコマンドメニュー（▲未実装）"), sub_menu_system_debug_syscommenu_full_mode.get_handle(), true);
	sub_menu_system_debug.add_item(IDM_SYSTEM_DEBUG_CONFIG_FULL_MODE, _T("全環境設定"), true);
	sub_menu_system_debug.add_item(IDM_SYSTEM_DEBUG_SAVE_SOLO_DIALOG, _T("同期版セーブダイアログ"), true);
	sub_menu_system_debug.add_item(IDM_SYSTEM_DEBUG_LOAD_SOLO_DIALOG, _T("同期版ロードダイアログ"), true);

	// システム
	sub_menu_system.create();
	sub_menu_system.add_sub_menu(IDM_SYSTEM_DEBUG_SYSCOMMENU_FULL_MODE, _T("システムコマンドメニュー"), sub_menu_system_syscommenu.get_handle(), true);
	sub_menu_system.add_item(IDM_SYSTEM_CONFIG, _T("環境設定"), true);
	sub_menu_system.add_item(IDM_SYSTEM_SAVE_MODAL_DIALOG, _T("セーブダイアログ"), true);
	sub_menu_system.add_item(IDM_SYSTEM_LOAD_MODAL_DIALOG, _T("ロードダイアログ"), true);
	sub_menu_system.add_sub_menu(IDM_SYSTEM_CONFIG_SOLO, _T("個別の環境設定"), sub_menu_system_config_solo.get_handle(), true);
	sub_menu_system.add_sub_menu(IDM_SYSTEM_CONFIG_MODAL, _T("個別の環境設定（モーダル版）"), sub_menu_system_config_modal.get_handle(), true);
	sub_menu_system.add_item(IDM_SYSTEM_TWEET_SOLO_DIALOG, _T("ツイートダイアログ"), true);
	sub_menu_system.add_sub_menu(IDM_SYSTEM_DEBUG, _T("★デバッグ用"), sub_menu_system_debug.get_handle(), true);

	// デバッグ
	sub_menu_debug.create();
	sub_menu_debug.add_item(IDM_DEBUG_RESTART, _T("最初から始める ( F1 )"), true);
	sub_menu_debug.add_item(IDM_DEBUG_TRACE, _T("トレース ( F3 )"), true);
	sub_menu_debug.add_item(IDM_DEBUG_DRAW_OBJECT_HIT_RECT, _T("オブジェクト当たり矩形の表示"), true);

	// ウィンドウ：Direct3D 情報
	sub_menu_window_d3d.create();
	sub_menu_window_d3d.add_item(IDM_WINDOW_DB_WND_INFO_D3D_DEVICE, _T("デバイス情報"), true);
	sub_menu_window_d3d.add_item(IDM_WINDOW_DB_WND_INFO_D3D_RESOURCE, _T("リソース情報"), true);

	// ウィンドウ：システム情報
	sub_menu_window_system.create();
	sub_menu_window_system.add_sub_menu(IDM_WINDOW_DB_WND_INFO_SYSTEM_D3D, _T("Direct3D 情報"), sub_menu_window_d3d.get_handle(), true);
	sub_menu_window_system.add_item(IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM, _T("システム情報"), true);
	sub_menu_window_system.add_item(IDM_WINDOW_DB_WND_INFO_SYSTEM_DIR, _T("ディレクトリ情報"), true);
	sub_menu_window_system.add_item(IDM_WINDOW_DB_WND_INFO_SYSTEM_STACK, _T("スタック情報"), true);
	sub_menu_window_system.add_item(IDM_WINDOW_DB_WND_INFO_SYSTEM_PROC_STACK, _T("プロセススタック情報"), true);

	// ウィンドウ：フラグ
	sub_menu_window_flag.create();
	TSTR flag_name[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("X"), _T("S"), _T("G"), _T("Z"), _T("M"), _T("L"), _T("K")};
	for (int i = 0; i < TNM_FLAG_CNT; i++)	{
		sub_menu_window_flag.add_item(IDM_WINDOW_DB_WND_INFO_FLAG_TOP + i, str_format(_T("フラグ%s"), flag_name[i].c_str()));
	}

	// ウィンドウ：Flag.ini
	sub_menu_window_flag_ini.create();
	for (int i = 0; i < 20; i++)	{
		if (Gp_flag_ini->page[i].title.empty())	{
			sub_menu_window_flag_ini.add_item(IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP + i, str_format(_T("フラグ%02d"), i));
		}
		else	{
			sub_menu_window_flag_ini.add_item(IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP + i, Gp_flag_ini->page[i].title);
		}
	}

	// ウィンドウ：オブジェクト
	sub_menu_window_object.create();
	TSTR object_name[] = {_T("back.object"), _T("front.object"), _T("next.object"), _T("excall.back.object"), _T("excall.front.object"), _T("excall.next.object")};
	for (int i = 0; i < 6; i++)	{
		sub_menu_window_object.add_item(IDM_WINDOW_DB_WND_INFO_OBJECT_TOP + i, object_name[i]);
	}

	// ウィンドウ
	sub_menu_window.create();
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_DEBUG, _T("デバッグ設定"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_SWITCH, _T("デバッグスイッチ"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_SCN_CONTROL, _T("シーンコントロール"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_CALL_HISTORY, _T("呼び出し履歴"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_SAVE_HISTORY, _T("セーブポイント履歴"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_TRACE, _T("トレースウィンドウ"), true);
	sub_menu_window.add_separator();
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_ERROR, _T("エラー情報 ( F8 )"), true);
	sub_menu_window.add_sub_menu(IDM_WINDOW_DB_WND_INFO_SYSTEM, _T("システム情報"), sub_menu_window_system.get_handle(), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_SCRIPT, _T("ゲーム情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_INPUT, _T("入力情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_BUTTON, _T("ボタン情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_NAMAE, _T("【名前】情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_READ_FLAG, _T("既読フラグ情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_CG_TABLE, _T("ＣＧテーブル情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_BGM_TABLE, _T("ＢＧＭテーブル情報"), true);
	sub_menu_window.add_separator();
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_ELEMENT, _T("★エレメント情報"), true);
	sub_menu_window.add_sub_menu(IDM_WINDOW_DB_WND_INFO_FLAG, _T("フラグ情報"), sub_menu_window_flag.get_handle(), true);
	sub_menu_window.add_sub_menu(IDM_WINDOW_DB_WND_INFO_FLAG_INI, _T("Flag.ini 情報"), sub_menu_window_flag_ini.get_handle(), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_USER_FLAG, _T("ユーザフラグ情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_USER_CALL_FLAG, _T("ユーザコールフラグ情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_NAMAE_FLAG, _T("名前フラグ情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_COUNTER, _T("カウンタ情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_WORLD, _T("ワールド情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_GROUP, _T("グループ情報"), true);
	sub_menu_window.add_sub_menu(IDM_WINDOW_DB_WND_INFO_OBJECT, _T("オブジェクト情報"), sub_menu_window_object.get_handle(), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_MWND, _T("メッセージウィンドウ情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_SOUND, _T("サウンド情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_VOLUME_CHANNEL, _T("ボリュームチャンネル情報"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_KOE, _T("声ウォッチ情報"), true);

	// コマンド
	sub_menu_command.create();

	// 本体
	C_menu::create();
	add_sub_menu(IDM_SYSTEM, _T("ｼｽﾃﾑ"), sub_menu_system.get_handle(), true);
	add_sub_menu(IDM_DEBUG, _T("ﾃﾞﾊﾞｯｸﾞ"), sub_menu_debug.get_handle(), true);
	add_sub_menu(IDM_WINDOW, _T("ｳｨﾝﾄﾞｳ"), sub_menu_window.get_handle(), true);
	add_sub_menu(IDM_COMMAND, _T("ｺﾏﾝﾄﾞ"), sub_menu_command.get_handle(), true);
}

// メニュー破棄
void C_tnm_wnd_menu::destroy()
{
	// システム
	sub_menu_system_debug_syscommenu_full_mode.destroy();
	sub_menu_system_debug.destroy();
	sub_menu_system_config_modal.destroy();
	sub_menu_system_config_solo.destroy();
	sub_menu_system_syscommenu.destroy();
	sub_menu_system.destroy();

	// デバッグ
	sub_menu_debug.destroy();

	// ウィンドウ
	sub_menu_window_system.destroy();
	sub_menu_window_d3d.destroy();
	sub_menu_window.destroy();

	// コマンド
	sub_menu_command.destroy();

	// 本体
	C_menu::destroy();
}

// ****************************************************************
// コマンド
// ================================================================
void C_tnm_wnd_menu::on_command(DWORD menu_id)
{
	switch (menu_id)	{

		// システム：環境設定
		case IDM_SYSTEM_CONFIG:
			sub_menu_system.get_check(IDM_SYSTEM_CONFIG) ? Gp_dlg_mng->cfg_wnd_config_base.close() : Gp_dlg_mng->cfg_wnd_config_base.open();
			break;

		// システム：セーブダイアログ
		case IDM_SYSTEM_SAVE_MODAL_DIALOG:
			tnm_syscom_open_save(true, true, true);	// （★セーブ：ウィンドウメニュー）ダイアログが呼ばれる場合はキャプチャーする
			break;

		// システム：ロードダイアログ
		case IDM_SYSTEM_LOAD_MODAL_DIALOG:
			tnm_syscom_open_load(true, true);
			break;

		// システム：個別の環境設定
		case IDM_SYSTEM_CONFIG_SOLO_SCREEN:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_SCREEN) ? Gp_dlg_mng->cfg_wnd_solo_screen.close() : Gp_dlg_mng->cfg_wnd_solo_screen.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_FONT_MSGSPD:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_FONT_MSGSPD) ? Gp_dlg_mng->cfg_wnd_solo_font_msgspd.close() : Gp_dlg_mng->cfg_wnd_solo_font_msgspd.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_FONT:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_FONT) ? Gp_dlg_mng->cfg_wnd_solo_font.close() : Gp_dlg_mng->cfg_wnd_solo_font.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_MSGSPD:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_MSGSPD) ? Gp_dlg_mng->cfg_wnd_solo_msgspd.close() : Gp_dlg_mng->cfg_wnd_solo_msgspd.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_VOLUME_BGMFADE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_VOLUME_BGMFADE) ? Gp_dlg_mng->cfg_wnd_solo_volume_bgmfade.close() : Gp_dlg_mng->cfg_wnd_solo_volume_bgmfade.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_VOLUME:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_VOLUME) ? Gp_dlg_mng->cfg_wnd_solo_volume.close() : Gp_dlg_mng->cfg_wnd_solo_volume.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_BGMFADE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_BGMFADE) ? Gp_dlg_mng->cfg_wnd_solo_bgmfade.close() : Gp_dlg_mng->cfg_wnd_solo_bgmfade.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_KOEMODE_CHRKOE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_KOEMODE_CHRKOE) ? Gp_dlg_mng->cfg_wnd_solo_koemode_chrkoe.close() : Gp_dlg_mng->cfg_wnd_solo_koemode_chrkoe.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_KOEMODE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_KOEMODE) ? Gp_dlg_mng->cfg_wnd_solo_koemode.close() : Gp_dlg_mng->cfg_wnd_solo_koemode.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_CHRKOE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_CHRKOE) ? Gp_dlg_mng->cfg_wnd_solo_chrkoe.close() : Gp_dlg_mng->cfg_wnd_solo_chrkoe.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_MWNDBK:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_MWNDBK) ? Gp_dlg_mng->cfg_wnd_solo_mwndbk.close() : Gp_dlg_mng->cfg_wnd_solo_mwndbk.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_AUTOMODE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_AUTOMODE) ? Gp_dlg_mng->cfg_wnd_solo_automode.close() : Gp_dlg_mng->cfg_wnd_solo_automode.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_JITAN:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_JITAN) ? Gp_dlg_mng->cfg_wnd_solo_jitan.close() : Gp_dlg_mng->cfg_wnd_solo_jitan.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_ELSE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_ELSE) ? Gp_dlg_mng->cfg_wnd_solo_else.close() : Gp_dlg_mng->cfg_wnd_solo_else.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_SYSTEM:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_SYSTEM) ? Gp_dlg_mng->cfg_wnd_solo_system.close() : Gp_dlg_mng->cfg_wnd_solo_system.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_MOVTYPE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_MOVTYPE) ? Gp_dlg_mng->cfg_wnd_solo_movtype.close() : Gp_dlg_mng->cfg_wnd_solo_movtype.open();
			break;

		// システム：個別の環境設定：全て開く
		case IDM_SYSTEM_CONFIG_SOLO_ALL_OPEN:
			Gp_dlg_mng->cfg_wnd_solo_screen.open();				// ソロ：画面モード
			Gp_dlg_mng->cfg_wnd_solo_font_msgspd.open();		// ソロ：メッセージ（フォント、文字速度）
			Gp_dlg_mng->cfg_wnd_solo_font.open();				// ソロ：フォント
			Gp_dlg_mng->cfg_wnd_solo_msgspd.open();				// ソロ：文字速度
			Gp_dlg_mng->cfg_wnd_solo_volume_bgmfade.open();		// ソロ：音量（音量、ＢＧＭフェード）
			Gp_dlg_mng->cfg_wnd_solo_volume.open();				// ソロ：音量
			Gp_dlg_mng->cfg_wnd_solo_bgmfade.open();			// ソロ：ＢＧＭフェード
			Gp_dlg_mng->cfg_wnd_solo_koemode_chrkoe.open();		// ソロ：音声（音声モード、キャラクター音声）
			Gp_dlg_mng->cfg_wnd_solo_koemode.open();			// ソロ：音声モード
			Gp_dlg_mng->cfg_wnd_solo_chrkoe.open();				// ソロ：キャラクター音声
			Gp_dlg_mng->cfg_wnd_solo_mwndbk.open();				// ソロ：ウィンドウ背景色
			Gp_dlg_mng->cfg_wnd_solo_automode.open();			// ソロ：オートモード
			Gp_dlg_mng->cfg_wnd_solo_jitan.open();				// ソロ：時短再生
			Gp_dlg_mng->cfg_wnd_solo_else.open();				// ソロ：その他
			Gp_dlg_mng->cfg_wnd_solo_system.open();				// ソロ：システム
			Gp_dlg_mng->cfg_wnd_solo_movtype.open();			// ソロ：ムービーの再生方法
			break;

		// システム：個別の環境設定：全て閉じる
		case IDM_SYSTEM_CONFIG_SOLO_ALL_CLOSE:
			Gp_dlg_mng->cfg_wnd_solo_screen.close();			// ソロ：画面モード
			Gp_dlg_mng->cfg_wnd_solo_font_msgspd.close();		// ソロ：メッセージ（フォント、文字速度）
			Gp_dlg_mng->cfg_wnd_solo_font.close();				// ソロ：フォント
			Gp_dlg_mng->cfg_wnd_solo_msgspd.close();			// ソロ：文字速度
			Gp_dlg_mng->cfg_wnd_solo_volume_bgmfade.close();	// ソロ：音量（音量、ＢＧＭフェード）
			Gp_dlg_mng->cfg_wnd_solo_volume.close();			// ソロ：音量
			Gp_dlg_mng->cfg_wnd_solo_bgmfade.close();			// ソロ：ＢＧＭフェード
			Gp_dlg_mng->cfg_wnd_solo_koemode_chrkoe.close();	// ソロ：音声（音声モード、キャラクター音声）
			Gp_dlg_mng->cfg_wnd_solo_koemode.close();			// ソロ：音声モード
			Gp_dlg_mng->cfg_wnd_solo_chrkoe.close();			// ソロ：キャラクター音声
			Gp_dlg_mng->cfg_wnd_solo_mwndbk.close();			// ソロ：ウィンドウ背景色
			Gp_dlg_mng->cfg_wnd_solo_automode.close();			// ソロ：オートモード
			Gp_dlg_mng->cfg_wnd_solo_jitan.close();				// ソロ：時短再生
			Gp_dlg_mng->cfg_wnd_solo_else.close();				// ソロ：その他
			Gp_dlg_mng->cfg_wnd_solo_system.close();			// ソロ：システム
			Gp_dlg_mng->cfg_wnd_solo_movtype.close();			// ソロ：ムービーの再生方法
			break;

		// システム：個別の環境設定（モーダル版）
		case IDM_SYSTEM_CONFIG_MODAL_SCREEN:
			{
				C_cfg_wnd_modal_screen cfg_wnd_modal_screen;
				cfg_wnd_modal_screen.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_FONT_MSGSPD:
			{
				C_cfg_wnd_modal_font_msgspd cfg_wnd_modal_font_msgspd;
				cfg_wnd_modal_font_msgspd.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_FONT:
			{
				C_cfg_wnd_modal_font cfg_wnd_modal_font;
				cfg_wnd_modal_font.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_MSGSPD:
			{
				C_cfg_wnd_modal_msgspd cfg_wnd_modal_msgspd;
				cfg_wnd_modal_msgspd.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_VOLUME_BGMFADE:
			{
				Ccfg_wnd_modal_volume_bgmfade cfg_wnd_modal_volume_bgmfade;
				cfg_wnd_modal_volume_bgmfade.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_VOLUME:
			{
				Ccfg_wnd_modal_volume cfg_wnd_modal_volume;
				cfg_wnd_modal_volume.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_BGMFADE:
			{
				Ccfg_wnd_modal_bgmfade cfg_wnd_modal_bgmfade;
				cfg_wnd_modal_bgmfade.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_KOEMODE_CHRKOE:
			{
				Ccfg_wnd_modal_koemode_chrkoe cfg_wnd_modal_koemode_chrkoe;
				cfg_wnd_modal_koemode_chrkoe.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_KOEMODE:
			{
				Ccfg_wnd_modal_koemode cfg_wnd_modal_koemode;
				cfg_wnd_modal_koemode.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_CHRKOE:
			{
				Ccfg_wnd_modal_chrkoe cfg_wnd_modal_chrkoe;
				cfg_wnd_modal_chrkoe.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_MWNDBK:
			{
				Ccfg_wnd_modal_mwndbk cfg_wnd_modal_mwndbk;
				cfg_wnd_modal_mwndbk.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_AUTOMODE:
			{
				Ccfg_wnd_modal_automode cfg_wnd_modal_automode;
				cfg_wnd_modal_automode.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_JITAN:
			{
				Ccfg_wnd_modal_jitan cfg_wnd_modal_jitan;
				cfg_wnd_modal_jitan.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_ELSE:
			{
				Ccfg_wnd_modal_else cfg_wnd_modal_else;
				cfg_wnd_modal_else.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_SYSTEM:
			{
				Ccfg_wnd_modal_system cfg_wnd_modal_system;
				cfg_wnd_modal_system.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_MOVTYPE:
			{
				Ccfg_wnd_modal_movtype cfg_wnd_modal_movtype;				// サポート用のモーダル形式の「ムービーの再生方法」の設定を呼び出す
				bool res = cfg_wnd_modal_movtype.open_modal_result_bool();	// res : false = 再生しない
			}
			break;

		// システム：同期版ツイートダイアログ
		case IDM_SYSTEM_TWEET_SOLO_DIALOG:
			sub_menu_system.get_check(IDM_SYSTEM_TWEET_SOLO_DIALOG) ? Gp_dlg_mng->sys_wnd_solo_tweet.close() : Gp_dlg_mng->sys_wnd_solo_tweet.open();
			break;

		// システム：デバッグ用：全環境設定
		case IDM_SYSTEM_DEBUG_CONFIG_FULL_MODE:
			sub_menu_system.get_check(IDM_SYSTEM_DEBUG_CONFIG_FULL_MODE) ? Gp_dlg_mng->cfg_wnd_config_base_full_mode.close() : Gp_dlg_mng->cfg_wnd_config_base_full_mode.open();
			break;

		// システム：デバッグ用：同期版セーブダイアログ
		case IDM_SYSTEM_DEBUG_SAVE_SOLO_DIALOG:
			sub_menu_system.get_check(IDM_SYSTEM_DEBUG_SAVE_SOLO_DIALOG) ? Gp_dlg_mng->sys_wnd_solo_save.close() : Gp_dlg_mng->sys_wnd_solo_save.open();
			break;

		// システム：デバッグ用：同期版ロードダイアログ
		case IDM_SYSTEM_DEBUG_LOAD_SOLO_DIALOG:
			sub_menu_system.get_check(IDM_SYSTEM_DEBUG_LOAD_SOLO_DIALOG) ? Gp_dlg_mng->sys_wnd_solo_load.close() : Gp_dlg_mng->sys_wnd_solo_load.open();
			break;

		// デバッグ
		case IDM_DEBUG_RESTART:					// 最初から始める
			tnm_syscom_restart_from_start();
			break;
		case IDM_DEBUG_TRACE:					// トレース
			tnm_syscom_change_break();
			break;

		// ウィンドウ
		case IDM_WINDOW_DB_WND_INFO_INPUT:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_INPUT) ? Gp_dlg_mng->db_wnd_info_input.close() : Gp_dlg_mng->db_wnd_info_input.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_DEBUG:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_DEBUG) ? Gp_dlg_mng->db_wnd_info_debug.close() : Gp_dlg_mng->db_wnd_info_debug.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SCN_CONTROL:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_SCN_CONTROL) ? Gp_dlg_mng->db_wnd_info_scn_control.close() : Gp_dlg_mng->db_wnd_info_scn_control.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_CALL_HISTORY:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_CALL_HISTORY) ? Gp_dlg_mng->db_wnd_info_call_history.close() : Gp_dlg_mng->db_wnd_info_call_history.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SAVE_HISTORY:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_SAVE_HISTORY) ? Gp_dlg_mng->db_wnd_info_save_history.close() : Gp_dlg_mng->db_wnd_info_save_history.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_TRACE:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_TRACE) ? Gp_dlg_mng->db_wnd_info_trace.close() : Gp_dlg_mng->db_wnd_info_trace.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_ERROR:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_ERROR) ? Gp_dlg_mng->db_wnd_info_error.close() : Gp_dlg_mng->db_wnd_info_error.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SCRIPT:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_SCRIPT) ? Gp_dlg_mng->db_wnd_info_script.close() : Gp_dlg_mng->db_wnd_info_script.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_NAMAE:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_NAMAE) ? Gp_dlg_mng->db_wnd_info_namae.close() : Gp_dlg_mng->db_wnd_info_namae.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_CG_TABLE:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_CG_TABLE) ? Gp_dlg_mng->db_wnd_info_cg_table.close() : Gp_dlg_mng->db_wnd_info_cg_table.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_BGM_TABLE:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_BGM_TABLE) ? Gp_dlg_mng->db_wnd_info_bgm_table.close() : Gp_dlg_mng->db_wnd_info_bgm_table.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_READ_FLAG:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_READ_FLAG) ? Gp_dlg_mng->db_wnd_info_read_flag.close() : Gp_dlg_mng->db_wnd_info_read_flag.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_ELEMENT:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_ELEMENT) ? Gp_dlg_mng->db_wnd_info_element.close() : Gp_dlg_mng->db_wnd_info_element.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_USER_FLAG:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_USER_FLAG) ? Gp_dlg_mng->db_wnd_info_user_flag.close() : Gp_dlg_mng->db_wnd_info_user_flag.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_USER_CALL_FLAG:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_USER_CALL_FLAG) ? Gp_dlg_mng->db_wnd_info_user_call_flag.close() : Gp_dlg_mng->db_wnd_info_user_call_flag.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_NAMAE_FLAG:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_NAMAE_FLAG) ? Gp_dlg_mng->db_wnd_info_namae_flag.close() : Gp_dlg_mng->db_wnd_info_namae_flag.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_COUNTER:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_COUNTER) ? Gp_dlg_mng->db_wnd_info_counter.close() : Gp_dlg_mng->db_wnd_info_counter.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_BUTTON:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_BUTTON) ? Gp_dlg_mng->db_wnd_info_btn.close() : Gp_dlg_mng->db_wnd_info_btn.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_GROUP:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_GROUP) ? Gp_dlg_mng->db_wnd_info_group.close() : Gp_dlg_mng->db_wnd_info_group.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_WORLD:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_WORLD) ? Gp_dlg_mng->db_wnd_info_world.close() : Gp_dlg_mng->db_wnd_info_world.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_MWND:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_MWND) ? Gp_dlg_mng->db_wnd_info_mwnd.close() : Gp_dlg_mng->db_wnd_info_mwnd.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SOUND:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_SOUND) ? Gp_dlg_mng->db_wnd_info_sound.close() : Gp_dlg_mng->db_wnd_info_sound.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_VOLUME_CHANNEL:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_VOLUME_CHANNEL) ? Gp_dlg_mng->db_wnd_info_system_sound.close() : Gp_dlg_mng->db_wnd_info_system_sound.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_KOE:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_KOE) ? Gp_dlg_mng->db_wnd_info_koe.close() : Gp_dlg_mng->db_wnd_info_koe.open();
			break;

		// ウィンドウ：Direct3D 情報
		case IDM_WINDOW_DB_WND_INFO_D3D_DEVICE:
			sub_menu_window_d3d.get_check(IDM_WINDOW_DB_WND_INFO_D3D_DEVICE) ? Gp_dlg_mng->db_wnd_info_d3d_device.close() : Gp_dlg_mng->db_wnd_info_d3d_device.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_D3D_RESOURCE:
			sub_menu_window_d3d.get_check(IDM_WINDOW_DB_WND_INFO_D3D_RESOURCE) ? Gp_dlg_mng->db_wnd_info_d3d_resource.close() : Gp_dlg_mng->db_wnd_info_d3d_resource.open();
			break;

		// ウィンドウ：システム情報
		case IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM:
			sub_menu_window_system.get_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM) ? Gp_dlg_mng->db_wnd_info_system.close() : Gp_dlg_mng->db_wnd_info_system.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SYSTEM_DIR:
			sub_menu_window_system.get_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_DIR) ? Gp_dlg_mng->db_wnd_info_dir.close() : Gp_dlg_mng->db_wnd_info_dir.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SYSTEM_STACK:
			sub_menu_window_system.get_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_STACK) ? Gp_dlg_mng->db_wnd_info_stack.close() : Gp_dlg_mng->db_wnd_info_stack.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SYSTEM_PROC_STACK:
			sub_menu_window_system.get_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_PROC_STACK) ? Gp_dlg_mng->db_wnd_info_proc_stack.close() : Gp_dlg_mng->db_wnd_info_proc_stack.open();
			break;

		// ステータス

		default:
			// フラグ
			if (IDM_WINDOW_DB_WND_INFO_FLAG_TOP <= menu_id && menu_id < IDM_WINDOW_DB_WND_INFO_FLAG_TOP + TNM_FLAG_CNT)	{
				int dialog_id = menu_id - IDM_WINDOW_DB_WND_INFO_FLAG_TOP;
				sub_menu_window_flag.get_check(menu_id) ? Gp_dlg_mng->db_wnd_info_flag[dialog_id].close() : Gp_dlg_mng->db_wnd_info_flag[dialog_id].open();
			}
			// Flag.ini
			if (IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP <= menu_id && menu_id < IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP + 20)	{
				int dialog_id = menu_id - IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP;
				sub_menu_window_flag_ini.get_check(menu_id) ? Gp_dlg_mng->db_wnd_info_flag_ini[dialog_id].close() : Gp_dlg_mng->db_wnd_info_flag_ini[dialog_id].open();
			}
			// オブジェクト
			if (IDM_WINDOW_DB_WND_INFO_OBJECT_TOP <= menu_id && menu_id < IDM_WINDOW_DB_WND_INFO_OBJECT_TOP + 6)	{
				int dialog_id = menu_id - IDM_WINDOW_DB_WND_INFO_OBJECT_TOP;
				sub_menu_window_object.get_check(menu_id) ? Gp_dlg_mng->db_wnd_info_object[dialog_id].close() : Gp_dlg_mng->db_wnd_info_object[dialog_id].open();
			}
			// システムコマンドメニュー
			if (IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM <= menu_id && menu_id < IDM_SYSTEM_COMMAND_MENU + 1000)	{
				tnm_rmenu_execute(menu_id - IDM_SYSTEM_COMMAND_MENU);
			}
			break;
	}
}

// ****************************************************************
// メニュー更新
// ================================================================
void C_tnm_wnd_menu::update()
{
	// システム：システムコマンドメニュー
	sub_menu_system_syscommenu.remove_all_item();
	tnm_rmenu_create_funk(sub_menu_system_syscommenu, IDM_SYSTEM_COMMAND_MENU);

	// システム：環境設定
	sub_menu_system.set_check(IDM_SYSTEM_CONFIG, Gp_dlg_mng->cfg_wnd_config_base.get_handle() ? true : false);											// 環境設定

	// システム：セーブダイアログ

	// システム：ロードダイアログ

	// システム：個別の環境設定
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_SCREEN, Gp_dlg_mng->cfg_wnd_solo_screen.get_handle() ? true : false);					// ソロ：画面モード
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_FONT_MSGSPD, Gp_dlg_mng->cfg_wnd_solo_font_msgspd.get_handle() ? true : false);		// ソロ：メッセージ（フォント、文字速度）
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_FONT, Gp_dlg_mng->cfg_wnd_solo_font.get_handle() ? true : false);						// ソロ：フォント
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_MSGSPD, Gp_dlg_mng->cfg_wnd_solo_msgspd.get_handle() ? true : false);					// ソロ：文字速度
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_VOLUME_BGMFADE, Gp_dlg_mng->cfg_wnd_solo_volume_bgmfade.get_handle() ? true : false);	// ソロ：音量（音量、ＢＧＭフェード）
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_VOLUME, Gp_dlg_mng->cfg_wnd_solo_volume.get_handle() ? true : false);					// ソロ：音量
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_BGMFADE, Gp_dlg_mng->cfg_wnd_solo_bgmfade.get_handle() ? true : false);				// ソロ：ＢＧＭフェード
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_KOEMODE_CHRKOE, Gp_dlg_mng->cfg_wnd_solo_koemode_chrkoe.get_handle() ? true : false);	// ソロ：音声（音声モード、キャラクター音声）
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_KOEMODE, Gp_dlg_mng->cfg_wnd_solo_koemode.get_handle() ? true : false);				// ソロ：音声モード
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_CHRKOE, Gp_dlg_mng->cfg_wnd_solo_chrkoe.get_handle() ? true : false);					// ソロ：キャラクター音声
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_MWNDBK, Gp_dlg_mng->cfg_wnd_solo_mwndbk.get_handle() ? true : false);					// ソロ：ウィンドウ背景色
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_AUTOMODE, Gp_dlg_mng->cfg_wnd_solo_automode.get_handle() ? true : false);				// ソロ：オートモード
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_JITAN, Gp_dlg_mng->cfg_wnd_solo_jitan.get_handle() ? true : false);					// ソロ：時短再生
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_ELSE, Gp_dlg_mng->cfg_wnd_solo_else.get_handle() ? true : false);						// ソロ：その他
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_SYSTEM, Gp_dlg_mng->cfg_wnd_solo_system.get_handle() ? true : false);					// ソロ：システム
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_MOVTYPE, Gp_dlg_mng->cfg_wnd_solo_movtype.get_handle() ? true : false);				// ソロ：ムービーの再生方法

	// システム：個別の環境設定（モーダル版）

	// システム：ツイートダイアログ
	sub_menu_system.set_check(IDM_SYSTEM_TWEET_SOLO_DIALOG, Gp_dlg_mng->sys_wnd_solo_tweet.get_handle() ? true : false);								// ツイートダイアログ

	// システム：デバッグ用：全システムコマンドメニュー
	sub_menu_system_debug_syscommenu_full_mode.remove_all_item();
	tnm_rmenu_create_funk(sub_menu_system_debug_syscommenu_full_mode, IDM_SYSTEM_COMMAND_MENU);

	// システム：デバッグ用：全環境設定
	sub_menu_system_debug.set_check(IDM_SYSTEM_DEBUG_CONFIG_FULL_MODE, Gp_dlg_mng->cfg_wnd_config_base_full_mode.get_handle() ? true : false);			// 全環境設定

	// システム：デバッグ用：同期版セーブダイアログ
	sub_menu_system_debug.set_check(IDM_SYSTEM_DEBUG_SAVE_SOLO_DIALOG, Gp_dlg_mng->sys_wnd_solo_save.get_handle() ? true : false);						// 同期版セーブダイアログ

	// システム：デバッグ用：同期版ロードダイアログ
	sub_menu_system_debug.set_check(IDM_SYSTEM_DEBUG_LOAD_SOLO_DIALOG, Gp_dlg_mng->sys_wnd_solo_load.get_handle() ? true : false);						// 同期版ロードダイアログ

	// ウィンドウ
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_INPUT, Gp_dlg_mng->db_wnd_info_input.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_DEBUG, Gp_dlg_mng->db_wnd_info_debug.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_SCN_CONTROL, Gp_dlg_mng->db_wnd_info_scn_control.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_SAVE_HISTORY, Gp_dlg_mng->db_wnd_info_save_history.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_TRACE, Gp_dlg_mng->db_wnd_info_trace.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_ERROR, Gp_dlg_mng->db_wnd_info_error.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_NAMAE, Gp_dlg_mng->db_wnd_info_namae.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_CG_TABLE, Gp_dlg_mng->db_wnd_info_cg_table.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_BGM_TABLE, Gp_dlg_mng->db_wnd_info_bgm_table.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_READ_FLAG, Gp_dlg_mng->db_wnd_info_read_flag.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_ELEMENT, Gp_dlg_mng->db_wnd_info_element.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_USER_FLAG, Gp_dlg_mng->db_wnd_info_user_flag.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_USER_CALL_FLAG, Gp_dlg_mng->db_wnd_info_user_call_flag.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_NAMAE_FLAG, Gp_dlg_mng->db_wnd_info_namae_flag.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_COUNTER, Gp_dlg_mng->db_wnd_info_counter.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_BUTTON, Gp_dlg_mng->db_wnd_info_btn.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_GROUP, Gp_dlg_mng->db_wnd_info_group.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_WORLD, Gp_dlg_mng->db_wnd_info_world.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_MWND, Gp_dlg_mng->db_wnd_info_mwnd.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_SOUND, Gp_dlg_mng->db_wnd_info_sound.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_VOLUME_CHANNEL, Gp_dlg_mng->db_wnd_info_system_sound.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_KOE, Gp_dlg_mng->db_wnd_info_koe.get_handle() ? true : false);

	// ウィンドウ：Direct3D 情報
	sub_menu_window_d3d.set_check(IDM_WINDOW_DB_WND_INFO_D3D_DEVICE, Gp_dlg_mng->db_wnd_info_d3d_device.get_handle() ? true : false);
	sub_menu_window_d3d.set_check(IDM_WINDOW_DB_WND_INFO_D3D_RESOURCE, Gp_dlg_mng->db_wnd_info_d3d_resource.get_handle() ? true : false);

	// ウィンドウ：システム情報
	sub_menu_window_system.set_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM, Gp_dlg_mng->db_wnd_info_system.get_handle() ? true : false);
	sub_menu_window_system.set_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_DIR, Gp_dlg_mng->db_wnd_info_dir.get_handle() ? true : false);
	sub_menu_window_system.set_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_STACK, Gp_dlg_mng->db_wnd_info_stack.get_handle() ? true : false);
	sub_menu_window_system.set_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_PROC_STACK, Gp_dlg_mng->db_wnd_info_proc_stack.get_handle() ? true : false);

	// ウィンドウ：フラグ
	for (int i = 0; i < TNM_FLAG_CNT; i++)	{
		sub_menu_window_flag.set_check(IDM_WINDOW_DB_WND_INFO_FLAG_TOP + i, Gp_dlg_mng->db_wnd_info_flag[i].get_handle() ? true : false);
	}
	// ウィンドウ：Flag.ini
	for (int i = 0; i < 20; i++)	{
		sub_menu_window_flag_ini.set_check(IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP + i, Gp_dlg_mng->db_wnd_info_flag_ini[i].get_handle() ? true : false);
	}
	// ウィンドウ：オブジェクト
	for (int i = 0; i < 6; i++)	{
		sub_menu_window_object.set_check(IDM_WINDOW_DB_WND_INFO_OBJECT_TOP + i, Gp_dlg_mng->db_wnd_info_object[i].get_handle() ? true : false);
	}
}

