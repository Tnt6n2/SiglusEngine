#pragma		once

// ****************************************************************
// G00 �o�b�t�@�̃C���^�[�t�F�[�X
// ================================================================

// G00 �o�b�t�@�̌����擾����
int tnm_g00_buf_get_g00_buf_cnt();

// G00 �o�b�t�@�Ƀt�@�C����ǂݍ���
void tnm_g00_buf_load_file(C_elm_g00_buf* p_g00_buf, CTSTR& file_name);

// G00 �o�b�t�@���������
void tnm_g00_buf_free_buf(C_elm_g00_buf* p_g00_buf);

// G00 �o�b�t�@��S�ĉ������
void tnm_g00_buf_free_all_buf();

// G00 �o�b�t�@�Ƀt�@�C�����ǂݍ��܂�Ă��邩���擾����
bool tnm_g00_buf_is_load(C_elm_g00_buf* p_g00_buf);

// G00 �o�b�t�@�ɓǂݍ��܂ꂽ�t�@�C���̃t�@�C�������擾����
TSTR tnm_g00_buf_get_file_name(C_elm_g00_buf* p_g00_buf);
