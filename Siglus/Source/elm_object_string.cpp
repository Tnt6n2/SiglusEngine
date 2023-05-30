#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_def.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_color_table.h"
#include	"data/tnm_moji.h"
#include	"data/tnm_moji_manager.h"
#include	"element/elm_object.h"
#include	"engine/eng_config.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_message.h"

// ****************************************************************
// オブジェクト：文字列オブジェクトを作成
// ================================================================
bool C_elm_object::create_string(TSTR str)
{
	// 既に作られている場合は何もしない
	if (m_op.type == TNM_OBJECT_TYPE_STRING && m_op.str == str)
		return true;

	// タイプ別のパラメータを初期化する
	init_type(true);

	// パラメータを設定
	m_op.type = TNM_OBJECT_TYPE_STRING;
	m_op.str = str;

	// 文字列オブジェクトを構築する
	if (!restruct_string())
		return false;

	return true;
}

// ****************************************************************
// オブジェクト：文字列を設定
// ================================================================
bool C_elm_object::set_string(TSTR str)
{
	// 文字列オブジェクトでなかったら何もしない
	if (m_op.type != TNM_OBJECT_TYPE_STRING)
		return false;

	// 変更されていない場合は何もしない
	if (m_op.str == str)
		return true;

	// タイプ別のパラメータを解放する（自分のみ）
	free_type(false);

	// パラメータを設定
	m_op.str = str;

	// 文字列オブジェクトを構築する
	if (!restruct_string())
		return false;

	return true;
}

// ****************************************************************
// オブジェクト：文字列パラメータを設定
// ================================================================
bool C_elm_object::set_string_param(int moji_size, int moji_space_x, int moji_space_y, int moji_cnt, int moji_color, int shadow_color, int fuchi_color, int shadow_mode)
{
	// 文字列オブジェクトでなかったら何もしない
	if (m_op.type != TNM_OBJECT_TYPE_STRING)
		return true;

	// 変更されていない場合は何もしない
	if (m_op.string.moji_size == moji_size && m_op.string.moji_space_x == moji_space_x && m_op.string.moji_space_y == moji_space_y && m_op.string.moji_cnt == moji_cnt && m_op.string.moji_color == moji_color && m_op.string.shadow_color == shadow_color && m_op.string.fuchi_color == fuchi_color && m_op.string.shadow_mode == shadow_mode)
		return true;

	// タイプ別のパラメータを解放する（自分のみ）
	free_type(false);

	// パラメータを設定
	m_op.string.moji_size = moji_size;
	m_op.string.moji_space_x = moji_space_x;
	m_op.string.moji_space_y = moji_space_y;
	m_op.string.moji_cnt = moji_cnt;
	m_op.string.moji_color = moji_color;
	m_op.string.shadow_color = shadow_color;
	m_op.string.fuchi_color = fuchi_color;
	m_op.string.shadow_mode = shadow_mode;

	// 文字列オブジェクトを構築する
	if (!restruct_string())
		return false;

	return true;
}

// ****************************************************************
// オブジェクト：文字列オブジェクトを再構築
// ================================================================
bool C_elm_object::restruct_string()
{
	// 文字リストを作成
	int num = 0, cnt = 0, cur_x = 0, cur_y = 0, color_no = m_op.string.moji_color, size = m_op.string.moji_size;
	for (TSTR::const_iterator itr = m_op.str.begin(), end = m_op.str.end(); itr != m_op.str.end(); )	{
		int moji_type = TNM_MOJI_TYPE_MOJI;
		int moji_code = 0;

		// コマンド
		if (str_analize_check_moji(itr, end, _T('#')))	{
			if (str_analize_check_moji(itr, end, _T('#')))	{
				moji_code = _T('#');		// ## は # を出力する。RealLive と異なるので注意！
			}
			else if (str_analize_check_moji(itr, end, _T('D')))	{		// 改行
				cnt = 0;

				if (Gp_ini->tateyoko_mode == 1)
				{
					cur_x -= size + m_op.string.moji_space_y;
					cur_y = 0;
				}
				else
				{
					cur_x = 0;
					cur_y += size + m_op.string.moji_space_y;
				}
				continue;
			}
			else if (str_analize_get_integer(itr, end, &num))	{

				if (false);
				else if (str_analize_check_moji(itr, end, _T('C')))	{
					color_no = num;		// 色を設定
					continue;
				}
				else if (str_analize_check_moji(itr, end, _T('S')))	{
					size = num;			// サイズを設定
					continue;
				}
				else if (str_analize_check_str(itr, end, _T("RX")))	{
					cur_x += num;		// Ｘ座標を指定
					continue;
				}
				else if (str_analize_check_moji(itr, end, _T('X')))	{
					cur_x = num;		// Ｘ座標を指定
					continue;
				}
				else if (str_analize_check_str(itr, end, _T("RY")))	{
					cur_y += num;		// Ｙ座標を指定
					continue;
				}
				else if (str_analize_check_moji(itr, end, _T('Y')))	{
					cur_y = num;		// Ｙ座標を指定
					continue;
				}
			}
		}
		// 通常の文字または絵文字
		else	{
			tnm_msg_proc_expand_name_flag(itr, end, &moji_type, &moji_code);
		}

		// 入りきらなければ改行する（m_op.string.moji_cnt <= 0 のときは改行しない）
		if (m_op.string.moji_cnt > 0)	{
			int new_cnt = 2;
			if (moji_type == TNM_MOJI_TYPE_MOJI && is_hankaku((TCHAR)moji_code))	{
				new_cnt = 1;	// 内部では半角を１文字と数えてます
			}
			if (cnt + new_cnt > m_op.string.moji_cnt * 2)	{
				cnt = 0;

				if (Gp_ini->tateyoko_mode == 1)
				{
					cur_x -= size + m_op.string.moji_space_y;
					cur_y = 0;
				}
				else
				{
					cur_x = 0;
					cur_y += size + m_op.string.moji_space_y;
				}
			}
		}

		// パラメータの設定
		C_elm_object_param_moji moji_param;
		moji_param.type = moji_type;	// 文字タイプ
		moji_param.code = moji_code;	// 文字コード
		moji_param.pos.x = cur_x;		// 位置Ｘ
		moji_param.pos.y = cur_y;		// 位置Ｙ
		moji_param.color = Gp_color_table->get_color(color_no);	// 色
		moji_param.size = size;			// 大きさ

		// 現在位置を進める
		if (Gp_ini->tateyoko_mode == 1)
		{
			if (moji_type != TNM_MOJI_TYPE_MOJI)	{ cnt += 2;	cur_y += size + m_op.string.moji_space_x; }
			if (is_hankaku((TCHAR)moji_code))		{ cnt += 1;	cur_y += (size + 1) / 2 + m_op.string.moji_space_x; }
			else									{ cnt += 2;	cur_y += size + m_op.string.moji_space_x; }
		}
		else
		{
			if (moji_type != TNM_MOJI_TYPE_MOJI)	{ cnt += 2;	cur_x += size + m_op.string.moji_space_x; }
			if (is_hankaku((TCHAR)moji_code))		{ cnt += 1;	cur_x += (size + 1) / 2 + m_op.string.moji_space_x; }
			else									{ cnt += 2;	cur_x += size + m_op.string.moji_space_x; }
		}

		// 文字リストに追加
		m_moji_list.push_back(moji_param);
	}

	// スプライトの作成
	m_sprite_list.resize(m_moji_list.size() * 3);	// * 3: 影
	for (int i = 0; i < (int)m_moji_list.size() * 3; i++)	{
		if (!m_sprite_list[i].create_polygon_no_init())
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("文字列オブジェクトの作成に失敗しました。"));

		m_sprite_list[i].set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_sprite_list[i].set_index_buffer(G_rect_index_buffer);
	}

	return true;
}

// ****************************************************************
// オブジェクト：文字列オブジェクトのフレーム処理
// ================================================================
void C_elm_object::string_frame()
{
	// 文字の設定
	int moji_cnt = (int)m_moji_list.size();
	for (int i = 0; i < moji_cnt; i++)
	{
		// 基本パラメータを設定
		trp_to_rp(&m_sprite_list[i].rpex, &m_sprite_list[i].rp);									// 影
		m_sprite_list[i].rp.size_fit_to_texture = 0;
		trp_to_rp(&m_sprite_list[i + moji_cnt].rpex, &m_sprite_list[i + moji_cnt].rp);				// 縁
		m_sprite_list[i + moji_cnt].rp.size_fit_to_texture = 0;
		trp_to_rp(&m_sprite_list[i + moji_cnt * 2].rpex, &m_sprite_list[i + moji_cnt * 2].rp);		// 文字
		m_sprite_list[i + moji_cnt * 2].rp.size_fit_to_texture = 0;

		int shadow_mode = (m_op.string.shadow_mode == -1) ? Gp_global->total_font_shadow : m_op.string.shadow_mode;

		// 影テクスチャを読み込む（影／縁＋影）
		if (shadow_mode == TNM_FONT_SHADOW_MODE_SHADOW || shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
		{
			// 文字のタイプを求める（細い：袋なら９方向、影なら１方向／太い：袋なら１６方向、影なら４方向）
			MOJI_FACE_TYPE moji_face_type;
			if (Gp_global->total_font_futoku)
			{
				if (shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
				{
					moji_face_type = MOJI_FACE_TYPE_16;
				}
				else
				{
					moji_face_type = MOJI_FACE_TYPE_4;
				}
			}
			else
			{
				if (shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
				{
					moji_face_type = MOJI_FACE_TYPE_9;
				}
				else
				{
					moji_face_type = MOJI_FACE_TYPE_NONE;
				}
			}

			BSP<C_d3d_texture> texture_shadow = G_moji_manager.load_moji(m_moji_list[i].type, m_moji_list[i].code, m_moji_list[i].size, Gp_color_table->get_color(m_op.string.shadow_color), moji_face_type, Gp_ini->tateyoko_mode == 1, 1);
			if (texture_shadow)
			{
				float rep_x = 0.0;
				float rep_y = 0.0;

				if (shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
				{
					rep_x = linear_limit(m_moji_list[i].size, 0, 0.5f, 32, 1.5f) - 1.0f;	// 2.0f → 1.5f に修正
					rep_y = linear_limit(m_moji_list[i].size, 0, 0.5f, 32, 1.5f) - 1.0f;	// 2.0f → 1.5f に修正
				}
				else
				{
					rep_x = linear_limit(m_moji_list[i].size, 0, 0.5f, 32, 2.0f);
					rep_y = linear_limit(m_moji_list[i].size, 0, 0.5f, 32, 2.0f);
				}

				// 位置を求める
				//float rep_x = linear_limit(m_moji_list[i].size, 0, 0.5f, 32, 2.0f);
				//float rep_y = rep_x;
				
				// 縦書きの場合は起点を右上から左上に変換
				if (Gp_ini->tateyoko_mode == 1)
				{
					rep_x -= (float)m_moji_list[i].size;
				}

				// テクスチャを設定
				m_sprite_list[i].set_texture(texture_shadow);
				m_sprite_list[i].rp.center.x -= (float)m_moji_list[i].pos.x + rep_x;
				m_sprite_list[i].rp.center.y -= (float)m_moji_list[i].pos.y + rep_y;
			}
		}

		// 縁テクスチャを読み込む
		if (shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU || shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
		{
			// 文字のタイプを求める（細い：袋なら９方向、影なら１方向／太い：袋なら１６方向、影なら４方向）
			MOJI_FACE_TYPE moji_face_type;
			if (Gp_global->total_font_futoku)
			{
				moji_face_type = MOJI_FACE_TYPE_16;
			}
			else
			{
				moji_face_type = MOJI_FACE_TYPE_9;
			}

			BSP<C_d3d_texture> texture_shadow = G_moji_manager.load_moji(m_moji_list[i].type, m_moji_list[i].code, m_moji_list[i].size, Gp_color_table->get_color(m_op.string.fuchi_color), moji_face_type, Gp_ini->tateyoko_mode == 1, 1);
			if (texture_shadow)	{

				// 位置を求める
				float rep_x = -1.0f;
				float rep_y = rep_x;

				// 縦書きの場合は起点を右上から左上に変換
				if (Gp_ini->tateyoko_mode == 1)
				{
					rep_x -= (float)m_moji_list[i].size;
				}

				// テクスチャを設定
				m_sprite_list[i + moji_cnt].set_texture(texture_shadow);
				m_sprite_list[i + moji_cnt].rp.center.x -= (float)m_moji_list[i].pos.x + rep_x;
				m_sprite_list[i + moji_cnt].rp.center.y -= (float)m_moji_list[i].pos.y + rep_y;
			}
		}

		// 文字テクスチャを読み込む
		{
			// 文字のタイプを求める（細い：１方向／太い：４方向）
			MOJI_FACE_TYPE moji_face_type = Gp_global->total_font_futoku ? MOJI_FACE_TYPE_4 : MOJI_FACE_TYPE_NONE;

			// 文字テクスチャを読み込む
			BSP<C_d3d_texture> texture_moji = G_moji_manager.load_moji(m_moji_list[i].type, m_moji_list[i].code, m_moji_list[i].size, m_moji_list[i].color, moji_face_type, Gp_ini->tateyoko_mode == 1, 0);
			if (texture_moji)	{

				// 位置を求める
				float rep_x = 0.0f;
				float rep_y = 0.0f;

				// 縦書きの場合は起点を右上から左上に変換
				if (Gp_ini->tateyoko_mode == 1)
				{
					rep_x -= (float)m_moji_list[i].size;
				}

				// テクスチャを設定
				m_sprite_list[i + moji_cnt * 2].set_texture(texture_moji);
				m_sprite_list[i + moji_cnt * 2].rp.center.x -= m_moji_list[i].pos.x + rep_x;
				m_sprite_list[i + moji_cnt * 2].rp.center.y -= m_moji_list[i].pos.y + rep_y;
			}
		}
	}
}
