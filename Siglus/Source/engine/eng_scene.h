#pragma		once

#include	"data/tnm_proc.h"

// ****************************************************************
// �V�[������
// ================================================================

struct	S_tid;
class	C_tnm_save_stream;
struct	S_tnm_local_save;
class	C_elm_call;

// �V�[���̏����擾

// ���݂̃v���Z�X���擾
C_tnm_proc* tnm_get_cur_proc();

// ���݂̃R�[�����擾
C_elm_call* tnm_get_cur_call();

// ���݂̃R�[�������擾
int tnm_get_cur_call_cnt();

// Scene.pck ��ǂݒ���
bool	tnm_reload_scene_pck();

// �V�[���J�ڏ���
bool	tnm_scene_proc_gosub(int label_no, int ret_form_code);		// ���x���R�[��
bool	tnm_scene_proc_jump(int scn_no, int z_no);					// �V�[���W�����v�i�V�[���ԍ����w��j
bool	tnm_scene_proc_jump(CTSTR& scn_name, int z_no);				// �V�[���W�����v�i�V�[�������w��j
bool	tnm_scene_proc_call(int scn_no, int sub_no);				// �T�u���[�`���R�[���i�T�u���[�`���ԍ����w��j
bool	tnm_scene_proc_call(int scn_no, CTSTR& sub_name);			// �T�u���[�`���R�[���i�T�u���[�`�������w��j
bool	tnm_scene_proc_farcall(int scn_no, int z_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag);							// �V�[���R�[���i�V�[���ԍ����w��j
bool	tnm_scene_proc_farcall(CTSTR& scn_name, int z_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag);					// �V�[���R�[���i�V�[�������w��j
bool	tnm_scene_proc_call_user_cmd(int scn_no, int user_cmd_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag);			// ���[�U�R�}���h�R�[���i���[�U�R�}���h�ԍ����w��j
bool	tnm_scene_proc_call_user_cmd(int scn_no, CTSTR& user_cmd_name, int ret_form_code, bool ex_call_flag, bool frame_action_flag);		// ���[�U�R�}���h�R�[���i���[�U�R�}���h�����w��j
bool	tnm_scene_proc_call_user_cmd(CTSTR& scn_name, CTSTR& user_cmd_name, int ret_form_code, bool ex_call_flag, bool frame_action_flag);	// ���[�U�R�}���h�R�[���i�V�[�������w��j
bool	tnm_scene_proc_return();									// ���^�[��

// �R�[���X�^�b�N����
void	tnm_scene_init_call_stack();
void	tnm_scene_del_call_stack(int cnt);
void	tnm_scene_set_call_stack_cnt(int cnt);
int		tnm_scene_get_call_stack_cnt();

bool	tnm_scene_proc_start_from_start_scene();					// �X�^�[�g�V�[������X�^�[�g
bool	tnm_scene_proc_restart_from_start_scene();					// �X�^�[�g�V�[�����烊�X�^�[�g
bool	tnm_scene_proc_restart_from_menu_scene();					// ���j���[�V�[�����烊�X�^�[�g
bool	tnm_scene_proc_restart(CTSTR& scn_name, int z_no);			// �w��̃V�[�����烊�X�^�[�g�i�V�[�������w��j
bool	tnm_scene_proc_restart(int scn_no, int z_no);				// �w��̃V�[�����烊�X�^�[�g�i�V�[���ԍ����w��j

bool	tnm_scene_proc_call_syscom_scene();							// �V�X�e���R�}���h�V�[�����Ă�
bool	tnm_scene_proc_call_flick_scene(int no);					// �t���b�N�V�[�����Ă�
bool	tnm_scene_proc_call_save_scene();							// �Z�[�u�V�[�����Ă�
bool	tnm_scene_proc_call_load_scene();							// ���[�h�V�[�����Ă�
bool	tnm_scene_proc_call_config_scene();							// ���ݒ�V�[�����Ă�

// �Z�[�u���[�h����
bool	tnm_saveload_proc_save(int save_no);									// �Z�[�u
bool	tnm_saveload_proc_load(int save_no);									// ���[�h
bool	tnm_saveload_proc_quick_save(int save_no);								// �N�C�b�N�Z�[�u
bool	tnm_saveload_proc_quick_load(int save_no);								// �N�C�b�N���[�h
bool	tnm_saveload_proc_end_save();											// �G���h�Z�[�u
bool	tnm_saveload_proc_end_load();											// �G���h���[�h
bool	tnm_saveload_proc_backlog_load(S_tid save_id);							// �o�b�N���O���[�h
bool	tnm_saveload_proc_load_save_history(S_tnm_local_save* p_local_save);	// �Z�[�u�|�C���g�������烍�[�h����
bool	tnm_saveload_proc_return_to_sel();										// �O�̑I�����ɖ߂�

bool	tnm_saveload_proc_create_inner_save(int inner_save_no);					// �C���i�[�Z�[�u���쐬����
bool	tnm_saveload_proc_load_inner_save(int inner_save_no);					// �C���i�[�Z�[�u�����[�h����
bool	tnm_saveload_proc_clear_inner_save(int inner_save_no);					// �C���i�[�Z�[�u���N���A����
bool	tnm_saveload_proc_copy_inner_save(int from, int to);					// �C���i�[�Z�[�u���R�s�[����
bool	tnm_saveload_proc_check_inner_save(int inner_save_no);					// �C���i�[�Z�[�u�𔻒肷��

// �Z�[�u�|�C���g
void	tnm_set_save_point();					// �Z�[�u�|�C���g�𗧂Ă�
void	tnm_clear_save_point();					// �Z�[�u�|�C���g���N���A����
bool	tnm_check_save_point();					// �Z�[�u�|�C���g�𔻒肷��

// �I�����|�C���g
void	tnm_set_sel_point();					// �I�����|�C���g�𗧂Ă�
void	tnm_clear_sel_point();					// �I�����|�C���g���N���A����
bool	tnm_check_sel_point();					// �I�����|�C���g�𔻒肷��
void	tnm_stack_sel_point();					// �I�����|�C���g��ς�
void	tnm_drop_sel_point();					// �I�����|�C���g���~�낷

// �V�[�����G�f�B�^�ŊJ��
bool	tnm_open_by_editor(bool open_dialog);
bool	tnm_open_koe(bool open_dialog);
