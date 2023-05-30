#pragma		once

#include	"main.h"

// ****************************************************************
// メインダイアログ
// ================================================================
class C_tnm_project_setting_dialog : public C_modal_dialog
{
public:
	bool		open(HWND parent_h_wnd, PROJECT_SETTING_MODE proc_mode);	// 開く（作成）

private:

	class Cc_stc_dragdrop : public Cc_static
	{
	public:
		C_tnm_project_setting_dialog* p_parent_class;
	private:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	};

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_edit		edt_project_name;
		Cc_edit		edt_scn_path;
		Cc_edit		edt_extend_scn_name;
		Cc_edit		edt_tmp_path;
		Cc_edit		edt_out_path;
		Cc_edit		edt_out_path_noangou;
		Cc_edit		edt_exe_path;
		Cc_edit		edt_work_path;
		Cc_edit		edt_editor_path;
		Cc_edit		edt_manual_path;

		Cc_edit		edt_gameexe_ini;
		Cc_edit		edt_extend_inc;
		Cc_edit		edt_gameexe_dat;
		Cc_edit		edt_scene_pck;

		Cc_stc_dragdrop	stc_dragdrop;
		Cc_check	chk_utf8;
		Cc_button	btn_ok;
		Cc_button	btn_cancel;
	}	ci;

	struct WORK
	{
		PROJECT_SETTING_MODE	proc_mode;		// 処理モード
		bool					return_value;	// 戻り値
	}	work;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init(WPARAM wp, LPARAM lp);					// 初期化処理
	bool		on_close(WPARAM wp, LPARAM lp);					// クローズ処理
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド処理

	void		set_path_easy(TSTR path);						// ドラッグアンドドロップで一括パス設定
	void		set_path_easy_for_exe_path(TSTR path);			// ドラッグアンドドロップで一括パス設定（ＥＸＥパス用）
};
