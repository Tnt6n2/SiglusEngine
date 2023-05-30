#pragma		once

#include	"data/tnm_tree.h"
#include	"data/tnm_sprite.h"

class	C_tnm_save_stream;
struct	S_tnm_render_param;

// ****************************************************************
// キー待ちアイコン
// ================================================================
class C_elm_mwnd_key_icon
{
public:

	void	init();									// 初期化
	void	reinit();								// 再初期化
	void	release();								// 解放
	void	save(C_tnm_save_stream& stream);		// セーブ
	void	load(C_tnm_save_stream& stream);		// ロード
	void	copy(C_elm_mwnd_key_icon* src);			// コピー
	void	set_template(int template_no);			// テンプレートを設定

	// フレーム初期化処理
	void	frame_init();
	// フレーム処理
	void	frame(int cur_time, C_rect window_rect, const S_tnm_render_param* p_parent_tdp);

	// スプライトツリーを取得
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);

	// 操作処理
	void	set_appear(bool appear);
	void	set_mode(int mode)		{	m_icon_mode = mode;		}
	void	set_pos(C_point pos)	{	m_icon_pos = pos;		}

private:

	// テンプレートを再構築
	void	restruct_template();

	// パラメータ
	int		m_template_no;

	// 作業変数
	bool	m_icon_appear;	// 表示フラグ
	int		m_icon_mode;	// 0:キー待ち、1:ページ待ち
	C_point	m_icon_pos;
	bool	m_anime_start_flag;
	int		m_anime_start_time;

	BSP<C_d3d_album>	m_album_key_icon;
	BSP<C_d3d_album>	m_album_page_icon;
	C_tnm_sprite		m_sprite_icon;
};
