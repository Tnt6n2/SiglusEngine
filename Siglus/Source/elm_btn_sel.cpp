#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_input.h"
#include	"data/tnm_tool.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_lexer.h"

#include	"element/elm_group_def.h"
#include	"element/elm_btn_sel.h"
#include	"element/elm_btn_sel_item.h"
#include	"element/elm_mwnd_moji.h"
#include	"element/elm_sound.h"

#include	"engine/eng_input.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/flow_script.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_fetch_point.h"


// ****************************************************************
// 初期化
// ================================================================
void C_elm_btn_select::init(S_element element, CTSTR& name)
{
	// アイテムリストの初期化
	m_item_list.init(element, FM_BTNSELITEMLIST, name, 0, true, NULL);

	// 再初期化
	reinit();
}

// ****************************************************************
// 再初期化
// ================================================================
void C_elm_btn_select::reinit()
{
	// テンプレート番号
	m_template_no = -1;

	// デフォルトパラメータの初期化
	m_def.base_pos = C_point(0, 0);
	m_def.rep_pos = C_point(0, 0);
	m_def.x_align = 0;
	m_def.y_align = 0;
	m_def.max_y_cnt = 0;
	m_def.line_width = 0;
	m_def.moji_cnt = 0;
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_size = 0;
	m_def.moji_space = C_size(0, 0);
	m_def.moji_color = 0;
	m_def.moji_hit_color = 0;
	m_def.shadow_color = 0;
	m_def.fuchi_color = 0;
	m_def.btn_action_no = -1;
	m_def.open_anime_type = 0;
	m_def.open_anime_time = 0;
	m_def.close_anime_type = 0;
	m_def.close_anime_time = 0;
	m_def.decide_anime_type = 0;
	m_def.decide_anime_time = 0;
	m_def.sync_type = 0;

	// 処理変数初期化
	init_work_variable();
}

// ****************************************************************
// ボタン選択肢：終了
// ================================================================
void C_elm_btn_select::finish()
{
	// アイテムリスト
	m_item_list.finish();
}

// ****************************************************************
// ボタン選択肢：処理変数初期化
// ================================================================
void C_elm_btn_select::init_work_variable()
{
	m_cur = m_def;		// デフォルトパラメータを現在のパラメータへコピーする

	m_open_anime_type = 0;
	m_open_anime_time = 0;
	m_open_anime_cur_time = 0;
	m_close_anime_type = 0;
	m_close_anime_time = 0;
	m_close_anime_cur_time = 0;
	m_decide_anime_type = 0;
	m_decide_anime_time = 0;
	m_decide_anime_cur_time = 0;
	m_decide_sel_no = -1;
	m_sync_type = 0;
	m_appear_flag = false;
	m_processing_flag_0 = false;
	m_processing_flag_1 = false;
	m_processing_flag_2 = false;
	m_cancel_enable_flag = false;
	m_capture_flag = false;
	m_sel_start_call_scn.clear();
	m_sel_start_call_z_no = -1;
	m_read_flag.scn_no = -1;
	m_read_flag.flag_no = -1;

	m_item_list.clear();

	m_capture_now_flag = false;
}

// ****************************************************************
// テンプレートの値を設定
// ================================================================
void C_elm_btn_select::set_template(int template_no)
{
	if (template_no == m_template_no)
		return;

	// テンプレート番号
	m_template_no = template_no;

	// テンプレートを再構築
	restruct_template();

	// アイテム
	for (int i = 0; i < (int)m_item_list.get_size(); i++)
		m_item_list[i].set_template(template_no);

	// 処理変数初期化
	init_work_variable();
}

// ****************************************************************
// テンプレートを再構築
// ================================================================
void C_elm_btn_select::restruct_template()
{
	m_def.base_pos = Gp_ini->sel_btn[m_template_no].base_pos;
	m_def.rep_pos = Gp_ini->sel_btn[m_template_no].rep_pos;
	m_def.x_align = Gp_ini->sel_btn[m_template_no].x_align;
	m_def.y_align = Gp_ini->sel_btn[m_template_no].y_align;
	m_def.max_y_cnt = Gp_ini->sel_btn[m_template_no].max_y_cnt;
	m_def.line_width = Gp_ini->sel_btn[m_template_no].line_width;
	m_def.moji_cnt = Gp_ini->sel_btn[m_template_no].moji_cnt;
	m_def.moji_pos = Gp_ini->sel_btn[m_template_no].moji_pos;
	m_def.moji_size = Gp_ini->sel_btn[m_template_no].moji_size;
	m_def.moji_space = Gp_ini->sel_btn[m_template_no].moji_space;
	m_def.moji_x_align = Gp_ini->sel_btn[m_template_no].moji_x_align;
	m_def.moji_y_align = Gp_ini->sel_btn[m_template_no].moji_y_align;
	m_def.moji_color = Gp_ini->sel_btn[m_template_no].moji_color;
	m_def.moji_hit_color = Gp_ini->sel_btn[m_template_no].moji_hit_color;
	m_def.shadow_color = Gp_ini->mwnd.shadow_color;
	m_def.fuchi_color = Gp_ini->mwnd.fuchi_color;
	m_def.btn_action_no = Gp_ini->sel_btn[m_template_no].btn_action_no;
	m_def.open_anime_type = Gp_ini->sel_btn[m_template_no].open_anime_type;
	m_def.open_anime_time = Gp_ini->sel_btn[m_template_no].open_anime_time;
	m_def.close_anime_type = Gp_ini->sel_btn[m_template_no].close_anime_type;
	m_def.close_anime_time = Gp_ini->sel_btn[m_template_no].close_anime_time;
	m_def.decide_anime_type = Gp_ini->sel_btn[m_template_no].decide_anime_type;
	m_def.decide_anime_time = Gp_ini->sel_btn[m_template_no].decide_anime_time;
}

// ****************************************************************
// セーブ
// ================================================================
void C_elm_btn_select::save(C_tnm_save_stream& stream)
{
	// テンプレート番号
	stream.save(m_template_no);

	// 現在のパラメータ
	stream.save(m_cur);
	stream.save(m_appear_flag);
	stream.save(m_processing_flag_0);
	stream.save(m_processing_flag_1);
	stream.save(m_processing_flag_2);
	stream.save(m_cancel_enable_flag);
	stream.save(m_capture_flag);
	stream.save(m_sel_start_call_scn);
	stream.save(m_sel_start_call_z_no);

	// アイテム
	m_item_list.save(stream);
}

// ****************************************************************
// ロード
// ================================================================
void C_elm_btn_select::load(C_tnm_save_stream& stream)
{
	// テンプレート番号
	stream.load(m_template_no);

	// テンプレートを再構築
	restruct_template();

	// 現在のパラメータ
	stream.load(m_cur);
	stream.load(m_appear_flag);
	stream.load(m_processing_flag_0);
	stream.load(m_processing_flag_1);
	stream.load(m_processing_flag_2);
	stream.load(m_cancel_enable_flag);
	stream.load(m_capture_flag);
	stream.load(m_sel_start_call_scn);
	stream.load(m_sel_start_call_z_no);

	// アイテム
	m_item_list.load(stream);
}

// ****************************************************************
// コピー
// ================================================================
void C_elm_btn_select::copy(C_elm_btn_select* src)
{
	// テンプレート番号
	m_template_no = src->m_template_no;

	// テンプレートを再構築
	restruct_template();

	// 現在のパラメータ
	m_cur = src->m_cur;
	m_appear_flag = src->m_appear_flag;
	m_processing_flag_0 = src->m_processing_flag_0;
	m_processing_flag_1 = src->m_processing_flag_1;
	m_processing_flag_2 = src->m_processing_flag_2;
	m_cancel_enable_flag = src->m_cancel_enable_flag;
	m_capture_flag = src->m_capture_flag;
	m_sel_start_call_scn = src->m_sel_start_call_scn;
	m_sel_start_call_z_no = src->m_sel_start_call_z_no;
	
	// アイテム
	m_item_list.resize(src->m_item_list.get_size());
	for (int i = 0; i < m_item_list.get_size(); i++)
		m_item_list[i] = src->m_item_list[i];
}

// ****************************************************************
// フレーム初期化処理
// ================================================================
void C_elm_btn_select::frame_init()
{
	// アイテム
	for (int i = 0; i < m_item_list.get_size(); i++)
		m_item_list[i].frame_init();
}

// ****************************************************************
// 時間を進める
// ================================================================
void C_elm_btn_select::update_time(int past_game_time, int past_real_time)
{
	// 時間を進める
	m_open_anime_cur_time += past_game_time;
	m_close_anime_cur_time += past_game_time;
	m_decide_anime_cur_time += past_game_time;

	// アイテム
	for (int i = 0; i < m_item_list.get_size(); i++)
		m_item_list[i].update_time(past_game_time, past_real_time);

	// 出現しているか閉じアニメ中のみ進める
	if (m_appear_flag || is_close_anime_doing())	{

		int open_anime_end_time = m_open_anime_time;
		int close_anime_end_time = m_close_anime_time;
		int decide_anime_end_time = m_decide_anime_time;

		// 縦方向のアイテム数を計算しておく
		int y_cnt;
		if (m_cur.max_y_cnt > 0)	{
			y_cnt = std::min(m_cur.max_y_cnt, m_item_list.get_size());
		}
		else	{
			y_cnt = m_item_list.get_size();
		}

		// アニメによっては終わる時間が少しずれる
		if (m_open_anime_type == 2 || m_open_anime_type == 3 || m_open_anime_type == 4 || m_open_anime_type == 5 || m_open_anime_type == 6)	{

			// 縦方向のアイテム数×50ms
			open_anime_end_time += y_cnt * 50;
		}
		if (m_close_anime_type == 2 || m_close_anime_type == 3 || m_close_anime_type == 4 || m_close_anime_type == 5 || m_close_anime_type == 6)	{

			// 縦方向のアイテム数×50ms
			close_anime_end_time += y_cnt * 50;
		}

		// キャプチャー中だった場合はキャプチャー終了
		if (is_capture_doing())	{
			end_capture();
		}
		// 時間がたてば開きアニメ終了
		if (is_open_anime_doing() && m_open_anime_cur_time - open_anime_end_time >= 0)	{
			end_open_anime();
		}
		// 時間がたてば閉じアニメ終了
		if (is_close_anime_doing() && m_close_anime_cur_time - close_anime_end_time >= 0)	{
			end_close_anime();
		}
		// 時間がたてば決定アニメ終了
		if (is_decide_anime_doing() && m_decide_anime_cur_time - decide_anime_end_time >= 0)	{
			end_decide_anime();
		}
	}
}

// ****************************************************************
// フレーム処理
// ================================================================
void C_elm_btn_select::frame(bool draw_futoku, bool draw_shadow)
{
	// 出現しているか閉じアニメ中のみ表示する
	if (m_appear_flag || is_close_anime_doing())	{

		// 縦方向のアイテム数を計算しておく
		int y_cnt;
		if (m_cur.max_y_cnt > 0)	{
			y_cnt = std::min(m_cur.max_y_cnt, m_item_list.get_size());
		}
		else	{
			y_cnt = m_item_list.get_size();
		}

		// アイテムのフレーム処理
		for (int i = 0; i < m_item_list.get_size(); i++)	{
			int tr_rep = 255;
			int x_rep = 0, y_rep = 0;

			// 開きアニメによる補正
			if (false);
			else if (m_open_anime_type == 1)	{
				tr_rep = linear_limit(m_open_anime_cur_time, 0, 0, m_open_anime_time, 255);
			}
			else if (m_open_anime_type == 2)	{
				int time = m_open_anime_cur_time - 50 * (y_cnt - (i % y_cnt));				// 50ﾐﾘ秒ずつ遅らせる
				tr_rep = linear_limit(time, 0, 0, m_open_anime_time, 255);
				y_rep = speed_down_limit(time, 0, - (int)Gp_global->game_screen_size.cy, m_open_anime_time, 0);
			}
			else if (m_open_anime_type == 3)	{
				int time = m_open_anime_cur_time - 50 * (i % y_cnt);						// 50ﾐﾘ秒ずつ遅らせる
				tr_rep = linear_limit(time, 0, 0, m_open_anime_time, 255);
				y_rep = speed_down_limit(time, 0, (int)Gp_global->game_screen_size.cy, m_open_anime_time, 0);
			}
			else if (m_open_anime_type == 4)	{
				int time = m_open_anime_cur_time - 50 * (i % y_cnt);						// 50ﾐﾘ秒ずつ遅らせる
				tr_rep = linear_limit(time, 0, 0, m_open_anime_time, 255);
				x_rep = speed_down_limit(time, 0, - (int)Gp_global->game_screen_size.cx, m_open_anime_time, 0);
			}
			else if (m_open_anime_type == 5)	{
				int time = m_open_anime_cur_time - 50 * (i % y_cnt);						// 50ﾐﾘ秒ずつ遅らせる
				tr_rep = linear_limit(time, 0, 0, m_open_anime_time, 255);
				x_rep = speed_down_limit(time, 0, (int)Gp_global->game_screen_size.cx, m_open_anime_time, 0);
			}
			else if (m_open_anime_type == 6)	{
				int time = m_open_anime_cur_time - 50 * (i % y_cnt);						// 50ﾐﾘ秒ずつ遅らせる
				tr_rep = linear_limit(time, 0, 0, m_open_anime_time, 255);
				if ((i % y_cnt) % 2 == 0)	x_rep = speed_down_limit(time, 0, (int)Gp_global->game_screen_size.cx, m_open_anime_time, 0);
				else						x_rep = speed_down_limit(time, 0, - (int)Gp_global->game_screen_size.cx, m_open_anime_time, 0);
			}

			// 決定アニメによる補正
			if (m_decide_sel_no >= 0 && i != m_decide_sel_no)	{

				if (false);
				else if (m_decide_anime_type == 1)	{
					tr_rep = linear_limit(m_decide_anime_cur_time, 0, 255, m_decide_anime_time, 0);
				}
				else if (m_decide_anime_type == 2)	{
					C_point decided_pos = m_item_list[m_decide_sel_no].get_pos();
					C_point my_pos = m_item_list[i].get_pos();
					C_point rep_pos = decided_pos - my_pos;

					tr_rep = linear_limit(m_decide_anime_cur_time, 0, 255, m_decide_anime_time, 0);
					x_rep = speed_down_limit(m_decide_anime_cur_time, 0, 0, m_decide_anime_time, rep_pos.x);
					y_rep = speed_down_limit(m_decide_anime_cur_time, 0, 0, m_decide_anime_time, rep_pos.y);
				}
			}

			// 決定アニメがある場合、選択されたアイテム以外は消えているはず
			if (m_close_anime_type >= 1 && m_cur.decide_anime_type >= 1 && m_decide_sel_no >= 0 && i != m_decide_sel_no)	{
				tr_rep = 0;
			}
			else	{

				// 閉じアニメによる補正
				if (false);
				else if (m_close_anime_type == 1)	{
					tr_rep = linear_limit(m_close_anime_cur_time, 0, 255, m_close_anime_time, 0);
				}
				else if (m_close_anime_type == 2)	{
					int time = m_close_anime_cur_time - 50 * (i % y_cnt);						// 50ﾐﾘ秒ずつ遅らせる
					tr_rep = linear_limit(time, 0, 255, m_close_anime_time, 0);
					y_rep = speed_up_limit(time, 0, 0, m_close_anime_time, - (int)Gp_global->game_screen_size.cy);
				}
				else if (m_close_anime_type == 3)	{
					int time = m_close_anime_cur_time - 50 * (y_cnt - (i % y_cnt));				// 50ﾐﾘ秒ずつ遅らせる
					tr_rep = linear_limit(time, 0, 255, m_close_anime_time, 0);
					y_rep = speed_up_limit(time, 0, 0, m_close_anime_time, (int)Gp_global->game_screen_size.cy);
				}
				else if (m_close_anime_type == 4)	{
					int time = m_close_anime_cur_time - 50 * (i % y_cnt);						// 50ﾐﾘ秒ずつ遅らせる
					tr_rep = linear_limit(time, 0, 255, m_close_anime_time, 0);
					x_rep = speed_up_limit(time, 0, 0, m_close_anime_time, - (int)Gp_global->game_screen_size.cx);
				}
				else if (m_close_anime_type == 5)	{
					int time = m_close_anime_cur_time - 50 * (i % y_cnt);						// 50ﾐﾘ秒ずつ遅らせる
					tr_rep = linear_limit(time, 0, 255, m_close_anime_time, 0);
					x_rep = speed_up_limit(time, 0, 0, m_close_anime_time, (int)Gp_global->game_screen_size.cx);
				}
				else if (m_close_anime_type == 6)	{
					int time = m_close_anime_cur_time - 50 * (i % y_cnt);						// 50ﾐﾘ秒ずつ遅らせる
					tr_rep = linear_limit(time, 0, 255, m_close_anime_time, 0);
					if ((i % y_cnt) % 2 == 0)	x_rep = speed_up_limit(time, 0, 0, m_close_anime_time, (int)Gp_global->game_screen_size.cx);
					else						x_rep = speed_up_limit(time, 0, 0, m_close_anime_time, - (int)Gp_global->game_screen_size.cx);
				}
			}

			// アイテムに引き継ぐレンダーパラメータ
			S_tnm_render_param item_trp;
			item_trp.disp = true;
			item_trp.sorter = S_tnm_sorter(Gp_ini->mwnd.order, 0);
			item_trp.tr = tr_rep;
			
			// アイテムのフレーム処理
			m_item_list[i].frame(m_cur.base_pos + C_point(x_rep, y_rep), &item_trp, m_cur.moji_hit_color, m_cur.btn_action_no, draw_futoku, draw_shadow);
		}
	}
}

// ****************************************************************
// スプライトツリーを構築
// ================================================================
void C_elm_btn_select::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	// 出現しているか閉じアニメ中のみ表示する
	if (m_appear_flag || is_close_anime_doing())	{

		// 選択肢
		for (int i = 0; i < m_item_list.get_size(); i++)
			m_item_list[i].get_sprite_tree(root);
	}
}

// ****************************************************************
// ボタンを登録
// ================================================================
void C_elm_btn_select::regist_button()
{
	// 出現していて開きアニメが終わってキャプチャーが終了している場合のみ処理
	if (m_appear_flag && !is_open_anime_doing() && !is_decide_anime_doing() && !m_capture_now_flag)	{

		// 選択肢
		for (int i = 0; i < m_item_list.get_size(); i++)
			m_item_list[i].regist_button();
	}
}

// ****************************************************************
// ボタンイベント処理
// ================================================================
void C_elm_btn_select::button_event()
{
	// 出現していて開きアニメが終わっている場合のみ処理
	if (m_appear_flag && !is_open_anime_doing() && !is_decide_anime_doing())
	{
		int sel_no = TNM_GROUP_NOT_DECIDED;	// まだ決定されていない

		// キャンセルされた場合	
		if (m_cancel_enable_flag && tnm_input_use_key_down_up(VK_EX_CANCEL))
		{
			sel_no = TNM_GROUP_CANCELED;
		}
		else
		{
			// キャンセルされてないので各選択肢の処理を行います
			for (int i = 0; i < m_item_list.get_size(); i++)
			{
				if (m_item_list[i].button_event())
					sel_no = i;
			}
		}

		// 選択肢が決定またはキャンセルされた！
		if (sel_no >= -1)
		{
			// 決定
			decide(sel_no);
			// スクリプト取得用のメッセージ
			if (sel_no == TNM_GROUP_CANCELED)
			{
				Gp_global->last_sel_msg = _T("（キャンセル）");
			}
			else
			{
				Gp_global->last_sel_msg = m_item_list[sel_no].get_msg();
			}

			// 登録された既読フラグを立てる
			set_read_flag();
		}
	}
}

// ****************************************************************
// 空かどうか判定
// ================================================================
bool C_elm_btn_select::is_empty()
{
	return m_item_list.is_empty();
}

// ****************************************************************
// 選択肢の初期位置を設定する
// ================================================================
void C_elm_btn_select::set_def_pos(C_point pos)
{
	m_def.moji_pos = pos;
	m_cur.moji_pos = pos;
}

// ****************************************************************
// 選択肢を追加
// ================================================================
void C_elm_btn_select::add_sel(CTSTR& msg, int item_type, int color)
{
	// 色がデフォルトの場合は指定する
	if (color == -1)
		color = m_cur.moji_color;

	// アイテムを作成
	int old_cnt = m_item_list.get_size();
	m_item_list.resize(old_cnt + 1);
	m_item_list[old_cnt].set_template(m_template_no);
	m_item_list[old_cnt].set_msg(msg, item_type, m_cur.moji_size, m_cur.moji_pos, m_cur.moji_space, m_cur.moji_x_align, m_cur.moji_y_align, color, m_cur.shadow_color, m_cur.fuchi_color);
}

// ****************************************************************
// 選択肢を再構築
// ================================================================
void C_elm_btn_select::restruct_item()
{
	// ================================================================
	// 縦にずらーっと並べます。全体矩形を計算します。
	// ----------------------------------------------------------------

	// 選択肢の数
	int item_cnt = m_item_list.get_size();
	if (!m_item_list.is_empty())	{

		// アイテムの位置を計算する
		C_point offset;
		C_point max_offset;
		int org_offset_x = 0;
		for (int i = 0, y_cnt = 0; i < item_cnt; i++)	{

			// type == off の場合は何もしない
			if (m_item_list[i].get_item_type() != TNM_SEL_ITEM_TYPE_OFF)	{

				m_item_list[i].set_pos(offset);
				offset += m_cur.rep_pos;

				if (offset.x > max_offset.x)	{
					max_offset.x = offset.x;
				}
				if (offset.y > max_offset.y)	{
					max_offset.y = offset.y;
				}

				y_cnt++;

				// Ｙの最大値が指定されている場合は改行（改列？）
				if (m_cur.max_y_cnt > 0 && y_cnt >= m_cur.max_y_cnt)	{
					offset.x = org_offset_x + m_cur.line_width;
					offset.y = 0;
					org_offset_x = offset.x;
					y_cnt = 0;
				}
			}
		}

		// アイテムの大きさを取得する
		C_size item_size = m_item_list[0].get_item_size();

		// 選択肢全体の大きさを計算する
		int total_x = max_offset.x - m_cur.rep_pos.x + item_size.cx;
		int total_y = max_offset.y - m_cur.rep_pos.y + item_size.cy;

		// アイテムの位置を再計算する
		for (int i = 0; i < item_cnt; i++)	{

			// type == off の場合は何もしない
			if (m_item_list[i].get_item_type() != TNM_SEL_ITEM_TYPE_OFF)	{
				int rep_x = 0, rep_y = 0;

				if (false);
				else if (m_cur.x_align == 1)	{	rep_x -= total_x / 2;	}
				else if (m_cur.x_align == 2)	{	rep_x -= total_x;		}

				if (false);
				else if (m_cur.y_align == 1)	{	rep_y -= total_y / 2;	}
				else if (m_cur.y_align == 2)	{	rep_y -= total_y;		}

				m_item_list[i].set_pos(m_item_list[i].get_pos() + C_point(rep_x, rep_y));
			}
		}
	}
}

// ****************************************************************
// キャンセル可能を設定する
// ================================================================
void C_elm_btn_select::set_cancel_enable(bool enable)
{
	m_cancel_enable_flag = enable;
}

// ****************************************************************
// キャプチャーフラグを設定する
// ================================================================
void C_elm_btn_select::set_capture_flag(bool value)
{
	m_capture_flag = value;
}

// ****************************************************************
// 選択開始時に呼ばれるコールを設定する
// ================================================================
void C_elm_btn_select::set_sel_start_call(CTSTR& scn_name, int z_no)
{
	m_sel_start_call_scn = scn_name;
	m_sel_start_call_z_no = z_no;
}

// ****************************************************************
// 選択開始時に呼ばれるコールを行う
// ================================================================
void C_elm_btn_select::do_sel_start_call()
{
	// シーンをコール
	if (!tnm_scene_proc_farcall(m_sel_start_call_scn, m_sel_start_call_z_no, FM_VOID, false, true))
		return;

	// 再度スクリプトを実行する
	tnm_proc_script();
}

// ****************************************************************
// 選択を開始する
// ================================================================
void C_elm_btn_select::start_sel(int sync_type)
{
	m_cur.sync_type = sync_type;

	// 開きアニメを設定
	m_open_anime_type = m_cur.open_anime_type;
	m_open_anime_time = m_cur.open_anime_time;
	m_open_anime_cur_time = 0;

	// 選択肢出現
	m_appear_flag = true;
	m_processing_flag_0 = true;
	m_processing_flag_1 = true;
	m_processing_flag_2 = true;
	m_sync_type = sync_type;

	// アイテムを表示する
	for (int i = 0; i < m_item_list.get_size(); i++)	{
		m_item_list[i].set_disp();
	}

	// フェッチポイントを初期化する
	tnm_init_fetch_point();

	// フェッチの修正座標を計算
	C_point rep_pos;
	if (!m_item_list.is_empty())	{
		rep_pos.y = m_item_list[0].get_item_size().cy / 2;
		rep_pos.x = rep_pos.y;
	}

	// フェッチポイントを追加する
	for (int i = 0; i < m_item_list.get_size(); i++)	{
		if (m_item_list[i].get_item_type() == TNM_SEL_ITEM_TYPE_ON)	{	// 選択できるものだけ設定
			C_point pos = m_cur.base_pos + m_item_list[i].get_pos() + rep_pos;
			C_rect rect = C_rect::by_size(m_cur.base_pos + m_item_list[i].get_pos(), m_item_list[i].get_item_size());
			tnm_add_fetch_point(pos, rect);
		}
	}

	// フェッチ初期移動
	tnm_fetch_proc_initial_move();

	// オートモード中に消していたカーソルを復帰させる
	Gp_global->auto_mode_cursor_off = false;
}

// ****************************************************************
// 閉じアニメを開始する
// ================================================================
void C_elm_btn_select::close()
{
	// 出現フラグを降ろす
	m_appear_flag = false;

	// 閉じアニメを設定
	m_close_anime_type = m_cur.close_anime_type;
	m_close_anime_time = m_cur.close_anime_time;
	m_close_anime_cur_time = 0;

	// 決定アニメがある場合は閉じアニメが変更される
	if (m_decide_anime_type >= 1 && m_decide_sel_no >= 0 && m_close_anime_type >= 1)
		m_close_anime_type = 1;

	// 閉じアニメがない場合は閉じアニメ終了
	if (m_close_anime_type == 0)	{
		end_close_anime();
	}

	// 処理中フラグをおろす（その２）
	m_processing_flag_1 = false;
}

// ****************************************************************
// 決定する
// ================================================================
void C_elm_btn_select::decide(int sel_no)
{
	// 決定した番号を保存する
	m_decide_sel_no = sel_no;

	// 効果音
	tnm_play_se(m_decide_sel_no >= 0 ? TNM_SE_TYPE_DECIDE : TNM_SE_TYPE_CANCEL);

	// 結果をスタックに積む
	tnm_stack_push_int(sel_no);

	// 選択肢ポイントを立てる
	tnm_set_sel_point();

	// 処理中フラグを下す（その３）
	m_processing_flag_2 = false;

	// 決定アニメを設定
	if (m_decide_sel_no >= 0 && m_cur.decide_anime_type >= 1)	{
		m_decide_anime_type = m_cur.decide_anime_type;
		m_decide_anime_time = m_cur.decide_anime_time;
		m_decide_anime_cur_time = 0;
	}
	// キャンセル、もしくは決定アニメがない場合はすぐ閉じる
	else	{
		close();
	}

	// フェッチポイントを初期化する
	tnm_init_fetch_point();
}

// ****************************************************************
// キャプチャーを終了する
// ================================================================
void C_elm_btn_select::end_capture()
{
	m_capture_now_flag = false;

	// 選択肢開始コールを行う
	if (!m_sel_start_call_scn.empty() && m_sel_start_call_z_no >= 0)	{
		Gp_global->sel_start_call_flag = true;
	}
}

// ****************************************************************
// 開きアニメを終了する
// ================================================================
void C_elm_btn_select::end_open_anime()
{
	m_open_anime_type = 0;

	// キャプチャーを行う
	if (m_capture_flag)	{
		m_capture_now_flag = true;
		tnm_syscom_create_capture(TNM_CAPTURE_PRIOR_SAVE, false);
	}
}

// ****************************************************************
// 閉じアニメを終了する
// ================================================================
void C_elm_btn_select::end_close_anime()
{
	m_close_anime_type = 0;

	// キャプチャーを解放する
	if (m_capture_flag)	{
		tnm_syscom_free_capture(TNM_CAPTURE_PRIOR_SAVE);
	}

	// アイテムリストをクリア
	m_item_list.clear();

	// 処理中フラグを降ろす
	// 念のため全てのフラグを降ろします。これで安全。
	m_processing_flag_0 = false;
	m_processing_flag_1 = false;
	m_processing_flag_2 = false;
}

// ****************************************************************
// 決定アニメを終了する
// ================================================================
void C_elm_btn_select::end_decide_anime()
{
	// 閉じる（中で m_decide_anime_type を使ってるので先に行う！）
	close();

	// 次にフラグを降ろす
	m_decide_anime_type = 0;
}

// ****************************************************************
// キャプチャー中を判定する
// ================================================================
bool C_elm_btn_select::is_capture_doing()
{
	return m_capture_now_flag;
}

// ****************************************************************
// 開きアニメ中を判定する
// ================================================================
bool C_elm_btn_select::is_open_anime_doing()
{
	return m_open_anime_type > 0;
}

// ****************************************************************
// 閉じアニメ中を判定する
// ================================================================
bool C_elm_btn_select::is_close_anime_doing()
{
	return m_close_anime_type > 0;
}

// ****************************************************************
// 決定アニメ中を判定する
// ================================================================
bool C_elm_btn_select::is_decide_anime_doing()
{
	return m_decide_anime_type > 0;
}

// ****************************************************************
// 処理中かどうかを判定する
// ================================================================
bool C_elm_btn_select::is_processing()
{
	switch (m_sync_type)	{
		case 0:		return m_processing_flag_0;
		case 1:		return m_processing_flag_1;
		case 2:		return m_processing_flag_2;
	}

	return false;
}

// ****************************************************************
// 既読フラグを登録する
// ================================================================
void C_elm_btn_select::regist_read_flag(int scn_no, int flag_no)
{
	m_read_flag.scn_no = scn_no;
	m_read_flag.flag_no = flag_no;
}

// ****************************************************************
// 登録した既読フラグを立てる
// ================================================================
void C_elm_btn_select::set_read_flag()
{
	if (0 <= m_read_flag.scn_no && m_read_flag.scn_no < (int)Gp_read_flag->size())	{
		if (0 <= m_read_flag.flag_no && m_read_flag.flag_no < (int)(*Gp_read_flag)[m_read_flag.scn_no].size())	{
			(*Gp_read_flag)[m_read_flag.scn_no][m_read_flag.flag_no] = 1;
		}
	}

	m_read_flag.scn_no = -1;
	m_read_flag.flag_no = -1;
}

