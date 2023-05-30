#include	"pch.h"
#include	"data/tnm_fog.h"
#include	"data/tnm_save_00.h"
#include	"engine/eng_graphics.h"

void C_tnm_fog::init_local()
{
	m_fog_name.clear();
	m_fog_x.init(0);
	m_fog_near = 0;
	m_fog_far = 0;
	m_album.reset();
}

void C_tnm_fog::save_local(C_tnm_save_stream& stream)
{
	stream.save(m_fog_name);
	stream.save(m_fog_x);
	stream.save(m_fog_near);
	stream.save(m_fog_far);
}

void C_tnm_fog::load_local(C_tnm_save_stream& stream)
{
	stream.load(m_fog_name);
	stream.load(m_fog_x);
	stream.load(m_fog_near);
	stream.load(m_fog_far);

	// 再構築
	restruct();
}

void C_tnm_fog::update_time(int past_game_time, int past_real_time)
{
	m_fog_x.update_time(past_game_time, past_real_time);
}

void C_tnm_fog::frame()
{
	m_fog_x.frame();
}

// フォグを作成
void C_tnm_fog::create_fog(CTSTR& fog_name)
{
	// 連続読み込みに備えてアルバムを保持しておく
	BSP<C_d3d_album> temp = m_album;

	// ローカル初期化
	init_local();

	// パラメータを設定
	m_fog_name = fog_name;

	// 再構築
	restruct();
}

// 再構築
void C_tnm_fog::restruct()
{
	// フォグテクスチャを設定してみる
	if (!m_fog_name.empty())	{
		m_album = tnm_load_pct_d3d(m_fog_name, false, false);
		if (!m_album)
			m_fog_name.clear();		// 失敗した場合は初期化
	}
}

// パラメータを設定／取得
/*
void C_tnm_fog::set_fog_x(int fog_x)
{
	m_fog_x = fog_x;
}
int C_tnm_fog::get_fog_x()
{
	return m_fog_x;
}
void C_tnm_fog::set_fog_near(int fog_near)
{
	m_fog_near = fog_near;
}
int C_tnm_fog::get_fog_near()
{
	return m_fog_near;
}
void C_tnm_fog::set_fog_far(int fog_far)
{
	m_fog_far = fog_far;
}
int C_tnm_fog::get_fog_far()
{
	return m_fog_far;
}
*/

// パラメータを取得
TSTR C_tnm_fog::get_fog_name()
{
	return m_fog_name;
}
BSP<C_d3d_album> C_tnm_fog::get_fog_album()
{
	return m_album;
}





