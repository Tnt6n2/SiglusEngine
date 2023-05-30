#pragma		once

#include	"data/tnm_wipe.h"
#include	"data/tnm_wnd_menu.h"

// ****************************************************************
// 前方宣言
// ================================================================
template <typename TYPE>	class C_tree2;
struct	S_tnm_sorter;
class	C_tnm_sprite;
class	C_elm_stage;
class	C_elm_screen;
class   C_elm_world;

// ****************************************************************
// 描画用の頂点バッファ
// ================================================================

// ****************************************************************
// ウィンドウ
// ================================================================
class C_tnm_wnd : public C_window
{
public:

	// グローバル初期化
	void	init();

	// 描画処理
	bool	disp_proc();													// 描画処理
	bool	disp_proc_main();												// 描画処理メイン
	bool	disp_proc_sprite_tree_to_sprite_list(C_tree2<C_tnm_sprite *>& node, ARRAY<C_tnm_sprite *>& sprite_list);
	bool	disp_proc_sprite_list(ARRAY<C_tnm_sprite *>& sprite_list);		// スプライトリストを描画する
	bool	disp_proc_capture_for_save_thumb();								// キャプチャーする
	bool	disp_proc_capture_for_object();									// キャプチャーする（オブジェクト用）
	bool	disp_proc_capture_for_thumb();									// キャプチャーする（サムネイル用）
	bool	disp_proc_capture_for_tweet();									// キャプチャーする（ツイート用）
	bool	disp_proc_init_wipe_sprite(C_d3d_sprite* p_sprite);				// ワイプスプライトを初期化する
	bool	disp_proc_sprite(C_d3d_sprite* p_sprite);						// スプライトを描画する

	// ステージを描画準備する
	bool	disp_proc_stage_ready(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter);
	// ステージを描画する
	bool	disp_proc_stage(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter);
	bool	disp_proc_stage_sub(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter);

	// 全てのオーダーを描画準備する
	bool	disp_proc_all_order_ready(int stage_no);
	// 全てのオーダーを描画する
	bool	disp_proc_all_order(int stage_no);
	// ワイプより上のオーダーを描画準備する
	bool	disp_proc_over_order_ready(int stage_no);
	// ワイプより上のオーダーを描画する
	bool	disp_proc_over_order(int stage_no);
	// ワイプするオーダーを描画準備する
	bool	disp_proc_wipe_order_ready(int stage_no);
	// ワイプするオーダーを描画する
	bool	disp_proc_wipe_order(int stage_no);
	// ワイプより下のオーダーを描画準備する
	bool	disp_proc_under_order_ready(int stage_no);
	// ワイプより下のオーダーを描画する
	bool	disp_proc_under_order(int stage_no);
	// 最前面を描画する
	bool	disp_proc_top_most();

	// ワイプバッファ０を準備する
	bool	disp_proc_ready_wipe_buffer_0();
	// ワイプバッファ１を準備する
	bool	disp_proc_ready_wipe_buffer_1();

	// ワイプパラメータチェック＆コピー
	bool	check_and_copy_for_wipe_param();
	// マスク作成
	void	make_mask();

	// ワイプ描画
	bool	disp_proc_wipe();												// ワイプ描画
	bool	disp_proc_wipe_for_not_wipe_after_wait(int wipe_prg);			// ワイプ描画：ワイプなし（表示してから待つ）
	bool	disp_proc_wipe_for_not_wipe_befor_wait(int wipe_prg);			// ワイプ描画：ワイプなし（待ってから表示）
	bool	disp_proc_wipe_for_cross_fade(int wipe_prg);					// ワイプ描画：クロスフェード

	bool	disp_proc_wipe_for_mask(int wipe_prg);									// ワイプ描画：マスク
	void	disp_proc_wipe_for_mask_func(C_d3d_sprite* p_sprite, int wipe_prg);		// ワイプ描画：マスクＦＵＮＣ
	
	bool	disp_proc_wipe_for_shimi(int wipe_prg);									// ワイプ描画：シミ
	void	disp_proc_wipe_for_shimi_func(C_d3d_sprite* p_sprite, int wipe_prg);	// ワイプ描画：シミＦＵＮＣ

	bool	disp_proc_wipe_for_cross_raster(int wipe_prg);							// ワイプ描画：クロスラスタ
	bool	disp_proc_wipe_for_raster(int wipe_prg);								// ワイプ描画：ラスタ
	void	disp_proc_wipe_for_raster_func(C_d3d_sprite* p_sprite, int wipe_prg);	// ワイプ描画：ラスタＦＵＮＣ

	bool	disp_proc_wipe_for_move(int wipe_prg);							// ワイプ描画：移動系
	bool	disp_proc_wipe_for_move_get_stage(int wipe_prg);				// ワイプ描画：移動系ステージ取得
	bool	disp_proc_wipe_for_move_switch(int wipe_prg, bool is_front);	// ワイプ描画：移動系スイッチ
	bool	disp_proc_wipe_for_page(int wipe_prg, bool is_front);			// ワイプ描画：ページめくり	
	
	void	disp_proc_wipe_for_move_scroll_and_expansion_and_contraction(C_d3d_sprite* p_sprite, int wipe_prg, bool is_front);	// ワイプ描画：移動系：スクロールと伸縮変形
	void	disp_proc_wipe_for_move_func_scroll(int wipe_prg, C_d3d_sprite *sprite, int dir, bool is_in);						// ワイプ描画：移動系ＦＵＮＣ：スクロール
	void	disp_proc_wipe_for_move_func_expansion_and_contraction(int wipe_prg, C_d3d_sprite *sprite, int dir, bool is_in);	// ワイプ描画：移動系ＦＵＮＣ：伸縮変形

	bool	disp_proc_wipe_for_scale(int wipe_prg);							// ワイプ描画：スケール系
	bool	disp_proc_wipe_for_scale_get_stage(int wipe_prg);				// ワイプ描画：スケール系ステージ取得
	bool	disp_proc_wipe_for_scale_switch(int wipe_prg, bool is_front);	// ワイプ描画：スケール系スイッチ
	void	disp_proc_wipe_for_scale_func(C_d3d_sprite* sprite, int wipe_prg);		// ワイプ描画：スケール系ＦＵＮＣ
	void	disp_proc_wipe_for_scale_func(D3DXVECTOR4 pos[], D3DXVECTOR2 uv[], C_d3d_sprite* sprite, int wipe_prg);		// ワイプ描画：スケール系ＦＵＮＣ

	bool	disp_proc_wipe_for_explosion_blur(int wipe_prg);								// ワイプ描画：爆発ブラー
	bool	disp_proc_wipe_for_explosion_blur_get_stage(int wipe_prg);						// ワイプ描画：爆発ブラーステージ取得
	void	disp_proc_wipe_for_explosion_blur_func(C_d3d_sprite* sprite, int wipe_prg);		// ワイプ描画：爆発ブラーＦＵＮＣ

	bool	disp_proc_wipe_for_mosaic(int wipe_prg);								// ワイプ描画：モザイク
	bool	disp_proc_wipe_for_mosaic_get_stage(int wipe_prg);						// ワイプ描画：モザイクステージ取得
	void	disp_proc_wipe_for_mosaic_func(C_d3d_sprite* sprite, int wipe_prg);		// ワイプ描画：モザイクＦＵＮＣ

	// キャプションを更新
	void	update_caption();

	// キャプチャーバッファ
	bool	create_capture_buffer(int width, int height);
	bool	destroy_capture_buffer();
	bool	disp_proc_capture_to_buffer();

	// ビュー
	C_d3d_renderer		m_view;
	C_argb				m_back_color;
	C_argb				m_game_back_color;
	C_argb				m_wipe_back_color;

	// ウィンドウメニュー
	C_tnm_wnd_menu		m_wnd_menu;

	// ワイプ
	C_d3d_surface		m_screen_back_buffer;				// スクリーンのバックバッファ
	C_d3d_surface		m_screen_dpst_buffer;				// スクリーンの深度ステンシルバッファ
	C_d3d_surface		m_target_back_buffer;				// ターゲットとなるバックバッファ
	C_d3d_surface		m_target_dpst_buffer;				// ターゲットとなる深度ステンシルバッファ
	C_size				m_target_buffer_size;				// ターゲットとなるバッファサイズ
	C_size				m_target_buffer_size_ex;			// ターゲットとなるバッファサイズＥＸ
	BSP<C_d3d_texture>	m_game_back_buffer_texture;			// ゲーム画面用のバックバッファ
	BSP<C_d3d_surface>	m_game_dpst_buffer_surface;			// ゲーム画面用の深度ステンシルバッファ
	BSP<C_d3d_texture>	m_wipe_back_buffer_texture_0;		// ワイプ用のバックバッファ０
	BSP<C_d3d_surface>	m_wipe_dpst_buffer_surface_0;		// ワイプ用の深度ステンシルバッファ０
	BSP<C_d3d_texture>	m_wipe_back_buffer_texture_1;		// ワイプ用のバックバッファ１
	BSP<C_d3d_texture>	m_mask_buffer_texture;				// マスクバッファ
	BSP<C_dib_chip>		m_mask_buffer_dib;					// マスクバッファＤＩＢ
	BSP<C_d3d_texture>	m_capture_texture_for_save_thumb;	// キャプチャー用のテクスチャ
	BSP<C_d3d_texture>	m_capture_texture_for_object;		// キャプチャー用のテクスチャ
	BSP<C_d3d_texture>	m_capture_texture_for_tweet;		// キャプチャー用のテクスチャ

	BSP<C_d3d_texture>	m_capture_back_buffer_texture;		// キャプチャーバッファ用のテクスチャ
	BSP<C_d3d_surface>	m_capture_dpst_buffer_surface;		// キャプチャーバッファ用のテクスチャ
	BSP<C_d3d_texture>	m_capture_temp_buffer_texture;		// キャプチャーバッファ用のテクスチャ
	int					m_capture_buffer_width;
	int					m_capture_buffer_height;

private:
	int					m_ready_wipe_type;								// 準備できたワイプ番号
	TSTR				m_ready_wipe_file;								// 準備できたマスクワイプファイル
	int					m_ready_wipe_option[TNM_WIPE_OPTION_MAX];		// 準備できたワイプオプション

	bool	disp_proc_sprite_list_sub(C_elm_world* p_world, ARRAY<C_tnm_sprite *>& sprite_list);	// スプライトリストを描画する

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);

private:
	bool	on_create();
	bool	on_close();
	bool	on_paint();
	bool	on_command(WPARAM wp, LPARAM lp);
	bool	on_set_cursor(WPARAM wp, LPARAM lp);

	// パラメータ
	struct	STATE
	{
		TSTR	title;
	}	m_ld;

	bool	m_first;

	// ウィンドウサイズとクライアントサイズの差分
	C_size m_diff;
};



