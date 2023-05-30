#pragma		once

#include	"element/elm_object.h"
#include	"element/elm_group.h"
#include	"element/elm_mwnd.h"
#include	"element/elm_btn_sel.h"
#include	"element/elm_world.h"
#include	"element/elm_screen.h"

// ****************************************************************
// ステージ
// ================================================================
class C_elm_stage : public C_tnm_element
{
public:
	void	init(S_element element, CTSTR& name, bool use_ini);		// 初期化
	void	reinit(bool restruct_flag);								// 再初期化
	void	finish();												// 終了
	void	save(C_tnm_save_stream& stream);						// セーブ
	void	load(C_tnm_save_stream& stream);						// ロード
	void	frame_init();											// フレーム初期化
	void	update_time(int past_game_time, int past_real_time);	// 時間を更新
	bool	frame(bool draw_futoku, bool draw_shadow, int order);	// フレーム処理
	void	frame_action();											// フレームアクション処理
	void	button_frame_init();									// ボタンフレーム初期化
	void	regist_button(int min_order);							// ボタンを登録
	void	button_event(C_elm_mwnd* p_sel_mwnd);					// ボタンイベント
	void	copy(C_elm_stage* src);									// コピー

	// 描画処理
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter);		// スプライトツリーの取得

	C_elm_object_list	m_object_list;		// オブジェクト
	C_elm_group_list	m_group_list;		// グループ
	C_elm_mwnd_list		m_mwnd_list;		// メッセージウィンドウ
	C_elm_btn_select	m_btn_sel;			// ボタン選択肢
	C_elm_world_list	m_world_list;		// ワールド
	C_elm_effect_list	m_effect_list;		// エフェクト
	C_elm_quake_list	m_quake_list;		// クエイク
};

// ****************************************************************
// ステージリスト
// ================================================================
class C_elm_stage_list : public C_elm_list_ex<C_elm_stage>
{
public:
	void	init(S_element element, CTSTR& name, CTSTR& parent_name, int size, bool extend_enable, bool use_ini, C_tnm_element* p_parent);	// 初期化
	void	wipe(S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter);		// ワイプ
	void	finish();

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end, bool restruct_flag);		// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード

	// 固定パラメータ
	TSTR	m_parent_element_name;
	bool	m_use_ini;
};
