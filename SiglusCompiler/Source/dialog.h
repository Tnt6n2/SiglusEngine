#pragma		once

#include	"data.h"

// ****************************************************************
// メインダイアログ：ini ビュー
// ================================================================
class C_tnm_compiler_ini_lv : public Cc_listview
{
protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
};

// ****************************************************************
// メインダイアログ：inc ビュー
// ================================================================
class C_tnm_compiler_inc_lv : public Cc_listview
{
protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
};

// ****************************************************************
// メインダイアログ：scn ビュー
// ================================================================
class C_tnm_compiler_scn_lv : public Cc_listview
{
protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
};

// ****************************************************************
// メインダイアログ：ログビュー
// ================================================================
class C_tnm_compiler_log_lv : public Cc_listview
{
protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
};

// ****************************************************************
// メインダイアログ
// ================================================================
class C_tnm_compiler_dialog : public C_dialog
{
public:

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_combobox		cmb_project;
		Cc_button		btn_project_config;
		Cc_button		btn_new_project;
		Cc_button		btn_del_project;
		Cc_check		chk_debug_on;
		Cc_check		chk_debug_off;
		Cc_check		chk_execute;
		Cc_check		chk_start_scene;
		Cc_edit			edt_start_scene;
		Cc_edit			edt_start_z_no;
		Cc_spin			spn_start_z_no;
		Cc_button		btn_all_sel_all;
		Cc_button		btn_find;
		Cc_button		btn_sel_auto;
		Cc_button		btn_compile;
		Cc_button		btn_run;
		Cc_button		btn_all_sel_auto;
		Cc_button		btn_manual;
		Cc_edit			edt_detail1;
		Cc_edit			edt_detail2;
		Cc_static		stc_version;
		Cc_check		chk_test;
		Cc_check		chk_no_interval;
		Cc_check		chk_easy_link;
		Cc_check		chk_overflow_error_off;
		Cc_button		btn_explain_under;

		// リストビュー
		C_tnm_compiler_ini_lv	lv_ini;
		C_tnm_compiler_inc_lv	lv_inc;
		C_tnm_compiler_scn_lv	lv_scn;
		C_tnm_compiler_log_lv	lv_log;

		// ダイアログセパレータ
		C_dialog_separater		dlgsep00;
		C_dialog_separater		dlgsep01;
		C_dialog_separater		dlgsep02;

	}	ci;

public:
	void		on_lv_log_dblclk(int row_no);
	void		set_enable_control(bool enable);
	void		set_detail1_text(CTSTR& text);
	void		set_detail2_text(CTSTR& text);

private:
	// ウィンドウプロシージャ
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	bool		on_init_dialog();
	bool		on_close();
	bool		on_command(WPARAM wp, LPARAM lp);
	bool		on_notify(WPARAM wp, LPARAM lp);
	bool		on_initmenu(WPARAM wp, LPARAM lp);
};
