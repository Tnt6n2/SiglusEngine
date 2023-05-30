#pragma		once

#include	"data/tnm_def.h"
#include	"data/tnm_guid.h"
#include	"data/tnm_sorter.h"

// ****************************************************************
// ボタン／ボタンマネージャ
// ================================================================
//		一括してボタンを判定するためのシステムです。
//		各エレメントは、ボタン（C_tnm_btn）を持っています。
// ================================================================


// ****************************************************************
// ボタン
// ================================================================
//		当たり判定を行うオブジェクトに持たせます。
// ================================================================
class C_tnm_btn
{
	friend	class	C_tnm_btn_mng;

public:
	C_tnm_btn();
	~C_tnm_btn();

	void	init();											// 初期化
	void	frame_init();									// 初期化
	void	set_rect(C_rect rect);							// 矩形を設定
	void	set_rect_2(bool use, C_rect rect);				// 矩形その２を設定
	void	set_center(C_point center);						// 中心を設定
	void	set_rotate(int rotate);							// 回転を設定
	void	set_sorter(S_tnm_sorter sorter);				// ソーターを設定
	void	set_push_keep(bool push_keep);					// プッシュキープを設定
	void	set_param(C_rect rect, S_tnm_sorter sorter);	// パラメータを設定
	void	set_texture(BSP<C_d3d_texture> texture);		// テクスチャを設定
	bool	hit_test(C_point pos);							// ヒットテスト

	bool	is_decided()		{ return m_is_decided; }
	void	create_guid()		{ m_guid.create_guid(); }
	int		get_guid()			{ return m_guid.get_guid(); }

	const	C_rect& rect() const			{ return m_rect; }
	const	S_tnm_sorter& sorter() const	{ return m_sorter; }

private:
	C_tnm_guid		m_guid;			// GUID
	C_rect			m_rect;			// 当たり矩形
	C_rect			m_rect_2;		// 当たり矩形その２
	C_point			m_center;		// 矩形の回転
	int				m_rotate;		// 矩形の回転
	S_tnm_sorter	m_sorter;		// ソーター
	bool			m_rect_2_use;	// 当たり矩形その２を使う
	bool			m_push_keep;	// 押されたをキープ
	bool			m_is_decided;	// 決定された

	BSP<C_d3d_texture>	m_texture;		// テクスチャ
};

// ****************************************************************
// ボタンマネージャ
// ================================================================
class C_tnm_btn_mng
{
public:
	void	init();
	bool	frame_init();
	bool	hit_test_proc();

	// ボタンを登録する
	void	regist_button(C_tnm_btn* btn);
	int		get_btn_cnt()	{	return (int)m_btn_list.size();	}

	// アクティブなボタンのパラメータを取得
	int				get_active_btn_guid()	{	return m_active_btn_guid;		}
	S_tnm_sorter	get_active_btn_sorter()	{	return m_active_btn_sorter;		}
	int				get_active_btn_state()	{	return m_active_btn_state;		}

	// ボタンのステートを求める
	bool	is_active(C_tnm_btn* btn);
	int		get_hit_state(C_tnm_btn* btn);

	// 現在当たっているかどうかなどを判定
	bool	is_hit(C_tnm_btn* btn)			{	return m_active_btn_guid >= 0 && btn->m_guid.get_guid() == m_active_btn_guid && m_active_btn_state == TNM_BTN_STATE_HIT;	}
	bool	is_pushed(C_tnm_btn* btn)		{	return m_active_btn_guid >= 0 && btn->m_guid.get_guid() == m_active_btn_guid && m_active_btn_state == TNM_BTN_STATE_PUSH;	}
	bool	is_decided(C_tnm_btn* btn)		{	return btn->is_decided();	}

	// ボタンが今回当たったかどうかなどを判定
	bool	is_hit_this_frame(C_tnm_btn* btn)		{	return m_hit_this_frame && m_active_btn_guid >= 0 && btn->m_guid.get_guid() == m_active_btn_guid;			}	// ボタン：当たった
	bool	is_pushed_this_frame(C_tnm_btn* btn)	{	return m_pushed_this_frame && m_active_btn_guid >= 0 && btn->m_guid.get_guid() == m_active_btn_guid;		}	// ボタン：押された
	bool	is_decided_this_frame(C_tnm_btn* btn)	{	return m_decided_this_frame && m_active_btn_guid >= 0 && btn->m_guid.get_guid() == m_active_btn_guid;		}	// ボタン：決定した
	bool	is_hit_this_frame()						{	return m_hit_this_frame;				}
	bool	is_pushed_this_frame()					{	return m_pushed_this_frame;				}
	bool	is_decided_this_frame()					{	return m_decided_this_frame;			}

private:
	int				m_active_btn_guid;			// アクティブなボタンのGUID
	S_tnm_sorter	m_active_btn_sorter;		// アクティブなボタンのソーター
	int				m_active_btn_state;			// アクティブなボタンのステート

	bool			m_last_mouse_push_flag;		// 前回のマウスを押しているフラグ
	int				m_last_active_group_guid;	// 前回のアクティブなグループのGUID
	int				m_last_active_btn_guid;		// 前回のアクティブなボタンのGUID
	S_tnm_sorter	m_last_active_btn_sorter;	// 前回のアクティブなボタンのソーター
	int				m_last_active_btn_state;	// 前回のアクティブなボタンのステート

	bool			m_hit_this_frame;			// 今回当たった
	bool			m_pushed_this_frame;		// 今回押された
	bool			m_decided_this_frame;		// 今回決定された

	ARRAY<C_tnm_btn *>		m_btn_list;
};

