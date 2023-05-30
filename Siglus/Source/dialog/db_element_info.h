#pragma		once

struct S_db_edit_param
{
	bool	enable;
	int		min;
	int		max;
	int		column_width;	// �R�����̕�
	TCHAR*	param_name;		// ini �ۑ��p�̖��O�i�A���t�@�x�b�g�E�������j
	TCHAR*	item_str;		// �A�C�e���ɕ\���p�̕�����i���{��n�j�j
	TCHAR*	column_str;		// �R�����ɕ\���p�̕�����i���{��n�j�A�Z�߂Ɂj
};

// �J�E���^
const S_db_edit_param G_counter_edit_param[] =
{
	{ true, INT_MIN, INT_MAX, 50, _T("value"), _T("�l"), _T("�l") },
	{ false, 0, 0, 50, _T("active"), _T("��è��"), _T("��è��") },
	{ false, 0, 0, 50, _T("time"), _T("����"), _T("��������") },
	{ false, 0, 0, 100, _T("type"), _T("����"), _T("����") },
};

// �}�X�N
const S_db_edit_param G_mask_edit_param[] =
{
	{ false, 0, 0, 100, _T("file"), _T("̧��"), _T("̧��") },
	{ true, INT_MIN, INT_MAX, 50, _T("x"), _T("x"), _T("x") },
	{ true, INT_MIN, INT_MAX, 50, _T("y"), _T("y"), _T("y") },
};

// �X�e�[�W
const S_db_edit_param G_stage_edit_param[] =
{
	{ false, 0, 0, 50, _T("object"), _T("object"), _T("�I�u�W�F�N�g�̌�") },
	{ false, 0, 0, 50, _T("group"), _T("group"), _T("�O���[�v�̌�") },
	{ false, 0, 0, 50, _T("mwnd"), _T("mwnd"), _T("ү���޳���޳�̌�") },
	{ false, 0, 0, 50, _T("quake"), _T("quake"), _T("�N�G�C�N�̌�") },
};

// �I�u�W�F�N�g
const S_db_edit_param G_object_edit_param[] =
{
	{ false, 0, 0, 50, _T("type"), _T("����"), _T("����") },
	{ false, 0, 0, 100, _T("file"), _T("̧��"), _T("̧��") },
	{ false, 0, 0, 50, _T("child"), _T("�q���̌�"), _T("�q��") },
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

// �O���[�v
const S_db_edit_param G_group_edit_param[] =
{
	{ false, 0, 0, 50, _T("doing"), _T("�I��"), _T("�I��") },
	{ false, 0, 0, 50, _T("pause"), _T("�|�[�Y��"), _T("�߰�ޒ�") },
	{ false, 0, 0, 50, _T("wait"), _T("�I��҂�"), _T("�I��҂�") },
	{ false, 0, 0, 50, _T("cancel_enable"), _T("�L�����Z��"), _T("��ݾ�") },
	{ false, 0, 0, 50, _T("cancel_se_no"), _T("�L�����Z����"), _T("��ݾى�") },
	{ false, 0, 0, 50, _T("hit_no"), _T("�������Ă���{�^���ԍ�"), _T("��") },
	{ false, 0, 0, 50, _T("pushed_no"), _T("������Ă���{�^���ԍ�"), _T("��") },
	{ false, 0, 0, 50, _T("decided_no"), _T("���肳�ꂽ�{�^���ԍ�"), _T("����") },
};

// ���b�Z�[�W�E�B���h�E
const S_db_edit_param G_mwnd_edit_param[] =
{
	{ false, 0, 0, 100, _T("msg"), _T("ү����"), _T("ү����") },
	{ false, INT_MIN, INT_MAX, 50, _T("waku_no"), _T("�g�ԍ�"), _T("�g") },
	{ false, INT_MIN, INT_MAX, 50, _T("namae_waku_no"), _T("���O�g�ԍ�"), _T("���O�g") },
	{ false, INT_MIN, INT_MAX, 50, _T("clear_ready"), _T("�ر����"), _T("�ر����") },
};

// �G�t�F�N�g
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

// ���[���h
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

