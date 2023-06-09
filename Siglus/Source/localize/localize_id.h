#pragma		once

// ****************************************************************
// ローカライズ：言語コード
// ================================================================

#define		LCL_LANGUAGE_CODE	_T("id")

// ****************************************************************
// ローカライズ：Gameexe.ini
// ================================================================

#define		LCL_GAMEEXE_DAT		_T("GameexeID.dat")

// ****************************************************************
// ローカライズ：Scene.pck
// ================================================================

#define		LCL_SCENE_PCK		_T("SceneID.pck")

// ****************************************************************
// ローカライズ：セーブフォルダ
// ================================================================

#define		LCL_SAVEDATA_DIR	_T("savedata_id")

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
const TSTR	LCL_STR_SYSCOMMENU_READ_SKIP_STR = _T("Mempercepat tayangan kalimat untuk dibaca");		// ★：一度読んだ文章の早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_UNREAD_SKIP_STR = _T("Mempercepat tayangan kalimat");				// ★：未読の文章でも早送りを開始します。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_AKIP_STR = _T("Mempercepat otomatis");							// ★：自動早送り機能をＯＮ／ＯＦＦします。
const TSTR	LCL_STR_SYSCOMMENU_AUTO_MODE_STR = _T("Auto mode");										// ★：オートモードを開始します。
const TSTR	LCL_STR_SYSCOMMENU_HIDE_MWND_STR = _T("Sembunyikan pesan");								// ★：メッセージウィンドウを隠します。
const TSTR	LCL_STR_SYSCOMMENU_MSG_BACK_STR = _T("Membuka pesan");									// ★：メッセージバックを開きます。
const TSTR	LCL_STR_SYSCOMMENU_SAVE_STR = _T("Simpan");												// ★：セーブを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_LOAD_STR = _T("Load");												// ★：ロードを呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_SEL_STR = _T("Kembali ke opsi sebelumnya");					// ★：前の選択肢に戻ります。
const TSTR	LCL_STR_SYSCOMMENU_CONFIG_STR = _T("Pengaturan");										// ★：環境設定を呼び出します。
const TSTR	LCL_STR_SYSCOMMENU_MANUAL_STR = _T("Melihat panduan manual");							// ★：マニュアルを表示します。
const TSTR	LCL_STR_SYSCOMMENU_VERSION_STR = _T("Informasi Versi");									// ★：バージョン情報を表示します。
const TSTR	LCL_STR_SYSCOMMENU_RETURN_MENU_STR = _T("Kembali ke Judul");							// ★：タイトルメニューに戻ります。
const TSTR	LCL_STR_SYSCOMMENU_GAME_END_STR = _T("Mengakhiri permainan");							// ★：ゲームを終了します。
const TSTR	LCL_STR_SYSCOMMENU_CANCEL_STR = _T("Membatalkan");										// ★：システムコマンドメニューをキャンセルします。
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_SWITCH_STR(i)	(str_format(_T("Switch for local general purpose no %d"), i))	// ★：ローカル汎用スイッチ：スイッチ名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_STR(i)		(str_format(_T("Mode of local general purpose no %d"), i))		// ★：ローカル汎用モード：モード名
#define		LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_STR(i)	(str_format(_T("Mode %d"), i))									// ★：ローカル汎用モード：アイテム名

// ****************************************************************
// ローカライズ：文字列：ダイアログ
// ================================================================

// ダイアログ共通
const TSTR	LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR = _T("Keputusan");
const TSTR	LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR = _T("Menutup");
const TSTR	LCL_STR_MODAL_DIALOG_INIT_BTN_STR = _T("Kembali ke awal");

// セーブダイアログ
const TSTR	LCL_STR_SAVE_DIALOG_CAPTION_TITLE_STR = _T("Simpan");													// ★：セーブダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DECIDE_BUTTON_STR = _T("Simpan");													// ★：セーブダイアログの決定ボタンの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DATALIST_NAMEHEADER_STR = _T("Simpan");												// ★：セーブダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_SAVE_DIALOG_WARNING_CHECKBOX_STR = _T("Sebelum menyimpan data silakan dikonfirmasi pada tampilan jendela  di monitor anda");	// ★：セーブダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_SAVE_DIALOG_DBLCLK_CHECKBOX_STR = _T("Untuk menyimpan data  silakan klik dua kali di list");		// ★：セーブダイアログのダブルクリックチェックボックスの文字列

// ロードダイアログ
const TSTR	LCL_STR_LOAD_DIALOG_CAPTION_TITLE_STR = _T("Load");												// ★：ロードダイアログのキャプションタイトルの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DECIDE_BUTTON_STR = _T("Load");												// ★：ロードダイアログの決定ボタンの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DATALIST_NAMEHEADER_STR = _T("Load");										// ★：ロードダイアログのデータリストの先頭文字列
const TSTR	LCL_STR_LOAD_DIALOG_WARNING_CHECKBOX_STR = _T("Sebelum memuat data silakan dikonfirmasi pada tampilan jendela di monitor anda");		// ★：ロードダイアログの確認チェックボックスの文字列
const TSTR	LCL_STR_LOAD_DIALOG_DBLCLK_CHECKBOX_STR = _T("Untuk menyimpan data yang sudah dimuat silakan klik dua kali");	// ★：ロードダイアログのダブルクリックチェックボックスの文字列

// セーブダイアログ＆ロードダイアログの共通情報
const TSTR	LCL_STR_SAVELOAD_DIALOG_NO_DATA_STR = _T("Tidak ada data");

// ユーザコンフィグ：環境設定ダイアログ
const TSTR	LCL_STR_CONFIG_DIALOG_SCREEN_STR = _T("Monitor");
const TSTR	LCL_STR_CONFIG_DIALOG_VOLUME_STR = _T("Volume suara");
const TSTR	LCL_STR_CONFIG_DIALOG_MESSAGE_STR = _T("Kalimat");
const TSTR	LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR = _T("Warna latar");
const TSTR	LCL_STR_CONFIG_DIALOG_VOICE_STR = _T("Suara");
const TSTR	LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR = _T("Mode otomatis");
const TSTR	LCL_STR_CONFIG_DIALOG_JITAN_STR = _T("Pemutaran ekspres");
const TSTR	LCL_STR_CONFIG_DIALOG_OTHERS_STR = _T("Lain-lain");
const TSTR	LCL_STR_CONFIG_DIALOG_SYSTEM_STR = _T("Sistem");

// ユーザコンフィグ：画面ダイアログ
const TSTR	LCL_STR_SCREEN_DIALOG_CURRENT_RESOLUTION_STR = _T("(Resolusi Terkini)");
const TSTR	LCL_STR_SCREEN_DIALOG_DEFAULT_RESOLUTION_STR = _T("(Resolusi Orjinal)");

// ユーザコンフィグ：音量・ＢＧＭフェードダイアログ
const TSTR	LCL_STR_VOLUME_BGMFADE_DIALOG_DIALOG_TITLE = _T("Pengaturan volume suara");

// ユーザコンフィグ：音量ダイアログ
const TSTR	LCL_STR_VOLUME_DIALOG_DIALOG_TITLE = _T("Pengaturan volume suara");
const TSTR	LCL_STR_VOLUME_DIALOG_GROUP_TITLE = _T("Volume Suara");
const TSTR	LCL_STR_VOLUME_DIALOG_ALL_GROUP_TITLE = _T("Keseluruhan");
const TSTR	LCL_STR_VOLUME_DIALOG_BGM_GROUP_TITLE = _T("BGM");
const TSTR	LCL_STR_VOLUME_DIALOG_KOE_GROUP_TITLE = _T("Suara");
const TSTR	LCL_STR_VOLUME_DIALOG_PCM_GROUP_TITLE = _T("Efek");
const TSTR	LCL_STR_VOLUME_DIALOG_SE_GROUP_TITLE = _T("Sistem");
const TSTR	LCL_STR_VOLUME_DIALOG_MOV_GROUP_TITLE = _T("Film");
const TSTR	LCL_STR_VOLUME_DIALOG_STC_PLAY = _T("Putar");

// ユーザコンフィグ：ＢＧＭフェードダイアログ
const TSTR	LCL_STR_BGMFADE_DIALOG_DIALOG_TITLE = _T("Pengaturan BGM Fade");
const TSTR	LCL_STR_BGMFADE_DIALOG_GROUP_TITLE = _T("BGM Fade");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_EXPLAINE = _T("Fungsi pengaturan suara volume BGM saat suara karakter sedang tayang");
const TSTR	LCL_STR_BGMFADE_DIALOG_STC_ENABLE = _T("Gunakan");

// ユーザコンフィグ：フォント・メッセージスピードダイアログ
const TSTR	LCL_STR_FONT_MSGSPD_DIALOG_DIALOG_TITLE = _T("Pengaturan Kalimat");

// ユーザコンフィグ：フォントダイアログ
const TSTR	LCL_STR_FONT_DIALOG_DIALOG_TITLE = _T("Pengaturan Font");
const TSTR	LCL_STR_FONT_DIALOG_GROUP_TITLE = _T("Font");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_ONOFF = _T("Tampilan Font Proporsional");
const TSTR	LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_DESC = _T("* Apabila font Proporsional digunakan, maka ada kemungkinan lay out  kalimatnya  akan berubah.");
const TSTR	LCL_STR_FONT_DIALOG_STC_BOLD = _T("Tebalkan huruf");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_OFF = _T("Tanpa bayangan");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_ON = _T("Dengan bayangan");
const TSTR	LCL_STR_FONT_DIALOG_STC_BORDER_ON = _T("Margin");
const TSTR	LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON = _T("edge+bayangan");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_SHORT = _T("VISUAL ARTS / [Siglus, Bahasa Indonesia ] 2018");
const TSTR	LCL_STR_FONT_DIALOG_SAMPLE_STR_LONG = _T("VISUAL ARTS / [Siglus, Bahasa Indonesia ] 2018");

// ユーザコンフィグ：メッセージスピードダイアログ
const TSTR	LCL_STR_MSGSPD_DIALOG_DIALOG_TITLE = _T("Pengaturan kecepatan huruf");
const TSTR	LCL_STR_MSGSPD_DIALOG_GROUP_TITLE = _T("Kecepatan huruf");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_SLOW = _T("Lambat");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_FAST = _T("Cepat");
const TSTR	LCL_STR_MSGSPD_DIALOG_STC_NOWAIT = _T("No Wait");

// ユーザーコンフィグ：音声ダイアログ
const TSTR	LCL_STR_KOEMOD_CHRKOE_DIALOG_DIALOG_TITLE = _T("Pengaturan penayangan suara");

// ユーザーコンフィグ：音声モードダイアログ
const TSTR	LCL_STR_KOEMOD_DIALOG_DIALOG_TITLE = _T("Pengaturan mode tayang untuk suara");
const TSTR	LCL_STR_KOEMOD_DIALOG_GROUP_TITLE = _T("Mode untuk tayang suara");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ON = _T("Penayangan dengan suara");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_OFF = _T("Penayangan tanpa suara");
const TSTR	LCL_STR_KOEMOD_DIALOG_KOE_ONLY = _T("Penayangan hanya suara ( tanpa kalimat )");

// ユーザーコンフィグ：キャラクター音声ダイアログ
const TSTR	LCL_STR_CHRKOE_DIALOG_DIALOG_TITLE = _T("Seting untuk suara karakter ( Penayangan media dan pencocokan suara )");
const TSTR	LCL_STR_CHRKOE_DIALOG_GROUP_TITLE = _T("Suara tokoh karakter ( Penayangan media dan volume suara )");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_ON = _T("Nyalakan seluruh perangkat");
const TSTR	LCL_STR_CHRKOE_DIALOG_ALL_OFF = _T("Matikan seluruh perangkat");

// ユーザーコンフィグ：オートモードダイアログ
const TSTR	LCL_STR_DIALOG_AUTOMODE_DIALOG_TITLE = _T("Pengaturan mode otomatis ( Fungsi  penayangan teks otomatis )");
const TSTR	LCL_STR_DIALOG_AUTOMODE_GROUP_TITLE = _T("Mode otomatis  ( Fungsi  penayangan teks otomatis )");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_EXPLAINE = _T("Waktu menunggu dihitung dari [ jumlah huruf  X  waktu minimum ]");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MOJI_TIME = _T("Waktu untuk huruf");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_MIN_TIME = _T("Waktu minimum");
const TSTR	LCL_STR_DIALOG_AUTOMODE_STC_TEN_SECONDS = _T("Waktu tunggu saat menampilkan 10 huruf");
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_1(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%d"), n2) + _T(" detik")
#define		LCL_STR_AUTOMODE_DIALOG_TIME_STR_2(n1, n2)	str_format(_T("%d"), n1) + _T(".") + str_format(_T("%02d"), n2) + _T(" detik")

// ユーザコンフィグ：システムダイアログ
const TSTR	LCL_STR_SYSTEM_DIALOG_SLEEP_STR = _T("Perlambat pengoperasian program ini sehingga program lain dapat beroperasi dengan lancar.");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_WIPE_ANIME_STR = _T("Efek penggelapan layar akan menonaktifkan pemutaran animasi.");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_WIPE_ANIME_STR = _T("Abaikan efek penggelapan layar dengan  mengklik mouse.");
const TSTR	LCL_STR_SYSTEM_DIALOG_NO_MWND_ANIME_STR = _T("Saat buka-tutup pesan di Message Window, animasi akan menjadi non aktif.");
const TSTR	LCL_STR_SYSTEM_DIALOG_WHEEL_NEXT_MESSAGE_STR = _T("Anda dapat terus membaca dengan tombol roda mouse ( tombol foil ) menghadap ke bawah.");
const TSTR	LCL_STR_SYSTEM_DIALOG_KOE_DONT_STOP_STR = _T("Tayangan akan terus diputar walau anda mempercepat membaca ke kalimat berikutnya di tengah penayangan suara.");
const TSTR	LCL_STR_SYSTEM_DIALOG_SKIP_UNREAD_MESSAGE_STR = _T("Mampu mempercepat tayangan walaupun kalimatnya  belum dibaca.");
const TSTR	LCL_STR_SYSTEM_DIALOG_PLAY_SILENT_SOUND_STR = _T("Apabila ada suara yang mengganggu saat tayangan dimulai mohon dicek.");

// ****************************************************************
// ローカライズ：文字列：確認メッセージ
// ================================================================

// ユーザコンフィグ：各種確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RESTART_WARNING_STR = _T("Apakah anda yakin akan memulainya dari awal ?");					// ★：ゲームを最初から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_SCENESTART_WARNING_STR = _T("Apakah anda takin  akan memulainya dari pertengahan ?");		// ★：ゲームを途中から始める時の確認メッセージの文字列（デバッグ機能）
const TSTR	LCL_STR_CONFIG_RETURNMENU_WARNING_STR = _T("Apakah anda yakin  akan kembali ke bagian judul ?");			// ★：タイトルメニューに戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_RETURNSEL_WARNING_STR = _T("Apakah anda yakin akan kembali ke opsi sebelumnya ?");			// ★：前の選択肢に戻る時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_GAMEEND_WARNING_STR = _T("Apakah anda yakin akan mengakhirkannya ?");						// ★：ゲームを終了する時の確認メッセージの文字列
const TSTR	LCL_STR_CONFIG_SAVE_WARNING_STR = _T("Apakah anda yakin akan menyimpan data ini ?");						// ★：セーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_SAVE_WARNING_STR = _T("Apakah anda yakin  akan menyimpannya dengan cara quick save ?");// ★：クイックセーブ確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_LOAD_WARNING_STR = _T("Anda yakin akan memuat data ini ?");									// ★：ロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_QUICK_LOAD_WARNING_STR = _T("Apakah anda yakin akan memuat data dengan cara quick load ?");	// ★：クイックロード確認の確認文章の文字列
const TSTR	LCL_STR_CONFIG_MSGBK_LOAD_WARNING_STR = _T("Anda yakin akan memuat data ini ?");						// ★：メッセージバックロード確認の確認文章の文字列

// ****************************************************************
// ローカライズ：解像度変更
// ================================================================

#define		LCL_STR_RESOLUTION_EXPLAIN_STR(cx, cy)\
str_format(\
_T("Resolusi optimal dari produk ini adalah \"%d × %d\".\n") \
_T("\n") \
_T("Apabila resolusi \"%d * %d\" dapat dipilih, maka silakan untuk memilihnya.\n") \
_T("Akan tetapi, apabila tidak ada, maka pilih rasio yang mendekati[resolusi terbaik] yang lebih memungkinakan.\n") \
_T("\n") \
_T("« Mengenai tambahan untuk monitor LCD »\n") \
_T("Ada beberapa monitor yang bila disetel ke \"resolusi optimal\", maka tampilannya akan kabur atau kasar.\n") \
_T("Dan bahkan ada juga yang menjadikan tampilannya memanjang secara vertikal atau horizontal.\n") \
_T("Mohon agar masalah monitor yang seperti ini dapat dimaklumi oleh anda sekalian.") \
, cx, cy, cx, cy, cx, cy)

#define		LCL_STR_CHANGE_RESOLUTION_WARNING_STR	\
_T("Mengubah resolusi\n") \
_T("\n") \
_T("\" Mohon diwaspadai \"\n") \
_T("Akibat resolusi yang anda lakukan mungkin saja layar monitor akan berkedip - kedip.\n") \
_T("Dan, apabila anda ingin mengembalikannya ke tampilan normal, mohon tunggu sekitar 20 detik dan  layar akan kembali ke pengaturan awal kembali.\n") \
_T("\n") \
_T("«Tentang monitor LCD»\n") \
_T("Kadang kala apabila kita mengubah resolusi, layar komputer anda mungkin tiba - tiba akan menjadi gelap.\n") \
_T("Fenomena ini terjadi karena monitor tidak dapat menerima sinyal tampilan yang akan dimuat.Alhasil saat itu monitor akan berada dalam kondisi tanpa sinyal.\n") \
_T("Untuk mengatasi hal ini, maka matikan tombol  monitor sebentar, kemudian hidupkanlah lagi, dan monitor akan tampil secara normal kembali.")

#define		LCL_STR_CHANGE_RESOLUTION_AFTER_STR(cnt)\
str_format(\
_T("Pengubahan resolusi telah dilaksanakan.\n") \
_T("Dalam %d detik akan kembali ke resolusi awal.") \
, cnt)

// ****************************************************************
// ローカライズ：エラーメッセージ
// ================================================================

// フォントエラー
#define		LCL_STR_NOT_EXIST_FONT(font)\
_T("Font tidak dapat ditemukan .") \
_T("\n(Mungkin karena rusak)") \
_T("\n\n　　Font yang sekarang digunakan : ") + font + \
_T("\n\nApakah anda akan mengubah font nya ?") \
_T("\n\n*Apabila anda memilih([Tidak]), maka huruf tidak akan ditayangkan .")

// ****************************************************************
// ローカライズ：文字列：サポート情報
// ================================================================

const TSTR	LCL_STR_GENERAL_SUPPORT_STR = _T("\n\n{ Info Tambahan }\nPesan ini dapat di copy dengan menekan Ctrl+C.");

// ****************************************************************
// ローカライズ：文字列：Twitter
// ================================================================

const TSTR	LCL_STR_TWITTER_DIALOG_TWITTER = _T("Twitter");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_AUTH = _T("Otentifikasi");
const TSTR	LCL_STR_TWITTER_DIALOG_BTN_POST = _T("Posting ke Twitter");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_NEED = _T("Untuk posting diperlukan otentifikasi.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_AUTH_SUCCESS = _T("Berhasil diotentifikasi.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_POST_SUCCESS = _T("Berhasil Posting.");
const TSTR	LCL_STR_TWITTER_DIALOG_STR_SAMPLE = _T("Posting dari Siglus");

// ****************************************************************
// ローカライズ：ダイアログリソース
// ================================================================

// 共通ダイアログ
const int	LCL_IDD_CFG_WIN_BASE_COMMON = IDD_CFG_WIN_BASE_COMMON_ID;
const int	LCL_IDD_CFG_WIN_SOLO_COMMON = IDD_CFG_WIN_SOLO_COMMON_ID;

// （旧）個別ダイアログ
const int	LCL_IDD_CFG_WIN_BASE = IDD_CFG_WIN_BASE_ID;
//const int	LCL_IDD_CFG_WIN_BASE_AUTOMODE = IDD_CFG_WIN_BASE_AUTOMODE;
//const int	LCL_IDD_CFG_WIN_BASE_CHRKOE = IDD_CFG_WIN_BASE_CHRKOE;
const int	LCL_IDD_CFG_WIN_BASE_ELSE = IDD_CFG_WIN_BASE_ELSE_ID;
//const int	LCL_IDD_CFG_WIN_BASE_FONT_MSGSPD = IDD_CFG_WIN_BASE_FONT_MSGSPD;
const int	LCL_IDD_CFG_WIN_BASE_JITAN = IDD_CFG_WIN_BASE_JITAN_ID;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE = IDD_CFG_WIN_BASE_KOEMODE;
//const int	LCL_IDD_CFG_WIN_BASE_KOEMODE_CHRKOE = IDD_CFG_WIN_BASE_KOEMODE_CHRKOE;
const int	LCL_IDD_CFG_WIN_BASE_MWNDBK = IDD_CFG_WIN_BASE_MWNDBK_ID;
const int	LCL_IDD_CFG_WIN_BASE_SCREEN = IDD_CFG_WIN_BASE_SCREEN_ID;
const int	LCL_IDD_CFG_WIN_BASE_SYSTEM = IDD_CFG_WIN_BASE_SYSTEM_ID;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME = IDD_CFG_WIN_BASE_VOLUME;
//const int	LCL_IDD_CFG_WIN_BASE_VOLUME_BGMFADE = IDD_CFG_WIN_BASE_VOLUME_BGMFADE;
//const int	LCL_IDD_CFG_WIN_SOLO_AUTOMODE = IDD_CFG_WIN_SOLO_AUTOMODE;
//const int	LCL_IDD_CFG_WIN_SOLO_BGMFADE = IDD_CFG_WIN_SOLO_BGMFADE;
//const int	LCL_IDD_CFG_WIN_SOLO_CHRKOE = IDD_CFG_WIN_SOLO_CHRKOE;
const int	LCL_IDD_CFG_WIN_SOLO_ELSE = IDD_CFG_WIN_SOLO_ELSE_ID;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT = IDD_CFG_WIN_SOLO_FONT;
//const int	LCL_IDD_CFG_WIN_SOLO_FONT_MSGSPD = IDD_CFG_WIN_SOLO_FONT_MSGSPD;
const int	LCL_IDD_CFG_WIN_SOLO_JITAN = IDD_CFG_WIN_SOLO_JITAN_ID;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE = IDD_CFG_WIN_SOLO_KOEMODE;
//const int	LCL_IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE = IDD_CFG_WIN_SOLO_KOEMODE_CHRKOE;
//const int	LCL_IDD_CFG_WIN_SOLO_MSGSPD = IDD_CFG_WIN_SOLO_MSGSPD;
const int	LCL_IDD_CFG_WIN_SOLO_MOVTYPE = IDD_CFG_WIN_SOLO_MOVTYPE_ID;
const int	LCL_IDD_CFG_WIN_SOLO_MWNDBK = IDD_CFG_WIN_SOLO_MWNDBK_ID;
const int	LCL_IDD_CFG_WIN_SOLO_SCREEN = IDD_CFG_WIN_SOLO_SCREEN_ID;
const int	LCL_IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE = IDD_CFG_WIN_SOLO_SUPPORT_MOVTYPE;
const int	LCL_IDD_CFG_WIN_SOLO_SYSTEM = IDD_CFG_WIN_SOLO_SYSTEM_ID;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME = IDD_CFG_WIN_SOLO_VOLUME;
//const int	LCL_IDD_CFG_WIN_SOLO_VOLUME_BGMFADE = IDD_CFG_WIN_SOLO_VOLUME_BGMFADE;
const int	LCL_IDD_SYS_WIN_SOLO_SAVELOAD = IDD_SYS_WIN_SOLO_SAVELOAD_ID;
const int	LCL_IDD_CHANGE_RESOLUTION_WARNING = IDD_CHANGE_RESOLUTION_WARNING_ID;
const int	LCL_IDD_TWEET = IDD_TWEET_ID;
