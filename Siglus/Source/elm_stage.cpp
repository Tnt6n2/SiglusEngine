#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"element/elm_stage.h"
#include	"engine/eng_etc.h"

// ****************************************************************
// ステージ：初期化
// ================================================================
void C_elm_stage::init(S_element element, CTSTR& name, bool use_ini)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_STAGE, name, NULL);

	// グループリストを初期化
	m_group_list.init(element + S_element(ELM_STAGE_OBJBTNGROUP), name + _T(".group"), Gp_ini->button.group_cnt, false, element + S_element(ELM_STAGE_OBJECT));

	// オブジェクトリストを初期化
	m_object_list.init(element + S_element(ELM_STAGE_OBJECT), name + _T(".object"), Gp_ini->object_cnt, false, use_ini, true, true, this, S_element(ELM_UP, ELM_STAGE_OBJBTNGROUP), S_element(ELM_UP, ELM_STAGE_WORLD));

	// メッセージウィンドウを初期化
	m_mwnd_list.init(element + S_element(ELM_STAGE_MWND), name + _T(".mwnd"), Gp_ini->mwnd.sub_cnt, false, this, S_element(ELM_UP, ELM_STAGE_OBJBTNGROUP));

	// ボタン選択肢を初期化
	m_btn_sel.init(element + S_element(ELM_STAGE_BTNSELITEM), name + _T(".btnselitem"));
	
	// ワールドを初期化
	m_world_list.init(element + S_element(ELM_STAGE_WORLD), FM_WORLDLIST, name + _T(".world"), Gp_ini->world_cnt, false, NULL);

	// エフェクトを初期化
	m_effect_list.init(element + S_element(ELM_STAGE_EFFECT), FM_EFFECTLIST, name + _T(".effect"), Gp_ini->effect_cnt, false, NULL);

	// クエイクを初期化
	m_quake_list.init(element + S_element(ELM_STAGE_QUAKE), FM_QUAKELIST, name + _T(".quake"), Gp_ini->quake_cnt, false, NULL);

	// 再初期化
	reinit(true);
}

// ****************************************************************
// ステージ：再初期化
// ================================================================
void C_elm_stage::reinit(bool restruct_flag)
{
	// グループリストを再初期化
	m_group_list.reinit();

	// オブジェクトリストを再初期化
	m_object_list.reinit(restruct_flag);

	// メッセージウィンドウを再初期化
	m_mwnd_list.reinit(restruct_flag);

	// ボタン選択肢を再初期化
	m_btn_sel.reinit();

	// ワールドを再初期化
	m_world_list.reinit();

	// エフェクトを再初期化
	m_effect_list.reinit();

	// クエイクを再初期化
	m_quake_list.reinit();
}

// ****************************************************************
// ステージ：終了
// ================================================================
void C_elm_stage::finish()
{
	// オブジェクト
	m_object_list.finish();

	// メッセージウィンドウ
	m_mwnd_list.finish();

	// ボタン選択肢
	m_btn_sel.finish();
}

// ****************************************************************
// ステージ：セーブ
// ================================================================
void C_elm_stage::save(C_tnm_save_stream& stream)
{
	// グループ
	m_group_list.save(stream);

	// オブジェクト
	m_object_list.save(stream);

	// メッセージウィンドウ
	m_mwnd_list.save(stream);

	// ボタン選択肢
	m_btn_sel.save(stream);

	// ワールド
	m_world_list.save(stream);

	// エフェクト
	m_effect_list.save(stream);

	// クエイク
	m_quake_list.save(stream);
}

// ****************************************************************
// ステージ：ロード
// ================================================================
void C_elm_stage::load(C_tnm_save_stream& stream)
{
	// グループ
	m_group_list.load(stream);

	// オブジェクト
	m_object_list.load(stream);

	// メッセージウィンドウ
	m_mwnd_list.load(stream);

	// ボタン選択肢
	m_btn_sel.load(stream);

	// ワールド
	m_world_list.load(stream);

	// エフェクト
	m_effect_list.load(stream);

	// クエイク
	m_quake_list.load(stream);
}

// ****************************************************************
// ステージ：フレーム初期化処理
// ================================================================
void C_elm_stage::frame_init()
{
	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
	{
		if (m_object_list[i].is_use())
			m_object_list[i].frame_init();
	}

	// メッセージウィンドウ
	int mwnd_cnt = (int)m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)
		m_mwnd_list[i].frame_init();

	// ボタン選択肢
	m_btn_sel.frame_init();
}

// ****************************************************************
// ステージ：時間を更新
// ================================================================
void C_elm_stage::update_time(int past_game_time, int past_real_time)
{
	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
	{
		if (m_object_list[i].is_use())
			m_object_list[i].update_time(past_game_time, past_real_time, 0);
	}

	// メッセージウィンドウ
	int mwnd_cnt = m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)
		m_mwnd_list[i].update_time(past_game_time, past_real_time);

	// ボタン選択肢
	m_btn_sel.update_time(past_game_time, past_real_time);

	// ワールド
	int world_cnt = m_world_list.get_size();
	for (int i = 0; i < world_cnt; i++)
		m_world_list[i].update_time(past_game_time, past_real_time);

	// エフェクト
	int effect_cnt = m_effect_list.get_size();
	for (int i = 0; i < effect_cnt; i++)
		m_effect_list[i].update_time(past_game_time, past_real_time);

	// クエイク
	int quake_cnt = m_quake_list.get_size();
	for (int i = 0; i < quake_cnt; i++)
		m_quake_list[i].update_time(past_game_time);
}

// ****************************************************************
// ステージ：フレーム処理
// ================================================================
bool C_elm_stage::frame(bool draw_futoku, bool draw_shadow, int order)
{
	// ワールド（必ずオブジェクトより先！）
	int world_cnt = m_world_list.get_size();
	for (int i = 0; i < world_cnt; i++)
		m_world_list[i].frame();

	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
	{
		if (m_object_list[i].is_use())
			m_object_list[i].frame(NULL, order, 0);
	}

	// メッセージウィンドウ
	int mwnd_cnt = m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)
	{
		if (!m_mwnd_list[i].frame(draw_futoku, draw_shadow))
			return false;
	}

	// ボタン選択肢
	m_btn_sel.frame(draw_futoku, draw_shadow);

	// エフェクト
	int effect_cnt = m_effect_list.get_size();
	for (int i = 0; i < effect_cnt; i++)
		m_effect_list[i].frame();

	// クエイク
	int quake_cnt = m_quake_list.get_size();
	for (int i = 0; i < quake_cnt; i++)
		m_quake_list[i].frame();

	return true;
}

// ****************************************************************
// ステージ：フレームアクション処理
// ================================================================
void C_elm_stage::frame_action()
{
	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		if (m_object_list[i].is_use())
			m_object_list[i].do_frame_action();
	}

	// メッセージウィンドウ
	int mwnd_cnt = m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)
		m_mwnd_list[i].do_frame_action();
}

// ****************************************************************
// ステージ：ボタンフレーム初期化処理
// ================================================================
void C_elm_stage::button_frame_init()
{
	// グループ
	int group_cnt = m_group_list.get_size();
	for (int i = 0; i < group_cnt; i++)
		m_group_list[i].button_frame_init();
}

// ****************************************************************
// ステージ：ボタン登録処理
// ================================================================
void C_elm_stage::regist_button(int min_order)
{
	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
	{
		if (m_object_list[i].is_use())
		{
			m_object_list[i].regist_button(min_order);
		}
	}

	// メッセージウィンドウ
	if (!tnm_is_hide_mwnd())
	{
		int mwnd_cnt = m_mwnd_list.get_size();
		for (int i = 0; i < mwnd_cnt; i++)
		{
			m_mwnd_list[i].regist_button();
		}
	}

	// ボタン選択肢
	if (!tnm_is_hide_mwnd())
	{
		m_btn_sel.regist_button();
	}
}

// ****************************************************************
// ステージ：ボタンイベント処理
// ================================================================
void C_elm_stage::button_event(C_elm_mwnd* p_sel_target_mwnd)
{
	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		if (m_object_list[i].is_use())
			m_object_list[i].button_event();
	}

	// メッセージウィンドウ
	if (!tnm_is_hide_mwnd())	{
		int mwnd_cnt = m_mwnd_list.get_size();
		for (int i = 0; i < mwnd_cnt; i++)
			m_mwnd_list[i].button_event(p_sel_target_mwnd == &m_mwnd_list[i]);
	}

	// ボタン選択肢
	if (!tnm_is_hide_mwnd())
		m_btn_sel.button_event();
}

// ****************************************************************
// ステージ：コピーを作成
// ================================================================
void C_elm_stage::copy(C_elm_stage* src)
{
	// グループ
	int group_cnt = m_group_list.get_size();
	for (int i = 0; i < group_cnt; i++)
		m_group_list[i].copy(&src->m_group_list[i]);

	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		if (m_object_list[i].is_use())
			m_object_list[i].copy(&src->m_object_list[i], true);
	}

	// メッセージウィンドウ
	int mwnd_cnt = m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)
		m_mwnd_list[i].copy(&src->m_mwnd_list[i]);

	// ボタン選択肢
	m_btn_sel.copy(&src->m_btn_sel);

	// ワールド
	int world_cnt = m_world_list.get_size();
	for (int i = 0; i < world_cnt; i++)
		m_world_list[i].copy(&src->m_world_list[i]);

	// エフェクト
	int effect_cnt = m_effect_list.get_size();
	for (int i = 0; i < effect_cnt; i++)
		m_effect_list[i].copy(&src->m_effect_list[i]);

	// クエイク
	int quake_cnt = m_quake_list.get_size();
	for (int i = 0; i < quake_cnt; i++)
		m_quake_list[i].copy(&src->m_quake_list[i]);
}

// ****************************************************************
// ステージ：ワイプする
// ================================================================
//		コピーして、コピー元を初期化する
// ================================================================
void C_elm_stage_list::wipe(S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter)
{
	C_elm_stage* p_back = &m_sub[TNM_STAGE_BACK];
	C_elm_stage* p_front = &m_sub[TNM_STAGE_FRONT];
	C_elm_stage* p_next = &m_sub[TNM_STAGE_NEXT];

	// オブジェクト
	int object_cnt = p_front->m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		C_elm_object* p_back_object = &p_back->m_object_list[i];
		C_elm_object* p_front_object = &p_front->m_object_list[i];
		C_elm_object* p_next_object = &p_next->m_object_list[i];

		if (p_front_object->is_use())	{
			S_tnm_sorter back_sorter = p_back_object->get_sorter();
			S_tnm_sorter front_sorter = p_front_object->get_sorter();

			// フロントがオーダー内ならワイプする
			// バックが準備されていたらワイプする
			if ((begin_sorter <= front_sorter && front_sorter <= end_sorter) || p_back_object->get_type() != TNM_OBJECT_TYPE_NONE || p_back_object->get_child_cnt() > 0)	{
				p_next_object->copy(p_front_object, true);

				// フロントにワイプコピーが立っていなければワイプする
				// バックが準備されていたらワイプする
				// バックにワイプ消去が立っていればワイプする
				if (p_front_object->get_wipe_copy() == 0 || p_back_object->get_type() != TNM_OBJECT_TYPE_NONE || p_back_object->get_child_cnt() > 0 || p_back_object->get_wipe_erase() == 1)	{
					p_front_object->copy(p_back_object, true);
					p_back_object->reinit(true);
				}
			}
		}
	}

	// メッセージウィンドウ
	int mwnd_cnt = p_front->m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)	{
		S_tnm_sorter front_sorter = p_front->m_mwnd_list[i].get_sorter();
		if (begin_sorter <= front_sorter && front_sorter <= end_sorter)	{
			p_next->m_mwnd_list[i].copy(&p_front->m_mwnd_list[i]);
			p_front->m_mwnd_list[i].copy(&p_back->m_mwnd_list[i]);
			p_back->m_mwnd_list[i].reinit(true);
		}
	}

	// グループ
	int group_cnt = p_front->m_group_list.get_size();
	for (int i = 0; i < group_cnt; i++)	{
		S_tnm_sorter front_sorter = p_front->m_group_list[i].get_sorter();
		if (begin_sorter <= front_sorter && front_sorter <= end_sorter)	{
			p_next->m_group_list[i].copy(&p_front->m_group_list[i]);
			p_front->m_group_list[i].copy(&p_back->m_group_list[i]);
			p_back->m_group_list[i].reinit();
		}
	}

	// ボタン選択肢
	p_next->m_btn_sel.copy(&p_front->m_btn_sel);
	p_front->m_btn_sel.copy(&p_back->m_btn_sel);
	p_back->m_btn_sel.reinit();

	// ワールド
	int world_cnt = p_front->m_world_list.get_size();
	for (int i = 0; i < world_cnt; i++)	{
		C_elm_world* p_back_world = &p_back->m_world_list[i];
		C_elm_world* p_front_world = &p_front->m_world_list[i];
		C_elm_world* p_next_world = &p_next->m_world_list[i];

		S_tnm_sorter back_sorter = p_back_world->get_sorter();
		S_tnm_sorter front_sorter = p_front_world->get_sorter();

		// フロントがオーダー内ならワイプする
		if ((begin_sorter <= front_sorter && front_sorter <= end_sorter))	{
			p_next_world->copy(p_front_world);

			// フロントにワイプコピーが立っていなければワイプする
			// バックにワイプ消去が立っていればワイプする
			if (p_front_world->get_wipe_copy() == 0 || p_back_world->get_wipe_erase() == 1)	{
				p_front_world->copy(p_back_world);
				p_back_world->reinit();
			}
		}
	}

	// エフェクト
	int effect_cnt = p_front->m_effect_list.get_size();
	for (int i = 0; i < effect_cnt; i++)	{
		C_elm_effect* p_back_effect = &p_back->m_effect_list[i];
		C_elm_effect* p_front_effect = &p_front->m_effect_list[i];
		C_elm_effect* p_next_effect = &p_next->m_effect_list[i];

		p_next_effect->copy(p_front_effect);

		// フロントにワイプコピーが立っていなければワイプする
		// バックにワイプ消去が立っていればワイプする
		if (p_front_effect->get_wipe_copy() == 0 || p_back_effect->get_wipe_erase() == 1)	{
			p_front_effect->copy(p_back_effect);
			p_back_effect->reinit();
		}
	}

	// クエイク
	int quake_cnt = p_front->m_quake_list.get_size();
	for (int i = 0; i < quake_cnt; i++)	{
		p_next->m_quake_list[i].copy(&p_front->m_quake_list[i]);
		p_front->m_quake_list[i].copy(&p_back->m_quake_list[i]);
		p_back->m_quake_list[i].reinit();
	}
}

// ****************************************************************
// ステージ：スプライトツリーの取得
// ================================================================
void C_elm_stage::get_sprite_tree(C_tree2<C_tnm_sprite *>& root, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter)
{
	// オブジェクト
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		if (m_object_list[i].is_use())	{
			S_tnm_sorter sorter = m_object_list[i].get_total_sorter();	// ソーターの合計値を取得
			if (begin_sorter <= sorter && sorter <= end_sorter)	{		// トップレベルの親だけはソーターを参照する
				m_object_list[i].get_sprite_tree(root);
			}
		}
	}

	// メッセージウィンドウ
	int mwnd_cnt = m_mwnd_list.get_size();
	if (!tnm_is_hide_mwnd())	{
		for (int i = 0; i < mwnd_cnt; i++)	{
			S_tnm_sorter sorter = m_mwnd_list[i].get_sorter();			// ここも合計値にしないと…
			if (begin_sorter <= sorter && sorter <= end_sorter)	{
				m_mwnd_list[i].get_sprite_tree(root);
			}
		}
	}

	// ボタン選択肢
	if (!tnm_is_hide_mwnd())	{
		S_tnm_sorter sorter(Gp_ini->mwnd.order, 0);
		if (begin_sorter <= sorter && sorter <= end_sorter)	{			// ここも合計値にしないと…
			m_btn_sel.get_sprite_tree(root);
		}
	}
}

// ****************************************************************
// ステージリスト：初期化
// ================================================================
void C_elm_stage_list::init(S_element element, CTSTR& name, CTSTR& parent_name, int size, bool extend_enable, bool use_ini, C_tnm_element* p_parent)
{
	// 固定パラメータ
	m_parent_element_name = parent_name;
	m_use_ini = use_ini;

	// 基底クラスの初期化
	C_elm_list_ex<C_elm_stage>::init(element, FM_STAGELIST, name, size, extend_enable, p_parent);
}

// ****************************************************************
// ステージリスト：終了
// ================================================================
void C_elm_stage_list::finish()
{
	int stage_cnt = (int)m_sub.size();
	for (int i = 0; i < stage_cnt; i++)
		m_sub[i].finish();
}

// ****************************************************************
// ステージリスト：サブの初期化
// ================================================================
void C_elm_stage_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)	{
		TSTR element_name;
		switch (i)	{
			case TNM_STAGE_BACK:	element_name = m_parent_element_name + _T("back");	break;
			case TNM_STAGE_FRONT:	element_name = m_parent_element_name + _T("front");	break;
			case TNM_STAGE_NEXT:	element_name = m_parent_element_name + _T("next");	break;
		}

		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), element_name, m_use_ini);
	}
}

// ****************************************************************
// ステージリスト：サブの再初期化
// ================================================================
void C_elm_stage_list::_reinit(int begin, int end, bool restruct_flag)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit(restruct_flag);
}

// ****************************************************************
// ステージリスト：サブのセーブ
// ================================================================
void C_elm_stage_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// ステージリスト：サブのロード
// ================================================================
void C_elm_stage_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}

