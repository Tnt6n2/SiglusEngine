#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_icon.h"
#include	"engine/eng_system.h"
#include	"../resource.h"

// ****************************************************************
// ちはやベンチ：情報ダイアログ
// ================================================================
class C_info_dialog : public C_modal_dialog
{
public:
	int			create(CTSTR& option_str, PTR dialog_id, CTSTR& name, bool limit_min, HWND parent = NULL);

	Cc_edit		edt_info;
	Cc_button	btn_clipboard;
	Cc_button	btn_close;

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	bool		on_init_dialog();
	bool		on_close();
	bool		on_command(WPARAM wp, LPARAM lp);

private:
	TSTR		m_option_str;
};

int C_info_dialog::create(CTSTR& option_str, PTR dialog_id, CTSTR& name, bool limit_min, HWND parent)
{
	m_option_str = option_str;

	return C_modal_dialog::create(dialog_id, name, limit_min, parent);
}

LRESULT C_info_dialog::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init_dialog())		return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
	}

	return C_modal_dialog::window_proc(msg, wp, lp);
}

bool C_info_dialog::on_init_dialog()
{
	set_text(Gp_ini->game_name);
	set_icon(Gp_icon->get_large_icon());
	set_small_icon(Gp_icon->get_small_icon());

	bind_control(&edt_info, IDC_EDT_INFO, _T("edt_info"), 5);
	bind_control(&btn_clipboard, IDC_BTN_CLIPBOARD, _T("btn_clipboard"), 2);
	bind_control(&btn_close, IDC_BTN_CLOSE, _T("btn_close"), 3);

	// オプション文字列を改行で分割する
	ARRAY<TSTR> str_list;
	TSTR::iterator prev_itr = m_option_str.begin();
	for (TSTR::iterator itr = m_option_str.begin(); itr != m_option_str.end(); ++itr)	{
		if (*itr == _T('\n'))	{
			str_list.push_back(TSTR(prev_itr, itr));
			prev_itr = itr + 1;
		}
	}
	str_list.push_back(TSTR(prev_itr, m_option_str.end()));

	TSTR total_text;
	for (ARRAY<TSTR>::iterator itr = str_list.begin(); itr != str_list.end(); ++itr)	{
		total_text += *itr + _T("\n");
	}
	edt_info.set_text(total_text);

	set_focus();	// フォーカスを当てる

	return true;
}

bool C_info_dialog::on_close()
{
	destroy(IDCANCEL);

	// デフォルトの動作を実行しない！
	return false;
}

bool C_info_dialog::on_command(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);

	switch (ctrl_id)	{
		case IDC_BTN_CLIPBOARD:
			str_copy_to_clip_board_in_MB(m_option_str);
			message_box(_T("情報をクリップボードにコピーしました。\nメモ帳などのエディタで「貼り付け」または「ペースト」で情報を貼り付けることができます。"), Gp_ini->game_name, MB_OK);
			set_active();	// アクティブにする（これがなかったら親がアクティブになってしまう）
			return false;

		case IDC_BTN_CLOSE:		destroy(IDCANCEL);	return false;
		case IDCANCEL:			destroy(IDCANCEL);	return false;	// ★Esc キーを押すとここを通る
	}

	return true;
}

// ****************************************************************
// ちはやベンチ：ダイアログを表示する
// ================================================================
bool tnm_open_chihaya_bench_dialog(CTSTR& str)
{
	C_info_dialog dlg;
	dlg.create(str, IDD_SYS_WIN_SOLO_INFO, _T("sys_win_info_solo"), false, G_app.h_wnd);

	return true;
}

// ****************************************************************
// ちはやベンチ：スペック情報を表示する
// ================================================================
TSTR tnm_get_spec_info_for_chihaya_bench()
{
	TSTR tmp_str;
	TSTR result;

	// OS 名
	tnm_get_os_name(tmp_str);
	result += _T("OS = ") + tmp_str;

	// CPU 名
	result += _T("\nCPU = ") + tnm_get_cpu_name();

	// ビデオカード名
	if (!Gp_system_info->display_list.empty())
		result += _T("\nビデオカード = ") + MBSTR_to_TSTR(Gp_system_info->display_list[0].Description);

	return result;
}