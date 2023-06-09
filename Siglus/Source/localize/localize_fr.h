#pragma		once

// ****************************************************************
// ローカライズ：言語コード
// ================================================================

#define		LCL_LANGUAGE_CODE	_T("fr")

// ****************************************************************
// ローカライズ：Gameexe.ini
// ================================================================

#define		LCL_GAMEEXE_DAT		_T("GameexeFR.dat")

// ****************************************************************
// ローカライズ：Scene.pck
// ================================================================

#define		LCL_SCENE_PCK		_T("SceneFR.pck")

// ****************************************************************
// ローカライズ：セーブフォルダ
// ================================================================

#define		LCL_SAVEDATA_DIR	_T("savedata_fr")

// ****************************************************************
// ローカライズ：ユーザーフォルダ
// ================================================================

#define		LCL_USERDATA_DIR	_T("userdata")

// ****************************************************************
// ローカライズ：日付表記
// ================================================================

extern TCHAR* MONTH_STR_FR[];

#define		LCL_DATE_FORMAT		str_format(_T("%d %s %d à %02d:%02d:%02d"), p_header->day, MONTH_STR_FR[p_header->month - 1], p_header->year, p_header->hour, p_header->minute, p_header->second);

// ****************************************************************
// ローカライズ：文字列：システムコマンドメニュー
// ================================================================

// システムコマンドメニュー：文字列
const TSTR	LCL_STR_SYSCOMMENU_READ_SKIP_STR = _T("Passer le texte déjà lu");				// ★：一度読んだ文章の早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_UNREAD_SKIP_STR = _T("Passer le texte");						// ★：未読の文章でも早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_AKIP_STR = _T("Lecture automatique");					// ★：自動早送り機能をＯＮ／ＯＦＦします。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_MODE_STR = _T("Mode automatique");						// ★：オートモードを開始します。
const TSTR	LCL_STR_SYSCOMMENU_HIDE_MWND_STR = _T("Cacher les messages");					// ★：メッセージウィンドウを隠します。
const TSTR	LCL_STR_SYSCOMMENU_MSG_BACK_STR = _T("Ouvrir le journal de messages");			// ★：メッセージバックを開きます。
const TSTR	LCL_STR_SYSCOMMENU_SAVE_STR = _T("Sauvegarder");								// ★：セーブを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_LOAD_STR = _T("Charger");									// ★：ロードを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_SEL_STR = _T("Revenir au choix précédent");			// ★：前の選択肢に戻ります。
const TSTR	LCL_STR_SYSCOMMENU_CONFIG_STR = _T("Réglages d'environnement");					// ★：環境設定を呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_MANUAL_STR = _T("Ouvrir le manuel");							// ★：マニュアルを表示します。
const TSTR	LCL_STR_SYSCOMMENU_VERSION_STR = _T("Informations de version");					// ★：バージョン情報を表示します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_MENU_STR = _T("Revenir à l'écran-titre");				// ★：タイトルメニューに戻ります。
const TSTR	LCL_STR_SYSCOMMENU_GAME_END_STR = _T("Fermer le jeu");							// ★：ゲームを終了します。
const TSTR	LCL_STR_SYSCOMMENU_CANCEL_STR = _T("Annuler");									// ★：システムコマンドメニューをキャンセルします。
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_SWITCH_STR(i)	(str_format(_T("Bascule générique locale %d"), i))	// ★：ローカル汎用スイッチ：スイッチ名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_STR(i)		(str_format(_T("Mode générique local %d"), i))		// ★：ローカル汎用モード：モード名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_STR(i)	(str_format(_T("Mode %d"), i))						// ★：ローカル汎用モード：アイテム名

// ****************************************************************
// ローカライズ：文字列：ダイアログ
// ================================================================

// ダイアログ共通
const TSTR	LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR = _T("Valider");
const TSTR	LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR = _T("Fermer");
const TSTR	LCL_STR_MODAL_DIALOG_INIT_BTN_STR = _T("Réinitialiser");

// セーブダイアログ
const TSTR	LCL_STR_SAVE_DIALOG_CAPTION_TITLE_STR = _T("Sauvegarder");													// ★：セーブダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DECIDE_BUTTON_STR = _T("Sauvegarder");													// ★：セーブダイアログの決定ボタンの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DATALIST_NAMEHEADER_STR = _T("Sauvegarder");											// ★：セーブダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_SAVE_DIALOG_WARNING_CHECKBOX_STR = _T("Afficher la fenêtre de confirmation avant de sauvegarder");	// ★：セーブダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DBLCLK_CHECKBOX_STR = _T("Sauvegarder en double - cliquant sur la liste");				// ★：セーブダイアログのダブルクリックチェックボックスの文字列

// ロードダイアログ
const TSTR	LCL_STR_LOAD_DIALOG_CAPTION_TITLE_STR = _T("Charger");														// ★：ロードダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DECIDE_BUTTON_STR = _T("Charger");														// ★：ロードダイアログの決定ボタンの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DATALIST_NAMEHEADER_STR = _T("Charger");												// ★：ロードダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_LOAD_DIALOG_WARNING_CHECKBOX_STR = _T("Afficher la fenêtre de confirmation avant de charger");		// ★：ロードダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DBLCLK_CHECKBOX_STR = _T("Charger en double - cliquant sur la liste");					// ★：ロードダイアログのダブルクリックチェックボックスの文字列

// セーブダイアログ＆ロードダイアログの共通情報
const TSTR	LCL_STR_SAVELOAD_DIALOG_NO_DATA_STR = _T("Pas de données");

// ユーザコンフィグ：環境設定ダイアログ
const TSTR	LCL_STR_CONFIG_DIALOG_SCREEN_STR = _T("Écran");
const TSTR	LCL_STR_CONFIG_DIALOG_VOLUME_STR = _T("Volume sonore");
const TSTR	LCL_STR_CONFIG_DIALOG_MESSAGE_STR = _T("Texte");
const TSTR	LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR = _T("Couleur du fond");
const TSTR	LCL_STR_CONFIG_DIALOG_VOICE_STR = _T("Voix");
const TSTR	LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR = _T("Mode automatique");
const TSTR	LCL_STR_CONFIG_DIALOG_JITAN_STR = _T("Jouer en accéléré");
const TSTR	LCL_STR_CONFIG_DIALOG_OTHERS_STR = _T("Autres");
const TSTR	LCL_STR_CONFIG_DIALOG_SYSTEM_STR = _T("Système");

// ユーザコンフィグ：画面ダイアログ
const TSTR	LCL_STR_SCREEN_DIALOG_CURRENT_RESOLUTION_STR = _T("(Résolution actuelle)");
const TSTR	LCL_STR_SCREEN_DIALOG_DEFAULT_RESOLUTION_STR = _T("(Résolution initiale)");

// ユーザコンフィグ：音量・ＢＧＭフェードダイアログ
const TSTR	LCL_STR_VOLUME_BGMFADE_DIALOG_DIALOG_TITLE = _T("Réglage du volume sonore");

// ユーザコンフィグ：音量ダイアログ
const TSTR	LCL_STR_VOLUME_DIALOG_DIALOG_TITLE = _T("Réglage du volume sonore");
const TSTR	LCL_STR_VOLUME_DIALOG_GROUP_TITLE = _T("Volume");
const TSTR	LCL_STR_VOLUME_DIALOG_ALL_GROUP_TITLE = _T("Global");
const TSTR	LCL_STR_VOLUME_DIALOG_BGM_GROUP_TITLE = _T("Musiques");
const TSTR	LCL_STR_VOLUME_DIALOG_KOE_GROUP_TITLE = _T("Voix");
const TSTR	LCL_STR_VOLUME_DIALOG_PCM_GROUP_TITLE = _T("Effets");
const TSTR	LCL_STR_VOLUME_DIALOG_SE_GROUP_TITLE = _T("Système");
const TSTR	LCL_STR_VOLUME_DIALOG_MOV_GROUP_TITLE = _T("Vidéo");
const TSTR	LCL_STR_VOLUME_DIALOG_STC_PLAY = _T("Jouer");

// ユーザコンフィグ：ＢＧＭフェードダイアログ
const TSTR	LCL_STR_BGMFADE_DIALOG_DIALOG_TITLE = _T("Réglage du gain automatique");
const TSTR	LCL_STR_BGMFADE_DIALOG_GROUP_TITLE = _T("Gain automatique");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_EXPLAINE = _T("Cette ajuste Le volume des musiques lorsque des parlent.");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_ENABLE = _T("Utiliser");

// ユーザコンフィグ：フォント・メッセージスピードダイアログ
const TSTR	LCL_STR_FONT_MSGSPD_DIALOG_DIALOG_TITLE = _T("Réglages du texte");

// ユーザコンフィグ：フォントダイアログ
const TSTR	LCL_STR_FONT_DIALOG_DIALOG_TITLE = _T("Réglages de la police de caractères");
const TSTR	LCL_STR_FONT_DIALOG_GROUP_TITLE = _T("Police de caractères");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_ONOFF = _T("Afficher des polices proportionnelles");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_DESC = _T("* En utilisant des polices proportionnelles, il se peut que l'affichage du texte ne se fasse pas correctement.");
const TSTR	LCL_STR_FONT_DIALOG_STC_BOLD = _T("Afficher le texte en gras");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_OFF = _T("Sans ombre");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_ON = _T("Avec ombre");
const TSTR	LCL_STR_FONT_DIALOG_STC_BORDER_ON = _T("Contour");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON = _T("Contour + Ombre");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_SHORT = _T("VISUAL ARTS / [ Sigus, Français ] 2018");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_LONG = _T("VISUAL ARTS / [ Sigus, Français ] 2018");

// ユーザコンフィグ：メッセージスピードダイアログ
const TSTR	LCL_STR_MSGSPD_DIALOG_DIALOG_TITLE = _T("Réglage de la vitesse d'affichage du texte");
const TSTR	LCL_STR_MSGSPD_DIALOG_GROUP_TITLE = _T("Vitesse d'affichage du texte");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_SLOW = _T("Lent");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_FAST = _T("Rapide");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_NOWAIT = _T("Sans attente");

// ユーザーコンフィグ：音声ダイアログ
const TSTR	LCL_STR_KOEMOD_CHRKOE_DIALOG_DIALOG_TITLE = _T("Réglage des voix");

// ユーザーコンフィグ：音声モードダイアログ
const TSTR	LCL_STR_KOEMOD_DIALOG_DIALOG_TITLE = _T("Réglage du mode vocal");
const TSTR	LCL_STR_KOEMOD_DIALOG_GROUP_TITLE = _T("Mode vocal");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ON = _T("Avec voix");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_OFF = _T("Sans voix");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ONLY = _T("Voix uniquement(sans texte)");

// ユーザーコンフィグ：キャラクター音声ダイアログ
const TSTR	LCL_STR_CHRKOE_DIALOG_DIALOG_TITLE = _T("Réglage des voix des personnages");
const TSTR	LCL_STR_CHRKOE_DIALOG_GROUP_TITLE = _T("Voix des personnages");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_ON = _T("Toutes actives");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_OFF = _T("Toutes désactivées");

// ユーザーコンフィグ：オートモードダイアログ
const TSTR	LCL_STR_DIALOG_AUTOMODE_DIALOG_TITLE = _T("Réglage du mode automatique(Lecture automatique du texte)");
const TSTR	LCL_STR_DIALOG_AUTOMODE_GROUP_TITLE = _T("Mode automatique(Lecture automatique du texte)");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_EXPLAINE = _T("Le délai d'attente : Temps minimum + Nombre de caractères affichés x Temps par caractère");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MOJI_TIME = _T("Temps par caractère");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MIN_TIME = _T("Temps minimum");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_TEN_SECONDS = _T("Délai à attendre pour dix caractères");
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_1(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%d"), n2) + _T(" secondes")
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_2(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%02d"), n2) + _T(" secondes")

// ユーザコンフィグ：システムダイアログ
const TSTR	LCL_STR_SYSTEM_DIALOG_SLEEP_STR = _T("Ralentir l'exécution du programme pour favoriser la fluidité des autres programmes.");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_WIPE_ANIME_STR = _T("Désactiver les animations de fondu au noir.");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_WIPE_ANIME_STR = _T("Passer les fondus au noir avec un clic de souris.");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_MWND_ANIME_STR = _T("Désactiver les animations d'ouverture/fermeture de la fenêtre de messages.");
const TSTR	LCL_STR_SYSTEM_DIALOG_WHEEL_NEXT_MESSAGE_STR = _T("Avancer le texte avec la molette de la souris.");
const TSTR	LCL_STR_SYSTEM_DIALOG_KOE_DONT_STOP_STR = _T("Continuer la lecture et avancer le texte lors des diffusions de voix.");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_UNREAD_MESSAGE_STR = _T("Permettre de passer les textes qui n'ont pas déjà été lus.");
const TSTR	LCL_STR_SYSTEM_DIALOG_PLAY_SILENT_SOUND_STR = _T("Cochez cette option si vous avez des interférences lorsque des sons sont joués.");

// ****************************************************************
// ローカライズ：文字列：確認メッセージ
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RESTART_WARNING_STR = _T("Êtes - vous sûr de vouloir commencer depuis le début ?");			// ★：ゲームを最初から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_SCENESTART_WARNING_STR = _T("Êtes - vous sûr de vouloir reprendre la partie ?");				// ★：ゲームを途中から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_RETURNMENU_WARNING_STR = _T("Êtes - vous sûr de vouloir revenir à l'écran-titre ?");			// ★：タイトルメニューに戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RETURNSEL_WARNING_STR = _T("Êtes - vous sûr de vouloir revenir au choix précédent ?");		// ★：前の選択肢に戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_GAMEEND_WARNING_STR = _T("Êtes - vous sûr de vouloir fermer ?");								// ★：ゲームを終了する時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_SAVE_WARNING_STR = _T("Êtes - vous sûr de vouloir sauvegarder ?");							// ★：セーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_SAVE_WARNING_STR = _T("Êtes - vous sûr de vouloir faire une sauvegarde rapide ?");		// ★：クイックセーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_LOAD_WARNING_STR = _T("Êtes - vous sûr de vouloir charger ?");								// ★：ロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_LOAD_WARNING_STR = _T("Êtes - vous sûr de vouloir charger une sauvegarde rapide ?");	// ★：クイックロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_MSGBK_LOAD_WARNING_STR = _T("Êtes - vous sûr de vouloir charger ?");							// ★：メッセージバックロード確認の確認文章の文字列

// ****************************************************************
// ローカライズ：解像度変更
// ================================================================

#define		LCL_STR_RESOLUTION_EXPLAIN_STR(cx, cy)\
str_format(\
_T("La résolution recommandée pour ce jeu est %d * %d.\n") \
_T("\n") \
_T("S'il vous est possible de sélectionner cette option, nous vous la recommandons.\n") \
_T("Autrement, sélectionnez une résolution avec des proportions proches de la résolution recommandée.\n") \
_T("\n") \
_T("\"Informations complémentaires concernant les écrans LCD\"\n") \
_T("Même avec la résolution recommandée, il est possible que l'affichage soit flou, mauvais,\n") \
_T("ou encore qu'il soit aplati dans le sens de la hauteur ou de la longueur.\n") \
_T("Ces problèmes sont liés aux fonctions de votre écran, et non au jeu lui - même.") \
, cx, cy, cx, cy, cx, cy)

#define		LCL_STR_CHANGE_RESOLUTION_WARNING_STR	\
_T("Nous allons changer la résolution.\n") \
_T("\n") \
_T("\"Attention\"\n") \
_T("Suivant les résolutions, il se peut que l'écran clignote.\n") \
_T("Après le changement, si l'écran ne s'affiche pas correctement, attendez 20 secondes sans rien faire, et nous reviendrons au réglage initial.\n") \
_T("\n") \
_T("\"Informations complémentaires concernant les écrans LCD\"\n") \
_T("Lors du changement de résolution, il se peut que l'écran devienne tout noir.\n") \
_T("Cela arrive parce que l'écran ne reçoit pas bien le signal et considère qu'aucun signal ne lui est envoyé.\n") \
_T("Éteignez puis rallumez votre écran, et l'affichage reviendra à la normale.")

#define		LCL_STR_CHANGE_RESOLUTION_AFTER_STR(cnt)\
str_format(\
_T("La résolution a été modifiée.\n") \
_T("Dans %d secondes, nous reviendrons à la résolution initiale.") \
, cnt)

// ****************************************************************
// ローカライズ：エラーメッセージ
// ================================================================

// フォントエラー
#define		LCL_STR_NOT_EXIST_FONT(font)\
_T("La police de caractères n'a pas pu être trouvée. (Ou est corrompue)") \
_T("\n\n　　Police actuelle: ") + font + \
_T("\n\nVoulez - vous changer la police de caractères ?") \
_T("\n\n*Si vous choisissez \"Non\", le texte ne s'affichera plus.")

// ****************************************************************
// ローカライズ：文字列：サポート情報
// ================================================================

const TSTR	LCL_STR_GENERAL_SUPPORT_STR = _T("\n\n\"Informations de support\"\nVous pouvez copier ce message avec Ctrl + C.");

// ****************************************************************
// ローカライズ：文字列：Twitter
// ================================================================

const TSTR	LCL_STR_TWITTER_DIALOG_TWITTER = _T("Twitter");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_AUTH = _T("Authentification");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_POST = _T("Envoyer sur Twitter");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_NEED = _T("Vous devez vous authentifier pour envoyer votre message.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_SUCCESS = _T("Authentification réussie.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_POST_SUCCESS = _T("Message envoyé.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_SAMPLE = _T("Message posté via Siglus.");

// ****************************************************************
// ローカライズ：ダイアログリソース
// ================================================================

// 共通ダイアログ
const int	LCL_IDD_CFG_WIN_BASE_COMMON = IDD_CFG_WIN_BASE_COMMON_FR;
const int	LCL_IDD_CFG_WIN_SOLO_COMMON = IDD_CFG_WIN_SOLO_COMMON_FR;

// （旧）個別ダイアログ
const int	LCL_IDD_CFG_WIN_BASE = IDD_CFG_WIN_BASE_FR;
//const int	LCL_IDD_CFG_WIN_BASE_AUTOMODE = IDD_CFG_WIN_BASE_AUTOMODE;
//const int	LCL_IDD_CFG_WIN_BASE_CHRKOE = IDD_CFG_WIN_BASE_CHRKOE;
const int	LCL_IDD_CFG_WIN_BASE_ELSE = IDD_CFG_WIN_BASE_ELSE_FR;
//const int	LCL_IDD_CFG_WIN_BASE_FONT_MSGSPD = IDD_CFG_WIN_BASE_FONT_MSGSPD;
const int	LCL_IDD_CFG_WIN_BASE_JITAN = IDD_CFG_WIN_BASE_JITAN_FR;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE = IDD_CFG_WIN_BASE_KOEMODE;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE_CHRKOE = IDD_CFG_WIN_BASE_KOEMODE_CHRKOE;
const int	LCL_IDD_CFG_WIN_BASE_MWNDBK = IDD_CFG_WIN_BASE_MWNDBK_FR;
const int	LCL_IDD_CFG_WIN_BASE_SCREEN = IDD_CFG_WIN_BASE_SCREEN_FR;
const int	LCL_IDD_CFG_WIN_BASE_SYSTEM = IDD_CFG_WIN_BASE_SYSTEM_FR;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME = IDD_CFG_WIN_BASE_VOLUME;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME_BGMFADE = IDD_CFG_WIN_BASE_VOLUME_BGMFADE;
//const int	LCL_IDD_CFG_WIN_SOLO_AUTOMODE = IDD_CFG_WIN_SOLO_AUTOMODE;
//const int	LCL_IDD_CFG_WIN_SOLO_BGMFADE = IDD_CFG_WIN_SOLO_BGMFADE;
//const int	LCL_IDD_CFG_WIN_SOLO_CHRKOE = IDD_CFG_WIN_SOLO_CHRKOE;
const int	LCL_IDD_CFG_WIN_SOLO_ELSE = IDD_CFG_WIN_SOLO_ELSE_FR;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT = IDD_CFG_WIN_SOLO_FONT;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT_MSGSPD = IDD_CFG_WIN_SOLO_FONT_MSGSPD;
const int	LCL_IDD_CFG_WIN_SOLO_JITAN = IDD_CFG_WIN_SOLO_JITAN_FR;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE = IDD_CFG_WIN_SOLO_KOEMODE;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE = IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE;
//const int	LCL_IDD_CFG_WIN_SOLO_MSGSPD = IDD_CFG_WIN_SOLO_MSGSPD;
const int	LCL_IDD_CFG_WIN_SOLO_MOVTYPE = IDD_CFG_WIN_SOLO_MOVTYPE_FR;
const int	LCL_IDD_CFG_WIN_SOLO_MWNDBK = IDD_CFG_WIN_SOLO_MWNDBK_FR;
const int	LCL_IDD_CFG_WIN_SOLO_SCREEN = IDD_CFG_WIN_SOLO_SCREEN_FR;
const int	LCL_IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE = IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE;
const int	LCL_IDD_CFG_WIN_SOLO_SYSTEM = IDD_CFG_WIN_SOLO_SYSTEM_FR;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME = IDD_CFG_WIN_SOLO_VOLUME;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME_BGMFADE = IDD_CFG_WIN_SOLO_VOLUME_BGMFADE;
const int	LCL_IDD_SYS_WIN_SOLO_SAVELOAD = IDD_SYS_WIN_SOLO_SAVELOAD_FR;
const int	LCL_IDD_CHANGE_RESOLUTION_WARNING = IDD_CHANGE_RESOLUTION_WARNING_FR;
const int	LCL_IDD_TWEET = IDD_TWEET_FR;
