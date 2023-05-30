#pragma		once

// ****************************************************************
// コマンドライン
// ================================================================
class C_tnm_cmd_line
{
public:
	C_tnm_cmd_line();

	bool	analize_command_line();

	bool	select_scene_pck_set;
	TSTR	select_scene_pck;
	bool	start_scene_set;
	TSTR	start_scene;
	bool	start_z_no_set;
	int		start_z_no;
	bool	debug_on_set;
	bool	debug_off_set;
	TSTR	debug_password;
	TSTR	debug_key;
	bool	work_dir_set;
	TSTR	work_dir;
	bool	no_interval_set;
	bool	end_start_set;
	bool	log_set;
	bool	window_mode_set;
	bool	fullscreen_mode_set;
	bool	not_disp_overflow_error_set;
	bool	multi_user_id_set;
	TSTR	multi_user_id;
	bool	double_start_set;
};
