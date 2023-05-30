#pragma		once

#include	"IA.h"
#include	"CA.h"
#include	"LA.h"
#include	"SA.h"

// �R���p�C���F���C��
class	C_tnm_compiler_main;
extern	C_tnm_compiler_main*	G_main;

// �R���p�C���F�_�C�A���O
class	C_tnm_compiler_dialog;
extern	C_tnm_compiler_dialog*	G_dlg;

// �R���p�C���F�f�[�^
class	C_tnm_compiler_data;
extern	C_tnm_compiler_data*	G_data;

// �X�N���v�g�F�R���p�C���f�[�^
struct	C_tnms_scn_compile_data
{
	int				lv_item_no;
	TSTR			scn_name;
	S_tnms_ia_data	ia_data;
	S_tnms_ca_data	ca_data;
	S_tnms_la_data	la_data;
	S_tnms_sa_data	sa_data;
};

// �v���W�F�N�g�ݒ胂�[�h
enum	PROJECT_SETTING_MODE
{
	PROJECT_SETTING_MODE_SETTING,
	PROJECT_SETTING_MODE_CREATE,
	PROJECT_SETTING_MODE_DELETE,
};


// ****************************************************************
// ���C��
// ================================================================
class C_tnm_compiler_main
{
public:
	// �R���X�g���N�^
	C_tnm_compiler_main();									// �R���X�g���N�^

	// ����
	void	save_ini();										// ini �Z�[�u
	void	load_ini();										// ini ���[�h
	void	get_project_item_state();						// �v���W�F�N�g�֘A�̃A�C�e����Ԃ��擾
	void	set_project_item_state();						// �v���W�F�N�g�֘A�̃A�C�e����Ԃ�ݒ�
	void	update_dialog_for_window_title();				// �E�B���h�E�^�C�g���� ss �̃p�X��\��
	void	update_dialog_for_all_btn(bool is_cancel);		// �S���{�^���i�Č����������I��or�S�I�����r���h�����s�j�i�Č����������I�����r���h�����s�j
	void	update_dialog_for_start_item();					// �X�^�[�g�֘A�̃A�C�e��
	void	update_dialog();								// �_�C�A���O���X�V
	bool	update_scn_list();								// �V�[�����X�g���X�V

	// ����
	void	change_project(int no);							// �v���W�F�N�g��ύX
	bool	on_setting_project();							// �v���W�F�N�g�ݒ�
	bool	on_create_project();							// �v���W�F�N�g�쐬�i�V�K�v���W�F�N�g�j
	bool	on_delete_project();							// �v���W�F�N�g�폜
	bool	on_command_find();								// �Č���
	bool	on_command_sel_all();							// �S�I��
	bool	on_command_unsel_all();							// �S����
	bool	on_command_sel_auto();							// �����I��
	bool	on_command_build();								// �r���h
	bool	on_command_run();								// ���s
	bool	on_command_all(bool is_sel_auto);				// �S���i�Č����������I��or�S�I�����r���h�����s�j
	bool	on_command_manual();							// �}�j���A��
	bool	start_auto_process();							// ������������s
	bool	explain_under();								// �E���ɂ��Ă̐���

	// ���O����
	void	set_log(CTSTR& text);							// ���O��ݒ�
	void	set_log_jump(CTSTR& text, CTSTR& detail_file_path, CTSTR& jump_file_path, int line_no);

	// �r���[����J��
	void	open_ini_by_ini_lv(int row_no);					// ini �r���[���� ini ���J��
	void	open_inc_by_inc_lv(int row_no);					// inc �r���[���� inc ���J��
	void	open_scn_by_scn_lv(int row_no);					// scn �r���[���� scn ���J��
	void	open_scn_by_log_lv(int row_no);					// ���O�r���[���� scn ���J��

	// �t�H���_���J��
	bool	open_folder(CTSTR& path);						// �t�H���_�I�[�v��
	bool	open_folder_from_scn_path();					// scn �p�X�̃t�H���_���J��
	bool	open_folder_from_tmp_path();					// tmp �p�X�̃t�H���_���J��
	bool	open_folder_from_out_path();					// out �p�X�̃t�H���_���J��
	bool	open_folder_from_exe_path();					// exe �p�X�̃t�H���_���J��
	bool	open_folder_from_work_path();					// work �p�X�̃t�H���_���J��
	bool	open_folder_from_editor_path();					// editor �p�X�̃t�H���_���J��
	bool	open_folder_from_manual_path();					// manual �p�X�̃t�H���_���J��

	// �|�b�v�A�b�v���j���[����
	void	popup_menu_proc(CTSTR& start_scene_str = _T(""));

	// �G���[�̏o���s�̏ڍׂ��X�e�[�^�X�o�[�ɕ\��
	void	show_err_detail(int row_no);					// �G���[�̏o���s�̏ڍׂ��X�e�[�^�X�o�[�ɕ\��

	// �v���W�F�N�g���I������Ă��邩�ǂ������肷��
	bool	check_current_project(bool is_warning);			// �v���W�F�N�g���I������Ă��邩�ǂ������肷��

	// �Í��֘A
	bool	load_exe_angou_dat(TSTR& angou_str);			// �Í�.dat �̓ǂݍ���
	void	manual_exe_angou();								// �Í��ɂ��Ă̐���
	void	func_exe_angou();								// �d�w�d���Í�������

private:

	// ����̑O��
	bool	on_command_befor_proc(int mod);					// ����̑O����
	bool	on_command_after_proc(int mod);					// ����̌㏈��

	// ����
	bool	set_project(PROJECT_SETTING_MODE proc_mode);				// �v���W�F�N�g�ݒ�
	bool	proc_find();												// �Č���
	bool	proc_sel_all(bool mod);										// �S�I���^�S����
	bool	proc_sel_auto();											// �����I��
	bool	proc_build();												// �r���h
	bool	proc_build_after();											// �r���h��̏���
	bool	proc_run(CTSTR& start_scene_str = _T(""), int z_no = 0);	// ���s
	bool	proc_all(bool is_sel_auto);									// �S���i�Č����������I��or�S�I�����r���h�����s�j

	// �����F�r���h�֘A�FGameexe.ini ���
	bool	proc_gei();										// Gameexe.ini ���

	// �����F�r���h�֘A�Finc �t�@�C�����
	bool	proc_inc(S_tnms_ia_data* piad);					// inc �t�@�C�����

	// �����F�r���h�֘A�F�R���p�C��
	struct COMPILE_FUNK_STRUCT
	{
		TSTR	scn_dir_path;
		TSTR	tmp_dir_path;
		TSTR	scn_file_path;
		TSTR	scn_file_name;
		TSTR	ca_file_path;
		bool	utf8;
		bool	test_check;
	};
	bool	proc_compile(S_tnms_ia_data* piad, int* p_error_cnt_ret);															// �R���p�C���{��
	bool	proc_compile_funk_ca_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data, CTSTR scn_text);	// �R���p�C���F�������
	bool	proc_compile_funk_la_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data);					// �R���p�C���F������
	bool	proc_compile_funk_sa_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data);					// �R���p�C���F�\�����
	bool	proc_compile_funk_ma_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data);					// �R���p�C���F�Ӗ����
	bool	proc_compile_funk_hankaku_param_format(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data);			// �R���p�C���F���p������p�����[�^�̕ϊ��i���G���[�͂��肦�Ȃ��̂ŃG���[�����͂܂������l���Ă��܂���j

	// �����F�r���h�֘A�F�����N
	bool	proc_link(S_tnms_ia_data* piad);			// �����N

	// �v���Z�X�t���O
	bool	proc_execut_flag;		// �v���Z�X���s���t���O
	bool	proc_cancel_flag;		// �v���Z�X�L�����Z���t���O

	// �V�[���R���p�C���f�[�^
};
