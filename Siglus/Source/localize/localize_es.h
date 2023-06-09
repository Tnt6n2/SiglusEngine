#pragma		once

// ****************************************************************
// ローカライズ：言語コード
// ================================================================

#define		LCL_LANGUAGE_CODE	_T("es")

// ****************************************************************
// ローカライズ：Gameexe.ini
// ================================================================

#define		LCL_GAMEEXE_DAT		_T("GameexeES.dat")

// ****************************************************************
// ローカライズ：Scene.pck
// ================================================================

#define		LCL_SCENE_PCK		_T("SceneES.pck")

// ****************************************************************
// ローカライズ：セーブフォルダ
// ================================================================

#define		LCL_SAVEDATA_DIR	_T("savedata_es")

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
const TSTR	LCL_STR_SYSCOMMENU_READ_SKIP_STR = _T("Saltar texto leído");					// ★：一度読んだ文章の早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_UNREAD_SKIP_STR = _T("Saltar texto");						// ★：未読の文章でも早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_AKIP_STR = _T("Avance rápido automático");				// ★：自動早送り機能をＯＮ／ＯＦＦします。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_MODE_STR = _T("Modo automático");						// ★：オートモードを開始します。
const TSTR	LCL_STR_SYSCOMMENU_HIDE_MWND_STR = _T("Esconder mensajes");						// ★：メッセージウィンドウを隠します。
const TSTR	LCL_STR_SYSCOMMENU_MSG_BACK_STR = _T("Abrir log de mensajes");					// ★：メッセージログを開きます。
const TSTR	LCL_STR_SYSCOMMENU_SAVE_STR = _T("Guardar");									// ★：セーブを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_LOAD_STR = _T("Cargar");										// ★：ロードを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_SEL_STR = _T("Volver a la opción anterior");			// ★：前の選択肢に戻ります。
const TSTR	LCL_STR_SYSCOMMENU_CONFIG_STR = _T("Preferencias");								// ★：環境設定を呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_MANUAL_STR = _T("Abrir manual");								// ★：マニュアルを表示します。
const TSTR	LCL_STR_SYSCOMMENU_VERSION_STR = _T("Información de la versión");				// ★：バージョン情報を表示します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_MENU_STR = _T("Regresar el título");					// ★：タイトルメニューに戻ります。
const TSTR	LCL_STR_SYSCOMMENU_GAME_END_STR = _T("Cerrar juego");							// ★：ゲームを終了します。
const TSTR	LCL_STR_SYSCOMMENU_CANCEL_STR = _T("Cancelar");									// ★：システムコマンドメニューをキャンセルします。
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_SWITCH_STR(i)	(_T("Interruptor local general Numero ") + str_format(_T("%d"), i))				// ★：ローカル汎用スイッチ：モード名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_STR(i)		(_T("Interruptor local de uso general Numero ") + str_format(_T("%d"), i))		// ★：ローカル汎用モード：モード名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_STR(i)	(_T("Modo ") + str_format(_T("%d"), i))											// ★：ローカル汎用モード：アイテム名

// ****************************************************************
// ローカライズ：文字列：ダイアログ
// ================================================================

// ダイアログ共通
const TSTR	LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR = _T("Opción");
const TSTR	LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR = _T("Cerrar");
const TSTR	LCL_STR_MODAL_DIALOG_INIT_BTN_STR = _T("Deshacer");

// セーブダイアログ
const TSTR	LCL_STR_SAVE_DIALOG_CAPTION_TITLE_STR = _T("Guardar");												// ★：セーブダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DECIDE_BUTTON_STR = _T("Guardar");												// ★：セーブダイアログの決定ボタンの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DATALIST_NAMEHEADER_STR = _T("Guardar");										// ★：セーブダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_SAVE_DIALOG_WARNING_CHECKBOX_STR = _T("Mostrar ventana de confirmación de sobreescritura antes de guardar");	// ★：セーブダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DBLCLK_CHECKBOX_STR = _T("Doble click en la lista para guardar");				// ★：セーブダイアログのダブルクリックチェックボックスの文字列

// ロードダイアログ
const TSTR	LCL_STR_LOAD_DIALOG_CAPTION_TITLE_STR = _T("Cargar");												// ★：ロードダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DECIDE_BUTTON_STR = _T("Cargar");												// ★：ロードダイアログの決定ボタンの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DATALIST_NAMEHEADER_STR = _T("Cargar");											// ★：ロードダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_LOAD_DIALOG_WARNING_CHECKBOX_STR = _T("Mostrar ventana de confirmación antes de cargar");	// ★：ロードダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DBLCLK_CHECKBOX_STR = _T("Haga doble click para guardar los datos");			// ★：ロードダイアログのダブルクリックチェックボックスの文字列

// セーブダイアログ＆ロードダイアログの共通情報
const TSTR	LCL_STR_SAVELOAD_DIALOG_NO_DATA_STR = _T("No hay datos disponibles");

// ユーザコンフィグ：環境設定ダイアログ
const TSTR	LCL_STR_CONFIG_DIALOG_SCREEN_STR = _T("Pantalla");
const TSTR	LCL_STR_CONFIG_DIALOG_VOLUME_STR = _T("Sonido");
const TSTR	LCL_STR_CONFIG_DIALOG_MESSAGE_STR = _T("Letras");
const TSTR	LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR = _T("Color de fondo del texto");
const TSTR	LCL_STR_CONFIG_DIALOG_VOICE_STR = _T("Voces");
const TSTR	LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR = _T("Modo automático");
const TSTR	LCL_STR_CONFIG_DIALOG_JITAN_STR = _T("Velocidad de reproducción");
const TSTR	LCL_STR_CONFIG_DIALOG_OTHERS_STR = _T("Otros");
const TSTR	LCL_STR_CONFIG_DIALOG_SYSTEM_STR = _T("Sistema");

// ユーザコンフィグ：画面ダイアログ
const TSTR	LCL_STR_SCREEN_DIALOG_CURRENT_RESOLUTION_STR = _T("(Resolución actual)");
const TSTR	LCL_STR_SCREEN_DIALOG_DEFAULT_RESOLUTION_STR = _T("(Resolución original)");

// ユーザコンフィグ：音量・ＢＧＭフェードダイアログ
const TSTR	LCL_STR_VOLUME_BGMFADE_DIALOG_DIALOG_TITLE = _T("Ajustes de volumen");

// ユーザコンフィグ：音量ダイアログ
const TSTR	LCL_STR_VOLUME_DIALOG_DIALOG_TITLE = _T("Ajustes de volumen");
const TSTR	LCL_STR_VOLUME_DIALOG_GROUP_TITLE = _T("Volumen");
const TSTR	LCL_STR_VOLUME_DIALOG_ALL_GROUP_TITLE = _T("Completo");
const TSTR	LCL_STR_VOLUME_DIALOG_BGM_GROUP_TITLE = _T("BGM");
const TSTR	LCL_STR_VOLUME_DIALOG_KOE_GROUP_TITLE = _T("Voces");
const TSTR	LCL_STR_VOLUME_DIALOG_PCM_GROUP_TITLE = _T("Efecto");
const TSTR	LCL_STR_VOLUME_DIALOG_SE_GROUP_TITLE = _T("Sistema");
const TSTR	LCL_STR_VOLUME_DIALOG_MOV_GROUP_TITLE = _T("Video");
const TSTR	LCL_STR_VOLUME_DIALOG_STC_PLAY = _T("On");

// ユーザコンフィグ：ＢＧＭフェードダイアログ
const TSTR	LCL_STR_BGMFADE_DIALOG_DIALOG_TITLE = _T("Ajustes atenuar BGM");
const TSTR	LCL_STR_BGMFADE_DIALOG_GROUP_TITLE = _T("Atenuar BGM");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_EXPLAINE = _T("Función para ajustar el volumen del BGM cuando reproduces la voz.");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_ENABLE = _T("Uso");

// ユーザコンフィグ：フォント・メッセージスピードダイアログ
const TSTR	LCL_STR_FONT_MSGSPD_DIALOG_DIALOG_TITLE = _T("Configuración de texto");

// ユーザコンフィグ：フォントダイアログ
const TSTR	LCL_STR_FONT_DIALOG_DIALOG_TITLE = _T("Configuración de fuente");
const TSTR	LCL_STR_FONT_DIALOG_GROUP_TITLE = _T("Fuente");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_ONOFF = _T("Mostrar fuente proporcional");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_DESC = _T("* Cuando se use la fuente proporcional cabe la posibilidad de que el texto se rompa");
const TSTR	LCL_STR_FONT_DIALOG_STC_BOLD = _T("Engruesar las letras");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_OFF = _T("Sin sombra");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_ON = _T("Con sombra");
const TSTR	LCL_STR_FONT_DIALOG_STC_BORDER_ON = _T("Borde");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON = _T("Borde + Sombra");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_SHORT = _T("VISUAL ARTS / [ Sigus, español ] 2018");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_LONG = _T("VISUAL ARTS / [ Sigus, español ] 2018");

// ユーザコンフィグ：メッセージスピードダイアログ
const TSTR	LCL_STR_MSGSPD_DIALOG_DIALOG_TITLE = _T("Ajustes de velocidad de texto");
const TSTR	LCL_STR_MSGSPD_DIALOG_GROUP_TITLE = _T("Velocidad del texto");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_SLOW = _T("Lenta");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_FAST = _T("Rápida");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_NOWAIT = _T("Sin espera");

// ユーザーコンフィグ：音声ダイアログ
const TSTR	LCL_STR_KOEMOD_CHRKOE_DIALOG_DIALOG_TITLE = _T("Ajustes de voz");

// ユーザーコンフィグ：音声モードダイアログ
const TSTR	LCL_STR_KOEMOD_DIALOG_DIALOG_TITLE = _T("Ajustes del modo voz");
const TSTR	LCL_STR_KOEMOD_DIALOG_GROUP_TITLE = _T("Modo voz");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ON = _T("Con voz");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_OFF = _T("Sin voz");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ONLY = _T("Solo voz(sin texto)");

// ユーザーコンフィグ：キャラクター音声ダイアログ
const TSTR	LCL_STR_CHRKOE_DIALOG_DIALOG_TITLE = _T("Ajustes de la voz del personaje");
const TSTR	LCL_STR_CHRKOE_DIALOG_GROUP_TITLE = _T("Voz del personaje");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_ON = _T("Todo ON");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_OFF = _T("Todo OFF");

// ユーザーコンフィグ：オートモードダイアログ
const TSTR	LCL_STR_DIALOG_AUTOMODE_DIALOG_TITLE = _T("Ajustes del modo automático(Función para pasar el texto automaticamente)");
const TSTR	LCL_STR_DIALOG_AUTOMODE_GROUP_TITLE = _T("Modo automático(Función para pasar el texto automaticamente)");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_EXPLAINE = _T("Espera = (Caracteres mostrados * tiempo por caracter) + tiempo mínimo.");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MOJI_TIME = _T("Tiempo por caracter");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MIN_TIME = _T("Tiempo mínimo");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_TEN_SECONDS = _T("Si son 10 caracteres el tiempo de espera serán");
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_1(n1, n2)	str_format(_T("%d"), n1) + _T("．") + str_format(_T("%d"), n2) + _T(" segundos")
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_2(n1, n2)	str_format(_T("%d"), n1) + _T("．") + str_format(_T("%02d"), n2) + _T(" segundos")

// ユーザコンフィグ：システムダイアログ
const TSTR	LCL_STR_SYSTEM_DIALOG_SLEEP_STR = _T("Las acciones de este programa se han vuelto lentas, haga que los demás programas operen de manera más fluida.");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_WIPE_ANIME_STR = _T("Deshabilitar el efecto oscuro de la animación.");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_WIPE_ANIME_STR = _T("Saltar el efecto oscuro de la animación con un click del ratón.");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_MWND_ANIME_STR = _T("Deshabilitar la animación a la hora de cerrar o abrir la ventana de mensajes.");
const TSTR	LCL_STR_SYSTEM_DIALOG_WHEEL_NEXT_MESSAGE_STR = _T("Puedes avanzar el texto moviendo el scroll del ratón hacia abajo.");
const TSTR	LCL_STR_SYSTEM_DIALOG_KOE_DONT_STOP_STR = _T("Hacer que la reproducción de voz siga aun habíendo pasado al siguiente texto.");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_UNREAD_MESSAGE_STR = _T("Poder avanzar rápido incluso los textos no leídos.");
const TSTR	LCL_STR_SYSTEM_DIALOG_PLAY_SILENT_SOUND_STR = _T("Vigile que no haya ruido a la hora de reproducir el sonido.");

// ****************************************************************
// ローカライズ：文字列：確認メッセージ
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RESTART_WARNING_STR = _T("¿Está seguro que desea comenzar desde el principio?");					// ★：ゲームを最初から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_SCENESTART_WARNING_STR = _T("¿Está seguro que desea comenzar desde el último dato guardado?");	// ★：ゲームを途中から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_RETURNMENU_WARNING_STR = _T("¿Está seguro que desea regresar el título?");						// ★：タイトルメニューに戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RETURNSEL_WARNING_STR = _T("¿Está seguro que desea regresar a la opción anterior?");				// ★：前の選択肢に戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_GAMEEND_WARNING_STR = _T("¿Está seguro que desea salir?");					// ★：ゲームを終了する時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_SAVE_WARNING_STR = _T("¿Desea guardar?");									// ★：セーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_SAVE_WARNING_STR = _T("¿Desea hacer un guardado rápido?");				// ★：クイックセーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_LOAD_WARNING_STR = _T("¿Desea cargar?");										// ★：ロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_LOAD_WARNING_STR = _T("¿Desea hacer un cargado rápido?");				// ★：クイックロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_MSGBK_LOAD_WARNING_STR = _T("¿Desea cargar?");								// ★：メッセージバックロード確認の確認文章の文字列

// ****************************************************************
// ローカライズ：解像度変更
// ================================================================

#define		LCL_STR_RESOLUTION_EXPLAIN_STR(cx, cy)\
str_format(\
_T("La resolución optimizada de este producto es \"%d X %d\".\n") \
_T("\n") \
_T("En el caso de que pueda elegir una resolución de \"%d X %d\" eliga esa por favor.\n") \
_T("En el caso de que no pueda, elija la resolución que este más cerca de la resolución optimizada.\n") \
_T("\n") \
_T("[ Acerca el monitor de cristal líquido ]\n") \
_T("Se puede volver oscuro a pesar de tener la resolución optimizada.\n") \
_T("O tal vez, puede ponerse en vertical o volverse lento.\n") \
_T("Debido a que esto es una función del monitor, tenga cuidado.") \
, cx, cy, cx, cy)

#define		LCL_STR_CHANGE_RESOLUTION_WARNING_STR	\
_T("Vamos a cambiar la resolución.\n") \
_T("\n") \
_T("[ Advertencia ]\n") \
_T("Hay ocasiones, que debido a la resolución la pantalla parpadea.\n") \
_T("En el caso que después del cambio la pantalla no vuelva a su modo normal, solamente espere por favor.\n") \
_T("En 20 segundos volverá a su ajuste anterior.\n") \
_T("\n") \
_T("[ Acerca de los monitores de cristal liquido ]\n") \
_T("Hay veces que la pantalla se quedará en negro al haber cambiado la resolución.\n") \
_T("Este fenómeno es debido a que el monitor no puede recibir bien la señal de video y se queda en estado de sin señal.\n") \
_T("Para que vuelva a estar en modo normal, ponga en OFF el interruptor de alimentación y después vuelva a ponerlo en ON.")

#define		LCL_STR_CHANGE_RESOLUTION_AFTER_STR(cnt)\
str_format( \
_T("Has cambiado la resolución.\n") \
_T("En %d segundos volverás a la anterior resolución.") \
, cnt)

// ****************************************************************
// ローカライズ：エラーメッセージ
// ================================================================

// フォントエラー
#define		LCL_STR_NOT_EXIST_FONT(font)\
_T("No se pudo encontrar la fuente. (O en otro caso está dañada)") \
_T("\n\n　　La fuente actual es: ") + font + \
_T("\n\n¿Desea cambiar la fuente?") \
_T("\n\n* En caso de seleccionar [No] la fuente no se visualizará.")

// ****************************************************************
// ローカライズ：文字列：サポート情報
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_GENERAL_SUPPORT_STR = _T("\n\n[ Mensaje de soporte ]\nPuede copiar este mensaje con Ctrl+C.");

// ****************************************************************
// ローカライズ：文字列：Twitter
// ================================================================

const TSTR	LCL_STR_TWITTER_DIALOG_TWITTER = _T("Twitter");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_AUTH = _T("Autentificación");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_POST = _T("Publicar en twitter");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_NEED = _T("Es necesario autentificarse para publicar.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_SUCCESS = _T("Autentificación realizada.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_POST_SUCCESS = _T("Has publicado.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_SAMPLE = _T("Publicación desde Siglus.");

// ****************************************************************
// ローカライズ：ダイアログリソース
// ================================================================

// 共通ダイアログ
const int	LCL_IDD_CFG_WIN_BASE_COMMON = IDD_CFG_WIN_BASE_COMMON_ES;
const int	LCL_IDD_CFG_WIN_SOLO_COMMON = IDD_CFG_WIN_SOLO_COMMON_ES;

// （旧）個別ダイアログ
const int	LCL_IDD_CFG_WIN_BASE = IDD_CFG_WIN_BASE_ES;
//const int	LCL_IDD_CFG_WIN_BASE_AUTOMODE = IDD_CFG_WIN_BASE_AUTOMODE;
//const int	LCL_IDD_CFG_WIN_BASE_CHRKOE = IDD_CFG_WIN_BASE_CHRKOE;
const int	LCL_IDD_CFG_WIN_BASE_ELSE = IDD_CFG_WIN_BASE_ELSE_ES;
//const int	LCL_IDD_CFG_WIN_BASE_FONT_MSGSPD = IDD_CFG_WIN_BASE_FONT_MSGSPD;
const int	LCL_IDD_CFG_WIN_BASE_JITAN = IDD_CFG_WIN_BASE_JITAN_ES;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE = IDD_CFG_WIN_BASE_KOEMODE;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE_CHRKOE = IDD_CFG_WIN_BASE_KOEMODE_CHRKOE;
const int	LCL_IDD_CFG_WIN_BASE_MWNDBK = IDD_CFG_WIN_BASE_MWNDBK_ES;
const int	LCL_IDD_CFG_WIN_BASE_SCREEN = IDD_CFG_WIN_BASE_SCREEN_ES;
const int	LCL_IDD_CFG_WIN_BASE_SYSTEM = IDD_CFG_WIN_BASE_SYSTEM_ES;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME = IDD_CFG_WIN_BASE_VOLUME;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME_BGMFADE = IDD_CFG_WIN_BASE_VOLUME_BGMFADE;
//const int	LCL_IDD_CFG_WIN_SOLO_AUTOMODE = IDD_CFG_WIN_SOLO_AUTOMODE;
//const int	LCL_IDD_CFG_WIN_SOLO_BGMFADE = IDD_CFG_WIN_SOLO_BGMFADE;
//const int	LCL_IDD_CFG_WIN_SOLO_CHRKOE = IDD_CFG_WIN_SOLO_CHRKOE;
const int	LCL_IDD_CFG_WIN_SOLO_ELSE = IDD_CFG_WIN_SOLO_ELSE_ES;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT = IDD_CFG_WIN_SOLO_FONT;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT_MSGSPD = IDD_CFG_WIN_SOLO_FONT_MSGSPD;
const int	LCL_IDD_CFG_WIN_SOLO_JITAN = IDD_CFG_WIN_SOLO_JITAN_ES;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE = IDD_CFG_WIN_SOLO_KOEMODE;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE = IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE;
//const int	LCL_IDD_CFG_WIN_SOLO_MSGSPD = IDD_CFG_WIN_SOLO_MSGSPD;
const int	LCL_IDD_CFG_WIN_SOLO_MOVTYPE = IDD_CFG_WIN_SOLO_MOVTYPE_ES;
const int	LCL_IDD_CFG_WIN_SOLO_MWNDBK = IDD_CFG_WIN_SOLO_MWNDBK_ES;
const int	LCL_IDD_CFG_WIN_SOLO_SCREEN = IDD_CFG_WIN_SOLO_SCREEN_ES;
const int	LCL_IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE = IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE;
const int	LCL_IDD_CFG_WIN_SOLO_SYSTEM = IDD_CFG_WIN_SOLO_SYSTEM_ES;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME = IDD_CFG_WIN_SOLO_VOLUME;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME_BGMFADE = IDD_CFG_WIN_SOLO_VOLUME_BGMFADE;
const int	LCL_IDD_SYS_WIN_SOLO_SAVELOAD = IDD_SYS_WIN_SOLO_SAVELOAD_ES;
const int	LCL_IDD_CHANGE_RESOLUTION_WARNING = IDD_CHANGE_RESOLUTION_WARNING_ES;
const int	LCL_IDD_TWEET = IDD_TWEET_ES;

