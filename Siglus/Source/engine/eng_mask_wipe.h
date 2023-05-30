#pragma		once

// ****************************************************************
// �}�X�N�o�b�t�@�\����
// ================================================================
struct S_mask_buffer
{
	BUFFER p;
	int w;
	int h;
};

// �N���b�s���O
bool tnm_clip_line(int *x1, int *y1, int *x2, int *y2, int ex1, int ey1, int ex2, int ey2);

// �R�Q�r�b�g���C���`��
void tnm_draw_32bit_line(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2);
// �R�Q�r�b�g�G���v�e�B�[�{�b�N�X�`��
void tnm_draw_32bit_empty_box(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2);
// �R�Q�r�b�g�t�B���{�b�N�X�`��
void tnm_draw_32bit_fill_box(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2);
// �R�Q�r�b�g�t�B���|���S���`��
void tnm_draw_32bit_fillpolygon(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, int x3, int y3, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2);
// �R�Q�r�b�g��]�g��k���`��
void tnm_draw_32bit_mask_rotate_scale(BYTE *dst, int dst_w, int dst_h, BYTE *src, int src_w, int src_h, int src_center_x, int src_center_y, int dst_x, int dst_y, int rotate, double dbl_scale_x, double dbl_scale_y, bool is_high_quality);


// �}�X�N�o�b�t�@�m��
bool tnm_alloc_mask_buffer(S_mask_buffer *buf, int mask_w, int mask_h, BYTE **dp, int *ex1, int *ey1, int *ex2, int *ey2);
// �p�^�[�����B
bool tnm_duplicate_mask(S_mask_buffer *dst_buf, int dst_w, int dst_h, S_mask_buffer *src_buf);
// �}�X�N�e�N�X�`���쐬
bool tnm_make_mask_texture(BSP<C_d3d_texture> *texture, S_mask_buffer *buf);
// �}�X�N�e�N�X�`���쐬�i�p�^�[�����B�Łj
bool tnm_make_mask_texture_with_duplicate(BSP<C_d3d_texture> *texture, int target_w, int target_h, S_mask_buffer *buf);


// �}�X�N�e�N�X�`���쐬�F�S�~�S�̂��킾���X�p�^�[��
bool tnm_make_mask_texture_for_4x4_jiwa9(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h);
// �}�X�N�e�N�X�`���쐬�F�S�~�S�̂��킾���V�p�^�[��
bool tnm_make_mask_texture_for_4x4_jiwa7(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h);
// �}�X�N�e�N�X�`���쐬�F�S�~�S�̉��悤��
bool tnm_make_mask_texture_for_4x4_turn_around(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);
// �}�X�N�e�N�X�`���쐬�F�S�~�S�̉��s���悤��
bool tnm_make_mask_texture_for_4x4_turn_ret(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);
// �}�X�N�e�N�X�`���쐬�F�S�~�S�̐܂�Ԃ��悤��
bool tnm_make_mask_texture_for_4x4_turn_down(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);


// �}�X�N�e�N�X�`���쐬�F�W�~�W�̉��悤��
bool tnm_make_mask_texture_for_8x8_turn_around(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);
// �}�X�N�e�N�X�`���쐬�F�W�~�W�̉��s���悤��
bool tnm_make_mask_texture_for_8x8_turn_ret(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);
// �}�X�N�e�N�X�`���쐬�F�W�~�W�̐܂�Ԃ��悤��
bool tnm_make_mask_texture_for_8x8_turn_down(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);


// �}�X�N�e�N�X�`���쐬�F�����_��
bool tnm_make_mask_texture_for_random(BSP<C_d3d_texture> *texture, int target_w, int target_h, int pat_w, int pat_h);
// �}�X�N�e�N�X�`���쐬�F�����_�����C��
bool tnm_make_mask_texture_for_random_line(BSP<C_d3d_texture> *texture, int target_w, int target_h, int dir, int line_len);
// �}�X�N�e�N�X�`���쐬�F�����_���΂߃��C��
bool tnm_make_mask_texture_for_random_slant_line(BSP<C_d3d_texture> *texture, int target_w, int target_h, int dir, int line_len);
// �}�X�N�e�N�X�`���쐬�F�����_���N���X
bool tnm_make_mask_texture_for_random_cross(BSP<C_d3d_texture> *texture, int target_w, int target_h, int pat_w, int pat_h);
// �}�X�N�e�N�X�`���쐬�F�����_���΂߃N���X
bool tnm_make_mask_texture_for_random_slant_cross(BSP<C_d3d_texture> *texture, int target_w, int target_h, int line_len);


// �}�X�N�e�N�X�`���쐬�F�P��
bool tnm_make_32bit_mask_for_around_one(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir);
bool tnm_make_mask_texture_for_around_one(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir);
// �}�X�N�e�N�X�`���쐬�F����
bool tnm_make_32bit_mask_for_around_half(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir);
bool tnm_make_mask_texture_for_around_half(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir);
// �}�X�N�e�N�X�`���쐬�F������
bool tnm_make_32bit_mask_for_around_divide(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int divide_mod);
bool tnm_make_mask_texture_for_around_divide(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int divide_mod);
// �}�X�N�e�N�X�`���쐬�F���S��
bool tnm_make_32bit_mask_for_oogi_center(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse);
bool tnm_make_mask_texture_for_oogi_center(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);
// �}�X�N�e�N�X�`���쐬�F�p��
bool tnm_make_32bit_mask_for_oogi_corner(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, bool is_distant);
bool tnm_make_mask_texture_for_oogi_corner(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, bool is_distant);
// �}�X�N�e�N�X�`���쐬�F����
bool tnm_make_32bit_mask_for_oogi_edge(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int oogi_mod, bool is_distant);
bool tnm_make_mask_texture_for_oogi_edge(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int oogi_mod, bool is_distant);


// �}�X�N�e�N�X�`���쐬�F�l�p�`
bool tnm_make_32bit_mask_for_square(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse);
bool tnm_make_mask_texture_for_square(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);
// �}�X�N�e�N�X�`���쐬�F�H�`
bool tnm_make_32bit_mask_for_rhombus(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse);
bool tnm_make_mask_texture_for_rhombus(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);
// �}�X�N�e�N�X�`���쐬�F�\��
bool tnm_make_32bit_mask_for_jyuuji(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int center_x, int center_y);
bool tnm_make_mask_texture_for_jyuuji(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);
// �}�X�N�e�N�X�`���쐬�F�e���r
bool tnm_make_32bit_mask_for_television(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse);
bool tnm_make_mask_texture_for_television(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);


// �}�X�N�e�N�X�`���쐬�F�w���������
bool tnm_make_32bit_mask_for_direction(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir);
bool tnm_make_mask_texture_for_direction(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir);
// �}�X�N�e�N�X�`���쐬�F�w���������X���C�X
bool tnm_make_32bit_mask_for_direction_slice(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int slice_len);
bool tnm_make_mask_texture_for_direction_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int slice_len);
// �}�X�N�e�N�X�`���쐬�F�w���������u���C���h
bool tnm_make_mask_texture_for_direction_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len);

// �}�X�N�e�N�X�`���쐬�F�w��̎΂ߕ�������
bool tnm_make_mask_texture_for_direction_slant(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir);
// �}�X�N�e�N�X�`���쐬�F�w���������u���b�N�u���C���h
bool tnm_make_mask_texture_for_direction_block_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len, int block_len);


// �}�X�N�e�N�X�`���쐬�F����������
bool tnm_make_mask_texture_for_bothdir(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir);
// �}�X�N�e�N�X�`���쐬�F����������X���C�X
bool tnm_make_mask_texture_for_bothdir_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int slice_len);
// �}�X�N�e�N�X�`���쐬�F����������u���C���h
bool tnm_make_32bit_mask_for_bothdir_blind_func(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int blind_len);
bool tnm_make_mask_texture_for_bothdir_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len);

// �}�X�N�e�N�X�`���쐬�F����������X�g���C�v
bool tnm_make_mask_texture_for_bothdir_stripe(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int stripe_len);
// �}�X�N�e�N�X�`���쐬�F����������X�g���C�v�Q
bool tnm_make_mask_texture_for_bothdir_stripe2(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int stripe_len);
// �}�X�N�e�N�X�`���쐬�F����������u���b�N�u���C���h
bool tnm_make_mask_texture_for_bothdir_block_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len, int block_len);


// �}�X�N�e�N�X�`���쐬�F�S��������
bool tnm_make_32bit_mask_for_crossdir(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse);
bool tnm_make_mask_texture_for_crossdir(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);
// �}�X�N�e�N�X�`���쐬�F�S��������X���C�X
bool tnm_make_32bit_mask_for_crossdir_slice(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int slice_len);
bool tnm_make_mask_texture_for_crossdir_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int slice_len);
// �}�X�N�e�N�X�`���쐬�F�S��������u���C���h
bool tnm_make_32bit_mask_for_crossdir_blind(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int blind_len);
bool tnm_make_mask_texture_for_crossdir_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int blind_len);

// �����炵�W�J�R�s�[
bool tnm_make_mask_texture_for_gap(S_mask_buffer *buf, int pat_w, int pat_h, S_mask_buffer *tmp);
// �}�X�N�e�N�X�`���쐬�F����`�̂P��
bool tnm_make_mask_texture_for_srect_around_one(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir);
// �}�X�N�e�N�X�`���쐬�F����`�̔���
bool tnm_make_mask_texture_for_srect_around_half(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir);
// �}�X�N�e�N�X�`���쐬�F����`�̕�����
bool tnm_make_mask_texture_for_srect_around_divide(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int divide_mod);
// �}�X�N�e�N�X�`���쐬�F����`�̒��S��
bool tnm_make_mask_texture_for_srect_oogi_center(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod);
// �}�X�N�e�N�X�`���쐬�F����`�̊p��
bool tnm_make_mask_texture_for_srect_oogi_corner(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir, bool is_distant);
// �}�X�N�e�N�X�`���쐬�F����`�̉���
bool tnm_make_mask_texture_for_srect_oogi_edge(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir, int oogi_mod, bool is_distant);


// �}�X�N�e�N�X�`���쐬�F����`�̎l�p�`
bool tnm_make_mask_texture_for_srect_square(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod);
// �}�X�N�e�N�X�`���쐬�F����`�̕H�`
bool tnm_make_mask_texture_for_srect_rhombus(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod);
// �}�X�N�e�N�X�`���쐬�F����`�̏\��
bool tnm_make_mask_texture_for_srect_jyuuji(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod);
// �}�X�N�e�N�X�`���쐬�F����`�̃e���r
bool tnm_make_mask_texture_for_srect_television(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod);


// �}�X�N�e�N�X�`���쐬�F����`�̂S��������
bool tnm_make_mask_texture_for_srect_crossdir(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h);
// �}�X�N�e�N�X�`���쐬�F����`�̂S��������X���C�X
bool tnm_make_mask_texture_for_srect_crossdir_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int slice_len);
// �}�X�N�e�N�X�`���쐬�F����`�̂S��������u���C���h
bool tnm_make_mask_texture_for_srect_crossdir_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int blind_len);

// �}�X�N�e�N�X�`���쐬�F�X�g���C�v�i���}�X�N���C�v�p����Ȃ��j
bool tnm_make_mask_texture_for_stripe(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int stripe_len);

// �}�X�N�e�N�X�`���쐬�F�}�X�N�t�@�C�����g��
bool tnm_make_mask_texture_from_file(BSP<C_d3d_texture> *texture, BSP<C_dib_chip> *dib, int target_w, int target_h, CTSTR& mask_file);
