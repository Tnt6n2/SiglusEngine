#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_thumb_table.h"
#include	"data/tnm_fog.h"

#include	"element/elm_flag.h"
#include	"element/elm_world.h"
#include	"element/elm_screen.h"
#include	"element/elm_stage.h"
#include	"element/elm_msg_back.h"
#include	"element/elm_excall.h"

#include	"engine/eng_graphics.h"
#include	"engine/eng_fog.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_etc.h"
#include	"engine/eng_save.h"

#if __USE_EMOTE
#include	"engine/eng_emote.h"
#endif

// ステージを取得
inline C_elm_stage* STAGE(int excall_no, int stage_no)
{
	if (excall_no == 0)	{
		return &(*Gp_stage_list)[stage_no];
	}
	else	{
		return &Gp_excall->m_stage_list[stage_no];
	}
}

// ****************************************************************
// ウィンドウ：ゲームバッファを使うかどうかを判定する
// ================================================================
bool is_use_game_buffer()
{
	// ゲームスクリーンサイズの大きさが標準でなければ使う
	if (Gp_global->game_screen_size != Gp_global->total_game_screen_size)
		return true;

	// ゲームスクリーンサイズとスクリーンサイズが違えば使う
	if (Gp_global->total_game_screen_size != Gp_global->total_screen_size)
		return true;

	// ゲームスクリーン位置が標準でなければ使う
	if (Gp_global->total_game_screen_pos != C_point(0, 0))
		return true;

	// フルシェイク中なら使う
	if (Gp_screen->shake().m_no >= 0)
		return true;

	// キャプチャー中なら使う
	if (Gp_global->capture_type != TNM_CAPTURE_TYPE_NONE)
		return true;

	// 使わない
	return false;
}

// ****************************************************************
// ウィンドウ：フォーマット
// ================================================================
void C_tnm_wnd::init()
{
	// 背景色
	m_back_color = C_argb(255, 0, 0, 0);				// ★こっちが本番
	m_game_back_color = C_argb(255, 0, 0, 0);
	m_wipe_back_color = C_argb(255, 0, 0, 0);
	//m_back_color = C_argb(255, 0, 0, 255);			// ★デバッグ時はこっちを使うといいかも
	//m_game_back_color = C_argb(255, 0, 255, 0);
	//m_wipe_back_color = C_argb(255, 255, 0, 0);

	m_ready_wipe_type = -1;

	// パラメータ
	m_first = true;
}

// ****************************************************************
// 描画処理
// ================================================================
//		通常時はフロントを直接描画します。
//		ワイプ時はフロントを描画した後、ネクストをワイプ描画します。
//		バックを描画することはありません。
// ================================================================
//		スクリーンバッファ：最終描画バッファ（いわゆるバックバッファ）
//		ゲームバッファ：標準ウィンドウ時の最終バッファ。フルスクリーン時などはこれを拡縮してスクリーンバッファに描画する。
// ================================================================
bool C_tnm_wnd::disp_proc()
{
	// ステートをテンプレートで初期化する
	G_d3d.state.set_template();

	// スクリーンバッファをクリア（画面転送をしないときは処理しない）
	if (Gp_global->present_flag)
	{
		// スクリーンのバックバッファと深度ステンシルバッファを取得しておく
		m_screen_back_buffer = m_view.get_back_buffer();
		m_screen_dpst_buffer = m_view.get_dpst_buffer();

		// レンダーターゲットをスクリーンバッファに設定する
		m_view.set_render_target(m_screen_back_buffer.body());
		m_view.set_dpst_buffer(m_screen_dpst_buffer.body());

		// ビューポートをテクスチャ全体にする
		if (!G_d3d.device.set_view_port(C_point(0, 0), Gp_global->total_screen_size))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ビューポートの設定に失敗しました。\n") + get_last_error_msg());
			return false;
		}

		// レンダーターゲットを黒でクリア
		m_view.clear_render_target(m_back_color);
	}

	// ムービー再生中フラグ
	bool movie_playing_flag = tnm_is_movie_playing() && !tnm_is_movie_readying();

	// ムービー再生中は以下の処理を行わない（ただしキャプチャー中は行う。エンドセーブのため）
	if (!movie_playing_flag || Gp_global->capture_type != TNM_CAPTURE_TYPE_NONE)
	{
		// ゲームバッファを設定する
		{
			// バッファキャプチャーの場合はキャプチャーバッファがターゲット
			if (Gp_global->capture_type == TNM_CAPTURE_TYPE_BUFFER)
			{
				// ターゲットバッファはキャプチャーバッファ
				m_target_back_buffer = m_capture_back_buffer_texture->get_surface(0);
				m_target_dpst_buffer = *m_capture_dpst_buffer_surface;

				// レンダーターゲットをターゲットバッファに設定する
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// ビューポートをテクスチャ全体にする
				G_d3d.device.set_view_port(C_point(0, 0), m_capture_back_buffer_texture->get_size_ex());

				// レンダーターゲットをクリア
				m_view.clear_render_target(C_argb(0, 0, 0, 0));

				// ビューポートをバッファサイズにあわせる
				m_view.set_default_view_port(C_rect(0, 0, m_capture_buffer_width, m_capture_buffer_height));
			}
			// ゲームバッファを使わない場合はスクリーンバッファがターゲットバッファ
			else if (!is_use_game_buffer())
			{
				// ゲームバッファテクスチャを解放
				if (m_game_back_buffer_texture)
					m_game_back_buffer_texture.reset();
				if (m_game_dpst_buffer_surface)
					m_game_dpst_buffer_surface.reset();

				// ターゲットバッファはスクリーンバッファ
				m_target_back_buffer = m_screen_back_buffer;
				m_target_dpst_buffer = m_screen_dpst_buffer;

				// ビューポートをゲーム画面サイズにあわせる
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			}
			// ゲームバッファを使う場合はゲームバッファを作成
			else
			{
				// ゲームバッファテクスチャがなければ新規作成
				if (!m_game_back_buffer_texture || !m_game_dpst_buffer_surface)	{
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ゲームバッファを作成します。"));

					// 念のため、いったんゲームバッファをクリアする
					m_game_back_buffer_texture.reset();
					m_game_dpst_buffer_surface.reset();

					// ゲームバッファを作成する
					C_size size = Gp_global->game_screen_size;
					m_game_back_buffer_texture = G_rsm.create_texture(_T("ｹﾞｰﾑﾊﾞｯﾌｧ"), size.cx, size.cy, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, true);
					if (!m_game_back_buffer_texture)	{
						tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("ゲームバッファ用のテクスチャの作成に失敗しました。(%d x %d)\n"), size.cx, size.cy) + get_last_error_msg());
						return false;
					}
					m_game_dpst_buffer_surface = G_rsm.create_dpst_buffer(_T("ｹﾞｰﾑﾊﾞｯﾌｧ"), size.cx, size.cy, true);
					if (!m_game_dpst_buffer_surface)	{
						tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("ゲームバッファ用の深度ステンシルバッファの作成に失敗しました。(%d x %d)\n"), size.cx, size.cy) + get_last_error_msg());
						return false;
					}

					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ゲームバッファを作成しました。"));
				}

				// ゲームバッファをターゲットバッファにする
				m_target_back_buffer = m_game_back_buffer_texture->get_surface(0);
				m_target_dpst_buffer = *m_game_dpst_buffer_surface;

				// レンダーターゲットをターゲットバッファに設定する
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// ビューポートをテクスチャ全体にする
				G_d3d.device.set_view_port(C_point(0, 0), m_game_back_buffer_texture->get_size_ex());

				// レンダーターゲットをクリア
				m_view.clear_render_target(m_game_back_color);

				// ビューポートをゲーム画面サイズにあわせる
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			}
		}

		// 描画処理メイン
		if (!disp_proc_main())
			return false;

		// 画面転送を行う場合は続いて処理
		if (Gp_global->present_flag)	{

			// ゲームバッファを使っている場合はゲームバッファをスクリーンに描画する（ムービー再生中は行わない）
			if (is_use_game_buffer() && !movie_playing_flag)	{

				// レンダーターゲットをスクリーンに設定する
				m_view.set_render_target(m_screen_back_buffer.body());
				m_view.set_dpst_buffer(m_screen_dpst_buffer.body());			// === ここからは当たり判定に関係しない補正値を求める

				// 各サイズのエイリアス
				C_size gssize = Gp_global->game_screen_size;			// ゲームスクリーンサイズ
				C_point t_gspos = Gp_global->total_game_screen_pos;		// 合計ゲームスクリーン位置
				C_size t_gssize = Gp_global->total_game_screen_size;	// 合計ゲームスクリーンサイズ

				// フルシェイクの補正値を求める
				C_point shake_rep_pos;
				shake_rep_pos.x = Gp_screen->shake().m_cur_shake_x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx;
				shake_rep_pos.y = Gp_screen->shake().m_cur_shake_y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy;

				// ゲームスプライトの初期設定
				C_d3d_sprite game_sprite;
				game_sprite.create_polygon_no_init();
				game_sprite.rp.disp = true;
				game_sprite.rp.d2_rect = true;
				game_sprite.rp.size = gssize;
				game_sprite.rp.pos.x = (float)(t_gspos.x + shake_rep_pos.x);
				game_sprite.rp.pos.y = (float)(t_gspos.y + shake_rep_pos.y);
				game_sprite.rp.scale.x = (float)t_gssize.cx / gssize.cx;
				game_sprite.rp.scale.y = (float)t_gssize.cy / gssize.cy;
				game_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
				game_sprite.set_index_buffer(G_rect_index_buffer);
				game_sprite.set_texture(m_game_back_buffer_texture, 0);

				// ビューポートを計算（フルシェイクを考慮）
				int l = std::max<int>(0, std::max<int>(t_gspos.x, t_gspos.x + shake_rep_pos.x));
				int t = std::max<int>(0, std::max<int>(t_gspos.y, t_gspos.y + shake_rep_pos.y));
				int r = std::min<int>(Gp_global->total_screen_size.cx, std::min<int>(t_gspos.x + t_gssize.cx, t_gspos.x + t_gssize.cx + shake_rep_pos.x));
				int b = std::min<int>(Gp_global->total_screen_size.cy, std::min<int>(t_gspos.y + t_gssize.cy, t_gspos.y + t_gssize.cy + shake_rep_pos.y));

				// ビューポートを画面サイズにあわせる（フルシェイクを考慮）
				m_view.set_default_view_port(C_rect(l, t, r, b));

				// ゲームスプライトを更新する
				void* p_locked_vertex_buffer;
				G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
				if (game_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
					G_rect_vertex_buffer_d2_screen->unlock();

					// ゲームスプライトを描画
					if (!disp_proc_sprite(&game_sprite))
						return false;
				}
				else	{
					G_rect_vertex_buffer_d2_screen->unlock();
				}
			}
		}
	}

	// スクリーンバッファを解放する
	m_screen_back_buffer.release();
	m_screen_dpst_buffer.release();

	// ターゲットバッファを解放する
	m_target_back_buffer.release();
	m_target_dpst_buffer.release();

	return true;
}

// ****************************************************************
// 描画処理メイン
// ================================================================
bool C_tnm_wnd::disp_proc_main()
{
	// ワイプ中でない場合
	if (!Gp_wipe->is_wipe_doing())	{

		// システムワイプ中
		if (Gp_global->system_wipe_flag)	{

			// 何も描画しない
		}
		else	{

			// 全てのオーダーを描画準備する
			if (!disp_proc_all_order_ready(TNM_STAGE_FRONT))
				return false;
			// 全てのオーダーを描画する
			if (!disp_proc_all_order(TNM_STAGE_FRONT))
				return false;
		}
	}
	// ワイプ中の場合
	else	{

		// ワイプ描画処理
		if (!disp_proc_wipe())
			return false;
	}

	return true;
}

// ****************************************************************
// ワイプバッファ０を準備する
// ================================================================
bool C_tnm_wnd::disp_proc_ready_wipe_buffer_0()
{
	// ワイプバッファ用のテクスチャのサーフェスを取得
	C_d3d_surface wipe_buffer_surface = m_wipe_back_buffer_texture_0->get_surface(0);
	if (!wipe_buffer_surface.body())
		return false;

	// レンダーターゲットをワイプバッファに設定する
	m_view.set_render_target(wipe_buffer_surface.body());
	m_view.set_dpst_buffer(m_wipe_dpst_buffer_surface_0->body());

	// ビューポートをテクスチャ全体にする
	G_d3d.device.set_view_port(C_point(0, 0), m_wipe_back_buffer_texture_0->get_size_ex());

	// レンダーターゲットを透明でクリア！
	if (!m_view.clear_render_target(C_argb(0, 0, 0, 0)))
		return false;

	// ビューポートをゲームサイズにあわせる
	m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));

	return true;
}

// ****************************************************************
// ワイプバッファ１を準備する
// ================================================================
bool C_tnm_wnd::disp_proc_ready_wipe_buffer_1()
{
	// ワイプバッファ用のテクスチャのサーフェスを取得
	C_d3d_surface wipe_buffer_surface = m_wipe_back_buffer_texture_1->get_surface(0);
	if (!wipe_buffer_surface.body())
		return false;

	// レンダーターゲットをワイプバッファに設定する
	m_view.set_render_target(wipe_buffer_surface.body());
	m_view.set_dpst_buffer(m_wipe_dpst_buffer_surface_0->body());

	// ビューポートをテクスチャ全体にする
	G_d3d.device.set_view_port(C_point(0, 0), m_wipe_back_buffer_texture_1->get_size_ex());

	// レンダーターゲットを透明でクリア！
	if (!m_view.clear_render_target(C_argb(0, 0, 0, 0)))
		return false;

	// ビューポートをゲームサイズにあわせる
	m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));

	return true;
}

// ****************************************************************
// ワイプ描画：ワイプスプライトを初期化
// ================================================================
bool C_tnm_wnd::disp_proc_init_wipe_sprite(C_d3d_sprite* p_sprite)
{
	p_sprite->rp.disp = true;
	p_sprite->rp.d2_rect = true;
	p_sprite->rp.alpha_test = true;
	p_sprite->rp.alpha_blend = true;
	p_sprite->rp.blend_type = E_d3d_blend_type_alpha;
	p_sprite->rp.size = m_wipe_back_buffer_texture_0->get_size();
	p_sprite->set_texture(m_wipe_back_buffer_texture_0, 0);

	return true;
}

// ****************************************************************
// ワイプ描画：全てのオーダーを描画準備する
// ================================================================
bool C_tnm_wnd::disp_proc_all_order_ready(int stage_no)
{
	if (stage_no == -1)
		return true;

	// 全てのオーダーを描画準備する
	if (!disp_proc_stage_ready(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), S_tnm_sorter(INT_MAX, INT_MAX)))
		return false;

	return true;
}

// ****************************************************************
// ワイプ描画：全てのオーダーを描画する
// ================================================================
bool C_tnm_wnd::disp_proc_all_order(int stage_no)
{
	if (stage_no == -1)
		return true;

	// 全てのオーダーを描画する
	if (!disp_proc_stage(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), S_tnm_sorter(INT_MAX, INT_MAX)))
		return false;

	return true;
}

// ****************************************************************
// ワイプ描画：ワイプより下のオーダーを描画準備する
// ================================================================
bool C_tnm_wnd::disp_proc_under_order_ready(int stage_no)
{
	if (stage_no == -1)
		return true;

	// システムワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// ワイプより下はありえないので何もしない
	}
	// ＥＸコールワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ワイプより下を描画準備する
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MIN, INT_MIN))	{
			begin_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
			if (!disp_proc_stage_ready(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), begin_sorter.minus_1()))
				return false;
		}
	}
	// 通常のワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		// ワイプより下を描画準備する
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MIN, INT_MIN))	{
			if (!disp_proc_stage_ready(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), begin_sorter.minus_1()))
				return false;
		}
	}

	return true;
}

// ****************************************************************
// ワイプ描画：ワイプより下のオーダーを描画する
// ================================================================
bool C_tnm_wnd::disp_proc_under_order(int stage_no)
{
	if (stage_no == -1)
		return true;

	// システムワイプの場合
	if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// ワイプより下はありえないので何もしない
	}
	// ＥＸコールワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ワイプより下を描画する
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MIN, INT_MIN))	{
			begin_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
			if (!disp_proc_stage(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), begin_sorter.minus_1()))
				return false;
		}
	}
	// 通常のワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		// ワイプより下を描画する
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MIN, INT_MIN))	{
			if (!disp_proc_stage(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), begin_sorter.minus_1()))
				return false;
		}
	}

	return true;
}

// ****************************************************************
// ワイプ描画：ワイプするオーダーを描画準備する
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_order_ready(int stage_no)
{
	if (stage_no == -1)
		return true;

	// システムワイプの場合
	if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// システムワイプアウト中は、「フロント→黒」にワイプします。フロント＝黒、ネクスト＝フロント　なのです。
		if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{
			if (stage_no == TNM_STAGE_NEXT)
				stage_no = TNM_STAGE_FRONT;
			else
				return true;
		}

		// ワイプするオーダーを描画する
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (!disp_proc_stage_ready(stage_no, begin_sorter, end_sorter))
			return false;
	}
	// ＥＸコールワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ワイプするオーダーを描画する
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			begin_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
		}
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			end_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
		}
		if (!disp_proc_stage_ready(stage_no, begin_sorter, end_sorter))
			return false;
	}
	// 通常のワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		// ワイプするオーダーを描画する
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (!disp_proc_stage_ready(stage_no, begin_sorter, end_sorter))
			return false;
	}

	return true;
}

// ****************************************************************
// ワイプ描画：ワイプするオーダーを描画する
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_order(int stage_no)
{
	if (stage_no == -1)
		return true;

	// システムワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// システムワイプアウト中は、「フロント→黒」にワイプします。フロント＝黒、ネクスト＝フロント　なのです。
		if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{
			if (stage_no == TNM_STAGE_NEXT)
				stage_no = TNM_STAGE_FRONT;
			else
				return true;
		}

		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (!disp_proc_stage(stage_no, begin_sorter, end_sorter))
			return false;
	}
	// ＥＸコールワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ワイプするオーダーを描画する
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			begin_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
		}
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			end_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
		}
		if (!disp_proc_stage(stage_no, begin_sorter, end_sorter))
			return false;
	}
	// 通常のワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		// ワイプするオーダーを描画する
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (!disp_proc_stage(stage_no, begin_sorter, end_sorter))
			return false;
	}

	return true;
}

// ****************************************************************
// ワイプ描画：ワイプより上のオーダーを描画準備する
// ================================================================
bool C_tnm_wnd::disp_proc_over_order_ready(int stage_no)
{
	if (stage_no == -1)
		return true;

	// システムワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// ワイプより上はありえないので何もしない
	}
	// ＥＸコールワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ワイプより上を描画準備する
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			end_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
			if (!disp_proc_stage_ready(stage_no, end_sorter.plus_1(), S_tnm_sorter(INT_MAX, INT_MAX)))
				return false;
		}
	}
	// 通常のワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		// ワイプより上を描画準備する
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			if (!disp_proc_stage_ready(stage_no, end_sorter.plus_1(), S_tnm_sorter(INT_MAX, INT_MAX)))
				return false;
		}
	}

	return true;
}

// ****************************************************************
// ワイプ描画：ワイプより上のオーダーを描画する
// ================================================================
bool C_tnm_wnd::disp_proc_over_order(int stage_no)
{
	if (stage_no == -1)
		return true;

	// システムワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// ワイプより上はありえないので何もしない
	}
	// ＥＸコールワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ワイプより上を描画する
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			end_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
			if (!disp_proc_stage(stage_no, end_sorter.plus_1(), S_tnm_sorter(INT_MAX, INT_MAX)))
				return false;
		}
	}
	// 通常のワイプの場合
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			if (!disp_proc_stage(stage_no, end_sorter.plus_1(), S_tnm_sorter(INT_MAX, INT_MAX)))
				return false;
		}
	}

	return true;
}

// ****************************************************************
// ワイプ描画：ステージを描画する（準備）
// ================================================================
void disp_proc_effect_to_sprite_list(ARRAY<C_tnm_sprite *>& sprite_list, C_elm_effect* p_effect)
{
	ARRAY<C_tnm_sprite *>::iterator sprite_itr;
	ARRAY<C_tnm_sprite *>::iterator sprite_end;

	if (p_effect->is_use())	{

		// エフェクトのレンダーデータを取得
		S_d3d_render_param effect_rp;
		p_effect->get_rp(&effect_rp);

		// エフェクトが使用される場合、全スプライトに対して適用
		if (effect_rp.disp)	{
			sprite_end = sprite_list.end();
			for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
				// スプライトがオーダー内に入っている場合のみ適用
				if (p_effect->get_begin_sorter() <= (*sprite_itr)->rpex.sorter && (*sprite_itr)->rpex.sorter <= p_effect->get_end_sorter())	{
					tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &effect_rp);
				}
			}
		}
	}
}
void disp_proc_quake_to_sprite_list(ARRAY<C_tnm_sprite *>& sprite_list, C_elm_quake* p_quake)
{
	ARRAY<C_tnm_sprite *>::iterator sprite_itr;
	ARRAY<C_tnm_sprite *>::iterator sprite_end;

	if (p_quake->is_quake_doing())	{

		// クエイクのレンダーデータを取得
		S_d3d_render_param quake_rp;
		p_quake->get_rp(&quake_rp);

		// クエイクが使用される場合、全スプライトに対して適用
		if (quake_rp.disp)	{
			sprite_end = sprite_list.end();
			for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
				// スプライトがオーダー内に入っている場合のみ適用
				if (p_quake->get_begin_order() <= (*sprite_itr)->rpex.sorter.order && (*sprite_itr)->rpex.sorter.order <= p_quake->get_end_order())	{
					tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &quake_rp);
				}
			}
		}
	}
}


bool C_tnm_wnd::disp_proc_stage_ready(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter)
{
	//
	// ＥＸコール０
	//
	{
		// ルートスプライトを作成
		C_tnm_sprite root_sprite;
		C_tree2<C_tnm_sprite *> root;
		root.data = &root_sprite;

		// ステージのスプライトツリーを取得する
		STAGE(0, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);

		// スプライトをリストにする
		ARRAY<C_tnm_sprite *> sprite_list;
		disp_proc_sprite_tree_to_sprite_list(root, sprite_list);

		// スクリーンのエフェクトを取得
		{
			int effect_cnt = Gp_screen->effect_list().get_size();
			for (int i = 0; i < effect_cnt; i++)	{
				C_elm_effect* p_effect = &Gp_screen->effect_list()[i];
				disp_proc_effect_to_sprite_list(sprite_list, p_effect);
			}
		}

		// クエイク停止フラグが立っていない場合
		if (!Gp_local->pod.quake_stop_flag)	{

			// スクリーンのクエイクを取得
			int quake_cnt = Gp_screen->quake_list().get_size();
			for (int i = 0; i < quake_cnt; i++)	{
				C_elm_quake* p_quake = &Gp_screen->quake_list()[i];
				disp_proc_quake_to_sprite_list(sprite_list, p_quake);
			}
		}

		// ステージのエフェクトを取得
		{
			int effect_cnt = STAGE(0, stage_no)->m_effect_list.get_size();
			for (int i = 0; i < effect_cnt; i++)	{
				C_elm_effect* p_effect = &STAGE(0, stage_no)->m_effect_list[i];
				disp_proc_effect_to_sprite_list(sprite_list, p_effect);
			}
		}

		// クエイク停止フラグが立っていない場合
		if (!Gp_local->pod.quake_stop_flag)	{

			// ステージのクエイクを取得
			int quake_cnt = STAGE(0, stage_no)->m_quake_list.get_size();
			for (int i = 0; i < quake_cnt; i++)	{
				C_elm_quake* p_quake = &STAGE(0, stage_no)->m_quake_list[i];
				disp_proc_quake_to_sprite_list(sprite_list, p_quake);
			}
		}

		// キャプチャー位置を指定する場合
		if (Gp_global->capture_type == TNM_CAPTURE_TYPE_BUFFER)	{

			for (ARRAY<C_tnm_sprite *>::iterator itr = sprite_list.begin(); itr != sprite_list.end(); ++itr)	{
				(*itr)->rp.pos.x += Gp_global->capture_x;
				(*itr)->rp.pos.y += Gp_global->capture_y;
			}
		}
	}

	//
	// ＥＸコール１
	//
	if (Gp_excall->is_ready())	{

		// ルートスプライトを作成
		C_tnm_sprite root_sprite;
		C_tree2<C_tnm_sprite *> root;
		root.data = &root_sprite;

		// ステージのスプライトツリーを取得する
		STAGE(1, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);

		// スプライトをリストにする
		ARRAY<C_tnm_sprite *> sprite_list;
		disp_proc_sprite_tree_to_sprite_list(root, sprite_list);

		// ステージのエフェクトを取得
		{
			int effect_cnt = STAGE(1, stage_no)->m_effect_list.get_size();
			for (int i = 0; i < effect_cnt; i++)	{
				C_elm_effect* p_effect = &STAGE(1, stage_no)->m_effect_list[i];
				disp_proc_effect_to_sprite_list(sprite_list, p_effect);
			}
		}

		// クエイク停止フラグが立っていない場合
		if (!Gp_local->pod.quake_stop_flag)	{

			// ステージのクエイクを取得
			int quake_cnt = STAGE(1, stage_no)->m_quake_list.get_size();
			for (int i = 0; i < quake_cnt; i++)	{
				C_elm_quake* p_quake = &STAGE(1, stage_no)->m_quake_list[i];
				disp_proc_quake_to_sprite_list(sprite_list, p_quake);
			}
		}
	}


#if 0


	// ステージのスプライトツリーを取得する
	STAGE(0, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);
	//if (Gp_excall->is_ready())	{
	//	STAGE(1, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);
	//}
	//p_stage->get_sprite_tree(root, begin_sorter, end_sorter);

	// スプライトをリストにする
	ARRAY<C_tnm_sprite *> sprite_list;
	disp_proc_sprite_tree_to_sprite_list(root, sprite_list);

	//
	// スクリーンのレンダーデータを適用する
	//
	if (p_screen)	{

		// スクリーンのエフェクトを取得
		int effect_cnt = p_screen->effect_list().get_size();
		for (int i = 0; i < effect_cnt; i++)	{
			C_elm_effect* p_effect = &p_screen->effect_list()[i];
			if (p_effect->is_use())	{

				// エフェクトのレンダーデータを取得
				S_d3d_render_param effect_rp;
				p_effect->get_rp(&effect_rp);

				// エフェクトが使用される場合、全スプライトに対して適用
				if (effect_rp.disp)	{
					sprite_end = sprite_list.end();
					for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
						// スプライトがオーダー内に入っている場合のみ適用
						if (p_effect->get_begin_sorter() <= (*sprite_itr)->rpex.sorter && (*sprite_itr)->rpex.sorter <= p_effect->get_end_sorter())	{
							tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &effect_rp);
						}
					}
				}
			}
		}

		// クエイク停止フラグが立っていない場合
		if (!Gp_local->quake_stop_flag)	{

			// スクリーンのクエイクを取得
			int quake_cnt = p_screen->quake_list().get_size();
			for (int i = 0; i < quake_cnt; i++)	{
				C_elm_quake* p_quake = &p_screen->quake_list()[i];
				if (p_quake->is_quake_doing())	{

					// クエイクのレンダーデータを取得
					S_d3d_render_param quake_rp;
					p_quake->get_rp(&quake_rp);

					// クエイクが使用される場合、全スプライトに対して適用
					if (quake_rp.disp)	{
						sprite_end = sprite_list.end();
						for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
							// スプライトがオーダー内に入っている場合のみ適用
							if (p_quake->get_begin_order() <= (*sprite_itr)->rpex.sorter.order && (*sprite_itr)->rpex.sorter.order <= p_quake->get_end_order())	{
								tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &quake_rp);
							}
						}
					}
				}
			}
		}
	}

	//
	// ステージのレンダーデータを適用する
	//

	{

		// ステージのエフェクトを取得
		int effect_cnt = p_stage->m_effect_list.get_size();
		for (int i = 0; i < effect_cnt; i++)	{
			C_elm_effect* p_effect = &p_stage->m_effect_list[i];
			if (p_effect->is_use())	{

				// エフェクトのレンダーデータを取得
				S_d3d_render_param effect_rp;
				p_effect->get_rp(&effect_rp);

				// エフェクトが使用される場合、全スプライトに対して適用
				if (effect_rp.disp)	{

					sprite_end = sprite_list.end();
					for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
						// スプライトがオーダー内に入っている場合のみ適用
						if (p_effect->get_begin_sorter() <= (*sprite_itr)->rpex.sorter && (*sprite_itr)->rpex.sorter <= p_effect->get_end_sorter())	{
							tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &effect_rp);
						}
					}
				}
			}
		}

		// クエイク停止フラグが立っていない場合
		if (!Gp_local->quake_stop_flag)	{

			// ステージのクエイクを取得
			int quake_cnt = p_stage->m_quake_list.get_size();
			for (int i = 0; i < quake_cnt; i++)	{
				C_elm_quake* p_quake = &p_stage->m_quake_list[i];
				if (p_quake->is_quake_doing())	{

					// クエイクのレンダーデータを取得
					S_d3d_render_param quake_rp;
					p_quake->get_rp(&quake_rp);

					// クエイクが使用される場合、全スプライトに対して適用
					if (quake_rp.disp)	{
						sprite_end = sprite_list.end();
						for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
							// スプライトがオーダー内に入っている場合のみ適用
							if (p_quake->get_begin_order() <= (*sprite_itr)->rpex.sorter.order && (*sprite_itr)->rpex.sorter.order <= p_quake->get_end_order())	{
								tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &quake_rp);
							}
						}
					}
				}
			}
		}
	}
#endif

	return true;
}

// ****************************************************************
// ワイプ描画：ステージを描画する：第１段階：キャプチャーの前後で分ける
// ================================================================
bool C_tnm_wnd::disp_proc_stage(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter)
{
	// キャプチャーの範囲で分けてみよう…
	S_tnm_sorter capture_sorter = Gp_global->capture_sorter;

	// キャプチャーを挟む場合
	if (Gp_global->capture_type != TNM_CAPTURE_TYPE_NONE && stage_no == TNM_STAGE_FRONT && begin_sorter <= capture_sorter && capture_sorter <= end_sorter)	{

		// キャプチャーより下
		disp_proc_stage_sub(stage_no, begin_sorter, capture_sorter);

		// キャプチャー
		if (Gp_global->capture_type == TNM_CAPTURE_TYPE_SAVE_THUMB)	{
			if (!disp_proc_capture_for_save_thumb())	// この中でターゲットバッファが変わってます！
				return false;
		}
		else if (Gp_global->capture_type == TNM_CAPTURE_TYPE_OBJECT)	{
			if (!disp_proc_capture_for_object())		// この中でターゲットバッファが変わってます！
				return false;
		}
		else if (Gp_global->capture_type == TNM_CAPTURE_TYPE_THUMB)	{
			if (!disp_proc_capture_for_thumb())			// この中でターゲットバッファが変わってます！
				return false;
		}
		else if (Gp_global->capture_type == TNM_CAPTURE_TYPE_BUFFER)	{
			if (!disp_proc_capture_to_buffer())			// この中でターゲットバッファが変わってます！
				return false;
		}
		else if (Gp_global->capture_type == TNM_CAPTURE_TYPE_TWEET)	{
			if (!disp_proc_capture_for_tweet())			// この中でターゲットバッファが変わってます！
				return false;
		}

		// ローカルファイルに保存（セーブサムネイル）
		if (Gp_global->capture_type == TNM_CAPTURE_TYPE_THUMB && m_capture_texture_for_object)	{
			int width = m_capture_texture_for_object->get_width();
			int height = m_capture_texture_for_object->get_height();

			D3DLOCKED_RECT locked_rect;
			if (!m_capture_texture_for_object->lock_rect(0, &locked_rect, NULL, D3DLOCK_DISCARD))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ローカルファイルへのキャプチャーに失敗しました。\n") + get_last_error_msg());
			}
			else	{

				time_t now = time(NULL);
				TSTR file_path = Gp_dir->save_dir + _T("\\") + tostr(now) + _T(".png");
				tnm_create_png_from_texture_and_save_to_file(file_path, width, height, &locked_rect, false);	// アルファ値を使わない

				Gp_global->capture_time = (int)now;

				m_capture_texture_for_object->unlock_rect(0);
			}
			m_capture_texture_for_object.reset();
		}
		// ローカルファイルに保存（png 出力）
		else if (Gp_global->capture_type == TNM_CAPTURE_TYPE_BUFFER && !Gp_global->capture_file_name.empty())	{

			int width = m_capture_temp_buffer_texture->get_width();
			int height = m_capture_temp_buffer_texture->get_height();

			// こっちはセーブフォルダ固定
			TSTR file_path = Gp_dir->save_dir + _T("\\") + tostr(Gp_global->capture_file_name) + _T(".png");

			// 指定のファイルに保存
			D3DLOCKED_RECT locked_rect;
			if (!m_capture_temp_buffer_texture->lock_rect(0, &locked_rect, NULL, D3DLOCK_DISCARD))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーバッファのロックに失敗しました。\n") + get_last_error_msg());
			}
			else	{

				tnm_create_png_from_texture_and_save_to_file(file_path, width, height, &locked_rect, true);	// アルファ値を使う

				m_capture_temp_buffer_texture->unlock_rect(0);
			}
		}

		// 画面転送を行う場合は続けて処理
		if (Gp_global->present_flag)	{

			// レンダーターゲットをターゲットバッファに戻す
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// キャプチャーより上
			if (capture_sorter < end_sorter)	{
				disp_proc_stage_sub(stage_no, capture_sorter.plus_1(), end_sorter);
			}
		}
	}
	else	{

		// 指定された全てのオーダー
		disp_proc_stage_sub(stage_no, begin_sorter, end_sorter);
	}

	return true;
}

// ****************************************************************
// ワイプ描画：ステージを描画する：第２段階：スプライトリストを構築して描画する
// ================================================================
bool C_tnm_wnd::disp_proc_stage_sub(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter)
{
	// ルートスプライトを作成
	C_tnm_sprite root_sprite;
	C_tree2<C_tnm_sprite *> root;
	root.data = &root_sprite;

	// ステージのスプライトツリーを取得する
	STAGE(0, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);

	// メッセージバック
	if (!Gp_local->pod.msg_back_disp_off)	{
		Gp_msg_back->get_sprite_tree(root);
	}

	// ＥＸコール
	if (Gp_excall->is_ready())	{
		STAGE(1, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);
	}

	// スプライトをリストにする
	ARRAY<C_tnm_sprite *> sprite_list;
	disp_proc_sprite_tree_to_sprite_list(root, sprite_list);

	// スプライトリストを描画する
	if (!disp_proc_sprite_list(sprite_list))
		return false;

	return true;
}

// ****************************************************************
// スプライトツリーから描画するスプライトリストを作成する
// ================================================================

inline bool node_cmp_default(C_tree2<C_tnm_sprite *> lhs, C_tree2<C_tnm_sprite *> rhs)
{
	return lhs.data->rpex.sorter < rhs.data->rpex.sorter;
}
inline bool node_cmp_pat_no(C_tree2<C_tnm_sprite *> lhs, C_tree2<C_tnm_sprite *> rhs)
{
	if (lhs.data->rpex.sorter < rhs.data->rpex.sorter)	return true;
	if (lhs.data->rpex.sorter > rhs.data->rpex.sorter)	return false;
	if (lhs.data->get_texture(0).get() < rhs.data->get_texture(0).get())	return true;
	return false;
}
inline bool node_cmp_test(C_tree2<C_tnm_sprite *> lhs, C_tree2<C_tnm_sprite *> rhs)
{
	if (lhs.data->rpex.sorter < rhs.data->rpex.sorter)	return true;
	if (lhs.data->rpex.sorter > rhs.data->rpex.sorter)	return false;
	if (lhs.data->rp.pos.z < rhs.data->rp.pos.z)		return true;
	if (lhs.data->rp.pos.z > rhs.data->rp.pos.z)		return false;
	if (lhs.data->get_texture(0).get() < rhs.data->get_texture(0).get())	return true;
	return false;
}

bool C_tnm_wnd::disp_proc_sprite_tree_to_sprite_list(C_tree2<C_tnm_sprite *>& node, ARRAY<C_tnm_sprite *>& sprite_list)
{
	// ノードのスプライトをリストに追加する
	if (node.data->type != E_d3d_sprite_type_none && node.data->rp.disp)	{

		// スプライトを登録する
		sprite_list.push_back(node.data);
	}

	int child_cnt = (int)node.child_list.size();
	if (child_cnt > 0)	{

		// ノードの子供をソートする
		if (node.data->rpex.child_sort_type == TNM_SPRITE_SORT_TYPE_DEFAULT)	{
			std::stable_sort(node.child_list.begin(), node.child_list.end(), node_cmp_default);
		}

		// 全ての子供に対して同様の処理を行う
		for (int i = 0; i < child_cnt; i++)
			disp_proc_sprite_tree_to_sprite_list(node.child_list[i], sprite_list);
	}

	return true;
}

// ****************************************************************
// スプライトリストを描画する
// ================================================================
bool C_tnm_wnd::disp_proc_sprite_list_sub(C_elm_world* p_world, ARRAY<C_tnm_sprite *>& sprite_list)
{
	// ３Ｄの場合
	/*
	if (world_no >= 0)	{

		// ブレンドするものとしないものに分ける
		ARRAY<C_tnm_sprite *> blend_sprite_list, no_blend_sprite_list;
		for (ARRAY<C_tnm_sprite *>::iterator itr = sprite_list.begin(); itr != sprite_list.end(); ++itr)	{
			if ((*itr)->rp.alpha_blend)	{
				blend_sprite_list.push_back(*itr);
			}
			else	{
				no_blend_sprite_list.push_back(*itr);
			}
		}

		// ブレンドしないものを描画
		{
#if 0
			C_elm_world* p_world = &(*Gp_world_list)[world_no];
			int sprite_cnt = (int)no_blend_sprite_list.size();
			ARRAY<float> z_value(sprite_cnt);
			for (int i = 0; i < sprite_cnt; i++)	{
				D3DXVECTOR3 a = *(D3DXVECTOR3 *)&no_blend_sprite_list[i]->rp.pos - m_view.m_camera_eye;
				D3DXVECTOR3 b = m_view.m_camera_dir;
				D3DXVec3Normalize(&b, &b);
				float dot = D3DXVec3Dot(&a, &b);
				z_value[i] = dot;
			}

			for (int i = 0; i < sprite_cnt; i++)	{
				int min_i = i;
				for (int j = i + 1; j < sprite_cnt; j++)	{
					if (z_value[j] < z_value[min_i])	{
						min_i = j;
					}
				}
				std::swap(z_value[i], z_value[min_i]);
				std::swap(no_blend_sprite_list[i], no_blend_sprite_list[min_i]);
			}
			for (ARRAY<C_tnm_sprite *>::iterator itr = no_blend_sprite_list.begin(); itr != no_blend_sprite_list.end(); ++itr)	{
				m_view.regist_sprite(*itr);
			}
			if (!m_view.render())	{
				return false;
			}
			m_view.clear_sprite_list();
#else
			for (ARRAY<C_tnm_sprite *>::iterator itr = no_blend_sprite_list.begin(); itr != no_blend_sprite_list.end(); ++itr)	{
				m_view.regist_sprite(*itr);
			}
			if (!m_view.render())	{
				return false;
			}
			m_view.clear_sprite_list();
#endif
		}
		// ブレンドするものを描画
		{
			// ３Ｄでアルファブレンドを行うので、Ｚソートを行う
			C_elm_world* p_world = &(*Gp_world_list)[world_no];
			int sprite_cnt = (int)blend_sprite_list.size();
			ARRAY<float> z_value(sprite_cnt);
			for (int i = 0; i < sprite_cnt; i++)	{
				D3DXVECTOR3 a = *(D3DXVECTOR3 *)&blend_sprite_list[i]->rp.pos - m_view.m_camera_eye;
				D3DXVECTOR3 b = m_view.m_camera_dir;
				D3DXVec3Normalize(&b, &b);
				float dot = D3DXVec3Dot(&a, &b);
				z_value[i] = dot;
			}

			for (int i = 0; i < sprite_cnt; i++)	{
				int max_i = i;
				for (int j = i + 1; j < sprite_cnt; j++)	{
					if (z_value[j] > z_value[max_i])	{
						max_i = j;
					}
				}
				std::swap(z_value[i], z_value[max_i]);
				std::swap(blend_sprite_list[i], blend_sprite_list[max_i]);
			}
			for (ARRAY<C_tnm_sprite *>::iterator itr = blend_sprite_list.begin(); itr != blend_sprite_list.end(); ++itr)	{
				m_view.regist_sprite(*itr);
			}
			if (!m_view.render())	{
				return false;
			}
			m_view.clear_sprite_list();
		}

		// のスプライトリストをクリア
		sprite_list.clear();

		// レンダーターゲットの深度ステンシルバッファをクリア
		m_view.clear_render_target_z_buffer();
		m_view.clear_render_target_stencil();
	}

	// ３Ｄでない場合
	else	{
	*/

		// 貯まったスプライトを登録
		for (ARRAY<C_tnm_sprite *>::iterator itr = sprite_list.begin(); itr != sprite_list.end(); ++itr)	{
			m_view.regist_sprite(*itr);
		}

		// 登録したスプライトをレンダー
		if (!m_view.render())	{
			return false;
		}
		// ビューのスプライトリストをクリア
		m_view.clear_sprite_list();

		// スプライトリストをクリア
		sprite_list.clear();

		// レンダーターゲットの深度ステンシルバッファをクリア
		m_view.clear_render_target_z_buffer();
		m_view.clear_render_target_stencil();
//	}

	return true;
}

bool C_tnm_wnd::disp_proc_sprite_list(ARRAY<C_tnm_sprite *>& sprite_list)
{
	C_size gsize = Gp_global->game_screen_size;

	ARRAY<C_tnm_sprite *>::iterator sprite_itr;
	ARRAY<C_tnm_sprite *>::iterator sprite_end;

	// 全てのスプライトの頂点バッファを更新する
	ARRAY<C_tnm_sprite *> sprite_list_2;
	if (!tnm_update_vertex_list(sprite_list, sprite_list_2))
		return false;

	// ビューのスプライトリストをクリア
	m_view.clear_sprite_list();

	// アルファブレンド用のスプライトリスト
	ARRAY<C_tnm_sprite *> sprite_list_3;

	// 全てのスプライトを描画する
	C_elm_world* p_last_world = NULL;
	for (sprite_itr = sprite_list_2.begin(); sprite_itr != sprite_list_2.end(); ++ sprite_itr)	{
		S_d3d_render_param* rp = &(*sprite_itr)->rp;
		C_elm_world* p_world = (*sprite_itr)->rpex.p_world;

		// ワールドが変わったら描画
		if (p_world != p_last_world)	{

			// 貯まったスプライトを描画
			if (!disp_proc_sprite_list_sub(p_last_world, sprite_list_3))
				return false;

			// ワールドを準備する
			if (p_world != NULL)	{

				// ビューのカメラを設定

				// 2D通常モード
				if (p_world->get_mode() == TNM_CAMERA_MODE_2D)	{
				}
				// 3D通常モード
				else if (p_world->get_mode() == TNM_CAMERA_MODE_3D)	{

					D3DXVECTOR3 camera_eye, camera_pint, camera_up;
					float camera_aspect, camera_view_angle, zn, zf;
					camera_eye.x = (float)p_world->get_camera_eye_total_x() / TNM_WORLD_UNIT;
					camera_eye.y = (float)p_world->get_camera_eye_total_y() / TNM_WORLD_UNIT;
					camera_eye.z = (float)p_world->get_camera_eye_total_z() / TNM_WORLD_UNIT;
					camera_pint.x = (float)p_world->get_camera_pint_total_x() / TNM_WORLD_UNIT;
					camera_pint.y = (float)p_world->get_camera_pint_total_y() / TNM_WORLD_UNIT;
					camera_pint.z = (float)p_world->get_camera_pint_total_z() / TNM_WORLD_UNIT;
					camera_up.x = (float)p_world->get_camera_up_total_x() / TNM_WORLD_UNIT;
					camera_up.y = (float)p_world->get_camera_up_total_y() / TNM_WORLD_UNIT;
					camera_up.z = (float)p_world->get_camera_up_total_z() / TNM_WORLD_UNIT;
					camera_view_angle = D3DXToRadian((float)p_world->get_camera_view_angle() / TNM_ANGLE_UNIT);
					camera_aspect = (float)gsize.cx / gsize.cy;
					zn = 1.0f;
					zf = 10000.0f;

					// ライトを設定してみる
					C_d3d_light light;
					ZeroMemory(&light, sizeof(light));
					light.Type = D3DLIGHT_DIRECTIONAL;
					light.Position.x = camera_eye.x;
					light.Position.y = camera_eye.y;
					light.Position.z = camera_eye.z;
					light.Diffuse.a = 1.0f;
					light.Diffuse.r = 1.0f;
					light.Diffuse.g = 1.0f;
					light.Diffuse.b = 1.0f;
					light.Ambient.a = 1.0f;
					light.Ambient.r = 3.0f;
					light.Ambient.g = 3.0f;
					light.Ambient.b = 3.0f;
					D3DXVECTOR3 dir(0.0f, 1.0f, 0.0f);
					D3DXVec3Normalize(&dir, &dir);
					light.Direction = dir;

					// ライトの計算方法
					light.camera_light_calc_type = D3D_AMBIENT_TYPE_MULTIPLE;
					light.ambient_light_calc_type = D3D_AMBIENT_TYPE_MULTIPLE;
					G_d3d.state.set_light(0, &light);

					// カメラの向きを設定
					m_view.m_camera_dir = camera_pint - camera_eye;
					m_view.m_camera_eye = camera_eye;
					D3DXVec3Normalize(&m_view.m_camera_dir, &m_view.m_camera_dir);

					// フォグ
					m_view.m_fog_use = 0;
					m_view.m_fog_x = (float)Gp_fog->get_total_x();
					m_view.m_fog_near = (float)Gp_fog->get_fog_near();
					m_view.m_fog_far = (float)Gp_fog->get_fog_far();

					// ビュー行列を設定
					D3DXMatrixLookAtLH(&m_view.m_mat_view, &camera_eye, &camera_pint, &camera_up);

					// 射影行列を設定
					float camera_h_half = zn * tan(camera_view_angle / 2);
					float camera_w_half = camera_h_half * camera_aspect;
					D3DXMatrixPerspectiveOffCenterLH(&m_view.m_mat_proj, - camera_w_half, camera_w_half, - camera_h_half, camera_h_half, zn, zf);
				}

				// 簡易モード
				else	{

					D3DXVECTOR3 camera_eye, camera_pint, camera_up;
					float camera_aspect, camera_view_angle, zn, zf;
					float tan_y;

					camera_view_angle = D3DXToRadian((float)p_world->get_camera_view_angle() / TNM_ANGLE_UNIT);
					tan_y = tan(camera_view_angle / 2);
					camera_aspect = - (float)gsize.cx / gsize.cy;
					camera_eye.x = (*sprite_itr)->rp.pos.x;
					camera_eye.y = (*sprite_itr)->rp.pos.y;
					camera_eye.z = - gsize.cy / 2 / tan_y;
					camera_pint.x = (*sprite_itr)->rp.pos.x;
					camera_pint.y = (*sprite_itr)->rp.pos.y;
					camera_pint.z = 0.0f;
					camera_up.x = 0.0f;
					camera_up.y = -1.0f;
					camera_up.z = 0.0f;
						// ↑マイナスにすることで反転しています！

					zn = 1.0f;
					zf = 10000.0f;

					// ビュー行列の設定
					D3DXMatrixLookAtLH(&m_view.m_mat_view, &camera_eye, &camera_pint, &camera_up);

					// 射影行列の設定
					D3DXMatrixPerspectiveOffCenterLH(&m_view.m_mat_proj, 
						(0.0f - rp->pos.x / (gsize.cx / 2)) * camera_aspect * zn * zf / (zf - zn) * tan_y, 
						(2.0f - rp->pos.x / (gsize.cx / 2)) * camera_aspect * zn * zf / (zf - zn) * tan_y, 
						(-2.0f + rp->pos.y / (gsize.cy / 2)) * zn * zf / (zf - zn) * tan_y, 
						(0.0f + rp->pos.y / (gsize.cy / 2)) * zn * zf / (zf - zn) * tan_y, 
						zn, zf);
				}
			}
		}

		// 次のワールドへ
		p_last_world = p_world;

		// スプライトを登録
		sprite_list_3.push_back(*sprite_itr);
	}

	// 貯まったスプライトを描画
	if (!disp_proc_sprite_list_sub(p_last_world, sprite_list_3))
		return false;

	return true;
}

// ****************************************************************
// スプライトを描画する
// ================================================================
bool C_tnm_wnd::disp_proc_sprite(C_d3d_sprite* p_sprite)
{
	// スプライトリストをクリア
	m_view.clear_sprite_list();

	// スプライトを追加
	m_view.regist_sprite(p_sprite);

	// ステージを描画
	if (!m_view.render())
		return false;

	return true;
}

// ****************************************************************
// 最前面を描画する
// ================================================================
bool C_tnm_wnd::disp_proc_top_most()
{
	// ルートスプライトを作成
	C_tnm_sprite root_sprite;
	C_tree2<C_tnm_sprite *> root;
	root.data = &root_sprite;

	// メッセージバック
	if (!Gp_local->pod.msg_back_disp_off)	{
		Gp_msg_back->get_sprite_tree(root);
	}

	// スプライトをリストにする
	ARRAY<C_tnm_sprite *> sprite_list;
	disp_proc_sprite_tree_to_sprite_list(root, sprite_list);

	// スプライトリストの頂点バッファを更新する
	ARRAY<C_tnm_sprite *> sprite_list_2;
	if (!tnm_update_vertex_list(sprite_list, sprite_list_2))
		return false;

	// スプライトリストをクリア
	m_view.clear_sprite_list();

	// スプライトを追加
	for (ARRAY<C_tnm_sprite *>::iterator sprite = sprite_list_2.begin(); sprite != sprite_list_2.end(); ++ sprite)
		m_view.regist_sprite(*sprite);

	// バックバッファにステージを描画
	if (!m_view.render())
		return false;

	return true;
}

// ****************************************************************
// キャプションを更新
// ================================================================
void C_tnm_wnd::update_caption()
{
	STATE cd;

	// 現在のパラメータを構築する
	cd.title = tnm_get_full_title();

	// ウィンドウタイトルを設定
	if (m_first || cd.title != m_ld.title)
		set_text(cd.title);

	// パラメータを保存
	m_ld = cd;

	// 初回フラグをおろす
	m_first = false;
}
