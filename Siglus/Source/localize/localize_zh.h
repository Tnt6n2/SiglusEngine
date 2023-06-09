#pragma		once

// ****************************************************************
// ローカライズ：言語コード
// ================================================================

#define		LCL_LANGUAGE_CODE	_T("zh")

// ****************************************************************
// ローカライズ：Gameexe.ini
// ================================================================

#define		LCL_GAMEEXE_DAT		_T("GameexeZH.dat")

// ****************************************************************
// ローカライズ：Scene.pck
// ================================================================

#define		LCL_SCENE_PCK		_T("SceneZH.pck")

// ****************************************************************
// ローカライズ：セーブフォルダ
// ================================================================

#define		LCL_SAVEDATA_DIR	_T("savedata_zh")

// ****************************************************************
// ローカライズ：ユーザーフォルダ
// ================================================================

#define		LCL_USERDATA_DIR	_T("userdata")

// ****************************************************************
// ローカライズ：日付表記
// ================================================================

extern TCHAR* MONTH_STR_EN[];

#define		LCL_DATE_FORMAT		str_format(_T("%02d/%02d/%02d %02d:%02d:%02d"), p_header->year, p_header->month, p_header->day, p_header->hour, p_header->minute, p_header->second);

// ****************************************************************
// ローカライズ：文字列：システムコマンドメニュー
// ================================================================

// システムコマンドメニュー：文字列
const TSTR	LCL_STR_SYSCOMMENU_READ_SKIP_STR = _T("快进已读文本");								// ★：一度読んだ文章の早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_UNREAD_SKIP_STR = _T("快进文本");								// ★：未読の文章でも早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_AKIP_STR = _T("自动快进");									// ★：自動早送り機能をＯＮ／ＯＦＦします。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_MODE_STR = _T("自动播放");									// ★：オートモードを開始します。
const TSTR	LCL_STR_SYSCOMMENU_HIDE_MWND_STR = _T("隐藏对话框");								// ★：メッセージウィンドウを隠します。
const TSTR	LCL_STR_SYSCOMMENU_MSG_BACK_STR = _T("打开对话记录");								// ★：メッセージバックを開きます。
const TSTR	LCL_STR_SYSCOMMENU_SAVE_STR = _T("存储");											// ★：セーブを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_LOAD_STR = _T("读取");											// ★：ロードを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_SEL_STR = _T("返回上一个选项");							// ★：前の選択肢に戻ります。
const TSTR	LCL_STR_SYSCOMMENU_CONFIG_STR = _T("环境设置");										// ★：環境設定を呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_MANUAL_STR = _T("打开帮助");										// ★：マニュアルを表示します。
const TSTR	LCL_STR_SYSCOMMENU_VERSION_STR = _T("版本信息");									// ★：バージョン情報を表示します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_MENU_STR = _T("返回标题画面");							// ★：タイトルメニューに戻ります。
const TSTR	LCL_STR_SYSCOMMENU_GAME_END_STR = _T("结束游戏");									// ★：ゲームを終了します。
const TSTR	LCL_STR_SYSCOMMENU_CANCEL_STR = _T("取消");											// ★：システムコマンドメニューをキャンセルします。
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_SWITCH_STR(i)	(str_format(_T("本地通用开关 %d 号"), i))		// ★：ローカル汎用スイッチ：スイッチ名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_STR(i)		(str_format(_T("本地通用模式 %d 号"), i))		// ★：ローカル汎用モード：モード名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_STR(i)	(str_format(_T("模式 %d 号"), i))				// ★：ローカル汎用モード：アイテム名

// ****************************************************************
// ローカライズ：文字列：ダイアログ
// ================================================================

// ダイアログ共通
const TSTR	LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR = _T("决定");
const TSTR	LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR = _T("关闭");
const TSTR	LCL_STR_MODAL_DIALOG_INIT_BTN_STR = _T("恢复原始设置");

// セーブダイアログ
const TSTR	LCL_STR_SAVE_DIALOG_CAPTION_TITLE_STR = _T("存储");													// ★：セーブダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DECIDE_BUTTON_STR = _T("存储");													// ★：セーブダイアログの決定ボタンの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DATALIST_NAMEHEADER_STR = _T("存储");											// ★：セーブダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_SAVE_DIALOG_WARNING_CHECKBOX_STR = _T("存储前显示覆盖存档的确认窗口");						// ★：セーブダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DBLCLK_CHECKBOX_STR = _T("双击列表以存储");										// ★：セーブダイアログのダブルクリックチェックボックスの文字列

// ロードダイアログ
const TSTR	LCL_STR_LOAD_DIALOG_CAPTION_TITLE_STR = _T("读取");												// ★：ロードダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DECIDE_BUTTON_STR = _T("读取");												// ★：ロードダイアログの決定ボタンの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DATALIST_NAMEHEADER_STR = _T("读取");										// ★：ロードダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_LOAD_DIALOG_WARNING_CHECKBOX_STR = _T("读取前显示确认窗口");							// ★：ロードダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DBLCLK_CHECKBOX_STR = _T("双击以读取");										// ★：ロードダイアログのダブルクリックチェックボックスの文字列

// セーブダイアログ＆ロードダイアログの共通情報
const TSTR	LCL_STR_SAVELOAD_DIALOG_NO_DATA_STR = _T("无存档数据");

// ユーザコンフィグ：環境設定ダイアログ
const TSTR	LCL_STR_CONFIG_DIALOG_SCREEN_STR = _T("画面");
const TSTR	LCL_STR_CONFIG_DIALOG_VOLUME_STR = _T("音量");
const TSTR	LCL_STR_CONFIG_DIALOG_MESSAGE_STR = _T("文本");
const TSTR	LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR = _T("背景颜色");
const TSTR	LCL_STR_CONFIG_DIALOG_VOICE_STR = _T("声音");
const TSTR	LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR = _T("自动模式");
const TSTR	LCL_STR_CONFIG_DIALOG_JITAN_STR = _T("缩短播放时间");
const TSTR	LCL_STR_CONFIG_DIALOG_OTHERS_STR = _T("其他");
const TSTR	LCL_STR_CONFIG_DIALOG_SYSTEM_STR = _T("系统");

// ユーザコンフィグ：音量・ＢＧＭフェードダイアログ
const TSTR	LCL_STR_VOLUME_BGMFADE_DIALOG_DIALOG_TITLE = _T("音量设置");

// ユーザコンフィグ：音量ダイアログ
const TSTR	LCL_STR_VOLUME_DIALOG_DIALOG_TITLE = _T("音量设置");
const TSTR	LCL_STR_VOLUME_DIALOG_GROUP_TITLE = _T("音量");
const TSTR	LCL_STR_VOLUME_DIALOG_ALL_GROUP_TITLE = _T("整体音量");
const TSTR	LCL_STR_VOLUME_DIALOG_BGM_GROUP_TITLE = _T("BGM");
const TSTR	LCL_STR_VOLUME_DIALOG_KOE_GROUP_TITLE = _T("角色声音");
const TSTR	LCL_STR_VOLUME_DIALOG_PCM_GROUP_TITLE = _T("效果音");
const TSTR	LCL_STR_VOLUME_DIALOG_SE_GROUP_TITLE = _T("系统音");
const TSTR	LCL_STR_VOLUME_DIALOG_MOV_GROUP_TITLE = _T("视频声音");
const TSTR	LCL_STR_VOLUME_DIALOG_STC_PLAY = _T("播放");

// ユーザコンフィグ：ＢＧＭフェードダイアログ
const TSTR	LCL_STR_BGMFADE_DIALOG_DIALOG_TITLE = _T("BGM淡出设置");
const TSTR	LCL_STR_BGMFADE_DIALOG_GROUP_TITLE = _T("BGM淡出");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_EXPLAINE = _T("播放角色声音时调整BGM音量的功能");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_ENABLE = _T("使用");

// ユーザコンフィグ：画面ダイアログ
const TSTR	LCL_STR_SCREEN_DIALOG_CURRENT_RESOLUTION_STR = _T("（当前分辨率）");
const TSTR	LCL_STR_SCREEN_DIALOG_DEFAULT_RESOLUTION_STR = _T("（原分辨率）");

// ユーザコンフィグ：フォント・メッセージスピードダイアログ
const TSTR	LCL_STR_FONT_MSGSPD_DIALOG_DIALOG_TITLE = _T("文本设置");

// ユーザコンフィグ：フォントダイアログ
const TSTR	LCL_STR_FONT_DIALOG_DIALOG_TITLE = _T("字体设置");
const TSTR	LCL_STR_FONT_DIALOG_GROUP_TITLE = _T("字体");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_ONOFF = _T("显示比例字体");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_DESC = _T("※使用比例字体可能导致文本排版显示的异常。");
const TSTR	LCL_STR_FONT_DIALOG_STC_BOLD = _T("文字加粗");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_OFF = _T("无阴影");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_ON = _T("有阴影");
const TSTR	LCL_STR_FONT_DIALOG_STC_BORDER_ON = _T("显示文字描边");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON = _T("描边+阴影");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_SHORT = _T("“请不要将天堂…分开”");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_LONG = _T("“请不要将天堂…分开”");

// ユーザコンフィグ：メッセージスピードダイアログ
const TSTR	LCL_STR_MSGSPD_DIALOG_DIALOG_TITLE = _T("文字速度设置");
const TSTR	LCL_STR_MSGSPD_DIALOG_GROUP_TITLE = _T("文字速度");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_SLOW = _T("慢");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_FAST = _T("快");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_NOWAIT = _T("不等待");

// ユーザーコンフィグ：音声ダイアログ
const TSTR	LCL_STR_KOEMOD_CHRKOE_DIALOG_DIALOG_TITLE = _T("声音设定");

// ユーザーコンフィグ：音声モードダイアログ
const TSTR	LCL_STR_KOEMOD_DIALOG_DIALOG_TITLE = _T("声音模式设定");
const TSTR	LCL_STR_KOEMOD_DIALOG_GROUP_TITLE = _T("声音模式");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ON = _T("有声");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_OFF = _T("无声");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ONLY = _T("只限声音（无文字）");

// ユーザーコンフィグ：キャラクター音声ダイアログ
const TSTR	LCL_STR_CHRKOE_DIALOG_DIALOG_TITLE = _T("角色语音设定");
const TSTR	LCL_STR_CHRKOE_DIALOG_GROUP_TITLE = _T("角色语音");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_ON = _T("全部开启");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_OFF = _T("全部关闭");

// ユーザーコンフィグ：オートモードダイアログ
const TSTR	LCL_STR_DIALOG_AUTOMODE_DIALOG_TITLE = _T("自动模式（文本自动播放功能）设置");
const TSTR	LCL_STR_DIALOG_AUTOMODE_GROUP_TITLE = _T("自动模式（文本自动播放功能）");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_EXPLAINE = _T("等待时间=「（显示文字数量×文字时间）+最小时间」");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MOJI_TIME = _T("文字时间");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MIN_TIME = _T("最小时间");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_TEN_SECONDS = _T("显示每10个文字时的等待时间");
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_1(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%d"), n2) + _T(" 秒")
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_2(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%02d"), n2) + _T(" 秒")

// ユーザコンフィグ：システムダイアログ
const TSTR	LCL_STR_SYSTEM_DIALOG_SLEEP_STR = _T("延迟本游戏程序，以使其他程序流畅运行");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_WIPE_ANIME_STR = _T("停用画面变暗的动画效果");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_WIPE_ANIME_STR = _T("鼠标单击以跳过画面变暗效果");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_MWND_ANIME_STR = _T("停用打开与关闭对话框时的动画效果");
const TSTR	LCL_STR_SYSTEM_DIALOG_WHEEL_NEXT_MESSAGE_STR = _T("向下滑动鼠标滚轮以阅读文本");
const TSTR	LCL_STR_SYSTEM_DIALOG_KOE_DONT_STOP_STR = _T("阅读下一条文本时不停止声音的播放");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_UNREAD_MESSAGE_STR = _T("快进未读文本");
const TSTR	LCL_STR_SYSTEM_DIALOG_PLAY_SILENT_SOUND_STR = _T("声音播放时若存在杂音则检查设置");

// ****************************************************************
// ローカライズ：文字列：確認メッセージ
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RESTART_WARNING_STR = _T("要从头开始吗？");									// ★：ゲームを最初から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_SCENESTART_WARNING_STR = _T("要从中途开始吗？");								// ★：ゲームを途中から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_RETURNMENU_WARNING_STR = _T("返回标题画面吗？");								// ★：タイトルメニューに戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RETURNSEL_WARNING_STR = _T("返回上一个选项吗？");							// ★：前の選択肢に戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_GAMEEND_WARNING_STR = _T("结束游戏吗？");									// ★：ゲームを終了する時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_SAVE_WARNING_STR = _T("保存进度吗？");										// ★：セーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_SAVE_WARNING_STR = _T("快速保存进度吗？");								// ★：クイックセーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_LOAD_WARNING_STR = _T("读取进度吗？");										// ★：ロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_LOAD_WARNING_STR = _T("快速读取进度吗？");								// ★：クイックロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_MSGBK_LOAD_WARNING_STR = _T("读取进度吗？");									// ★：メッセージバックロード確認の確認文章の文字列

// ****************************************************************
// ローカライズ：文字列：解像度変更
// ================================================================

#define		LCL_STR_RESOLUTION_EXPLAIN_STR(cx, cy)\
str_format(\
_T("本作品的最佳匹配分辨率为 %d × %d。\n") \
_T("\n") \
_T("如果可以设置 %d × %d 分辨率，请以此进行设置。\n") \
_T("如果无法设置，请尽量选择最接近“最佳匹配分辨率的”画面比例。\n") \
_T("\n") \
_T("<有关液晶显示器的补充说明>\n") \
_T("即使设置成“最佳匹配分辨率”，也可能出现显示模糊或者粗糙的情况。\n") \
_T("即使设置成“最佳匹配分辨率”，也可能出现横向或者纵向拉长的情况。\n") \
_T("这属于显示器性能的问题，请予以谅解。\n") \
, cx, cy, cx, cy)

#define		LCL_STR_CHANGE_RESOLUTION_WARNING_STR	\
_T("将更改画面分辨率。\n") \
_T("\n") \
_T("<注意>\n") \
_T("因分辨率不同，可能会出现画面闪烁的状况。\n") \
_T("更改分辨率后，如果画面仍未正常显示，请耐心等待。\n") \
_T("20 秒后将会恢复到原来的设置状态。\n") \
_T("\n") \
_T("<有关液晶显示器的说明>\n") \
_T("更改分辨率时，可能会出现黑屏现象。\n") \
_T("这种情况是由于显示器无法正常收到信号，切换到无信号状态后导致的。\n") \
_T("重启显示器电源后即可正常显示。")

#define		LCL_STR_CHANGE_RESOLUTION_AFTER_STR(cnt)\
str_format( \
_T("分辨率已更改。\n") \
_T("%d 秒后将恢复原本的分辨率。") \
, cnt)

// ****************************************************************
// ローカライズ：文字列：エラーメッセージ
// ================================================================

// フォントエラー
#define		LCL_STR_NOT_EXIST_FONT(font)\
_T("未发现字体。（或字体已损坏。）") \
_T("\n\n　　现在的字体：") + font + \
_T("\n\n要更改字体吗？") \
_T("\n\n※若选择“否”，将不会显示文字。")

// ****************************************************************
// ローカライズ：文字列：サポート情報
// ================================================================

// 各種確認メッセージの文字列
const TSTR	LCL_STR_GENERAL_SUPPORT_STR = _T("\n\n[ 帮助信息 ]\n按下 Ctrl+C 可复制该信息。");

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
const int	LCL_IDD_CFG_WIN_BASE_COMMON = IDD_CFG_WIN_BASE_COMMON_ZH;
const int	LCL_IDD_CFG_WIN_SOLO_COMMON = IDD_CFG_WIN_SOLO_COMMON_ZH;

// （旧）個別ダイアログ
const int	LCL_IDD_CFG_WIN_BASE = IDD_CFG_WIN_BASE_ZH;
//const int	LCL_IDD_CFG_WIN_BASE_AUTOMODE = IDD_CFG_WIN_BASE_AUTOMODE_ZH;
//const int	LCL_IDD_CFG_WIN_BASE_CHRKOE = IDD_CFG_WIN_BASE_CHRKOE_ZH;
const int	LCL_IDD_CFG_WIN_BASE_ELSE = IDD_CFG_WIN_BASE_ELSE_ZH;
//const int	LCL_IDD_CFG_WIN_BASE_FONT_MSGSPD = IDD_CFG_WIN_BASE_FONT_MSGSPD_ZH;
const int	LCL_IDD_CFG_WIN_BASE_JITAN = IDD_CFG_WIN_BASE_JITAN_ZH;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE = IDD_CFG_WIN_BASE_KOEMODE_ZH;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE_CHRKOE = IDD_CFG_WIN_BASE_KOEMODE_CHRKOE_ZH;
const int	LCL_IDD_CFG_WIN_BASE_MWNDBK = IDD_CFG_WIN_BASE_MWNDBK_ZH;
const int	LCL_IDD_CFG_WIN_BASE_SCREEN = IDD_CFG_WIN_BASE_SCREEN_ZH;
const int	LCL_IDD_CFG_WIN_BASE_SYSTEM = IDD_CFG_WIN_BASE_SYSTEM_ZH;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME = IDD_CFG_WIN_BASE_VOLUME_ZH;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME_BGMFADE = IDD_CFG_WIN_BASE_VOLUME_BGMFADE_ZH;
//const int	LCL_IDD_CFG_WIN_SOLO_AUTOMODE = IDD_CFG_WIN_SOLO_AUTOMODE_ZH;
//const int	LCL_IDD_CFG_WIN_SOLO_BGMFADE = IDD_CFG_WIN_SOLO_BGMFADE_ZH;
//const int	LCL_IDD_CFG_WIN_SOLO_CHRKOE = IDD_CFG_WIN_SOLO_CHRKOE_ZH;
const int	LCL_IDD_CFG_WIN_SOLO_ELSE = IDD_CFG_WIN_SOLO_ELSE;								// ★翻訳なし
//const int	LCL_IDD_CFG_WIN_SOLO_FONT = IDD_CFG_WIN_SOLO_FONT_ZH;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT_MSGSPD = IDD_CFG_WIN_SOLO_FONT_MSGSPD_ZH;
const int	LCL_IDD_CFG_WIN_SOLO_JITAN = IDD_CFG_WIN_SOLO_JITAN_ZH;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE = IDD_CFG_WIN_SOLO_KOEMODE_ZH;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE = IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE_ZH;
//const int	LCL_IDD_CFG_WIN_SOLO_MSGSPD = IDD_CFG_WIN_SOLO_MSGSPD_ZH;
const int	LCL_IDD_CFG_WIN_SOLO_MOVTYPE = IDD_CFG_WIN_SOLO_MOVTYPE_ZH;
const int	LCL_IDD_CFG_WIN_SOLO_MWNDBK = IDD_CFG_WIN_SOLO_MWNDBK_ZH;
const int	LCL_IDD_CFG_WIN_SOLO_SCREEN = IDD_CFG_WIN_SOLO_SCREEN_ZH;
const int	LCL_IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE = IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE;		// ★翻訳なし
const int	LCL_IDD_CFG_WIN_SOLO_SYSTEM = IDD_CFG_WIN_SOLO_SYSTEM_ZH;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME = IDD_CFG_WIN_SOLO_VOLUME_ZH;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME_BGMFADE = IDD_CFG_WIN_SOLO_VOLUME_BGMFADE_ZH;
const int	LCL_IDD_SYS_WIN_SOLO_SAVELOAD = IDD_SYS_WIN_SOLO_SAVELOAD;						// ★翻訳なし
const int	LCL_IDD_CHANGE_RESOLUTION_WARNING = IDD_CHANGE_RESOLUTION_WARNING_ZH;
const int	LCL_IDD_TWEET = IDD_TWEET;
