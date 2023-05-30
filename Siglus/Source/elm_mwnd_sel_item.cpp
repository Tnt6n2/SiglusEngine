#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_mwnd_sel_item.h"
#include	"element/elm_mwnd_moji.h"
#include	"element/elm_sound.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_message.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// 初期化
// ================================================================
void C_elm_mwnd_select_item::init()
{
	m_msg.clear();
	m_moji_list.clear();

	m_button.init();
	m_item_type = TNM_SEL_ITEM_TYPE_ON;
}

// ****************************************************************
// セーブ
// ================================================================
void C_elm_mwnd_select_item::save(C_tnm_save_stream& stream)
{
	// パラメータ
	stream.save(m_item_type);
	stream.save(m_pos);
	stream.save(m_msg);
	stream.save(m_msg_size);

	// 文字
	stream.save(m_moji_list);
}

// ****************************************************************
// ロード
// ================================================================
void C_elm_mwnd_select_item::load(C_tnm_save_stream& stream)
{
	// パラメータ
	stream.load(m_item_type);
	stream.load(m_pos);
	stream.load(m_msg);
	stream.load(m_msg_size);

	// 文字
	stream.load(m_moji_list);
}

// ****************************************************************
// コピーを作成
// ================================================================
void C_elm_mwnd_select_item::copy(C_elm_mwnd_select_item* src)
{
	// 現在のパラメータ
	m_item_type = src->m_item_type;
	m_pos = src->m_pos;
	m_msg = src->m_msg;
	m_msg_size = src->m_msg_size;
	
	// 文字
	m_moji_list.resize(src->m_moji_list.size());
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i] = src->m_moji_list[i];
}

// ****************************************************************
// フレーム初期化
// ================================================================
void C_elm_mwnd_select_item::frame_init()
{
	// ボタンをフレーム初期化
	m_button.frame_init();

	// 文字をフレーム初期化
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].frame_init();
}

// ****************************************************************
// フレーム処理
// ================================================================
void C_elm_mwnd_select_item::frame(const S_tnm_render_param* parent_trp, C_point pos_rep, int dark_rep, bool draw_futoku, bool draw_shadow)
{
	if (m_item_type == TNM_SEL_ITEM_TYPE_ON || m_item_type == TNM_SEL_ITEM_TYPE_READ)	{

		// 当たり判定はテキスト空間で持っているので、ここでウィンドウ空間に直す
		m_button.set_param(m_hit_rect_temp + C_point((int)parent_trp->pos_x, (int)parent_trp->pos_y) + pos_rep, S_tnm_sorter(0, Gp_ini->mwnd.moji_layer_rep) + parent_trp->sorter);

		// ボタンの当たり状態を取得
		int btn_state = Gp_btn_mng->get_hit_state(&m_button);

		// 文字
		int dark = dark_rep + (btn_state == TNM_BTN_STATE_NORMAL ? 64 : 0);
		for (int i = 0; i < (int)m_moji_list.size(); i++)
			m_moji_list[i].frame(parent_trp, m_pos + pos_rep, dark, 0, C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), draw_futoku, draw_shadow);
	}
}

// ****************************************************************
// スプライトツリーを構築
// ================================================================
void C_elm_mwnd_select_item::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].get_sprite_tree(root, root);
}

// ****************************************************************
// ボタンを登録
// ================================================================
void C_elm_mwnd_select_item::regist_button()
{
	if (m_item_type == TNM_SEL_ITEM_TYPE_ON)
		Gp_btn_mng->regist_button(&m_button);
}

// ****************************************************************
// ボタンイベント処理
// ================================================================
bool C_elm_mwnd_select_item::button_event(bool wait_flag)
{
	// 当たった
	if (Gp_btn_mng->is_hit_this_frame(&m_button))	{

		// 効果音
		tnm_play_se(TNM_SE_TYPE_SELECT);
	}

	// 決定された
	if (Gp_btn_mng->is_decided_this_frame(&m_button))	{

		return true;
	}

	return false;
}

// ****************************************************************
// 当たっているかを取得
// ================================================================
bool C_elm_mwnd_select_item::is_active_hit()
{
	return Gp_btn_mng->is_active(&m_button);
}

// ****************************************************************
// 文字を設定
// ================================================================
void C_elm_mwnd_select_item::set_msg(CTSTR& msg, int item_type, int moji_size, C_size moji_space, int moji_color, int shadow_color, int fuchi_color)
{
	m_item_type = item_type;
	m_msg = msg;

	// アイテムリストを作成
	C_point pos(0, 0);
	for (TSTR::const_iterator itr = msg.begin(); itr != msg.end();)
	{
		int moji_type = TNM_MOJI_TYPE_MOJI;
		int moji_code = -1;

		// 絵文字判定
		tnm_msg_proc_expand_name_flag(itr, msg.end(), &moji_type, &moji_code);

		// 文字を作成
		C_elm_mwnd_moji moji;
		moji.set_moji(moji_type, moji_code, moji_size, moji_color, shadow_color, fuchi_color, pos);

		// 文字リストに追加
		m_moji_list.push_back(moji);

		// 次の文字位置の設定
		int this_moji_size = 0;
		if (moji_type != TNM_MOJI_TYPE_MOJI)	this_moji_size = moji_size;
		else if (is_hankaku((TCHAR)moji_code))	this_moji_size = moji_size / 2;
		else									this_moji_size = moji_size;
		pos.x += this_moji_size;

		//if (is_hankaku(*itr))	pos.x += (moji_size + moji_space.cx) / 2;
		//else					pos.x += moji_size + moji_space.cx;
	}

	// メッセージサイズを計算（可変枠の大きさの計算に使います）
	m_msg_size = C_size(pos.x - moji_space.cx, moji_size);
}

// ****************************************************************
// 選択矩形を設定
// ================================================================
void C_elm_mwnd_select_item::set_hit_rect(C_rect hit_rect)
{
	m_hit_rect_temp = hit_rect;
}

// ****************************************************************
// 表示座標を設定
// ================================================================
void C_elm_mwnd_select_item::set_pos(C_point pos)
{
	m_pos = pos;
}

// ****************************************************************
// 選択肢を表示
// ================================================================
void C_elm_mwnd_select_item::set_disp()
{
	for (ARRAY<C_elm_mwnd_moji>::iterator itr = m_moji_list.begin(); itr != m_moji_list.end(); ++itr)	{
		itr->set_appear(true);
	}
}

// ****************************************************************
// メッセージを取得
// ================================================================
TSTR C_elm_mwnd_select_item::get_msg()
{
	return m_msg;
}

// ****************************************************************
// メッセージサイズを取得
// ================================================================
C_size C_elm_mwnd_select_item::get_msg_size()
{
	return m_msg_size;
}

