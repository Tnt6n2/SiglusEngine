#pragma		once

#include	"db_wnd_info.h"

// ****************************************************************
// デバッグ情報ウィンドウ：シーンコントロール
// ================================================================
class C_db_wnd_info_scn_control : public C_db_wnd_info
{

public:

	void		init();					// 初期化
	void		load_state();			// ステータス読み込み
	void		save_state();			// ステータス保存

	void		start_up();

	TSTR		get_scn_name();			// シーン名取得
	int			get_scn_no();			// シーン番号取得
	int			get_z_no();				// Ｚ番号取得

private:

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_button			btn_restart;
		Cc_button			btn_select_start;
		C_tnm_dbg_listbox	lst_scn_name;
		C_tnm_dbg_listbox	lst_z_no;
		C_dialog_separater	spa_v00;
	}	ci;

	// ステータス
	struct STATE
	{
		TSTR	scn_name;
		int		z_no;
	}	m_state;

	ARRAY<int>	m_z_no_list;		// Ｚ番号リスト



	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド

	void		make_scn_name_list();							// シーン名リスト作成
	void		make_z_no_list();								// Ｚ番号リスト作成
	int			get_item_no_from_z_no(int z_no);				// 実際のＺ番号からＺ番号リストのアイテム番号を取得

};

