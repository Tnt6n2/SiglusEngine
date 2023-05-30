#pragma		once

#include	"elm_mwnd_key_icon.h"
#include	"elm_object.h"

// ****************************************************************
// メッセージウィンドウ：枠
// ================================================================
class C_elm_mwnd_waku
{
public:

	// 基本処理
	void	init(S_element element, CTSTR& name);				// 初期化
	void	reinit(bool restruct_flag);							// 再初期化
	void	finish();											// 終了
	void	save(C_tnm_save_stream& stream);					// セーブ
	void	load(C_tnm_save_stream& stream);					// ロード
	void	copy_all(C_elm_mwnd_waku* src, bool init_flag);		// コピー（全て）
	void	copy_waku(C_elm_mwnd_waku* src, bool init_flag);	// コピー（枠）
	void	copy_key_icon(C_elm_mwnd_waku* src);				// コピー（キー待ちアイコン）
	void	copy_face(C_elm_mwnd_waku* src);					// コピー（顔グラ）
	void	copy_object(C_elm_mwnd_waku* src);					// コピー（オブジェクト）
	void	set_template(int template_no, bool init_flag);		// テンプレートの値を設定

	void	update_time(int past_game_time, int past_real_time);	// 時間を進める
	void	frame_init();											// フレーム初期化処理
	void	frame(int cur_time, C_rect window_rect, const S_tnm_render_param* p_parent_tdp, bool face_appear_flag);		// フレーム処理
	void	do_frame_action();										// フレームアクション処理

	// スプライトツリーを構築
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);

	// 枠
	void	init_waku_file();
	void	set_waku_file(CTSTR& file);
	TSTR	get_waku_file()		{	return m_waku_file;	}

	// フィルター
	void	init_filter_file();
	void	set_filter_file(CTSTR& file);
	TSTR	get_filter_file()		{	return m_filter_file;	}

	// ボタン処理
	void	regist_button();								// ボタンを登録
	void	button_event();									// ボタンイベント処理

	// 顔グラ
	void	clear_face();
	void	set_face(int face_no, CTSTR& file_path);
	bool	is_exist_face();

	// キー待ちアイコン
	void	set_key_icon_appear(bool appear)	{	m_key_icon.set_appear(appear);	}
	void	set_key_icon_mode(int mode)			{	m_key_icon.set_mode(mode);		}
	void	set_key_icon_pos(C_point pos)		{	m_key_icon.set_pos(pos);		}

	// 取得／設定
	int					get_template_no()				{	return m_template_no;	}
	int					get_extend_type()				{	return m_extend_type;	}
	bool				set_extend_waku(C_rect* window_rect);
	C_elm_object_list&	button_list()					{	return m_btn_list;		}
	C_elm_object_list&	face_list()						{	return m_face_list;		}
	C_elm_object_list&	object_list()					{	return m_object_list;	}

private:

	void	init_work_variable();	// 処理変数初期化
	void	restruct_template();	// テンプレートを再構築
	void	restruct_waku();		// 枠を再構築
	void	restruct_filter();		// フィルターを再構築

	// ウィンドウステート
	struct STATE
	{
		C_rect	filter_margin;			// フィルターのマージン（ウィンドウからの座標）
		C_argb	filter_color;			// フィルターの色
		bool	filter_config_color;	// 環境設定のフィルター色を適用する
		bool	filter_config_tr;		// 環境設定のフィルター透明度を適用する
	};

	int					m_template_no;			// テンプレート番号
	S_element			m_element;				// エレメント
	TSTR				m_name;					// 名前
	int					m_extend_type;			// ウィンドウの可変タイプ
	TSTR				m_waku_file;			// 枠のファイル名
	TSTR				m_filter_file;			// フィルターのファイル名
	STATE				m_def;					// デフォルトのウィンドウステート
	STATE				m_cur;					// 現在のウィンドウステート

	C_elm_mwnd_key_icon		m_key_icon;			// キー待ちアイコン
	C_elm_object_list		m_btn_list;			// ボタン
	C_elm_object_list		m_face_list;		// 顔グラ
	C_elm_object_list		m_object_list;		// オブジェクト

	BSP<C_d3d_album>	m_album_waku;
	BSP<C_d3d_album>	m_album_filter;
	C_tnm_sprite		m_spr_waku;
	C_tnm_sprite		m_spr_filter;
};


