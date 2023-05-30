#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_local_save.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_timer.h"
#include	"data/tnm_tool.h"
#include	"data/tnm_color_table.h"
#include	"element/elm_msg_back.h"
#include	"element/elm_mwnd_moji.h"
#include	"engine/eng_save.h"
#include	"engine/eng_input.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_syscom.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// 初期化
// ================================================================
void C_elm_msg_back::init()
{
	// デフォルトのパラメータを設定する
	m_def.window_rect = C_rect::by_size(Gp_ini->msg_back.window_pos, Gp_ini->msg_back.window_size);
	m_def.disp_margin = Gp_ini->msg_back.disp_margin;
	m_def.msg_pos = Gp_ini->msg_back.msg_pos;
	m_def.koe_btn_pos = Gp_ini->msg_back.koe_btn_pos;
	m_def.load_btn_pos = Gp_ini->msg_back.load_btn_pos;
	m_history_cnt_max = Gp_ini->msg_back.history_cnt;

	// ヒストリーを構築する
	m_history_list.clear();
	m_history_list.resize(m_history_cnt_max);
	for (int i = 0; i < m_history_cnt_max; i++)	{
		m_history_list[i].msg.init(S_element(), NULL);
		m_history_list[i].msg.set_def_param(Gp_ini->msg_back.moji_cnt
			, Gp_ini->msg_back.moji_size, Gp_ini->msg_back.moji_space, Gp_ini->mwnd.moji_color, Gp_ini->mwnd.shadow_color, Gp_ini->mwnd.fuchi_color, 10, 10
			, Gp_ini->msg_back.moji_size, 0, 0, C_rect(0, 0, 0, 0));
	}

	// セパレータ
	m_separator_list.clear();

	// スプライトをクリア
	m_spr_waku.init();
	m_spr_filter.init();

	// アルバムをクリア
	m_album_waku.reset();
	m_album_filter.reset();
	m_album_separator.reset();
	m_album_separator_top.reset();
	m_album_separator_bottom.reset();

	// 処理変数を初期化する
	init_work_variable();
}

// ****************************************************************
// セーブ
// ================================================================
void C_elm_msg_back::save(C_tnm_save_stream& stream)
{
	stream.save(m_history_cnt);
	for (int i = 0; i < m_history_cnt; i++)
	{
		stream.save(m_history_list[i].pct_flag);
		stream.save(m_history_list[i].msg_str);
		stream.save(m_history_list[i].original_name);
		stream.save(m_history_list[i].disp_name);
		stream.save(m_history_list[i].pct_pos);
		stream.save(m_history_list[i].koe_no_list);
		stream.save(m_history_list[i].chr_no_list);
		stream.save(m_history_list[i].koe_play_no);
		stream.save(m_history_list[i].debug_msg);
		stream.save(m_history_list[i].scn_no);
		stream.save(m_history_list[i].line_no);
		stream.save(m_history_list[i].save_id);
		stream.save(m_history_list[i].save_id_check_flag);
	}
	stream.save(m_history_start_pos);
	stream.save(m_history_last_pos);
	stream.save(m_history_insert_pos);
	stream.save(m_new_msg_flag);
}

// ****************************************************************
// ロード
// ================================================================
void C_elm_msg_back::load(C_tnm_save_stream& stream)
{
	stream.load(m_history_cnt);
	for (int i = 0; i < m_history_cnt; i++)
	{
		stream.load(m_history_list[i].pct_flag);
		stream.load(m_history_list[i].msg_str);
		stream.load(m_history_list[i].original_name);
		stream.load(m_history_list[i].disp_name);
		stream.load(m_history_list[i].pct_pos);
		stream.load(m_history_list[i].koe_no_list);
		stream.load(m_history_list[i].chr_no_list);
		stream.load(m_history_list[i].koe_play_no);
		stream.load(m_history_list[i].debug_msg);
		stream.load(m_history_list[i].scn_no);
		stream.load(m_history_list[i].line_no);
		stream.load(m_history_list[i].save_id);
		stream.load(m_history_list[i].save_id_check_flag);
	}
	stream.load(m_history_start_pos);
	stream.load(m_history_last_pos);
	stream.load(m_history_insert_pos);
	stream.load(m_new_msg_flag);
}

// ****************************************************************
// 整理する
// ================================================================
void C_elm_msg_back::remove()
{
	// マップが大きくなりすぎた場合のみ処理
	if ((int)Gp_backlog_save->map.size() > Gp_ini->backlog_save_cnt)
	{
		// 新しいマップ
		std::map<S_tid, BSP<S_tnm_local_save>> new_map;

		// メッセージバックを逆からチェックしていく
		for (int i = m_history_cnt - 1; i >= 0; i--)
		{
			int history_pos = (m_history_start_pos + i) % m_history_cnt_max;
			S_tid save_id = m_history_list[history_pos].save_id;

			if (save_id != S_tid())
			{
				// セーブデータが実際に存在するかを確認
				std::map<S_tid, BSP<S_tnm_local_save>>::iterator save_find = Gp_backlog_save->map.find(save_id);

				// セーブデータが存在すれば
				if (save_find != Gp_backlog_save->map.end())
				{
					// 新しいマップに追加
					new_map.insert(std::pair<S_tid, BSP<S_tnm_local_save>>(save_id, save_find->second));

					// サイズが限界まで達したら終了
					if ((int)new_map.size() >= Gp_ini->backlog_save_cnt)
					{
						break;
					}

				}
			}
		}

		// マップを交換
		Gp_backlog_save->map = new_map;
	}
}

// ****************************************************************
// 処理変数初期化
// ================================================================
void C_elm_msg_back::init_work_variable()
{
	m_cur = m_def;	// デフォルトパラメータを現在のパラメータへコピーする
	m_window_appear = false;

	m_history_cnt = 0;
	m_history_start_pos = 0;
	m_history_last_pos = 0;
	m_history_insert_pos = 0;
	m_new_msg_flag = true;

	m_msg_total_height = 0;
	m_target_no = -1;
	m_mouse_target_no = -1;

	m_scroll_pos = 0;
	m_slider_pos = Gp_ini->msg_back.slider_rect.top;
	m_slider_operating = false;
	m_slider_start_slider_pos = 0;
	m_slider_start_mouse_pos = 0;

	m_drag_ready_flag = false;
	m_drag_flag = false;
	m_drag_start_mouse_pos = C_point(0, 0);
	m_drag_start_scroll_pos = 0;

	m_drag_aftereffects_flag = false;
}

// ****************************************************************
// フレーム初期化処理
// ================================================================
void C_elm_msg_back::frame_init()
{
	// ウィンドウが非表示なら戻る
	if (!m_window_appear)
		return;
	
	// スプライトを初期化
	m_spr_waku.rp.init();
	m_spr_filter.rp.init();

	// セパレータ
	for (int i = 0; i < (int)m_separator_list.size(); i++)
		m_separator_list[i].sprite.rp.init();

	// テキスト
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].msg.frame_init();

	// 声ボタン
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].koe_btn.frame_init();

	// ロードボタン
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].load_btn.frame_init();

	// スライダー
	m_slider.frame_init();

	// ボタン
	m_close_btn.frame_init();
	m_msg_up_btn.frame_init();
	m_msg_down_btn.frame_init();
	m_ex_btn_1.frame_init();
	m_ex_btn_2.frame_init();
	m_ex_btn_3.frame_init();
	m_ex_btn_4.frame_init();
}

// ****************************************************************
// フレーム処理
// ================================================================
void C_elm_msg_back::frame(int time, bool draw_futoku, bool draw_shadow)
{
	// ウィンドウが非表示なら戻る
	if (!m_window_appear)
		return;

	// ファイルを使う場合
	if (m_spr_waku.get_texture())
	{
		// 枠スプライトのパラメータ
		m_spr_waku.rpex.sorter = S_tnm_sorter(Gp_ini->msg_back_order, Gp_ini->mwnd.waku_layer_rep);
		m_spr_waku.rp.disp = true;
		m_spr_waku.rp.d2_rect = true;
		m_spr_waku.rp.alpha_test = true;
		m_spr_waku.rp.alpha_blend = true;
		m_spr_waku.rp.pos = tnm_d2to3(Gp_ini->msg_back.window_pos);
		m_spr_waku.rp.size_fit_to_texture = 0;
	}

	// フィルタのスプライトパラメータ
	m_spr_filter.rpex.sorter = S_tnm_sorter(Gp_ini->msg_back_order, Gp_ini->mwnd.filter_layer_rep);
	m_spr_filter.rp.disp = true;
	m_spr_filter.rp.d2_rect = true;
	m_spr_filter.rp.alpha_test = true;
	m_spr_filter.rp.alpha_blend = true;
	m_spr_filter.rp.pos = tnm_d2to3(Gp_ini->msg_back.window_pos);
	m_spr_filter.rp.tr = Gp_config->filter_color.a;				// 透明度
	m_spr_filter.rp.color_add_b = Gp_config->filter_color.b;	// 色加算
	m_spr_filter.rp.color_add_g = Gp_config->filter_color.g;	// 色加算
	m_spr_filter.rp.color_add_r = Gp_config->filter_color.r;	// 色加算

	// ファイルを使う場合
	if (m_spr_filter.get_texture())
	{
		m_spr_filter.rp.size_fit_to_texture = 0;
	}
	// ファイルを使わない場合
	else
	{
		C_point pos = Gp_ini->msg_back.window_pos + Gp_ini->msg_back.filter_margin.left_top();
		m_spr_filter.rp.pos = tnm_d2to3(pos);
		m_spr_filter.rp.size.x = (float)Gp_ini->msg_back.window_size.cx - Gp_ini->msg_back.filter_margin.left - Gp_ini->msg_back.filter_margin.right;
		m_spr_filter.rp.size.y = (float)Gp_ini->msg_back.window_size.cy - Gp_ini->msg_back.filter_margin.top - Gp_ini->msg_back.filter_margin.bottom;
		m_spr_filter.rp.filter.color = Gp_ini->msg_back.filter_color;	// フィルタの色
	}

	// セパレータ
	for (int i = 0; i < (int)m_separator_list.size(); i++)
	{
		m_separator_list[i].sprite.rpex.sorter = S_tnm_sorter(Gp_ini->msg_back_order, Gp_ini->mwnd.waku_layer_rep);
		m_separator_list[i].sprite.rp.disp = true;
		m_separator_list[i].sprite.rp.d2_rect = true;
		m_separator_list[i].sprite.rp.alpha_test = true;
		m_separator_list[i].sprite.rp.alpha_blend = true;
		m_separator_list[i].sprite.rp.pos = tnm_d2to3(C_point(m_cur.window_rect.left, m_cur.window_rect.top + m_separator_list[i].total_pos + m_scroll_pos));
		m_separator_list[i].sprite.rp.size_fit_to_texture = 0;

		C_rect clip = C_rect(C_point(m_cur.window_rect.left_top() + m_cur.disp_margin.left_top()), C_point(m_cur.window_rect.right_bottom() - m_cur.disp_margin.right_bottom()));
		m_separator_list[i].sprite.rp.dst_clip_use = true;
		m_separator_list[i].sprite.rp.dst_clip = clip;
	}

	// メッセージのフレーム処理
	if (m_history_cnt > 0)
	{
		// 各メッセージに関して判定
		for (int i = 0; i < m_history_cnt; i++)
		{
			int history_pos = (m_history_start_pos + i) % m_history_cnt_max;

			int pos_y = m_history_list[history_pos].total_pos;
			C_rect clip = C_rect(C_point(m_cur.window_rect.left_top() + m_cur.disp_margin.left_top()), C_point(m_cur.window_rect.right_bottom() - m_cur.disp_margin.right_bottom()));

			// メッセージがテキスト矩形内にあるか？
			bool is_in_rect = false;
			if (Gp_ini->tateyoko_mode == 1)
			{
				if (
					pos_y - m_scroll_pos < m_cur.window_rect.width() - m_cur.disp_margin.right &&			// メッセージの左端がテキスト矩形に入っている？
					pos_y - m_scroll_pos + m_history_list[history_pos].height > m_cur.disp_margin.left		// メッセージの右端がテキスト矩形に入っている？
					)
				{
					is_in_rect = true;
				}
			}
			else
			{
				if (
					pos_y + m_scroll_pos + m_history_list[history_pos].height > m_cur.disp_margin.top &&	// メッセージの下端がテキスト矩形に入っている？
					pos_y + m_scroll_pos < m_cur.window_rect.height() - m_cur.disp_margin.bottom			// メッセージの上端がテキスト矩形に入っている？
					)
				{
					is_in_rect = true;
				}
			}
			if (is_in_rect)
			{
				// ターゲットの場合は色を変える
				C_argb color;
				C_argb shadow_color;
				C_argb fuchi_color;
				int color_rate = 0;
				if (Gp_global->debug_flag && history_pos == m_target_no)	{
					color_rate = 255;	// デバッグ時はターゲットを水色にする
					color = Gp_color_table->get_color(Gp_ini->msg_back.debug_moji_color);
					shadow_color = Gp_color_table->get_color(Gp_ini->msg_back.debug_moji_shadow_color);
					fuchi_color = Gp_color_table->get_color(Gp_ini->msg_back.debug_moji_fuchi_color);
				}
				if (history_pos == m_mouse_target_no)	{
					color_rate = 255;	// マウスターゲットを黄色にする
					color = Gp_color_table->get_color(Gp_ini->msg_back.active_moji_color);
					shadow_color = Gp_color_table->get_color(Gp_ini->msg_back.active_moji_shadow_color);
					fuchi_color = Gp_color_table->get_color(Gp_ini->msg_back.active_moji_fuchi_color);
				}

				// メッセージに引き継ぐレンダーパラメータ
				S_tnm_render_param msg_tdp;
				msg_tdp.disp = true;
				msg_tdp.sorter = S_tnm_sorter(Gp_ini->msg_back_order, 0);
				if (Gp_ini->tateyoko_mode == 1)
				{
					msg_tdp.pos_x = (float)m_cur.window_rect.right - pos_y + m_scroll_pos;
					msg_tdp.pos_y = (float)m_cur.window_rect.top + m_cur.msg_pos;
				}
				else
				{
					msg_tdp.pos_x = (float)m_cur.window_rect.left + m_cur.msg_pos;
					msg_tdp.pos_y = (float)m_cur.window_rect.top + pos_y + m_scroll_pos;
				}
				msg_tdp.dst_clip_use = true;
				msg_tdp.dst_clip_left = clip.left;
				msg_tdp.dst_clip_top = clip.top;
				msg_tdp.dst_clip_right = clip.right;
				msg_tdp.dst_clip_bottom = clip.bottom;

				// メッセージのフレーム処理
				m_history_list[history_pos].msg.frame(&msg_tdp, C_point(0, 0), color_rate, color, shadow_color, fuchi_color, draw_futoku, draw_shadow);
			}
			else
			{
				// 矩形内にない場合はテクスチャを解放（グラフィックメモリの節約のため）
				m_history_list[history_pos].msg.clear_texture();
			}

			// ピクチャ（声ボタンを使いまわしてます…？）
			if (m_history_list[history_pos].pct_flag)
			{
				C_point pos = m_cur.window_rect.left_top() + C_point(m_cur.msg_pos, pos_y + m_scroll_pos);
				m_history_list[history_pos].koe_btn.set_disp(1);
				m_history_list[history_pos].koe_btn.set_order(Gp_ini->msg_back_order);
				m_history_list[history_pos].koe_btn.set_pos_x(pos.x + m_history_list[history_pos].pct_pos.x);
				m_history_list[history_pos].koe_btn.set_pos_y(pos.y + m_history_list[history_pos].pct_pos.y);
				m_history_list[history_pos].koe_btn.set_clip_use(1);
				m_history_list[history_pos].koe_btn.set_clip_left(clip.left);
				m_history_list[history_pos].koe_btn.set_clip_top(clip.top);
				m_history_list[history_pos].koe_btn.set_clip_right(clip.right);
				m_history_list[history_pos].koe_btn.set_clip_bottom(clip.bottom);
				m_history_list[history_pos].koe_btn.frame(NULL, 0, 0);
			}

			// 声アイコン
			else if (!m_history_list[history_pos].koe_no_list.empty())
			{
				C_point pos;
				if (Gp_ini->tateyoko_mode == 1)
				{
					pos.x = m_cur.window_rect.right - m_history_list[history_pos].total_pos + m_scroll_pos;
					pos.y = m_cur.window_rect.top + m_cur.msg_pos;
				}
				else
				{
					pos.x = m_cur.window_rect.left + m_cur.msg_pos;
					pos.y = m_cur.window_rect.top + m_history_list[history_pos].total_pos + m_scroll_pos;
				}

				m_history_list[history_pos].koe_btn.set_disp(1);
				m_history_list[history_pos].koe_btn.set_order(Gp_ini->msg_back_order);
				m_history_list[history_pos].koe_btn.set_pos_x(pos.x + m_def.koe_btn_pos.x);
				m_history_list[history_pos].koe_btn.set_pos_y(pos.y + m_def.koe_btn_pos.y);
				m_history_list[history_pos].koe_btn.set_clip_use(1);
				m_history_list[history_pos].koe_btn.set_clip_left(clip.left);
				m_history_list[history_pos].koe_btn.set_clip_top(clip.top);
				m_history_list[history_pos].koe_btn.set_clip_right(clip.right);
				m_history_list[history_pos].koe_btn.set_clip_bottom(clip.bottom);
				m_history_list[history_pos].koe_btn.frame(NULL, 0, 0);
			}

			// ロードボタン
			if (m_history_list[history_pos].save_id != S_tid())
			{
				if (tnm_check_backlog_save(m_history_list[history_pos].save_id))
				{
					C_point pos;
					if (Gp_ini->tateyoko_mode == 1)
					{
						pos.x = m_cur.window_rect.right - m_history_list[history_pos].total_pos + m_scroll_pos;
						pos.y = m_cur.window_rect.top + m_cur.msg_pos;
					}
					else
					{
						pos.x = m_cur.window_rect.left + m_cur.msg_pos;
						pos.y = m_cur.window_rect.top + m_history_list[history_pos].total_pos + m_scroll_pos;
					}

					m_history_list[history_pos].load_btn.set_disp(1);
					m_history_list[history_pos].load_btn.set_order(Gp_ini->msg_back_order);
					m_history_list[history_pos].load_btn.set_pos_x(pos.x + m_def.load_btn_pos.x);
					m_history_list[history_pos].load_btn.set_pos_y(pos.y + m_def.load_btn_pos.y);
					m_history_list[history_pos].load_btn.set_clip_use(1);
					m_history_list[history_pos].load_btn.set_clip_left(clip.left);
					m_history_list[history_pos].load_btn.set_clip_top(clip.top);
					m_history_list[history_pos].load_btn.set_clip_right(clip.right);
					m_history_list[history_pos].load_btn.set_clip_bottom(clip.bottom);
					m_history_list[history_pos].load_btn.frame(NULL, 0, 0);
				}
			}
		}
	}

	// スライダー
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_slider.set_pos_x(m_cur.window_rect.left + m_slider_pos);
		m_slider.set_pos_y(m_cur.window_rect.top + Gp_ini->msg_back.slider_rect.top);
	}
	else
	{
		m_slider.set_pos_x(m_cur.window_rect.left + Gp_ini->msg_back.slider_rect.left);
		m_slider.set_pos_y(m_cur.window_rect.top + m_slider_pos);
	}
	m_slider.frame(NULL, 0, 0);

	// ボタン
	m_close_btn.frame(NULL, 0, 0);
	m_msg_up_btn.frame(NULL, 0, 0);
	m_msg_down_btn.frame(NULL, 0, 0);
	m_ex_btn_1.frame(NULL, 0, 0);
	m_ex_btn_2.frame(NULL, 0, 0);
	m_ex_btn_3.frame(NULL, 0, 0);
	m_ex_btn_4.frame(NULL, 0, 0);
}

// ****************************************************************
// ボタンを登録
// ================================================================
void C_elm_msg_back::regist_button(int min_order)
{
	// ウィンドウが非表示なら戻る
	if (!m_window_appear)
		return;

	// メッセージバックがアクティブでないなら戻る
	if (Gp_local->cur_proc.type != TNM_PROC_TYPE_MSG_BACK)
		return;

	// 声アイコン
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].koe_btn.regist_button(min_order);

	// ロードアイコン
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].load_btn.regist_button(min_order);

	// スライダー
	m_slider.regist_button(min_order);

	// ボタン
	m_close_btn.regist_button(min_order);
	m_msg_up_btn.regist_button(min_order);
	m_msg_down_btn.regist_button(min_order);
	m_ex_btn_1.regist_button(min_order);
	m_ex_btn_2.regist_button(min_order);
	m_ex_btn_3.regist_button(min_order);
	m_ex_btn_4.regist_button(min_order);
}

// ****************************************************************
// ボタンイベント
// ================================================================

#define		TNM_MSG_BACK_AFTEREFFECTS_ALPHA		10

void C_elm_msg_back::msg_click_event()
{
	if (m_mouse_target_no >= 0)
	{
		S_history* p_hisotry = &m_history_list[m_mouse_target_no];

		if (Gp_ini->msg_back.msg_click_action == 1)
		{
			if (!p_hisotry->koe_no_list.empty())
			{
				// 再生する声番号を補正する
				if (p_hisotry->koe_play_no < 0 || (int)p_hisotry->koe_no_list.size() <= p_hisotry->koe_play_no)
					p_hisotry->koe_play_no = 0;

				// 声を再生（時短再生有効）
				int koe_no = p_hisotry->koe_no_list[p_hisotry->koe_play_no];
				int chr_no = p_hisotry->chr_no_list[p_hisotry->koe_play_no];
				tnm_play_koe(koe_no, chr_no, Gp_config->jitan_msgbk_onoff ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL, 0, false, false, false, true, false);

				// 次の声に進める（複数の声がある場合があります）
				p_hisotry->koe_play_no++;
			}
		}
		else if (Gp_ini->msg_back.msg_click_action == 2)
		{
			if (tnm_check_backlog_save(p_hisotry->save_id))
			{
				// ロードを行う
				load_call(p_hisotry->save_id);
			}
		}
	}
}

void C_elm_msg_back::button_event()
{
	// ウィンドウが非表示なら戻る
	if (!m_window_appear)
		return;

	// メッセージバックがアクティブでないなら戻る
	if (Gp_local->cur_proc.type != TNM_PROC_TYPE_MSG_BACK)
		return;

	// 声アイコン
	for (int i = 0; i < m_history_cnt_max; i++)
	{
		S_history* p_hisotry = &m_history_list[i];

		// デフォルトのボタンイベント
		p_hisotry->koe_btn.button_event();

		// 声アイコンの決定処理
		if (p_hisotry->koe_btn.is_decided_this_frame())
		{
			if (!p_hisotry->koe_no_list.empty())
			{
				// 再生する声番号を補正する
				if (p_hisotry->koe_play_no < 0 || (int)p_hisotry->koe_no_list.size() <= p_hisotry->koe_play_no)
					p_hisotry->koe_play_no = 0;

				// 声を再生（時短再生有効）
				int koe_no = p_hisotry->koe_no_list[p_hisotry->koe_play_no];
				int chr_no = p_hisotry->chr_no_list[p_hisotry->koe_play_no];
				tnm_play_koe(koe_no, chr_no, Gp_config->jitan_msgbk_onoff ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL, 0, false, false, false, true, false);

				// 次の声に進める（複数の声がある場合があります）
				p_hisotry->koe_play_no ++;
			}
		}
	}

	// ロードボタン
	for (int i = 0; i < m_history_cnt_max; i++)
	{
		S_history* p_hisotry = &m_history_list[i];

		if (p_hisotry->save_id != S_tid())
		{
			// デフォルトのボタンイベント
			p_hisotry->load_btn.button_event();

			// ロードボタンの決定処理
			if (p_hisotry->load_btn.is_decided_this_frame())
			{
				// ロードを行う
				load_call(p_hisotry->save_id);
			}
		}
	}

	// スライダー
	m_slider.button_event();

	// スライダーの決定処理
	if (m_slider.is_pushed_this_frame())
	{
		m_slider_operating = true;
		m_slider_start_slider_pos = m_slider_pos;
		if (Gp_ini->tateyoko_mode == 1)
		{
			m_slider_start_mouse_pos = Gp_cur_input->mouse.pos.x;
		}
		else
		{
			m_slider_start_mouse_pos = Gp_cur_input->mouse.pos.y;
		}
	}
	if (m_slider.is_decided_this_frame())
	{
		m_slider_operating = false;

		// スクロールからスライダー位置を更新（スライダーを放した直後に一度だけスライダー位置を調整する：バックログが１つしかなかった場合にスライダーの位置を下限に配置する為の補正処理です）
		calc_slider_pos_from_scroll();
	}
	if (!m_slider.is_pushed())
	{
		m_slider_operating = false;
	}

	// スライダーを動かした処理
	if (m_slider_operating && m_history_cnt > 0)
	{
		// スライダーの位置を求める
		if (Gp_ini->tateyoko_mode == 1)
		{
			m_slider_pos = m_slider_start_slider_pos + (Gp_cur_input->mouse.pos.x - m_slider_start_mouse_pos);
			m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.left, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_x(0));
		}
		else
		{
			m_slider_pos = m_slider_start_slider_pos + (Gp_cur_input->mouse.pos.y - m_slider_start_mouse_pos);
			m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.top, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0));
		}

		// スライダー位置を更新する
		update_pos_from_slider();
	}

	// 閉じるボタン
	m_close_btn.button_event();

	// 閉じるボタンの決定処理
	if (m_close_btn.is_decided_this_frame())
	{
		// メッセージバックを閉じる
		tnm_syscom_close_msg_back();
	}

	// メッセージアップボタン
	m_msg_up_btn.button_event();

	// メッセージアップボタンの決定処理
	if (m_page_up_key || m_msg_up_btn.is_pushed())
	{
		target_up();
	}
	else
	{
		m_target_up_start_time = 0;
	}
	m_page_up_key = false;

	// メッセージダウンボタン
	m_msg_down_btn.button_event();

	// メッセージダウンボタンの決定処理
	if (m_page_dw_key || m_msg_down_btn.is_pushed())
	{
		target_down();
	}
	else
	{
		m_target_dw_start_time = 0;
	}
	m_page_dw_key = false;

	// 汎用ボタン１
	m_ex_btn_1.button_event();

	// 汎用ボタン１の決定処理
	if (m_ex_btn_1.is_decided_this_frame())
	{
		ex_btn_call(1);
	}

	// 汎用ボタン２
	m_ex_btn_2.button_event();

	// 汎用ボタン２の決定処理
	if (m_ex_btn_2.is_decided_this_frame())
	{
		ex_btn_call(2);
	}

	// 汎用ボタン３
	m_ex_btn_3.button_event();

	// 汎用ボタン３の決定処理
	if (m_ex_btn_3.is_decided_this_frame())
	{
		ex_btn_call(3);
	}

	// 汎用ボタン４
	m_ex_btn_4.button_event();

	// 汎用ボタン４の決定処理
	if (m_ex_btn_4.is_decided_this_frame())
	{
		ex_btn_call(4);
	}

	// ドラッグ準備
	if (Gp_input->now.mouse.left.use_down_stock())
	{
		// ドラッグ準備開始
		m_drag_ready_flag = true;
		m_drag_start_mouse_pos = Gp_cur_input->mouse.pos;
		m_drag_start_scroll_pos = m_scroll_pos;

		// ドラッグの余波
		m_drag_aftereffects_flag = false;
		m_drag_aftereffects_start_time = ::timeGetTime();
		if (Gp_ini->tateyoko_mode == 1)
		{
			m_drag_aftereffects_mouse_pos = Gp_cur_input->mouse.pos.x;
		}
		else
		{
			m_drag_aftereffects_mouse_pos = Gp_cur_input->mouse.pos.y;
		}
		m_drag_aftereffects_distance = 0;
	}

	// 少しでも動けばドラッグ開始
	if (m_drag_ready_flag && m_drag_start_mouse_pos != Gp_cur_input->mouse.pos)
	{
		m_drag_flag = true;
	}

	// ドラッグ終了
	if (Gp_input->now.mouse.left.use_up_stock())
	{
		// ドラッグ準備だけしててドラッグしていない場合、声を再生する
		if (m_drag_ready_flag && !m_drag_flag && m_mouse_target_no >= 0)
		{
			msg_click_event();
		}

		// ドラッグの余波
		if (m_drag_flag && m_drag_aftereffects_distance != 0)
		{
			m_drag_aftereffects_flag = true;						// 余波が発生しました。
			int power = m_drag_aftereffects_distance * TNM_MSG_BACK_AFTEREFFECTS_ALPHA;			// * TNM_MSG_BACK_AFTEREFFECTS_ALPHA = スクロールする距離に関わる係数
			m_drag_aftereffects_start_scroll_pos = m_scroll_pos;
			m_drag_aftereffects_end_scroll_pos = m_scroll_pos + power;
			m_drag_aftereffects_start_time = ::timeGetTime();
		}

		// ドラッグ終了
		m_drag_ready_flag = false;
		m_drag_flag = false;
	}

	// 決定キー
	if (tnm_input_use_key_down_up(VK_EX_KEYBOARD_DECIDE))
	{
		// 声を再生する
		if (m_mouse_target_no >= 0)	{
			msg_click_event();
		}
	}

	// ドラッグ中
	if (m_drag_flag)
	{
		// ドラッグの余波
		DWORD now_time = ::timeGetTime();
		int keika = now_time - m_drag_aftereffects_start_time;
		if (keika >= 25)	{			// 25㍉秒単位で余波を計測しています。
			m_drag_aftereffects_start_time = now_time;
			if (Gp_ini->tateyoko_mode == 1)
			{
				m_drag_aftereffects_distance = Gp_cur_input->mouse.pos.x - m_drag_aftereffects_mouse_pos;
				m_drag_aftereffects_mouse_pos = Gp_cur_input->mouse.pos.x;
			}
			else
			{
				m_drag_aftereffects_distance = Gp_cur_input->mouse.pos.y - m_drag_aftereffects_mouse_pos;
				m_drag_aftereffects_mouse_pos = Gp_cur_input->mouse.pos.y;
			}
		}

		// スクロール
		if (Gp_ini->tateyoko_mode == 1)
		{
			m_scroll_pos = m_drag_start_scroll_pos - (m_drag_start_mouse_pos.x - Gp_cur_input->mouse.pos.x);
		}
		else
		{
			m_scroll_pos = m_drag_start_scroll_pos - (m_drag_start_mouse_pos.y - Gp_cur_input->mouse.pos.y);
		}

		// スクロールから位置を更新する
		update_pos_from_scroll();
	}

	// ドラッグの余波の中断（マウスが押されました）
	if (Gp_input->now.mouse.left.is_down() || Gp_input->now.mouse.right.is_down() || Gp_input->now.keyboard.key[VK_PRIOR].is_down() || Gp_input->now.keyboard.key[VK_NEXT].is_down() || Gp_input->now.mouse.wheel.use_up_stock() || Gp_input->now.mouse.wheel.use_down_stock())
	{
		m_drag_aftereffects_flag = false;
	}

	// ドラッグの余波
	if (m_drag_aftereffects_flag)
	{
		DWORD now_time = ::timeGetTime();
		int keika = now_time - m_drag_aftereffects_start_time;

		// スクロール（175 * TNM_MSG_BACK_AFTEREFFECTS_ALPHA ㍉秒でスクロールします）
		m_scroll_pos = speed_down_limit(keika, 0, m_drag_aftereffects_start_scroll_pos, 175 * TNM_MSG_BACK_AFTEREFFECTS_ALPHA, m_drag_aftereffects_end_scroll_pos);

		// スクロールから位置を更新する
		update_pos_from_scroll();

		// スクロール位置まで到達したので処理完了！
		if (m_scroll_pos == m_drag_aftereffects_end_scroll_pos)	{
			m_drag_aftereffects_flag = false;
		}
	}

	// マウスターゲット
	m_mouse_target_no = -1;

	if (Gp_ini->msg_back.msg_click_action > 0)
	{
		if (!m_drag_flag)
		{
			for (int i = 0; i < m_history_cnt; i++)
			{
				// ターゲットになるかどうかの判定
				bool is_target_enable = false;
				if (Gp_ini->msg_back.msg_click_action == 1)			// 声を再生する場合
				{
					// 声が登録されている場合のみターゲットになる
					if (!m_history_list[i].koe_no_list.empty())
					{
						is_target_enable = true;
					}
				}
				else if (Gp_ini->msg_back.msg_click_action == 2)	// ロードを行う場合
				{
					// かならずターゲットになる
					is_target_enable = true;
				}

				// ターゲットになる場合は当たり判定
				if (is_target_enable)
				{
					if (Gp_ini->tateyoko_mode == 1)
					{
						int right = m_cur.window_rect.right - m_history_list[i].total_pos + m_scroll_pos;
						int left = right - m_history_list[i].height;
						if (left <= Gp_cur_input->mouse.pos.x && Gp_cur_input->mouse.pos.x < right)
						{
							m_mouse_target_no = i;
							break;
						}
					}
					else
					{
						int top = m_cur.window_rect.top + m_history_list[i].total_pos + m_scroll_pos;
						int bottom = top + m_history_list[i].height;
						if (top <= Gp_cur_input->mouse.pos.y && Gp_cur_input->mouse.pos.y < bottom)
						{
							m_mouse_target_no = i;
							break;
						}
					}
				}
			}
		}
	}
}

// ****************************************************************
// ロードを行う
// ================================================================
void C_elm_msg_back::load_call(S_tid save_id)
{
	if (tnm_check_backlog_save(save_id))
	{
		if (!Gp_ini->msg_back.load_call_scene_name.empty())
		{
			int scene_no = Gp_lexer->get_scn_no(Gp_ini->msg_back.load_call_scene_name);
			int command_no = Gp_lexer->get_user_cmd_no(scene_no, Gp_ini->msg_back.load_call_command_name);
			if (scene_no < 0)
			{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("指定されたシーンが見つかりません。"));
			}
			else if (command_no < 0)
			{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("指定されたコマンドが見つかりません。"));
			}
			else
			{
				Gp_global->msg_back_load_tid = save_id;		// スクリプトから呼び出せるように ID を記憶しておく

				// 指定のコマンドをコール
				tnm_scene_proc_call_user_cmd(scene_no, command_no, FM_VOID, true, false);
			}
		}
		else
		{
			tnm_syscom_msgbk_load(save_id, true, true, true);
		}
	}
}

// ****************************************************************
// 汎用ボタン
// ================================================================
void C_elm_msg_back::ex_btn_call(int btn_no)
{
	TSTR scene_name;
	TSTR command_name;

	switch (btn_no)
	{
	case 1:
		scene_name = Gp_ini->msg_back.ex_btn_1_scene_name;
		command_name = Gp_ini->msg_back.ex_btn_1_command_name;
		break;
	case 2:
		scene_name = Gp_ini->msg_back.ex_btn_2_scene_name;
		command_name = Gp_ini->msg_back.ex_btn_2_command_name;
		break;
	case 3:
		scene_name = Gp_ini->msg_back.ex_btn_3_scene_name;
		command_name = Gp_ini->msg_back.ex_btn_3_command_name;
		break;
	case 4:
		scene_name = Gp_ini->msg_back.ex_btn_4_scene_name;
		command_name = Gp_ini->msg_back.ex_btn_4_command_name;
		break;
	}

	if (!scene_name.empty())
	{
		int scene_no = Gp_lexer->get_scn_no(scene_name);
		int command_no = Gp_lexer->get_user_cmd_no(scene_no, command_name);
		if (scene_no < 0)
		{
			tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("指定されたシーンが見つかりません。"));
		}
		else if (command_no < 0)
		{
			tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("指定されたコマンドが見つかりません。"));
		}
		else
		{
			// 指定のコマンドをコール
			tnm_scene_proc_call_user_cmd(scene_no, command_no, FM_VOID, true, false);
		}
	}
}

// ****************************************************************
// スクロールの余波が発生しているかどうかを判定する
// ================================================================
bool C_elm_msg_back::check_aftereffects()
{
	return m_drag_aftereffects_flag;
}

// ****************************************************************
// スプライトツリーを構築
// ================================================================
void C_elm_msg_back::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	// ウィンドウが非表示なら戻る
	if (!m_window_appear)
		return;

	// 背景
	root.add_child(&m_spr_waku);
	root.add_child(&m_spr_filter);

	// セパレーター
	for (int i = 0; i < (int)m_separator_list.size(); i++)
		root.add_child(&m_separator_list[i].sprite);

	// テキスト
	for (int i = 0; i < (int)m_history_list.size(); i++)
		m_history_list[i].msg.get_sprite_tree(root);

	// 声アイコン
	for (int i = 0; i < (int)m_history_list.size(); i++)
		m_history_list[i].koe_btn.get_sprite_tree(root);

	// ロードボタン
	for (int i = 0; i < (int)m_history_list.size(); i++)
		m_history_list[i].load_btn.get_sprite_tree(root);

	// スライダー
	m_slider.get_sprite_tree(root);

	// ボタン
	m_close_btn.get_sprite_tree(root);
	m_msg_up_btn.get_sprite_tree(root);
	m_msg_down_btn.get_sprite_tree(root);
	m_ex_btn_1.get_sprite_tree(root);
	m_ex_btn_2.get_sprite_tree(root);
	m_ex_btn_3.get_sprite_tree(root);
	m_ex_btn_4.get_sprite_tree(root);
}

// ****************************************************************
// 開く
// ================================================================
void C_elm_msg_back::open()
{
	m_window_appear = true;

	// スプライトを初期化
	m_spr_waku.init();
	m_spr_filter.init();
	m_separator_list.clear();

	// ウィンドウ関連の情報
	TSTR waku_file_name = Gp_ini->msg_back.waku_file;

	// ファイル名の指定がある場合、ファイルを読み込む
	if (!waku_file_name.empty())
	{
		// ウィンドウのスプライトを作成
		m_spr_waku.create_polygon_no_init();
		m_spr_waku.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_spr_waku.set_index_buffer(G_rect_index_buffer);

		// テクスチャを読み込む
		m_album_waku = tnm_load_pct_d3d(waku_file_name, false, false);
		if (m_album_waku)
			m_spr_waku.set_texture(m_album_waku->get_texture(0));
	}
	else
	{
		// ウィンドウのスプライトを作成
		m_spr_waku.create_polygon_no_init();
		m_spr_waku.set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
		m_spr_waku.set_index_buffer(G_rect_index_buffer);
	}

	// フィルター関連の情報
	TSTR filter_file_name = Gp_ini->msg_back.filter_file;
	C_rect filter_margin = Gp_ini->msg_back.filter_margin;
	C_argb filter_color = Gp_ini->msg_back.filter_color;

	// ファイル名の指定がある場合、ファイルを読み込む
	if (!filter_file_name.empty())
	{
		// フィルターのスプライトを作成
		m_spr_filter.create_polygon_no_init();
		m_spr_filter.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_spr_filter.set_index_buffer(G_rect_index_buffer);

		// テクスチャを読み込む
		TSTR filter_file_path = filter_file_name;
		m_album_filter = tnm_load_pct_d3d(filter_file_path, false, false);
		if (m_album_filter)
			m_spr_filter.set_texture(m_album_filter->get_texture(0));
	}
	else
	{
		// フィルターのスプライトを作成
		m_spr_filter.create_polygon_no_init();
		m_spr_filter.set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
		m_spr_filter.set_index_buffer(G_rect_index_buffer);
	}

	// メッセージ
	for (int i = 0; i < m_history_cnt_max; i++)
	{
		m_history_list[i].msg.set_def_param(Gp_ini->msg_back.moji_cnt
			, Gp_ini->msg_back.moji_size, Gp_ini->msg_back.moji_space, Gp_ini->mwnd.moji_color, Gp_ini->mwnd.shadow_color, Gp_ini->mwnd.fuchi_color, 0, 0
			, Gp_ini->msg_back.moji_size, 0, 0, C_rect(0, 0, 0, 0));

		// 声アイコン
		m_history_list[i].koe_btn.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());

		// ロードボタン
		m_history_list[i].load_btn.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	}

	// スライダー
	m_slider.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.slider_file.empty())
	{
		m_slider.create_pct(Gp_ini->msg_back.slider_file);
		m_slider.set_order(Gp_ini->msg_back_order);
		m_slider.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_slider.set_disp(1);
		m_slider.set_button_param(0, 0, Gp_ini->msg_back.slider_action_no, Gp_ini->msg_back.slider_se_no);
		m_slider.set_button_push_keep(1);
	}

	// 閉じるボタン
	m_close_btn.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.close_btn_file.empty())
	{
		m_close_btn.create_pct(Gp_ini->msg_back.close_btn_file);
		m_close_btn.set_order(Gp_ini->msg_back_order);
		m_close_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_close_btn.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.close_btn_pos.x);
		m_close_btn.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.close_btn_pos.y);
		m_close_btn.set_disp(1);
		m_close_btn.set_button_param(0, 0, Gp_ini->msg_back.close_btn_action_no, Gp_ini->msg_back.close_btn_se_no);
	}

	// メッセージアップボタン
	m_msg_up_btn.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.msg_up_btn_file.empty())
	{
		m_msg_up_btn.create_pct(Gp_ini->msg_back.msg_up_btn_file);
		m_msg_up_btn.set_order(Gp_ini->msg_back_order);
		m_msg_up_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_msg_up_btn.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.msg_up_btn_pos.x);
		m_msg_up_btn.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.msg_up_btn_pos.y);
		m_msg_up_btn.set_disp(1);
		m_msg_up_btn.set_button_param(0, 0, Gp_ini->msg_back.msg_up_btn_action_no, Gp_ini->msg_back.msg_up_btn_se_no);
	}

	// メッセージダウンボタン
	m_msg_down_btn.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.msg_down_btn_file.empty())
	{
		m_msg_down_btn.create_pct(Gp_ini->msg_back.msg_down_btn_file);
		m_msg_down_btn.set_order(Gp_ini->msg_back_order);
		m_msg_down_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_msg_down_btn.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.msg_down_btn_pos.x);
		m_msg_down_btn.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.msg_down_btn_pos.y);
		m_msg_down_btn.set_disp(1);
		m_msg_down_btn.set_button_param(0, 0, Gp_ini->msg_back.msg_down_btn_action_no, Gp_ini->msg_back.msg_down_btn_se_no);
	}

	// 汎用ボタン１
	m_ex_btn_1.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.ex_btn_1_file.empty())
	{
		m_ex_btn_1.create_pct(Gp_ini->msg_back.ex_btn_1_file);
		m_ex_btn_1.set_order(Gp_ini->msg_back_order);
		m_ex_btn_1.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_ex_btn_1.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.ex_btn_1_pos.x);
		m_ex_btn_1.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.ex_btn_1_pos.y);
		m_ex_btn_1.set_disp(1);
		m_ex_btn_1.set_button_param(0, 0, Gp_ini->msg_back.ex_btn_1_action_no, Gp_ini->msg_back.ex_btn_1_se_no);
	}

	// 汎用ボタン２
	m_ex_btn_2.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.ex_btn_2_file.empty())
	{
		m_ex_btn_2.create_pct(Gp_ini->msg_back.ex_btn_2_file);
		m_ex_btn_2.set_order(Gp_ini->msg_back_order);
		m_ex_btn_2.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_ex_btn_2.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.ex_btn_2_pos.x);
		m_ex_btn_2.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.ex_btn_2_pos.y);
		m_ex_btn_2.set_disp(1);
		m_ex_btn_2.set_button_param(0, 0, Gp_ini->msg_back.ex_btn_2_action_no, Gp_ini->msg_back.ex_btn_2_se_no);
	}

	// 汎用ボタン３
	m_ex_btn_3.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.ex_btn_3_file.empty())
	{
		m_ex_btn_3.create_pct(Gp_ini->msg_back.ex_btn_3_file);
		m_ex_btn_3.set_order(Gp_ini->msg_back_order);
		m_ex_btn_3.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_ex_btn_3.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.ex_btn_3_pos.x);
		m_ex_btn_3.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.ex_btn_3_pos.y);
		m_ex_btn_3.set_disp(1);
		m_ex_btn_3.set_button_param(0, 0, Gp_ini->msg_back.ex_btn_3_action_no, Gp_ini->msg_back.ex_btn_3_se_no);
	}

	// 汎用ボタン４
	m_ex_btn_4.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.ex_btn_4_file.empty())
	{
		m_ex_btn_4.create_pct(Gp_ini->msg_back.ex_btn_4_file);
		m_ex_btn_4.set_order(Gp_ini->msg_back_order);
		m_ex_btn_4.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_ex_btn_4.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.ex_btn_4_pos.x);
		m_ex_btn_4.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.ex_btn_4_pos.y);
		m_ex_btn_4.set_disp(1);
		m_ex_btn_4.set_button_param(0, 0, Gp_ini->msg_back.ex_btn_4_action_no, Gp_ini->msg_back.ex_btn_4_se_no);
	}

	// セパレーター
	if (!Gp_ini->msg_back.separator_file.empty())
	{
		// テクスチャを読み込む
		m_album_separator = tnm_load_pct_d3d(Gp_ini->msg_back.separator_file, false, false);
	}
	if (!Gp_ini->msg_back.separator_top_file.empty())
	{
		// テクスチャを読み込む
		m_album_separator_top = tnm_load_pct_d3d(Gp_ini->msg_back.separator_top_file, false, false);
	}
	if (!Gp_ini->msg_back.separator_bottom_file.empty())
	{
		// テクスチャを読み込む
		m_album_separator_bottom = tnm_load_pct_d3d(Gp_ini->msg_back.separator_bottom_file, false, false);
	}

	// メッセージを作成する
	create_msg();

	// 初期のスライダー位置を決める
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_slider_pos = (int)Gp_ini->msg_back.slider_rect.left;
	}
	else
	{
		m_slider_pos = (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0);
	}

	// スライダーから位置を計算する
	update_pos_from_slider();

	// 改めてスクロールからスライダー位置を計算（メッセージが１つのときにスライダーの位置を調整するため）
	update_pos_from_scroll();

	// ドラッグの余波を初期化する
	m_drag_ready_flag = false;
	m_drag_flag = false;
	m_drag_aftereffects_flag = false;

	// PageUp / PageDown キーが押された事を初期化する
	m_page_up_key = false;
	m_page_dw_key = false;

	// PageUp / PageDown のリピートタイマーを初期化する
	m_target_up_start_time = 0;
	m_target_dw_start_time = 0;

	// ターゲットを決定する
	m_target_no = m_history_last_pos;
}

// ****************************************************************
// 閉じる
// ================================================================
void C_elm_msg_back::close()
{
	m_window_appear = false;

	// メッセージをクリアする
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].msg.clear_msg();

	// 声ボタン
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].koe_btn.reinit(true);

	// ロードボタン
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].load_btn.reinit(true);

	// 各種ボタン
	m_slider.reinit(true);
	m_close_btn.reinit(true);
	m_msg_up_btn.reinit(true);
	m_msg_down_btn.reinit(true);
	m_ex_btn_1.reinit(true);
	m_ex_btn_2.reinit(true);
	m_ex_btn_3.reinit(true);
	m_ex_btn_4.reinit(true);

	// セパレーター
	m_separator_list.clear();

	// アルバムをクリア
	m_album_waku.reset();
	m_album_filter.reset();
	m_album_separator.reset();
	m_album_separator_top.reset();
	m_album_separator_bottom.reset();

	// スプライトをクリア
	m_spr_waku.init();
	m_spr_filter.init();
}

// ****************************************************************
// メッセージを追加準備
// ================================================================
void C_elm_msg_back::ready_msg()
{
	// メッセージを作成する

	// まだメッセージがあふれていない場合
	if (m_history_cnt < m_history_cnt_max)
	{
		// 新規メッセージの場合はカウントを増やす
		if (m_new_msg_flag)
		{
			m_new_msg_flag = false;
			m_history_cnt ++;

			// この位置のメッセージをクリアする
			m_history_list[m_history_insert_pos].save_id = S_tid();
			m_history_list[m_history_insert_pos].save_id_check_flag = false;
			m_history_list[m_history_insert_pos].pct_flag = false;
			m_history_list[m_history_insert_pos].original_name.clear();
			m_history_list[m_history_insert_pos].disp_name.clear();
			m_history_list[m_history_insert_pos].msg_str.clear();
			m_history_list[m_history_insert_pos].debug_msg.clear();
			m_history_list[m_history_insert_pos].scn_no = -1;
			m_history_list[m_history_insert_pos].line_no = -1;
			m_history_list[m_history_insert_pos].koe_no_list.clear();
			m_history_list[m_history_insert_pos].chr_no_list.clear();
			m_history_list[m_history_insert_pos].koe_play_no = 0;
		}
	}
	// メッセージがあふれている場合
	else
	{
		// 新規メッセージの場合はスタート位置をずらす
		if (m_new_msg_flag)
		{
			m_new_msg_flag = false;
			m_history_start_pos = (m_history_start_pos + 1) % m_history_cnt_max;

			// この位置のメッセージをクリアする
			m_history_list[m_history_insert_pos].save_id = S_tid();
			m_history_list[m_history_insert_pos].save_id_check_flag = false;
			m_history_list[m_history_insert_pos].pct_flag = false;
			m_history_list[m_history_insert_pos].original_name.clear();
			m_history_list[m_history_insert_pos].disp_name.clear();
			m_history_list[m_history_insert_pos].msg_str.clear();
			m_history_list[m_history_insert_pos].debug_msg.clear();
			m_history_list[m_history_insert_pos].scn_no = -1;
			m_history_list[m_history_insert_pos].line_no = -1;
			m_history_list[m_history_insert_pos].koe_no_list.clear();
			m_history_list[m_history_insert_pos].chr_no_list.clear();
			m_history_list[m_history_insert_pos].koe_play_no = 0;
		}
	}
}

// ****************************************************************
// セーブ ID を設定
// ================================================================
void C_elm_msg_back::set_save_id(S_tid save_id)
{
	if (m_history_insert_pos < 0 || m_history_cnt <= m_history_insert_pos)
		return;

	m_history_list[m_history_insert_pos].save_id = save_id;
}

void C_elm_msg_back::set_save_id_check_flag(bool save_id_check_flag)
{
	if (m_history_insert_pos < 0 || m_history_cnt <= m_history_insert_pos)
		return;

	m_history_list[m_history_insert_pos].save_id_check_flag = save_id_check_flag;
}

bool C_elm_msg_back::get_save_id_check_flag()
{
	if (m_history_insert_pos < 0 || m_history_cnt <= m_history_insert_pos)
		return false;

	return m_history_list[m_history_insert_pos].save_id_check_flag;
}

// ****************************************************************
// 声を追加
// ================================================================
bool C_elm_msg_back::add_koe(int koe_no, int chr_no, int scn_no, int line_no)
{
	if (koe_no < 0)
		return true;

	// メッセージを追加準備する
	ready_msg();

	// メッセージを追加する
	m_history_list[m_history_insert_pos].koe_no_list.push_back(koe_no);
	m_history_list[m_history_insert_pos].chr_no_list.push_back(chr_no);
	m_history_list[m_history_insert_pos].scn_no = scn_no;
	m_history_list[m_history_insert_pos].line_no = line_no;

	// ここがメッセージの最後
	m_history_last_pos = m_history_insert_pos;

	return true;
}

// ****************************************************************
// 名前を追加
// ================================================================
bool C_elm_msg_back::add_name(CTSTR& original_name, CTSTR& disp_name, int scn_no, int line_no)
{
	if (disp_name.empty())
		return true;

	// メッセージを追加準備する
	ready_msg();

	// メッセージを追加する
	m_history_list[m_history_insert_pos].original_name = original_name;
	m_history_list[m_history_insert_pos].disp_name = disp_name;
	m_history_list[m_history_insert_pos].scn_no = scn_no;
	m_history_list[m_history_insert_pos].line_no = line_no;

	// ここがメッセージの最後
	m_history_last_pos = m_history_insert_pos;

	return true;
}

// ****************************************************************
// メッセージを追加
// ================================================================
bool C_elm_msg_back::add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no)
{
	if (msg.empty())
		return true;

	// メッセージを追加準備する
	ready_msg();

	// メッセージを追加する
	m_history_list[m_history_insert_pos].msg_str += msg;
	m_history_list[m_history_insert_pos].debug_msg = debug_msg;
	m_history_list[m_history_insert_pos].scn_no = scn_no;
	m_history_list[m_history_insert_pos].line_no = line_no;

	// ここがメッセージの最後
	m_history_last_pos = m_history_insert_pos;

	return true;
}

// ****************************************************************
// 改行を追加
// ================================================================
bool C_elm_msg_back::add_new_line_indent(int scn_no, int line_no)
{
	// メッセージを追加準備する
	ready_msg();

	// メッセージを追加する（デバッグメッセージには改行は入れないことに注意）
	m_history_list[m_history_insert_pos].msg_str += _T("\n");
	m_history_list[m_history_insert_pos].scn_no = scn_no;
	m_history_list[m_history_insert_pos].line_no = line_no;

	// ここがメッセージの最後
	m_history_last_pos = m_history_insert_pos;

	return true;
}
bool C_elm_msg_back::add_new_line_no_indent(int scn_no, int line_no)
{
	// メッセージを追加準備する
	ready_msg();

	// メッセージを追加する
	m_history_list[m_history_insert_pos].msg_str += _T("\a");
	//m_history_list[m_history_insert_pos].debug_msg = debug_msg;	// デバッグメッセージには改行は入れない
	m_history_list[m_history_insert_pos].scn_no = scn_no;
	m_history_list[m_history_insert_pos].line_no = line_no;

	// ここがメッセージの最後
	m_history_last_pos = m_history_insert_pos;

	return true;
}

// ****************************************************************
// 画像を追加
// ================================================================
bool C_elm_msg_back::add_pct(CTSTR& file_name, C_point pos)
{
	// ファイルが存在するかを判定する
	if (!tnm_check_pct(file_name, true))
		return false;

	// 必ず次のメッセージへ
	next_msg();

	// メッセージを追加準備する
	ready_msg();

	// メッセージに画像名を設定する
	m_history_list[m_history_insert_pos].pct_flag = true;
	m_history_list[m_history_insert_pos].pct_pos = pos;
	m_history_list[m_history_insert_pos].msg_str = file_name;

	// ここがメッセージの最後
	m_history_last_pos = m_history_insert_pos;

	// さらに次メッセージへ
	next_msg();

	return true;
}

// ****************************************************************
// メッセージをクリア
// ================================================================
void C_elm_msg_back::clear_msg()
{
	m_history_list.clear();
	m_history_list.resize(m_history_cnt_max);
	for (int i = 0; i < m_history_cnt_max; i++)	{
		m_history_list[i].msg.init(S_element(), NULL);
		m_history_list[i].msg.set_def_param(Gp_ini->msg_back.moji_cnt
			, Gp_ini->msg_back.moji_size, Gp_ini->msg_back.moji_space, Gp_ini->mwnd.moji_color, Gp_ini->mwnd.shadow_color, Gp_ini->mwnd.fuchi_color, 10, 10
			, Gp_ini->msg_back.moji_size, 0, 0, C_rect(0, 0, 0, 0));
	}

	m_history_cnt = 0;
	m_history_start_pos = 0;
	m_history_last_pos = 0;
	m_history_insert_pos = 0;
	m_new_msg_flag = true;
}

// ****************************************************************
// 次の位置へ
// ================================================================
void C_elm_msg_back::next_msg()
{
	if (m_new_msg_flag)
		return;

	// メッセージが空なら何もしない
	if (m_history_list[m_history_insert_pos].pct_flag == 0 && m_history_list[m_history_insert_pos].msg_str.empty())
		return;

	// 次のメッセージへ
	m_history_insert_pos = (m_history_insert_pos + 1) % m_history_cnt_max;

	// メッセージを初期化する準備
	m_new_msg_flag = true;
}

// ****************************************************************
// メッセージを構築
// ================================================================
void C_elm_msg_back::create_msg()
{
	int last_margin = 0;

	m_msg_total_height = 0;

	// セパレーター（最初）
	if (m_history_cnt > 0 && m_album_separator_top && m_album_separator_top->get_texture(0))
	{
		S_separator separator;
		separator.total_pos = - m_album_separator_top->get_texture(0)->get_original_height();
		separator.height = m_album_separator_top->get_texture(0)->get_original_height();
		separator.sprite.create_polygon_no_init();
		separator.sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		separator.sprite.set_index_buffer(G_rect_index_buffer);
		separator.sprite.set_texture(m_album_separator_top->get_texture(0));
		m_separator_list.push_back(separator);

		//m_msg_total_height += separator.height;
	}

	// メッセージ
	for (int i = 0; i < m_history_cnt; i++)
	{
		int history_pos = (m_history_start_pos + i) % m_history_cnt_max;

		// メッセージを構築していく
		m_history_list[history_pos].msg.clear_msg();
		if (m_history_list[history_pos].pct_flag)
		{
			// koe_btn を使いまわして画像を読み込む
			m_history_list[history_pos].koe_btn.create_pct(m_history_list[history_pos].msg_str);
			m_history_list[history_pos].koe_btn.set_disp(1);
			m_history_list[history_pos].koe_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
			m_history_list[history_pos].total_pos = m_msg_total_height;
			m_history_list[history_pos].height = m_history_list[history_pos].koe_btn.get_size_y(0);

			m_msg_total_height += m_history_list[history_pos].height;	// 文章そのものの高さ
			last_margin = 0;											// 次のマージン
		}
		else
		{
			if (!m_history_list[history_pos].disp_name.empty())		// 名前
			{
				m_history_list[history_pos].msg.add_msg(m_history_list[history_pos].disp_name, NULL);
				m_history_list[history_pos].msg.new_line_no_indent();
			}
			if (!m_history_list[history_pos].msg_str.empty())		// メッセージ
			{
				m_history_list[history_pos].msg.add_msg(m_history_list[history_pos].msg_str, NULL);
				m_history_list[history_pos].msg.new_line_no_indent();
			}
			m_history_list[history_pos].msg.disp_all_moji();
			m_history_list[history_pos].total_pos = m_msg_total_height + last_margin;
			m_history_list[history_pos].height = m_history_list[history_pos].msg.get_msg_height();

			// koe_btn にアイコンを読み込む
			m_history_list[history_pos].koe_btn.create_pct(Gp_ini->msg_back.koe_btn_file);
			m_history_list[history_pos].koe_btn.set_disp(1);
			m_history_list[history_pos].koe_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
			m_history_list[history_pos].koe_btn.set_button_param(0, 0, Gp_ini->msg_back.koe_btn_action_no, Gp_ini->msg_back.koe_btn_se_no);

			// load_btn にアイコンを読み込む
			m_history_list[history_pos].load_btn.create_pct(Gp_ini->msg_back.load_btn_file);
			m_history_list[history_pos].load_btn.set_disp(1);
			m_history_list[history_pos].load_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
			m_history_list[history_pos].load_btn.set_button_param(0, 0, Gp_ini->msg_back.load_btn_action_no, Gp_ini->msg_back.load_btn_se_no);

			m_msg_total_height += last_margin;							// 文章の間隔分
			m_msg_total_height += m_history_list[history_pos].height;	// 文章そのものの高さ
			last_margin = Gp_ini->msg_back.moji_size;					// 次のマージン
		}

		// セパレーター（間）
		if (i < m_history_cnt - 1)
		{
			if (m_album_separator && m_album_separator->get_texture(0))
			{
				S_separator separator;
				separator.total_pos = m_msg_total_height;
				separator.height = m_album_separator->get_texture(0)->get_original_height();
				separator.sprite.create_polygon_no_init();
				separator.sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
				separator.sprite.set_index_buffer(G_rect_index_buffer);
				separator.sprite.set_texture(m_album_separator->get_texture(0));
				m_separator_list.push_back(separator);

				m_msg_total_height += separator.height;

				last_margin = 0;									// 次のマージン
			}
		}
		// セパレーター（最後）
		else
		{
			if (m_album_separator_bottom && m_album_separator_bottom->get_texture(0))
			{
				S_separator separator;
				separator.total_pos = m_msg_total_height;
				separator.height = m_album_separator_bottom->get_texture(0)->get_height();
				separator.sprite.create_polygon_no_init();
				separator.sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
				separator.sprite.set_index_buffer(G_rect_index_buffer);
				separator.sprite.set_texture(m_album_separator_bottom->get_texture(0));
				m_separator_list.push_back(separator);

				m_msg_total_height += separator.height;
			}
		}
	}
}

// ****************************************************************
// デバッグメッセージを取得
// ================================================================

int C_elm_msg_back::get_koe_no()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return -1;

	if (m_history_list[m_target_no].koe_no_list.empty())
		return -1;

	return m_history_list[m_target_no].koe_no_list[0];
}

TSTR C_elm_msg_back::get_disp_name()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return _T("");

	return m_history_list[m_target_no].disp_name;
}

TSTR C_elm_msg_back::get_original_name()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return _T("");

	return m_history_list[m_target_no].original_name;
}

TSTR C_elm_msg_back::get_debug_msg()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return _T("");

	return m_history_list[m_target_no].debug_msg;
}

int C_elm_msg_back::get_scn_no()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return -1;

	return m_history_list[m_target_no].scn_no;
}

int C_elm_msg_back::get_line_no()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return -1;

	return m_history_list[m_target_no].line_no;
}

// ****************************************************************
// PageUp / PageDown キーが押された
// ================================================================
void C_elm_msg_back::page_up_key_down()
{
	m_page_up_key = true;
}

void C_elm_msg_back::page_dw_key_down()
{
	m_page_dw_key = true;
}

// ****************************************************************
// ターゲットを移動
// ================================================================
void C_elm_msg_back::target_up_sub(int cnt)
{
	if (m_target_no >= m_history_start_pos)	{
		m_target_no -= cnt;
		if (m_target_no < m_history_start_pos)
			m_target_no = m_history_start_pos;
	}
	else	{
		m_target_no -= cnt;
		if (m_target_no < 0)
			m_target_no += m_history_cnt;
	}
}

void C_elm_msg_back::target_down_sub(int cnt)
{
	int history_end_pos = (m_history_start_pos + m_history_cnt - 1) % m_history_cnt_max;

	if (m_target_no <= history_end_pos)	{
		m_target_no += cnt;
		if (m_target_no > history_end_pos)
			m_target_no = history_end_pos;
	}
	else	{
		m_target_no += cnt;
		if (m_target_no >= m_history_cnt)
			m_target_no -= m_history_cnt;
	}
}


void C_elm_msg_back::target_up()
{
	if (m_target_up_start_time == 0)	{
		m_target_up_start_time = ::timeGetTime();
	}
	else	{
		DWORD past_time = ::timeGetTime() - m_target_up_start_time;
		if (past_time <= 200)	{	return;	}
	}

	// ターゲットを１つ上に移動する
	target_up_sub(1);

	// ターゲットから位置を更新する
	update_pos_from_target();
}

void C_elm_msg_back::target_down()
{
	if (m_target_dw_start_time == 0)	{
		m_target_dw_start_time = ::timeGetTime();
	}
	else	{
		DWORD past_time = ::timeGetTime() - m_target_dw_start_time;
		if (past_time <= 200)	{	return;	}
	}

	// ターゲットを１つ下に移動する
	target_down_sub(1);

	// ターゲット番号からスクロール位置を決める
	if (0 <= m_target_no && m_target_no < m_history_cnt)
		m_scroll_pos = m_cur.window_rect.height() / 2 - (m_history_list[m_target_no].total_pos + m_history_list[m_target_no].height / 2);

	// ターゲットから位置を更新する
	update_pos_from_target();
}

// ****************************************************************
// ターゲットを１つ上に（ホイール用）
// ================================================================
void C_elm_msg_back::target_up_for_wheel()
{
	// ターゲットを１つ上に移動する
	target_up_sub(1);

	// ターゲットから位置を更新する
	update_pos_from_target();
}

void C_elm_msg_back::target_down_for_wheel()
{
	// ターゲットを１つ下に移動する
	target_down_sub(1);

	// ターゲットから位置を更新する
	update_pos_from_target();
}

// ****************************************************************
// ターゲットから位置を更新
// ================================================================
void C_elm_msg_back::update_pos_from_target()
{
	// ターゲットからスクロール位置を求める
	calc_scroll_pos_from_target();

	// スクロールからスライダー位置を求める
	calc_slider_pos_from_scroll();
}

// ****************************************************************
// スクロールから位置を更新
// ================================================================
void C_elm_msg_back::update_pos_from_scroll()
{
	// スクロールからターゲット番号を求める
	calc_target_no_from_scroll();
	
	// スクロールからスライダー位置を求める
	calc_slider_pos_from_scroll();
}

// ****************************************************************
// スライダーから位置を更新
// ================================================================
void C_elm_msg_back::update_pos_from_slider()
{
	// スライダーからスクロール位置を求める
	calc_scroll_pos_from_slider();

	// スクロールからターゲット番号を求める
	calc_target_no_from_scroll();
}

// ****************************************************************
// ターゲットからスクロール位置を更新
// ================================================================
void C_elm_msg_back::calc_scroll_pos_from_target()
{
	// ターゲットを補正する
	m_target_no = limit(0, m_target_no, m_history_cnt - 1);

	// スクロール位置を求める
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_scroll_pos = - m_cur.window_rect.width() / 2 + (m_history_list[m_target_no].total_pos + m_history_list[m_target_no].height / 2);
	}
	else
	{
		m_scroll_pos = m_cur.window_rect.height() / 2 - (m_history_list[m_target_no].total_pos + m_history_list[m_target_no].height / 2);
	}
}

// ****************************************************************
// スライダーからスクロール位置を更新
// ================================================================
void C_elm_msg_back::calc_scroll_pos_from_slider()
{
	if (m_history_cnt <= 0)
		return;

	if (Gp_ini->tateyoko_mode == 1)
	{
		// ヒストリーの右端と左端を求める
		int histsp = m_history_start_pos;
		int histep = (m_history_start_pos + m_history_cnt - 1) % m_history_cnt_max;
		int msgsh = m_history_list[histsp].height;
		int msgeh = m_history_list[histep].height;
		int wind_width = m_cur.window_rect.width();		// 枠の幅
		int msgsp = - (wind_width / 2) + (msgsh / 2);
		int msgep = - (wind_width / 2) - (msgeh / 2) + m_msg_total_height;
		if (m_history_cnt == 1)	{ msgep = msgsp; }

		// 先にスライダー位置を補正する
		m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.left, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_x(0));

		// 上端と下端からスクロールの位置を求める
		m_scroll_pos = ::linear(m_slider_pos, (int)Gp_ini->msg_back.slider_rect.left, msgep, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_x(0), msgsp);
		m_scroll_pos = ::limit(msgsp, m_scroll_pos, msgep);
	}
	else
	{
		// ヒストリーの上端と下端を求める
		int histsp = m_history_start_pos;
		int histep = (m_history_start_pos + m_history_cnt - 1) % m_history_cnt_max;
		int msgsh = m_history_list[histsp].height;
		int msgeh = m_history_list[histep].height;
		int wind_height = m_cur.window_rect.height();	// 枠の高さ
		int msgsp = (wind_height / 2) - (msgsh / 2);
		int msgep = (wind_height / 2) + (msgeh / 2) - m_msg_total_height;
		if (m_history_cnt == 1)	{ msgep = msgsp; }

		// 先にスライダー位置を補正する
		m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.top, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0));

		// 上端と下端からスクロールの位置を求める
		m_scroll_pos = ::linear(m_slider_pos, (int)Gp_ini->msg_back.slider_rect.top, msgsp, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0), msgep);
		m_scroll_pos = ::limit(msgep, m_scroll_pos, msgsp);
	}
}

// ****************************************************************
// スクロールからターゲット番号を更新
// ================================================================
void C_elm_msg_back::calc_target_no_from_scroll()
{
	m_target_no = (m_history_start_pos + m_history_cnt - 1 + m_history_cnt_max) % m_history_cnt_max;

	if (Gp_ini->tateyoko_mode == 1)
	{
		for (int i = m_history_cnt - 1; i >= 0; i--)	{
			int index = (i + m_history_start_pos) % m_history_cnt_max;
			if (m_cur.window_rect.width() + m_scroll_pos - m_history_list[index].total_pos - m_history_list[index].height <= m_cur.window_rect.width() / 2)	{
				m_target_no = index;
			}
		}
	}
	else
	{
		for (int i = m_history_cnt - 1; i >= 0; i--)	{
			int index = (i + m_history_start_pos) % m_history_cnt_max;
			if (m_scroll_pos + m_history_list[index].total_pos + m_history_list[index].height >= m_cur.window_rect.height() / 2)	{
				m_target_no = index;
			}
		}
	}
}

// ****************************************************************
// スクロールからスライダー位置を更新
// ================================================================
void C_elm_msg_back::calc_slider_pos_from_scroll()
{
	if (m_history_cnt <= 0)
		return;

	if (Gp_ini->tateyoko_mode == 1)
	{
		// ヒストリーの右端と左端を求める
		int histsp = m_history_start_pos;
		int histep = (m_history_start_pos + m_history_cnt - 1) % m_history_cnt_max;
		int msgsh = m_history_list[histsp].height;
		int msgeh = m_history_list[histep].height;
		int wind_width = m_cur.window_rect.width();		// 枠の幅
		int msgsp = - (wind_width / 2) + (msgsh / 2);
		int msgep = - (wind_width / 2) - (msgeh / 2) + m_msg_total_height;
		if (m_history_cnt == 1)	{ msgep = msgsp; }

		// 先にスクロール位置を補正する
		m_scroll_pos = ::limit(msgsp, m_scroll_pos, msgep);

		// 右端と左端からスライダーの位置を求める
		m_slider_pos = ::linear(m_scroll_pos, msgsp, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_x(0), msgep, (int)Gp_ini->msg_back.slider_rect.left);
		m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.left, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_x(0));
	}
	else
	{
		// ヒストリーの上端と下端を求める
		int histsp = m_history_start_pos;
		int histep = (m_history_start_pos + m_history_cnt - 1) % m_history_cnt_max;
		int msgsh = m_history_list[histsp].height;
		int msgeh = m_history_list[histep].height;
		int wind_height = m_cur.window_rect.height();	// 枠の高さ
		int msgsp = (wind_height / 2) - (msgsh / 2);
		int msgep = (wind_height / 2) + (msgeh / 2) - m_msg_total_height;
		if (m_history_cnt == 1)	{ msgep = msgsp; }

		// 先にスクロール位置を補正する
		m_scroll_pos = ::limit(msgep, m_scroll_pos, msgsp);

		// 上端と下端からスライダーの位置を求める
		m_slider_pos = ::linear(m_scroll_pos, msgep, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0), msgsp, (int)Gp_ini->msg_back.slider_rect.top);
		m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.top, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0));
	}
}

