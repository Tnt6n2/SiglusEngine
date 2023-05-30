#pragma		once

const int SCN_ID_NONE = -1;		// �����l

// ****************************************************************
// �v���W�F�N�g�f�[�^
// ================================================================
struct S_tnm_compiler_project_data
{
	TSTR	project_name;
	TSTR	scn_path;
	TSTR	tmp_path;
	TSTR	out_path;
	TSTR	out_path_noangou;
	TSTR	exe_path;
	TSTR	work_path;
	TSTR	editor_path;
	TSTR	manual_path;
	TSTR	build_after;
	TSTR	angou_str;
	TSTR	gameexe_ini;
	TSTR	extend_inc;
	TSTR	gameexe_dat;
	TSTR	scene_pck;
	TSTR	start_scene;

	int		start_z_no;

	bool	start_check;
	bool	angou_check;
	bool	utf8;
};

// ****************************************************************
// �f�[�^
// ================================================================

// �G���[�W�����v�\����
struct S_error_jump
{
	TSTR	detail_file_path;
	TSTR	jump_file_path;
	int		line_no;
};

// �V�[�����
struct S_scn_info
{
	S_scn_info()
	{
		scn_id = SCN_ID_NONE;
	}
	S_scn_info(CTSTR& scn_name_, int scn_id_)
	{
		scn_name = scn_name_;
		scn_id = scn_id_;
	}

	TSTR	scn_name;
	int		scn_id;
};

class C_tnm_compiler_data
{
public:
	C_tnm_compiler_data();

	// �v���W�F�N�g�ݒ�
	ARRAY<S_tnm_compiler_project_data>	project;

	int		cur_project_no;		// ���݂̃v���W�F�N�g�ԍ�
	bool	gui_flag;			// GUI �t���O

	// �R�}���h���C�����
	bool	cl_project_no_set;	// �R�}���h���C�����F�v���W�F�N�g�ԍ��F�ݒ肳��Ă���
	bool	cl_log_set;			// �R�}���h���C�����F�o�͐�F�ݒ肳��Ă���
	bool	cl_full_set;		// �R�}���h���C�����F�t���R���p�C���F�ݒ肳��Ă���
	int		cl_project_no;		// �R�}���h���C�����F�v���W�F�N�g�ԍ�
	TSTR	cl_log;				// �R�}���h���C�����F�o�͐�

	// CUI ���[�h�p�̃��O
	TSTR	cui_full_log;		// CUI ���[�h�̃��O

	// ini ���X�g
	ARRAY<TSTR>	ini_list;

	// inc ���X�g
	ARRAY<TSTR>	inc_list;

	// scn ���X�g
	ARRAY<S_scn_info>	scn_list;

	// �G���[�W�����v���X�g
	ARRAY<S_error_jump>	error_jump_list;
};
