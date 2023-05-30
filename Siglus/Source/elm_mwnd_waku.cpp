#include	"pch.h"

#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_mwnd_waku.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// 初期化
// ================================================================
void C_elm_mwnd_waku::init(S_element element, CTSTR& name)
{
	// パラメータをクリア
	m_template_no = -1;
	m_extend_type = 0;

	// デフォルトの設定
	m_element = element;
	m_name = name;
	m_def.filter_margin = C_rect(0, 0, 0, 0);
	m_def.filter_color = C_argb(0, 0, 0, 0);
	m_def.filter_config_color = false;
	m_def.filter_config_tr = false;

	// 処理変数初期化
	init_work_variable();

	// キー待ちアイコン
	m_key_icon.init();

	// ボタン
	m_btn_list.init(m_element + S_element(ELM_MWND_BUTTON), m_name + _T(".button"), Gp_ini->mwnd_waku_btn_cnt, false, false, false, false, NULL, S_element(), S_element());

	// 顔グラ
	m_face_list.init(m_element + S_element(ELM_MWND_FACE), m_name + _T(".face"), Gp_ini->mwnd_waku_face_cnt, false, false, false, false, NULL, S_element(), S_element());

	// オブジェクト
	m_object_list.init(m_element + S_element(ELM_MWND_OBJECT), m_name + _T(".object"), Gp_ini->mwnd_waku_object_cnt, false, false, false, false, NULL, S_element(), S_element());
}

// ****************************************************************
// 再初期化
// ================================================================
void C_elm_mwnd_waku::reinit(bool restruct_flag)
{
	// スプライトをクリア
	m_spr_waku.init();
	m_spr_filter.init();

	// アルバムをクリア
	m_album_waku.reset();
	m_album_filter.reset();

	// パラメータをクリア
	m_extend_type = 0;
	m_waku_file.clear();

	// ボタン
	m_btn_list.reinit(restruct_flag);

	// 顔グラ
	m_face_list.reinit(restruct_flag);

	// オブジェクト
	m_object_list.reinit(restruct_flag);

	// キー待ちアイコン
	m_key_icon.reinit();
}

// ****************************************************************
// 終了
// ================================================================
void C_elm_mwnd_waku::finish()
{
	// ボタン
	m_btn_list.finish();

	// 顔グラ
	m_face_list.finish();

	// オブジェクト
	m_object_list.finish();
}

// ****************************************************************
// 処理変数初期化
// ================================================================
void C_elm_mwnd_waku::init_work_variable()
{
	m_cur = m_def;	// デフォルトパラメータを現在のパラメータへコピーする
}

// ****************************************************************
// テンプレートの値を設定
// ================================================================
void C_elm_mwnd_waku::set_template(int template_no, bool reinit_flag)
{
	// 再初期化
	if (reinit_flag)
		reinit(false);

	// テンプレート番号
	m_template_no = template_no;

	// テンプレートなし
	if (m_template_no < 0)
		return;

	// テンプレートパラメータ
	m_waku_file = Gp_ini->mwnd_waku[m_template_no].waku_file;
	m_filter_file = Gp_ini->mwnd_waku[m_template_no].filter_file;

	// テンプレートを再構築
	restruct_template();

	// 処理変数初期化
	init_work_variable();

	// キー待ちアイコン
	m_key_icon.set_template(template_no);
}

// ****************************************************************
// テンプレートを再構築
// ================================================================
void C_elm_mwnd_waku::restruct_template()
{
	// テンプレートなし
	if (m_template_no < 0)
		return;

	// 枠関連の情報
	m_extend_type = Gp_ini->mwnd_waku[m_template_no].extend_type;

	// 枠を再構築
	restruct_waku();

	// フィルター関連の情報
	m_def.filter_margin = Gp_ini->mwnd_waku[m_template_no].filter_margin;
	m_def.filter_color = Gp_ini->mwnd_waku[m_template_no].filter_color;
	m_def.filter_config_color = Gp_ini->mwnd_waku[m_template_no].filter_config_color;
	m_def.filter_config_tr = Gp_ini->mwnd_waku[m_template_no].filter_config_tr;

	// フィルターを再構築
	restruct_filter();

	// ボタン
	int btn_cnt = (int)m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)	{

		// ファイル名が指定してあればボタンと認識する
		if (!Gp_ini->mwnd_waku[m_template_no].btn[i].btn_file.empty())	{
			m_btn_list[i].create_pct(Gp_ini->mwnd_waku[m_template_no].btn[i].btn_file);
			m_btn_list[i].set_layer(Gp_ini->mwnd.moji_layer_rep);
			m_btn_list[i].set_disp(1);
			m_btn_list[i].set_button_cut_no(Gp_ini->mwnd_waku[m_template_no].btn[i].cut_no);
			m_btn_list[i].set_button_param(0, 0, Gp_ini->mwnd_waku[m_template_no].btn[i].action_no, Gp_ini->mwnd_waku[m_template_no].btn[i].se_no);
			m_btn_list[i].set_button_sys_type(Gp_ini->mwnd_waku[m_template_no].btn[i].sys_type, Gp_ini->mwnd_waku[m_template_no].btn[i].sys_type_opt);
			m_btn_list[i].set_button_mode(Gp_ini->mwnd_waku[m_template_no].btn[i].btn_mode);
			m_btn_list[i].set_frame_action(-1, 0, Gp_ini->mwnd_waku[m_template_no].btn[i].frame_action_scn_name, Gp_ini->mwnd_waku[m_template_no].btn[i].frame_action_cmd_name, C_tnm_prop_list());
			if (!Gp_ini->mwnd_waku[m_template_no].btn[i].scn_name.empty() && !Gp_ini->mwnd_waku[m_template_no].btn[i].cmd_name.empty())
				m_btn_list[i].set_button_decided_action(Gp_ini->mwnd_waku[m_template_no].btn[i].scn_name, Gp_ini->mwnd_waku[m_template_no].btn[i].cmd_name);
			else if (!Gp_ini->mwnd_waku[m_template_no].btn[i].scn_name.empty() && Gp_ini->mwnd_waku[m_template_no].btn[i].z_no >= 0)
				m_btn_list[i].set_button_decided_action(Gp_ini->mwnd_waku[m_template_no].btn[i].scn_name, Gp_ini->mwnd_waku[m_template_no].btn[i].z_no);
		}
	}
}

// ****************************************************************
// セーブ
// ================================================================
void C_elm_mwnd_waku::save(C_tnm_save_stream& stream)
{
	// テンプレート番号
	stream.save(m_template_no);

	// テンプレートパラメータ
	stream.save(m_waku_file);
	stream.save(m_filter_file);

	// 現在のパラメータ
	stream.save(m_cur);

	// キー待ちアイコン
	m_key_icon.save(stream);

	// ボタンはセーブしない

	// 顔グラ
	m_face_list.save(stream);

	// オブジェクト
	m_object_list.save(stream);
}

// ****************************************************************
// ロード
// ================================================================
void C_elm_mwnd_waku::load(C_tnm_save_stream& stream)
{
	// ★初期化済みを前提としています。

	// テンプレート番号
	stream.load(m_template_no);

	// テンプレートパラメータ
	stream.load(m_waku_file);
	stream.load(m_filter_file);

	// テンプレートを再構築
	restruct_template();

	// 現在のパラメータ
	stream.load(m_cur);

	// キー待ちアイコン
	m_key_icon.load(stream);

	// ボタンはロードしない

	// 顔グラ
	m_face_list.load(stream);

	// オブジェクト
	m_object_list.load(stream);
}

// ****************************************************************
// コピー（全て）
// ================================================================
void C_elm_mwnd_waku::copy_all(C_elm_mwnd_waku* src, bool reinit_flag)
{
	// 再初期化
	if (reinit_flag)
		reinit(false);

	// テンプレート番号
	m_template_no = src->m_template_no;

	// テンプレートパラメータ
	m_waku_file = src->m_waku_file;
	m_filter_file = src->m_filter_file;

	// テンプレートを再構築
	restruct_template();

	// 現在のパラメータ
	m_cur = src->m_cur;

	// キー待ちアイコン
	m_key_icon.copy(&src->m_key_icon);

	// ボタン
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)
		m_btn_list[i].copy(&src->m_btn_list[i], false);

	// 顔グラ
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].copy(&src->m_face_list[i], false);

	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].copy(&src->m_object_list[i], false);
}

// ****************************************************************
// コピー（枠）
// ================================================================
void C_elm_mwnd_waku::copy_waku(C_elm_mwnd_waku* src, bool reinit_flag)
{
	// 再初期化
	if (reinit_flag)
		reinit(false);

	// テンプレート番号
	m_template_no = src->m_template_no;

	// テンプレートパラメータ
	m_waku_file = src->m_waku_file;
	m_filter_file = src->m_filter_file;

	// テンプレートを再構築
	restruct_template();

	// 現在のパラメータ
	m_cur = src->m_cur;
}

// ****************************************************************
// コピー（キー待ちアイコン）
// ================================================================
void C_elm_mwnd_waku::copy_key_icon(C_elm_mwnd_waku* src)
{
	// キー待ちアイコン
	m_key_icon.copy(&src->m_key_icon);
}

// ****************************************************************
// コピー（顔グラ）
// ================================================================
void C_elm_mwnd_waku::copy_face(C_elm_mwnd_waku* src)
{
	// 顔グラ
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].copy(&src->m_face_list[i], false);
}

// ****************************************************************
// コピー（オブジェクト）
// ================================================================
void C_elm_mwnd_waku::copy_object(C_elm_mwnd_waku* src)
{
	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].copy(&src->m_object_list[i], false);
}

// ****************************************************************
// フレーム初期化処理
// ================================================================
void C_elm_mwnd_waku::frame_init()
{
	// スプライトを初期化
	m_spr_waku.rp.init();
	m_spr_filter.rp.init();

	// キー待ちアイコン
	m_key_icon.frame_init();

	// ボタン
	int bnt_cnt = m_btn_list.get_size();
	for (int i = 0; i < bnt_cnt; i++)
		m_btn_list[i].frame_init();

	// 顔グラ
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)	{
		m_face_list[i].frame_init();
		m_face_list[i].set_disp(0);		// 非表示にする
	}

	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].frame_init();
}

// ****************************************************************
// 時間を進める
// ================================================================
void C_elm_mwnd_waku::update_time(int past_game_time, int past_real_time)
{
	// ボタン
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)
		m_btn_list[i].update_time(past_game_time, past_real_time, 0);

	// 顔グラ
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].update_time(past_game_time, past_real_time, 0);

	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].update_time(past_game_time, past_real_time, 0);
}

// ****************************************************************
// フレーム処理
// ================================================================
void C_elm_mwnd_waku::frame(int cur_time, C_rect window_rect, const S_tnm_render_param* parent_trp, bool face_appear_flag)
{
	if (m_template_no < 0)
		return;

	// 枠ファイルを使う場合
	if (m_spr_waku.get_texture())	{

		// 枠のレンダーパラメータ
		S_tnm_render_param waku_trp;
		waku_trp.disp = true;
		waku_trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.waku_layer_rep);
		waku_trp.pos_x = (float)window_rect.left;
		waku_trp.pos_y = (float)window_rect.top;

		// 親のレンダーパラメータを加える
		tnm_add_parent_trp(&waku_trp, parent_trp);

		// 枠スプライトのレンダーパラメータ
		tnm_trp_to_rp(&waku_trp, &m_spr_waku.rpex, &m_spr_waku.rp);
		m_spr_waku.rp.d2_rect = true;
		m_spr_waku.rp.size_fit_to_texture = 0;	// ０番テクスチャにフィット
	}

	// フィルタのレンダーパラメータ
	{
		S_tnm_render_param filter_trp;
		filter_trp.disp = true;
		filter_trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.filter_layer_rep);
		filter_trp.pos_x = (float)window_rect.left + m_cur.filter_margin.left;
		filter_trp.pos_y = (float)window_rect.top + m_cur.filter_margin.top;
		if (m_def.filter_config_tr)	{
			filter_trp.tr = Gp_config->filter_color.a;				// 透明度
		}
		if (m_def.filter_config_color)	{
			filter_trp.color_add_b = Gp_config->filter_color.b;		// 色加算
			filter_trp.color_add_g = Gp_config->filter_color.g;		// 色加算
			filter_trp.color_add_r = Gp_config->filter_color.r;		// 色加算
		}

		// 親のレンダーパラメータを加える
		tnm_add_parent_trp(&filter_trp, parent_trp);

		// フィルタースプライトのレンダーパラメータ
		tnm_trp_to_rp(&filter_trp, &m_spr_filter.rpex, &m_spr_filter.rp);
		m_spr_filter.rp.d2_rect = true;

		// ファイルを使う場合
		if (m_spr_filter.get_texture())	{
			m_spr_filter.rp.size_fit_to_texture = 0;	// ０番テクスチャにフィット
		}
		// ファイルを使わない場合
		else	{
			int width = window_rect.width() - m_cur.filter_margin.left - m_cur.filter_margin.right;
			int height = window_rect.height() - m_cur.filter_margin.top - m_cur.filter_margin.bottom;
			m_spr_filter.rp.size = C_float2((float)width, (float)height);	// フィルタのサイズ
			m_spr_filter.rp.filter.color = m_cur.filter_color;				// フィルタの色
		}
	}

	// キー待ちアイコン
	m_key_icon.frame(cur_time, window_rect, parent_trp);

	// ボタン
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)	{

		// 子に引き継ぐためのレンダーパラメータ
		S_tnm_render_param trp;
		trp.disp = true;
		trp.pos_x = (float)window_rect.left;
		trp.pos_y = (float)window_rect.top;

		// 位置を計算する
		switch (Gp_ini->mwnd_waku[m_template_no].btn[i].pos_base)	{
			case 0:
				trp.pos_x += Gp_ini->mwnd_waku[m_template_no].btn[i].pos.x;
				trp.pos_y += Gp_ini->mwnd_waku[m_template_no].btn[i].pos.y;
				break;
			case 1:
				trp.pos_x += window_rect.width() - Gp_ini->mwnd_waku[m_template_no].btn[i].pos.x;
				trp.pos_y += Gp_ini->mwnd_waku[m_template_no].btn[i].pos.y;
				break;
			case 2:
				trp.pos_x += Gp_ini->mwnd_waku[m_template_no].btn[i].pos.x;
				trp.pos_y += window_rect.height() - Gp_ini->mwnd_waku[m_template_no].btn[i].pos.y;
				break;
			case 3:
				trp.pos_x += window_rect.width() - Gp_ini->mwnd_waku[m_template_no].btn[i].pos.x;
				trp.pos_y += window_rect.height() - Gp_ini->mwnd_waku[m_template_no].btn[i].pos.y;
				break;
		}

		// 設定によってボタンを不許可にする
		if (Gp_local->pod.mwnd_btn_disable_all || Gp_local->mwnd_btn_disable[i].value)
			trp.button_state = TNM_BTN_STATE_DISABLE;

		// 補正レイヤー値設定
		trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.waku_layer_rep);

		// 親のレンダーパラメータを加える
		tnm_add_parent_trp(&trp, parent_trp);

		// フレーム処理
		m_btn_list[i].frame(&trp, 0, 0);
	}

	// 顔グラ
	if (face_appear_flag)	{	// 表示する場合

		int face_cnt = m_face_list.get_size();
		for (int i = 0; i < face_cnt; i++)	{

			// 子に引き継ぐためのレンダーパラメータ
			S_tnm_render_param trp;
			trp.disp = true;
			trp.pos_x = (float)window_rect.left + Gp_ini->mwnd_waku[m_template_no].face_pos[i].x;
			trp.pos_y = (float)window_rect.top + Gp_ini->mwnd_waku[m_template_no].face_pos[i].y;

			// 表示する
			m_face_list[i].set_disp(1);

			// 補正レイヤー値設定
			trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.face_layer_rep);

			// 親のレンダーパラメータを加える
			tnm_add_parent_trp(&trp, parent_trp);

			// フレーム処理
			m_face_list[i].frame(&trp, 0, 0);
		}
	}

	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{

		// 子に引き継ぐためのレンダーパラメータ
		S_tnm_render_param trp;
		trp.disp = true;
		trp.pos_x = (float)window_rect.left;
		trp.pos_y = (float)window_rect.top;

		// 親のレンダーパラメータを加える
		tnm_add_parent_trp(&trp, parent_trp);

		// フレーム処理
		m_object_list[i].frame(&trp, 0, 0);
	}
}

// ****************************************************************
// フレームアクション処理
// ================================================================
void C_elm_mwnd_waku::do_frame_action()
{
	if (m_template_no < 0)
		return;

	// ボタン
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)
		m_btn_list[i].do_frame_action();

	// 顔グラ
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].do_frame_action();

	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].do_frame_action();
}

// ****************************************************************
// スプライトツリーを構築
// ================================================================
void C_elm_mwnd_waku::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	if (m_template_no < 0)
		return;

	root.add_child(&m_spr_waku);
	root.add_child(&m_spr_filter);

	// キー待ちアイコン
	m_key_icon.get_sprite_tree(root);

	// ボタン
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)
		m_btn_list[i].get_sprite_tree(root);

	// 顔グラ
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].get_sprite_tree(root);

	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].get_sprite_tree(root);
}

// ****************************************************************
// ボタンを登録
// ================================================================
void C_elm_mwnd_waku::regist_button()
{
	// ボタン
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)	{
		m_btn_list[i].regist_button(INT_MIN);
	}

	// 顔グラ
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)	{
		m_face_list[i].regist_button(INT_MIN);
	}

	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		m_object_list[i].regist_button(INT_MIN);
	}
}

// ****************************************************************
// ボタンイベント
// ================================================================
void C_elm_mwnd_waku::button_event()
{
	// ボタン
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)	{
		int parent_button_state = TNM_BTN_STATE_NORMAL;

		// 設定によってボタンを不許可にする
		if (Gp_local->pod.mwnd_btn_disable_all || Gp_local->mwnd_btn_disable[i].value)	{
			parent_button_state = TNM_BTN_STATE_DISABLE;
		}

		// disable でなければイベント
		if (parent_button_state != TNM_BTN_STATE_DISABLE)	{
			m_btn_list[i].button_event();
		}
	}

	// 顔グラ
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)	{
		m_face_list[i].button_event();
	}

	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		m_object_list[i].button_event();
	}
}

// ****************************************************************
// 可変枠の設定
// ================================================================
//		テキストの内容から、可変枠の大きさを計算し、スプライトを作成します。
// ================================================================
bool C_elm_mwnd_waku::set_extend_waku(C_rect* window_rect)
{
	// 固定枠の場合
	if (m_extend_type == 0)	{

		// 画像を使っている場合は画像サイズ
		if (m_album_waku && m_album_waku->get_texture(0))	{
			*window_rect = C_rect(C_point(0, 0), m_album_waku->get_texture(0)->get_original_size());

			// スプライトにテクスチャを設定する
			m_spr_waku.set_texture(m_album_waku->get_texture(0));
		}
	}

	// 可変枠の場合
	else if (m_extend_type == 1)	{

		// 枠ファイルが指定されている場合
		if (!m_waku_file.empty())
		{
			// g00 をさがす
			int pct_type = 0;
			TSTR file_path = tnm_find_g00(m_waku_file, &pct_type);

			if (file_path.empty())
			{
				tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("枠ファイル ") + m_waku_file + _T(" が見つかりませんでした。"));
				return false;
			}
			if (pct_type != 1)
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("枠ファイルは g00 しか対応していません。\n") + m_waku_file);
				return false;
			}

			// g00 から枠を読み込み
			BSP<C_d3d_texture> texture = G_rsm.create_texture_from_mwnd(file_path, m_extend_type, window_rect->size(), false);
			if (!texture)
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("枠ファイル ") + m_waku_file + _T(" が読み込めませんでした。") + get_last_error_msg());
				return false;
			}

			*window_rect = C_rect(C_point(0, 0), texture->get_size());

			// スプライトにテクスチャを設定する
			m_spr_waku.set_texture(texture);
		}
	}

	return true;
}

// ****************************************************************
// 枠ファイルを初期化
// ================================================================
void C_elm_mwnd_waku::init_waku_file()
{
	// スプライトとアルバムを解放
	m_spr_waku.init();
	m_album_waku.reset();

	// テンプレートなし
	if (m_template_no < 0)
		return;

	// 枠ファイルを設定
	m_waku_file = Gp_ini->mwnd_waku[m_template_no].waku_file;

	// 枠を再構築
	restruct_waku();
}

// ****************************************************************
// 枠ファイルを変更
// ================================================================
void C_elm_mwnd_waku::set_waku_file(CTSTR& waku_file)
{
	// 枠が変わらない場合は何もしない
	if (waku_file == m_waku_file)
		return;

	// スプライトとアルバムを解放
	m_spr_waku.init();
	m_album_waku.reset();

	// 枠ファイルを設定
	m_waku_file = waku_file;
	
	// 枠を再構築
	restruct_waku();
}

// ****************************************************************
// 枠を再構築
// ================================================================
void C_elm_mwnd_waku::restruct_waku()
{
	// ファイル名の指定がある場合、ファイルを読み込む
	if (!m_waku_file.empty())	{

		// ウィンドウのスプライトを作成
		m_spr_waku.create_polygon_no_init();
		m_spr_waku.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_spr_waku.set_index_buffer(G_rect_index_buffer);

		// テクスチャを読み込む
		m_album_waku = tnm_load_pct_d3d(m_waku_file, false, false);
		//if (m_album_waku)
		//	m_spr_waku.set_texture(m_album_waku->get_texture(0));
		// ↑たぶん可変枠の場合を考えてコメントアウト？？？
	}
	// ファイル名がない場合はフィルター
	else	{

		// ウィンドウのスプライトを作成
		m_spr_waku.create_polygon_no_init();
		m_spr_waku.set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
		m_spr_waku.set_index_buffer(G_rect_index_buffer);
	}
}

// ****************************************************************
// フィルターファイルを初期化
// ================================================================
void C_elm_mwnd_waku::init_filter_file()
{
	// スプライトとアルバムを解放
	m_spr_filter.init();
	m_album_filter.reset();

	// テンプレートなし
	if (m_template_no < 0)
		return;

	// フィルターファイルを設定
	m_filter_file = Gp_ini->mwnd_waku[m_template_no].filter_file;

	// フィルターを再構築
	restruct_filter();
}

// ****************************************************************
// フィルターファイルを変更
// ================================================================
void C_elm_mwnd_waku::set_filter_file(CTSTR& filter_file)
{
	// フィルターが変わらない場合は何もしない
	if (filter_file == m_filter_file)
		return;

	// スプライトとアルバムを解放
	m_spr_filter.init();
	m_album_filter.reset();

	// フィルターファイルを設定
	m_filter_file = filter_file;
	
	// フィルターを再構築
	restruct_filter();
}

// ****************************************************************
// フィルターを再構築
// ================================================================
void C_elm_mwnd_waku::restruct_filter()
{
	// ファイル名の指定がある場合、ファイルを読み込む
	if (!m_filter_file.empty())	{

		// フィルターのスプライトを作成
		m_spr_filter.create_polygon_no_init();
		m_spr_filter.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_spr_filter.set_index_buffer(G_rect_index_buffer);

		// テクスチャを読み込む
		m_album_filter = tnm_load_pct_d3d(m_filter_file, false, false);
		if (m_album_filter)
			m_spr_filter.set_texture(m_album_filter->get_texture(0));
	}
	// ファイル名がない場合はフィルター
	else	{

		// フィルターのスプライトを作成
		m_spr_filter.create_polygon_no_init();
		m_spr_filter.set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
		m_spr_filter.set_index_buffer(G_rect_index_buffer);
	}

}

// ****************************************************************
// 顔グラをクリア
// ================================================================
void C_elm_mwnd_waku::clear_face()
{
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].reinit(true);
}

// ****************************************************************
// 顔グラの設定
// ================================================================
void C_elm_mwnd_waku::set_face(int face_no, CTSTR& file_path)
{
	if (face_no < 0 || m_face_list.get_size() <= face_no)
		return;

	m_face_list[face_no].create_pct(file_path);
}

// ****************************************************************
// 顔グラの存在判定
// ================================================================
bool C_elm_mwnd_waku::is_exist_face()
{
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)	{
		if (m_face_list[i].get_type() == TNM_OBJECT_TYPE_PCT)	{
			return true;
		}
	}

	return false;
}
