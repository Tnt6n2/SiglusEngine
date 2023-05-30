#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_sorter.h"
#include	"element/elm_mwnd_key_icon.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// 初期化
// ================================================================
void C_elm_mwnd_key_icon::init()
{
	// パラメータ
	m_template_no = -1;
	m_icon_appear = false;
	m_icon_mode = 0;

	// 作業変数
	m_anime_start_flag = false;
	m_anime_start_time = 0;

	// テンプレートを再構築
	restruct_template();
}

// ****************************************************************
// 再初期化
// ================================================================
void C_elm_mwnd_key_icon::reinit()
{
	// 解放
	release();

	// 初期化
	m_template_no = -1;
	m_icon_appear = false;
	m_icon_mode = 0;

	// 作業変数
	m_anime_start_flag = false;
	m_anime_start_time = 0;

	// テンプレートを再構築
	restruct_template();
}

// ****************************************************************
// 解放
// ================================================================
void C_elm_mwnd_key_icon::release()
{
	// スプライトをクリア
	m_album_key_icon.reset();
	m_album_page_icon.reset();
	m_sprite_icon.release();
}

// ****************************************************************
// テンプレートの値を設定
// ================================================================
void C_elm_mwnd_key_icon::set_template(int template_no)
{
	// 解放
	release();

	// テンプレート番号
	m_template_no = template_no;

	// テンプレートを再構築
	restruct_template();
}

// ****************************************************************
// セーブ
// ================================================================
void C_elm_mwnd_key_icon::save(C_tnm_save_stream& stream)
{
	// テンプレート番号
	stream.save(m_template_no);

	// 現在のパラメータ
	stream.save(m_icon_mode);
	stream.save(m_icon_pos);
}

// ****************************************************************
// ロード
// ================================================================
void C_elm_mwnd_key_icon::load(C_tnm_save_stream& stream)
{
	// テンプレート番号
	stream.load(m_template_no);

	// テンプレートを再構築
	restruct_template();

	// パラメータ
	stream.load(m_icon_mode);
	stream.load(m_icon_pos);
}

// ****************************************************************
// コピー
// ================================================================
void C_elm_mwnd_key_icon::copy(C_elm_mwnd_key_icon* src)
{
	// 解放
	release();

	// テンプレート番号
	m_template_no = src->m_template_no;

	// テンプレートを再構築
	restruct_template();

	// パラメータ
	m_icon_appear = src->m_icon_appear;
	m_icon_mode = src->m_icon_mode;
	m_icon_pos = src->m_icon_pos;

	// 作業パラメータ
	m_anime_start_flag = src->m_anime_start_flag;
	m_anime_start_time = src->m_anime_start_time;
}

// ****************************************************************
// テンプレートを再構築（内部用：解放しない）
// ================================================================
void C_elm_mwnd_key_icon::restruct_template()
{
	// テンプレートなし
	if (m_template_no < 0)
		return;

	// キー待ちアイコン
	int key_icon_no = Gp_ini->mwnd_waku[m_template_no].icon_no;
	if (0 <= key_icon_no && key_icon_no < Gp_ini->icon_cnt)
	{
		// アイコンのファイル名
		TSTR file_name = Gp_ini->icon[key_icon_no].file_name;

		// ファイル名の指定がある場合、ファイルを読み込む
		if (!file_name.empty())
			m_album_key_icon = tnm_load_pct_d3d(file_name, false, false);
	}

	// ページ待ちアイコン
	int page_icon_no = Gp_ini->mwnd_waku[m_template_no].page_icon_no;
	if (0 <= page_icon_no && page_icon_no < Gp_ini->icon_cnt)
	{
		TSTR file_name = Gp_ini->icon[page_icon_no].file_name;
		if (!file_name.empty())
			m_album_page_icon = tnm_load_pct_d3d(file_name, false, false);
	}
}

// ****************************************************************
// 表示／非表示を設定
// ================================================================
void C_elm_mwnd_key_icon::set_appear(bool appear)
{
	m_icon_appear = appear;

	// 非表示にした場合はアニメを止める
	if (!appear)
	{
		m_anime_start_flag = false;
	}
}

// ****************************************************************
// フレーム初期化処理
// ================================================================
void C_elm_mwnd_key_icon::frame_init()
{
	// スプライトを初期化
	m_sprite_icon.init();
}

// ****************************************************************
// フレーム処理
// ================================================================
void C_elm_mwnd_key_icon::frame(int cur_time, C_rect window_rect, const S_tnm_render_param* parent_trp)
{
	// テンプレートがなければ何もしない
	if (m_template_no < 0 || Gp_ini->mwnd_waku_cnt <= m_template_no)
		return;

	// 出現していない場合は何もしない
	if (!m_icon_appear)
		return;

	// アニメがスタートしていない場合はスタートする
	if (!m_anime_start_flag)
	{
		m_anime_start_flag = true;
		m_anime_start_time = cur_time;
	}

	// モードによってアイコン番号が変わる
	int icon_no = (m_icon_mode == 0 ? Gp_ini->mwnd_waku[m_template_no].icon_no : Gp_ini->mwnd_waku[m_template_no].page_icon_no);
	if (icon_no < 0 || Gp_ini->icon_cnt <= icon_no)
		return;

	// パターン番号と速度を求める
	int anime_speed = Gp_ini->icon[icon_no].anime_speed;
	int anime_pat_cnt = Gp_ini->icon[icon_no].anime_pat_cnt;
	if (anime_speed == 0 || anime_pat_cnt == 0)
		return;

	int pat_no = (cur_time - m_anime_start_time) / anime_speed % anime_pat_cnt;

	// モードによって使うアルバムを変える
	C_d3d_album* p_album = (m_icon_mode == 0 ? m_album_key_icon.get() : m_album_page_icon.get());

	// ファイルがある場合
	if (p_album && p_album->get_texture(pat_no))
	{
		// アイコンの位置を求める（どの角にくっつくかで場合わけ）
		C_point pos = C_point(0, 0);
		if (Gp_ini->mwnd_waku[m_template_no].icon_pos_type == 0)
		{
			switch (Gp_ini->mwnd_waku[m_template_no].icon_pos_base)
			{
				case 0:
					pos.x = window_rect.left + Gp_ini->mwnd_waku[m_template_no].icon_pos.x;
					pos.y = window_rect.top + Gp_ini->mwnd_waku[m_template_no].icon_pos.y;
					break;
				case 1:
					pos.x = window_rect.right - Gp_ini->mwnd_waku[m_template_no].icon_pos.x - p_album->get_texture(pat_no)->get_original_width();
					pos.y = window_rect.top + Gp_ini->mwnd_waku[m_template_no].icon_pos.y;
					break;
				case 2:
					pos.x = window_rect.left + Gp_ini->mwnd_waku[m_template_no].icon_pos.x;
					pos.y = window_rect.bottom - Gp_ini->mwnd_waku[m_template_no].icon_pos.y - p_album->get_texture(pat_no)->get_original_height();
					break;
				case 3:
					pos.x = window_rect.right - Gp_ini->mwnd_waku[m_template_no].icon_pos.x - p_album->get_texture(pat_no)->get_original_width();
					pos.y = window_rect.bottom - Gp_ini->mwnd_waku[m_template_no].icon_pos.y - p_album->get_texture(pat_no)->get_original_height();
					break;
			}
		}
		else if (Gp_ini->mwnd_waku[m_template_no].icon_pos_type == 1)
		{
			if (Gp_ini->tateyoko_mode == 1)
			{
				pos.x = window_rect.right - m_icon_pos.x - p_album->get_texture(pat_no)->get_original_width();
				pos.y = window_rect.top + m_icon_pos.y;
			}
			else
			{
				pos.x = window_rect.left + m_icon_pos.x;
				pos.y = window_rect.top + m_icon_pos.y;
			}
		}

		// レンダーパラメータ
		S_tnm_render_param icon_trp;
		icon_trp.disp = true;
		icon_trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.waku_layer_rep);
		icon_trp.pos_x = (float)pos.x;
		icon_trp.pos_y = (float)pos.y;

		// 親のレンダーパラメータを加える
		tnm_add_parent_trp(&icon_trp, parent_trp);

		// スプライトを作成
		m_sprite_icon.create_polygon_no_init();

		// スプライトのレンダーパラメータ
		tnm_trp_to_rp(&icon_trp, &m_sprite_icon.rpex, &m_sprite_icon.rp);
		m_sprite_icon.rp.d2_rect = true;
		m_sprite_icon.rp.size_fit_to_texture = 0;	// ０番テクスチャにフィット
		m_sprite_icon.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_sprite_icon.set_index_buffer(G_rect_index_buffer);
		m_sprite_icon.set_texture(p_album->get_texture(pat_no));
	}
}

// ****************************************************************
// オブジェクト：スプライトツリーを構築
// ================================================================
void C_elm_mwnd_key_icon::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	root.add_child(&m_sprite_icon);
}



