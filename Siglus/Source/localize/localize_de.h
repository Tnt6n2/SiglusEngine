#pragma		once

// ****************************************************************
// ローカライズ：言語コード
// ================================================================

#define		LCL_LANGUAGE_CODE	_T("de")

// ****************************************************************
// ローカライズ：Gameexe.ini
// ================================================================

#define		LCL_GAMEEXE_DAT		_T("GameexeDE.dat")

// ****************************************************************
// ローカライズ：Scene.pck
// ================================================================

#define		LCL_SCENE_PCK		_T("SceneDE.pck")

// ****************************************************************
// ローカライズ：セーブフォルダ
// ================================================================

#define		LCL_SAVEDATA_DIR	_T("savedata_de")

// ****************************************************************
// ローカライズ：ユーザーフォルダ
// ================================================================

#define		LCL_USERDATA_DIR	_T("userdata")

// ****************************************************************
// ローカライズ：日付表記
// ================================================================

extern TCHAR* MONTH_STR_EN[];

#define		LCL_DATE_FORMAT		str_format(_T("%d.%d.%d %02d:%02d:%02d"), p_header->day, p_header->month, p_header->year, p_header->hour, p_header->minute, p_header->second);

// ****************************************************************
// ローカライズ：文字列：システムコマンドメニュー
// ================================================================

// システムコマンドメニュー：文字列
const TSTR	LCL_STR_SYSCOMMENU_READ_SKIP_STR = _T("Bereits gelesenen Text überspringen");		// ★：一度読んだ文章の早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_UNREAD_SKIP_STR = _T("Vorspulen");								// ★：未読の文章でも早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_AKIP_STR = _T("Automatisch vorspulen");						// ★：自動早送り機能をＯＮ／ＯＦＦします。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_MODE_STR = _T("Automodus");									// ★：オートモードを開始します。
const TSTR	LCL_STR_SYSCOMMENU_HIDE_MWND_STR = _T("Textfenster verstecken");					// ★：メッセージウィンドウを隠します。
const TSTR	LCL_STR_SYSCOMMENU_MSG_BACK_STR = _T("Textlog öffnen");								// ★：メッセージバックを開きます。
const TSTR	LCL_STR_SYSCOMMENU_SAVE_STR = _T("Spielstand speichern");										// ★：セーブを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_LOAD_STR = _T("Spielstand laden");											// ★：ロードを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_SEL_STR = _T("Zur vorigen Auswahl zurückkehren");			// ★：前の選択肢に戻ります。
const TSTR	LCL_STR_SYSCOMMENU_CONFIG_STR = _T("Einstellungsmenü");								// ★：環境設定を呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_MANUAL_STR = _T("Spielanleitung öffnen");						// ★：マニュアルを表示します。
const TSTR	LCL_STR_SYSCOMMENU_VERSION_STR = _T("Versionsinfo");								// ★：バージョン情報を表示します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_MENU_STR = _T("Zum Titel zurückkehren");					// ★：タイトルメニューに戻ります。
const TSTR	LCL_STR_SYSCOMMENU_GAME_END_STR = _T("Spiel beenden");								// ★：ゲームを終了します。
const TSTR	LCL_STR_SYSCOMMENU_CANCEL_STR = _T("Abbrechen");									// ★：システムコマンドメニューをキャンセルします。
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_SWITCH_STR(i)	(str_format(_T("Universeller Lokalschalter Nr.%d"), i))		// ★：ローカル汎用スイッチ：スイッチ名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_STR(i)		(str_format(_T("Universeller Lokalmodus Nr.%d"), i))		// ★：ローカル汎用モード：モード名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_STR(i)	(str_format(_T("Modus Nr.%d"), i))							// ★：ローカル汎用モード：アイテム名

// ****************************************************************
// ローカライズ：文字列：確認メッセージ
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RESTART_WARNING_STR = _T("Vom Anfang starten?");							// ★：ゲームを最初から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_SCENESTART_WARNING_STR = _T("Vom letzten Spielstand starten?");			// ★：ゲームを途中から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_RETURNMENU_WARNING_STR = _T("Zum Titel zurückkehren?");					// ★：タイトルメニューに戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RETURNSEL_WARNING_STR = _T("Zur vorigen Auswahl zurückkehren?");			// ★：前の選択肢に戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_GAMEEND_WARNING_STR = _T("Wirklich beenden?");							// ★：ゲームを終了する時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_SAVE_WARNING_STR = _T("Wirklich speichern?");							// ★：セーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_SAVE_WARNING_STR = _T("Wirklich schnellspeichern?");				// ★：クイックセーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_LOAD_WARNING_STR = _T("Wirklich laden?");								// ★：ロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_LOAD_WARNING_STR = _T("Wirklich schnellladen?");					// ★：クイックロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_MSGBK_LOAD_WARNING_STR = _T("Wirklich zurückspringen?");					// ★：メッセージバックロード確認の確認文章の文字列

// ****************************************************************
// ローカライズ：文字列：ダイアログ
// ================================================================

// ダイアログ共通
const TSTR	LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR = _T("Bestätigen");
const TSTR	LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR = _T("Schließen");
const TSTR	LCL_STR_MODAL_DIALOG_INIT_BTN_STR = _T("Zurücksetzen");

// セーブダイアログ
const TSTR	LCL_STR_SAVE_DIALOG_CAPTION_TITLE_STR = _T("Speichern");											// ★：セーブダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DECIDE_BUTTON_STR = _T("Speichern");											// ★：セーブダイアログの決定ボタンの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DATALIST_NAMEHEADER_STR = _T("Speichern");										// ★：セーブダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_SAVE_DIALOG_WARNING_CHECKBOX_STR = _T("Vor dem Überschreiben eines Spielstandes warnen");	// ★：セーブダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DBLCLK_CHECKBOX_STR = _T("Per Doppelklick speichern");							// ★：セーブダイアログのダブルクリックチェックボックスの文字列

// ロードダイアログ
const TSTR	LCL_STR_LOAD_DIALOG_CAPTION_TITLE_STR = _T("Laden");											// ★：ロードダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DECIDE_BUTTON_STR = _T("Laden");											// ★：ロードダイアログの決定ボタンの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DATALIST_NAMEHEADER_STR = _T("Laden");										// ★：ロードダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_LOAD_DIALOG_WARNING_CHECKBOX_STR = _T("Vor dem Laden eines Spielstandes warnen");		// ★：ロードダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DBLCLK_CHECKBOX_STR = _T("Per Doppelklick speichern");						// ★：ロードダイアログのダブルクリックチェックボックスの文字列

// セーブダイアログ＆ロードダイアログの共通情報
const TSTR	LCL_STR_SAVELOAD_DIALOG_NO_DATA_STR = _T("Keine Daten vorhanden");

// ユーザコンフィグ：環境設定ダイアログ
const TSTR	LCL_STR_CONFIG_DIALOG_SCREEN_STR = _T("Fenster");
const TSTR	LCL_STR_CONFIG_DIALOG_VOLUME_STR = _T("Lautstärke");
const TSTR	LCL_STR_CONFIG_DIALOG_MESSAGE_STR = _T("Text");
const TSTR	LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR = _T("Hintergrundfarbe");
const TSTR	LCL_STR_CONFIG_DIALOG_VOICE_STR = _T("Stimmen");
const TSTR	LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR = _T("Automodus");
const TSTR	LCL_STR_CONFIG_DIALOG_JITAN_STR = _T("Beschleunigte Wiedergabe");
const TSTR	LCL_STR_CONFIG_DIALOG_OTHERS_STR = _T("Andere Benachrichtigungen");
const TSTR	LCL_STR_CONFIG_DIALOG_SYSTEM_STR = _T("System");

// ユーザコンフィグ：音量・ＢＧＭフェードダイアログ
const TSTR	LCL_STR_VOLUME_BGMFADE_DIALOG_DIALOG_TITLE = _T("Lautstärke");

// ユーザコンフィグ：音量ダイアログ
const TSTR	LCL_STR_VOLUME_DIALOG_DIALOG_TITLE = _T("Lautstärke");
const TSTR	LCL_STR_VOLUME_DIALOG_GROUP_TITLE = _T("Lautstärke");
const TSTR	LCL_STR_VOLUME_DIALOG_ALL_GROUP_TITLE = _T("Master");
const TSTR	LCL_STR_VOLUME_DIALOG_BGM_GROUP_TITLE = _T("Musik");
const TSTR	LCL_STR_VOLUME_DIALOG_KOE_GROUP_TITLE = _T("Stimmen");
const TSTR	LCL_STR_VOLUME_DIALOG_PCM_GROUP_TITLE = _T("Effekte");
const TSTR	LCL_STR_VOLUME_DIALOG_SE_GROUP_TITLE = _T("Signaltöne");
const TSTR	LCL_STR_VOLUME_DIALOG_MOV_GROUP_TITLE = _T("Videos");
const TSTR	LCL_STR_VOLUME_DIALOG_STC_PLAY = _T("An");

// ユーザコンフィグ：ＢＧＭフェードダイアログ
const TSTR	LCL_STR_BGMFADE_DIALOG_DIALOG_TITLE = _T("Musikausblendung");
const TSTR	LCL_STR_BGMFADE_DIALOG_GROUP_TITLE = _T("Musikausblendung");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_EXPLAINE = _T("Musiklautstärke während Stimmwiedergabe");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_ENABLE = _T("An");

// ユーザコンフィグ：画面ダイアログ
const TSTR	LCL_STR_SCREEN_DIALOG_CURRENT_RESOLUTION_STR = _T("(Momentane Auflösung)");
const TSTR	LCL_STR_SCREEN_DIALOG_DEFAULT_RESOLUTION_STR = _T("(Vorherige Auflösung)");

// ユーザコンフィグ：フォント・メッセージスピードダイアログ
const TSTR	LCL_STR_FONT_MSGSPD_DIALOG_DIALOG_TITLE = _T("Texteinstellungen");

// ユーザコンフィグ：フォントダイアログ
const TSTR	LCL_STR_FONT_DIALOG_DIALOG_TITLE = _T("Schriftart");
const TSTR	LCL_STR_FONT_DIALOG_GROUP_TITLE = _T("Schriftart");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_ONOFF = _T("Proportionale Schriftart darstellen");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_DESC = _T("* Durch proportionale Schriftart kann das Textlayout zusammenbrechen.");
const TSTR	LCL_STR_FONT_DIALOG_STC_BOLD = _T("Schriftzeichen fett darstellen");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_OFF = _T("Keine Schatten");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_ON = _T("Mit Schatten");
const TSTR	LCL_STR_FONT_DIALOG_STC_BORDER_ON = _T("Textrand entfernen");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON = _T("Textrand + Schatten");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_SHORT = _T("Beispieltext");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_LONG = _T("Dies ist ein Beispieltext.");

// ユーザコンフィグ：メッセージスピードダイアログ
const TSTR	LCL_STR_MSGSPD_DIALOG_DIALOG_TITLE = _T("Textgeschwindigkeit");
const TSTR	LCL_STR_MSGSPD_DIALOG_GROUP_TITLE = _T("Textgeschwindigkeit");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_SLOW = _T("Langsam");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_FAST = _T("Schnell");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_NOWAIT = _T("Sofort");

// ユーザーコンフィグ：音声ダイアログ
const TSTR	LCL_STR_KOEMOD_CHRKOE_DIALOG_DIALOG_TITLE = _T("Stimmeneinstellungen");

// ユーザーコンフィグ：音声モードダイアログ
const TSTR	LCL_STR_KOEMOD_DIALOG_DIALOG_TITLE = _T("Stimmenmodus");
const TSTR	LCL_STR_KOEMOD_DIALOG_GROUP_TITLE = _T("Stimmenmodus");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ON = _T("Mit Stimmen");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_OFF = _T("Ohne Stimmen");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ONLY = _T("Nur Stimmen(Kein Text)");

// ユーザーコンフィグ：キャラクター音声ダイアログ
const TSTR	LCL_STR_CHRKOE_DIALOG_DIALOG_TITLE = _T("Charakterstimmen");
const TSTR	LCL_STR_CHRKOE_DIALOG_GROUP_TITLE = _T("Charakterstimmen");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_ON = _T("Alle an");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_OFF = _T("Alle aus");

// ユーザーコンフィグ：オートモードダイアログ
const TSTR	LCL_STR_DIALOG_AUTOMODE_DIALOG_TITLE = _T("Automodus （Automatisches Fortfahren des Texts) ");
const TSTR	LCL_STR_DIALOG_AUTOMODE_GROUP_TITLE = _T("Automodus （Automatisches Fortfahren des Texts) ");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_EXPLAINE = _T("Wartezeit entspricht (Anzahl der dargestellten Zeichen × Sekunden pro Zeichen) + Minimalintervall");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MOJI_TIME = _T("Sekunden pro Zeichen");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MIN_TIME = _T("Minimalintervall");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_TEN_SECONDS = _T("Wartezeit für 10 Zeichen");
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_1(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%d"), n2) + _T(" Sekunden")
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_2(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%02d"), n2) + _T(" Sekunden")

// ユーザコンフィグ：システムダイアログ
const TSTR	LCL_STR_SYSTEM_DIALOG_SLEEP_STR = _T("Geschwindigkeit des Programms verlangsamen um CPU-Auslastung zu senken.");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_WIPE_ANIME_STR = _T("Deaktivieren der Bildschirmdimmung.");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_WIPE_ANIME_STR = _T("Bildschirmdimmung per Mausklick überspringen.");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_MWND_ANIME_STR = _T("Animation des Textfensters deaktivieren.");
const TSTR	LCL_STR_SYSTEM_DIALOG_WHEEL_NEXT_MESSAGE_STR = _T("Durch Runterscrollen des Mausrads den Text fortsetzen.");
const TSTR	LCL_STR_SYSTEM_DIALOG_KOE_DONT_STOP_STR = _T("Stimmenwiedergabe wird bei Überspringen des Textes nicht unterbrochen.");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_UNREAD_MESSAGE_STR = _T("Noch nicht gelesene Textpassagen können auch übersprungen werden.");
const TSTR	LCL_STR_SYSTEM_DIALOG_PLAY_SILENT_SOUND_STR = _T("Bei Verzerrungen in der Tonwiedergabe bitte hier aktivieren.");

// ****************************************************************
// ローカライズ：解像度変更
// ================================================================

#define		LCL_STR_RESOLUTION_EXPLAIN_STR(cx, cy)\
str_format(\
_T("Die optimale Auflösung ist \"%d X %d\".\n") \
_T("\n") \
_T("Falls \"%d X %d\" einstellbar ist wāhlen Sie diese bitte aus.\n") \
_T("Fall diese Auflösung nicht auswählbar ist stellen Sie eine Auflösung mit ähnlichem Seitenverhältnis ein.\n") \
_T("\n") \
_T("[ Bei Nutzung eines LCD Bildschirms ]\n") \
_T("Selbst bei optimaler Auflösung kann es zu verschwommener oder unscharfer Darstellung kommen.\n") \
_T(" Ebenso eine vertikale wie horizontale Dehnung.\n") \
_T(" Dies hat seine Ursache am verwendeten Bildschirm.") \
, cx, cy, cx, cy)

#define		LCL_STR_CHANGE_RESOLUTION_WARNING_STR	\
_T("Auflösung wird geändert.\n") \
_T("\n") \
_T("≪Hinweis≫\n") \
_T("Je nach Auflösung kann es zu Bildschirmflimmern kommen.\n") \
_T("Falls die gewählte Auflösung nicht funktioniert, wird nach 20 Sekunden automatisch die vorherige Auflösung eingestellt.\n") \
_T("\n") \
_T("[ Über LCD Monitore ]\n") \
_T("Nach dem ändern der Auflösung kann der Bildschirm schwarz bleiben.\n") \
_T("Ursache dafür ist dass der Monitor die gewählte Auflösung nicht unterstützt.\n") \
_T("Ein - und Ausschalten des Monitors hilft in den meisten Fällen.")\

#define		LCL_STR_CHANGE_RESOLUTION_AFTER_STR(cnt)\
str_format( \
_T("Auflösung wurde geändert.\n") \
_T("In xxx Sekunden wird auf die vorherige Auflösung zurückgesetzt.") \
, cnt)

// ****************************************************************
// ローカライズ：エラーメッセージ
// ================================================================

// フォントエラー
#define		LCL_STR_NOT_EXIST_FONT(font)\
_T("Schriftart kann nicht gefunden werden (oder ist beschädigt).") \
_T("\n\n　　Momentane Schriftart: ") + font + \
_T("\n\nSchriftart ändern?") \
_T("\n\n* [ Nein ] Bei der Auswahl \"Nein\" wird gar kein Text angezeigt.")

// ****************************************************************
// ローカライズ：文字列：サポート情報
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_GENERAL_SUPPORT_STR = _T("\n\n[ Support Info ]\nDiese Benachrichtigung kann mit Ctrl+C kopiert werden.");

// ****************************************************************
// ローカライズ：文字列：Twitter
// ================================================================

const TSTR	LCL_STR_TWITTER_DIALOG_TWITTER = _T("Twitter");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_AUTH = _T("Verifizierung");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_POST = _T("Auf Twitter hochladen");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_NEED = _T("Vor dem Hochladen auf Twitter ist eine Kontoverifizierung notwendig.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_SUCCESS = _T("Verifizierung erfolgreich.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_POST_SUCCESS = _T("Hochladen erfolgreich.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_SAMPLE = _T("Von Siglus hochgeladen.");

// ****************************************************************
// ローカライズ：ダイアログリソース
// ================================================================

// 共通ダイアログ
const int	LCL_IDD_CFG_WIN_BASE_COMMON = IDD_CFG_WIN_BASE_COMMON_DE;
const int	LCL_IDD_CFG_WIN_SOLO_COMMON = IDD_CFG_WIN_SOLO_COMMON_DE;

// （旧）個別ダイアログ
const int	LCL_IDD_CFG_WIN_BASE = IDD_CFG_WIN_BASE_DE;
//const int	LCL_IDD_CFG_WIN_BASE_AUTOMODE = IDD_CFG_WIN_BASE_AUTOMODE_DE;
//const int	LCL_IDD_CFG_WIN_BASE_CHRKOE = IDD_CFG_WIN_BASE_CHRKOE_DE;
const int	LCL_IDD_CFG_WIN_BASE_ELSE = IDD_CFG_WIN_BASE_ELSE_DE;
//const int	LCL_IDD_CFG_WIN_BASE_FONT_MSGSPD = IDD_CFG_WIN_BASE_FONT_MSGSPD_DE;
const int	LCL_IDD_CFG_WIN_BASE_JITAN = IDD_CFG_WIN_BASE_JITAN_DE;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE = IDD_CFG_WIN_BASE_KOEMODE_DE;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE_CHRKOE = IDD_CFG_WIN_BASE_KOEMODE_CHRKOE_DE;
const int	LCL_IDD_CFG_WIN_BASE_MWNDBK = IDD_CFG_WIN_BASE_MWNDBK_DE;
const int	LCL_IDD_CFG_WIN_BASE_SCREEN = IDD_CFG_WIN_BASE_SCREEN_DE;
const int	LCL_IDD_CFG_WIN_BASE_SYSTEM = IDD_CFG_WIN_BASE_SYSTEM_DE;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME = IDD_CFG_WIN_BASE_VOLUME_DE;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME_BGMFADE = IDD_CFG_WIN_BASE_VOLUME_BGMFADE_DE;
//const int	LCL_IDD_CFG_WIN_SOLO_AUTOMODE = IDD_CFG_WIN_SOLO_AUTOMODE_DE;
//const int	LCL_IDD_CFG_WIN_SOLO_BGMFADE = IDD_CFG_WIN_SOLO_BGMFADE_DE;
//const int	LCL_IDD_CFG_WIN_SOLO_CHRKOE = IDD_CFG_WIN_SOLO_CHRKOE_DE;
const int	LCL_IDD_CFG_WIN_SOLO_ELSE = IDD_CFG_WIN_SOLO_ELSE_DE;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT = IDD_CFG_WIN_SOLO_FONT_DE;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT_MSGSPD = IDD_CFG_WIN_SOLO_FONT_MSGSPD_DE;
const int	LCL_IDD_CFG_WIN_SOLO_JITAN = IDD_CFG_WIN_SOLO_JITAN_DE;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE = IDD_CFG_WIN_SOLO_KOEMODE_DE;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE = IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE_DE;
//const int	LCL_IDD_CFG_WIN_SOLO_MSGSPD = IDD_CFG_WIN_SOLO_MSGSPD_DE;
const int	LCL_IDD_CFG_WIN_SOLO_MOVTYPE = IDD_CFG_WIN_SOLO_MOVTYPE_DE;
const int	LCL_IDD_CFG_WIN_SOLO_MWNDBK = IDD_CFG_WIN_SOLO_MWNDBK_DE;
const int	LCL_IDD_CFG_WIN_SOLO_SCREEN = IDD_CFG_WIN_SOLO_SCREEN_DE;
const int	LCL_IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE = IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE;
const int	LCL_IDD_CFG_WIN_SOLO_SYSTEM = IDD_CFG_WIN_SOLO_SYSTEM_DE;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME = IDD_CFG_WIN_SOLO_VOLUME_DE;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME_BGMFADE = IDD_CFG_WIN_SOLO_VOLUME_BGMFADE_DE;
const int	LCL_IDD_SYS_WIN_SOLO_SAVELOAD = IDD_SYS_WIN_SOLO_SAVELOAD_DE;
const int	LCL_IDD_CHANGE_RESOLUTION_WARNING = IDD_CHANGE_RESOLUTION_WARNING_DE;
const int	LCL_IDD_TWEET = IDD_TWEET_DE;
