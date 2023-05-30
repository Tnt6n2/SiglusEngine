#pragma		once

struct S_db_edit_param
{
	bool	enable;
	int		min;
	int		max;
	int		column_width;	// コラムの幅
	TCHAR*	param_name;		// ini 保存用の名前（アルファベット・正式名）
	TCHAR*	item_str;		// アイテムに表示用の文字列（日本語ＯＫ）
	TCHAR*	column_str;		// コラムに表示用の文字列（日本語ＯＫ、短めに）
};

// カウンタ
const S_db_edit_param G_counter_edit_param[] =
{
	{ true, INT_MIN, INT_MAX, 50, _T("value"), _T("値"), _T("値") },
	{ false, 0, 0, 50, _T("active"), _T("ｱｸﾃｨﾌﾞ"), _T("ｱｸﾃｨﾌﾞ") },
	{ false, 0, 0, 50, _T("time"), _T("時間"), _T("時間ﾀｲﾌﾟ") },
	{ false, 0, 0, 100, _T("type"), _T("ﾀｲﾌﾟ"), _T("ﾀｲﾌﾟ") },
};

// マスク
const S_db_edit_param G_mask_edit_param[] =
{
	{ false, 0, 0, 100, _T("file"), _T("ﾌｧｲﾙ"), _T("ﾌｧｲﾙ") },
	{ true, INT_MIN, INT_MAX, 50, _T("x"), _T("x"), _T("x") },
	{ true, INT_MIN, INT_MAX, 50, _T("y"), _T("y"), _T("y") },
};

// ステージ
const S_db_edit_param G_stage_edit_param[] =
{
	{ false, 0, 0, 50, _T("object"), _T("object"), _T("オブジェクトの個数") },
	{ false, 0, 0, 50, _T("group"), _T("group"), _T("グループの個数") },
	{ false, 0, 0, 50, _T("mwnd"), _T("mwnd"), _T("ﾒｯｾｰｼﾞｳｨﾝﾄﾞｳの個数") },
	{ false, 0, 0, 50, _T("quake"), _T("quake"), _T("クエイクの個数") },
};

// オブジェクト
const S_db_edit_param G_object_edit_param[] =
{
	{ false, 0, 0, 50, _T("type"), _T("ﾀｲﾌﾟ"), _T("ﾀｲﾌﾟ") },
	{ false, 0, 0, 100, _T("file"), _T("ﾌｧｲﾙ"), _T("ﾌｧｲﾙ") },
	{ false, 0, 0, 50, _T("child"), _T("子供の個数"), _T("子供") },
	{ true, 0, 1, 50, _T("disp"), _T("disp"), _T("disp") },
	{ true, INT_MIN, INT_MAX, 50, _T("patno"), _T("patno"), _T("patno") },
	{ true, INT_MIN, INT_MAX, 50, _T("order"), _T("order"), _T("order") },
	{ true, INT_MIN, INT_MAX, 50, _T("layer"), _T("layer"), _T("layer") },
	{ true, INT_MIN, INT_MAX, 50, _T("world"), _T("world"), _T("world") },
	{ true, INT_MIN, INT_MAX, 50, _T("x"), _T("x"), _T("x") },
	{ true, INT_MIN, INT_MAX, 50, _T("y"), _T("y"), _T("y") },
	{ true, INT_MIN, INT_MAX, 50, _T("z"), _T("z"), _T("z") },
	{ true, INT_MIN, INT_MAX, 100, _T("center_x"), _T("center_x"), _T("center_x") },
	{ true, INT_MIN, INT_MAX, 100, _T("center_y"), _T("center_y"), _T("center_y") },
	{ true, INT_MIN, INT_MAX, 100, _T("center_z"), _T("center_z"), _T("center_z") },
	{ true, INT_MIN, INT_MAX, 100, _T("center_rep_x"), _T("center_rep_x"), _T("center_rep_x") },
	{ true, INT_MIN, INT_MAX, 100, _T("center_rep_y"), _T("center_rep_y"), _T("center_rep_y") },
	{ true, INT_MIN, INT_MAX, 100, _T("center_rep_z"), _T("center_rep_z"), _T("center_rep_z") },
	{ true, INT_MIN, INT_MAX, 100, _T("scale_x"), _T("scale_x"), _T("scale_x") },
	{ true, INT_MIN, INT_MAX, 100, _T("scale_y"), _T("scale_y"), _T("scale_y") },
	{ true, INT_MIN, INT_MAX, 100, _T("scale_z"), _T("scale_z"), _T("scale_z") },
	{ true, INT_MIN, INT_MAX, 100, _T("rotate_x"), _T("rotate_x"), _T("rotate_x") },
	{ true, INT_MIN, INT_MAX, 100, _T("rotate_y"), _T("rotate_y"), _T("rotate_y") },
	{ true, INT_MIN, INT_MAX, 100, _T("rotate_z"), _T("rotate_z"), _T("rotate_z") },
	{ true, 0, 1, 100, _T("clip_use"), _T("clip_use"), _T("clip_use") },
	{ true, INT_MIN, INT_MAX, 100, _T("clip_left"), _T("clip_left"), _T("clip_l") },
	{ true, INT_MIN, INT_MAX, 100, _T("clip_top"), _T("clip_top"), _T("clip_t") },
	{ true, INT_MIN, INT_MAX, 100, _T("clip_right"), _T("clip_right"), _T("clip_r") },
	{ true, INT_MIN, INT_MAX, 100, _T("clip_bottom"), _T("clip_bottom"), _T("clip_b") },
	{ true, 0, 1, 100, _T("src_clip_use"), _T("src_clip_use"), _T("src_clip_use") },
	{ true, INT_MIN, INT_MAX, 100, _T("src_clip_left"), _T("src_clip_left"), _T("src_clip_l") },
	{ true, INT_MIN, INT_MAX, 100, _T("src_clip_top"), _T("src_clip_top"), _T("src_clip_t") },
	{ true, INT_MIN, INT_MAX, 100, _T("src_clip_right"), _T("src_clip_right"), _T("src_clip_r") },
	{ true, INT_MIN, INT_MAX, 100, _T("src_clip_bottom"), _T("src_clip_bottom"), _T("src_clip_b") },
	{ true, INT_MIN, INT_MAX, 50, _T("tr"), _T("tr"), _T("tr") },
	{ true, INT_MIN, INT_MAX, 50, _T("mono"), _T("mono"), _T("mono") },
	{ true, INT_MIN, INT_MAX, 50, _T("reverse"), _T("reverse"), _T("reverse") },
	{ true, INT_MIN, INT_MAX, 50, _T("bright"), _T("bright"), _T("bright") },
	{ true, INT_MIN, INT_MAX, 50, _T("dark"), _T("dark"), _T("dark") },
	{ true, INT_MIN, INT_MAX, 50, _T("color_r"), _T("color_r"), _T("color_r") },
	{ true, INT_MIN, INT_MAX, 50, _T("color_g"), _T("color_g"), _T("color_g") },
	{ true, INT_MIN, INT_MAX, 50, _T("color_b"), _T("color_b"), _T("color_b") },
	{ true, INT_MIN, INT_MAX, 100, _T("color_rate"), _T("color_rate"), _T("color_rate") },
	{ true, INT_MIN, INT_MAX, 100, _T("color_add_r"), _T("color_add_r"), _T("color_add_r") },
	{ true, INT_MIN, INT_MAX, 100, _T("color_add_g"), _T("color_add_g"), _T("color_add_g") },
	{ true, INT_MIN, INT_MAX, 100, _T("color_add_b"), _T("color_add_b"), _T("color_add_b") },
	{ true, INT_MIN, INT_MAX, 100, _T("tonecurve_no"), _T("tonecurve_no"), _T("tonecurve_no") },
	{ true, 0, 1, 100, _T("culling"), _T("culling"), _T("culling") },
	{ true, 0, 1, 100, _T("alpha_test"), _T("alpha_test"), _T("alpha_test") },
	{ true, 0, 1, 100, _T("alpha_blend"), _T("alpha_blend"), _T("alpha_blend") },
	{ true, INT_MIN, INT_MAX, 100, _T("blend_type"), _T("blend_type"), _T("blend_type") },
	{ true, INT_MIN, INT_MAX, 50, _T("btn_no"), _T("btn_no"), _T("btn_no") },
	{ true, INT_MIN, INT_MAX, 100, _T("btn_group_no"), _T("btn_group_no"), _T("btn_group_no") },
	{ true, INT_MIN, INT_MAX, 100, _T("btn_action_no"), _T("btn_action_no"), _T("btn_action_no") },
	{ true, INT_MIN, INT_MAX, 100, _T("btn_se_no"), _T("btn_se_no"), _T("btn_se_no") },
	{ true, 0, 2, 100, _T("btn_state"), _T("btn_state"), _T("btn_state") },
	{ false, 0, 0, 100, _T("btn_call_scn"), _T("btn_call_scn"), _T("btn_call_scn") },
	{ false, 0, 0, 100, _T("btn_call_cmd"), _T("btn_call_cmd"), _T("btn_call_cmd") },
	{ false, 0, 0, 100, _T("btn_call_z_no"), _T("btn_call_z_no"), _T("btn_call_z_no") },
	{ false, 0, 0, 100, _T("frame_action_scn"), _T("frame_action_scn"), _T("frame_action_scn") },
	{ false, 0, 0, 100, _T("frame_action_cmd"), _T("frame_action_cmd"), _T("frame_action_cmd") },
	{ false, 0, 0, 100, _T("frame_action_counter"), _T("frame_action_counter"), _T("frame_action_counter") },
	{ false, 0, 0, 100, _T("movie_time"), _T("movie_time"), _T("movie_time") },
	{ true, 0, 1, 100, _T("wipe_copy"), _T("wipe_copy"), _T("wipe_copy") },
	{ true, 0, 1, 100, _T("wipe_erase"), _T("wipe_erase"), _T("wipe_erase") },
	{ true, 0, 1, 50, _T("hints"), _T("hints"), _T("hints") },
	{ true, INT_MIN, INT_MAX, 100, _T("child_sort"), _T("child_sort"), _T("child_sort") },
};

// グループ
const S_db_edit_param G_group_edit_param[] =
{
	{ false, 0, 0, 50, _T("doing"), _T("選択中"), _T("選択中") },
	{ false, 0, 0, 50, _T("pause"), _T("ポーズ中"), _T("ﾎﾟｰｽﾞ中") },
	{ false, 0, 0, 50, _T("wait"), _T("選択待ち"), _T("選択待ち") },
	{ false, 0, 0, 50, _T("cancel_enable"), _T("キャンセル"), _T("ｷｬﾝｾﾙ") },
	{ false, 0, 0, 50, _T("cancel_se_no"), _T("キャンセル音"), _T("ｷｬﾝｾﾙ音") },
	{ false, 0, 0, 50, _T("hit_no"), _T("当たっているボタン番号"), _T("当") },
	{ false, 0, 0, 50, _T("pushed_no"), _T("押されているボタン番号"), _T("押") },
	{ false, 0, 0, 50, _T("decided_no"), _T("決定されたボタン番号"), _T("決定") },
};

// メッセージウィンドウ
const S_db_edit_param G_mwnd_edit_param[] =
{
	{ false, 0, 0, 100, _T("msg"), _T("ﾒｯｾｰｼﾞ"), _T("ﾒｯｾｰｼﾞ") },
	{ false, INT_MIN, INT_MAX, 50, _T("waku_no"), _T("枠番号"), _T("枠") },
	{ false, INT_MIN, INT_MAX, 50, _T("namae_waku_no"), _T("名前枠番号"), _T("名前枠") },
	{ false, INT_MIN, INT_MAX, 50, _T("clear_ready"), _T("ｸﾘｱ準備"), _T("ｸﾘｱ準備") },
};

// エフェクト
const S_db_edit_param G_effect_edit_param[] =
{
	{ true, INT_MIN, INT_MAX, 50, _T("x"), _T("x"), _T("x") },
	{ true, INT_MIN, INT_MAX, 50, _T("y"), _T("y"), _T("y") },
	{ true, INT_MIN, INT_MAX, 50, _T("z"), _T("z"), _T("z") },
	{ true, INT_MIN, INT_MAX, 50, _T("mono"), _T("mono"), _T("mono") },
	{ true, INT_MIN, INT_MAX, 50, _T("reverse"), _T("reverse"), _T("reverse") },
	{ true, INT_MIN, INT_MAX, 50, _T("bright"), _T("bright"), _T("bright") },
	{ true, INT_MIN, INT_MAX, 50, _T("dark"), _T("dark"), _T("dark") },
	{ true, INT_MIN, INT_MAX, 50, _T("color_r"), _T("color_r"), _T("color_r") },
	{ true, INT_MIN, INT_MAX, 50, _T("color_g"), _T("color_g"), _T("color_g") },
	{ true, INT_MIN, INT_MAX, 50, _T("color_b"), _T("color_b"), _T("color_b") },
	{ true, INT_MIN, INT_MAX, 100, _T("color_rate"), _T("color_rate"), _T("color_rate") },
	{ true, INT_MIN, INT_MAX, 100, _T("color_add_r"), _T("color_add_r"), _T("color_add_r") },
	{ true, INT_MIN, INT_MAX, 100, _T("color_add_g"), _T("color_add_g"), _T("color_add_g") },
	{ true, INT_MIN, INT_MAX, 100, _T("color_add_b"), _T("color_add_b"), _T("color_add_b") },
	{ true, INT_MIN, INT_MAX, 50, _T("begin_order"), _T("begin_order"), _T("begin_order") },
	{ true, INT_MIN, INT_MAX, 50, _T("begin_layer"), _T("begin_layer"), _T("begin_layer") },
	{ true, INT_MIN, INT_MAX, 50, _T("end_order"), _T("end_order"), _T("end_order") },
	{ true, INT_MIN, INT_MAX, 50, _T("end_layer"), _T("end_layer"), _T("end_layer") },
	{ true, 0, 1, 100, _T("wipe_copy"), _T("wipe_copy"), _T("wipe_copy") },
	{ true, 0, 1, 100, _T("wipe_erase"), _T("wipe_erase"), _T("wipe_erase") },
};

// ワールド
const S_db_edit_param G_world_edit_param[] = 
{
	{ true, INT_MIN, INT_MAX, 50, _T("camera_eye_x"), _T("camera_eye_x"), _T("eye_x") },
	{ true, INT_MIN, INT_MAX, 50, _T("camera_eye_y"), _T("camera_eye_y"), _T("eye_y") },
	{ true, INT_MIN, INT_MAX, 50, _T("camera_eye_z"), _T("camera_eye_z"), _T("eye_z") },
	{ true, INT_MIN, INT_MAX, 50, _T("camera_pint_x"), _T("camera_pint_x"), _T("pint_x") },
	{ true, INT_MIN, INT_MAX, 50, _T("camera_pint_y"), _T("camera_pint_y"), _T("pint_y") },
	{ true, INT_MIN, INT_MAX, 50, _T("camera_pint_z"), _T("camera_pint_z"), _T("pint_z") },
	{ true, INT_MIN, INT_MAX, 50, _T("camera_up_x"), _T("camera_up_x"), _T("up_x") },
	{ true, INT_MIN, INT_MAX, 50, _T("camera_up_y"), _T("camera_up_y"), _T("up_y") },
	{ true, INT_MIN, INT_MAX, 50, _T("camera_up_z"), _T("camera_up_z"), _T("up_z") },
	{ true, INT_MIN, INT_MAX, 50, _T("camera_view_angle"), _T("camera_view_angle"), _T("view_angle") },
	{ true, INT_MIN, INT_MAX, 50, _T("order"), _T("order"), _T("order") },
	{ true, INT_MIN, INT_MAX, 50, _T("layer"), _T("layer"), _T("layer") },
	{ true, 0, 1, 100, _T("wipe_copy"), _T("wipe_copy"), _T("wipe_copy") },
	{ true, 0, 1, 100, _T("wipe_erase"), _T("wipe_erase"), _T("wipe_erase") },
};

