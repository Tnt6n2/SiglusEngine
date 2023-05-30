#pragma		once

// ****************************************************************
// エラータイプ
// ================================================================
enum TNM_ERROR_TYPE
{
	TNM_ERROR_TYPE_NONE,	// エラーなし

	TNM_ERROR_TYPE_INFO,	// 必ずエラーボックスを出すべきもの
	TNM_ERROR_TYPE_FATAL,	// 必ずエラーボックスを出すべきもの
	TNM_ERROR_TYPE_DEBUG,	// デバッグ版のみエラーボックスを出すべきもの
	TNM_ERROR_TYPE_WARNING,	// デバッグダイアログに表示するだけのもの
	TNM_ERROR_TYPE_SUCCESS,	// デバッグダイアログに表示するだけのもの

	TNM_ERROR_TYPE_DIVIDED_BY_ZERO,		// ０除算
	TNM_ERROR_TYPE_OUT_OF_RANGE,		// 範囲外アクセス
	TNM_ERROR_TYPE_OVERFLOW,			// オーバーフロー
	TNM_ERROR_TYPE_FILE_NOT_FOUND,		// ファイルエラー
	TNM_ERROR_TYPE_PCT_NOT_FOUND,		// 画像ファイルエラー
	TNM_ERROR_TYPE_SOUND_NOT_FOUND,		// 音ファイルエラー
	TNM_ERROR_TYPE_KOE_NOT_FOUND,		// 声ファイルエラー
	TNM_ERROR_TYPE_PAT_NO,				// パターン番号エラー
};

// ****************************************************************
// 汎用
// ================================================================
const int	TNM_USE_DEFAULT = -2;
const int	TNM_EMPTY = -1;

// ****************************************************************
// 単位
// ================================================================
const int	TNM_SCALE_UNIT = 1000;
const int	TNM_ANGLE_UNIT = 10;
const int	TNM_WORLD_UNIT = 1;

// ****************************************************************
// フラグ数（ウィンドウメニューやデバッグウィンドウの個数に関わる）
// ================================================================
const int	TNM_FLAG_CNT = 13;		// A,B,C,D,E,F,G,Z,X,S,M,L,K

// ****************************************************************
// ワールド
// ================================================================
const int	TNM_WORLD_NONE = -1;

// ****************************************************************
// ステージ
// ================================================================
const int	TNM_STAGE_BACK = 0;
const int	TNM_STAGE_FRONT = 1;
const int	TNM_STAGE_NEXT = 2;
const int	TNM_STAGE_CNT = 3;

// ****************************************************************
// トーンカーブ
// ================================================================
const int	TNM_TONE_CURVE_NONE = -1;

// ****************************************************************
// マスク
// ================================================================
const int	TNM_MASK_NONE = -1;

// ****************************************************************
// ライト
// ================================================================
const int	TNM_LIGHT_NONE = -1;

// ****************************************************************
// 合成方法
// ================================================================
const int	TNM_BLEND_TYPE_NORMAL = 0;
const int	TNM_BLEND_TYPE_ADD    = 1;
const int	TNM_BLEND_TYPE_SUB    = 2;
const int	TNM_BLEND_TYPE_MUL    = 3;
const int	TNM_BLEND_TYPE_SCREEN = 4;
const int	TNM_BLEND_TYPE_MAX    = 5;

// ****************************************************************
// テクスチャステージ
// ================================================================
const int	TNM_TEXTURE_STAGE_PCT			= 0;
const int	TNM_TEXTURE_STAGE_MASK			= 1;
const int	TNM_TEXTURE_STAGE_TONE_CURVE	= 2;
const int	TNM_TEXTURE_STAGE_FOG			= 3;

// ****************************************************************
// ボタンの状態
// ================================================================
const int	TNM_BTN_STATE_NORMAL   = 0;
const int	TNM_BTN_STATE_HIT      = 1;
const int	TNM_BTN_STATE_PUSH     = 2;
const int	TNM_BTN_STATE_SELECT   = 3;
const int	TNM_BTN_STATE_DISABLE  = 4;
const int	TNM_BTN_STATE_MAX      = 5;

// ****************************************************************
// セーブサムネイルのタイプ
// ================================================================
const int	TNM_SAVE_THUMB_TYPE_BMP = 0;
const int	TNM_SAVE_THUMB_TYPE_PNG = 1;

// ****************************************************************
// キャプチャーのタイプ
// ================================================================
const int	TNM_CAPTURE_TYPE_NONE = 0;
const int	TNM_CAPTURE_TYPE_SAVE_THUMB = 1;
const int	TNM_CAPTURE_TYPE_OBJECT = 2;
const int	TNM_CAPTURE_TYPE_THUMB = 3;
const int	TNM_CAPTURE_TYPE_BUFFER = 4;
const int	TNM_CAPTURE_TYPE_TWEET = 5;

// ****************************************************************
// キャプチャー優先度
// ================================================================
const int	TNM_CAPTURE_PRIOR_NONE		= 0;	// キャプチャーなし
const int	TNM_CAPTURE_PRIOR_SAVE		= 1;	// セーブダイアログなど
const int	TNM_CAPTURE_PRIOR_END		= 2;	// ゲーム終了
const int	TNM_CAPTURE_PRIOR_CAPTURE	= 3;	// キャプチャーコマンド

// ****************************************************************
// ウィンドウ枠
// ================================================================
//const int	TNM_MWND_WAKU_NONE = -1;			// 枠なし

// ****************************************************************
// 名前表示タイプ
// ================================================================
const int	TNM_MWND_NAME_DISP_TYPE_NAME = 0;
const int	TNM_MWND_NAME_DISP_TYPE_MSG = 1;
const int	TNM_MWND_NAME_DISP_TYPE_NONE = 2;

// ****************************************************************
// 名前括弧タイプ
// ================================================================
const int	TNM_MWND_NAME_BRACKET_TYPE_NONE = 0;
const int	TNM_MWND_NAME_BRACKET_TYPE_SUMI = 1;

// ****************************************************************
// 名前表示タイプ（メッセージバック）
// ================================================================
const int	TNM_MSGBK_NAME_DISP_TYPE_DEFAULT = -1;
const int	TNM_MSGBK_NAME_DISP_TYPE_OFF = 0;
const int	TNM_MSGBK_NAME_DISP_TYPE_ON = 1;

// ****************************************************************
// 名前括弧タイプ
// ================================================================
const int	TNM_MSGBK_NAME_BRACKET_TYPE_NONE = 0;
const int	TNM_MSGBK_NAME_BRACKET_TYPE_SUMI = 1;

// ****************************************************************
// 選択肢アイテムの状態
// ================================================================
const int	TNM_SEL_ITEM_TYPE_OFF = 0;
const int	TNM_SEL_ITEM_TYPE_ON = 1;
const int	TNM_SEL_ITEM_TYPE_READ = 2;
const int	TNM_SEL_ITEM_TYPE_HIDE = 3;

// ****************************************************************
// BGM
// ================================================================
const int	TNM_BGM_START_POS_INI = -1;	// BGM の開始位置：ini に従う

// ****************************************************************
// SE タイプ
// ================================================================
const int	TNM_SE_TYPE_SELECT = 0;		// 選択音
const int	TNM_SE_TYPE_DECIDE = 1;		// 決定音
const int	TNM_SE_TYPE_CANCEL = 2;		// キャンセル音
const int	TNM_SE_TYPE_WARNING = 3;	// 警告音
const int	TNM_SE_TYPE_SAVE = 4;		// セーブ音
const int	TNM_SE_TYPE_LOAD = 5;		// ロード音
const int	TNM_SE_TYPE_MENU = 6;		// タイトルに戻る音
const int	TNM_SE_TYPE_PREV_SEL = 7;	// 前の選択肢に戻る音

// ****************************************************************
// ボリュームタイプ
// ================================================================
const int	TNM_VOLUME_TYPE_NONE = -1;
const int	TNM_VOLUME_TYPE_BGM = 0;
const int	TNM_VOLUME_TYPE_KOE = 1;
const int	TNM_VOLUME_TYPE_PCM = 2;
const int	TNM_VOLUME_TYPE_SE = 3;
const int	TNM_VOLUME_TYPE_MOV = 4;
//const int	TNM_VOLUME_TYPE_ALL = 5;		// 全体は5番になりました。中途半端です。
const int	TNM_VOLUME_TYPE_EXSOUND = 16;	// 追加ボリューム
const int	TNM_VOLUME_TYPE_MAX = 32;

// ****************************************************************
// システム：ムービー
// ================================================================
const int	TNM_MOVIE_FILE_TYPE_NONE = 0;
const int	TNM_MOVIE_FILE_TYPE_AVI = 1;
const int	TNM_MOVIE_FILE_TYPE_MPG = 2;
const int	TNM_MOVIE_FILE_TYPE_WMV = 3;

// ****************************************************************
// システム：セーブヘッダ
// ================================================================
const int	TNM_SAVE_APPEND_DIR_MAX_LEN = 256;
const int	TNM_SAVE_APPEND_NAME_MAX_LEN = 256;
const int	TNM_SAVE_TITLE_MAX_LEN = 256;
const int	TNM_SAVE_MESSAGE_MAX_LEN = 256;
const int	TNM_SAVE_FULL_MESSAGE_MAX_LEN = 256;
const int	TNM_SAVE_COMMENT_MAX_LEN = 256;
const int	TNM_SAVE_COMMENT2_MAX_LEN = 256;
const int	TNM_SAVE_FLAG_MAX_CNT = 256;

// ****************************************************************
// システム：セーブ履歴数
// ================================================================
//const int	TNM_SAVE_HISTORY_MAX_CNT = 100;

// ****************************************************************
// システム：モード、タイプなどの定義
// ================================================================

// 画面モード
const int	TNM_SCREEN_SIZE_MODE_WINDOW = 0;		// 50%, 75%, 100%, ...
const int	TNM_SCREEN_SIZE_MODE_FULL = 1;			// フルスクリーン
const int	TNM_SCREEN_SIZE_MODE_FREE = 2;			// 自由サイズ

// フルスクリーンの表示モード
const int	TNM_FULLSCREEN_MODE_NORMAL = 0;			// 原寸
const int	TNM_FULLSCREEN_MODE_FIT = 1;			// 全画面
const int	TNM_FULLSCREEN_MODE_UDLRFIT = 2;		// 全画面フィット
const int	TNM_FULLSCREEN_MODE_UDFIT = 3;			// 上下フィット
const int	TNM_FULLSCREEN_MODE_LRFIT = 4;			// 左右フィット
const int	TNM_FULLSCREEN_MODE_CUSTOM = 5;			// カスタム

// フォントの種類
const int	TNM_FONT_TYPE_MS_GOTHIC = 0;			// ＭＳゴシック
const int	TNM_FONT_TYPE_MS_MINTYOU = 1;			// ＭＳ明朝
const int	TNM_FONT_TYPE_MEIRYO = 2;				// メイリオ
const int	TNM_FONT_TYPE_ORIGINAL = 3;				// 独自フォント

// フォントの名前
const TSTR	TNM_FONT_NAME_MS_GOTHIC = _T("ＭＳ ゴシック");		// ＭＳゴシック
const TSTR	TNM_FONT_NAME_MS_MINTYOU = _T("ＭＳ 明朝");			// ＭＳ明朝
const TSTR	TNM_FONT_NAME_MS_MEIRYO = _T("メイリオ");			// メイリオ

// フォントの影モード
const int	TNM_FONT_SHADOW_MODE_NONE = 0;				// 影なし
const int	TNM_FONT_SHADOW_MODE_SHADOW = 1;			// 影あり
const int	TNM_FONT_SHADOW_MODE_FUTIDORU = 2;			// 縁取る
const int	TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW = 3;	// 縁取って影あり

// 時短再生
const int	TNM_JITAN_RATE_NORMAL = 100;			// 標準速

// 音声モード
const int	TNM_KOE_MODE_NORMAL = 0;				// 音声あり（文章あり）
const int	TNM_KOE_MODE_TEXTONLY = 1;				// 音声なし（文章のみ）
const int	TNM_KOE_MODE_KOEONLY = 2;				// 音声あり（文章なし）

// キャラクター音声のチェックモード
const int	TNM_CHRKOE_CHECK_MODE_NONE = 0;			// 伏字状態でチェックしない
const int	TNM_CHRKOE_CHECK_MODE_LOOK = 1;			// 公開状態でチェックしない
const int	TNM_CHRKOE_CHECK_MODE_AUTO = 2;			// 伏字状態でチェックする

// ムービーの再生方法
const int	TNM_MOVIE_PLAY_TYPE_MCI = 0;			// MCI
const int	TNM_MOVIE_PLAY_TYPE_WMP = 1;			// Windows Media Player

// ****************************************************************
// システム：バッファの個数
// ================================================================

// ローカル汎用スイッチの数
const int	TNM_LOCAL_EXTRA_SWITCH_CNT = 4;			// ローカル汎用スイッチの数

// ローカル汎用モードの数
const int	TNM_LOCAL_EXTRA_MODE_CNT = 4;			// ローカル汎用モードの数

// ローカル汎用モードのアイテム数（項目数）
const int	TNM_LOCAL_EXTRA_MODE_ITEM_CNT = 8;		// ローカル汎用モードのアイテム数（項目数）

// オブジェクト表示の数
const int	TNM_OBJECT_DISP_CNT = 4;				// オブジェクト表示の数

// グローバル汎用スイッチの数
const int	TNM_GLOBAL_EXTRA_SWITCH_CNT = 4;		// グローバル汎用スイッチの数

// グローバル汎用モードの数
const int	TNM_GLOBAL_EXTRA_MODE_CNT = 4;			// グローバル汎用モードの数

// グローバル汎用モードのアイテム数（項目数）
const int	TNM_GLOBAL_EXTRA_MODE_ITEM_CNT = 8;		// グローバル汎用モードのアイテム数（項目数）

// ****************************************************************
// システム：値の最小値、最大値などの定義
// ================================================================

// 時短再生
const int	TNM_JITAN_SPEED_MIN = 100;
const int	TNM_JITAN_SPEED_MAX = 300;

// 文字速度
const int	TNM_MESSAGE_SPEED_MIN = 0;
const int	TNM_MESSAGE_SPEED_MAX = 100;

// オートモード
const int	TNM_AUTO_MODE_MOJI_WAIT_MIN = 0;	// オートモード文字時間
const int	TNM_AUTO_MODE_MOJI_WAIT_MAX = 500;	// オートモード文字時間
const int	TNM_AUTO_MODE_MIN_WAIT_MIN = 0;		// オートモード最小時間
const int	TNM_AUTO_MODE_MIN_WAIT_MAX = 5000;	// オートモード最小時間

// 声のみ
const int	TNM_KOE_ONLY_WAIT = 500;			// 声のみモードで声の再生が終わってからの待ち時間

// ****************************************************************
// システムコマンドタイプ
// ================================================================
const int	TNM_SYSCOM_TYPE_NONE = 0;
const int	TNM_SYSCOM_TYPE_SAVE = 1;
const int	TNM_SYSCOM_TYPE_LOAD = 2;
const int	TNM_SYSCOM_TYPE_READ_SKIP = 3;
const int	TNM_SYSCOM_TYPE_AUTO_MODE = 4;
const int	TNM_SYSCOM_TYPE_RETURN_SEL = 5;
const int	TNM_SYSCOM_TYPE_HIDE_MWND = 6;
const int	TNM_SYSCOM_TYPE_MSG_BACK = 7;
const int	TNM_SYSCOM_TYPE_KOE_PLAY = 8;
const int	TNM_SYSCOM_TYPE_QUICK_SAVE = 9;
const int	TNM_SYSCOM_TYPE_QUICK_LOAD = 10;
const int	TNM_SYSCOM_TYPE_CONFIG = 11;
const int	TNM_SYSCOM_TYPE_LOCAL_EXTRA_SWITCH = 12;
const int	TNM_SYSCOM_TYPE_LOCAL_EXTRA_MODE = 13;
const int	TNM_SYSCOM_TYPE_GLOBAL_EXTRA_SWITCH = 14;
const int	TNM_SYSCOM_TYPE_GLOBAL_EXTRA_MODE = 15;

// ****************************************************************
// システム：セットアップタイプ
// ================================================================
const int	TNM_SETUP_TYPE_NONE = -1;	// セットアップされていない
const int	TNM_SETUP_TYPE_FULL = 0;	// 完全セットアップ
const int	TNM_SETUP_TYPE_MIN = 1;		// 最小セットアップ

// ****************************************************************
// プロセスタイプ
// ================================================================
enum TNM_PROC_TYPE
{
#define		PROC(type, id, key_wait)		TNM_PROC_TYPE_##type = id,
#include	"tnm_proc_def_00.h"
#undef		PROC
};

// ****************************************************************
// プロセス名
// ================================================================
const TCHAR* const TNM_PROC_STR[] = 
{
#define		PROC(type, id, key_wait)		_T(#type),
#include	"tnm_proc_def_00.h"
#undef		PROC
};

// ****************************************************************
// プロセスステート
// ================================================================
struct S_tnm_proc_state
{
	bool				key_wait;
};

const S_tnm_proc_state TNM_PROC_STATE[] = 
{
#define		PROC(type, id, key_wait)		{key_wait},
#include	"tnm_proc_def_00.h"
#undef		PROC
};

