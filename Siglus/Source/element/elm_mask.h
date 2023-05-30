#pragma		once

#include	"data/tnm_element.h"
#include	"element/elm_list.h"
#include	"element/elm_int_event.h"

class	C_tnm_save_stream;

// ****************************************************************
// エレメント：マスク：パラメータ
// ================================================================
struct C_elm_mask_param
{
	C_elm_int_event		x;
	C_elm_int_event		y;
};

// ****************************************************************
// エレメント：マスク
// ================================================================
class C_elm_mask : public C_tnm_element
{
public:
	// エレメントの基本動作
	void	init(S_element element, CTSTR& name);					// 初期化
	void	reinit();												// 再初期化
	void	save(C_tnm_save_stream& stream);						// セーブ
	void	load(C_tnm_save_stream& stream);						// ロード

	// マスクを作成
	void	create(CTSTR& file_name);

	// フレーム処理
	void	update_time(int past_game_time, int past_real_time);	// 時間を更新
	void	frame();												// フレーム処理

	// パラメータを設定
	void				set_x(int x)			{	m_param.x.set_value(x);		}
	void				set_y(int y)			{	m_param.y.set_value(y);		}

	// パラメータを取得
	int					get_x() const			{	return m_param.x.get_value();	}
	int					get_y() const			{	return m_param.y.get_value();	}
	TSTR				get_mask_name() const	{	return m_mask_name;				}
	BSP<C_d3d_album>	get_mask_album() const	{	return m_album;					}

	// イベントを取得
	C_elm_int_event&	x_event()				{	return m_param.x;	}
	C_elm_int_event&	y_event()				{	return m_param.y;	}

	// 合計値を取得
	int					get_total_x() const		{	return m_param.x.get_total_value();	}
	int					get_total_y() const		{	return m_param.y.get_total_value();	}

private:

	// 再構築
	void	restruct();

	// パラメータ
	TSTR				m_mask_name;
	C_elm_mask_param	m_param;

	// 作業パラメータ
	BSP<C_d3d_album>	m_album;
};

// ****************************************************************
// エレメント：マスクリスト
// ================================================================
class C_elm_mask_list : public C_elm_list<C_elm_mask>
{
protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};

