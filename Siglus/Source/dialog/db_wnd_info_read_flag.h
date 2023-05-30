#pragma		once

#include	"db_wnd_info.h"

class	C_tnm_scene_lexer;

// ****************************************************************
// デバッグ情報ウィンドウ：既読フラグ
// ================================================================
class C_db_wnd_info_read_flag : public C_db_wnd_info
{
public:
	void		init();					// 初期化
	void		load_state();			// ステータス読み込み
	void		save_state();			// ステータス保存
	void		frame();				// フレーム処理

private:

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		C_tnm_dbg_listview	lv_scn;
		C_tnm_dbg_listview	lv_read_flag;
		Cc_button			btn_find;
		Cc_static			stc_status;
		C_dialog_separater	separater;
	}	ci;

	// ステータス
	struct STATE
	{
		struct SCENE
		{
			ARRAY<BYTE>	read_flag;
			int			flag_cnt;
			int			read_cnt;
		};

		ARRAY<SCENE>	scn_data;
		ARRAY<BYTE>		read_flag;
		int				cur_scn_no;
		TSTR			status;
	}	m_ld, m_cd;

	// 作業変数
	bool		m_first;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知

	void		create_read_flag_view();
};

