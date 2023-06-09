#pragma		once

// ****************************************************************
// ローカライズ：言語コード
// ================================================================

#define		LCL_LANGUAGE_CODE	_T("ja")

// ****************************************************************
// ローカライズ：Gameexe.ini
// ================================================================

#define		LCL_GAMEEXE_DAT		_T("Gameexe.dat")

// ****************************************************************
// ローカライズ：Scene.pck
// ================================================================

#define		LCL_SCENE_PCK		_T("Scene.pck")

// ****************************************************************
// ローカライズ：セーブフォルダ
// ================================================================

#define		LCL_SAVEDATA_DIR	_T("savedata")

// ****************************************************************
// ローカライズ：ユーザーフォルダ
// ================================================================

#define		LCL_USERDATA_DIR	_T("userdata")

// ****************************************************************
// ローカライズ：日付表記
// ================================================================

#define		LCL_DATE_FORMAT		str_format(_T("%02d/%02d/%02d %02d:%02d:%02d"), p_header->year, p_header->month, p_header->day, p_header->hour, p_header->minute, p_header->second);

// ****************************************************************
// ローカライズ：文字列：システムコマンドメニュー
// ================================================================

// システムコマンドメニュー：文字列
const TSTR	LCL_STR_SYSCOMMENU_READ_SKIP_STR = _T("既読文章を早送り");							// ★：一度読んだ文章の早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_UNREAD_SKIP_STR = _T("文章を早送り");							// ★：未読の文章でも早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_AKIP_STR = _T("自動早送り");								// ★：自動早送り機能をＯＮ／ＯＦＦします。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_MODE_STR = _T("オートモード");								// ★：オートモードを開始します。
const TSTR	LCL_STR_SYSCOMMENU_HIDE_MWND_STR = _T("メッセージを隠す");							// ★：メッセージウィンドウを隠します。
const TSTR	LCL_STR_SYSCOMMENU_MSG_BACK_STR = _T("メッセージログを開く");						// ★：メッセージログを開きます。
const TSTR	LCL_STR_SYSCOMMENU_SAVE_STR = _T("セーブ");											// ★：セーブを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_LOAD_STR = _T("ロード");											// ★：ロードを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_SEL_STR = _T("前の選択肢に戻る");							// ★：前の選択肢に戻ります。
const TSTR	LCL_STR_SYSCOMMENU_CONFIG_STR = _T("環境設定");										// ★：環境設定を呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_MANUAL_STR = _T("マニュアル");									// ★：マニュアルを表示します。
const TSTR	LCL_STR_SYSCOMMENU_VERSION_STR = _T("バージョン情報");								// ★：バージョン情報を表示します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_MENU_STR = _T("タイトルに戻る");							// ★：タイトルメニューに戻ります。
const TSTR	LCL_STR_SYSCOMMENU_GAME_END_STR = _T("ゲームを終了する");							// ★：ゲームを終了します。
const TSTR	LCL_STR_SYSCOMMENU_CANCEL_STR = _T("キャンセル");									// ★：システムコマンドメニューをキャンセルします。
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_SWITCH_STR(i)	(_T("ローカル汎用スイッチ") + str_to_zenkaku_ret(str_format(_T("%d"), i)) + _T("番"))	// ★：ローカル汎用スイッチ：モード名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_STR(i)		(_T("ローカル汎用モード") + str_to_zenkaku_ret(str_format(_T("%d"), i)) + _T("番"))		// ★：ローカル汎用モード：モード名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_STR(i)	(_T("モード") + str_to_zenkaku_ret(str_format(_T("%d"), i)))							// ★：ローカル汎用モード：アイテム名

// ****************************************************************
// ローカライズ：文字列：ダイアログ
// ================================================================

// ダイアログ共通
const TSTR	LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR = _T("決定");
const TSTR	LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR = _T("閉じる");
const TSTR	LCL_STR_MODAL_DIALOG_INIT_BTN_STR = _T("初期状態に戻す");

// セーブダイアログ
const TSTR	LCL_STR_SAVE_DIALOG_CAPTION_TITLE_STR = _T("セーブ");												// ★：セーブダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DECIDE_BUTTON_STR = _T("セーブ");												// ★：セーブダイアログの決定ボタンの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DATALIST_NAMEHEADER_STR = _T("セーブ");											// ★：セーブダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_SAVE_DIALOG_WARNING_CHECKBOX_STR = _T("セーブする前に上書きの確認ウィンドウを表示する。");	// ★：セーブダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DBLCLK_CHECKBOX_STR = _T("リストをダブルクリックでセーブする。");				// ★：セーブダイアログのダブルクリックチェックボックスの文字列

// ロードダイアログ
const TSTR	LCL_STR_LOAD_DIALOG_CAPTION_TITLE_STR = _T("ロード");											// ★：ロードダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DECIDE_BUTTON_STR = _T("ロード");											// ★：ロードダイアログの決定ボタンの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DATALIST_NAMEHEADER_STR = _T("ロード");										// ★：ロードダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_LOAD_DIALOG_WARNING_CHECKBOX_STR = _T("ロードする前に確認ウィンドウを表示する。");		// ★：ロードダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DBLCLK_CHECKBOX_STR = _T("リストをダブルクリックでロードする。");			// ★：ロードダイアログのダブルクリックチェックボックスの文字列

// セーブダイアログ＆ロードダイアログの共通情報
const TSTR	LCL_STR_SAVELOAD_DIALOG_NO_DATA_STR = _T("データがありません。");

// ユーザコンフィグ：環境設定ダイアログ
const TSTR	LCL_STR_CONFIG_DIALOG_SCREEN_STR = _T("画面");
const TSTR	LCL_STR_CONFIG_DIALOG_VOLUME_STR = _T("音量");
const TSTR	LCL_STR_CONFIG_DIALOG_MESSAGE_STR = _T("文章");
const TSTR	LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR = _T("背景色");
const TSTR	LCL_STR_CONFIG_DIALOG_VOICE_STR = _T("音声");
const TSTR	LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR = _T("オートモード");
const TSTR	LCL_STR_CONFIG_DIALOG_JITAN_STR = _T("時短再生");
const TSTR	LCL_STR_CONFIG_DIALOG_OTHERS_STR = _T("その他");
const TSTR	LCL_STR_CONFIG_DIALOG_SYSTEM_STR = _T("システム");

// ユーザコンフィグ：画面ダイアログ
const TSTR	LCL_STR_SCREEN_DIALOG_CURRENT_RESOLUTION_STR = _T("（現在の解像度）");
const TSTR	LCL_STR_SCREEN_DIALOG_DEFAULT_RESOLUTION_STR = _T("（元の解像度）");

// ユーザコンフィグ：音量・ＢＧＭフェードダイアログ
const TSTR	LCL_STR_VOLUME_BGMFADE_DIALOG_DIALOG_TITLE = _T("音量の設定");

// ユーザコンフィグ：音量ダイアログ
const TSTR	LCL_STR_VOLUME_DIALOG_DIALOG_TITLE = _T("音量の設定");
const TSTR	LCL_STR_VOLUME_DIALOG_GROUP_TITLE = _T("音量");
const TSTR	LCL_STR_VOLUME_DIALOG_ALL_GROUP_TITLE = _T("全体");
const TSTR	LCL_STR_VOLUME_DIALOG_BGM_GROUP_TITLE = _T("BGM");
const TSTR	LCL_STR_VOLUME_DIALOG_KOE_GROUP_TITLE = _T("音声");
const TSTR	LCL_STR_VOLUME_DIALOG_PCM_GROUP_TITLE = _T("効果音");
const TSTR	LCL_STR_VOLUME_DIALOG_SE_GROUP_TITLE = _T("ｼｽﾃﾑ音");
const TSTR	LCL_STR_VOLUME_DIALOG_MOV_GROUP_TITLE = _T("ﾑｰﾋﾞｰ");
const TSTR	LCL_STR_VOLUME_DIALOG_STC_PLAY = _T("再生");

// ユーザコンフィグ：ＢＧＭフェードダイアログ
const TSTR	LCL_STR_BGMFADE_DIALOG_DIALOG_TITLE = _T("BGMフェードの設定");
const TSTR	LCL_STR_BGMFADE_DIALOG_GROUP_TITLE = _T("BGMフェード");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_EXPLAINE = _T("音声の再生時、BGMの音量を調整する機能です。");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_ENABLE = _T("使用する");

// ユーザコンフィグ：フォント・メッセージスピードダイアログ
const TSTR	LCL_STR_FONT_MSGSPD_DIALOG_DIALOG_TITLE = _T("文章の設定");

// ユーザコンフィグ：フォントダイアログ
const TSTR	LCL_STR_FONT_DIALOG_DIALOG_TITLE = _T("フォントの設定");
const TSTR	LCL_STR_FONT_DIALOG_GROUP_TITLE = _T("フォント");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_ONOFF = _T("プロポーショナルフォントを表示する");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_DESC = _T("※プロポーショナルフォントを使うと、文章のレイアウトが崩れる可能性があります。");
const TSTR	LCL_STR_FONT_DIALOG_STC_BOLD = _T("文字を太くする");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_OFF = _T("影なし");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_ON = _T("影あり");
const TSTR	LCL_STR_FONT_DIALOG_STC_BORDER_ON = _T("縁取る");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON = _T("縁＋影");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_SHORT = _T("「あいう漢字カナ薔薇」");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_LONG = _T("「あいう漢字カナＡＢＣ０１２薔薇」");

// ユーザコンフィグ：メッセージスピードダイアログ
const TSTR	LCL_STR_MSGSPD_DIALOG_DIALOG_TITLE = _T("文字速度の設定");
const TSTR	LCL_STR_MSGSPD_DIALOG_GROUP_TITLE = _T("文字速度");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_SLOW = _T("遅い");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_FAST = _T("速い");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_NOWAIT = _T("ノーウェイト");

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
const TSTR	LCL_STR_DIALOG_AUTOMODE_DIALOG_TITLE = _T("オートモード （文章の自動送り機能）の設定");
const TSTR	LCL_STR_DIALOG_AUTOMODE_GROUP_TITLE = _T("オートモード （文章の自動送り機能）");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_EXPLAINE = _T("待ち時間は 『 （ 表示した文字数 × 文字時間 ） ＋ 最小時間 』 で計算されます。");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MOJI_TIME = _T("文字時間");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MIN_TIME = _T("最小時間");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_TEN_SECONDS = _T("１０文字表示した際の待ち時間");
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_1(n1, n2)	str_to_zenkaku_ret(str_format(_T("%d"), n1)) + _T("．") + str_to_zenkaku_ret(str_format(_T("%d"), n2)) + _T(" 秒")
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_2(n1, n2)	str_to_zenkaku_ret(str_format(_T("%d"), n1)) + _T("．") + str_to_zenkaku_ret(str_format(_T("%02d"), n2)) + _T(" 秒")

// ユーザコンフィグ：システムダイアログ
const TSTR	LCL_STR_SYSTEM_DIALOG_SLEEP_STR = _T("本プログラムの動作を遅くして、他のプログラムがスムーズに動作するようにする。");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_WIPE_ANIME_STR = _T("画面暗転効果のアニメを無効にする。");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_WIPE_ANIME_STR = _T("画面暗転効果をマウスクリックで飛ばす。");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_MWND_ANIME_STR = _T("メッセージウィンドウの開閉時のアニメを無効にする。");
const TSTR	LCL_STR_SYSTEM_DIALOG_WHEEL_NEXT_MESSAGE_STR = _T("マウスのホイールボタンの下回しで文章を読み進める。");
const TSTR	LCL_STR_SYSTEM_DIALOG_KOE_DONT_STOP_STR = _T("声の再生中に次の文章に進んでも再生を続ける。");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_UNREAD_MESSAGE_STR = _T("未読の文章も早送りできるようにする。");
const TSTR	LCL_STR_SYSTEM_DIALOG_PLAY_SILENT_SOUND_STR = _T("サウンド再生時に雑音が入る場合はチェックして下さい。");

// ****************************************************************
// ローカライズ：文字列：確認メッセージ
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RESTART_WARNING_STR = _T("最初から始めてもよろしいですか？");				// ★：ゲームを最初から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_SCENESTART_WARNING_STR = _T("途中から始めてもよろしいですか？");				// ★：ゲームを途中から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_RETURNMENU_WARNING_STR = _T("タイトルに戻ってもよろしいですか？");			// ★：タイトルメニューに戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RETURNSEL_WARNING_STR = _T("前の選択肢に戻ってもよろしいですか？");			// ★：前の選択肢に戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_GAMEEND_WARNING_STR = _T("終了してもよろしいですか？");						// ★：ゲームを終了する時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_SAVE_WARNING_STR = _T("セーブしてもよろしいですか？");						// ★：セーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_SAVE_WARNING_STR = _T("クイックセーブしてもよろしいですか？");			// ★：クイックセーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_LOAD_WARNING_STR = _T("ロードしてもよろしいですか？");						// ★：ロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_LOAD_WARNING_STR = _T("クイックロードしてもよろしいですか？");			// ★：クイックロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_MSGBK_LOAD_WARNING_STR = _T("ロードしてもよろしいですか？");					// ★：メッセージバックロード確認の確認文章の文字列

// ****************************************************************
// ローカライズ：解像度変更
// ================================================================

#define		LCL_STR_RESOLUTION_EXPLAIN_STR(cx, cy)\
str_format(\
_T("本製品の【最適な解像度】は 『 %d × %d 』 です。\n") \
_T("\n") \
_T("『 %d × %d 』の解像度が選択できる場合は、それを選択して下さい。\n") \
_T("それができない場合は、なるべく【最適な解像度】に近い比率のものを選択して下さい。\n") \
_T("\n") \
_T("≪液晶モニターについての補足≫\n") \
_T("【最適な解像度】にしても表示がぼやけたり、粗くなったりするものがあります。\n") \
_T("また、表示が縦もしくは横に間延びしてしまうものがあります。\n") \
_T("これはモニターの性能上の問題となりますのでご了承ください。") \
, cx, cy, cx, cy)

#define		LCL_STR_CHANGE_RESOLUTION_WARNING_STR	\
_T("解像度を変更します。\n") \
_T("\n") \
_T("≪注意≫\n") \
_T("解像度によっては、画面がちらつく場合があります。\n") \
_T("変更後、画面が正常に表示されない場合は、そのままお待ちください。\n") \
_T("20 秒で元の設定に戻ります。\n") \
_T("\n") \
_T("≪液晶モニターについて≫\n") \
_T("解像度を変更した際、画面が真っ暗なままになる事があります。\n") \
_T("この現象は、モニターが映像信号をうまく受信できず、無信号状態になってしまう為です。\n") \
_T("モニターの電源スイッチを一度ＯＦＦにしてからＯＮにすると正常に表示されます。")

#define		LCL_STR_CHANGE_RESOLUTION_AFTER_STR(cnt)\
str_format( \
_T("解像度を変更しました。\n") \
_T("%d 秒で元の解像度に戻ります。") \
, cnt)

// ****************************************************************
// ローカライズ：エラーメッセージ
// ================================================================

// フォントエラー
#define		LCL_STR_NOT_EXIST_FONT(font)\
_T("フォントが見つかりません。（もしくは、壊れています。）") \
_T("\n\n　　現在のフォント：") + font + \
_T("\n\nフォントを変更しますか？") \
_T("\n\n※ [ いいえ ] を選択した場合、文字が表示されなくなります。")

// ****************************************************************
// ローカライズ：文字列：サポート情報
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_GENERAL_SUPPORT_STR = _T("\n\n[ サポート情報 ]\nこのメッセージは Ctrl+C でコピーできます。");

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
const int	LCL_IDD_CFG_WIN_BASE_COMMON = IDD_CFG_WIN_BASE_COMMON;
const int	LCL_IDD_CFG_WIN_SOLO_COMMON = IDD_CFG_WIN_SOLO_COMMON;

// （旧）個別ダイアログ
const int	LCL_IDD_CFG_WIN_BASE = IDD_CFG_WIN_BASE;
//const int	LCL_IDD_CFG_WIN_BASE_AUTOMODE = IDD_CFG_WIN_BASE_AUTOMODE;
//const int	LCL_IDD_CFG_WIN_BASE_CHRKOE = IDD_CFG_WIN_BASE_CHRKOE;
const int	LCL_IDD_CFG_WIN_BASE_ELSE = IDD_CFG_WIN_BASE_ELSE;
//const int	LCL_IDD_CFG_WIN_BASE_FONT_MSGSPD = IDD_CFG_WIN_BASE_FONT_MSGSPD;
const int	LCL_IDD_CFG_WIN_BASE_JITAN = IDD_CFG_WIN_BASE_JITAN;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE = IDD_CFG_WIN_BASE_KOEMODE;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE_CHRKOE = IDD_CFG_WIN_BASE_KOEMODE_CHRKOE;
const int	LCL_IDD_CFG_WIN_BASE_MWNDBK = IDD_CFG_WIN_BASE_MWNDBK;
const int	LCL_IDD_CFG_WIN_BASE_SCREEN = IDD_CFG_WIN_BASE_SCREEN;
const int	LCL_IDD_CFG_WIN_BASE_SYSTEM = IDD_CFG_WIN_BASE_SYSTEM;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME = IDD_CFG_WIN_BASE_VOLUME;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME_BGMFADE = IDD_CFG_WIN_BASE_VOLUME_BGMFADE;
//const int	LCL_IDD_CFG_WIN_SOLO_AUTOMODE = IDD_CFG_WIN_SOLO_AUTOMODE;
//const int	LCL_IDD_CFG_WIN_SOLO_BGMFADE = IDD_CFG_WIN_SOLO_BGMFADE;
//const int	LCL_IDD_CFG_WIN_SOLO_CHRKOE = IDD_CFG_WIN_SOLO_CHRKOE;
const int	LCL_IDD_CFG_WIN_SOLO_ELSE = IDD_CFG_WIN_SOLO_ELSE;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT = IDD_CFG_WIN_SOLO_FONT;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT_MSGSPD = IDD_CFG_WIN_SOLO_FONT_MSGSPD;
const int	LCL_IDD_CFG_WIN_SOLO_JITAN = IDD_CFG_WIN_SOLO_JITAN;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE = IDD_CFG_WIN_SOLO_KOEMODE;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE = IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE;
//const int	LCL_IDD_CFG_WIN_SOLO_MSGSPD = IDD_CFG_WIN_SOLO_MSGSPD;
const int	LCL_IDD_CFG_WIN_SOLO_MOVTYPE = IDD_CFG_WIN_SOLO_MOVTYPE;
const int	LCL_IDD_CFG_WIN_SOLO_MWNDBK = IDD_CFG_WIN_SOLO_MWNDBK;
const int	LCL_IDD_CFG_WIN_SOLO_SCREEN = IDD_CFG_WIN_SOLO_SCREEN;
const int	LCL_IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE = IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE;
const int	LCL_IDD_CFG_WIN_SOLO_SYSTEM = IDD_CFG_WIN_SOLO_SYSTEM;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME = IDD_CFG_WIN_SOLO_VOLUME;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME_BGMFADE = IDD_CFG_WIN_SOLO_VOLUME_BGMFADE;
const int	LCL_IDD_SYS_WIN_SOLO_SAVELOAD = IDD_SYS_WIN_SOLO_SAVELOAD;
const int	LCL_IDD_CHANGE_RESOLUTION_WARNING = IDD_CHANGE_RESOLUTION_WARNING;
const int	LCL_IDD_TWEET = IDD_TWEET;

