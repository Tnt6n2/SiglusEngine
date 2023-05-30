#pragma		once

#include	"data/tnm_def.h"
#include	"dialog/db_wnd_info.h"

// ****************************************************************
// デバッグ情報ウィンドウ：エラー
// ================================================================
class C_db_wnd_info_error : public C_db_wnd_info
{
public:

	void		init();		// 初期化
	void		frame();	// フレーム

private:

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		C_tnm_dbg_listview	lv;
		Cc_check			chk_only_error;
	}	ci;

	// ステータス
	struct STATE
	{
		int		error_cnt;
	}	m_ld;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド

	void		clear_lv();
	void		add_lv_item(TNM_ERROR_TYPE error_type, CTSTR& error_str);
};

