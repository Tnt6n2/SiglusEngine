#include	"pch.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_tool.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_read_flag.h"
#include	"element/elm_mwnd.h"
#include	"element/elm_mwnd_moji.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_config.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_etc.h"


// ****************************************************************
// メッセージウィンドウ：初期化
// ================================================================
void C_elm_mwnd::init(int mwnd_no, S_element element, CTSTR& name, C_elm_mwnd_list* p_parent, S_element elm_group_list)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_MWND, name, p_parent);

	// 固定パラメータ
	m_mwnd_no = mwnd_no;
	m_def_elm_group_list = elm_group_list;

	// メッセージ枠
	m_msg_waku.init(element, name);

	// 名前枠
	m_name_waku.init(element, name);

	// 再初期化
	reinit(true);
}

// ****************************************************************
// メッセージウィンドウ：再初期化
// ================================================================
void C_elm_mwnd::reinit(bool restruct_flag)
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	// 終了は必ずしてみよう
	finish();

	// テンプレートを再構築
	restruct_template();

	// 処理変数初期化
	init_work_variable();

	// メッセージ
	m_msg_list.resize(1);	// メッセージは１つ
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].init(m_def_elm_group_list, this);

	// メッセージにデフォルトのパラメータを与える
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].set_def_param(p_ini_sub->moji_cnt
		, p_ini_sub->moji_size, p_ini_sub->moji_space
		, p_ini_sub->moji_color >= 0 ? p_ini_sub->moji_color : p_ini->moji_color
		, p_ini_sub->shadow_color >= 0 ? p_ini_sub->shadow_color : p_ini->shadow_color
		, p_ini_sub->fuchi_color >= 0 ? p_ini_sub->fuchi_color : p_ini->fuchi_color
		, p_ini_sub->ruby_size, p_ini_sub->ruby_space
		, p_ini_sub->name_moji_size, p_ini_sub->name_newline, p_ini_sub->name_bracket, p_ini_sub->talk_margin);

	// 名前
	m_name.init();

	// 選択肢
	m_sel.init();

	// メッセージ枠
	m_msg_waku.reinit(false);	// 再構築しない

	// 名前枠
	m_name_waku.reinit(false);	// 再構築しない

	// 再構築
	if (restruct_flag)	{

		// 名前にデフォルトのパラメータを与える
		m_name.set_template(m_mwnd_no);

		// 選択肢にデフォルトのパラメータを与える
		m_sel.set_template(m_mwnd_no);

		// 枠を整える
		set_waku(p_ini_sub->waku_no, p_ini_sub->name_waku_no, false);
	}
}

// ****************************************************************
// メッセージウィンドウ：終了
// ================================================================
void C_elm_mwnd::finish()
{
	// 既読フラグを立てる！
	set_read_flag();

	// メッセージ枠
	m_msg_waku.finish();

	// 名前枠
	m_name_waku.finish();
}

// ****************************************************************
// メッセージウィンドウ：処理変数を初期化
// ================================================================
void C_elm_mwnd::init_work_variable()
{
	m_time = 0;
	m_msg_block_started_flag = false;
	m_auto_proc_ready_flag = false;
	m_window_appear = false;
	m_name_appear = false;
	m_clear_ready_flag = false;
	m_auto_mode_end_moji_cnt = 0;
	m_target_msg_no = 0;
	m_slide_msg = false;
	m_slide_time = 0;
	m_read_flag_stock_list.clear();
	m_koe_no = -1;
	m_koe_play_flag = false;
	m_koe_no_auto_mode_flag = false;
	m_open_anime_type = -1;
	m_open_anime_time = 0;
	m_open_anime_start_time = 0;
	m_close_anime_type = -1;
	m_close_anime_time = 0;
	m_close_anime_start_time = 0;
	m_slide_anime_type = -1;
	m_slide_anime_time = 0;
	m_slide_anime_start_time = 0;
	m_slide_pixel = 0;
}

// ****************************************************************
// メッセージウィンドウ：テンプレートを再構築する
// ================================================================
void C_elm_mwnd::restruct_template()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	// Gameexe.ini からデフォルトのパラメータを取得する
	m_param.novel_mode = p_ini_sub->novel_mode;
	m_param.sorter = S_tnm_sorter(p_ini->order, 0);
	m_param.world = -1;
	m_param.extend_type = p_ini_sub->extend_type;
	m_param.window_pos = p_ini_sub->window_pos;
	m_param.window_size = p_ini_sub->window_size;
	m_param.msg_pos = p_ini_sub->msg_pos;
	m_param.msg_margin = p_ini_sub->msg_margin;
	m_param.msg_moji_cnt = p_ini_sub->moji_cnt;
	m_param.name_disp_mode = p_ini_sub->name_disp_mode;
	m_param.name_bracket = p_ini_sub->name_bracket;
	m_param.name_extend_type = p_ini_sub->name_extend_type;
	m_param.name_window_align = p_ini_sub->name_window_align;
	m_param.name_window_pos = p_ini_sub->name_window_pos;
	m_param.name_window_size = p_ini_sub->name_window_size;
	m_param.name_msg_pos = p_ini_sub->name_msg_pos;
	m_param.name_msg_pos_rep = p_ini_sub->name_msg_pos_rep;
	m_param.name_msg_margin = p_ini_sub->name_msg_margin;
	m_param.over_flow_check_size = p_ini_sub->over_flow_check_size;
	m_param.face_hide_name = p_ini_sub->face_hide_name;
	m_param.open_anime_type = p_ini_sub->open_anime_type;
	m_param.open_anime_time = p_ini_sub->open_anime_time;
	m_param.close_anime_type = p_ini_sub->close_anime_type;
	m_param.close_anime_time = p_ini_sub->close_anime_time;
}

// ****************************************************************
// メッセージウィンドウ：セーブ
// ================================================================
void C_elm_mwnd::save(C_tnm_save_stream& stream)
{
	// 現在のパラメータ
	stream.save(m_param);

	// 処理変数
	stream.save(m_time);
	stream.save(m_msg_block_started_flag);
	stream.save(m_auto_proc_ready_flag);
	stream.save(m_window_appear);
	stream.save(m_name_appear);
	stream.save(m_clear_ready_flag);
	stream.save(m_auto_mode_end_moji_cnt);
	stream.save(m_target_msg_no);
	stream.save(m_slide_msg);
	stream.save(m_slide_time);
	stream.save(m_koe_no);
	stream.save(m_koe_play_flag);
	stream.save(m_open_anime_type);
	stream.save(m_open_anime_time);
	stream.save(m_open_anime_start_time);
	stream.save(m_close_anime_type);
	stream.save(m_close_anime_time);
	stream.save(m_close_anime_start_time);

	// メッセージ
	stream.save(m_msg_list);

	// メッセージ枠
	m_msg_waku.save(stream);

	// 名前
	m_name.save(stream);

	// 名前枠
	m_name_waku.save(stream);

	// 選択肢
	m_sel.save(stream);
}

// ****************************************************************
// メッセージウィンドウ：ロード
// ================================================================
void C_elm_mwnd::load(C_tnm_save_stream& stream)
{
	// 現在のパラメータ
	stream.load(m_param);

	// 処理変数
	stream.load(m_time);
	stream.load(m_msg_block_started_flag);
	stream.load(m_auto_proc_ready_flag);
	stream.load(m_window_appear);
	stream.load(m_name_appear);
	stream.load(m_clear_ready_flag);
	stream.load(m_auto_mode_end_moji_cnt);
	stream.load(m_target_msg_no);
	stream.load(m_slide_msg);
	stream.load(m_slide_time);
	stream.load(m_koe_no);
	stream.load(m_koe_play_flag);
	stream.load(m_open_anime_type);
	stream.load(m_open_anime_time);
	stream.load(m_open_anime_start_time);
	stream.load(m_close_anime_type);
	stream.load(m_close_anime_time);
	stream.load(m_close_anime_start_time);

	// メッセージ
	stream.load(m_msg_list);

	// メッセージ枠
	m_msg_waku.load(stream);

	// 名前
	m_name.load(stream);

	// 名前枠
	m_name_waku.load(stream);

	// 選択肢
	m_sel.load(stream);

	// ★大きさの再設定

	// メッセージ枠の再構築
	restruct_msg_waku();

	// 名前枠の再構築
	restruct_name_waku();

	// 選択肢の再構築
	restruct_sel();
}

// ****************************************************************
// メッセージウィンドウ：コピー
// ================================================================
void C_elm_mwnd::copy(C_elm_mwnd* src)
{
	// 現在のパラメータ
	m_param = src->m_param;

	// 処理変数
	m_time = src->m_time;
	m_msg_block_started_flag = src->m_msg_block_started_flag;
	m_auto_proc_ready_flag = src->m_auto_proc_ready_flag;
	m_window_appear = src->m_window_appear;
	m_name_appear = src->m_name_appear;
	m_clear_ready_flag = src->m_clear_ready_flag;
	m_auto_mode_end_moji_cnt = src->m_auto_mode_end_moji_cnt;
	m_slide_msg = src->m_slide_msg;
	m_slide_time = src->m_slide_time;
	m_koe_no = src->m_koe_no;
	m_koe_play_flag = src->m_koe_play_flag;
	m_koe_no_auto_mode_flag = src->m_koe_no_auto_mode_flag;
	m_open_anime_type = src->m_open_anime_type;
	m_open_anime_time = src->m_open_anime_time;
	m_open_anime_start_time = src->m_open_anime_start_time;
	m_close_anime_type = src->m_close_anime_type;
	m_close_anime_time = src->m_close_anime_time;
	m_close_anime_start_time = src->m_close_anime_start_time;

	// メッセージ
	m_msg_list.resize(src->m_msg_list.size());
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].copy(&src->m_msg_list[i]);

	// メッセージ枠
	m_msg_waku.copy_all(&src->m_msg_waku, true);

	// 名前
	m_name.copy(&src->m_name);

	// 名前枠
	m_name_waku.copy_all(&src->m_name_waku, true);

	// 選択肢
	m_sel.copy(&src->m_sel);

	// ★大きさの再設定

	// メッセージ枠の再構築
	restruct_msg_waku();

	// 名前枠の再構築
	restruct_name_waku();

	// 選択肢の再構築
	restruct_sel();
}

// ****************************************************************
// メッセージウィンドウ：枠を設定
// ================================================================
void C_elm_mwnd::set_waku(int msg_waku_no, int name_waku_no, bool reinit_flag)
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	// デフォルト値を設定
	if (msg_waku_no == TNM_USE_DEFAULT)
		msg_waku_no = p_ini_sub->waku_no;
	if (name_waku_no == TNM_USE_DEFAULT)
		name_waku_no = p_ini_sub->name_waku_no;

	// メッセージ枠
	m_msg_waku.set_template(msg_waku_no, reinit_flag);

	// メッセージ枠の再構築
	restruct_msg_waku();

	// 名前枠
	m_name_waku.set_template(name_waku_no, reinit_flag);

	// 名前枠の再構築
	restruct_name_waku();
}

// ****************************************************************
// メッセージウィンドウ：フレーム初期化処理
// ================================================================
void C_elm_mwnd::frame_init()
{
	// メッセージ
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].frame_init();

	// メッセージ枠
	m_msg_waku.frame_init();

	// 名前
	m_name.frame_init();

	// 名前枠
	m_name_waku.frame_init();

	// 選択肢
	m_sel.frame_init();
}

// ****************************************************************
// メッセージウィンドウ：時間を進める
// ================================================================
void C_elm_mwnd::update_time(int past_game_time, int past_real_time)
{
	// 時刻を進める
	m_time += past_game_time;

	// メッセージ枠
	m_msg_waku.update_time(past_game_time, past_real_time);

	// 名前枠
	m_name_waku.update_time(past_game_time, past_real_time);
}

// ****************************************************************
// メッセージウィンドウ：フレーム処理
// ================================================================
bool C_elm_mwnd::frame(bool draw_futoku, bool draw_shadow)
{
	// メッセージを進める
	msg_frame();

	// ウィンドウのアニメ状態を初期化
	bool anime_open = true;
	int anime_type = 0;
	int anime_prog = 255;	// アニメ進行度

	// 開くアニメ：アニメ進行度は 0 → 255 に変化します
	if (m_open_anime_type >= 0)	{
		anime_open = true;
		anime_type = m_open_anime_type;
		anime_prog = C_tnm_tool::time_table<int, int>(m_time, m_open_anime_start_time, 0, m_open_anime_start_time + m_open_anime_time, 255);
		if (anime_prog == 255)	{
			end_open_anime();		// ウィンドウ開きアニメ終了
		}
	}

	// 閉じるアニメ：アニメ進行度は 255 → 0 に変化します
	if (m_close_anime_type >= 0)	{
		anime_open = false;
		anime_type = m_close_anime_type;
		anime_prog = C_tnm_tool::time_table<int, int>(m_time, m_close_anime_start_time, 255, m_close_anime_start_time + m_close_anime_time, 0);
		if (anime_prog == 0)	{
			end_close_anime();		// ウィンドウ閉じアニメ終了
		}
	}

	// スライドアニメ
	int slide_pixel = 0;
	if (m_slide_anime_type >= 0)	{
		slide_pixel = C_tnm_tool::time_table<int, int>(m_time, m_slide_anime_start_time, m_slide_pixel, m_slide_anime_start_time + m_slide_anime_time, 0);
		if (m_time >= m_slide_anime_start_time + m_slide_anime_time)	{
			slide_pixel = 0;
			m_slide_anime_type = -1; 
		}
	}

	// ウィンドウが開いているか、ウィンドウが閉じるアニメ中なら表示されるべき
	if (m_window_appear || m_close_anime_type >= 0)
	{
		// ウィンドウ矩形
		C_rect window_rect = C_rect::by_size(m_param.window_pos, m_param.window_size);

		// ウィンドウアニメによる修正値
		C_point	pos_rep = C_point(0, 0);
		C_point	abs_center = C_point(0, 0);
		C_size	abs_scale = C_size(TNM_SCALE_UNIT, TNM_SCALE_UNIT);
		int		abs_rotate = 0;
		BYTE	tr_rep = 255;

		if (false);
		// ウィンドウアニメ：タイプ０：アニメなし
		else if (anime_type == 0)	{
			// NOP
		}
		// ウィンドウアニメ：その他のタイプ
		else	{
			// ウィンドウと名前を囲う矩形（ウィンドウアニメ用）
			C_rect win_name_rect = window_rect;
			if (m_name_appear && !m_name.is_empty())	{		// 名前が表示されている場合
				C_rect name_rect = m_param.name_window_rect + m_param.name_window_pos + window_rect.left_top();
				if (win_name_rect.top > name_rect.top)			{	win_name_rect.top = name_rect.top;			}
				if (win_name_rect.bottom < name_rect.bottom)	{	win_name_rect.bottom = name_rect.bottom;	}
				if (win_name_rect.left > name_rect.left)		{	win_name_rect.left = name_rect.left;		}
				if (win_name_rect.right < name_rect.right)		{	win_name_rect.right = name_rect.right;		}
			}
			window_anime(anime_open, anime_type, anime_prog, &window_rect, &win_name_rect, &pos_rep, &abs_center, &abs_scale, &abs_rotate, &tr_rep);
		}

		// 固定ウィンドウの場合
		if (m_param.extend_type == 0)	{

			// 子に引き継ぐレンダーパラメータ
			S_tnm_render_param mwnd_tdp;
			mwnd_tdp.disp = true;
			mwnd_tdp.sorter = m_param.sorter;
			mwnd_tdp.center_x = (float)abs_center.x;
			mwnd_tdp.center_y = (float)abs_center.y;
			mwnd_tdp.scale_x = (float)abs_scale.cx / TNM_SCALE_UNIT;
			mwnd_tdp.scale_y = (float)abs_scale.cy / TNM_SCALE_UNIT;
			mwnd_tdp.rotate_z = D3DXToRadian((float)abs_rotate / TNM_ANGLE_UNIT);
			mwnd_tdp.tr = tr_rep;

			// メッセージ枠のフレーム処理
			m_msg_waku.frame(m_time, window_rect + pos_rep, &mwnd_tdp, m_name_appear);

			// メッセージの位置を計算
			C_point msg_pos;
			if (Gp_ini->tateyoko_mode == 1)
			{
				msg_pos.x = window_rect.right - m_param.msg_pos.x - slide_pixel;
				msg_pos.y = window_rect.top + m_param.msg_pos.y;
			}
			else
			{
				msg_pos.x = window_rect.left + m_param.msg_pos.x;
				msg_pos.y = window_rect.top + m_param.msg_pos.y + slide_pixel;
			}

			// スクロールメッセージに備えてクリップ領域を作成する
			// 上端だけ設定。左右下は画面いっぱいまで取る
			if (m_slide_msg)
			{
				C_rect dst_clip(0, window_rect.top + m_param.msg_pos.y - Gp_ini->mwnd.sub[m_mwnd_no].moji_space.cy + pos_rep.y, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy);
				mwnd_tdp.dst_clip_use = true;
				mwnd_tdp.dst_clip_left = dst_clip.left;
				mwnd_tdp.dst_clip_top = dst_clip.top;
				mwnd_tdp.dst_clip_right = dst_clip.right;
				mwnd_tdp.dst_clip_bottom = dst_clip.bottom;
			}

			// メッセージのフレーム処理
			for (int i = 0; i < (int)m_msg_list.size(); i++)	{
				if (!m_msg_list[i].frame(&mwnd_tdp, msg_pos + pos_rep, 0, C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), draw_futoku, draw_shadow))
					return false;
			}

			// 選択肢のフレーム処理
			m_sel.frame(&mwnd_tdp, msg_pos + pos_rep, draw_futoku, draw_shadow);
		}

		// 可変ウィンドウの場合
		else	{

			// 子に引き継ぐレンダーパラメータ
			S_tnm_render_param mwnd_tdp;
			mwnd_tdp.disp = true;
			mwnd_tdp.sorter = m_param.sorter;
			mwnd_tdp.center_x = (float)abs_center.x;
			mwnd_tdp.center_y = (float)abs_center.y;
			mwnd_tdp.scale_x = (float)abs_scale.cx / TNM_SCALE_UNIT;
			mwnd_tdp.scale_y = (float)abs_scale.cy / TNM_SCALE_UNIT;
			mwnd_tdp.rotate_z = D3DXToRadian((float)abs_rotate / TNM_ANGLE_UNIT);
			mwnd_tdp.tr = tr_rep;

			// メッセージ枠のフレーム処理
			m_msg_waku.frame(m_time, window_rect + pos_rep, &mwnd_tdp, m_name_appear);

			// メッセージ矩形を計算
			C_rect msg_rect = tnm_add_margin(window_rect, m_param.msg_margin);

			// スクロールメッセージに備えてクリップ領域を作成する
			// 上端だけ設定。左右下は画面いっぱいまで取る
			if (m_slide_msg)
			{
				C_rect dst_clip(0, window_rect.top + m_param.msg_pos.y - Gp_ini->mwnd.sub[m_mwnd_no].moji_space.cy + pos_rep.y, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy);
				mwnd_tdp.dst_clip_left = dst_clip.left;
				mwnd_tdp.dst_clip_top = dst_clip.top;
				mwnd_tdp.dst_clip_right = dst_clip.right;
				mwnd_tdp.dst_clip_bottom = dst_clip.bottom;
				mwnd_tdp.dst_clip_use = true;
			}

			// メッセージの位置を計算
			C_point msg_pos(msg_rect.left + pos_rep.x, msg_rect.top + pos_rep.y);

			// メッセージのフレーム処理
			for (int i = 0; i < (int)m_msg_list.size(); i++)	{
				if (!m_msg_list[i].frame(&mwnd_tdp, msg_pos, 0, C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), draw_futoku, draw_shadow))
					return false;
			}

			// 選択肢のフレーム処理
			m_sel.frame(&mwnd_tdp, msg_pos, draw_futoku, draw_shadow);
		}

		// 名前ウィンドウが表示されている場合
		if (m_param.name_disp_mode == TNM_MWND_NAME_DISP_TYPE_NAME && m_name_appear && !m_name.is_empty())	{

			// 「顔グラを表示したとき名前を隠す」
			if (m_param.face_hide_name == 1 && m_msg_waku.is_exist_face())	{
				// NOP
			}
			else	{

				// 名前ウィンドウ位置
				C_point name_window_pos = m_param.name_window_pos + window_rect.left_top();

				// 固定ウィンドウの場合
				if (m_param.name_extend_type == 0)	{

					// 子に引き継ぐレンダーパラメータ
					S_tnm_render_param mwnd_tdp;
					mwnd_tdp.disp = true;
					mwnd_tdp.sorter = m_param.sorter;
					mwnd_tdp.center_x = (float)abs_center.x;
					mwnd_tdp.center_y = (float)abs_center.y;
					mwnd_tdp.scale_x = (float)abs_scale.cx / TNM_SCALE_UNIT;
					mwnd_tdp.scale_y = (float)abs_scale.cy / TNM_SCALE_UNIT;
					mwnd_tdp.rotate_z = D3DXToRadian((float)abs_rotate / TNM_ANGLE_UNIT);
					mwnd_tdp.tr = tr_rep;

					// メッセージ枠のフレーム処理
					C_rect name_window_rect(m_param.name_window_rect + name_window_pos + pos_rep);
					m_name_waku.frame(m_time, name_window_rect, &mwnd_tdp, m_name_appear);

					// メッセージの位置を計算
					C_point name_msg_pos;

					if (false);
					else if (m_param.name_window_align == 0)	{	// 左寄せ
						name_msg_pos.x = name_window_pos.x + m_param.name_msg_pos.x + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_pos.y + m_param.name_msg_pos_rep.y;
					}
					else if (m_param.name_window_align == 1)	{	// 中央寄せ
						name_msg_pos.x = name_window_pos.x                          + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_pos.y + m_param.name_msg_pos_rep.y;
					}
					else if (m_param.name_window_align == 2)	{	// 右寄せ
						name_msg_pos.x = name_window_pos.x - m_param.name_msg_pos.x + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_pos.y + m_param.name_msg_pos_rep.y;
					}

					// メッセージのフレーム処理
					m_name.frame(&mwnd_tdp, name_msg_pos + pos_rep, draw_futoku, draw_shadow);
				}

				// 可変ウィンドウの場合
				else	{

					// 子に引き継ぐレンダーパラメータ
					S_tnm_render_param mwnd_tdp;
					mwnd_tdp.disp = true;
					mwnd_tdp.sorter = m_param.sorter;
					mwnd_tdp.center_x = (float)abs_center.x;
					mwnd_tdp.center_y = (float)abs_center.y;
					mwnd_tdp.scale_x = (float)abs_scale.cx / TNM_SCALE_UNIT;
					mwnd_tdp.scale_y = (float)abs_scale.cy / TNM_SCALE_UNIT;
					mwnd_tdp.rotate_z = (float)abs_rotate / TNM_ANGLE_UNIT;
					mwnd_tdp.tr = tr_rep;

					// メッセージ枠のフレーム処理
					m_name_waku.frame(m_time, m_param.name_window_rect + name_window_pos + pos_rep, &mwnd_tdp, m_name_appear);

					// メッセージの位置を計算
					C_point name_msg_pos;

					if (false);
					else if (m_param.name_window_align == 0)	{	// 左寄せ
						name_msg_pos.x = name_window_pos.x + m_param.name_msg_margin.left  + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_margin.top   + m_param.name_msg_pos_rep.y;
					}
					else if (m_param.name_window_align == 1)	{	// 中央寄せ
						name_msg_pos.x = name_window_pos.x                                 + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_margin.top   + m_param.name_msg_pos_rep.y;
					}
					else if (m_param.name_window_align == 2)	{	// 右寄せ
						name_msg_pos.x = name_window_pos.x - m_param.name_msg_margin.right + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_margin.top   + m_param.name_msg_pos_rep.y;
					}

					// メッセージのフレーム処理
					m_name.frame(&mwnd_tdp, name_msg_pos + pos_rep, draw_futoku, draw_shadow);
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// メッセージウィンドウ：ウィンドウアニメ処理
// ================================================================
void C_elm_mwnd::window_anime(bool anime_open, int anime_type, int anime_prog, C_rect *window_rect, C_rect *win_name_rect, C_point *ret_pos_rep, C_point *ret_abs_center, C_size *ret_abs_scale, int *ret_abs_rotate, BYTE *ret_tr_rep)
{
	// ウィンドウアニメ：★実験用
	// anime_type = 39;

	C_point pos_rep = C_point(0, 0);
	C_point abs_center = C_point(window_rect->left + window_rect->width() / 2, window_rect->top + window_rect->height() / 2);
	C_size abs_scale = C_size(TNM_SCALE_UNIT, TNM_SCALE_UNIT);
	int abs_rotate = 0;
	BYTE tr_rep = 255;

	// --------------------------------------------------------------------------------------
	// タイプ補正

	if (false);
	// ウィンドウアニメ：タイプ６：上下近い方から
	else if (anime_type == 6)	{
		// 名前の有る無しで方向性に影響が出ないよう、ウィンドウ矩形で計算します
		int type_2_length = window_rect->bottom;									// 上から
		int type_3_length = Gp_global->game_screen_size.cy - window_rect->top;		// 下から
		anime_type = (type_2_length <= type_3_length) ? 2 : 3;
	}
	// ウィンドウアニメ：タイプ７：左右近い方から
	else if (anime_type == 7)	{
		// 名前の有る無しで方向性に影響が出ないよう、ウィンドウ矩形で計算します
		int type_4_length = window_rect->right;										// 左から
		int type_5_length = Gp_global->game_screen_size.cx - window_rect->left;		// 右から
		anime_type = (type_4_length <= type_5_length) ? 4 : 5;
	}
	// ウィンドウアニメ：タイプ８：上下左右近い方から
	else if (anime_type == 8)	{
		// 名前の有る無しで方向性に影響が出ないよう、ウィンドウ矩形で計算します
		int type_2_length = window_rect->bottom;									// 上から
		int type_3_length = Gp_global->game_screen_size.cy - window_rect->top;		// 下から
		int type_4_length = window_rect->right;										// 左から
		int type_5_length = Gp_global->game_screen_size.cx - window_rect->left;		// 右から
		int type_ud, type_ud_length, type_lr, type_lr_length;
		if (type_2_length <= type_3_length)	{	type_ud = 2;	type_ud_length = type_2_length;	}
		else								{	type_ud = 3;	type_ud_length = type_3_length;	}
		if (type_4_length <= type_5_length)	{	type_lr = 4;	type_lr_length = type_4_length;	}
		else								{	type_lr = 5;	type_lr_length = type_5_length;	}
		anime_type = (type_ud_length <= type_lr_length) ? type_ud : type_lr;
	}

	// --------------------------------------------------------------------------------------
	// 拡大縮小系アニメのパラメータ

	bool	type_09_48_flag = false;
	int		ud_scale_mod = 0;		// 0=なし 1=拡大 2=縮小
	int		ud_scale_center = 0;	// 0=中心 1=上端 2=下端 3=画面上端 4=画面下端
	int		lr_scale_mod = 0;		// 0=なし 1=拡大 2=縮小
	int		lr_scale_center = 0;	// 0=中心 1=左端 2=右端 3=画面左端 4=画面右端

	if (false);
	// ウィンドウアニメ：タイプ０９：中心に対して左右拡大（上下拡大）
	// ウィンドウアニメ：タイプ１０：中心に対して左右拡大（上下縮小）
	// ウィンドウアニメ：タイプ１１：中心に対して左右拡大（上下そのまま）
	// ウィンドウアニメ：タイプ１２：中心に対して左右縮小（上下拡大）
	// ウィンドウアニメ：タイプ１３：中心に対して左右縮小（上下縮小）
	// ウィンドウアニメ：タイプ１４：中心に対して左右縮小（上下そのまま）
	// ウィンドウアニメ：タイプ１５：中心に対して左右そのまま（上下拡大）
	// ウィンドウアニメ：タイプ１６：中心に対して左右そのまま（上下縮小）
	else if (anime_type == 9)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	}
	else if (anime_type == 10)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	}
	else if (anime_type == 11)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	}
	else if (anime_type == 12)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	}
	else if (anime_type == 13)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	}
	else if (anime_type == 14)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	}
	else if (anime_type == 15)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	}
	else if (anime_type == 16)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	}
	// ウィンドウアニメ：タイプ１７：左端に対して左右縮小
	// ウィンドウアニメ：タイプ１８：右端に対して左右縮小
	// ウィンドウアニメ：タイプ１９：上端に対して左右縮小
	// ウィンドウアニメ：タイプ２０：下端に対して左右縮小
	else if (anime_type == 17)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 1;	}
	else if (anime_type == 18)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 2;	}
	else if (anime_type == 19)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 1;	lr_scale_mod = 0;	lr_scale_center = 0;	}
	else if (anime_type == 20)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 2;	lr_scale_mod = 0;	lr_scale_center = 0;	}
	// ウィンドウアニメ：タイプ２１：左上角に対して左右縮小
	// ウィンドウアニメ：タイプ２２：右上角に対して左右縮小
	// ウィンドウアニメ：タイプ２３：左下角に対して左右縮小
	// ウィンドウアニメ：タイプ２４：右下角に対して左右縮小
	else if (anime_type == 21)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 1;	lr_scale_mod = 2;	lr_scale_center = 1;	}
	else if (anime_type == 22)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 1;	lr_scale_mod = 2;	lr_scale_center = 2;	}
	else if (anime_type == 23)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 2;	lr_scale_mod = 2;	lr_scale_center = 1;	}
	else if (anime_type == 24)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 2;	lr_scale_mod = 2;	lr_scale_center = 2;	}
	// ウィンドウアニメ：タイプ２５：画面左端に対して左右縮小
	// ウィンドウアニメ：タイプ２６：画面右端に対して左右縮小
	// ウィンドウアニメ：タイプ２７：画面上端に対して左右縮小
	// ウィンドウアニメ：タイプ２８：画面下端に対して左右縮小
	else if (anime_type == 25)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 3;	}
	else if (anime_type == 26)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 4;	}
	else if (anime_type == 27)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 3;	lr_scale_mod = 0;	lr_scale_center = 0;	}
	else if (anime_type == 28)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 4;	lr_scale_mod = 0;	lr_scale_center = 0;	}

	// --------------------------------------------------------------------------------------
	// 回転系アニメのパラメータ

	int		rotate_cnt = 0;		// 4 で１回転

	if (false);
	// ウィンドウアニメ：タイプ２９：中心に対して縮小しながら右に１回転
	// ウィンドウアニメ：タイプ３０：中心に対して縮小しながら左に１回転
	else if (anime_type == 29)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = -4;	}
	else if (anime_type == 30)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = 4;		}
	// ウィンドウアニメ：タイプ３１：中心に対して縮小しながら右に２回転
	// ウィンドウアニメ：タイプ３２：中心に対して縮小しながら左に２回転
	else if (anime_type == 31)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = -8;	}
	else if (anime_type == 32)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = 8;		}

	// ウィンドウアニメ：タイプ３３：中心に対して拡大しながら右に１回転
	// ウィンドウアニメ：タイプ３４：中心に対して拡大しながら左に１回転
	else if (anime_type == 33)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	rotate_cnt = -4;	}
	else if (anime_type == 34)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	rotate_cnt = 4;		}
	// ウィンドウアニメ：タイプ３５：中心に対して拡大しながら右に２回転
	// ウィンドウアニメ：タイプ３６：中心に対して拡大しながら左に２回転
	else if (anime_type == 35)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	rotate_cnt = -8;	}
	else if (anime_type == 36)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	rotate_cnt = 8;		}

	// ウィンドウアニメ：タイプ３７：中心に対して上下縮小しながら右に１回転
	// ウィンドウアニメ：タイプ３８：中心に対して上下縮小しながら左に１回転
	else if (anime_type == 37)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = -4;	}
	else if (anime_type == 38)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = 4;		}
	// ウィンドウアニメ：タイプ３９：中心に対して左右縮小しながら右に１回転
	// ウィンドウアニメ：タイプ４０：中心に対して左右縮小しながら左に１回転
	else if (anime_type == 39)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = -4;	}
	else if (anime_type == 40)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = 4;		}

	// ウィンドウアニメ：タイプ４１：中心に対して上下縮小しながら右に半回転
	// ウィンドウアニメ：タイプ４２：中心に対して上下縮小しながら左に半回転
	else if (anime_type == 41)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = -2;	}
	else if (anime_type == 42)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = 2;		}
	// ウィンドウアニメ：タイプ４３：中心に対して左右縮小しながら右に半回転
	// ウィンドウアニメ：タイプ４４：中心に対して左右縮小しながら左に半回転
	else if (anime_type == 43)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = -2;	}
	else if (anime_type == 44)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = 2;		}

	// ウィンドウアニメ：タイプ４５：中心に対して上下縮小しながら右に四半回転
	// ウィンドウアニメ：タイプ４６：中心に対して上下縮小しながら左に四半回転
	else if (anime_type == 45)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = -1;	}
	else if (anime_type == 46)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = 1;		}
	// ウィンドウアニメ：タイプ４７：中心に対して左右縮小しながら右に四半回転
	// ウィンドウアニメ：タイプ４８：中心に対して左右縮小しながら左に四半回転
	else if (anime_type == 47)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = -1;	}
	else if (anime_type == 48)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = 1;		}

	// --------------------------------------------------------------------------------------
	// 個別のアニメ処理

	bool	tr_anime_flag = false;

	if (false);
	// ウィンドウアニメ：タイプ００：アニメなし
	else if (anime_type == 0)	{
		// NOP
	}
	// ウィンドウアニメ：タイプ０１：フェード
	else if (anime_type == 1)	{
		tr_rep = anime_prog;
	}
	// ウィンドウアニメ：タイプ０２：上から
	else if (anime_type == 2)	{
		tr_anime_flag = true;
		int length = -win_name_rect->bottom;
		if (anime_open)	{	pos_rep.y = speed_down_limit(anime_prog, 0, length, 255, 0);	}
		else			{	pos_rep.y = speed_up_limit(anime_prog, 0, length, 255, 0);		}
	}
	// ウィンドウアニメ：タイプ０３：下から
	else if (anime_type == 3)	{
		tr_anime_flag = true;
		int length = Gp_global->game_screen_size.cy - win_name_rect->top;
		if (anime_open)	{	pos_rep.y = speed_down_limit(anime_prog, 0, length, 255, 0);	}
		else			{	pos_rep.y = speed_up_limit(anime_prog, 0, length, 255, 0);		}
	}
	// ウィンドウアニメ：タイプ０４：左から
	else if (anime_type == 4)	{
		tr_anime_flag = true;
		int length = -win_name_rect->right;
		if (anime_open)	{	pos_rep.x = speed_down_limit(anime_prog, 0, length, 255, 0);	}
		else			{	pos_rep.x = speed_up_limit(anime_prog, 0, length, 255, 0);		}
	}
	// ウィンドウアニメ：タイプ０５：右から
	else if (anime_type == 5)	{
		tr_anime_flag = true;
		int length = Gp_global->game_screen_size.cx - win_name_rect->left;
		if (anime_open)	{	pos_rep.x = speed_down_limit(anime_prog, 0, length, 255, 0);	}
		else			{	pos_rep.x = speed_up_limit(anime_prog, 0, length, 255, 0);		}
	}

	// ウィンドウアニメ：タイプ０６：上下近い方から
	// ウィンドウアニメ：タイプ０７：左右近い方から
	// ウィンドウアニメ：タイプ０８：上下左右近い方から

	// ウィンドウアニメ：タイプ０９：中心に対して左右拡大（上下拡大）
	// ウィンドウアニメ：タイプ１０：中心に対して左右拡大（上下縮小）
	// ウィンドウアニメ：タイプ１１：中心に対して左右拡大（上下そのまま）
	// ウィンドウアニメ：タイプ１２：中心に対して左右縮小（上下拡大）
	// ウィンドウアニメ：タイプ１３：中心に対して左右縮小（上下縮小）
	// ウィンドウアニメ：タイプ１４：中心に対して左右縮小（上下そのまま）
	// ウィンドウアニメ：タイプ１５：中心に対して左右そのまま（上下拡大）
	// ウィンドウアニメ：タイプ１６：中心に対して左右そのまま（上下縮小）
	// ウィンドウアニメ：タイプ１７：左端に対して左右縮小
	// ウィンドウアニメ：タイプ１８：右端に対して左右縮小
	// ウィンドウアニメ：タイプ１９：上端に対して左右縮小
	// ウィンドウアニメ：タイプ２０：下端に対して左右縮小
	// ウィンドウアニメ：タイプ２１：左上角に対して左右縮小
	// ウィンドウアニメ：タイプ２２：右上角に対して左右縮小
	// ウィンドウアニメ：タイプ２３：左下角に対して左右縮小
	// ウィンドウアニメ：タイプ２４：右下角に対して左右縮小
	// ウィンドウアニメ：タイプ２５：画面左端に対して左右縮小
	// ウィンドウアニメ：タイプ２６：画面右端に対して左右縮小
	// ウィンドウアニメ：タイプ２７：画面上端に対して左右縮小
	// ウィンドウアニメ：タイプ２８：画面下端に対して左右縮小
	// ウィンドウアニメ：タイプ２９：中心に対して縮小しながら右に１回転
	// ウィンドウアニメ：タイプ３０：中心に対して縮小しながら左に１回転
	// ウィンドウアニメ：タイプ３１：中心に対して縮小しながら右に２回転
	// ウィンドウアニメ：タイプ３２：中心に対して縮小しながら左に２回転
	// ウィンドウアニメ：タイプ３３：中心に対して拡大しながら右に１回転
	// ウィンドウアニメ：タイプ３４：中心に対して拡大しながら左に１回転
	// ウィンドウアニメ：タイプ３５：中心に対して拡大しながら右に２回転
	// ウィンドウアニメ：タイプ３６：中心に対して拡大しながら左に２回転
	// ウィンドウアニメ：タイプ３７：中心に対して上下縮小しながら右に１回転
	// ウィンドウアニメ：タイプ３８：中心に対して上下縮小しながら左に１回転
	// ウィンドウアニメ：タイプ３９：中心に対して左右縮小しながら右に１回転
	// ウィンドウアニメ：タイプ４０：中心に対して左右縮小しながら左に１回転
	// ウィンドウアニメ：タイプ４１：中心に対して上下縮小しながら右に半回転
	// ウィンドウアニメ：タイプ４２：中心に対して上下縮小しながら左に半回転
	// ウィンドウアニメ：タイプ４３：中心に対して左右縮小しながら右に半回転
	// ウィンドウアニメ：タイプ４４：中心に対して左右縮小しながら左に半回転
	// ウィンドウアニメ：タイプ４５：中心に対して上下縮小しながら右に四半回転
	// ウィンドウアニメ：タイプ４６：中心に対して上下縮小しながら左に四半回転
	// ウィンドウアニメ：タイプ４７：中心に対して左右縮小しながら右に四半回転
	// ウィンドウアニメ：タイプ４８：中心に対して左右縮小しながら左に四半回転
	else if (type_09_48_flag)	{
		tr_anime_flag = true;
	}

	// ウィンドウアニメ：タイプ９９（内部てすと用）
	else if (anime_type == 99)	{
		pos_rep.x = (255 - anime_prog) * 800 / 255;
	}

	// --------------------------------------------------------------------------------------
	// 透明度アニメ処理
	if (tr_anime_flag)	{
		tr_rep = C_tnm_tool::time_table<int, int>(anime_prog, 0, 0, 224, 255);
	}

	// --------------------------------------------------------------------------------------
	// 拡大縮小系のアニメ処理

	// 上下
	if (ud_scale_mod)	{	// 0=なし 1=拡大 2=縮小
		if (ud_scale_center == 0)	{		// 中心
			abs_center.y = window_rect->top + (window_rect->height() / 2);
		}
		else if (ud_scale_center == 1)	{	// 上端
			abs_center.y = window_rect->top;
		}
		else if (ud_scale_center == 2)	{	// 下端
			abs_center.y = window_rect->bottom;
		}
		else if (ud_scale_center == 3)	{	// 画面上端
			abs_center.y = -(Gp_global->game_screen_size.cy / 16);
		}
		else if (ud_scale_center == 4)	{	// 画面下端
			abs_center.y = Gp_global->game_screen_size.cy + (Gp_global->game_screen_size.cy / 16);
		}
		int start_scale = (ud_scale_mod == 1) ? (TNM_SCALE_UNIT * 3) : 0;
		if (anime_open)	{	abs_scale.cy = speed_down_limit(anime_prog, 0, start_scale, 255, TNM_SCALE_UNIT);	}
		else			{	abs_scale.cy = speed_up_limit(anime_prog, 0, start_scale, 255, TNM_SCALE_UNIT);	}
	}

	// 左右
	if (lr_scale_mod)	{	// 0=なし 1=拡大 2=縮小
		if (lr_scale_center == 0)	{		// 中心
			abs_center.x = window_rect->left + (window_rect->width() / 2);
		}
		else if (lr_scale_center == 1)	{	// 左端
			abs_center.x = window_rect->left;
		}
		else if (lr_scale_center == 2)	{	// 右端
			abs_center.x = window_rect->right;
		}
		else if (lr_scale_center == 3)	{	// 画面左端
			abs_center.x = -(Gp_global->game_screen_size.cx / 16);
		}
		else if (lr_scale_center == 4)	{	// 画面右端
			abs_center.x = Gp_global->game_screen_size.cx + (Gp_global->game_screen_size.cx / 16);
		}
		int start_scale = (lr_scale_mod == 1) ? (TNM_SCALE_UNIT * 3) : 0;
		if (anime_open)	{	abs_scale.cx = speed_down_limit(anime_prog, 0, start_scale, 255, TNM_SCALE_UNIT);	}
		else			{	abs_scale.cx = speed_up_limit(anime_prog, 0, start_scale, 255, TNM_SCALE_UNIT);		}
	}

	// --------------------------------------------------------------------------------------
	// 回転系のアニメ処理

	if (rotate_cnt)	{
		int start_rotate = rotate_cnt * 90 * TNM_ANGLE_UNIT;		// 4 で１回転
		if (anime_open)	{	abs_rotate = speed_down_limit(anime_prog, 0, start_rotate, 255, 0);		}
		else			{	abs_rotate = speed_up_limit(anime_prog, 0, start_rotate, 255, 0);		}
	}

	// --------------------------------------------------------------------------------------
	// パラメータ移植

	*ret_pos_rep += pos_rep;
	*ret_abs_center = abs_center;
	*ret_abs_scale = abs_scale;
	*ret_abs_rotate = abs_rotate;
	*ret_tr_rep = (BYTE)((int)*ret_tr_rep * (int)tr_rep / 255);
}

// ****************************************************************
// メッセージウィンドウ：メッセージを進める
// ================================================================
void C_elm_mwnd::msg_frame()
{
	// ================================================================
	// メッセージウィンドウの処理順序
	// ----------------------------------------------------------------
	//		１．名前を表示する
	//		２．開きアニメを実行する
	//		３．メッセージを表示
	// ----------------------------------------------------------------

	// 自動処理フラグが立っている場合
	if (m_auto_proc_ready_flag)	{
		
		// 「音声あり」または「音声なし」の場合はメッセージウィンドウを表示する
		// 「音声のみ」でも、声がないまたは声を再生しないメッセージの場合はメッセージウィンドウを表示する
		if (Gp_config->koe_mode == TNM_KOE_MODE_NORMAL || Gp_config->koe_mode == TNM_KOE_MODE_TEXTONLY || m_koe_no == -1 || !m_koe_play_flag)	{

			// 名前を表示
			m_name_appear = true;

			// ウィンドウを開く
			open();

			// アニメが終わっていればメッセージを表示する
			if (m_open_anime_type == -1)	{

				// スライドアニメ中なら何もしない
				if (m_slide_anime_type == -1)	{

					// 選択肢
					if (!m_sel.is_empty())	{
						// 選択肢を表示する
						m_sel.set_disp();
					}

					// 通常のメッセージ
					for (int i = 0; i < (int)m_msg_list.size(); i++)	{

						// メッセージ開始処理
						if (!m_msg_list[i].get_progress_start_flag())	{
							m_msg_list[i].set_progress_start_flag(true);

							// 最後に表示した時間をリセット
							m_msg_list[i].set_last_moji_disp_time(m_time);
						}

						// 時間が来たら次の文字を表示する
						while (m_msg_list[i].check_not_appear_moji() && (m_time - (m_msg_list[i].get_last_moji_disp_time() + tnm_get_total_msg_speed()) >= 0))	{

							// 最後に表示した時間を覚えておく
							m_msg_list[i].set_last_moji_disp_time(m_msg_list[i].get_last_moji_disp_time() + tnm_get_total_msg_speed());

							// オーバーフロー中は表示せずに抜ける
							//if (m_msg_list[i].check_over_flow())
							//	break;

							// １文字表示
							m_msg_list[i].disp_one_moji();
						}
					}
				}
			}
		}
		// 「音声のみ」で声があるメッセージの場合はウィンドウを表示しない
		else	{

			// 名前を非表示
			m_name_appear = false;

			// ウィンドウを閉じる
			close();

			// アニメが終わっていていればメッセージを表示する
			if (m_close_anime_type == -1)	{

				// メッセージ開始
				for (int i = 0; i < (int)m_msg_list.size(); i++)	{
					m_msg_list[i].set_progress_start_flag(true);
				}

				// 一括表示（実際は表示されない）
				disp_all_moji();
			}
		}

		// 全て表示したら自動処理を抜ける
		if (!check_not_appear_moji())
			m_auto_proc_ready_flag = false;
	}
}

// ****************************************************************
// メッセージウィンドウ：メッセージウェイトまで処理を進める
// ================================================================
void C_elm_mwnd::jump_to_msg_wait_by_nowait()
{
	// 自動処理フラグが立っている場合
	if (m_auto_proc_ready_flag)	{

		// 「音声あり」または「音声なし」、または声がない、声を再生しないメッセージの場合はウィンドウを表示する
		if (Gp_config->koe_mode == TNM_KOE_MODE_NORMAL || Gp_config->koe_mode == TNM_KOE_MODE_TEXTONLY || m_koe_no == -1 || !m_koe_play_flag)	{
			m_name_appear = true;		// 名前を表示
			open();						// ウィンドウを開く
//			if (m_open_anime_type == -1)	{
				end_open_anime();				// 開きアニメを終了する
				if (!is_slide_anime_doing())	{
					for (int i = 0; i < (int)m_msg_list.size(); i++)	{
						m_msg_list[i].set_progress_start_flag(true);	// メッセージ開始
					}
					disp_all_moji();				// メッセージを表示する

					// 全て表示したら自動処理を抜ける
					if (!check_not_appear_moji())
						m_auto_proc_ready_flag = false;
				}
//			}
		}
		// 「音声のみ」で声があるメッセージの場合はウィンドウを表示しない
		else	{
			m_name_appear = false;		// 名前を非表示
			m_window_appear = false;	// ウィンドウを非表示
			end_open_anime();			// 開きアニメを終了する
			end_close_anime();			// 閉じアニメを終了する

//			close();					// ウィンドウを閉じる（これをやると無限呼び出しで強制終了になる）
//			if (m_close_anime_type == -1)	{
				end_close_anime();				// 閉じアニメを終了する
				if (!is_slide_anime_doing())	{
					for (int i = 0; i < (int)m_msg_list.size(); i++)	{
						m_msg_list[i].set_progress_start_flag(true);	// メッセージ開始
					}
					disp_all_moji();				// メッセージを表示する

					// 全て表示したら自動処理を抜ける
					if (!check_not_appear_moji())
						m_auto_proc_ready_flag = false;
				}
//			}
		}
	}
}

// ****************************************************************
// メッセージウィンドウ：フレームアクション処理
// ================================================================
void C_elm_mwnd::do_frame_action()
{
	// メッセージ枠
	m_msg_waku.do_frame_action();

	// 名前枠
	m_name_waku.do_frame_action();
}

// ****************************************************************
// オブジェクト：スプライトツリーを構築
// ================================================================
void C_elm_mwnd::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	// ウィンドウが開いているか、ウィンドウが閉じるアニメ中なら表示されるべき
	if (m_window_appear || (m_close_anime_type >= 0))	{

		// メッセージ
		for (int i = 0; i < (int)m_msg_list.size(); i++)
			m_msg_list[i].get_sprite_tree(root);

		// メッセージ枠
		m_msg_waku.get_sprite_tree(root);

		// 名前
		m_name.get_sprite_tree(root);

		// 名前枠
		m_name_waku.get_sprite_tree(root);

		// 選択肢
		m_sel.get_sprite_tree(root);
	}
}

// ****************************************************************
// ボタンの登録処理
// ================================================================
void C_elm_mwnd::regist_button()
{
	// ウィンドウが開いているか、ウィンドウが閉じるアニメ中なら表示されるべき
	if (m_window_appear || (m_close_anime_type >= 0))	{

		// メッセージ
		for (int i = 0; i < (int)m_msg_list.size(); i++)
			m_msg_list[i].regist_button();

		// 選択肢
		if (tnm_get_cur_proc()->type == TNM_PROC_TYPE_SEL)	{
			m_sel.regist_button();
		}

		// 枠
		m_msg_waku.regist_button();
	}
}

// ****************************************************************
// ボタンイベント処理
// ================================================================
void C_elm_mwnd::button_event(bool wait_flag)
{
	if (m_window_appear)	{

		// メッセージ
		for (int i = 0; i < (int)m_msg_list.size(); i++)
			m_msg_list[i].button_event();

		// 選択肢
		m_sel.button_event(wait_flag);

		// 枠
		m_msg_waku.button_event();
	}
}

// ****************************************************************
// メッセージブロックを開始したフラグを設定する
// ================================================================
void C_elm_mwnd::clear_msg_block_start_flag()
{
	m_msg_block_started_flag = false;
}
void C_elm_mwnd::set_msg_block_start_flag()
{
	m_msg_block_started_flag = true;
}
bool C_elm_mwnd::is_msg_block_started()
{
	return m_msg_block_started_flag;
}

// ****************************************************************
// 自動処理開始
// ================================================================
void C_elm_mwnd::set_auto_proc_ready()
{
	m_auto_proc_ready_flag = true;
}

// ****************************************************************
// 自動処理終了
// ================================================================
void C_elm_mwnd::clear_auto_proc_ready()
{
	m_auto_proc_ready_flag = false;
}

// ****************************************************************
// 開く
// ================================================================
void C_elm_mwnd::open()
{
	// 閉じアニメ終了
	end_close_anime();

	// 閉じているなら開きアニメ開始
	if (!m_window_appear)	{
		m_window_appear = true;

		if (tnm_is_mwnd_anime_use())	{
			m_open_anime_type = m_param.open_anime_type;
			m_open_anime_time = m_param.open_anime_time;
			m_open_anime_start_time = m_time;
		}
	}
}

// ****************************************************************
// 閉じる
// ================================================================
void C_elm_mwnd::close()
{
	// メッセージ処理を最後まで進める
	jump_to_msg_wait_by_nowait();

	// 閉きアニメ終了
	end_open_anime();

	// 開いているなら閉じアニメ開始
	if (m_window_appear)	{
		m_window_appear = false;

		if (tnm_is_mwnd_anime_use())	{
			m_close_anime_type = m_param.close_anime_type;
			m_close_anime_time = m_param.close_anime_time;
			m_close_anime_start_time = m_time;
		}
	}
}

// ****************************************************************
// 閉じるを終了
// ================================================================
void C_elm_mwnd::end_close()
{
	// 閉じアニメ終了
	end_close_anime();
}

// ****************************************************************
// 声の処理
// ================================================================

// 声のクリア
void C_elm_mwnd::clear_koe()
{
	m_koe_no = -1;
	m_koe_play_flag = false;
}

// 声を設定
void C_elm_mwnd::set_koe(int koe_no, bool play_flag, bool no_auto_mode_flag)
{
	clear_koe();

	// 名前を設定する
	m_koe_no = koe_no;
	m_koe_play_flag = play_flag;
	m_koe_no_auto_mode_flag = no_auto_mode_flag;
}

// ****************************************************************
// 名前処理
// ================================================================

// 名前のクリア
void C_elm_mwnd::clear_name()
{
	m_name.clear_name();
}

// 名前の設定
void C_elm_mwnd::set_name(CTSTR& name)
{
	// 名前を設定する
	m_name.set_name(name);

	// 名前枠の再構築
	restruct_name_waku();
}

// 名前の取得
TSTR C_elm_mwnd::get_name()
{
	return m_name.get_name();
}

// ****************************************************************
// 顔グラ処理
// ================================================================

// 顔グラのクリア
void C_elm_mwnd::clear_face()
{
	m_msg_waku.clear_face();
}

// 顔グラの設定
void C_elm_mwnd::set_face(int face_no, CTSTR& face_path)
{
	// 名前を設定する
	m_msg_waku.set_face(face_no, face_path);
}

// ****************************************************************
// メッセージ処理
// ================================================================

// メッセージのクリア
void C_elm_mwnd::clear_msg()
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].clear_msg();

	// ターゲットメッセージを元に戻す
	m_target_msg_no = 0;

	// メッセージを１つに戻す
	m_msg_list.resize(1);

	// オートモードここまで待ったカウントをリセット
	m_auto_mode_end_moji_cnt = 0;

	// キー待ちアイコン
	m_msg_waku.set_key_icon_appear(false);
}

// メッセージが入るかをチェック
bool C_elm_mwnd::add_msg_check(bool new_line_flag)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return false;

	// メッセージが入るかをチェック
	return m_msg_list[m_target_msg_no].add_msg_check(m_param.over_flow_check_size, new_line_flag);
}

// メッセージの追加
void C_elm_mwnd::add_msg(CTSTR& msg, TSTR* ret_over_flow_msg)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	// メッセージを追加する
	m_msg_list[m_target_msg_no].add_msg(msg, ret_over_flow_msg);

	// メッセージ枠を再構築する
	restruct_msg_waku();
}

// 名前をメッセージとして追加
void C_elm_mwnd::add_name_to_msg(CTSTR& name, TSTR* ret_over_flow_msg)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	// メッセージを追加する
	m_msg_list[m_target_msg_no].add_name_to_msg(name, ret_over_flow_msg);

	// メッセージ枠を再構築する
	restruct_msg_waku();
}

// 文字数を取得
int C_elm_mwnd::get_moji_cnt()
{
	return m_msg_list[0].get_moji_cnt();
}

// 表示した
int C_elm_mwnd::get_disp_moji_cnt()
{
	return m_msg_list[0].get_disp_moji_cnt();
}

// １つの文字を表示
void C_elm_mwnd::disp_one_moji()
{
	m_msg_list[0].disp_one_moji();
	m_sel.set_disp();
}

// 全ての文字を表示
void C_elm_mwnd::disp_all_moji()
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].disp_all_moji();

	m_sel.set_disp();
}

// 行の先頭にする
void C_elm_mwnd::set_line_head()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_line_head();
}
// 改行
void C_elm_mwnd::new_line_indent()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].new_line_indent();
}
void C_elm_mwnd::new_line_no_indent()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].new_line_no_indent();
}

// インデント設定
void C_elm_mwnd::set_indent()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_indent(0);
}

// インデントをクリア
void C_elm_mwnd::clear_indent()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].clear_indent();
}

// 文字の位置を設定
void C_elm_mwnd::set_moji_rep_pos(C_point pos)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_rep_pos(pos);
}

// 文字の位置をデフォルトに戻す
void C_elm_mwnd::set_moji_rep_pos_default()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_rep_pos_default();
}

// 文字の大きさを設定
void C_elm_mwnd::set_moji_size(int size)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_size(size);
}

void C_elm_mwnd::set_moji_size_default()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_size_default();
}

// 文字の色を設定
void C_elm_mwnd::set_moji_color(int color_no)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_color(color_no);
}
void C_elm_mwnd::set_moji_color_default()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_color_default();
}

// キャラクタ文字色をクリア
void C_elm_mwnd::clear_chara_moji_color()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].clear_chara_moji_color();

	// 名前の色
	m_name.clear_chara_moji_color();
}
void C_elm_mwnd::clear_chara_shadow_color()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].clear_chara_shadow_color();

	// 名前の色
	m_name.clear_chara_shadow_color();
}
void C_elm_mwnd::clear_chara_fuchi_color()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].clear_chara_fuchi_color();

	// 名前の色
	m_name.clear_chara_fuchi_color();
}

// キャラクタ文字色を設定
void C_elm_mwnd::set_chara_moji_color(int color_no)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_chara_moji_color(color_no);

	// 名前の色
	m_name.set_chara_moji_color(color_no);
}
void C_elm_mwnd::set_chara_shadow_color(int color_no)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_chara_shadow_color(color_no);

	// 名前の色
	m_name.set_chara_shadow_color(color_no);
}
void C_elm_mwnd::set_chara_fuchi_color(int color_no)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_chara_fuchi_color(color_no);

	// 名前の色
	m_name.set_chara_fuchi_color(color_no);
}

// ルビを設定
void C_elm_mwnd::start_ruby(CTSTR& ruby)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].start_ruby(ruby);
}
void C_elm_mwnd::end_ruby()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].end_ruby();
}

// ボタンを設定
void C_elm_mwnd::start_button(int btn_id, int group_no, int action_no, int se_no)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].start_button(btn_id, group_no, action_no, se_no);
}
void C_elm_mwnd::end_button()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].end_button();
}

// 最後に表示した時間を設定する
void C_elm_mwnd::set_last_moji_disp_time()
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].set_last_moji_disp_time(m_time);
}

// デバッグメッセージを取得
TSTR C_elm_mwnd::get_debug_msg()
{
	TSTR msg;
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		msg += m_msg_list[i].get_debug_msg();

	msg += m_sel.get_msg();

	return msg;
}

// ****************************************************************
// マルチメッセージ
// ================================================================

// 次のメッセージへ
void C_elm_mwnd::next_msg()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &Gp_ini->mwnd.sub[m_mwnd_no];

	// ターゲットを移動する
	m_target_msg_no ++;

	// メッセージを自動確保する
	if (m_target_msg_no >= (int)m_msg_list.size())	{
		m_msg_list.resize(m_target_msg_no + 1);
		m_msg_list[m_target_msg_no].init(m_def_elm_group_list, this);
		m_msg_list[m_target_msg_no].set_def_param(p_ini_sub->moji_cnt
			, p_ini_sub->moji_size, p_ini_sub->moji_space, p_ini->moji_color, p_ini->shadow_color, p_ini->fuchi_color, p_ini_sub->ruby_size, p_ini_sub->ruby_space
			, p_ini_sub->name_moji_size, p_ini_sub->name_newline, p_ini_sub->name_bracket, p_ini_sub->talk_margin);
	}

	// 位置を自動調整する
	C_point msg_pos = m_msg_list[m_target_msg_no - 1].get_cur_moji_pos();
	m_msg_list[m_target_msg_no].set_cur_moji_pos(msg_pos);
}

// ****************************************************************
// メッセージ速度計算関連
// ================================================================
void C_elm_mwnd::set_progress_start_flag(bool value)
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].set_progress_start_flag(value);
}

// ****************************************************************
// 選択肢処理
// ================================================================

// 選択肢をクリア
void C_elm_mwnd::clear_sel()
{
	m_sel.clear_sel();
}

// 選択肢を準備
void C_elm_mwnd::ready_sel()
{
	// 現在位置が先頭でない場合は改行する
	m_msg_list[0].new_line_if_not_line_head();

	// 現在の文字位置が最初の選択肢を置く位置になる
	m_sel.set_def_pos(m_msg_list[0].get_cur_moji_pos());
}

// 選択肢を追加
void C_elm_mwnd::add_sel(CTSTR& sel, int item_type, int color)
{
	// 選択肢の個数－１個、改行を入れる必要がある
	if (m_sel.is_empty())	{
		m_sel.add_sel(sel, item_type, color);
	}
	else	{
		m_sel.add_sel(sel, item_type, color);
		m_msg_list[m_target_msg_no].new_line_no_indent();
	}
}

// キャンセル可能フラグを設定
void C_elm_mwnd::set_sel_cancel_enable(bool enable)
{
	m_sel.set_cancel_enable(enable);
}

// 選択肢を整える
void C_elm_mwnd::restruct_sel()
{
	// 選択肢を整える
	m_sel.restruct_item(m_param.extend_type);

	// 可変ウィンドウの場合は矩形を再生成する！！
	if (m_param.extend_type == 1)	{

		// メッセージサイズを取得
		C_size msg_size = m_sel.get_msg_size();

		// ウィンドウサイズを求める
		m_param.window_size.cx = msg_size.cx + m_param.msg_margin.left + m_param.msg_margin.right;
		m_param.window_size.cy = msg_size.cy + m_param.msg_margin.top + m_param.msg_margin.bottom;

		// ウィンドウ矩形を求める
		C_rect window_rect = C_rect::by_size(m_param.window_pos, m_param.window_size);

		// ウィンドウ枠を更新
		m_msg_waku.set_extend_waku(&window_rect);

		// ウィンドウサイズを更新する
		m_param.window_size = window_rect.size();
	}
}

// ****************************************************************
// 枠の処理
// ================================================================

// メッセージ枠の再構築
bool C_elm_mwnd::restruct_msg_waku()
{
	if (false);
	else if (m_param.extend_type == 0)	{

		// ウィンドウ矩形を求める
		C_rect window_rect = C_rect::by_size(m_param.window_pos, m_param.window_size);

		// メッセージ枠を作成（ウィンドウサイズが変わることがあります）
		if (!m_msg_waku.set_extend_waku(&window_rect))
			return false;

		// ウィンドウサイズを更新する
		m_param.window_size = window_rect.size();
	}
	else if (m_param.extend_type == 1)	{

		// ウィンドウサイズを設定
		m_param.window_size.cx = m_msg_list[0].get_msg_size().cx + m_param.msg_margin.left + m_param.msg_margin.right;
		m_param.window_size.cy = m_msg_list[0].get_msg_size().cy + m_param.msg_margin.top + m_param.msg_margin.bottom;

		// ウィンドウ矩形を求める
		C_rect window_rect = C_rect::by_size(m_param.window_pos, m_param.window_size);

		// メッセージ枠を作成（ウィンドウサイズが変わることがあります）
		if (!m_msg_waku.set_extend_waku(&window_rect))
			return false;

		// ウィンドウサイズを更新する
		m_param.window_size = window_rect.size();
	}

	return true;
}

// 名前枠の再構築
bool C_elm_mwnd::restruct_name_waku()
{
	if (false);
	else if (m_param.name_extend_type == 0)	{

		// 名前ウィンドウ矩形を求める
		m_param.name_window_rect = C_rect(C_point(0, 0), m_param.name_window_size);
		
		// 名前枠を作成（ウィンドウサイズが変わることがあります）
		if (!m_name_waku.set_extend_waku(&m_param.name_window_rect))
			return false;

		// 揃え位置にそろえる
		if (false);
		else if (m_param.name_window_align == 0)	{	// 左寄せ
			// NOP
		}
		else if (m_param.name_window_align == 1)	{	// 中央寄せ
			m_param.name_window_rect -= C_point(m_param.name_window_rect.width() / 2, 0);
		}
		else if (m_param.name_window_align == 2)	{	// 右寄せ
			m_param.name_window_rect -= C_point(m_param.name_window_rect.width(), 0);
		}
	}
	else if (m_param.name_extend_type == 1)	{

		// 名前矩形を取得
		C_rect name_msg_rect = m_name.get_msg_rect();

		// 名前枠矩形を計算
		m_param.name_window_rect.left = name_msg_rect.left - m_param.msg_margin.left;
		m_param.name_window_rect.top = name_msg_rect.top - m_param.msg_margin.top;
		m_param.name_window_rect.right = name_msg_rect.right + m_param.msg_margin.right;
		m_param.name_window_rect.bottom = name_msg_rect.bottom + m_param.msg_margin.bottom;

		// 名前枠を作成（ウィンドウサイズが変わることがあります）
		if (!m_name_waku.set_extend_waku(&m_param.name_window_rect))
			return false;
	}

	return true;
}

// ****************************************************************
// キー待ちアイコンの表示設定
// ================================================================
void C_elm_mwnd::set_key_icon_appear(bool appear)
{
	// 表示フラグ
	m_msg_waku.set_key_icon_appear(appear);

	// アイコンの位置を調べる
	C_point icon_pos(0, 0);
	if (!m_msg_list.empty())
	{
		icon_pos = m_msg_list.back().get_cur_moji_pos();

		// 文字位置とアイコン位置は向きが違うようです
		if (Gp_ini->tateyoko_mode == 1)
		{
			icon_pos.x *= -1;
		}
	}

	// アイコンの位置を設定
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_msg_waku.set_key_icon_pos(m_param.msg_pos + icon_pos);
	}
	else
	{
		m_msg_waku.set_key_icon_pos(m_param.msg_pos + icon_pos);
	}
}
void C_elm_mwnd::set_key_icon_mode(int mode)
{
	// モード
	m_msg_waku.set_key_icon_mode(mode);
}

// ****************************************************************
// クリア
// ================================================================
void C_elm_mwnd::clear()
{
	// クリア
	clear_face();		// 顔グラをクリア
	clear_koe();		// 声をクリア
	clear_name();		// 名前をクリア
	clear_msg();		// メッセージをクリア
	clear_sel();		// 選択肢をクリア

	// クリア準備フラグを降ろす
	m_clear_ready_flag = false;
}

// ****************************************************************
// ノベルに対してメッセージ単位のクリア処理
// ================================================================
void C_elm_mwnd::clear_for_novel_one_msg()
{
	// キャラクタ文字色をクリア
	clear_chara_moji_color();
	clear_chara_shadow_color();
	clear_chara_fuchi_color();

	// 声番号をクリア
	clear_koe();

	// メッセージ開始処理フラグを降ろす
	for (int i = 0; i < (int)m_msg_list.size(); i++)	{
		m_msg_list[i].set_progress_start_flag(false);
	}

	// メッセージタイプ確定フラグを降ろす
	for (int i = 0; i < (int)m_msg_list.size(); i++)	{
		m_msg_list[i].set_cur_msg_type_decided(false);
	}
}

// ****************************************************************
// メッセージが空かどうかを調べる
// ================================================================
bool C_elm_mwnd::is_empty()
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)	{
		if (!m_msg_list[i].is_empty())
			return false;
	}

	if (!m_name.is_empty())
		return false;

	if (!m_sel.is_empty())
		return false;

	return true;
}

// ****************************************************************
// まだ出現していない文字が存在するか判定する
// ================================================================
bool C_elm_mwnd::check_not_appear_moji()
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)	{
		if (m_msg_list[i].check_not_appear_moji())
			return true;
	}

	if (m_sel.check_not_appear_moji())
		return true;

	return false;
}

// ****************************************************************
// 既読フラグを追加する
// ================================================================
void C_elm_mwnd::add_read_flag(int scn_no, int flag_no)
{
	S_tnm_read_flag read_flag;
	read_flag.scn_no = scn_no;
	read_flag.flag_no = flag_no;
	m_read_flag_stock_list.push_back(read_flag);
}

// ****************************************************************
// 既読フラグをクリアする
// ================================================================
void C_elm_mwnd::clear_read_flag()
{
	m_read_flag_stock_list.clear();
}

// ****************************************************************
// 登録した既読フラグを設定する
// ================================================================
void C_elm_mwnd::set_read_flag()
{
	for (int i = 0; i < (int)m_read_flag_stock_list.size(); i++)	{
		int scn_no = m_read_flag_stock_list[i].scn_no;		// 既読フラグのシーン番号
		int flag_no = m_read_flag_stock_list[i].flag_no;	// 既読フラグのフラグ番号
		if (scn_no >= 0 && flag_no >= 0)
			(*Gp_read_flag)[scn_no][flag_no] = 1;				// 既読！
	}

	// 既読フラグの登録をクリア
	m_read_flag_stock_list.clear();
}

// ****************************************************************
// アニメを終わらせる
// ================================================================
void C_elm_mwnd::end_open_anime()
{
	m_open_anime_type = -1;
}

void C_elm_mwnd::end_close_anime()
{
	m_close_anime_type = -1;
}

void C_elm_mwnd::end_slide_anime()
{
	m_slide_anime_type = -1;
}

// ****************************************************************
// アニメ中を判定
// ================================================================
bool C_elm_mwnd::is_open_anime_doing()
{
	return m_open_anime_type >= 0;
}

bool C_elm_mwnd::is_close_anime_doing()
{
	return m_close_anime_type >= 0;
}

bool C_elm_mwnd::is_slide_anime_doing()
{
	return m_slide_anime_type >= 0;
}

// ****************************************************************
// 文字列スライド
// ================================================================
void C_elm_mwnd::slide_msg()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].slide_msg();

	// スライドアニメの情報を設定
	m_slide_pixel = m_msg_list[m_target_msg_no].get_slide_pixel();
	m_slide_anime_type = 0;
	m_slide_anime_time = m_slide_time;
	m_slide_anime_start_time = m_time;
}

// ****************************************************************
// パラメータを初期化
// ================================================================
void C_elm_mwnd::init_window_pos()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.window_pos = p_ini_sub->window_pos;
}
void C_elm_mwnd::init_window_size()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.window_size = p_ini_sub->window_size;
}
void C_elm_mwnd::init_open_anime_type()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.open_anime_type = p_ini_sub->open_anime_type;
}
void C_elm_mwnd::init_open_anime_time()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.open_anime_time = p_ini_sub->open_anime_time;
}
void C_elm_mwnd::init_close_anime_type()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.close_anime_type = p_ini_sub->close_anime_type;
}
void C_elm_mwnd::init_close_anime_time()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.close_anime_time = p_ini_sub->close_anime_time;
}

// ****************************************************************
// デフォルトパラメータを取得
// ================================================================
int C_elm_mwnd::get_default_open_anime_type()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	return p_ini_sub->open_anime_type;
}
int C_elm_mwnd::get_default_open_anime_time()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	return p_ini_sub->open_anime_time;
}
int C_elm_mwnd::get_default_close_anime_type()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	return p_ini_sub->close_anime_type;
}
int C_elm_mwnd::get_default_close_anime_time()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	return p_ini_sub->close_anime_time;
}

// ****************************************************************
// メッセージパラメータ
// ================================================================
void C_elm_mwnd::init_window_moji_cnt()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.msg_moji_cnt = p_ini_sub->moji_cnt;

	// メッセージに反映
	int msg_cnt = (int)m_msg_list.size();
	for (int i = 0; i < msg_cnt; i++)	{
		m_msg_list[i].init_moji_cnt();
	}
}
void C_elm_mwnd::set_window_moji_cnt(C_size moji_cnt)
{
	m_param.msg_moji_cnt = moji_cnt;

	// メッセージに反映
	int msg_cnt = (int)m_msg_list.size();
	for (int i = 0; i < msg_cnt; i++)	{
		m_msg_list[i].set_moji_cnt(moji_cnt);
	}
}
C_size C_elm_mwnd::get_window_moji_cnt()
{
	return m_param.msg_moji_cnt;
}


// ****************************************************************
// メッセージウィンドウリスト：初期化
// ================================================================
void C_elm_mwnd_list::init(S_element element, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent, S_element elm_group_list)
{
	// 固定パラメータ
	m_elm_group_list = elm_group_list;

	// 基底クラスの初期化
	C_elm_list_ex<C_elm_mwnd>::init(element, FM_MWNDLIST, name, size, extend_enable, p_parent);
}

// ****************************************************************
// メッセージウィンドウリスト：終了
// ================================================================
void C_elm_mwnd_list::finish()
{
	int mwnd_cnt = (int)m_sub.size();
	for (int i = 0; i < mwnd_cnt; i++)
		m_sub[i].finish();
}

// ****************************************************************
// メッセージウィンドウリスト：サブの初期化
// ================================================================
void C_elm_mwnd_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(i, get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"), this, tnm_set_element_down(m_elm_group_list));
}

// ****************************************************************
// メッセージウィンドウリスト：サブの再初期化
// ================================================================
void C_elm_mwnd_list::_reinit(int begin, int end, bool restruct_flag)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit(restruct_flag);
}

// ****************************************************************
// メッセージウィンドウリスト：サブのセーブ
// ================================================================
void C_elm_mwnd_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// メッセージウィンドウリスト：サブのロード
// ================================================================
void C_elm_mwnd_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}


