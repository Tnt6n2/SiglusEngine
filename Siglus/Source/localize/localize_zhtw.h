#pragma		once

// ****************************************************************
// ローカライズ：言語コード
// ================================================================

#define		LCL_LANGUAGE_CODE	_T("zh-tw")

// ****************************************************************
// ローカライズ：Gameexe.ini
// ================================================================

#define		LCL_GAMEEXE_DAT		_T("GameexeZHTW.dat")

// ****************************************************************
// ローカライズ：Scene.pck
// ================================================================

#define		LCL_SCENE_PCK		_T("SceneZHTW.pck")

// ****************************************************************
// ローカライズ：セーブフォルダ
// ================================================================

#define		LCL_SAVEDATA_DIR	_T("savedata_zhtw")

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
const TSTR	LCL_STR_SYSCOMMENU_READ_SKIP_STR = _T("快進已讀文本");							// ★：一度読んだ文章の早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_UNREAD_SKIP_STR = _T("快進文本");							// ★：未読の文章でも早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_AKIP_STR = _T("自動快進");								// ★：自動早送り機能をＯＮ／ＯＦＦします。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_MODE_STR = _T("自動播放");								// ★：オートモードを開始します。
const TSTR	LCL_STR_SYSCOMMENU_HIDE_MWND_STR = _T("隱藏對話框");							// ★：メッセージウィンドウを隠します。
const TSTR	LCL_STR_SYSCOMMENU_MSG_BACK_STR = _T("打開對話記錄");							// ★：メッセージバックを開きます。
const TSTR	LCL_STR_SYSCOMMENU_SAVE_STR = _T("儲存");										// ★：セーブを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_LOAD_STR = _T("讀取");										// ★：ロードを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_SEL_STR = _T("返回上一個選項");						// ★：前の選択肢に戻ります。
const TSTR	LCL_STR_SYSCOMMENU_CONFIG_STR = _T("環境設置");									// ★：環境設定を呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_MANUAL_STR = _T("打開幫助");									// ★：マニュアルを表示します。
const TSTR	LCL_STR_SYSCOMMENU_VERSION_STR = _T("版本訊息");								// ★：バージョン情報を表示します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_MENU_STR = _T("返回標題畫面");						// ★：タイトルメニューに戻ります。
const TSTR	LCL_STR_SYSCOMMENU_GAME_END_STR = _T("結束游戲");								// ★：ゲームを終了します。
const TSTR	LCL_STR_SYSCOMMENU_CANCEL_STR = _T("取消");										// ★：システムコマンドメニューをキャンセルします。
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_SWITCH_STR(i)	(str_format(_T("本地通用開關 %d 號"), i))		// ★：ローカル汎用スイッチ：スイッチ名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_STR(i)		(str_format(_T("本地通用模式 %d 號"), i))		// ★：ローカル汎用モード：モード名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_STR(i)	(str_format(_T("模式 %d 號"), i))				// ★：ローカル汎用モード：アイテム名

// ****************************************************************
// ローカライズ：文字列：ダイアログ
// ================================================================

// ダイアログ共通
const TSTR	LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR = _T("決定");
const TSTR	LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR = _T("關閉");
const TSTR	LCL_STR_MODAL_DIALOG_INIT_BTN_STR = _T("恢復原始設置");

// セーブダイアログ
const TSTR	LCL_STR_SAVE_DIALOG_CAPTION_TITLE_STR = _T("儲存");								// ★：セーブダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DECIDE_BUTTON_STR = _T("儲存");								// ★：セーブダイアログの決定ボタンの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DATALIST_NAMEHEADER_STR = _T("儲存");						// ★：セーブダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_SAVE_DIALOG_WARNING_CHECKBOX_STR = _T("存儲前顯示覆蓋存檔的確認窗口");	// ★：セーブダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DBLCLK_CHECKBOX_STR = _T("雙擊列表以儲存");					// ★：セーブダイアログのダブルクリックチェックボックスの文字列

// ロードダイアログ
const TSTR	LCL_STR_LOAD_DIALOG_CAPTION_TITLE_STR = _T("讀取");								// ★：ロードダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DECIDE_BUTTON_STR = _T("讀取");								// ★：ロードダイアログの決定ボタンの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DATALIST_NAMEHEADER_STR = _T("讀取");						// ★：ロードダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_LOAD_DIALOG_WARNING_CHECKBOX_STR = _T("讀取前顯示確認窗口");			// ★：ロードダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DBLCLK_CHECKBOX_STR = _T("雙擊以讀取");						// ★：ロードダイアログのダブルクリックチェックボックスの文字列

// セーブダイアログ＆ロードダイアログの共通情報
const TSTR	LCL_STR_SAVELOAD_DIALOG_NO_DATA_STR = _T("無存檔數據");

// ユーザコンフィグ：環境設定ダイアログ
const TSTR	LCL_STR_CONFIG_DIALOG_TITLE = _T("環境設置");
const TSTR	LCL_STR_CONFIG_DIALOG_SCREEN_STR = _T("畫面");
const TSTR	LCL_STR_CONFIG_DIALOG_VOLUME_STR = _T("音量");
const TSTR	LCL_STR_CONFIG_DIALOG_MESSAGE_STR = _T("文本");
const TSTR	LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR = _T("背景顔色");
const TSTR	LCL_STR_CONFIG_DIALOG_VOICE_STR = _T("聲音");
const TSTR	LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR = _T("自動模式");
const TSTR	LCL_STR_CONFIG_DIALOG_JITAN_STR = _T("縮短播放時間");
const TSTR	LCL_STR_CONFIG_DIALOG_OTHERS_STR = _T("其他");
const TSTR	LCL_STR_CONFIG_DIALOG_SYSTEM_STR = _T("系統");
const TSTR	LCL_STR_CONFIG_DIALOG_CLOSE_ALL = _T("全部初始化");

// ユーザコンフィグ：画面ダイアログ
const TSTR	LCL_STR_SCREEN_DIALOG_CURRENT_RESOLUTION_STR = _T("（當前解析度）");
const TSTR	LCL_STR_SCREEN_DIALOG_DEFAULT_RESOLUTION_STR = _T("（原解析度）");

// ユーザコンフィグ：音量・ＢＧＭフェードダイアログ
const TSTR	LCL_STR_VOLUME_BGMFADE_DIALOG_DIALOG_TITLE = _T("音量設置");

// ユーザコンフィグ：音量ダイアログ
const TSTR	LCL_STR_VOLUME_DIALOG_DIALOG_TITLE = _T("音量設置");
const TSTR	LCL_STR_VOLUME_DIALOG_GROUP_TITLE = _T("音量設置");
const TSTR	LCL_STR_VOLUME_DIALOG_ALL_GROUP_TITLE = _T("整體音量");
const TSTR	LCL_STR_VOLUME_DIALOG_BGM_GROUP_TITLE = _T("BGM");
const TSTR	LCL_STR_VOLUME_DIALOG_KOE_GROUP_TITLE = _T("角色聲音");
const TSTR	LCL_STR_VOLUME_DIALOG_PCM_GROUP_TITLE = _T("效果音");
const TSTR	LCL_STR_VOLUME_DIALOG_SE_GROUP_TITLE = _T("系統音");
const TSTR	LCL_STR_VOLUME_DIALOG_MOV_GROUP_TITLE = _T("影片聲音");
const TSTR	LCL_STR_VOLUME_DIALOG_STC_PLAY = _T("播放");

// ユーザコンフィグ：ＢＧＭフェードダイアログ
const TSTR	LCL_STR_BGMFADE_DIALOG_DIALOG_TITLE = _T("BGM淡出設置");
const TSTR	LCL_STR_BGMFADE_DIALOG_GROUP_TITLE = _T("BGM淡出");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_EXPLAINE = _T("播放角色聲音時調整BGM音量的功能");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_ENABLE = _T("使用");

// ユーザコンフィグ：フォント・メッセージスピードダイアログ
const TSTR	LCL_STR_FONT_MSGSPD_DIALOG_DIALOG_TITLE = _T("文本設置");

// ユーザコンフィグ：フォントダイアログ
const TSTR	LCL_STR_FONT_DIALOG_DIALOG_TITLE = _T("字體設置");
const TSTR	LCL_STR_FONT_DIALOG_GROUP_TITLE = _T("字體設置");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_ONOFF = _T("顯示比例字體");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_DESC = _T("※使用比例字體可能導致文本排版顯示異常。");
const TSTR	LCL_STR_FONT_DIALOG_STC_BOLD = _T("文字加粗");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_OFF = _T("無陰影");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_ON = _T("有陰影");
const TSTR	LCL_STR_FONT_DIALOG_STC_BORDER_ON = _T("顯示文字描邊");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON = _T("描邊+陰影");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_SHORT = _T("「請不要將天堂…分開」");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_LONG = _T("「請不要將天堂…分開」");

// ユーザコンフィグ：メッセージスピードダイアログ
const TSTR	LCL_STR_MSGSPD_DIALOG_DIALOG_TITLE = _T("文字速度設置");
const TSTR	LCL_STR_MSGSPD_DIALOG_GROUP_TITLE = _T("文字速度");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_SLOW = _T("慢");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_FAST = _T("快");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_NOWAIT = _T("不等待");

// ユーザーコンフィグ：音声ダイアログ
const TSTR	LCL_STR_KOEMOD_CHRKOE_DIALOG_DIALOG_TITLE = _T("聲音設定");

// ユーザーコンフィグ：音声モードダイアログ
const TSTR	LCL_STR_KOEMOD_DIALOG_DIALOG_TITLE = _T("聲音模式設定");
const TSTR	LCL_STR_KOEMOD_DIALOG_GROUP_TITLE = _T("聲音模式");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ON = _T("有聲");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_OFF = _T("無聲");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ONLY = _T("僅聲音（無文字）");

// ユーザーコンフィグ：キャラクター音声ダイアログ
const TSTR	LCL_STR_CHRKOE_DIALOG_DIALOG_TITLE = _T("角色語音設定");
const TSTR	LCL_STR_CHRKOE_DIALOG_GROUP_TITLE = _T("角色語音");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_ON = _T("全部開啟");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_OFF = _T("全部關閉");

// ユーザーコンフィグ：オートモードダイアログ
const TSTR	LCL_STR_DIALOG_AUTOMODE_DIALOG_TITLE = _T("自動模式（文本自動播放功能）設置");
const TSTR	LCL_STR_DIALOG_AUTOMODE_GROUP_TITLE = _T("自動模式（文本自動播放功能）");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_EXPLAINE = _T("等待時間=「（顯示文字數量×文字時間）+最小時間」");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MOJI_TIME = _T("文字時間");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MIN_TIME = _T("最小時間");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_TEN_SECONDS = _T("顯示每10個文字時的等待時間");
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_1(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%d"), n2) + _T(" 秒")
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_2(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%02d"), n2) + _T(" 秒")

// ユーザコンフィグ：システムダイアログ
const TSTR	LCL_STR_SYSTEM_DIALOG_SLEEP_STR = _T("延遲本遊戲程序，以使其他程序流暢運行。");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_WIPE_ANIME_STR = _T("停用畫面變暗的動畫效果。");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_WIPE_ANIME_STR = _T("滑鼠單擊以跳過畫面變暗效果。");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_MWND_ANIME_STR = _T("停用打開與關閉對話框時的動畫效果。");
const TSTR	LCL_STR_SYSTEM_DIALOG_WHEEL_NEXT_MESSAGE_STR = _T("向下滾動滑鼠滾輪以閱讀文本。");
const TSTR	LCL_STR_SYSTEM_DIALOG_KOE_DONT_STOP_STR = _T("閱讀下壹條文本時不停止聲音的播放。");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_UNREAD_MESSAGE_STR = _T("快進未讀文本。");
const TSTR	LCL_STR_SYSTEM_DIALOG_PLAY_SILENT_SOUND_STR = _T("聲音播放時如果存在雜音，請檢查。");

// ****************************************************************
// ローカライズ：文字列：確認メッセージ
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RESTART_WARNING_STR = _T("要從頭開始嗎？");				// ★：ゲームを最初から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_SCENESTART_WARNING_STR = _T("要從中途開始嗎？");			// ★：ゲームを途中から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_RETURNMENU_WARNING_STR = _T("返回上一個選項嗎？");		// ★：タイトルメニューに戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RETURNSEL_WARNING_STR = _T("返回上一個選項嗎？");		// ★：前の選択肢に戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_GAMEEND_WARNING_STR = _T("結束游戲嗎？");				// ★：ゲームを終了する時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_SAVE_WARNING_STR = _T("儲存進度嗎？");					// ★：セーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_SAVE_WARNING_STR = _T("快速儲存進度嗎？");			// ★：クイックセーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_LOAD_WARNING_STR = _T("讀取進度嗎？");					// ★：ロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_LOAD_WARNING_STR = _T("快速讀取進度嗎？");			// ★：クイックロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_MSGBK_LOAD_WARNING_STR = _T("讀取進度嗎？");				// ★：メッセージバックロード確認の確認文章の文字列

// ****************************************************************
// ローカライズ：解像度変更
// ================================================================

#define		LCL_STR_RESOLUTION_EXPLAIN_STR(cx, cy)\
str_format(\
_T("本作品的最佳匹配解析度為 %d × %d。\n") \
_T("\n") \
_T("如果可以設置 %d × %d 解析度，請以此進行設置。\n") \
_T("如果無法設置，請盡量選擇最接近“最佳匹配解析度”的畫面比例。\n") \
_T("\n") \
_T("<有關液晶顯示器的補充說明>\n") \
_T("即使設置成“最佳匹配解析度”，也可能出現顯示模糊或者粗糙的情況。\n") \
_T("即使設置成“最佳匹配解析度”，也可能出現橫向或者縱向拉長的情況。\n") \
_T("這屬於顯示器性能的問題，請予以諒解。\n") \
, cx, cy, cx, cy)

#define		LCL_STR_CHANGE_RESOLUTION_WARNING_STR	\
_T("將更改畫面解析度。\n") \
_T("\n") \
_T("<註意>\n") \
_T("因解析度不同，可能會出現畫面閃爍的狀況。\n") \
_T("更改解析度後，如果畫面仍未正常顯示，請耐心等待。\n") \
_T("20 秒後將會恢復到原來的設置狀態。\n") \
_T("\n") \
_T("<有關液晶顯示器的說明>\n") \
_T("更改解析度時，可能會出現黑屏現象。\n") \
_T("此情況是由於顯示器無法正常收到信號，切換到無信號狀態後所導致。\n") \
_T("重啟顯示器電源後即可正常顯示。")

#define		LCL_STR_CHANGE_RESOLUTION_AFTER_STR(cnt)\
str_format( \
_T("解析度已更改。\n") \
_T("%d 秒後將恢復原本的解析度。") \
, cnt)


// ****************************************************************
// ローカライズ：エラーメッセージ
// ================================================================

// フォントエラー
#define		LCL_STR_NOT_EXIST_FONT(font)\
_T("未發現字體。（或字體已損壞。）") \
_T("\n\n　　當前字體: ") + font + \
_T("\n\nDo you wish to change the font?") \
_T("\n\n※若選擇“否”，將不會顯示文字。")

// ****************************************************************
// ローカライズ：文字列：サポート情報
// ================================================================

// 各種確認メッセージの文字列
const TSTR	LCL_STR_GENERAL_SUPPORT_STR = _T("\n\n[ 幫助訊息 ]\n按下 Ctrl+C 可復制該訊息。");

// ****************************************************************
// ローカライズ：文字列：Twitter
// ================================================================

const TSTR	LCL_STR_TWITTER_DIALOG_TWITTER = _T("推特");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_AUTH = _T("驗證");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_POST = _T("向推特發文");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_NEED = _T("發文須經驗證");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_SUCCESS = _T("驗證成功");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_POST_SUCCESS = _T("已發文");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_SAMPLE = _T("來自 Siglus 的發文");

// ****************************************************************
// ローカライズ：ダイアログリソース
// ================================================================

// 共通ダイアログ
const int	LCL_IDD_CFG_WIN_BASE_COMMON = IDD_CFG_WIN_BASE_COMMON_ZHTW;
const int	LCL_IDD_CFG_WIN_SOLO_COMMON = IDD_CFG_WIN_SOLO_COMMON_ZHTW;

// （旧）個別ダイアログ
const int	LCL_IDD_CFG_WIN_BASE = IDD_CFG_WIN_BASE_ZHTW;
//const int	LCL_IDD_CFG_WIN_BASE_AUTOMODE = IDD_CFG_WIN_BASE_AUTOMODE;
//const int	LCL_IDD_CFG_WIN_BASE_CHRKOE = IDD_CFG_WIN_BASE_CHRKOE;
const int	LCL_IDD_CFG_WIN_BASE_ELSE = IDD_CFG_WIN_BASE_ELSE_ZHTW;
//const int	LCL_IDD_CFG_WIN_BASE_FONT_MSGSPD = IDD_CFG_WIN_BASE_FONT_MSGSPD;
const int	LCL_IDD_CFG_WIN_BASE_JITAN = IDD_CFG_WIN_BASE_JITAN_ZHTW;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE = IDD_CFG_WIN_BASE_KOEMODE;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE_CHRKOE = IDD_CFG_WIN_BASE_KOEMODE_CHRKOE;
const int	LCL_IDD_CFG_WIN_BASE_MWNDBK = IDD_CFG_WIN_BASE_MWNDBK_ZHTW;
const int	LCL_IDD_CFG_WIN_BASE_SCREEN = IDD_CFG_WIN_BASE_SCREEN_ZHTW;
const int	LCL_IDD_CFG_WIN_BASE_SYSTEM = IDD_CFG_WIN_BASE_SYSTEM_ZHTW;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME = IDD_CFG_WIN_BASE_VOLUME;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME_BGMFADE = IDD_CFG_WIN_BASE_VOLUME_BGMFADE;
//const int	LCL_IDD_CFG_WIN_SOLO_AUTOMODE = IDD_CFG_WIN_SOLO_AUTOMODE;
//const int	LCL_IDD_CFG_WIN_SOLO_BGMFADE = IDD_CFG_WIN_SOLO_BGMFADE;
//const int	LCL_IDD_CFG_WIN_SOLO_CHRKOE = IDD_CFG_WIN_SOLO_CHRKOE;
const int	LCL_IDD_CFG_WIN_SOLO_ELSE = IDD_CFG_WIN_SOLO_ELSE;								// ★翻訳なし
//const int	LCL_IDD_CFG_WIN_SOLO_FONT = IDD_CFG_WIN_SOLO_FONT;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT_MSGSPD = IDD_CFG_WIN_SOLO_FONT_MSGSPD;
const int	LCL_IDD_CFG_WIN_SOLO_JITAN = IDD_CFG_WIN_SOLO_JITAN_ZHTW;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE = IDD_CFG_WIN_SOLO_KOEMODE;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE = IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE;
//const int	LCL_IDD_CFG_WIN_SOLO_MSGSPD = IDD_CFG_WIN_SOLO_MSGSPD;
const int	LCL_IDD_CFG_WIN_SOLO_MOVTYPE = IDD_CFG_WIN_SOLO_MOVTYPE_ZHTW;
const int	LCL_IDD_CFG_WIN_SOLO_MWNDBK = IDD_CFG_WIN_SOLO_MWNDBK_ZHTW;
const int	LCL_IDD_CFG_WIN_SOLO_SCREEN = IDD_CFG_WIN_SOLO_SCREEN_ZHTW;
const int	LCL_IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE = IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE;		// ★翻訳なし
const int	LCL_IDD_CFG_WIN_SOLO_SYSTEM = IDD_CFG_WIN_SOLO_SYSTEM_ZHTW;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME = IDD_CFG_WIN_SOLO_VOLUME;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME_BGMFADE = IDD_CFG_WIN_SOLO_VOLUME_BGMFADE;
const int	LCL_IDD_SYS_WIN_SOLO_SAVELOAD = IDD_SYS_WIN_SOLO_SAVELOAD;						// ★翻訳なし
const int	LCL_IDD_CHANGE_RESOLUTION_WARNING = IDD_CHANGE_RESOLUTION_WARNING_ZHTW;
const int	LCL_IDD_TWEET = IDD_TWEET_ZHTW;

