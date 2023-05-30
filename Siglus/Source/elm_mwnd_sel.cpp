#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_input.h"
#include	"data/tnm_save_00.h"

#include	"element/elm_mwnd_sel.h"
#include	"element/elm_mwnd_moji.h"
#include	"element/elm_group_def.h"
#include	"element/elm_sound.h"

#include	"engine/eng_input.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_scene.h"


// ****************************************************************
// 初期化
// ================================================================
void C_elm_mwnd_select::init()
{
	// テンプレート番号
	m_template_no = -1;

	// テンプレートの初期化
	m_def.moji_cnt = C_size(0, 0);
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_size = 0;
	m_def.moji_space = C_size(0, 0);
	m_def.moji_color = 0;
	m_def.shadow_color = 0;
	m_def.fuchi_color = 0;

	// 処理変数初期化
	init_work_variable();
}

// ****************************************************************
// 処理変数初期化
// ================================================================
void C_elm_mwnd_select::init_work_variable()
{
	m_cur = m_def;		// デフォルトパラメータを現在のパラメータへコピーする
	m_item_list.clear();
	m_disp_item_cnt = 0;
	m_cancel_enable_flag = false;
}

// ****************************************************************
// テンプレートの値を設定
// ================================================================
void C_elm_mwnd_select::set_template(int template_no)
{
	if (template_no == m_template_no)
		return;

	// テンプレート番号
	m_template_no = template_no;

	// テンプレートを再構築
	restruct_template();

	// 処理変数初期化
	init_work_variable();
}

// ****************************************************************
// テンプレートを再構築
// ================================================================
void C_elm_mwnd_select::restruct_template()
{
	m_def.moji_cnt = Gp_ini->mwnd.sub[m_template_no].moji_cnt;
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_size = Gp_ini->mwnd.sub[m_template_no].moji_size;
	m_def.moji_space = Gp_ini->mwnd.sub[m_template_no].moji_space;
	m_def.moji_color = Gp_ini->mwnd.sub[m_template_no].moji_color >= 0 ? Gp_ini->mwnd.sub[m_template_no].moji_color : Gp_ini->mwnd.moji_color;
	m_def.shadow_color = Gp_ini->mwnd.sub[m_template_no].shadow_color >= 0 ? Gp_ini->mwnd.sub[m_template_no].shadow_color : Gp_ini->mwnd.shadow_color;
	m_def.fuchi_color = Gp_ini->mwnd.sub[m_template_no].fuchi_color >= 0 ? Gp_ini->mwnd.sub[m_template_no].fuchi_color : Gp_ini->mwnd.fuchi_color;
}

// ****************************************************************
// セーブ
// ================================================================
void C_elm_mwnd_select::save(C_tnm_save_stream& stream)
{
	// テンプレート番号
	stream.save(m_template_no);

	// 現在のパラメータ
	stream.save(m_cur);
	stream.save(m_disp_item_cnt);
	stream.save(m_cancel_enable_flag);

	// アイテム
	stream.save<size_t>(m_item_list.size());
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].save(stream);
}

// ****************************************************************
// ロード
// ================================================================
void C_elm_mwnd_select::load(C_tnm_save_stream& stream)
{
	// テンプレート番号
	stream.load(m_template_no);

	// テンプレートを再構築
	restruct_template();

	// 現在のパラメータ
	stream.load(m_cur);
	stream.load(m_disp_item_cnt);
	stream.load(m_cancel_enable_flag);

	// アイテム
	m_item_list.resize(stream.load_ret<size_t>());
	for (int i = 0; i < (int)m_item_list.size(); i++)	{
		m_item_list[i].init();	// 初期化！
		m_item_list[i].load(stream);
	}
}

// ****************************************************************
// コピー
// ================================================================
void C_elm_mwnd_select::copy(C_elm_mwnd_select* src)
{
	// テンプレート番号
	m_template_no = src->m_template_no;
		
	// テンプレートを再構築
	restruct_template();

	// 現在のパラメータ
	m_cur = src->m_cur;
	m_disp_item_cnt = src->m_disp_item_cnt;
	m_cancel_enable_flag = src->m_cancel_enable_flag;
	
	// アイテム
	m_item_list.resize(src->m_item_list.size());
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].copy(&src->m_item_list[i]);
}

// ****************************************************************
// フレーム初期化処理
// ================================================================
void C_elm_mwnd_select::frame_init()
{
	// アイテム
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].frame_init();
}

// ****************************************************************
// フレーム処理
// ================================================================
void C_elm_mwnd_select::frame(const S_tnm_render_param* parent_trp, C_point pos_rep, bool draw_futoku, bool draw_shadow)
{
	// アイテム
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].frame(parent_trp, pos_rep, 0, draw_futoku, draw_shadow);
}

// ****************************************************************
// スプライトツリーを構築
// ================================================================
void C_elm_mwnd_select::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	// アイテム
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].get_sprite_tree(root);
}

// ****************************************************************
// ボタンを登録
// ================================================================
void C_elm_mwnd_select::regist_button()
{
	// アイテム
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].regist_button();
}

// ****************************************************************
// ボタンイベント処理
// ================================================================
void C_elm_mwnd_select::button_event(bool wait_flag)
{
	int sel_no = TNM_GROUP_NOT_DECIDED;

	// キャンセルされた場合
	if (m_cancel_enable_flag && tnm_input_use_key_down_up(VK_EX_CANCEL))
	{
		sel_no = TNM_GROUP_CANCELED;
	}
	else
	{
		// アイテム
		for (int i = 0; i < (int)m_item_list.size(); i++)
		{
			if (m_item_list[i].button_event(wait_flag))
			{
				sel_no = i;
			}
		}
	}

	// 決定またはキャンセルされた！
	if (sel_no == TNM_GROUP_CANCELED || sel_no >= 0)
	{
		// 効果音
		if (sel_no == TNM_GROUP_CANCELED)
		{
			tnm_play_se(TNM_SE_TYPE_CANCEL);

			Gp_global->last_sel_msg = _T("（キャンセル）");

		}
		else
		{
			tnm_play_se(TNM_SE_TYPE_DECIDE);

			Gp_global->last_sel_msg = m_item_list[sel_no].get_msg();
		}

		// 決定待ちの場合
		if (wait_flag)
		{
			tnm_stack_push_int(sel_no);	// 結果をスタックに積む
			tnm_pop_proc();				// 次のプロセスへ
		}

		// 選択肢ポイントを立てる
		tnm_set_sel_point();
	}
}

// ****************************************************************
// 空かどうか判定
// ================================================================
bool C_elm_mwnd_select::is_empty()
{
	return m_item_list.empty();
}

// ****************************************************************
// 選択肢のクリア
// ================================================================
void C_elm_mwnd_select::clear_sel()
{
	m_cur.moji_pos = C_point(0, 0);
	m_item_list.clear();
	m_disp_item_cnt = 0;
	m_cancel_enable_flag = false;
}

// ****************************************************************
// 選択肢の初期位置を設定する
// ================================================================
void C_elm_mwnd_select::set_def_pos(C_point pos)
{
	m_def.moji_pos = pos;
	m_cur.moji_pos = pos;
}

// ****************************************************************
// 選択肢を追加
// ================================================================
void C_elm_mwnd_select::add_sel(CTSTR& msg, int item_type, int color)
{
	// ここで色を変換
	color = (color >= 0) ? color : m_def.moji_color;

	// アイテムを作成
	C_elm_mwnd_select_item item;
	item.init();	// 初期化！
	item.set_msg(msg, item_type, m_cur.moji_size, m_cur.moji_space, color, m_cur.shadow_color, m_cur.fuchi_color);

	// アイテムリストに追加
	m_item_list.push_back(item);
}

// ****************************************************************
// キャンセル可能フラグを設定
// ================================================================
void C_elm_mwnd_select::set_cancel_enable(bool enable)
{
	m_cancel_enable_flag = enable;
}

// ****************************************************************
// 選択肢を再構築
// ================================================================
void C_elm_mwnd_select::restruct_item(int extend_type)
{
	// ================================================================
	// 固定枠の場合
	// ----------------------------------------------------------------
	//		整理して並べます。
	// ----------------------------------------------------------------
	//		１．あああ			４．ええ
	//		２．いいいいいい	５．おおおお
	//		３．うううう
	// ----------------------------------------------------------------
	if (extend_type == 0)	{

		// テキスト矩形を計算
		C_size text_area_size;
		text_area_size.cx = (m_def.moji_size * m_def.moji_cnt.cx) + (m_def.moji_space.cx * (m_def.moji_cnt.cx - 1));
		text_area_size.cy = (m_def.moji_size * m_def.moji_cnt.cy) + (m_def.moji_space.cy * (m_def.moji_cnt.cy - 1));

		// 選択肢の数
		int item_cnt = (int)m_item_list.size();

		// 縦に並ぶ項目数の計算
		int line_cnt = (text_area_size.cy - m_cur.moji_pos.y + m_cur.moji_space.cy) / (m_cur.moji_size + m_cur.moji_space.cy);
		line_cnt = std::max(1, line_cnt);

		// 横に並ぶ項目数の計算
		int column_cnt = (item_cnt - 1) / line_cnt + 1;

		// １つの選択肢の大きさの計算
		int sel_width = text_area_size.cx / column_cnt;
		int sel_height = m_cur.moji_size + m_cur.moji_space.cy;

		// 選択矩形と表示座標を再設定
		int pos_no = 0;
		for (int i = 0; i < (int)m_item_list.size(); i++)	{

			// type == off の場合は何もしない
			if (m_item_list[i].get_item_type() != TNM_SEL_ITEM_TYPE_OFF)	{
				int sel_rect_x = pos_no / line_cnt * sel_width;
				int sel_rect_y = pos_no % line_cnt * sel_height + m_def.moji_pos.y;
				m_item_list[i].set_pos(C_point(sel_rect_x, sel_rect_y));
				m_item_list[i].set_hit_rect(C_rect::by_size(sel_rect_x, sel_rect_y, sel_width, sel_height));
					// 一旦、テキスト座標で当たり判定領域を計算します。

				// 次の位置へ
				pos_no ++;
			}
		}
	}

	// ================================================================
	// 可変枠の場合
	// ----------------------------------------------------------------
	// 縦にずらーっと並べます。全体矩形を計算します。
	// ----------------------------------------------------------------
	else if (extend_type == 1)	{

		// 選択肢の数
		int item_cnt = (int)m_item_list.size();

		// メッセージサイズを計算する（可変枠のサイズ調整に使います）
		m_msg_size = C_size(0, m_def.moji_pos.y - m_cur.moji_space.cy);
		for (int i = 0; i < item_cnt; i++)	{

			// type == off の場合は何もしない
			if (m_item_list[i].get_item_type() != TNM_SEL_ITEM_TYPE_OFF)	{
				m_msg_size.cx = std::max(m_msg_size.cx, m_item_list[i].get_msg_size().cx);
				m_msg_size.cy += m_item_list[i].get_msg_size().cy + m_cur.moji_space.cy;
			}
		}

		// 選択矩形と表示座標を再設定
		int offset_y = m_def.moji_pos.y;
		for (int i = 0; i < (int)m_item_list.size(); i++)	{

			// type == off の場合は何もしない
			if (m_item_list[i].get_item_type() != TNM_SEL_ITEM_TYPE_OFF)	{

				// 幅: 最大のテキスト幅、高さ: このアイテムの高さ＋スペース
				m_item_list[i].set_pos(C_point(0, offset_y));
				m_item_list[i].set_hit_rect(C_rect::by_size(0, offset_y, m_msg_size.cx, m_item_list[i].get_msg_size().cy + m_cur.moji_space.cy));
				offset_y += m_item_list[i].get_msg_size().cy + m_cur.moji_space.cy;
			}
		}
	}
}

// ****************************************************************
// 選択肢を表示
// ================================================================
void C_elm_mwnd_select::set_disp()
{
	for (ARRAY<C_elm_mwnd_select_item>::iterator itr = m_item_list.begin(); itr != m_item_list.end(); ++itr)	{
		itr->set_disp();
	}

	m_disp_item_cnt = (int)m_item_list.size();
}

// ****************************************************************
// メッセージを取得
// ================================================================
TSTR C_elm_mwnd_select::get_msg()
{
	return m_msg;
}

// ****************************************************************
// メッセージサイズを取得
// ================================================================
C_size C_elm_mwnd_select::get_msg_size()
{
	return m_msg_size;
}

// ****************************************************************
// まだ出現していない文字が存在するか判定する
// ================================================================
bool C_elm_mwnd_select::check_not_appear_moji()
{
	return (int)m_item_list.size() > m_disp_item_cnt;
}
