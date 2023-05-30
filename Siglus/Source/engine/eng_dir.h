#pragma		once

extern enum TNM_PCT_FILE_TYPE;

// ƒtƒ@ƒCƒ‹‚ð‚³‚ª‚·
TSTR tnm_find_dat(CTSTR& file_name);
TSTR tnm_find_g00(CTSTR& base_dir, CTSTR& sub_dir, CTSTR& file_name, int* p_ret_type, bool find_append);
TSTR tnm_find_g00(CTSTR& file_name, int* p_ret_type);
TSTR tnm_find_gan(CTSTR& file_name);
TSTR tnm_find_x(CTSTR& file_name);
TSTR tnm_find_wav(CTSTR& base_dir, CTSTR& sub_dir, CTSTR& file_name, int* p_ret_type);
TSTR tnm_find_koe(CTSTR& base_dir, CTSTR& sub_dir, int koe_no, int* p_ret_type);
TSTR tnm_find_mov(CTSTR& base_dir, CTSTR& sub_dir, CTSTR& file_name, int* p_ret_type);
TSTR tnm_find_omv(CTSTR& file_name);
TSTR tnm_find_psb(CTSTR& file_name);


