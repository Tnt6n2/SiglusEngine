#pragma		once

class	C_elm_mwnd;
class	C_elm_btn_select;

struct S_tnm_sel_param
{
	TSTR	str;
	int		type;
	int		color;
};

// メッセージウィンドウ処理
C_elm_mwnd*	tnm_get_cur_mwnd();
C_elm_mwnd*	tnm_get_cur_sel_mwnd();

// メッセージ処理
void		tnm_msg_proc_start_msg_block(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_start_msg_pp_block(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_koe(C_elm_mwnd* p_mwnd, int koe_no, int chara_no, int rf_scn_no, int rf_flag_no, bool wait_flag, bool key_skip_flag, bool no_auto_mode_flag, bool return_value_flag);
void		tnm_msg_proc_face(C_elm_mwnd* p_mwnd, int face_no, CTSTR& file_path);
void		tnm_msg_proc_name(C_elm_mwnd* p_mwnd, TSTR name);
void		tnm_msg_proc_print(C_elm_mwnd* p_mwnd, TSTR msg, int read_flag_scn_no, int read_flag_flag_no, bool msg_back_flag);
void		tnm_msg_proc_msg_back_add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no);
void		tnm_msg_proc_over_flow_msg(C_elm_mwnd* p_mwnd, CTSTR& msg, CTSTR& over_flow_msg, int read_flag_scn_no, int read_flag_flag_no);
void		tnm_msg_proc_ruby(C_elm_mwnd* p_mwnd, TSTR ruby);
void		tnm_msg_proc_end_ruby(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_sel(C_elm_mwnd* p_mwnd, ARRAY<S_tnm_sel_param>& sel_list, int read_flag_scn_no, int read_flag_flag_no, bool cancel_enable, bool close_mwnd);
void		tnm_msg_proc_selbtn_ready(C_elm_btn_select* p_btn_sel, int template_no, ARRAY<S_tnm_sel_param>& sel_list, bool cancel_enable, bool capture_flag, CTSTR& sel_start_call_scn, int sel_start_call_z_no);
void		tnm_msg_proc_selbtn_start(C_elm_btn_select* p_btn_sel, int read_flag_scn_no, int read_flag_flag_no, int sync_type);
void		tnm_msg_proc_clear_ready(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_clear_ready_novel(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_clear(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_close(C_elm_mwnd* p_mwnd, bool wait_flag);
void		tnm_msg_proc_close_all(bool wait_flag);
void		tnm_msg_proc_command_msg_wait(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_command_pp(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_command_r(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_command_page(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_command_nl(C_elm_mwnd* p_mwnd);
void		tnm_msg_proc_command_nli(C_elm_mwnd* p_mwnd);
//void		tnm_msg_proc_add_msgbk_msg(CTSTR& msg);
//void		tnm_msg_proc_add_msgbk_name(CTSTR& name);
//void		tnm_msg_proc_add_msgbk_koe(int koe_no, int chara_no, int scn_no, int line_no);
//void		tnm_msg_proc_add_msgbk_pct(CTSTR& file_name, int x, int y);

// メッセージ処理
bool		tnm_msg_proc_expand_name_flag(TSTR::const_iterator& itr_ret, TSTR::const_iterator end, int* moji_type_ret, int* moji_code_ret);
