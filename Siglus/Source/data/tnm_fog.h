#pragma		once

#include	"element/elm_int_event.h"

class	C_tnm_save_stream;

// ****************************************************************
// フォグ
// ================================================================
class C_tnm_fog
{
public:
	void	init_local();
	void	save_local(C_tnm_save_stream& stream);
	void	load_local(C_tnm_save_stream& stream);
	void	update_time(int past_game_time, int past_real_time);
	void	frame();

	// フォグを作成
	void	create_fog(CTSTR& name);

	// パラメータを設定／取得
	void	set_fog_x(int fog_x)		{	m_fog_x.set_value(fog_x);	}
	void	set_fog_near(int fog_near)	{	m_fog_near = fog_near;		}
	void	set_fog_far(int fog_far)	{	m_fog_far = fog_far;		}
	int		get_fog_x()					{	return m_fog_x.get_value();	}
	int		get_fog_near()				{	return m_fog_near;			}
	int		get_fog_far()				{	return m_fog_far;			}

	// イベント
	int		get_total_x()				{	return m_fog_x.get_total_value();	}

	// イベント本体
	C_elm_int_event&	x_eve()			{	return m_fog_x;		}

	// パラメータを取得
	TSTR				get_fog_name();
	BSP<C_d3d_album>	get_fog_album();

private:

	// 再構築
	void	restruct();

	// パラメータ
	TSTR				m_fog_name;
	C_elm_int_event		m_fog_x;
	int					m_fog_near;
	int					m_fog_far;

	// 作業パラメータ
	BSP<C_d3d_album>	m_album;
};


