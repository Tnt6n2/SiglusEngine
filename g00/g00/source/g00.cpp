#include	"g00_pch.h"
#include	"g00.h"
#include	"unpack.h"
#include	"jpeg.h"

std::vector<BYTE> C_g00::m_unpack_data(2 * 1024 * 1024);		// 2MB くらいとっとく
std::vector<BYTE> C_g00_chip::m_unpack_buffer(2 * 1024 * 1024);	// 2MB くらいとっとく

// ****************************************************************
// G00 ヘッダ構造体
// ================================================================

// psd での元位置。ビューアなどで復元するときに使用。
// ゲーム上ではまず使わない。
struct G00_CUT_DATABASE
{
	int		sx;
	int		sy;
	int		ex;
	int		ey;
	int		cx;
	int		cy;
};

// カットデータのヘッダ
struct G00_CUT_HEADER_STRUCT
{
	BYTE	type;		// 0:ベタ 1:インデックスカラー 2:カット
	WORD	count;		// カット数
	int		x;			// 表示領域Ｘ
	int		y;			// 表示領域Ｙ
	int		disp_xl;	// 表示領域の幅
	int		disp_yl;	// 表示領域の高さ
	int		xc;			// 中心Ｘ
	int		yc;			// 中心Ｙ
	int		cut_xl;		// カット全体の幅
	int		cut_yl;		// カット全体の高さ
	int		keep[20];
};

// チップデータのヘッダ
struct G00_CHIP_HEADER_STRUCT
{
	WORD	x;			// チップＸ（原点：カット全体）
	WORD	y;			// チップＸ（原点：カット全体）
	BYTE	type;		// 0:ベタ 1:透過あり
	WORD	xl;			// チップの幅
	WORD	yl;			// チップの高さ
	int		keep[20];
};

// ****************************************************************
// g00
// ================================================================
C_g00::C_g00()
{
	m_g00_type = 0;
}

// ****************************************************************
// g00：データセット
// ================================================================
bool C_g00::set_data(BYTE* file_data, int data_size)
{
	// 探索ポインタのセット
	BYTE* p = file_data;
	// G00 のタイプを読み込む
	m_g00_type = *p++;

	// タイプによって分岐
	if (false);
	// タイプ０：ベタデータ
	else if (m_g00_type == 0)	{

		// カットは１枚
		m_cut_list.resize(1);
		// カットの作成
		m_cut_list[0].set_data(m_g00_type, p, data_size - 1);
	}
	// タイプ１：インデックスカラー
	else if (m_g00_type == 1)	{

		// カットは１枚
		m_cut_list.resize(1);
		// カットの作成
		m_cut_list[0].set_data(m_g00_type, p, data_size - 1);
	}
	// タイプ２：カットデータ
	else if (m_g00_type == 2)	{

		int width = *(WORD *)p;		p += 2;			// 幅
		int height = *(WORD *)p;	p += 2;			// 高さ
		int cut_cnt = *(int *)p;	p += 4;			// カット数
		p += sizeof(G00_CUT_DATABASE) * cut_cnt;	// カット復元用ヘッダ

		int unpack_size = LzssUnPack(p, NULL);		// 展開サイズ
		if ((int)m_unpack_data.size() < unpack_size + 64)	{
			m_unpack_data.resize(unpack_size + 64);		// 展開バッファ確保（+64：予備）
		}
		LzssUnPack(p, &m_unpack_data[0]);			// 展開
		p = &m_unpack_data[0];

		// カットリストを作成
		m_cut_list.resize(cut_cnt);
		for (int cut_no = 0; cut_no < cut_cnt; cut_no++)	{

			// カットデータの先頭を取得
			int size = 0;
			BYTE* p_cut_data = get_cut_data_point(p, cut_no, &size);

			// カットの初期化
			if (p_cut_data)
				m_cut_list[cut_no].set_data(m_g00_type, p_cut_data, size);
		}
	}
	// タイプ３：Jpeg データ
	else if (m_g00_type == 3)	{

		// カットは１枚
		m_cut_list.resize(1);
		// カットの作成
		m_cut_list[0].set_data(m_g00_type, p, data_size - 1);
	}

	return true;
}

// ****************************************************************
// g00：カットデータの先頭ポインタを取得
// ================================================================
BYTE* C_g00::get_cut_data_point(BYTE* g00_data, int cut_no, int* ret_size)
{
	if (g00_data == NULL)
		return false;

	DWORD offset;
	int size;

	// 探索ポインタ（UINT)
	UINT* uip = (UINT *)g00_data;

	int cut_cnt = *uip++;
	if (0 <= cut_no && cut_no < cut_cnt)	{
		offset = *(uip + cut_no * 2);
		size = *(uip + cut_no * 2 + 1);

		if (offset > 0 && size != 0)		// size がマイナスの時は同データのリンク
		{
			if (ret_size)
			{
				*ret_size = size;
			}

			return g00_data + offset;
		}
	}

	return NULL;
}

// ****************************************************************
// g00：情報の取得
// ================================================================
void C_g00::get_info(S_g00_info* info)
{
	info->cut_cnt = (int)m_cut_list.size();
}

// ****************************************************************
// g00：カットデータの取得
// ================================================================
C_g00_cut* C_g00::get_cut(int cut_no)
{
	return &m_cut_list[cut_no];
}

// ****************************************************************
// g00 カット：初期化
// ================================================================
void C_g00_cut::init()
{
	m_g00_type = 0;
	m_width = 0;
	m_height = 0;
	m_chip_list.clear();
}

// ****************************************************************
// g00 カット：データセット
// ================================================================
void C_g00_cut::set_data(int g00_type, BYTE* cut_data, int data_size)
{
	init();

	m_g00_type = g00_type;

	// 探索ポインタのセット
	BYTE* p = cut_data;

	if (false);
	else if (m_g00_type == 0 || m_g00_type == 1 || m_g00_type == 3)	{

		// 情報の設定
		m_width = *(WORD *)p;	p += 2;				// 幅
		m_height = *(WORD *)p;	p += 2;				// 高さ
		m_center.x = 0;
		m_center.y = 0;
		m_disp_rect.left = 0;
		m_disp_rect.top = 0;
		m_disp_rect.right = m_width;
		m_disp_rect.bottom = m_height;

		// チップは１枚
		m_chip_list.resize(1);
		m_chip_list[0].set_data_type0or1(m_g00_type, p, data_size - 4, m_width, m_height);
	}
	else if (m_g00_type == 2)	{

		// カットデータヘッダの取得
		G00_CUT_HEADER_STRUCT* cut_header = (G00_CUT_HEADER_STRUCT *)p;
		p += sizeof(G00_CUT_HEADER_STRUCT);

		// 情報の設定
		m_width = cut_header->cut_xl;
		m_height = cut_header->cut_yl;
		m_center.x = cut_header->xc;
		m_center.y = cut_header->yc;
		m_disp_rect.left = cut_header->x;
		m_disp_rect.top = cut_header->y;
		m_disp_rect.right = cut_header->x + cut_header->disp_xl;
		m_disp_rect.bottom = cut_header->y +  cut_header->disp_yl;

		// チップの情報を取得
		int chip_cnt = cut_header->count;
		m_chip_list.resize(chip_cnt);
		for (int chip_no = 0; chip_no < chip_cnt; chip_no++)
			m_chip_list[chip_no].set_data_type2(m_g00_type, p, &p);
	}
}

// ****************************************************************
// g00 カット：データを判定
// ================================================================
bool C_g00_cut::is_data()
{
	return m_width > 0 && m_height > 0;
}

// ****************************************************************
// g00 カット：情報の取得
// ================================================================
void C_g00_cut::get_info(S_g00_cut_info* info)
{
	info->width = m_width;
	info->height = m_height;
	info->center = m_center;
	info->disp_rect = m_disp_rect;
	info->chip_cnt = (int)m_chip_list.size();
}

// ****************************************************************
// g00 カット：チップの取得
// ================================================================
C_g00_chip* C_g00_cut::get_chip(int chip_no)
{
	return &m_chip_list[chip_no];
}

// ****************************************************************
// g00 チップ：初期化
// ================================================================
void C_g00_chip::init()
{
	m_g00_type = 0;
	m_pos_x = 0;
	m_pos_y = 0;
	m_width = 0;
	m_height = 0;
	m_data = NULL;
	m_data_size = 0;
	m_sprite = true;
}

// ****************************************************************
// g00 チップ：データセット（タイプ０、タイプ１）
// ================================================================
void C_g00_chip::set_data_type0or1(int g00_type_, BYTE* chip_data_, int data_size_, int width_, int height_)
{
	init();

	m_g00_type = g00_type_;
	m_pos_x = 0;
	m_pos_y = 0;
	m_width = width_;
	m_height = height_;
	m_data = chip_data_;
	m_data_size = data_size_;
	m_sprite = false;			// 透過なし
}

// ****************************************************************
// g00 チップ：データセット（タイプ２）
// ================================================================
void C_g00_chip::set_data_type2(int g00_type_, BYTE* chip_data, BYTE** next_chip_data)
{
	init();

	m_g00_type = g00_type_;

	// 探索ポインタのセット
	BYTE* p = chip_data;

	// ヘッダの解析
	G00_CHIP_HEADER_STRUCT* chip_header = (G00_CHIP_HEADER_STRUCT *)p;
	m_pos_x = chip_header->x;
	m_pos_y = chip_header->y;
	m_width = chip_header->xl;
	m_height = chip_header->yl;
	m_sprite = chip_header->type == 1 ? true : false;
	p += sizeof(G00_CHIP_HEADER_STRUCT);

	m_data = p;
	p += m_width * m_height * 4;

	// 次のチップデータの位置を返す
	*next_chip_data = p;
}

// ****************************************************************
// g00 チップ：情報の取得
// ================================================================
void C_g00_chip::get_info(S_g00_chip_info* info)
{
	info->x = m_pos_x;
	info->y = m_pos_y;
	info->width = m_width;
	info->height = m_height;
	info->sprite = m_sprite;
}

// ****************************************************************
// g00 チップ：データの取得
// ================================================================

#define			GET_DATA_OLD_PROCESS		0

std::vector<BYTE>* G__buf;

bool C_g00_chip::get_data(BYTE* dst, int dst_pitch)
{
	if (m_data == NULL)
		return false;

	if (false);
	else if (m_g00_type == 0)	{

		// ＬＺＳＳ展開
		int unpack_size = LzssUnPack32(m_data, NULL);
		if ((int)m_unpack_buffer.size() < unpack_size)	{
			m_unpack_buffer.resize(unpack_size);
		}
		LzssUnPack32(m_data, &m_unpack_buffer[0]);
		
		// チップを読み込む

#if GET_DATA_OLD_PROCESS
		for (int y = 0; y < height; y++)
			memcpy(dst + dst_pitch * y, &unpack_buffer[0] + width * 4 * y, width * 4);
#else
		BYTE* src = &m_unpack_buffer[0];
		int draw_cnt_x = m_width;
		int draw_cnt_y = m_height;
		int add_dst_addr = dst_pitch - (m_width * 4);

		// 描画本体
		_asm{
			cld
			mov		edi, dst
			mov		esi, src
			mov		ecx, draw_cnt_y
			mov		eax, draw_cnt_x
			mov		edx, add_dst_addr
		g00_type_0_loop_y:
			mov		ebx, ecx		// push ecx
			mov		ecx, eax
			rep		movsd
			add		edi, edx
			mov		ecx, ebx		// pop ecx
			loop	g00_type_0_loop_y
		}
#endif
	}
	else if (m_g00_type == 1)	{

		// ＬＺＳＳ展開
		int unpack_size = LzssUnPack(m_data, NULL);
		if ((int)m_unpack_buffer.size() < unpack_size)	{
			m_unpack_buffer.resize(unpack_size);
		}
		LzssUnPack(m_data, &m_unpack_buffer[0]);
		BYTE* p = &m_unpack_buffer[0];

		// パレットの読み込み
		int pal_cnt = *(WORD *)p;	p += 2;
		DWORD* pal = (DWORD *)p;	p += pal_cnt * 4;

		// チップを読み込む
		for (int y = 0; y < m_height; y++)	{
			DWORD* dst_4 = (DWORD *)(dst + dst_pitch * y);
			for (int x = 0; x < m_width; x++)	{
				*dst_4++ = *(pal + *p++);
			}
		}
	}
	else if (m_g00_type == 2)	{

		// チップを読み込む

#if GET_DATA_OLD_PROCESS
		for (int y = 0; y < height; y++)
			memcpy(dst + dst_pitch * y, data + width * 4 * y, width * 4);
#else
		BYTE* src = m_data;
		int draw_cnt_x = m_width;
		int draw_cnt_y = m_height;
		int add_dst_addr = dst_pitch - (m_width * 4);

		// 描画本体
		_asm{
			cld
			mov		edi, dst
			mov		esi, src
			mov		ecx, draw_cnt_y
			mov		eax, draw_cnt_x
			mov		edx, add_dst_addr
		g00_type_2_loop_y:
			mov		ebx, ecx		// push ecx
			mov		ecx, eax
			rep		movsd
			add		edi, edx
			mov		ecx, ebx		// pop ecx
			loop	g00_type_2_loop_y
		}
#endif
	}
	else if (m_g00_type == 3)	{

		Cva_jpeg jpeg_expander;
		std::vector<BYTE> buffer(m_width * m_height * 4);
		int buffer_w = 0, buffer_h = 0;

		G__buf = &buffer;
		
		if (!jpeg_expander.f_expand_ready(m_data, m_data_size, &buffer, &buffer_w, &buffer_h, 0))	{
			return false;
		}
		jpeg_expander.f_expand_proc();

		for (int y = 0; y < buffer_h; y++) {
			memcpy(dst + dst_pitch * y, &buffer[buffer_w * 4 * y], buffer_w * 4);
		}

		return true;
	}

	return true;
}

