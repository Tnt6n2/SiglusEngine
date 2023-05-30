#pragma		once

// ****************************************************************
// マスクバッファ構造体
// ================================================================
struct S_mask_buffer
{
	BUFFER p;
	int w;
	int h;
};

// クリッピング
bool tnm_clip_line(int *x1, int *y1, int *x2, int *y2, int ex1, int ey1, int ex2, int ey2);

// ３２ビットライン描画
void tnm_draw_32bit_line(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2);
// ３２ビットエンプティーボックス描画
void tnm_draw_32bit_empty_box(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2);
// ３２ビットフィルボックス描画
void tnm_draw_32bit_fill_box(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2);
// ３２ビットフィルポリゴン描画
void tnm_draw_32bit_fillpolygon(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, int x3, int y3, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2);
// ３２ビット回転拡大縮小描画
void tnm_draw_32bit_mask_rotate_scale(BYTE *dst, int dst_w, int dst_h, BYTE *src, int src_w, int src_h, int src_center_x, int src_center_y, int dst_x, int dst_y, int rotate, double dbl_scale_x, double dbl_scale_y, bool is_high_quality);


// マスクバッファ確保
bool tnm_alloc_mask_buffer(S_mask_buffer *buf, int mask_w, int mask_h, BYTE **dp, int *ex1, int *ey1, int *ex2, int *ey2);
// パターン増殖
bool tnm_duplicate_mask(S_mask_buffer *dst_buf, int dst_w, int dst_h, S_mask_buffer *src_buf);
// マスクテクスチャ作成
bool tnm_make_mask_texture(BSP<C_d3d_texture> *texture, S_mask_buffer *buf);
// マスクテクスチャ作成（パターン増殖版）
bool tnm_make_mask_texture_with_duplicate(BSP<C_d3d_texture> *texture, int target_w, int target_h, S_mask_buffer *buf);


// マスクテクスチャ作成：４×４のじわだし９パターン
bool tnm_make_mask_texture_for_4x4_jiwa9(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h);
// マスクテクスチャ作成：４×４のじわだし７パターン
bool tnm_make_mask_texture_for_4x4_jiwa7(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h);
// マスクテクスチャ作成：４×４の回るように
bool tnm_make_mask_texture_for_4x4_turn_around(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);
// マスクテクスチャ作成：４×４の改行すように
bool tnm_make_mask_texture_for_4x4_turn_ret(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);
// マスクテクスチャ作成：４×４の折り返すように
bool tnm_make_mask_texture_for_4x4_turn_down(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);


// マスクテクスチャ作成：８×８の回るように
bool tnm_make_mask_texture_for_8x8_turn_around(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);
// マスクテクスチャ作成：８×８の改行すように
bool tnm_make_mask_texture_for_8x8_turn_ret(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);
// マスクテクスチャ作成：８×８の折り返すように
bool tnm_make_mask_texture_for_8x8_turn_down(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod);


// マスクテクスチャ作成：ランダム
bool tnm_make_mask_texture_for_random(BSP<C_d3d_texture> *texture, int target_w, int target_h, int pat_w, int pat_h);
// マスクテクスチャ作成：ランダムライン
bool tnm_make_mask_texture_for_random_line(BSP<C_d3d_texture> *texture, int target_w, int target_h, int dir, int line_len);
// マスクテクスチャ作成：ランダム斜めライン
bool tnm_make_mask_texture_for_random_slant_line(BSP<C_d3d_texture> *texture, int target_w, int target_h, int dir, int line_len);
// マスクテクスチャ作成：ランダムクロス
bool tnm_make_mask_texture_for_random_cross(BSP<C_d3d_texture> *texture, int target_w, int target_h, int pat_w, int pat_h);
// マスクテクスチャ作成：ランダム斜めクロス
bool tnm_make_mask_texture_for_random_slant_cross(BSP<C_d3d_texture> *texture, int target_w, int target_h, int line_len);


// マスクテクスチャ作成：１周
bool tnm_make_32bit_mask_for_around_one(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir);
bool tnm_make_mask_texture_for_around_one(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir);
// マスクテクスチャ作成：半周
bool tnm_make_32bit_mask_for_around_half(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir);
bool tnm_make_mask_texture_for_around_half(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir);
// マスクテクスチャ作成：分割周
bool tnm_make_32bit_mask_for_around_divide(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int divide_mod);
bool tnm_make_mask_texture_for_around_divide(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int divide_mod);
// マスクテクスチャ作成：中心扇
bool tnm_make_32bit_mask_for_oogi_center(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse);
bool tnm_make_mask_texture_for_oogi_center(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);
// マスクテクスチャ作成：角扇
bool tnm_make_32bit_mask_for_oogi_corner(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, bool is_distant);
bool tnm_make_mask_texture_for_oogi_corner(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, bool is_distant);
// マスクテクスチャ作成：縁扇
bool tnm_make_32bit_mask_for_oogi_edge(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int oogi_mod, bool is_distant);
bool tnm_make_mask_texture_for_oogi_edge(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int oogi_mod, bool is_distant);


// マスクテクスチャ作成：四角形
bool tnm_make_32bit_mask_for_square(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse);
bool tnm_make_mask_texture_for_square(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);
// マスクテクスチャ作成：菱形
bool tnm_make_32bit_mask_for_rhombus(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse);
bool tnm_make_mask_texture_for_rhombus(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);
// マスクテクスチャ作成：十字
bool tnm_make_32bit_mask_for_jyuuji(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int center_x, int center_y);
bool tnm_make_mask_texture_for_jyuuji(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);
// マスクテクスチャ作成：テレビ
bool tnm_make_32bit_mask_for_television(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse);
bool tnm_make_mask_texture_for_television(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);


// マスクテクスチャ作成：指定方向から
bool tnm_make_32bit_mask_for_direction(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir);
bool tnm_make_mask_texture_for_direction(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir);
// マスクテクスチャ作成：指定方向からスライス
bool tnm_make_32bit_mask_for_direction_slice(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int slice_len);
bool tnm_make_mask_texture_for_direction_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int slice_len);
// マスクテクスチャ作成：指定方向からブラインド
bool tnm_make_mask_texture_for_direction_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len);

// マスクテクスチャ作成：指定の斜め方向から
bool tnm_make_mask_texture_for_direction_slant(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir);
// マスクテクスチャ作成：指定方向からブロックブラインド
bool tnm_make_mask_texture_for_direction_block_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len, int block_len);


// マスクテクスチャ作成：両方向から
bool tnm_make_mask_texture_for_bothdir(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir);
// マスクテクスチャ作成：両方向からスライス
bool tnm_make_mask_texture_for_bothdir_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int slice_len);
// マスクテクスチャ作成：両方向からブラインド
bool tnm_make_32bit_mask_for_bothdir_blind_func(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int blind_len);
bool tnm_make_mask_texture_for_bothdir_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len);

// マスクテクスチャ作成：両方向からストライプ
bool tnm_make_mask_texture_for_bothdir_stripe(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int stripe_len);
// マスクテクスチャ作成：両方向からストライプ２
bool tnm_make_mask_texture_for_bothdir_stripe2(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int stripe_len);
// マスクテクスチャ作成：両方向からブロックブラインド
bool tnm_make_mask_texture_for_bothdir_block_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len, int block_len);


// マスクテクスチャ作成：４方向から
bool tnm_make_32bit_mask_for_crossdir(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse);
bool tnm_make_mask_texture_for_crossdir(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse);
// マスクテクスチャ作成：４方向からスライス
bool tnm_make_32bit_mask_for_crossdir_slice(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int slice_len);
bool tnm_make_mask_texture_for_crossdir_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int slice_len);
// マスクテクスチャ作成：４方向からブラインド
bool tnm_make_32bit_mask_for_crossdir_blind(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int blind_len);
bool tnm_make_mask_texture_for_crossdir_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int blind_len);

// 半ずらし展開コピー
bool tnm_make_mask_texture_for_gap(S_mask_buffer *buf, int pat_w, int pat_h, S_mask_buffer *tmp);
// マスクテクスチャ作成：小矩形の１周
bool tnm_make_mask_texture_for_srect_around_one(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir);
// マスクテクスチャ作成：小矩形の半周
bool tnm_make_mask_texture_for_srect_around_half(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir);
// マスクテクスチャ作成：小矩形の分割周
bool tnm_make_mask_texture_for_srect_around_divide(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int divide_mod);
// マスクテクスチャ作成：小矩形の中心扇
bool tnm_make_mask_texture_for_srect_oogi_center(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod);
// マスクテクスチャ作成：小矩形の角扇
bool tnm_make_mask_texture_for_srect_oogi_corner(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir, bool is_distant);
// マスクテクスチャ作成：小矩形の縁扇
bool tnm_make_mask_texture_for_srect_oogi_edge(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir, int oogi_mod, bool is_distant);


// マスクテクスチャ作成：小矩形の四角形
bool tnm_make_mask_texture_for_srect_square(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod);
// マスクテクスチャ作成：小矩形の菱形
bool tnm_make_mask_texture_for_srect_rhombus(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod);
// マスクテクスチャ作成：小矩形の十字
bool tnm_make_mask_texture_for_srect_jyuuji(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod);
// マスクテクスチャ作成：小矩形のテレビ
bool tnm_make_mask_texture_for_srect_television(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod);


// マスクテクスチャ作成：小矩形の４方向から
bool tnm_make_mask_texture_for_srect_crossdir(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h);
// マスクテクスチャ作成：小矩形の４方向からスライス
bool tnm_make_mask_texture_for_srect_crossdir_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int slice_len);
// マスクテクスチャ作成：小矩形の４方向からブラインド
bool tnm_make_mask_texture_for_srect_crossdir_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int blind_len);

// マスクテクスチャ作成：ストライプ（※マスクワイプ用じゃない）
bool tnm_make_mask_texture_for_stripe(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int stripe_len);

// マスクテクスチャ作成：マスクファイルを使う
bool tnm_make_mask_texture_from_file(BSP<C_d3d_texture> *texture, BSP<C_dib_chip> *dib, int target_w, int target_h, CTSTR& mask_file);
