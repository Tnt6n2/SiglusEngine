#pragma		once

// ****************************************************************
// �a�f�l�e�[�u���̃C���^�[�t�F�[�X
// ================================================================

// BGM �e�[�u���� BGM �̌����擾
int tnm_bgm_table_get_bgm_cnt();

// BGM �e�[�u���̊����t���O��ݒ肷��
void tnm_bgm_table_set_listened(int bgm_no, bool value);

// BGM �e�[�u���̊����t���O��ݒ肷��
void tnm_bgm_table_set_listened(CTSTR& bgm_name, bool value);

// BGM �e�[�u���̊����t���O��S�Đݒ肷��
void tnm_bgm_table_set_all_flag(bool value);

// BGM �e�[�u���̊����t���O���擾����
bool tnm_bgm_table_get_listened(int bgm_no);

// BGM �e�[�u���̊����t���O���擾����i-1: �o�^����Ă��Ȃ��j
int tnm_bgm_table_get_listened(CTSTR& bgm_name);

