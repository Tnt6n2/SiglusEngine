#pragma		once

// ****************************************************************
// �G���W���F���͊֌W�̊֐�
// ================================================================

// �}�E�X�̈ʒu���擾
C_point tnm_input_get_mouse_pos();

// �L�[��������Ă��邩�𔻒�
bool tnm_input_is_key_down(int key);

// �L�[��������Ă��邩�𔻒�
bool tnm_input_is_key_up(int key);

// �L�[�̉����𔻒�
bool tnm_input_check_key_down(int key);

// �L�[�̕����𔻒�
bool tnm_input_check_key_up(int key);

// �L�[�̉��������𔻒�
bool tnm_input_check_key_down_up(int key);

// �L�[�̉������g�p
bool tnm_input_use_key_down(int key);

// �L�[�̕������g�p
bool tnm_input_use_key_up(int key);

// �L�[�̉����������g�p
bool tnm_input_use_key_down_up(int key);
