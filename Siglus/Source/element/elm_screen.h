#pragma		once

#include	"data/tnm_sorter.h"
#include	"data/tnm_element.h"
#include	"element/elm_list.h"
#include	"element/elm_int_event.h"

class	C_tnm_save_stream;

// ****************************************************************
// シェイク
// ================================================================
struct S_tnm_shake
{
	void	init();
	void	start(int shake_no, bool wait_flag);
	void	copy(const S_tnm_shake* src);
	void	end();
	void	update_time(int past_time);
	void	frame();

	// パラメータ
	int		m_no;				// シェイク番号
	int		m_cur_time;			// シェイクの現在時刻

	// 作業値
	int		m_cur_shake_x;		// 現在のシェイクＸ
	int		m_cur_shake_y;		// 現在のシェイクＹ
};

// ****************************************************************
// クエイク
// ================================================================

// 定数
const int	TNM_QUAKE_TYPE_NONE = -1;
const int	TNM_QUAKE_TYPE_VEC = 0;		// 上、右上、右、右下、下、左下、左、左上
const int	TNM_QUAKE_TYPE_DIR = 1;		// 上下、左右
const int	TNM_QUAKE_TYPE_ZOOM = 2;
const int	TNM_QUAKE_TYPE_ROTATE = 3;

const int	TNM_QUAKE_VEC_U = 0;
const int	TNM_QUAKE_VEC_RU = 1;
const int	TNM_QUAKE_VEC_R = 2;
const int	TNM_QUAKE_VEC_RD = 3;
const int	TNM_QUAKE_VEC_D = 4;
const int	TNM_QUAKE_VEC_LD = 5;
const int	TNM_QUAKE_VEC_L = 6;
const int	TNM_QUAKE_VEC_LU = 7;

// 通常パラメータ
struct C_elm_quake_param
{
	int		type;				// クエイクタイプ
	int		vec;				// 向き
	int		power;				// クエイクの強さ
	int		cur_time;			// クエイク現在時間
	int		total_time;			// クエイク合計時間
	int		end_flag;			// クエイク停止フラグ
	int		end_cur_time;		// クエイク停止現在時間
	int		end_total_time;		// クエイク停止合計時間
	int		cnt;				// クエイク回数
	int		end_cnt;			// クエイク終了回数
	int		center_x;			// クエイク中心Ｘ
	int		center_y;			// クエイク中心Ｙ
	int		begin_order;		// 開始オーダー
	int		end_order;			// 終了オーダー
};

// 一時パラメータ
struct C_elm_quake_work_param
{
	int		cur_pos_x;			// 現在の位置Ｘ
	int		cur_pos_y;			// 現在の位置Ｙ
	int		cur_scale;			// 現在のスケール
	int		cur_rotate;			// 現在の回転
};

// クエイク
class C_elm_quake : public C_tnm_element
{
public:

	// エレメント基本操作
	void	init(S_element element, CTSTR& name);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);
	void	copy(const C_elm_quake* p_src);
	void	update_time(int past_time);
	void	frame();
	void	get_rp(S_d3d_render_param* p_rp);
	bool	is_quake_doing()	{	return m_param.type >= 0;	}

	// スクリプトコマンド
	void	start_vec(int vec, int power, int time, int cnt, int end_cnt, int begin_order, int end_order, bool wait_flag, bool key_skip_flag);
	void	start_dir(int vec, int power, int time, int cnt, int end_cnt, int begin_order, int end_order, bool wait_flag, bool key_skip_flag);
	void	start_zoom(int power, int time, int cnt, int end_cnt, int center_x, int center_y, int begin_order, int end_order, bool wait_flag, bool key_flag);
	void	end(int time);
	void	wait(bool key_skip_flag);
	int		check();

	// パラメータ取得
	int		get_type() const		{	return m_param.type;			}
	int		get_begin_order() const	{	return m_param.begin_order;		}
	int		get_end_order() const	{	return m_param.end_order;		}

private:

	// パラメータ
	C_elm_quake_param		m_param;
	C_elm_quake_work_param	m_work;
};

// ****************************************************************
// クエイクリスト
// ================================================================
class C_elm_quake_list : public C_elm_list<C_elm_quake>
{
protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};

// ****************************************************************
// エフェクトパラメータ
// ================================================================

// 通常パラメータ
struct C_elm_effect_param
{
	C_elm_int_event		x;
	C_elm_int_event		y;
	C_elm_int_event		z;
	C_elm_int_event		mono;
	C_elm_int_event		reverse;
	C_elm_int_event		bright;
	C_elm_int_event		dark;
	C_elm_int_event		color_r;
	C_elm_int_event		color_g;
	C_elm_int_event		color_b;
	C_elm_int_event		color_rate;
	C_elm_int_event		color_add_r;
	C_elm_int_event		color_add_g;
	C_elm_int_event		color_add_b;

	S_tnm_sorter		begin_sorter;	// 開始ソーター
	S_tnm_sorter		end_sorter;		// 終了ソーター
	int					wipe_copy;		// ワイプコピー
	int					wipe_erase;		// ワイプ消滅
};

// ****************************************************************
// エフェクト
// ================================================================
class C_elm_effect : public C_tnm_element
{
public:
	void	init(S_element element, CTSTR& name);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);
	void	copy(const C_elm_effect* p_src);
	void	update_time(int past_game_time, int past_real_time);
	void	frame();

	bool	is_use();	// 使っているかを判定
	void	get_rp(S_d3d_render_param* p_rp);

	// パラメータを設定
	void	set_pos_x(int value)		{	m_param.x.set_value(value);								}
	void	set_pos_y(int value)		{	m_param.y.set_value(value);								}
	void	set_pos_z(int value)		{	m_param.z.set_value(value);								}
	void	set_mono(int value)			{	m_param.mono.set_value(limit(0, value, 255));			}
	void	set_reverse(int value)		{	m_param.reverse.set_value(limit(0, value, 255));		}
	void	set_bright(int value)		{	m_param.bright.set_value(limit(0, value, 255));			}
	void	set_dark(int value)			{	m_param.dark.set_value(limit(0, value, 255));			}
	void	set_color_r(int value)		{	m_param.color_r.set_value(limit(0, value, 255));		}
	void	set_color_g(int value)		{	m_param.color_g.set_value(limit(0, value, 255));		}
	void	set_color_b(int value)		{	m_param.color_b.set_value(limit(0, value, 255));		}
	void	set_color_rate(int value)	{	m_param.color_rate.set_value(limit(0, value, 255));		}
	void	set_color_add_r(int value)	{	m_param.color_add_r.set_value(limit(0, value, 255));	}
	void	set_color_add_g(int value)	{	m_param.color_add_g.set_value(limit(0, value, 255));	}
	void	set_color_add_b(int value)	{	m_param.color_add_b.set_value(limit(0, value, 255));	}
	void	set_begin_order(int value)	{	m_param.begin_sorter.order = value;						}
	void	set_begin_layer(int value)	{	m_param.begin_sorter.layer = value;						}
	void	set_end_order(int value)	{	m_param.end_sorter.order = value;						}
	void	set_end_layer(int value)	{	m_param.end_sorter.layer = value;						}
	void	set_wipe_copy(int value)	{	m_param.wipe_copy = value ? 1 : 0;						}
	void	set_wipe_erase(int value)	{	m_param.wipe_erase = value ? 1 : 0;						}

	// パラメータを取得
	int		get_pos_x() const				{	return m_param.x.get_value();			}
	int		get_pos_y() const				{	return m_param.y.get_value();			}
	int		get_pos_z() const				{	return m_param.z.get_value();			}
	int		get_mono() const				{	return m_param.mono.get_value();		}
	int		get_reverse() const				{	return m_param.reverse.get_value();		}
	int		get_bright() const				{	return m_param.bright.get_value();		}
	int		get_dark() const				{	return m_param.dark.get_value();		}
	int		get_color_r() const				{	return m_param.color_r.get_value();		}
	int		get_color_g() const				{	return m_param.color_g.get_value();		}
	int		get_color_b() const				{	return m_param.color_b.get_value();		}
	int		get_color_rate() const			{	return m_param.color_rate.get_value();	}
	int		get_color_add_r() const			{	return m_param.color_add_r.get_value();	}
	int		get_color_add_g() const			{	return m_param.color_add_g.get_value();	}
	int		get_color_add_b() const			{	return m_param.color_add_b.get_value();	}
	int		get_begin_order() const			{	return m_param.begin_sorter.order;		}
	int		get_begin_layer() const			{	return m_param.begin_sorter.layer;		}
	int		get_end_order() const			{	return m_param.end_sorter.order;		}
	int		get_end_layer() const			{	return m_param.end_sorter.layer;		}
	int		get_wipe_copy() const			{	return m_param.wipe_copy;				}
	int		get_wipe_erase() const			{	return m_param.wipe_erase;				}

	S_tnm_sorter	get_begin_sorter() const	{	return m_param.begin_sorter;			}
	S_tnm_sorter	get_end_sorter() const		{	return m_param.end_sorter;				}

	// イベントの計算値を取得
	int		get_x_total_value() const				{	return m_param.x.get_total_value();				}
	int		get_y_total_value() const				{	return m_param.y.get_total_value();				}
	int		get_z_total_value() const				{	return m_param.z.get_total_value();				}
	int		get_mono_total_value() const			{	return m_param.mono.get_total_value();			}
	int		get_reverse_total_value() const			{	return m_param.reverse.get_total_value();		}
	int		get_bright_total_value() const			{	return m_param.bright.get_total_value();		}
	int		get_dark_total_value() const			{	return m_param.dark.get_total_value();			}
	int		get_color_r_total_value() const			{	return m_param.color_r.get_total_value();		}
	int		get_color_g_total_value() const			{	return m_param.color_g.get_total_value();		}
	int		get_color_b_total_value() const			{	return m_param.color_b.get_total_value();		}
	int		get_color_rate_total_value() const		{	return m_param.color_rate.get_total_value();	}
	int		get_color_add_r_total_value() const		{	return m_param.color_add_r.get_total_value();	}
	int		get_color_add_g_total_value() const		{	return m_param.color_add_g.get_total_value();	}
	int		get_color_add_b_total_value() const		{	return m_param.color_add_b.get_total_value();	}

	// 子エレメント
	C_elm_int_event&	x()				{	return m_param.x;			}
	C_elm_int_event&	y()				{	return m_param.y;			}
	C_elm_int_event&	z()				{	return m_param.z;			}
	C_elm_int_event&	mono()			{	return m_param.mono;		}
	C_elm_int_event&	reverse()		{	return m_param.reverse;		}
	C_elm_int_event&	bright()		{	return m_param.bright;		}
	C_elm_int_event&	dark()			{	return m_param.dark;		}
	C_elm_int_event&	color_r()		{	return m_param.color_r;		}
	C_elm_int_event&	color_g()		{	return m_param.color_g;		}
	C_elm_int_event&	color_b()		{	return m_param.color_b;		}
	C_elm_int_event&	color_rate()	{	return m_param.color_rate;	}
	C_elm_int_event&	color_add_r()	{	return m_param.color_add_r;	}
	C_elm_int_event&	color_add_g()	{	return m_param.color_add_g;	}
	C_elm_int_event&	color_add_b()	{	return m_param.color_add_b;	}

private:

	// パラメータ
	C_elm_effect_param	m_param;
};

// ****************************************************************
// エフェクトリスト
// ================================================================
class C_elm_effect_list : public C_elm_list<C_elm_effect>
{
protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};

// ****************************************************************
// スクリーン
// ================================================================
class C_elm_screen : public C_tnm_element
{
public:
	void	init(S_element element, CTSTR& name);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);
	void	update_time(int past_game_time, int past_real_time);
	void	frame();

	// 子エレメント
	C_elm_effect_list&	effect_list()	{	return m_effect_list;	}
	S_tnm_shake&		shake()			{	return m_shake;			}
	C_elm_quake_list&	quake_list()	{	return m_quake_list;	}

	// 子エレメント（const）
	const C_elm_effect_list&	effect_list() const	{	return m_effect_list;	}
	const S_tnm_shake&			shake() const		{	return m_shake;			}
	const C_elm_quake_list&		quake_list() const	{	return m_quake_list;	}

private:

	// 子エレメント
	C_elm_effect_list	m_effect_list;
	S_tnm_shake			m_shake;
	C_elm_quake_list	m_quake_list;
};






