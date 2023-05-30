#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_world.h"


// ****************************************************************
// ワールド：初期化
// ================================================================
void C_elm_world::init(int world_no, S_element element, CTSTR& name)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_WORLD, name, NULL);

	// デフォルトパラメータを設定
	m_def.world_no = world_no;

	// 再初期化
	reinit();
}

// ****************************************************************
// ワールド：再初期化
// ================================================================
void C_elm_world::reinit()
{
	m_param.mode = TNM_CAMERA_MODE_3D;
	m_param.camera_eye_x.init(0);
	m_param.camera_eye_y.init(0);
	m_param.camera_eye_z.init(-1000);
	m_param.camera_pint_x.init(0);
	m_param.camera_pint_y.init(0);
	m_param.camera_pint_z.init(0);
	m_param.camera_up_x.init(0);
	m_param.camera_up_y.init(1);	// 上向き！
	m_param.camera_up_z.init(0);
	m_param.camera_view_angle = 450;
	m_param.mono = 0;

	m_param.order = 0;
	m_param.layer = 0;
	m_param.wipe_copy = 0;
	m_param.wipe_erase = 0;

	// 回転イベント
	m_param.camera_eye_xz_eve.m_loop_type = E_tnm_int_event_loop_type_none;
}

// ****************************************************************
// ワールド：セーブ
// ================================================================
void C_elm_world::save(C_tnm_save_stream& stream)
{
	// パラメータ
	stream.save(m_param);
}

// ****************************************************************
// ワールド：ロード
// ================================================================
void C_elm_world::load(C_tnm_save_stream& stream)
{
	// パラメータ
	stream.load(m_param);
}

// ****************************************************************
// エフェクト：コピー
// ================================================================
void C_elm_world::copy(const C_elm_world* p_src)
{
	// パラメータ
	m_param = p_src->m_param;
}

// ****************************************************************
// ワールド：時間を更新
// ================================================================
void C_elm_world::update_time(int past_game_time, int past_real_time)
{
	// イベントの時間を更新
	m_param.camera_eye_x.update_time(past_game_time, past_real_time);
	m_param.camera_eye_y.update_time(past_game_time, past_real_time);
	m_param.camera_eye_z.update_time(past_game_time, past_real_time);
	m_param.camera_pint_x.update_time(past_game_time, past_real_time);
	m_param.camera_pint_y.update_time(past_game_time, past_real_time);
	m_param.camera_pint_z.update_time(past_game_time, past_real_time);
	m_param.camera_up_x.update_time(past_game_time, past_real_time);
	m_param.camera_up_y.update_time(past_game_time, past_real_time);
	m_param.camera_up_z.update_time(past_game_time, past_real_time);

	// 回転イベント
	m_param.camera_eye_xz_eve.m_cur_time += past_game_time;
}

// ****************************************************************
// ワールド：フレーム処理
// ================================================================
inline void trans_2(double Sx, double Sy, int T, double Ex, double Ey, int now, double* ret_Bx, double* ret_By)
{
	double Edx,Edy,r,g,S_theta,E_theta,theta,Bx,By;

	r = Sx;
	Edx = Ex;
	Edy = sqrt(r * r - Ex * Ex);
	g = Ey / Edy;

	S_theta = atan2(Sy, Sx);
	E_theta = abs(Edx) < DBL_EPSILON ? D3DX_PI / 2 : atan2(Edy, Edx);
	theta = ::linear(now, 0, S_theta, T, E_theta);
	
	Bx = r * cos(theta);
	By = g * r * sin(theta);

	*ret_Bx = Bx;
	*ret_By = By;
}

inline void trans_1(double Px, double Py, double theta, double Sx, double Sy, double* ret_Sx, double* ret_Sy)
{
	double Sx2 = Sx - Px;
	double Sy2 = Sy - Py;
	double Sx3 = Sx2 * cos(- theta) - Sy2 * sin(- theta);
	double Sy3 = Sx2 * sin(- theta) + Sy2 * cos(- theta);

	*ret_Sx = Sx3;
	*ret_Sy = Sy3;
}

inline void trans_3(double Px, double Py, double theta, double Sx, double Sy, double* ret_Sx, double* ret_Sy)
{
	double Sx2 = Sx * cos(theta) - Sy * sin(theta);
	double Sy2 = Sx * sin(theta) + Sy * cos(theta);
	double Sx3 = Sx2 + Px;
	double Sy3 = Sy2 + Py;

	*ret_Sx = Sx3;
	*ret_Sy = Sy3;
}

void C_elm_world::frame_xz_rotate()
{
	// パラメータを取得
	int end_time = m_param.camera_eye_xz_eve.m_end_time;
	int cur_time = m_param.camera_eye_xz_eve.m_cur_time - m_param.camera_eye_xz_eve.m_delay_time;

	// ワンショットの場合は終了時刻を過ぎていたらイベント終了
	if (m_param.camera_eye_xz_eve.m_loop_type == E_tnm_int_event_loop_type_oneshot)	{
		if (cur_time - end_time >= 0)	{
			m_param.camera_eye_xz_eve.m_loop_type = E_tnm_int_event_loop_type_none;
			return;
		}
	}

	// 開始時刻になっていなければ開始値
	if (cur_time <= 0)	{
		m_param.camera_eye_x.m_cur_value = m_param.camera_eye_x.m_start_value;
		m_param.camera_eye_z.m_cur_value = m_param.camera_eye_z.m_start_value;
		return;
	}

	// ループの場合の時間補正
	if (m_param.camera_eye_xz_eve.m_loop_type == E_tnm_int_event_loop_type_loop)	{
		cur_time %= end_time;
	}

	// ターンの場合の時間補正
	if (m_param.camera_eye_xz_eve.m_loop_type == E_tnm_int_event_loop_type_turn)	{
		cur_time %= end_time * 2;

		if (cur_time - end_time > 0)	{
			cur_time = end_time - (cur_time - end_time);
		}
	}

	if (false);
	else if (m_param.camera_eye_xz_eve.m_speed_type == 0)	{	// 線形
		// NOP
	}
	else if (m_param.camera_eye_xz_eve.m_speed_type == 1)	{	// 加速
		cur_time = (int)((double)cur_time * cur_time / end_time);
	}
	else if (m_param.camera_eye_xz_eve.m_speed_type == 2)	{	// 減速
		cur_time = (int)(- (double)(cur_time - end_time) * (cur_time - end_time) / end_time + end_time);
	}

	double Px = (double)m_param.camera_pint_x.get_total_value();
	double Pz = (double)m_param.camera_pint_z.get_total_value();
	double Sx = (double)m_param.camera_eye_x.m_start_value;
	double Sz = (double)m_param.camera_eye_z.m_start_value;
	double Ex = (double)m_param.camera_eye_x.m_end_value;
	double Ez = (double)m_param.camera_eye_z.m_end_value;

	double Sdx = Sx - Px;
	double Sdz = Sz - Pz;
	double Edx = Ex - Px;
	double Edz = Ez - Pz;

	double S_len = sqrt(Sdx * Sdx + Sdz * Sdz);
	double E_len = sqrt(Edx * Edx + Edz * Edz);
	double T_len = linear(cur_time, 0, S_len, end_time, E_len);

	double S_theta = atan2(Sdz, Sdx);
	double E_theta = atan2(Edz, Edx);
	if (abs(S_theta - E_theta) > D3DX_PI)	{	// 回転が半周を超えた場合は逆回転にする（cos の境界を越えたときの対策です。）
		if (E_theta < 0)	{
			E_theta += D3DX_PI * 2;
		}
		else	{
			E_theta -= D3DX_PI * 2;
		}
	}

	double T_theta = linear(cur_time, 0, S_theta, end_time, E_theta);


	double tmp_x = T_len * cos(T_theta) + Px;
	double tmp_z = T_len * sin(T_theta) + Pz;

	m_param.camera_eye_x.m_cur_value = (int)tmp_x;
	m_param.camera_eye_z.m_cur_value = (int)tmp_z;

}

void C_elm_world::frame()
{
	// イベントのフレーム処理
	m_param.camera_eye_x.frame();
	m_param.camera_eye_y.frame();
	m_param.camera_eye_z.frame();
	m_param.camera_pint_x.frame();
	m_param.camera_pint_y.frame();
	m_param.camera_pint_z.frame();
	m_param.camera_up_x.frame();
	m_param.camera_up_y.frame();
	m_param.camera_up_z.frame();

	// 回転イベント
	if (m_param.camera_eye_xz_eve.m_loop_type != E_tnm_int_event_loop_type_none)	{

#if 1
		frame_xz_rotate();

#else

		double Bx, Bz;	// 出力

		double Px = (double)m_param.camera_pint_x.get_total_value();
		double Pz = (double)m_param.camera_pint_z.get_total_value();
		double Sx = (double)m_param.camera_eye_x.m_start_value;
		double Sz = (double)m_param.camera_eye_z.m_start_value;
		double Ex = (double)m_param.camera_eye_x.m_end_value;
		double Ez = (double)m_param.camera_eye_z.m_end_value;
		double theta = atan2(Sz - Pz, Sx - Px);
		int T = m_param.camera_eye_xz_eve.m_end_time;
		int now = m_param.camera_eye_xz_eve.m_cur_time;

		double L2_Sx, L2_Sz, L2_Ex, L2_Ez, L2_Bx, L2_Bz;

		trans_1(Px, Pz, theta, Sx, Sz, &L2_Sx, &L2_Sz);
		trans_1(Px, Pz, theta, Ex, Ez, &L2_Ex, &L2_Ez);
		trans_2(L2_Sx, L2_Sz, T, L2_Ex, L2_Ez, now, &L2_Bx, &L2_Bz);
		trans_3(Px, Pz, theta, L2_Bx, L2_Bz, &Bx, &Bz);

		m_param.camera_eye_x.m_cur_value = (int)Bx;
		m_param.camera_eye_z.m_cur_value = (int)Bz;

#endif
	}
}

// ****************************************************************
// ワールド：カメラの回転イベント
// ================================================================
void C_elm_world::set_camera_eve_xz_rotate(int x, int z, int time, int rep_time, int speed_type)
{
	m_param.camera_eye_xz_eve.m_loop_type = E_tnm_int_event_loop_type_oneshot;
	m_param.camera_eye_xz_eve.m_cur_time = 0;
	m_param.camera_eye_xz_eve.m_end_time = time;
	m_param.camera_eye_xz_eve.m_delay_time = rep_time;
	m_param.camera_eye_xz_eve.m_speed_type = speed_type;

	m_param.camera_eye_x.m_start_value = m_param.camera_eye_x.m_value;
	m_param.camera_eye_z.m_start_value = m_param.camera_eye_z.m_value;
	m_param.camera_eye_x.m_end_value = x;
	m_param.camera_eye_z.m_end_value = z;

	// 値を設定
	m_param.camera_eye_x.set_value(x);
	m_param.camera_eye_z.set_value(z);
}

// ****************************************************************
// ワールドリスト：サブの初期化
// ================================================================
void C_elm_world_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(i, get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// ワールドリスト：サブの再初期化
// ================================================================
void C_elm_world_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// ワールドリスト：サブのセーブ
// ================================================================
void C_elm_world_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// ワールドリスト：サブのロード
// ================================================================
void C_elm_world_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
