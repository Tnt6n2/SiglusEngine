#pragma		once

struct	S_tnm_save_header;
struct	S_tnm_local_save;


// ****************************************************************
// �Z�[�u���[�h����
// ================================================================

bool	tnm_save_local();						// �Z�[�u�i���[�J���j
bool	tnm_save_local_msg(CTSTR& msg);			// �Z�[�u�i���[�J���̌��݂̕��́j
bool	tnm_load_local();						// ���[�h�i���[�J���j
bool	tnm_save_call();						// �Z�[�u�i�R�[���j
bool	tnm_load_call();						// ���[�h�i�R�[���j

// �Z�[�u�f�[�^�V�X�e���F���[�J���Z�[�u
void	tnm_init_local_save();								// ���[�J���Z�[�u�f�[�^��������
void	tnm_set_local_save(S_tnm_local_save* p_local_save);	// ���[�J���Z�[�u�f�[�^��ݒ�
bool	tnm_is_exist_local_save();							// ���[�J���Z�[�u�f�[�^�𔻒�

// �Z�[�u�f�[�^�V�X�e���F�I�����Z�[�u
void	tnm_init_sel_save();								// �I�����Z�[�u�f�[�^��������
void	tnm_get_sel_save(S_tnm_local_save* p_local_save);	// �I�����Z�[�u�f�[�^���擾
bool	tnm_is_exist_sel_save();							// �I�����Z�[�u�f�[�^�𔻒�
void	tnm_copy_local_save_to_sel_save();					// ���[�J���Z�[�u�f�[�^��I�����Z�[�u�f�[�^�ɃR�s�[

// �Z�[�u�f�[�^�V�X�e���F�I�����Z�[�u�X�g�b�N
void	tnm_init_sel_save_stock();							// �I�����Z�[�u�f�[�^��������
void	tnm_copy_local_save_to_sel_save_stock();			// ���[�J���Z�[�u�f�[�^��I�����Z�[�u�f�[�^�ɃR�s�[
void	tnm_copy_sel_save_stock_to_sel_save();				// �I�����Z�[�u�f�[�^���R�s�[

// �Z�[�u�f�[�^�V�X�e���F�C���i�[�Z�[�u
void	tnm_savedata_system_clear_inner_save(int inner_save_no);
void	tnm_savedata_system_copy_local_save_to_inner_save(int inner_save_no);
void	tnm_savedata_system_copy_inner_save(int inner_save_no_from, int inner_save_no_to);
void	tnm_savedata_system_get_inner_save(int inner_save_no, S_tnm_local_save* p_local_save);
bool	tnm_savedata_system_check_inner_save(int inner_save_no);
int		tnm_savedata_system_get_inner_save_size(int inner_save_no);

// �Z�[�u�f�[�^�V�X�e���F�o�b�N���O�Z�[�u
bool	tnm_savedata_system_check_backlog_save(int save_index);

// �Z�[�u�f�[�^�V�X�e���F�Z�[�u����
void	tnm_init_save_history();				// �Z�[�u������������

// �Z�[�u�f�[�^�V�X�e���F���b�Z�[�W�o�b�N�Z�[�u
void	tnm_init_backlog_save();					// ���b�Z�[�W�o�b�N�Z�[�u��������
bool	tnm_check_backlog_save(S_tid save_id);		// ���b�Z�[�W�o�b�N�Z�[�u�̑��ݔ���
void	tnm_remove_backlog_save();					// ���b�Z�[�W�o�b�N�𐮗�

// �t�@�C���h�n
bool	tnm_load_system_on_file();				// �V�X�e���ݒ�f�[�^��ǂݍ���
bool	tnm_save_global_on_file();				// �O���[�o���f�[�^���t�@�C���ɃZ�[�u
bool	tnm_load_global_on_file();				// �O���[�o���f�[�^���t�@�C�����烍�[�h
bool	tnm_save_local_on_file(int save_no);	// ���[�J���f�[�^���t�@�C���ɃZ�[�u
bool	tnm_load_local_on_file(int save_no);	// ���[�J���f�[�^���t�@�C�����烍�[�h
bool	tnm_save_save_header(int save_no, S_tnm_save_header* p_header);		// �Z�[�u�w�b�_���Z�[�u
bool	tnm_load_save_header(int save_no, S_tnm_save_header* p_header);		// �Z�[�u�w�b�_�����[�h
int		tnm_get_new_save_no();							// �ŐV�̃Z�[�u�f�[�^�ԍ����擾����
int		tnm_get_new_save_no(int start, int cnt);		// �ŐV�̃Z�[�u�f�[�^�ԍ����擾����
int		tnm_get_new_quick_save_no();					// �ŐV�̃N�C�b�N�Z�[�u�f�[�^�ԍ����擾����
int		tnm_get_new_quick_save_no(int start, int cnt);	// �ŐV�̃N�C�b�N�Z�[�u�f�[�^�ԍ����擾����

bool	tnm_is_save_enable();					// �Z�[�u�\���ǂ�������
bool	tnm_is_save_enable(int save_no);		// �Z�[�u�\���ǂ�������i�ԍ��w��j
bool	tnm_is_load_enable();					// ���[�h�\���ǂ�������
bool	tnm_is_load_enable(int save_no);		// ���[�h�\���ǂ�������i�ԍ��w��j

bool	tnm_check_save_file(int save_no);						// �Z�[�u�f�[�^�̑��ݔ���
bool	tnm_copy_save_file(int save_no_from, int save_no_to);	// �Z�[�u�f�[�^���R�s�[����
bool	tnm_delete_save_file(int save_no);						// �Z�[�u�f�[�^���폜����
bool	tnm_change_save_file(int save_no_0, int save_no_1);		// �Z�[�u�f�[�^����������

