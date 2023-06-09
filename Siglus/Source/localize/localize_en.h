#pragma		once

// ****************************************************************
// ローカライズ：言語コード
// ================================================================

#define		LCL_LANGUAGE_CODE	_T("en")

// ****************************************************************
// ローカライズ：Gameexe.ini
// ================================================================

#define		LCL_GAMEEXE_DAT		_T("GameexeEN.dat")

// ****************************************************************
// ローカライズ：Scene.pck
// ================================================================

#define		LCL_SCENE_PCK		_T("SceneEN.pck")

// ****************************************************************
// ローカライズ：セーブフォルダ
// ================================================================

#define		LCL_SAVEDATA_DIR	_T("savedata_en")

// ****************************************************************
// ローカライズ：ユーザーフォルダ
// ================================================================

#define		LCL_USERDATA_DIR	_T("userdata")

// ****************************************************************
// ローカライズ：日付表記
// ================================================================

extern TCHAR* MONTH_STR_EN[];

#define		LCL_DATE_FORMAT		str_format(_T("%d %s %d at %02d:%02d:%02d"), p_header->day, MONTH_STR_EN[p_header->month - 1], p_header->year, p_header->hour, p_header->minute, p_header->second);

// ****************************************************************
// ローカライズ：文字列：システムコマンドメニュー
// ================================================================

// システムコマンドメニュー：文字列
const TSTR	LCL_STR_SYSCOMMENU_READ_SKIP_STR = _T("Skip through already read sentences");		// ★：一度読んだ文章の早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_UNREAD_SKIP_STR = _T("Fast-forward all sentences");				// ★：未読の文章でも早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_AKIP_STR = _T("Automatically fast-forward");				// ★：自動早送り機能をＯＮ／ＯＦＦします。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_MODE_STR = _T("Automatically reading");						// ★：オートモードを開始します。
const TSTR	LCL_STR_SYSCOMMENU_HIDE_MWND_STR = _T("Hide messages");								// ★：メッセージウィンドウを隠します。
const TSTR	LCL_STR_SYSCOMMENU_MSG_BACK_STR = _T("Show message log");							// ★：メッセージバックを開きます。
const TSTR	LCL_STR_SYSCOMMENU_SAVE_STR = _T("Save");											// ★：セーブを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_LOAD_STR = _T("Load");											// ★：ロードを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_SEL_STR = _T("Back to the latest choice");				// ★：前の選択肢に戻ります。
const TSTR	LCL_STR_SYSCOMMENU_CONFIG_STR = _T("Configuration");								// ★：環境設定を呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_MANUAL_STR = _T("Open the Instruction");							// ★：マニュアルを表示します。
const TSTR	LCL_STR_SYSCOMMENU_VERSION_STR = _T("Version Information");							// ★：バージョン情報を表示します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_MENU_STR = _T("Return to the title screen");				// ★：タイトルメニューに戻ります。
const TSTR	LCL_STR_SYSCOMMENU_GAME_END_STR = _T("Exit the game");								// ★：ゲームを終了します。
const TSTR	LCL_STR_SYSCOMMENU_CANCEL_STR = _T("Cancel");										// ★：システムコマンドメニューをキャンセルします。
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_SWITCH_STR(i)	(str_format(_T("Local general-purpose switch No.%d"), i))	// ★：ローカル汎用スイッチ：スイッチ名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_STR(i)		(str_format(_T("Local general-purpose mode No.%d"), i))		// ★：ローカル汎用モード：モード名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_STR(i)	(str_format(_T("Mode %d"), i))								// ★：ローカル汎用モード：アイテム名

// ****************************************************************
// ローカライズ：文字列：ダイアログ
// ================================================================

// ダイアログ共通
const TSTR	LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR = _T("Decide");
const TSTR	LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR = _T("Close");
const TSTR	LCL_STR_MODAL_DIALOG_INIT_BTN_STR = _T("Reset");

// セーブダイアログ
const TSTR	LCL_STR_SAVE_DIALOG_CAPTION_TITLE_STR = _T("Save");													// ★：セーブダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DECIDE_BUTTON_STR = _T("Save");													// ★：セーブダイアログの決定ボタンの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DATALIST_NAMEHEADER_STR = _T("Save");											// ★：セーブダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_SAVE_DIALOG_WARNING_CHECKBOX_STR = _T("Display confirmation window before overwriting save data.");	// ★：セーブダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DBLCLK_CHECKBOX_STR = _T("By double click, allows saving data to a save list.");		// ★：セーブダイアログのダブルクリックチェックボックスの文字列

// ロードダイアログ
const TSTR	LCL_STR_LOAD_DIALOG_CAPTION_TITLE_STR = _T("Load");												// ★：ロードダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DECIDE_BUTTON_STR = _T("Load");												// ★：ロードダイアログの決定ボタンの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DATALIST_NAMEHEADER_STR = _T("Load");										// ★：ロードダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_LOAD_DIALOG_WARNING_CHECKBOX_STR = _T("Display confirmation window before loading data.");		// ★：ロードダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DBLCLK_CHECKBOX_STR = _T("By double click, allows loading data to a load menu.");	// ★：ロードダイアログのダブルクリックチェックボックスの文字列

// セーブダイアログ＆ロードダイアログの共通情報
const TSTR	LCL_STR_SAVELOAD_DIALOG_NO_DATA_STR = _T("No data");

// ユーザコンフィグ：環境設定ダイアログ
const TSTR	LCL_STR_CONFIG_DIALOG_SCREEN_STR = _T("Display");
const TSTR	LCL_STR_CONFIG_DIALOG_VOLUME_STR = _T("Sound");
const TSTR	LCL_STR_CONFIG_DIALOG_MESSAGE_STR = _T("Chars");
const TSTR	LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR = _T("Message BG");
const TSTR	LCL_STR_CONFIG_DIALOG_VOICE_STR = _T("Voice");
const TSTR	LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR = _T("Automatic mode");
const TSTR	LCL_STR_CONFIG_DIALOG_JITAN_STR = _T("Reproduction speed");
const TSTR	LCL_STR_CONFIG_DIALOG_OTHERS_STR = _T("Option");
const TSTR	LCL_STR_CONFIG_DIALOG_SYSTEM_STR = _T("Operation");

// ユーザコンフィグ：画面ダイアログ
const TSTR	LCL_STR_SCREEN_DIALOG_CURRENT_RESOLUTION_STR = _T("(Current resolution)");
const TSTR	LCL_STR_SCREEN_DIALOG_DEFAULT_RESOLUTION_STR = _T("(Previous resolution)");

// ユーザコンフィグ：音量・ＢＧＭフェードダイアログ
const TSTR	LCL_STR_VOLUME_BGMFADE_DIALOG_DIALOG_TITLE = _T("Volume Settings");

// ユーザコンフィグ：音量ダイアログ
const TSTR	LCL_STR_VOLUME_DIALOG_DIALOG_TITLE = _T("Volume Settings");
const TSTR	LCL_STR_VOLUME_DIALOG_GROUP_TITLE = _T("Volume");
const TSTR	LCL_STR_VOLUME_DIALOG_ALL_GROUP_TITLE = _T("Master");
const TSTR	LCL_STR_VOLUME_DIALOG_BGM_GROUP_TITLE = _T("BGM");
const TSTR	LCL_STR_VOLUME_DIALOG_KOE_GROUP_TITLE = _T("Voice");
const TSTR	LCL_STR_VOLUME_DIALOG_PCM_GROUP_TITLE = _T("Effects");
const TSTR	LCL_STR_VOLUME_DIALOG_SE_GROUP_TITLE = _T("Operation");
const TSTR	LCL_STR_VOLUME_DIALOG_MOV_GROUP_TITLE = _T("Movie");
const TSTR	LCL_STR_VOLUME_DIALOG_STC_PLAY = _T("Play");

// ユーザコンフィグ：ＢＧＭフェードダイアログ
const TSTR	LCL_STR_BGMFADE_DIALOG_DIALOG_TITLE = _T("BGM fade effect Settings");
const TSTR	LCL_STR_BGMFADE_DIALOG_GROUP_TITLE = _T("BGM fade effect");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_EXPLAINE = _T("This feature reduce BGM volume, when play voices.");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_ENABLE = _T("Enable");

// ユーザコンフィグ：フォント・メッセージスピードダイアログ
const TSTR	LCL_STR_FONT_MSGSPD_DIALOG_DIALOG_TITLE = _T("Text Settings");

// ユーザコンフィグ：フォントダイアログ
const TSTR	LCL_STR_FONT_DIALOG_DIALOG_TITLE = _T("Font Settings");
const TSTR	LCL_STR_FONT_DIALOG_GROUP_TITLE = _T("Fonts");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_ONOFF = _T("Include proportional font.");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_DESC = _T("* If you will use the proportional font, the layout of sentences might break.");
const TSTR	LCL_STR_FONT_DIALOG_STC_BOLD = _T("Bold text");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_OFF = _T("Deactivate shadows");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_ON = _T("Activate shadows");
const TSTR	LCL_STR_FONT_DIALOG_STC_BORDER_ON = _T("With borders");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON = _T("With borders && shadows");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_SHORT = _T("'ABC GIJ, abc gij.'");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_LONG = _T("'ABC GIJ, abc gij, 0123.'");

// ユーザコンフィグ：メッセージスピードダイアログ
const TSTR	LCL_STR_MSGSPD_DIALOG_DIALOG_TITLE = _T("Character Rate Settings");
const TSTR	LCL_STR_MSGSPD_DIALOG_GROUP_TITLE = _T("Character Rate");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_SLOW = _T("Slow");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_FAST = _T("Fast");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_NOWAIT = _T("Instant");

// ユーザーコンフィグ：音声ダイアログ
const TSTR	LCL_STR_KOEMOD_CHRKOE_DIALOG_DIALOG_TITLE = _T("音声の設定");

// ユーザーコンフィグ：音声モードダイアログ
const TSTR	LCL_STR_KOEMOD_DIALOG_DIALOG_TITLE = _T("音声モードの設定");
const TSTR	LCL_STR_KOEMOD_DIALOG_GROUP_TITLE = _T("音声モード");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ON = _T("音声あり");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_OFF = _T("音声なし");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ONLY = _T("音声のみ（文章なし）");

// ユーザーコンフィグ：キャラクター音声ダイアログ
const TSTR	LCL_STR_CHRKOE_DIALOG_DIALOG_TITLE = _T("キャラクター音声の設定");
const TSTR	LCL_STR_CHRKOE_DIALOG_GROUP_TITLE = _T("キャラクター音声");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_ON = _T("全てＯＮ");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_OFF = _T("全てＯＦＦ");

// ユーザーコンフィグ：オートモードダイアログ
const TSTR	LCL_STR_DIALOG_AUTOMODE_DIALOG_TITLE = _T("Automatically reading Settings");
const TSTR	LCL_STR_DIALOG_AUTOMODE_GROUP_TITLE = _T("Automatically reading");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_EXPLAINE = _T("Delay = (Displayed Characters × Seconds per Characters) + Minimum Interval");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MOJI_TIME = _T("Seconds per Chars");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MIN_TIME = _T("Minimum Interval");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_TEN_SECONDS = _T("Time of displaying 10 Characters");
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_1(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%d"), n2) + _T(" Sec")
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_2(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%02d"), n2) + _T(" Sec")

// ユーザコンフィグ：システムダイアログ
const TSTR	LCL_STR_SYSTEM_DIALOG_SLEEP_STR = _T("Make slowly this program, for other programs work smoothly.");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_WIPE_ANIME_STR = _T("Disable dark change effects of the screen.");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_WIPE_ANIME_STR = _T("Skip over the dark change effects of the screen by the mouse click.");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_MWND_ANIME_STR = _T("Invalidate the open-close animation of the message window.");
const TSTR	LCL_STR_SYSTEM_DIALOG_WHEEL_NEXT_MESSAGE_STR = _T("Go on reading messages by turn of the mouse's wheel button.");
const TSTR	LCL_STR_SYSTEM_DIALOG_KOE_DONT_STOP_STR = _T("Continue with the previous voice playing even if the text got the next sentence.");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_UNREAD_MESSAGE_STR = _T("Fast forward not only read sentences but also unread sentences.");
const TSTR	LCL_STR_SYSTEM_DIALOG_PLAY_SILENT_SOUND_STR = _T("If you pick up a noise at playing sounds, please check it.");

// ****************************************************************
// ローカライズ：文字列：確認メッセージ
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RESTART_WARNING_STR = _T("Are you sure you want to start from beginning of the game?");		// ★：ゲームを最初から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_SCENESTART_WARNING_STR = _T("Are you sure you want to start sequel to the game?");			// ★：ゲームを途中から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_RETURNMENU_WARNING_STR = _T("Are you sure you want to return to the title screen?");			// ★：タイトルメニューに戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RETURNSEL_WARNING_STR = _T("Are you sure you want to back to the latest choice?");			// ★：前の選択肢に戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_GAMEEND_WARNING_STR = _T("Are you sure you want to exit the game?");							// ★：ゲームを終了する時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_SAVE_WARNING_STR = _T("Are you sure you want to save now data?");							// ★：セーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_SAVE_WARNING_STR = _T("Are you sure you want to quick save?");							// ★：クイックセーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_LOAD_WARNING_STR = _T("Are you sure you want to load this data?");							// ★：ロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_LOAD_WARNING_STR = _T("Are you sure you want to quick load?");							// ★：クイックロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_MSGBK_LOAD_WARNING_STR = _T("Are you sure you want to load this data?");						// ★：メッセージバックロード確認の確認文章の文字列

// ****************************************************************
// ローカライズ：解像度変更
// ================================================================

#define		LCL_STR_RESOLUTION_EXPLAIN_STR(cx, cy)\
str_format(\
_T("The recommended resolution of this game is %d * %d.\n") \
_T("\n") \
_T("Please select the %d * %d resolution if it is available and supported by your system.\n") \
_T("If %d * %d is not available, please select a resolution with similar aspect ratio.\n") \
_T("\n") \
_T("[ Additional info for LCD monitors ]\n") \
_T("Even if you select a resolution with the same aspect ratio, the image may appear blurred or pixelated.") \
_T("This is due to the specs of your monitor.\n") \
_T("\n") \
_T("Even if you select a similar aspect ratio the image may be stretched vertically or horizontally.") \
_T("This is also due to the monitor specs.") \
_T("Please select an aspect ratio similar to the original.") \
, cx, cy, cx, cy, cx, cy)

#define		LCL_STR_CHANGE_RESOLUTION_WARNING_STR	\
_T("The display resolution will now be changed.\n") \
_T("\n") \
_T("[ Attention ]\n") \
_T("The game's visuals may become distorted or black out.\n") \
_T("After 20 seconds, the game will automatically return to the previous resolution.\n") \
_T("You may also hit the Esc key to return to the previous resolution.\n") \
_T("\n") \
_T("[ About LCD monitors ]\n") \
_T("If you change the resolution the display may turn and stay black.\n") \
_T("This happens when your monitor doesn't support the video signal, and therefore doesn't show anything.\n") \
_T("The video signal will return if you reset your monitor.")

#define		LCL_STR_CHANGE_RESOLUTION_AFTER_STR(cnt)\
str_format(\
_T("The display resolution has been changed.\n") \
_T("The game will return to the previous resolution in %d seconds.\n") \
_T("Keep these display settings?") \
, cnt)

// ****************************************************************
// ローカライズ：エラーメッセージ
// ================================================================

// フォントエラー
#define		LCL_STR_NOT_EXIST_FONT(font)\
_T("The requested font is not installed (or corrupt).") \
_T("\n\n　　Current font: ") + font + \
_T("\n\nDo you wish to change the font?") \
_T("\n\n* If you select[No] characters will no longer be shown.")

// ****************************************************************
// ローカライズ：文字列：サポート情報
// ================================================================

const TSTR	LCL_STR_GENERAL_SUPPORT_STR = _T("\n\n[Support Info]\nYou can copy this message by pressing Ctrl+C on your keyboard.");

// ****************************************************************
// ローカライズ：文字列：Twitter
// ================================================================

const TSTR	LCL_STR_TWITTER_DIALOG_TWITTER = _T("Twitter");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_AUTH = _T("認証");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_POST = _T("Twitter へ投稿");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_NEED = _T("投稿するには認証が必要です。");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_SUCCESS = _T("認証に成功しました。");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_POST_SUCCESS = _T("投稿しました。");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_SAMPLE = _T("Siglus からの投稿です。");

// ****************************************************************
// ローカライズ：ダイアログリソース
// ================================================================

// 共通ダイアログ
const int	LCL_IDD_CFG_WIN_BASE_COMMON = IDD_CFG_WIN_BASE_COMMON_EN;
const int	LCL_IDD_CFG_WIN_SOLO_COMMON = IDD_CFG_WIN_SOLO_COMMON_EN;

// （旧）個別ダイアログ
const int	LCL_IDD_CFG_WIN_BASE = IDD_CFG_WIN_BASE_EN;
//const int	LCL_IDD_CFG_WIN_BASE_AUTOMODE = IDD_CFG_WIN_BASE_AUTOMODE_EN;
//const int	LCL_IDD_CFG_WIN_BASE_CHRKOE = IDD_CFG_WIN_BASE_CHRKOE;							// ▲未翻訳
const int	LCL_IDD_CFG_WIN_BASE_ELSE = IDD_CFG_WIN_BASE_ELSE;
//const int	LCL_IDD_CFG_WIN_BASE_FONT_MSGSPD = IDD_CFG_WIN_BASE_FONT_MSGSPD_EN;
const int	LCL_IDD_CFG_WIN_BASE_JITAN = IDD_CFG_WIN_BASE_JITAN;							// ▲未翻訳
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE = IDD_CFG_WIN_BASE_KOEMODE;						// ▲未翻訳
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE_CHRKOE = IDD_CFG_WIN_BASE_KOEMODE_CHRKOE;			// ▲未翻訳
const int	LCL_IDD_CFG_WIN_BASE_MWNDBK = IDD_CFG_WIN_BASE_MWNDBK_EN;
const int	LCL_IDD_CFG_WIN_BASE_SCREEN = IDD_CFG_WIN_BASE_SCREEN_EN;
const int	LCL_IDD_CFG_WIN_BASE_SYSTEM = IDD_CFG_WIN_BASE_SYSTEM_EN;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME = IDD_CFG_WIN_BASE_VOLUME_EN;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME_BGMFADE = IDD_CFG_WIN_BASE_VOLUME_BGMFADE_EN;
//const int	LCL_IDD_CFG_WIN_SOLO_AUTOMODE = IDD_CFG_WIN_SOLO_AUTOMODE_EN;
//const int	LCL_IDD_CFG_WIN_SOLO_BGMFADE = IDD_CFG_WIN_SOLO_BGMFADE_EN;
//const int	LCL_IDD_CFG_WIN_SOLO_CHRKOE = IDD_CFG_WIN_SOLO_CHRKOE;							// ▲未翻訳
const int	LCL_IDD_CFG_WIN_SOLO_ELSE = IDD_CFG_WIN_SOLO_ELSE;								// ▲未翻訳
//const int	LCL_IDD_CFG_WIN_SOLO_FONT = IDD_CFG_WIN_SOLO_FONT_EN;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT_MSGSPD = IDD_CFG_WIN_SOLO_FONT_MSGSPD_EN;
const int	LCL_IDD_CFG_WIN_SOLO_JITAN = IDD_CFG_WIN_SOLO_JITAN;							// ▲未翻訳
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE = IDD_CFG_WIN_SOLO_KOEMODE;						// ▲未翻訳
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE = IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE;			// ▲未翻訳
//const int	LCL_IDD_CFG_WIN_SOLO_MSGSPD = IDD_CFG_WIN_SOLO_MSGSPD_EN;
const int	LCL_IDD_CFG_WIN_SOLO_MOVTYPE = IDD_CFG_WIN_SOLO_MOVTYPE_EN;
const int	LCL_IDD_CFG_WIN_SOLO_MWNDBK = IDD_CFG_WIN_SOLO_MWNDBK_EN;
const int	LCL_IDD_CFG_WIN_SOLO_SCREEN = IDD_CFG_WIN_SOLO_SCREEN_EN;
const int	LCL_IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE = IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE;		// ▲未翻訳
const int	LCL_IDD_CFG_WIN_SOLO_SYSTEM = IDD_CFG_WIN_SOLO_SYSTEM_EN;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME = IDD_CFG_WIN_SOLO_VOLUME_EN;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME_BGMFADE = IDD_CFG_WIN_SOLO_VOLUME_BGMFADE_EN;
const int	LCL_IDD_SYS_WIN_SOLO_SAVELOAD = IDD_SYS_WIN_SOLO_SAVELOAD;
const int	LCL_IDD_CHANGE_RESOLUTION_WARNING = IDD_CHANGE_RESOLUTION_WARNING_EN;
const int	LCL_IDD_TWEET = IDD_TWEET;

