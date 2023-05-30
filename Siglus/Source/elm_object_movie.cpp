#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"element/elm_object.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// オブジェクト：ムービーオブジェクトの作成
// ================================================================
bool C_elm_object::create_movie(TSTR file_path, bool loop, bool auto_free, bool real_time, bool ready_only)
{
	// タイプ別のパラメータを初期化する
	init_type(true);

	// パラメータを設定
	m_op.type = TNM_OBJECT_TYPE_MOVIE;
	m_op.file_path = file_path;
	m_op.movie.loop_flag = loop;
	m_op.movie.auto_free_flag = auto_free;
	m_op.movie.real_time_flag = real_time;
	m_op.movie.pause_flag = ready_only ? true : false;

	// ムービーオブジェクトを再構築する
	if (!restruct_movie())
		return false;

	return true;
}

bool C_elm_object::restruct_movie()
{
	// ファイルを検索
	TSTR file_path = tnm_find_omv(m_op.file_path);
	if (file_path.empty())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ムービーファイル ") + m_op.file_path + _T(".omv が見つかりませんでした。"));
		init_type(true);		// タイプを初期化
		return false;
	}

	// omv を準備する
	m_omv_player = BSP<C_omv_player>(new C_omv_player);
	if (!m_omv_player->ready_omv(file_path, m_op.movie.loop_flag ? true : false, false))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ムービーオブジェクト ") + m_op.file_path + _T(" の作成に失敗しました。\n") + m_omv_player->get_last_error());
		init_type(true);		// タイプを初期化
		return false;
	}

	// テクスチャを作成する
	// 毎フレーム更新するので D3DUSAGE_DYNAMIC を指定する
	// その場合 D3DPOOL_MANAGED は使えないので、D3DPOOL_DEFAULT を使う
	// デバイスが消失すると作り直す必要があるので、最後の引数に true を与えてマネージャに管理させる
	S_omv_theora_info omv_theora_info;
	m_omv_player->get_theora_info(&omv_theora_info);

	BSP<C_d3d_texture> texture = G_rsm.create_texture(_T("ﾑｰﾋﾞｰｵﾌﾞｼﾞｪｸﾄ"), omv_theora_info.size.cx, omv_theora_info.size.cy, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);							// 縮小×
	//BSP<C_d3d_texture> texture = G_rsm.create_texture(_T("ﾑｰﾋﾞｰｵﾌﾞｼﾞｪｸﾄ"), omv_theora_info.size.cx, omv_theora_info.size.cy, 0, D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);	// 縮小×
	//BSP<C_d3d_texture> texture = G_rsm.create_texture(_T("ﾑｰﾋﾞｰｵﾌﾞｼﾞｪｸﾄ"), omv_theora_info.size.cx, omv_theora_info.size.cy, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);							// 縮小×
	//BSP<C_d3d_texture> texture = G_rsm.create_texture(_T("ﾑｰﾋﾞｰｵﾌﾞｼﾞｪｸﾄ"), omv_theora_info.size.cx, omv_theora_info.size.cy, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, true);						// 縮小◎
	if (!texture)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ムービーオブジェクト ") + m_op.file_path + _T(" の作成に失敗しました。\n") + get_last_error_msg());
		init_type(true);		// タイプを初期化
		return false;
	}

	// テクスチャをクリアしておく（必須。差分形式でデータが書き込まれるため、これがないとゴミが残る）
	texture->clear_color_ex(C_argb(0, 0, 0, 0));

	// アルバムを作成する
	m_album = BSP<C_d3d_album>(new C_d3d_album);
	m_album->add_texture(texture);

	return true;
}

// ****************************************************************
// オブジェクト：ムービーオブジェクトのフレーム処理
// ================================================================
void C_elm_object::movie_frame()
{
	// ビデオを取得する
	if (m_omv_player && m_album && m_album->get_texture(0))	{
		BSP<C_d3d_texture> texture = m_album->get_texture(0);

		// 時間に達していない場合、ムービーが更新されないことがあります。
		// その場合、前フレームのムービーを破棄してはいけません。
		// よって D3DLOCK_DISCARD を指定することはできません。

		// ループの場合は先に時刻をループさせておく
		// 「途中でループをやめる」の処理のために必須です。
		if (m_op.movie.loop_flag)	{
			int movie_total_time = m_omv_player->get_theora_total_frame_time();
			if (movie_total_time > 0)	{
				if (m_omv_timer > movie_total_time)	{
					m_omv_timer %= movie_total_time;
				}
			}
		}

		int new_frame_no;

		// dirty な場合は強制的に更新することになりました（2014/9/16）
		bool update_by_force = texture->is_dirty();
		if (m_omv_player->check_need_update(m_omv_timer, &new_frame_no, update_by_force))	{

#if 0
			// 案１：D3DPOOL_SYSTEMMEM なサーフェスに描画してから UpdateSurface で転送。
			// 遅いし安定しない。あうと。
			D3DLOCKED_RECT locked_rect;
			if (m_omv_surface->lock_rect(&locked_rect, NULL, D3DLOCK_DISCARD))	{

				m_omv_player->get_video_for_frame(new_frame_no, (BYTE *)locked_rect.pBits, locked_rect.Pitch, NULL);
				m_omv_surface->unlock_rect();

				C_d3d_surface dst_surface = texture->get_surface(0);
				C_rect src_rect(C_point(0, 0), m_omv_surface->get_size());
				C_point dst_point(0, 0);
				G_d3d.device->UpdateSurface(m_omv_surface->body(), &src_rect, dst_surface.body(), &dst_point);
			}
#else
			// 案２：テクスチャをロックして書き込み。
			// ムービーを全てキーフレームにしたので、D3DLOCK_DISCARD が指定できるようになった。
			// おかげでロックにかかる時間がほぼ０になり、結局これが最適になった。
			D3DLOCKED_RECT locked_rect;
			if (texture->lock_rect(0, &locked_rect, NULL, D3DLOCK_DISCARD))	{
				m_omv_player->get_video_for_frame(new_frame_no, (BYTE *)locked_rect.pBits, locked_rect.Pitch, false, update_by_force, NULL);
				texture->add_edge_for_filterring(&locked_rect);
				texture->unlock_rect(0);
			}

			// dirty フラグを降ろす
			texture->set_dirty_flag(false);
#endif
		}

		// 基本パラメータを設定
		trp_to_rp(&m_sprite->rpex, &m_sprite->rp);

		// テクスチャを設定
		m_sprite->set_texture(texture);
		m_sprite->rp.size_fit_to_texture = 0;
	}
}

// ****************************************************************
// オブジェクト：ムービーをシーク
// ================================================================
void C_elm_object::seek_movie(int time)
{
	m_omv_timer = time;

	if (m_omv_player)	{
		m_omv_player->update_time_only(time);
	}
}

// ****************************************************************
// オブジェクト：ムービーのシーク位置を設定
// ================================================================
int C_elm_object::get_movie_seek_time()
{
	if (m_omv_player)	{
		int movie_total_time = m_omv_player->get_theora_total_frame_time();
		if (movie_total_time <= 0)	{
			return 0;
		}
		else	{
			return m_omv_timer % movie_total_time;
		}
	}
	else	{
		return 0;
	}
}

// ****************************************************************
// オブジェクト：ムービーを判定
// ================================================================
bool C_elm_object::check_movie()
{
	if (m_omv_player)	{

		// 再生中を判定
		if (m_omv_player->is_playing())
			return true;
	}

	return false;
}

// ****************************************************************
// オブジェクト：ムービー終了を待つ
// ================================================================
void C_elm_object::wait_movie(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_OBJ_MOV_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// オブジェクト：ムービーのループを終了させる
// ================================================================
void C_elm_object::end_movie_loop()
{
	if (m_omv_player)	{
		m_omv_player->end_loop();
	}
}

// ****************************************************************
// オブジェクト：ムービーの自動解放フラグを設定する
// ================================================================
void C_elm_object::set_movie_auto_free(bool value)
{
	m_op.movie.auto_free_flag = value;
}
