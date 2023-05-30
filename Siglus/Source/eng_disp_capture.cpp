#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_thumb_table.h"

#include	"engine/eng_graphics.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// キャプチャー（セーブサムネイル）
// ================================================================
bool C_tnm_wnd::disp_proc_capture_for_save_thumb()
{
	// サムネイルテーブルからサムネイルを検索
	TSTR thumb_name = Gp_thumb_table->calc_thumb_file_name();
	
	// 専用サムネイルを使う場合
	if (!thumb_name.empty())	{

		// サムネイル画像を読み込む
		BSP<C_d3d_album> album = tnm_load_pct_d3d(thumb_name, false, false);
		if (!album)
			return false;

		// カット０番を使う
		m_capture_texture_for_save_thumb = album->get_texture(0);

		// 結果
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("★専用サムネイルをキャプチャーに設定しました。"));
	}

	// 画面をキャプチャーして使う場合
	else	{

		// セーブサムネイルのサイズに縮小する
		BSP<C_d3d_texture> capture_back_buffer_texture_src;
		BSP<C_d3d_texture> capture_back_buffer_texture_dst;
		BSP<C_d3d_surface> capture_dpst_buffer_surface_dst;
		int thumb_w = (int)Gp_ini->save_thumb_size.cx;
		int thumb_h = (int)Gp_ini->save_thumb_size.cy;

		// キャプチャー元はゲームバッファ
		capture_back_buffer_texture_src = m_game_back_buffer_texture;

		// キャプチャー先のバッファを作成する
		// テクスチャはレンダーターゲットにするため、D3DUSAGE_RENDERTARGET を指定
		// D3DUSAGE_RENDERTARGET にするには、D3DPOOL_DEFAULT を指定する必要がある
		// D3DPOOL_DEFAULT を指定した場合は、デバイスが消失したら自分で作成しなおす必要があるため、リソースマネージャを利用する
		// カラーフォーマットは、デバイスの作成時に調べた D3DFMT_X8R8G8B8 にすること。詳しくはデバイスの作成参照。
		capture_back_buffer_texture_dst = G_rsm.create_texture(_T("ｷｬﾌﾟﾁｬｰ"), thumb_w, thumb_h, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, true);
		if (!capture_back_buffer_texture_dst)	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("キャプチャー用のテクスチャの作成に失敗しました。(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
			return false;
		}
		capture_dpst_buffer_surface_dst = G_rsm.create_dpst_buffer(_T("ｷｬﾌﾟﾁｬｰ"), thumb_w, thumb_h, true);
		if (!capture_dpst_buffer_surface_dst)	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("キャプチャー用の深度ステンシルバッファの作成に失敗しました。(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
			return false;
		}

		// ゲームスプライトの初期設定
		C_d3d_sprite game_sprite;
		game_sprite.create_polygon_no_init();
		game_sprite.rp.disp = true;
		game_sprite.rp.d2_rect = true;			// ２Ｄ矩形
		game_sprite.rp.alpha_test = false;		// アルファテストしない
		game_sprite.rp.alpha_blend = false;		// アルファブレンドしない
		game_sprite.rp.size = C_size(thumb_w, thumb_h);
		game_sprite.rp.dst_clip_use = true;
		game_sprite.rp.dst_clip = C_rect(C_point(0, 0), C_size(thumb_w, thumb_h));
		game_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		game_sprite.set_index_buffer(G_rect_index_buffer);
		game_sprite.set_texture(capture_back_buffer_texture_src, 0);

		// ゲームスプライトを更新する
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (game_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// キャプチャーバッファを取り出す
			C_d3d_surface capture_back_buffer_dst = capture_back_buffer_texture_dst->get_surface(0);
			C_d3d_surface capture_dpst_buffer_dst = *capture_dpst_buffer_surface_dst;

			// レンダーターゲットをキャプチャーバッファに設定する
			m_view.set_render_target(capture_back_buffer_dst.body());
			m_view.set_dpst_buffer(capture_dpst_buffer_dst.body());

			// ゲームスプライトを描画
			if (!disp_proc_sprite(&game_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}

		// キャプチャー画像をシステムメモリに取り出す
		// こうしないとデバイスが消失したときに失ってしまう

		// データ取得用のテクスチャを作る
		// キャプチャー画像に合わせて D3DFMT_X8R8G8B8 にする必要がある
		// D3DPOOL_SYSTEMMEM でなければならない（重要！ D3DPOOL_MANAGED だとエラーが出る）
		BSP<C_d3d_texture> p_thumb_texture = G_rsm.create_texture(_T("ｾｰﾌﾞｻﾑﾈｲﾙ"), thumb_w, thumb_h, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, false);
		if (!p_thumb_texture)
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サムネイル用のテクスチャの作成に失敗しました。\n") + get_last_error_msg());
			return false;
		}
		// テクスチャにキャプチャー画像を取得する
		if (!capture_back_buffer_texture_dst->get_render_target_data(0, p_thumb_texture->get_surface(0).body()))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャー画像のデータが取得できませんでした。\n") + get_last_error_msg());
			return false;
		}

		// メンバに保存
		m_capture_texture_for_save_thumb = p_thumb_texture;

		// 結果
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("★セーブサムネイルを作成しました。"));
	}

	return true;
}

// ****************************************************************
// キャプチャー（ツイート）
// ================================================================
bool C_tnm_wnd::disp_proc_capture_for_tweet()
{
	// セーブサムネイルのサイズに縮小する
	BSP<C_d3d_texture> capture_back_buffer_texture_src;
	BSP<C_d3d_texture> capture_back_buffer_texture_dst;
	BSP<C_d3d_surface> capture_dpst_buffer_surface_dst;
	int thumb_w = Gp_ini->screen_size.cx;
	int thumb_h = Gp_ini->screen_size.cy;

	// キャプチャー元はゲームバッファ
	capture_back_buffer_texture_src = m_game_back_buffer_texture;

	// キャプチャー先のバッファを作成する
	// テクスチャはレンダーターゲットにするため、D3DUSAGE_RENDERTARGET を指定
	// D3DUSAGE_RENDERTARGET にするには、D3DPOOL_DEFAULT を指定する必要がある
	// 使い捨てなのでリソースマネージャの管理は必要ない
	// カラーフォーマットは、デバイスの作成時に調べた D3DFMT_X8R8G8B8 にすること。詳しくはデバイスの作成参照。
	capture_back_buffer_texture_dst = G_rsm.create_texture(_T("ｷｬﾌﾟﾁｬｰ"), thumb_w, thumb_h, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, false);
	if (!capture_back_buffer_texture_dst)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("キャプチャー用のテクスチャの作成に失敗しました。(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
		return false;
	}
	capture_dpst_buffer_surface_dst = G_rsm.create_dpst_buffer(_T("ｷｬﾌﾟﾁｬｰ"), thumb_w, thumb_h, true);
	if (!capture_dpst_buffer_surface_dst)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("キャプチャー用の深度ステンシルバッファの作成に失敗しました。(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
		return false;
	}

	// ゲームスプライトの初期設定
	C_d3d_sprite game_sprite;
	game_sprite.create_polygon_no_init();
	game_sprite.rp.disp = true;
	game_sprite.rp.d2_rect = true;			// ２Ｄ矩形
	game_sprite.rp.alpha_test = false;		// アルファテストしない
	game_sprite.rp.alpha_blend = false;		// アルファブレンドしない
	game_sprite.rp.size = C_size(thumb_w, thumb_h);
	game_sprite.rp.dst_clip_use = true;
	game_sprite.rp.dst_clip = C_rect(C_point(0, 0), C_size(thumb_w, thumb_h));
	game_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
	game_sprite.set_index_buffer(G_rect_index_buffer);
	game_sprite.set_texture(capture_back_buffer_texture_src, 0);

	// ゲームスプライトを更新する
	void* p_locked_vertex_buffer;
	G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
	if (!game_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))
	{
		G_rect_vertex_buffer_d2_screen->unlock();
		return false;
	}
	
	G_rect_vertex_buffer_d2_screen->unlock();

	// キャプチャーバッファを取り出す
	C_d3d_surface capture_back_buffer_dst = capture_back_buffer_texture_dst->get_surface(0);
	C_d3d_surface capture_dpst_buffer_dst = *capture_dpst_buffer_surface_dst;

	// レンダーターゲットをキャプチャーバッファに設定する
	m_view.set_render_target(capture_back_buffer_dst.body());
	m_view.set_dpst_buffer(capture_dpst_buffer_dst.body());

	// ゲームスプライトを描画
	if (!disp_proc_sprite(&game_sprite))
		return false;

	// ロゴを読み込む
	TSTR logo_file_name = Gp_ini->twitter_overlap_image;
	if (!logo_file_name.empty() && tnm_check_pct(logo_file_name, false))
	{
		BSP<C_d3d_album> album = tnm_load_pct_d3d(logo_file_name, false, false);
		if (album)
		{
			// ロゴスプライトの初期設定
			C_d3d_sprite logo_sprite;
			logo_sprite.create_polygon_no_init();
			logo_sprite.rp.disp = true;
			logo_sprite.rp.d2_rect = true;			// ２Ｄ矩形
			logo_sprite.rp.alpha_test = true;		// アルファテストする
			logo_sprite.rp.alpha_blend = true;		// アルファブレンドしない
			logo_sprite.rp.size = C_size(thumb_w, thumb_h);
			logo_sprite.rp.dst_clip_use = true;
			logo_sprite.rp.dst_clip = C_rect(C_point(0, 0), C_size(thumb_w, thumb_h));
			logo_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
			logo_sprite.set_index_buffer(G_rect_index_buffer);
			logo_sprite.set_texture(album->get_texture(0), 0);

			// ロゴスプライトを更新する
			G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
			if (!logo_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))
			{
				G_rect_vertex_buffer_d2_screen->unlock();
				return false;
			}

			G_rect_vertex_buffer_d2_screen->unlock();

			// ロゴスプライトを描画
			if (!disp_proc_sprite(&logo_sprite))
				return false;
		}
	}

	// キャプチャー画像をシステムメモリに取り出す

	// データ取得用のテクスチャを作る
	// キャプチャー画像に合わせて D3DFMT_X8R8G8B8 にする必要がある
	// D3DPOOL_SYSTEMMEM でなければならない（重要！ D3DPOOL_MANAGED だとエラーが出る）
	BSP<C_d3d_texture> p_thumb_texture = G_rsm.create_texture(_T("ﾂｲｰﾄ"), thumb_w, thumb_h, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, false);
	if (!p_thumb_texture)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サムネイル用のテクスチャの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
	// テクスチャにキャプチャー画像を取得する
	if (!capture_back_buffer_texture_dst->get_render_target_data(0, p_thumb_texture->get_surface(0).body()))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャー画像のデータが取得できませんでした。\n") + get_last_error_msg());
		return false;
	}

	// メンバに保存
	m_capture_texture_for_tweet = p_thumb_texture;

	// 結果
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("★ツイートサムネイルを作成しました。"));

	return true;
}

// ****************************************************************
// キャプチャー（サムネイル用）
// ================================================================
bool C_tnm_wnd::disp_proc_capture_for_thumb()
{
	// セーブサムネイルのサイズに縮小する
	BSP<C_d3d_texture> capture_back_buffer_texture_src;
	BSP<C_d3d_texture> capture_back_buffer_texture_dst;
	BSP<C_d3d_surface> capture_dpst_buffer_surface_dst;
	int thumb_w = Gp_global->capture_w;
	int thumb_h = Gp_global->capture_h;

	// キャプチャー元はゲームバッファ
	capture_back_buffer_texture_src = m_game_back_buffer_texture;

	// キャプチャー先のバッファを作成する
	// テクスチャはレンダーターゲットにするため、D3DUSAGE_RENDERTARGET を指定
	// D3DUSAGE_RENDERTARGET にするには、D3DPOOL_DEFAULT を指定する必要がある
	// D3DPOOL_DEFAULT を指定した場合は、デバイスが消失したら自分で作成しなおす必要があるため、リソースマネージャを利用する
	// カラーフォーマットは、デバイスの作成時に調べた D3DFMT_X8R8G8B8 にすること。詳しくはデバイスの作成参照。
	capture_back_buffer_texture_dst = G_rsm.create_texture(_T("ｷｬﾌﾟﾁｬｰ"), thumb_w, thumb_h, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, true);
	if (!capture_back_buffer_texture_dst)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("キャプチャー用のテクスチャの作成に失敗しました。(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
		return false;
	}
	capture_dpst_buffer_surface_dst = G_rsm.create_dpst_buffer(_T("ｷｬﾌﾟﾁｬｰ"), thumb_w, thumb_h, true);
	if (!capture_dpst_buffer_surface_dst)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("キャプチャー用の深度ステンシルバッファの作成に失敗しました。(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
		return false;
	}

	// ゲームスプライトの初期設定
	C_d3d_sprite game_sprite;
	game_sprite.create_polygon_no_init();
	game_sprite.rp.disp = true;
	game_sprite.rp.d2_rect = true;			// ２Ｄ矩形
	game_sprite.rp.alpha_test = false;		// アルファテストしない
	game_sprite.rp.alpha_blend = false;		// アルファブレンドしない
	game_sprite.rp.size = C_size(thumb_w, thumb_h);
	game_sprite.rp.dst_clip_use = true;
	game_sprite.rp.dst_clip = C_rect(C_point(0, 0), C_size(thumb_w, thumb_h));
	game_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
	game_sprite.set_index_buffer(G_rect_index_buffer);
	game_sprite.set_texture(capture_back_buffer_texture_src, 0);

	// ゲームスプライトを更新する
	void* p_locked_vertex_buffer;
	G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
	if (game_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))
	{
		G_rect_vertex_buffer_d2_screen->unlock();

		// キャプチャーバッファを取り出す
		C_d3d_surface capture_back_buffer_dst = capture_back_buffer_texture_dst->get_surface(0);
		C_d3d_surface capture_dpst_buffer_dst = *capture_dpst_buffer_surface_dst;

		// レンダーターゲットをキャプチャーバッファに設定する
		m_view.set_render_target(capture_back_buffer_dst.body());
		m_view.set_dpst_buffer(capture_dpst_buffer_dst.body());

		// ゲームスプライトを描画
		if (!disp_proc_sprite(&game_sprite))
			return false;
	}
	else
	{
		G_rect_vertex_buffer_d2_screen->unlock();
	}

	// キャプチャー画像をシステムメモリに取り出す
	// こうしないとデバイスが消失したときに失ってしまう

	// データ取得用のテクスチャを作る
	// キャプチャー画像に合わせて D3DFMT_X8R8G8B8 にする必要がある
	// D3DPOOL_SYSTEMMEM でなければならない（重要！ D3DPOOL_MANAGED だとエラーが出る）
	BSP<C_d3d_texture> p_thumb_texture = G_rsm.create_texture(_T("ｻﾑﾈｲﾙ"), thumb_w, thumb_h, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, false);
	if (!p_thumb_texture)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サムネイル用のテクスチャの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// テクスチャにキャプチャー画像を取得する
	if (!capture_back_buffer_texture_dst->get_render_target_data(0, p_thumb_texture->get_surface(0).body()))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャー画像のデータが取得できませんでした。\n") + get_last_error_msg());
		return false;
	}

	// メンバに保存
	m_capture_texture_for_object = p_thumb_texture;

	// 結果
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("★サムネイルを作成しました。"));

	return true;
}

// ****************************************************************
// キャプチャー（オブジェクト用）
// ================================================================
bool C_tnm_wnd::disp_proc_capture_for_object()
{
	bool ret = false;

	int sw = (int)Gp_ini->screen_size.cx;
	int sh = (int)Gp_ini->screen_size.cy;

	//
	// １．キャプチャー画像を取り出す
	//

	// キャプチャー画像取得用のテクスチャ sysmem_texture を作る。
	// GetRenderTargetData を使うには D3DPOOL_SYSTEMMEM でなければならない。
	// D3DPOOL_SYSTEMMEM では D3DUSAGE_AUTOGENMIPMAP は指定できない。
	// バックバッファに合わせて D3DFMT_X8R8G8B8 にする必要がある。
	BSP<C_d3d_texture> sysmem_texture = G_rsm.create_texture(_T("ｷｬﾌﾟﾁｬｰ(ﾃﾝﾎﾟﾗﾘ)"), sw, sh, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, false);
	if (!sysmem_texture)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャー用の一時テクスチャの作成に失敗しました。\n") + get_last_error_msg());
	}
	else	{

		// sysmem_texture にキャプチャー画像を取得する
		if (!m_game_back_buffer_texture->get_render_target_data(0, sysmem_texture->get_surface(0).body()))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーが取得できませんでした。\n") + get_last_error_msg());
		}
		else	{

			//
			// ２．キャプチャー画像を MANAGED テクスチャにコピーする
			//

			// MANAGED テクスチャを作成する。
			// オブジェクトに渡すため、D3DFMT_A8R8G8B8 を指定する必要がある。
			// D3DUSAGE_AUTOGENMIPMAP を忘れずに。
			BSP<C_d3d_texture> managed_texture = G_rsm.create_texture(_T("ｷｬﾌﾟﾁｬｰ"), sw, sh, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, false);
			if (!managed_texture)	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャー用のテクスチャの作成に失敗しました。\n") + get_last_error_msg());
			}
			else	{

				// ロックしてデータをコピーする
				// 不透明度は 255 で埋める
				D3DLOCKED_RECT lr_sysmem, lr_managed;
				if (!sysmem_texture->lock_rect(0, &lr_sysmem, NULL, D3DLOCK_DISCARD))	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャー用の一時テクスチャのロックに失敗しました。\n") + get_last_error_msg());
				}
				else	{
					if (!managed_texture->lock_rect(0, &lr_managed, NULL, D3DLOCK_DISCARD))	{
						tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャー用のテクスチャのロックに失敗しました。\n") + get_last_error_msg());
					}
					else	{
						for (int y = 0; y < sh; y++)	{
							BYTE* dst = ((BYTE *)lr_managed.pBits) + lr_managed.Pitch * y;
							BYTE* src = ((BYTE *)lr_sysmem.pBits) + lr_sysmem.Pitch * y;
							for (int x = 0; x < sw; x++)	{
								*dst++ = *src++;
								*dst++ = *src++;
								*dst++ = *src++;
								*dst++ = 255;	src++;
							}
						}

						ret = true;	// 成功！

						managed_texture->unlock_rect(0);
					}
					sysmem_texture->unlock_rect(0);
				}

				// メンバに保存
				m_capture_texture_for_object = managed_texture;
			}
		}
	}

	// 結果
	if (ret)	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("★画面をキャプチャーしました。"));
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("★画面のキャプチャーに失敗しました。"));
	}

	return ret;
}

// ****************************************************************
// キャプチャーバッファシステム
// ================================================================

// キャプチャーバッファを作成
bool C_tnm_wnd::create_capture_buffer(int width, int height)
{
	// セーブサムネイルのサイズに縮小する
	BSP<C_d3d_texture> capture_back_buffer_texture;
	BSP<C_d3d_surface> capture_dpst_buffer_surface;

	// キャプチャー先のバッファを作成する
	// テクスチャはレンダーターゲットにするため、D3DUSAGE_RENDERTARGET を指定
	// D3DUSAGE_RENDERTARGET にするには、D3DPOOL_DEFAULT を指定する必要がある
	// D3DPOOL_DEFAULT を指定した場合は、デバイスが消失したら自分で作成しなおす必要があるため、リソースマネージャを利用する
	// カラーフォーマットは、デバイスの作成時に調べた D3DFMT_X8R8G8B8 にすること。詳しくはデバイスの作成参照。
	capture_back_buffer_texture = G_rsm.create_texture(_T("ｷｬﾌﾟﾁｬｰ"), width, height, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);
	if (!capture_back_buffer_texture)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("キャプチャー用のテクスチャの作成に失敗しました。(%d x %d)\n"), width, height) + get_last_error_msg());
		return false;
	}
	capture_dpst_buffer_surface = G_rsm.create_dpst_buffer(_T("ｷｬﾌﾟﾁｬｰ"), width, height, true);
	if (!capture_dpst_buffer_surface)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("キャプチャー用の深度ステンシルバッファの作成に失敗しました。(%d x %d)\n"), width, height) + get_last_error_msg());
		return false;
	}
	
	// データ取得用のテクスチャを作る
	// こちらは透過度は必要なので D3DFMT_A8R8G8B8 にする必要がある
	// D3DPOOL_SYSTEMMEM でなければならない（重要！ D3DPOOL_MANAGED だとエラーが出る）
	BSP<C_d3d_texture> temp_texture = G_rsm.create_texture(_T("ｷｬﾌﾟﾁｬｰ"), width, height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, false);
	if (!temp_texture)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャー取り出し用のテクスチャの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	m_capture_back_buffer_texture = capture_back_buffer_texture;
	m_capture_dpst_buffer_surface = capture_dpst_buffer_surface;
	m_capture_temp_buffer_texture = temp_texture;
	m_capture_buffer_width = width;
	m_capture_buffer_height = height;

	return true;
}

// キャプチャーバッファを破壊
bool C_tnm_wnd::destroy_capture_buffer()
{
	m_capture_back_buffer_texture.reset();
	m_capture_dpst_buffer_surface.reset();
	m_capture_temp_buffer_texture.reset();
	m_capture_buffer_width = 0;
	m_capture_buffer_height = 0;

	return true;
}

// キャプチャー
bool C_tnm_wnd::disp_proc_capture_to_buffer()
{
	// テクスチャにキャプチャー画像を取得する
	if (!m_capture_back_buffer_texture->get_render_target_data(0, m_capture_temp_buffer_texture->get_surface(0).body()))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャー画像のデータが取得できませんでした。\n") + get_last_error_msg());
		return false;
	}

	return true;
}

