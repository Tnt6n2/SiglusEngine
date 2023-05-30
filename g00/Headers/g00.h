#pragma once

#include	"g00_pch.h"

// ****************************************************************
// G00 チップクラス
// ================================================================
struct S_g00_chip_info
{
	int		x;
	int		y;
	int		width;
	int		height;
	bool	sprite;
};

class C_g00_chip
{
public:
	void	init();
	void	set_data_type0or1(int g00_type, BYTE* chip_data, int data_size, int width, int height);
	void	set_data_type2(int g00_type, BYTE* chip_data, BYTE** next_chip_data);
	void	get_info(S_g00_chip_info* info);
	bool	get_data(BYTE* buffer, int pitch);

private:
	int		m_g00_type;
	int		m_pos_x;
	int		m_pos_y;
	int		m_width;
	int		m_height;
	bool	m_sprite;
	BYTE*	m_data;
	int		m_data_size;

	// static な展開用バッファ（★マルチスレッド未対応）
	static std::vector<BYTE> m_unpack_buffer;
};

// ****************************************************************
// G00 カットクラス
// ================================================================
struct S_g00_cut_info
{
	int		width;
	int		height;
	POINT	center;
	RECT	disp_rect;
	int		chip_cnt;
};

class C_g00_cut
{
public:
	C_g00_cut() : m_g00_type(0), m_width(0), m_height(0)
	{
		m_center.x = 0;
		m_center.y = 0;
		m_disp_rect.left = 0;
		m_disp_rect.top = 0;
		m_disp_rect.right = 0;
		m_disp_rect.bottom = 0;
	}

	void	init();
	bool	is_data();
	void	set_data(int g00_type, BYTE* cut_data, int data_size);
	void	get_info(S_g00_cut_info* info);

	C_g00_chip*	get_chip(int chip_no);

private:
	int		m_g00_type;
	int		m_width;
	int		m_height;
	POINT	m_center;
	RECT	m_disp_rect;

	std::vector<C_g00_chip>	m_chip_list;
};

// ****************************************************************
// G00 クラス
// ================================================================
struct S_g00_info
{
	int		cut_cnt;
};

class C_g00
{
public:
	C_g00();

	bool	set_data(BYTE* file_data, int data_size);
	void	get_info(S_g00_info* info);

	C_g00_cut*	get_cut(int cut_no);

private:
	BYTE*	get_cut_data_point(BYTE *g00_data, int cut_no, int* ret_size);

	int							m_g00_type;
	std::vector<C_g00_cut>		m_cut_list;

	// static な展開用バッファ（★マルチスレッド未対応）
	static	std::vector<BYTE>	m_unpack_data;
};

