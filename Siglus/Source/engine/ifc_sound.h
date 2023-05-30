#pragma		once

// ****************************************************************
// �T�E���h�F�C���^�[�t�F�[�X
// ================================================================

// BGM �̌����擾
int tnm_bgm_bgm_cnt();

// BGM �ԍ����� BGM �o�^�����擾
TSTR tnm_get_regist_name_by_bgm_no(int bgm_no);

// BGM �o�^������ BGM �ԍ����擾
int tnm_get_bgm_no_by_regist_name(CTSTR& bgm_regist_name);

// BGM ���Ȃ��Ă��邩�𔻒�
bool tnm_check_bgm_playing();

// �����Đ�����
void tnm_play_koe(int koe_no, int chara_no, int jitan_rate, int play_pos_mills, bool wait_flag, bool key_skip_flag, bool return_value_flag, bool ex_koe_flag, bool check_only_flag);

// �����~����
void tnm_stop_koe();

// SE ���Đ�����
void tnm_play_se(int se_no);

// ���[�r�[�����������𔻒�
bool tnm_is_movie_readying();

// ���[�r�[���Đ������𔻒�
bool tnm_is_movie_playing();

// ���[�r�[����ǂ݂��Ă݂�
bool tnm_preload_omv(CTSTR& file_name);

// �T�E���h�̃{�����[�����X�V����
bool tnm_update_sound_volume(bool change_force);
