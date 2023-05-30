
// プロセス定義
// 番号を飛ばしては行けません！配列のインデックスがずれてしまいます。

PROC(NONE,					0,  false)		// 何もしない
PROC(SCRIPT,				1,  false)		// スクリプト実行中
PROC(DISP,					2,  false)		// 画面を更新する
PROC(GAME_START_WIPE,		3,  false)		// ゲーム開始時のワイプ
PROC(GAME_END_WIPE,			4,  false)		// ゲーム終了時のワイプ
PROC(GAME_TIMER_START,		5,  false)		// ゲームタイマー開始
PROC(LOAD,					6,  false)		// ロード
PROC(QUICK_LOAD,			7,  false)		// クイックロード
PROC(RETURN_TO_MENU,		8,  false)		// メニューに戻る
PROC(RETURN_TO_SEL,			9,  false)		// 前の選択肢に戻る
PROC(DUMMY_10,				10, false)		// （欠番）
PROC(DUMMY_11,				11, false)		// （欠番）
PROC(DUMMY_12,				12, false)		// （欠番）
PROC(DUMMY_13,				13, false)		// （欠番）
PROC(SEL,					14, false)		// 選択肢
PROC(SEL_BTN,				15, false)		// ボタン選択肢
PROC(SEL_BTN_OBJ,			16, false)		// ボタンオブジェクト選択肢
PROC(MSG_BACK,				17, false)		// メッセージバック
PROC(COMMAND,				18, false)		// コマンドを実行する
PROC(KEY_WAIT,				19, true)		// キー待ち
PROC(TIME_WAIT,				20, true)		// 時間待ち
PROC(COUNT_WAIT,			21, true)		// カウンタ待ち
PROC(MWND_OPEN_WAIT,		22, true)		// メッセージウィンドウ開きアニメ待ち
PROC(MWND_CLOSE_WAIT,		23, true)		// メッセージウィンドウ閉じアニメ待ち
PROC(MWND_CLOSE_WAIT_ALL,	24, true)		// メッセージウィンドウ閉じアニメ待ち
PROC(MESSAGE_WAIT,			25, false)		// メッセージ表示終了待ち（特殊：メッセージ表示開始時にクリアする）
PROC(MESSAGE_KEY_WAIT,		26, true)		// メッセージキー待ち（キー待ちアイコンを表示する）
PROC(BGM_WAIT,				27, true)		// ＢＧＭ待ち
PROC(BGM_FADE_OUT_WAIT,		28, true)		// ＢＧＭフェードアウト待ち
PROC(KOE_WAIT,				29, true)		// 声待ち
PROC(PCM_WAIT,				30, true)		// 効果音待ち
PROC(PCMCH_WAIT,			31, true)		// 効果音チャンネル待ち
PROC(PCMCH_FADE_OUT_WAIT,	32, true)		// 効果音チャンネルフェードアウト待ち
PROC(PCM_EVENT_WAIT,		33, true)		// 効果音イベント待ち
PROC(MOV_WAIT,				34, true)		// ムービー待ち
PROC(WIPE_WAIT,				35, true)		// ワイプ待ち
PROC(SHAKE_WAIT,			36, true)		// シェイク待ち
PROC(QUAKE_WAIT,			37, true)		// クエイク待ち
PROC(EVENT_WAIT,			38, true)		// イベント待ち
PROC(ALL_EVENT_WAIT,		39, true)		// 全体イベント待ち
PROC(OBJ_MOV_WAIT,			40, true)		// ムービーオブジェクト待ち
PROC(SAVE_DIALOG,			41, false)		// セーブダイアログ
PROC(END_LOAD,				42, false)		// エンドロード
PROC(END_SAVE,				43, false)		// エンドセーブ
PROC(END_GAME,				44, false)		// ゲーム終了
PROC(START_WARNING,			45, false)		// 起動直後警告
PROC(CAPTURE_ONLY,			46, false)		// キャプチャー
PROC(INNER_LOAD,			47, false)		// インナーロード
PROC(EASY_LOAD,				48, false)		// 簡易ロード
PROC(BACKLOG_LOAD,			49, false)		// バックログロード
PROC(TWEET_CAPTURE_AFTER,   50, false)		// ツイート用のキャプチャーの後
PROC(OBJ_EMOTE_WAIT,		51, true)		// オブジェクト E-mote 待ち
PROC(MAX, 52, false)		// 
