#pragma		once

#include	"data/tnm_sorter.h"
#include	"element/elm_list.h"
#include	"element/elm_int_event.h"

class	C_tnm_save_stream;

const int	TNM_CAMERA_MODE_2D = 0;
const int	TNM_CAMERA_MODE_3D = 1;

// ****************************************************************
// ワールドの固定パラメータ
// ================================================================
struct S_tnm_world_def_param
{
	int			world_no;
};

// ****************************************************************
// ワールドのパラメータ
// ================================================================
struct S_tnm_camera_event_2
{
	int		m_cur_time;
	int		m_end_time;
	int		m_delay_time;
	int		m_loop_type;
	int		m_speed_type;
};

struct S_tnm_world_param
{
	int					mode;
	C_elm_int_event		camera_eye_x;
	C_elm_int_event		camera_eye_y;
	C_elm_int_event		camera_eye_z;
	C_elm_int_event		camera_pint_x;
	C_elm_int_event		camera_pint_y;
	C_elm_int_event		camera_pint_z;
	C_elm_int_event		camera_up_x;
	C_elm_int_event		camera_up_y;
	C_elm_int_event		camera_up_z;
	int					camera_view_angle;
	int					mono;

	S_tnm_camera_event_2	camera_eye_xz_eve;

	int					order;
	int					layer;
	int					wipe_copy;
	int					wipe_erase;
};

// ****************************************************************
// ワールド
// ================================================================
class C_elm_world : public C_tnm_element
{
public:

	// エレメントの基本操作
	void	init(int world_no, S_element element, CTSTR& name);		// 初期化
	void	reinit();												// 再初期化
	void	save(C_tnm_save_stream& stream);						// セーブ
	void	load(C_tnm_save_stream& stream);						// ロード
	void	copy(const C_elm_world* p_src);
	void	update_time(int past_game_time, int past_real_time);	// 時間を更新
	void	frame();												// フレーム処理

	// パラメータを設定
	void	set_mode(int value)					{ m_param.mode = value; }
	void	set_camera_eye_x(int value)			{ m_param.camera_eye_x.set_value(value); }
	void	set_camera_eye_y(int value)			{ m_param.camera_eye_y.set_value(value); }
	void	set_camera_eye_z(int value)			{ m_param.camera_eye_z.set_value(value); }
	void	set_camera_pint_x(int value)		{ m_param.camera_pint_x.set_value(value); }
	void	set_camera_pint_y(int value)		{ m_param.camera_pint_y.set_value(value); }
	void	set_camera_pint_z(int value)		{ m_param.camera_pint_z.set_value(value); }
	void	set_camera_up_x(int value)			{ m_param.camera_up_x.set_value(value); }
	void	set_camera_up_y(int value)			{ m_param.camera_up_y.set_value(value); }
	void	set_camera_up_z(int value)			{ m_param.camera_up_z.set_value(value); }
	void	set_camera_view_angle(int value)	{ m_param.camera_view_angle = value; }
	void	set_mono(int value)					{ m_param.mono = limit(0, value, 255); }
	void	set_order(int value)				{ m_param.order = value ? 1 : 0; }
	void	set_layer(int value)				{ m_param.layer = value ? 1 : 0; }
	void	set_wipe_copy(int value)			{ m_param.wipe_copy = value ? 1 : 0; }
	void	set_wipe_erase(int value)			{ m_param.wipe_erase = value ? 1 : 0; }

	void	set_camera_eye(int x, int y, int z)
	{
		m_param.camera_eye_x.set_value(x);	
		m_param.camera_eye_y.set_value(y);	
		m_param.camera_eye_z.set_value(z);	
	}
	void	set_camera_pint(int x, int y, int z)
	{
		m_param.camera_pint_x.set_value(x);	
		m_param.camera_pint_y.set_value(y);	
		m_param.camera_pint_z.set_value(z);
	}
	void	set_camera_up(int x, int y, int z)
	{
		m_param.camera_up_x.set_value(x);	
		m_param.camera_up_y.set_value(y);	
		m_param.camera_up_z.set_value(z);
	}

	// パラメータを取得
	int		get_world_no() const				{ return m_def.world_no; }
	int		get_mode() const					{ return m_param.mode; }
	int		get_camera_eye_x() const			{ return m_param.camera_eye_x.get_value(); }
	int		get_camera_eye_y() const			{ return m_param.camera_eye_y.get_value(); }
	int		get_camera_eye_z() const			{ return m_param.camera_eye_z.get_value(); }
	int		get_camera_pint_x() const			{ return m_param.camera_pint_x.get_value(); }
	int		get_camera_pint_y() const			{ return m_param.camera_pint_y.get_value(); }
	int		get_camera_pint_z() const			{ return m_param.camera_pint_z.get_value(); }
	int		get_camera_up_x() const				{ return m_param.camera_up_x.get_value(); }
	int		get_camera_up_y() const				{ return m_param.camera_up_y.get_value(); }
	int		get_camera_up_z() const				{ return m_param.camera_up_z.get_value(); }
	int		get_camera_view_angle() const		{ return m_param.camera_view_angle; }
	int		get_mono() const					{ return m_param.mono; }
	int		get_order() const					{ return m_param.order; }
	int		get_layer() const					{ return m_param.layer; }
	int		get_wipe_copy() const				{ return m_param.wipe_copy; }
	int		get_wipe_erase() const				{ return m_param.wipe_erase; }

	// イベントの最終値を取得
	int		get_camera_eye_total_x() const		{ return m_param.camera_eye_x.get_total_value(); }
	int		get_camera_eye_total_y() const		{ return m_param.camera_eye_y.get_total_value(); }
	int		get_camera_eye_total_z() const		{ return m_param.camera_eye_z.get_total_value(); }
	int		get_camera_pint_total_x() const		{ return m_param.camera_pint_x.get_total_value(); }
	int		get_camera_pint_total_y() const		{ return m_param.camera_pint_y.get_total_value(); }
	int		get_camera_pint_total_z() const		{ return m_param.camera_pint_z.get_total_value(); }
	int		get_camera_up_total_x() const		{ return m_param.camera_up_x.get_total_value(); }
	int		get_camera_up_total_y() const		{ return m_param.camera_up_y.get_total_value(); }
	int		get_camera_up_total_z() const		{ return m_param.camera_up_z.get_total_value(); }

	// 子エレメントを取得
	C_elm_int_event&	camera_eye_x_eve()		{ return m_param.camera_eye_x; }
	C_elm_int_event&	camera_eye_y_eve()		{ return m_param.camera_eye_y; }
	C_elm_int_event&	camera_eye_z_eve()		{ return m_param.camera_eye_z; }
	C_elm_int_event&	camera_pint_x_eve()		{ return m_param.camera_pint_x; }
	C_elm_int_event&	camera_pint_y_eve()		{ return m_param.camera_pint_y; }
	C_elm_int_event&	camera_pint_z_eve()		{ return m_param.camera_pint_z; }
	C_elm_int_event&	camera_up_x_eve()		{ return m_param.camera_up_x; }
	C_elm_int_event&	camera_up_y_eve()		{ return m_param.camera_up_y; }
	C_elm_int_event&	camera_up_z_eve()		{ return m_param.camera_up_z; }

	// ソーターの取得／設定
	void			set_sorter(S_tnm_sorter value)	{ m_param.order = value.order; m_param.layer = value.layer; }
	S_tnm_sorter	get_sorter()					{ return S_tnm_sorter(m_param.order, m_param.layer); }

	// 特殊イベント
	void		set_camera_eve_xz_rotate(int x, int z, int time, int rep_time, int speed_type);

private:
	S_tnm_world_def_param	m_def;
	S_tnm_world_param		m_param;

	void		frame_xz_rotate();
};

// ****************************************************************
// ワールドリスト
// ================================================================
class C_elm_world_list : public C_elm_list<C_elm_world>
{
protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};
