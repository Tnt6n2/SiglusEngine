#pragma		once

// ****************************************************************
// ダイアログ
// ================================================================
class C_tnm_dlg : public C_dialog
{
public:

	struct REGIST_STATE
	{
		TSTR		window_name;			// 登録ウィンドウ名
		bool		limit_min;				// ウィンドウの最小矩形リミット
		bool		attach;					// ウィンドウアタッチ
		int			dialog_id;				// ダイアログＩＤ
	}	regist_state;

	bool		initial_open_check();		// 初期オープン判定
	void		initial_open();				// 初期オープン
	void		save_ini_open_state();		// オープン状態をＩＮＩに保存

	// ヴァーチャル関数
	virtual		void		init() = 0;		// 初期化
	virtual		void		free();			// 解放
	virtual		void		open(HWND parent_h_wnd = G_app.h_wnd);		// 開く（作成）
	virtual		void		load_state();	// ステータス読み込み
	virtual		void		save_state();	// ステータス保存
	virtual		void		frame();		// フレーム

	bool		kill_focus_for_radio_item();			// ラジオボタンからフォーカスを外す
	void		regist(CTSTR window_name, bool limit_min, bool attach, int dialog_id);		// 登録

protected:

	void		window_proc_pre(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);		// ウィンドウプロシージャ
};

