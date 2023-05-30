#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_array.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_sorter.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_btn_template.h"
#include	"data/tnm_color_table.h"
#include	"element/elm_stage.h"
#include	"element/elm_mwnd_msg.h"
#include	"element/elm_mwnd_moji.h"
#include	"engine/eng_config.h"
#include	"engine/eng_message.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_error.h"
#include	"engine/flow_command.h"

// ****************************************************************
// 初期化
// ================================================================
void C_elm_mwnd_msg::init(S_element elm_group_list, C_elm_mwnd* p_parent)
{
	// デフォルトパラメータ
	m_def_parent = p_parent;
	m_def_elm_group_list = elm_group_list;

	// デフォルトの共通パラメータ
	m_def.moji_cnt = C_size(0, 0);
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_rep_pos = C_point(0, 0);
	m_def.moji_size = 0;
	m_def.moji_space = C_size(0, 0);
	m_def.moji_color = 0;
	m_def.shadow_color = 0;
	m_def.fuchi_color = 0;
	m_def.name_moji_size = 0;
	m_def.name_newline = 0;
	m_def.name_bracket = 0;

	// パラメータ初期化
	init_param();
}

// ****************************************************************
// デフォルトの値を設定
// ================================================================
void C_elm_mwnd_msg::set_def_param(C_size moji_cnt
	, int moji_size, C_size moji_space, int moji_color, int shadow_color, int fuchi_color, int ruby_size, int ruby_space
	, int name_moji_size, int name_newline, int name_bracket, C_rect talk_margin)
{
	// テンプレート番号によって初期化
	m_def.moji_cnt = moji_cnt;
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_rep_pos = C_point(0, 0);
	m_def.moji_size = moji_size;
	m_def.moji_space = moji_space;
	m_def.moji_color = moji_color;
	m_def.shadow_color = shadow_color;
	m_def.fuchi_color = fuchi_color;
	m_def.ruby_size = ruby_size;
	m_def.ruby_space = ruby_space;
	m_def.name_moji_size = name_moji_size;
	m_def.name_newline = name_newline;
	m_def.name_bracket = name_bracket;
	m_def.talk_margin = talk_margin;

	// パラメータ初期化
	init_param();
}

// ****************************************************************
// 処理変数初期化
// ================================================================
//		デフォルトのパラメータに戻します。
// ================================================================
void C_elm_mwnd_msg::init_param()
{
	m_cur = m_def;				// デフォルトパラメータを現在のパラメータへコピーする
	m_cur.moji_color = -1;		// 色については「デフォルトに従う」に設定しておく！
	m_cur.shadow_color = -1;	// 色については「デフォルトに従う」に設定しておく！
	m_cur.fuchi_color = -1;		// 色については「デフォルトに従う」に設定しておく！

	init_param_sub();
}

void C_elm_mwnd_msg::init_param_sub()
{
	m_chara_moji_color_no = -1;		// デフォルトに従う
	m_chara_shadow_color_no = -1;	// デフォルトに従う
	m_chara_fuchi_color_no = -1;	// デフォルトに従う
	m_indent_pos = 0;
	m_indent_moji = 0;
	m_indent_cnt = 0;
	m_cur_msg_type = -1;				// クリア後
	m_ruby_start_pos = C_point(0, 0);
	m_disp_moji_cnt = 0;
	m_hide_moji_cnt = 0;
	m_last_moji_disp_time = 0;
	m_btn_no = -1;
	m_btn_group_no = -1;
	m_btn_action_no = -1;
	m_btn_se_no = -1;

	m_cur_msg_type_decided = false;
	m_line_head_flag = true;
	m_ruby_start_ready = false;
	m_button_flag = false;
	m_progress_start_flag = false;

	m_debug_msg.clear();
	m_ruby.clear();
	m_moji_list.clear();
	m_msg_btn.clear();

	// メッセージ領域を大きさ計算する（文字数だけは現在の値を使う）
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
	}
	else
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
	}
}

// ****************************************************************
// セーブ
// ================================================================
void C_elm_mwnd_msg::save(C_tnm_save_stream& stream)
{
	// 現在のパラメータ
	stream.save(m_cur);
	stream.save(m_chara_moji_color_no);
	stream.save(m_chara_shadow_color_no);
	stream.save(m_chara_fuchi_color_no);
	stream.save(m_indent_pos);
	stream.save(m_indent_moji);
	stream.save(m_indent_cnt);
	stream.save(m_cur_msg_type);
	stream.save(m_ruby_start_pos);
	stream.save(m_disp_moji_cnt);
	stream.save(m_hide_moji_cnt);
	stream.save(m_debug_msg);
	stream.save(m_ruby);
	stream.save(m_btn_no);
	stream.save(m_btn_group_no);
	stream.save(m_btn_action_no);
	stream.save(m_btn_se_no);

	stream.save(m_cur_msg_type_decided);
	stream.save(m_line_head_flag);
	stream.save(m_ruby_start_ready);
	stream.save(m_button_flag);

	// 文字
	stream.save(m_moji_list);
}

// ****************************************************************
// ロード
// ================================================================
void C_elm_mwnd_msg::load(C_tnm_save_stream& stream)
{
	// 現在のパラメータ
	stream.load(m_cur);
	stream.load(m_chara_moji_color_no);
	stream.load(m_chara_shadow_color_no);
	stream.load(m_chara_fuchi_color_no);
	stream.load(m_indent_pos);
	stream.load(m_indent_moji);
	stream.load(m_indent_cnt);
	stream.load(m_cur_msg_type);
	stream.load(m_ruby_start_pos);
	stream.load(m_disp_moji_cnt);
	stream.load(m_hide_moji_cnt);
	stream.load(m_debug_msg);
	stream.load(m_ruby);
	stream.load(m_btn_no);
	stream.load(m_btn_group_no);
	stream.load(m_btn_action_no);
	stream.load(m_btn_se_no);

	stream.load(m_cur_msg_type_decided);
	stream.load(m_line_head_flag);
	stream.load(m_ruby_start_ready);
	stream.load(m_button_flag);

	// 文字
	stream.load(m_moji_list);

	// メッセージ領域を大きさ計算する（文字数だけは現在の値を使う）
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
	}
	else
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
	}
}

// ****************************************************************
// コピー
// ================================================================
void C_elm_mwnd_msg::copy(C_elm_mwnd_msg* src)
{
	// デフォルトのパラメータをコピー
	m_def = src->m_def;

	// 現在のパラメータ
	m_cur = src->m_cur;
	m_indent_pos = src->m_indent_pos;
	m_chara_moji_color_no = src->m_chara_moji_color_no;
	m_chara_shadow_color_no = src->m_chara_shadow_color_no;
	m_chara_fuchi_color_no = src->m_chara_fuchi_color_no;
	m_indent_moji = src->m_indent_moji;
	m_indent_cnt = src->m_indent_cnt;
	m_ruby_start_pos = src->m_ruby_start_pos;
	m_disp_moji_cnt = src->m_disp_moji_cnt;
	m_hide_moji_cnt = src->m_hide_moji_cnt;
	m_last_moji_disp_time = src->m_last_moji_disp_time;
	m_debug_msg = src->m_debug_msg;
	m_ruby = src->m_ruby;
	m_btn_no = src->m_btn_no;
	m_btn_group_no = src->m_btn_group_no;
	m_btn_action_no = src->m_btn_action_no;
	m_btn_se_no = src->m_btn_se_no;

	m_line_head_flag = src->m_line_head_flag;
	m_ruby_start_ready = src->m_ruby_start_ready;
	m_button_flag = src->m_button_flag;
	m_progress_start_flag = src->m_progress_start_flag;

	// メッセージ領域を大きさ計算する（文字数だけは現在の値を使う）
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
	}
	else
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
	}

	// 文字
	m_moji_list.resize(src->m_moji_list.size());
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i] = src->m_moji_list[i];
}

// ****************************************************************
// フレーム初期化処理
// ================================================================
void C_elm_mwnd_msg::frame_init()
{
	// すべての文字に対してフレーム初期化
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].frame_init();

	// ボタン
	for (std::map<int, C_msg_btn>::iterator itr = m_msg_btn.begin(); itr != m_msg_btn.end(); ++itr)
		itr->second.m_button.frame_init();
}

// ****************************************************************
// フレーム処理
// ================================================================
bool C_elm_mwnd_msg::frame(const S_tnm_render_param* parent_trp, C_point pos_rep, int color_rate, C_argb color, C_argb shadow_color, C_argb fuchi_color, bool draw_futoku, bool draw_shadow)
{
	// 当たってるボタンを探す
	int active_btn_no = -1;
	int active_btn_state = TNM_BTN_STATE_NORMAL;

	// グループを取得
	C_elm_group* p_group = get_group();
	if (p_group && p_group->is_doing())
	{
		for (std::map<int, C_msg_btn>::iterator itr = m_msg_btn.begin(); itr != m_msg_btn.end(); ++itr)
		{
			// アクティブな場合
			if (Gp_btn_mng->is_active(&itr->second.m_button))
			{
				active_btn_no = itr->second.m_btn_no;
				active_btn_state = Gp_btn_mng->get_hit_state(&itr->second.m_button);
				break;
			}
		}
	}

	// すべての文字に対してフレーム処理
	int moji_cnt = (int)m_moji_list.size();
	for (int i = 0; i < moji_cnt; i++)
	{
		C_point new_pos_rep = pos_rep;
		int this_color_rate = color_rate;
		C_argb this_color = color;
		C_argb this_shadow_color = shadow_color;
		C_argb this_fuchi_color = fuchi_color;

		// 縦書きの場合は起点を右上から左上に変換
		if (Gp_ini->tateyoko_mode == 1)
		{
			new_pos_rep.x -= m_moji_list[i].get_moji_size();
		}

		// ボタンの場合
		std::map<int, C_msg_btn>::iterator itr = m_msg_btn.find(i);
		if (itr != m_msg_btn.end())
		{
			// アクションを適用
			if (active_btn_no != -1 && itr->second.m_btn_no == active_btn_no)
			{
				if (0 <= itr->second.m_action_no && itr->second.m_action_no < Gp_ini->msgbtn_cnt)
				{
					int color_no = Gp_ini->msgbtn[itr->second.m_action_no].state[active_btn_state].color_no;
					if (0 <= color_no && color_no < Gp_ini->color_table_cnt)
					{
						this_color_rate = 255;
						this_color = Gp_color_table->get_color(color_no);
						this_shadow_color = Gp_color_table->get_color(Gp_ini->mwnd.shadow_color);
						this_fuchi_color = Gp_color_table->get_color(Gp_ini->mwnd.fuchi_color);
					}
				}
			}
			else
			{
				if (0 <= itr->second.m_action_no && itr->second.m_action_no < Gp_ini->msgbtn_cnt)
				{
					int color_no = Gp_ini->msgbtn[itr->second.m_action_no].state[TNM_BTN_STATE_NORMAL].color_no;
					if (0 <= color_no && color_no < Gp_ini->color_table_cnt)
					{
						this_color_rate = 255;
						this_color = Gp_color_table->get_color(color_no);
						this_shadow_color = Gp_color_table->get_color(Gp_ini->mwnd.shadow_color);
						this_fuchi_color = Gp_color_table->get_color(Gp_ini->mwnd.fuchi_color);
					}
				}
			}
		}

		// フレーム処理
		if (!m_moji_list[i].frame(parent_trp, new_pos_rep, 0, this_color_rate, this_color, this_shadow_color, this_fuchi_color, draw_futoku, draw_shadow))
			return false;
	}

	// ボタン処理
	for (std::map<int, C_msg_btn>::iterator itr = m_msg_btn.begin(); itr != m_msg_btn.end(); ++itr)
	{
		C_elm_mwnd_moji* p_moji = &m_moji_list[itr->first];

		int x = (int)parent_trp->pos_x + pos_rep.x + p_moji->get_moji_pos().x;
		int y = (int)parent_trp->pos_y + pos_rep.y + p_moji->get_moji_pos().y;
		int size = p_moji->get_moji_size();

		itr->second.m_button.set_param(C_rect::by_size(x, y, size, size), S_tnm_sorter());
	}

	return true;
}

// ****************************************************************
// テクスチャのクリア
// ================================================================
void C_elm_mwnd_msg::clear_texture()
{
	// すべての文字に対してクリア処理
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].clear_texture();
}

// ****************************************************************
// スプライトツリーを構築
// ================================================================
void C_elm_mwnd_msg::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	// すべての文字に対してスプライトツリーを構築
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].get_sprite_tree(root, root);
}

// ****************************************************************
// ボタンを登録
// ================================================================
void C_elm_mwnd_msg::regist_button()
{
	// 文字
	for (std::map<int, C_msg_btn>::iterator itr = m_msg_btn.begin(); itr != m_msg_btn.end(); ++itr)
		Gp_btn_mng->regist_button(&itr->second.m_button);
}

// ****************************************************************
// ボタンを登録
// ================================================================
void C_elm_mwnd_msg::button_event()
{
	// グループを取得
	C_elm_group* p_group = get_group();
	if (p_group && p_group->is_doing())
	{
		for (std::map<int, C_msg_btn>::iterator itr = m_msg_btn.begin(); itr != m_msg_btn.end(); ++itr)
		{
			// 当たった
			if (Gp_btn_mng->is_hit_this_frame(&itr->second.m_button))
			{
				// 効果音
				if (0 <= itr->second.m_se_no && itr->second.m_se_no < (int)Gp_btn_se_list->m_sub.size())
				{
					int se_no = Gp_btn_se_list->m_sub[itr->second.m_se_no].hit_no;
					tnm_play_se(se_no);
				}
			}
			// 当たっている
			if (Gp_btn_mng->is_hit(&itr->second.m_button))
			{
				p_group->set_hit_button_no(itr->second.m_btn_no);
			}
			// 押された
			if (Gp_btn_mng->is_pushed_this_frame(&itr->second.m_button))
			{
				// 効果音
				if (0 <= itr->second.m_se_no && itr->second.m_se_no < (int)Gp_btn_se_list->m_sub.size())
				{
					int se_no = Gp_btn_se_list->m_sub[itr->second.m_se_no].push_no;
					tnm_play_se(se_no);
				}
			}
			// 押されている
			if (Gp_btn_mng->is_pushed(&itr->second.m_button))
			{
				p_group->set_pushed_button_no(itr->second.m_btn_no);
			}
		}
	}
}

// ****************************************************************
// 所属するグループを取得
// ================================================================
C_elm_group* C_elm_mwnd_msg::get_group()
{
	if (!m_def_elm_group_list.is_empty())
	{
		if (m_btn_group_no >= 0)
		{
			S_element elm_group = m_def_elm_group_list + S_element(ELM_ARRAY, m_btn_group_no);

			// 相対パス
			if (elm_group.code[0] == ELM_UP)
			{
				S_tnm_command_proc_arg_struct arg;
				arg.disp_out_of_range_error = false;
				if (tnm_command_proc_mwnd(m_def_parent, elm_group.begin(), elm_group.begin(), elm_group.end(), &arg))
				{
					C_elm_group* p_group = (C_elm_group *)arg.p_ret_element;
					if (p_group)
					{
						return p_group;
					}
				}
			}
			// 絶対パス
			else
			{
				C_elm_group* p_group = (C_elm_group *)tnm_get_element_ptr(elm_group.begin(), elm_group.end(), false);
				if (p_group)
				{
					return p_group;
				}
			}
		}
	}

	return NULL;
}
// ****************************************************************
// 空かどうか判定
// ================================================================
bool C_elm_mwnd_msg::is_empty()
{
	return m_moji_list.empty();
}

// ****************************************************************
// メッセージのクリア
// ================================================================
void C_elm_mwnd_msg::clear_msg()
{
	m_cur.moji_pos = m_def.moji_pos;		// デフォルトの文字位置

	// パラメータの初期化（サブ）
	init_param_sub();
}

// ****************************************************************
// メッセージの追加
// ================================================================
bool is_kinsoku_moji(TCHAR moji)
{
	if (
		moji == _T('。') || moji == _T('、') || 
		moji == _T('！') || moji == _T('？') || 
		moji == _T('：') || moji == _T('；') || 
		moji == _T('」') || moji == _T('』') || 
		moji == _T('）') || moji == _T('】') || 
		moji == _T('｝') || moji == _T('］') || 
		moji == _T('〉') || moji == _T('》') || 
		moji == _T('”') || moji == _T('’') || moji == _T('〟') || 
		moji == _T('～') || moji == _T('ー') || 
		moji == _T('…') || moji == _T('‥') || moji == _T('・') || 
		moji == _T('．') || moji == _T('，') || 
		moji == _T('゛') || moji == _T('゜') ||	// ← 「゛」「゜」RealLive にない
		moji == _T('ぁ') || moji == _T('ぃ') || moji == _T('ぅ') || moji == _T('ぇ') || moji == _T('ぉ') || 
		moji == _T('っ') || moji == _T('ゃ') || moji == _T('ゅ') || moji == _T('ょ') || moji == _T('ゎ') || 
		moji == _T('ァ') || moji == _T('ィ') || moji == _T('ゥ') || moji == _T('ェ') || moji == _T('ォ') || 
		moji == _T('ッ') || moji == _T('ャ') || moji == _T('ュ') || moji == _T('ョ') || moji == _T('ヮ') ||
		moji == _T('ヵ') || moji == _T('ヶ') ||
		moji == _T('ﾞ')  || moji == _T('ﾟ')  ||
		moji == _T('｡')  || moji == _T('､')  ||
		moji == _T('!')  || moji == _T('?')  ||
		moji == _T(':')  || moji == _T(';')  ||
		moji == _T('｣')  || moji == _T(')')  ||
		moji == _T(']')  || moji == _T('>')  || moji == _T('}') ||
		moji == _T('\'') || moji == _T('"')  ||  // ← 「"」RealLive にない
		moji == _T('ｰ')  || moji == _T('･')  ||
		moji == _T('.')  || moji == _T(',')  ||
		moji == _T('ｧ')  || moji == _T('ｨ')  || moji == _T('ｩ') || moji == _T('ｪ') || moji == _T('ｫ') || 
		moji == _T('ｯ')  || moji == _T('ｬ')  || moji == _T('ｭ') || moji == _T('ｮ')
	)	{
		return true;
	}

	return false;
}

void C_elm_mwnd_msg::add_msg_sub(CTSTR& msg, TSTR* ret_over_flow_msg)
{
	if (msg.empty())
		return;

	// 文字の領域を予約しておく（確保ではない！）
	m_moji_list.reserve(m_moji_list.size() + msg.size());

	for (TSTR::const_iterator itr = msg.begin(); itr != msg.end();)
	{
		int moji_type = TNM_MOJI_TYPE_MOJI;
		int moji_code = -1;

		// 現在のイテレータを保存しておく
		TSTR::const_iterator old_itr = itr;

		// 絵文字判定
		tnm_msg_proc_expand_name_flag(itr, msg.end(), &moji_type, &moji_code);

		// 表示する文字の場合
		if (moji_code == _T('\n'))
		{
			new_line_indent();			// 強制的に改行（インデントあり）
		}
		else if (moji_code == _T('\a'))
		{
			new_line_no_indent();		// 強制的に改行（インデントなし）
		}
		else if (!(moji_type == TNM_MOJI_TYPE_MOJI && moji_code == 0))
		{
			bool auto_indent_flag = false;

			// 文字の大きさを取得
			int this_moji_size = 0;
			if (moji_type != TNM_MOJI_TYPE_MOJI)
			{
				this_moji_size = m_cur.moji_size;
			}
			else
			{
				this_moji_size = is_hankaku((TCHAR)moji_code) ? m_cur.moji_size / 2 : m_cur.moji_size;
			}

			// 自動改行の判定用のサイズ
			int this_check_size = this_moji_size + m_cur.moji_space.column();

#if defined (__SIGLUS_EN) || defined (__SIGLUS_ES) || defined (__SIGLUS_DE) || defined (__SIGLUS_ID) || defined (__SIGLUS_FR)

			// 英単語による自動改行
			if (moji_code == _T(' '))
			{
				for (TSTR::const_iterator itr_2 = itr; itr_2 != msg.end(); ++itr_2)
				{
					if (is_hankaku(*itr_2) && *itr_2 != _T(' '))
					{
						this_check_size += m_cur.moji_size / 2 + m_cur.moji_space.column();
					}
					else
					{
						break;
					}
				}
			}
#endif

			// 行に入りきらない場合は改行
			if (Gp_ini->tateyoko_mode == 1)
			{
				if (m_cur.moji_pos.y + this_check_size > m_msg_size.cy + m_def.moji_size)
				{
					new_line_indent();			// 禁則分を飛び越えたので必ず改行
					auto_indent_flag = true;	// 自動インデントフラグ
				}
				else if (m_cur.moji_pos.y + this_check_size > m_msg_size.cy)
				{
					if (!is_kinsoku_moji(moji_code))
					{
						new_line_indent();			// 禁則文字でなかったので改行
						auto_indent_flag = true;	// 自動インデントフラグ
					}
				}
			}
			else
			{
				if (m_cur.moji_pos.x + this_check_size > m_msg_size.cx + m_def.moji_size)
				{
					new_line_indent();			// 禁則分を飛び越えたので必ず改行
					auto_indent_flag = true;	// 自動インデントフラグ
				}
				else if (m_cur.moji_pos.x + this_check_size > m_msg_size.cx)
				{
					if (!is_kinsoku_moji(moji_code))
					{
						new_line_indent();			// 禁則文字でなかったので改行
						auto_indent_flag = true;	// 自動インデントフラグ
					}
				}
			}

			// 自動インデント直後のスペースは無視する
			if (auto_indent_flag)
			{
				if (moji_type == TNM_MOJI_TYPE_MOJI && (moji_code == _T(' ') || moji_code == _T('　')))
				{
					continue;
				}
			}

			// 行数を超えた場合は「改ページ」（オーバーフロー）
			if (Gp_ini->tateyoko_mode == 1)
			{
				if (m_cur.moji_pos.x <= -m_msg_size.cx)
				{
					// オーバーフローメッセージを設定
					if (ret_over_flow_msg)
					{
						*ret_over_flow_msg = TSTR(old_itr, msg.end());	// 表示しなかったので古いイテレータを指定
					}

					break;
				}
			}
			else
			{
				if (m_cur.moji_pos.y >= m_msg_size.cy)
				{
					// オーバーフローメッセージを設定
					if (ret_over_flow_msg)
					{
						*ret_over_flow_msg = TSTR(old_itr, msg.end());	// 表示しなかったので古いイテレータを指定
					}

					break;
				}
			}

			// 文字を作成
			C_elm_mwnd_moji moji;
			int moji_color_no = m_cur.moji_color != -1 ? m_cur.moji_color : (Gp_config->message_chrcolor_flag && m_chara_moji_color_no != -1) ? m_chara_moji_color_no : m_def.moji_color;
			int shadow_color_no = m_cur.shadow_color != -1 ? m_cur.shadow_color : (Gp_config->message_chrcolor_flag && m_chara_shadow_color_no != -1) ? m_chara_shadow_color_no : m_def.shadow_color;
			int fuchi_color_no = m_cur.fuchi_color != -1 ? m_cur.fuchi_color : (Gp_config->message_chrcolor_flag && m_chara_fuchi_color_no != -1) ? m_chara_fuchi_color_no : m_def.fuchi_color;
			moji.set_moji(moji_type, moji_code, m_cur.moji_size, moji_color_no, shadow_color_no, fuchi_color_no, m_cur.moji_pos + m_cur.moji_rep_pos);

			// ルビの開始位置を設定
			if (m_ruby_start_ready)
			{
				m_ruby_start_pos = m_cur.moji_pos;
				m_ruby_start_ready = false;
			}

			// 次の文字位置の設定
			if (Gp_ini->tateyoko_mode == 1)
			{
				m_cur.moji_pos.y += this_moji_size + m_cur.moji_space.column();
			}
			else
			{
				m_cur.moji_pos.x += this_moji_size + m_cur.moji_space.column();
			}

			// 括弧の場合は自動でインデントを設定
			if (moji_type == TNM_MOJI_TYPE_MOJI && (moji_code == _T('「') || moji_code == _T('『') || moji_code == _T('（')))
			{
				// １文字目の場合は実際にインデントを設定
				if (m_line_head_flag)	{
					set_indent(moji_code);
				}
				// 現在のインデント文字と同じ文字の場合はインデントカウンタを増やす
				else if (moji_code == m_indent_moji)	{
					m_indent_cnt ++;
				}
			}

			// インデント解除文字の判定
			if (m_indent_cnt > 0)
			{
				if (moji_type == TNM_MOJI_TYPE_MOJI && m_indent_moji == _T('「') && moji_code == _T('」'))	{
					m_indent_cnt --;
				}
				if (moji_type == TNM_MOJI_TYPE_MOJI && m_indent_moji == _T('『') && moji_code == _T('』'))	{
					m_indent_cnt --;
				}
				if (moji_type == TNM_MOJI_TYPE_MOJI && m_indent_moji == _T('（') && moji_code == _T('）'))	{
					m_indent_cnt --;
				}

				// インデントカウントが０になればインデントを解除する
				if (m_indent_cnt == 0)	{
					clear_indent();
				}
			}

			// ボタンとして登録
			if (m_button_flag)
			{
				C_msg_btn btn;
				btn.m_button.create_guid();
				btn.m_btn_no = m_btn_no;
				btn.m_action_no = m_btn_action_no;
				btn.m_se_no = m_btn_se_no;
				m_msg_btn.insert(std::pair<int, C_msg_btn>((int)m_moji_list.size(), btn));
			}

			// 文字リストに追加
			m_moji_list.push_back(moji);

			// デバッグ文字リストに追加
			if (moji_type == TNM_MOJI_TYPE_MOJI)
			{
				if (moji_code != _T('\n') && moji_code != _T('\a'))
				{
					m_debug_msg.push_back(moji_code);		// メッセージに文字を保存
				}
			}
			else
			{
				m_debug_msg += TSTR(old_itr, itr);		// 絵文字は展開せずに保存
			}

			// 行頭フラグをおろす
			m_line_head_flag = false;
		}
	}
}

// メッセージが入るかをチェック
// ノベルモードにおいて改ページを行うかの判定に使う
// new_line_flag: 改行を加えて判定する（キー待ちアイコン／ページ待ちアイコンの判定に使う）
bool C_elm_mwnd_msg::add_msg_check(int check_size, bool new_line_flag)
{
	if (Gp_ini->tateyoko_mode == 1)
	{
		int cur_x = m_cur.moji_pos.x;
		int cur_y = m_cur.moji_pos.y;

		// 行に入りきらない場合は改行
		if (cur_y + m_cur.moji_size > m_msg_size.cy + m_def.moji_size)	{
			cur_x -= m_cur.moji_size + m_cur.moji_space.row();
		}

		// 改行を入れて判定する
		if (new_line_flag)	{
			cur_x -= m_cur.moji_size + m_cur.moji_space.row();
		}

		// 行数を超えた場合は入りきらない！（早めに判定する！）
		if (cur_x <= - m_msg_size.cx + check_size)	{
			return false;
		}
	}
	else
	{
		int cur_x = m_cur.moji_pos.x;
		int cur_y = m_cur.moji_pos.y;

		// 行に入りきらない場合は改行
		if (cur_x + m_cur.moji_size > m_msg_size.cx + m_def.moji_size)	{
			cur_y += m_cur.moji_size + m_cur.moji_space.cy;
		}

		// 改行を入れて判定する
		if (new_line_flag)	{
			cur_y += m_cur.moji_size + m_cur.moji_space.cy;
		}

		// 行数を超えた場合は入りきらない！（早めに判定する！）
		if (cur_y >= m_msg_size.cy - check_size)	{
			return false;
		}
	}

	return true;
}

void C_elm_mwnd_msg::add_msg(CTSTR& msg, TSTR* ret_over_flow_msg)
{
	// 前回のメッセージタイプと比較して違えばスペースを空ける
	if (!m_cur_msg_type_decided)	{
		if (m_cur_msg_type == 1)	{	// 名前ありメッセージだった！
			m_cur.moji_pos.y += m_def.talk_margin.bottom;
		}
	}

	// メッセージを追加
	add_msg_sub(msg, ret_over_flow_msg);

	// メッセージタイプ：ト書き
	if (!m_cur_msg_type_decided)	{
		m_cur_msg_type = 0;
		m_cur_msg_type_decided = true;
	}
}

void C_elm_mwnd_msg::add_name_to_msg(CTSTR& name, TSTR* ret_over_flow_msg)
{
	// 前回のメッセージタイプと比較して違えばスペースを空ける
	if (!m_cur_msg_type_decided)	{
		if (m_cur_msg_type == 0)	{	// 名前なしメッセージだった！
			m_cur.moji_pos.y += m_def.talk_margin.top;
		}
	}

	// 名前をメッセージとして追加
	int old_moji_size = m_cur.moji_size;
	m_cur.moji_size = m_cur.name_moji_size;
	add_msg_sub(name, ret_over_flow_msg);
	if (m_def.name_newline == 1)	{
		m_cur.moji_pos.x = m_def.talk_margin.left;
		set_indent(0);			// メッセージのインデントを設定する
		new_line_indent();		// 名前の後に改行する
	}
	m_cur.moji_size = old_moji_size;

	// メッセージタイプ：名前ありメッセージ
	if (!m_cur_msg_type_decided)	{
		m_cur_msg_type = 1;
		m_cur_msg_type_decided = true;
	}
}

// ****************************************************************
// 文字数を取得
// ================================================================
int C_elm_mwnd_msg::get_moji_cnt()
{
	return (int)m_moji_list.size();
}
int C_elm_mwnd_msg::get_disp_moji_cnt()
{
	return m_disp_moji_cnt;
}
int C_elm_mwnd_msg::get_hide_moji_cnt()
{
	return m_hide_moji_cnt;
}

// ****************************************************************
// 次の１文字表示
// ================================================================
void C_elm_mwnd_msg::disp_one_moji()
{
	if (m_disp_moji_cnt < (int)m_moji_list.size())	{
		m_moji_list[m_disp_moji_cnt].set_appear(true);
		m_disp_moji_cnt ++;		// 表示した文字数

		// 次がルビならさらに表示する
		while (m_disp_moji_cnt < (int)m_moji_list.size() && m_moji_list[m_disp_moji_cnt].get_ruby_flag())	{
			m_moji_list[m_disp_moji_cnt].set_appear(true);
			m_disp_moji_cnt ++;		// 表示した文字数
		}
	}
}

// ****************************************************************
// 全ての文字を表示
// ================================================================
void C_elm_mwnd_msg::disp_all_moji()
{
	// 表示していない文字を順に表示する
	for (int i = m_disp_moji_cnt; i < (int)m_moji_list.size(); i++)	{
		m_moji_list[i].set_appear(true);
		m_disp_moji_cnt ++;
	}
}

// ****************************************************************
// 行の先頭にする
// ================================================================
//		インデント文字の判定が改めて行われるようになります。
// ================================================================
void C_elm_mwnd_msg::set_line_head()
{
	m_line_head_flag = true;
}

// ****************************************************************
// 改行（インデントあり）
// ================================================================
//		必ず改行します。インデントを行います。
// ================================================================
void C_elm_mwnd_msg::new_line_indent()
{
	if (Gp_ini->tateyoko_mode == 1)
	{
		// Ｙ座標をインデントの位置に設定
		m_cur.moji_pos.y = m_indent_pos;
		// Ｘ座標は次の行へ
		m_cur.moji_pos.x -= m_cur.moji_size + m_cur.moji_space.row();

		// メッセージとして保存
		m_debug_msg += _T("\n") + TSTR(_T("　")) * ((m_cur.moji_pos.y - m_def.moji_pos.y) / m_def.moji_size);
	}
	else
	{
		// Ｘ座標をインデントの位置に設定
		m_cur.moji_pos.x = m_indent_pos;
		// Ｙ座標は次の行へ
		m_cur.moji_pos.y += m_cur.moji_size + m_cur.moji_space.row();

		// メッセージとして保存
		m_debug_msg += _T("\n") + TSTR(_T("　")) * ((m_cur.moji_pos.x - m_def.moji_pos.x) / m_def.moji_size);
	}
}

// ****************************************************************
// 改行（インデントなし）
// ================================================================
//		必ず改行します。インデントを行いません。
// ================================================================
void C_elm_mwnd_msg::new_line_no_indent()
{
	clear_indent();		// インデントを消してから
	new_line_indent();	// 改行する

	// 改行直後なので行頭フラグを立てます。
	m_line_head_flag = true;
}

// ****************************************************************
// 改行（先頭でない場合のみ）
// ================================================================
//		選択肢命令の直前などに使います。
//		現在位置が先頭にない場合のみ改行を行います。
//		インデントを行いません。
// ================================================================
void C_elm_mwnd_msg::new_line_if_not_line_head()
{
	if (Gp_ini->tateyoko_mode == 1)
	{
		// 先頭でない場合
		if (m_cur.moji_pos.y != m_def.moji_pos.y)
			new_line_no_indent();
	}
	else
	{
		// 先頭でない場合
		if (m_cur.moji_pos.x != m_def.moji_pos.x)
			new_line_no_indent();
	}
}

// ****************************************************************
// インデントを設定
// ================================================================
void C_elm_mwnd_msg::set_indent(TCHAR moji)
{
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_indent_pos = m_cur.moji_pos.y;
		m_indent_moji = moji;
		m_indent_cnt = 1;
	}
	else
	{
		m_indent_pos = m_cur.moji_pos.x;
		m_indent_moji = moji;
		m_indent_cnt = 1;
	}
}

// ****************************************************************
// インデントをクリア
// ================================================================
void C_elm_mwnd_msg::clear_indent()
{
	m_indent_pos = 0;
	m_indent_moji = 0;
	m_indent_cnt = 0;
}

// ****************************************************************
// 文字補正位置を設定
// ================================================================
void C_elm_mwnd_msg::set_moji_rep_pos(C_point pos)
{
	m_cur.moji_rep_pos = pos;
}
void C_elm_mwnd_msg::set_moji_rep_pos_default()
{
	m_cur.moji_rep_pos = m_def.moji_rep_pos;
}

// ****************************************************************
// 文字サイズを設定
// ================================================================
void C_elm_mwnd_msg::set_moji_size(int size)
{
	m_cur.moji_size = size;
}
void C_elm_mwnd_msg::set_moji_size_default()
{
	m_cur.moji_size = m_def.moji_size;
}

// ****************************************************************
// 文字色を設定
// ================================================================
void C_elm_mwnd_msg::set_moji_color(int color_no)
{
	m_cur.moji_color = color_no;
}
void C_elm_mwnd_msg::set_moji_color_default()
{
	m_cur.moji_color = -1;		// デフォルトに従う
}

// ****************************************************************
// キャラクタ文字色をクリア
// ================================================================
void C_elm_mwnd_msg::clear_chara_moji_color()
{
	m_chara_moji_color_no = -1;
}
void C_elm_mwnd_msg::clear_chara_shadow_color()
{
	m_chara_shadow_color_no = -1;
}
void C_elm_mwnd_msg::clear_chara_fuchi_color()
{
	m_chara_fuchi_color_no = -1;
}

// ****************************************************************
// キャラクタ文字色を設定
// ================================================================
void C_elm_mwnd_msg::set_chara_moji_color(int color_no)
{
	m_chara_moji_color_no = color_no;
}
void C_elm_mwnd_msg::set_chara_shadow_color(int color_no)
{
	m_chara_shadow_color_no = color_no;
}
void C_elm_mwnd_msg::set_chara_fuchi_color(int color_no)
{
	m_chara_fuchi_color_no = color_no;
}

// ****************************************************************
// 現在の文字位置を取得
// ================================================================
C_point C_elm_mwnd_msg::get_cur_moji_pos()
{
	return m_cur.moji_pos;
}

// ****************************************************************
// 現在の文字位置を設定
// ================================================================
void C_elm_mwnd_msg::set_cur_moji_pos(C_point pos)
{
	m_cur.moji_pos = pos;
}

// ****************************************************************
// ボタンを開始
// ================================================================
void C_elm_mwnd_msg::start_button(int btn_no, int group_no, int action_no, int se_no)
{
	m_button_flag = true;
	m_btn_no = btn_no;
	m_btn_group_no = group_no;
	m_btn_action_no = action_no;
	m_btn_se_no = se_no;
}

// ****************************************************************
// ボタンを終了
// ================================================================
void C_elm_mwnd_msg::end_button()
{
	m_button_flag = false;
}

// ****************************************************************
// ルビを開始
// ================================================================
void C_elm_mwnd_msg::start_ruby(CTSTR& str)
{
	m_ruby = str;

	m_ruby_start_ready = true;
}

// ****************************************************************
// ルビを終了
// ================================================================
void C_elm_mwnd_msg::end_ruby()
{
	// ルビがあったらルビを生成！
	if (!m_ruby.empty())
	{
		// ルビ開始準備が立ったままの場合
		if (m_ruby_start_ready)
		{
			tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("ルビの対象の文字列が空です。"));
		}
		else
		{
			C_point ruby_start_pos = m_ruby_start_pos;
			C_point ruby_end_pos = m_cur.moji_pos;

			// ルビはもう縦書き／横書きで完全に分離します
			if (Gp_ini->tateyoko_mode == 1)
			{
				// ルビが改行にまたがっていた場合
				if (ruby_start_pos.x != ruby_end_pos.x)
				{
					tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("ルビの途中で改行されています。ルビは表示されません。"));
				}
				else
				{
					// ルビを均等割りする！
					int msg_height = ruby_end_pos.y - ruby_start_pos.y;
					int ruby_moji_cnt = (int)m_ruby.size();
					int ruby_moji_height = m_cur.ruby_size;
					int ruby_moji_space_y = (msg_height - (ruby_moji_height * ruby_moji_cnt)) / (ruby_moji_cnt + 1);

					// ルビの位置を設定する
					C_point pos = ruby_start_pos + C_point(m_cur.ruby_size + m_cur.ruby_space, ruby_moji_space_y);

					// 文字スペースが 0 以下になったら補正する
					if (ruby_moji_space_y < 0)
					{
						ruby_moji_space_y = 0;
						int ruby_total_height = ruby_moji_cnt * ruby_moji_height;
						pos.y = ruby_start_pos.y + (msg_height / 2) - (ruby_moji_height / 2);

						// 上端を越えたら上詰め
						if (pos.y < 0)
							pos.y = ruby_start_pos.y;

						// 下端を越えたら下詰め
						if (pos.y + ruby_total_height >= m_msg_size.cy + m_def.moji_size)
							pos.y = ruby_start_pos.y + msg_height - ruby_total_height;
					}

					m_debug_msg += _T(" [");
					for (TSTR::const_iterator itr = m_ruby.begin(); itr != m_ruby.end(); ++itr)
					{
						TCHAR moji_code = *itr;

						// 文字を作成
						C_elm_mwnd_moji moji;
						int hankaku_rep_y = is_hankaku(moji_code) ? ruby_moji_height / 4 : 0;	// 半角ならセンタリング
						int moji_color_no = (m_cur.moji_color != -1 ? m_cur.moji_color : m_chara_moji_color_no != -1 ? m_chara_moji_color_no : m_def.moji_color);
						int shadow_color_no = (m_cur.shadow_color != -1 ? m_cur.shadow_color : m_chara_shadow_color_no != -1 ? m_chara_shadow_color_no : m_def.shadow_color);
						int fuchi_color_no = (m_cur.fuchi_color != -1 ? m_cur.fuchi_color : m_chara_fuchi_color_no != -1 ? m_chara_fuchi_color_no : m_def.fuchi_color);
						moji.set_moji(TNM_MOJI_TYPE_MOJI, moji_code, ruby_moji_height, moji_color_no, shadow_color_no, fuchi_color_no, pos + C_point(0, hankaku_rep_y));
						moji.set_ruby_flag(true);

						// 文字リストに追加
						m_moji_list.push_back(moji);
						m_debug_msg.push_back(moji_code);

						// 次の位置へ
						pos += C_point(0, ruby_moji_height + ruby_moji_space_y);
					}
					m_debug_msg += _T("] ");
				}
			}
			else
			{
				// ルビが改行にまたがっていた場合
				if (ruby_start_pos.y != ruby_end_pos.y)
				{
					tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("ルビの途中で改行されています。ルビは表示されません。"));
				}
				else
				{
					// ルビを均等割りする！
					int msg_width = ruby_end_pos.x - ruby_start_pos.x;
					int ruby_moji_cnt = (int)m_ruby.size();
					int ruby_moji_width = m_cur.ruby_size;
					int ruby_moji_space_x = (msg_width - (ruby_moji_width * ruby_moji_cnt)) / (ruby_moji_cnt + 1);

					// ルビの位置を設定する
					C_point pos = ruby_start_pos + C_point(ruby_moji_space_x, -m_cur.ruby_size - m_cur.ruby_space);

					// 文字スペースが 0 以下になったら補正する
					if (ruby_moji_space_x < 0)
					{
						ruby_moji_space_x = 0;
						int ruby_total_width = ruby_moji_cnt * ruby_moji_width;
						pos.x = ruby_start_pos.x + (msg_width / 2) - (ruby_total_width / 2);

						// 左端を越えたら左詰め
						if (pos.x < 0)
							pos.x = ruby_start_pos.x;

						// 右端を越えたら右詰め
						if (pos.x + ruby_total_width >= m_msg_size.cx + m_def.moji_size)
							pos.x = ruby_start_pos.x + msg_width - ruby_total_width;
					}

					m_debug_msg += _T(" [");
					for (TSTR::const_iterator itr = m_ruby.begin(); itr != m_ruby.end(); ++itr)
					{
						TCHAR moji_code = *itr;

						// 文字を作成
						C_elm_mwnd_moji moji;
						int hankaku_rep_x = is_hankaku(moji_code) ? ruby_moji_width / 4 : 0;	// 半角ならセンタリング
						int moji_color_no = (m_cur.moji_color != -1 ? m_cur.moji_color : m_chara_moji_color_no != -1 ? m_chara_moji_color_no : m_def.moji_color);
						int shadow_color_no = (m_cur.shadow_color != -1 ? m_cur.shadow_color : m_chara_shadow_color_no != -1 ? m_chara_shadow_color_no : m_def.shadow_color);
						int fuchi_color_no = (m_cur.fuchi_color != -1 ? m_cur.fuchi_color : m_chara_fuchi_color_no != -1 ? m_chara_fuchi_color_no : m_def.fuchi_color);
						moji.set_moji(TNM_MOJI_TYPE_MOJI, moji_code, ruby_moji_width, moji_color_no, shadow_color_no, fuchi_color_no, pos + C_point(hankaku_rep_x, 0));
						moji.set_ruby_flag(true);

						// 文字リストに追加
						m_moji_list.push_back(moji);
						m_debug_msg.push_back(moji_code);

						// 次の位置へ
						pos += C_point(ruby_moji_width + ruby_moji_space_x, 0);
					}
					m_debug_msg += _T("] ");
				}
			}
		}
	}

	// ルビをクリア
	m_ruby.clear();

	m_ruby_start_pos = C_point(0, 0);
}

// ****************************************************************
// スライドメッセージ
// ================================================================
void C_elm_mwnd_msg::slide_msg()
{
	int slide_pixel = m_def.moji_size + m_def.moji_space.cy;

	// 既に表示した全文字をずらす
	int moji_cnt = m_moji_list.size();
	for (int i = 0; i < moji_cnt; i++)
	{
		C_point moji_pos = m_moji_list[i].get_moji_pos();
		m_moji_list[i].set_moji_pos(moji_pos - C_point(0, slide_pixel));
	}

	// 現在の文字位置をずらす
	m_cur.moji_pos.y -= slide_pixel;
}

// ****************************************************************
// まだ出現していない文字が存在するか判定する
// ================================================================
bool C_elm_mwnd_msg::check_not_appear_moji()
{
	return (int)m_moji_list.size() > m_disp_moji_cnt;
}

// ****************************************************************
// メッセージサイズの取得
// ================================================================
C_size C_elm_mwnd_msg::get_msg_size()
{
	return m_msg_size;
}

// ****************************************************************
// 表示したメッセージの高さを求める
// ================================================================
//		メッセージバックの際、
//		どこまで文章を表示するかを判定するのに使います。
// ================================================================
int C_elm_mwnd_msg::get_msg_height()
{
	if (Gp_ini->tateyoko_mode == 1)
	{
		return -(m_cur.moji_pos.x + m_def.moji_space.cx);
	}
	else
	{
		return m_cur.moji_pos.y - m_def.moji_space.cy;
	}
}

// ****************************************************************
// デバッグメッセージの取得
// ================================================================
TSTR C_elm_mwnd_msg::get_debug_msg()
{
	return m_debug_msg;
}

// ****************************************************************
// パラメータ処理：文字数を初期化／取得／設定
// ================================================================
void C_elm_mwnd_msg::init_moji_cnt()
{
	m_cur.moji_cnt = m_def.moji_cnt;
}
void C_elm_mwnd_msg::set_moji_cnt(C_size moji_cnt)
{
	m_cur.moji_cnt = moji_cnt;

	// メッセージ領域を大きさ計算する（文字数だけは現在の値を使う）
	m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.cx) + (m_def.moji_space.cx * (m_cur.moji_cnt.cx - 1));
	m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.cy) + (m_def.moji_space.cy * (m_cur.moji_cnt.cy - 1));
}


