#include	"pch.h"
#include	"nwa_pack.h"

// データ位置修正マクロ
#define		SRCADR					if (bit_ind >= 8)	{	sp++;	bit_ind &= 0x07;	}

// モードコードの取り出しマクロ
#define		GET_MOD_CODE1			SRCADR		mod_code = ((*(WORD*)sp) >> bit_ind) & 0x01;	bit_ind += 1;	bit_cnt += 1;
#define		GET_MOD_CODE2			SRCADR		mod_code = ((*(WORD*)sp) >> bit_ind) & 0x03;	bit_ind += 2;	bit_cnt += 2;
#define		GET_MOD_CODE3			SRCADR		mod_code = ((*(WORD*)sp) >> bit_ind) & 0x07;	bit_ind += 3;	bit_cnt += 3;
#define		GET_MOD_CODE4			SRCADR		mod_code = ((*(WORD*)sp) >> bit_ind) & 0x0f;	bit_ind += 4;	bit_cnt += 4;

// データコードの取り出しマクロ
#define		GET_DAT_CODE3			SRCADR		dat_code = ((*(WORD*)sp) >> bit_ind) & 0x007;	bit_ind += 3;	bit_cnt += 3;
#define		GET_DAT_CODE4			SRCADR		dat_code = ((*(WORD*)sp) >> bit_ind) & 0x00f;	bit_ind += 4;	bit_cnt += 4;
#define		GET_DAT_CODE5			SRCADR		dat_code = ((*(WORD*)sp) >> bit_ind) & 0x01f;	bit_ind += 5;	bit_cnt += 5;
#define		GET_DAT_CODE6			SRCADR		dat_code = ((*(WORD*)sp) >> bit_ind) & 0x03f;	bit_ind += 6;	bit_cnt += 6;
#define		GET_DAT_CODE7			SRCADR		dat_code = ((*(WORD*)sp) >> bit_ind) & 0x07f;	bit_ind += 7;	bit_cnt += 7;
#define		GET_DAT_CODE8			SRCADR		dat_code = ((*(WORD*)sp) >> bit_ind) & 0x0ff;	bit_ind += 8;	bit_cnt += 8;

// ゼロカウント取得マクロ
#define		GET_ZERO_COUNT \
			SRCADR		zero_cnt = ((*(WORD*)sp) >> bit_ind) & 0x01;	bit_ind += 1;	bit_cnt += 1; \
			if (zero_cnt == 1)	{ \
				SRCADR		zero_cnt = ((*(WORD*)sp) >> bit_ind) & 0x03;	bit_ind += 2;	bit_cnt += 2; \
				if (zero_cnt == 3)	{ \
					SRCADR		zero_cnt = ((*(WORD*)sp) >> bit_ind) & 0xff;	bit_ind += 8;	bit_cnt += 8; \
				} \
			}

// データコードの解凍マクロ
#define		UNPACK3_DATA1			GET_DAT_CODE3		if (dat_code & 0x04)	{	dat_code &= 0x03;	nowsmp -= (dat_code <<  5);	}		else	{	nowsmp += (dat_code << 5 );	}
#define		UNPACK3_DATA2			GET_DAT_CODE3		if (dat_code & 0x04)	{	dat_code &= 0x03;	nowsmp -= (dat_code <<  6);	}		else	{	nowsmp += (dat_code << 6 );	}
#define		UNPACK3_DATA3			GET_DAT_CODE3		if (dat_code & 0x04)	{	dat_code &= 0x03;	nowsmp -= (dat_code <<  7);	}		else	{	nowsmp += (dat_code << 7 );	}
#define		UNPACK3_DATA4			GET_DAT_CODE3		if (dat_code & 0x04)	{	dat_code &= 0x03;	nowsmp -= (dat_code <<  8);	}		else	{	nowsmp += (dat_code << 8 );	}
#define		UNPACK3_DATA5			GET_DAT_CODE3		if (dat_code & 0x04)	{	dat_code &= 0x03;	nowsmp -= (dat_code <<  9);	}		else	{	nowsmp += (dat_code << 9 );	}
#define		UNPACK3_DATA6			GET_DAT_CODE3		if (dat_code & 0x04)	{	dat_code &= 0x03;	nowsmp -= (dat_code << 10);	}		else	{	nowsmp += (dat_code <<10 );	}
#define		UNPACK3_DATA7			GET_DAT_CODE6		if (dat_code & 0x20)	{	dat_code &= 0x1f;	nowsmp -= (dat_code << 11);	}		else	{	nowsmp += (dat_code <<11 );	}

#define		UNPACK4_DATA1			GET_DAT_CODE4		if (dat_code & 0x08)	{	dat_code &= 0x07;	nowsmp -= (dat_code <<  4);	}		else	{	nowsmp += (dat_code << 4 );	}
#define		UNPACK4_DATA2			GET_DAT_CODE4		if (dat_code & 0x08)	{	dat_code &= 0x07;	nowsmp -= (dat_code <<  5);	}		else	{	nowsmp += (dat_code << 5 );	}
#define		UNPACK4_DATA3			GET_DAT_CODE4		if (dat_code & 0x08)	{	dat_code &= 0x07;	nowsmp -= (dat_code <<  6);	}		else	{	nowsmp += (dat_code << 6 );	}
#define		UNPACK4_DATA4			GET_DAT_CODE4		if (dat_code & 0x08)	{	dat_code &= 0x07;	nowsmp -= (dat_code <<  7);	}		else	{	nowsmp += (dat_code << 7 );	}
#define		UNPACK4_DATA5			GET_DAT_CODE4		if (dat_code & 0x08)	{	dat_code &= 0x07;	nowsmp -= (dat_code <<  8);	}		else	{	nowsmp += (dat_code << 8 );	}
#define		UNPACK4_DATA6			GET_DAT_CODE4		if (dat_code & 0x08)	{	dat_code &= 0x07;	nowsmp -= (dat_code <<  9);	}		else	{	nowsmp += (dat_code << 9 );	}
#define		UNPACK4_DATA7			GET_DAT_CODE7		if (dat_code & 0x40)	{	dat_code &= 0x3f;	nowsmp -= (dat_code << 10);	}		else	{	nowsmp += (dat_code <<10 );	}

#define		UNPACK5_DATA1			GET_DAT_CODE5		if (dat_code & 0x10)	{	dat_code &= 0x0f;	nowsmp -= (dat_code <<  3);	}		else	{	nowsmp += (dat_code << 3 );	}
#define		UNPACK5_DATA2			GET_DAT_CODE5		if (dat_code & 0x10)	{	dat_code &= 0x0f;	nowsmp -= (dat_code <<  4);	}		else	{	nowsmp += (dat_code << 4 );	}
#define		UNPACK5_DATA3			GET_DAT_CODE5		if (dat_code & 0x10)	{	dat_code &= 0x0f;	nowsmp -= (dat_code <<  5);	}		else	{	nowsmp += (dat_code << 5 );	}
#define		UNPACK5_DATA4			GET_DAT_CODE5		if (dat_code & 0x10)	{	dat_code &= 0x0f;	nowsmp -= (dat_code <<  6);	}		else	{	nowsmp += (dat_code << 6 );	}
#define		UNPACK5_DATA5			GET_DAT_CODE5		if (dat_code & 0x10)	{	dat_code &= 0x0f;	nowsmp -= (dat_code <<  7);	}		else	{	nowsmp += (dat_code << 7 );	}
#define		UNPACK5_DATA6			GET_DAT_CODE5		if (dat_code & 0x10)	{	dat_code &= 0x0f;	nowsmp -= (dat_code <<  8);	}		else	{	nowsmp += (dat_code << 8 );	}
#define		UNPACK5_DATA7			GET_DAT_CODE8		if (dat_code & 0x80)	{	dat_code &= 0x7f;	nowsmp -= (dat_code <<  9);	}		else	{	nowsmp += (dat_code << 9 );	}

#define		UNPACK6_DATA1			GET_DAT_CODE6		if (dat_code & 0x20)	{	dat_code &= 0x1f;	nowsmp -= (dat_code <<  2);	}		else	{	nowsmp += (dat_code << 2 );	}
#define		UNPACK6_DATA2			GET_DAT_CODE6		if (dat_code & 0x20)	{	dat_code &= 0x1f;	nowsmp -= (dat_code <<  3);	}		else	{	nowsmp += (dat_code << 3 );	}
#define		UNPACK6_DATA3			GET_DAT_CODE6		if (dat_code & 0x20)	{	dat_code &= 0x1f;	nowsmp -= (dat_code <<  4);	}		else	{	nowsmp += (dat_code << 4 );	}
#define		UNPACK6_DATA4			GET_DAT_CODE6		if (dat_code & 0x20)	{	dat_code &= 0x1f;	nowsmp -= (dat_code <<  5);	}		else	{	nowsmp += (dat_code << 5 );	}
#define		UNPACK6_DATA5			GET_DAT_CODE6		if (dat_code & 0x20)	{	dat_code &= 0x1f;	nowsmp -= (dat_code <<  6);	}		else	{	nowsmp += (dat_code << 6 );	}
#define		UNPACK6_DATA6			GET_DAT_CODE6		if (dat_code & 0x20)	{	dat_code &= 0x1f;	nowsmp -= (dat_code <<  7);	}		else	{	nowsmp += (dat_code << 7 );	}
#define		UNPACK6_DATA7			GET_DAT_CODE8		if (dat_code & 0x80)	{	dat_code &= 0x7f;	nowsmp -= (dat_code <<  9);	}		else	{	nowsmp += (dat_code << 9 );	}

#define		UNPACK7_DATA1			GET_DAT_CODE7		if (dat_code & 0x40)	{	dat_code &= 0x3f;	nowsmp -= (dat_code <<  2);	}		else	{	nowsmp += (dat_code << 2 );	}
#define		UNPACK7_DATA2			GET_DAT_CODE7		if (dat_code & 0x40)	{	dat_code &= 0x3f;	nowsmp -= (dat_code <<  3);	}		else	{	nowsmp += (dat_code << 3 );	}
#define		UNPACK7_DATA3			GET_DAT_CODE7		if (dat_code & 0x40)	{	dat_code &= 0x3f;	nowsmp -= (dat_code <<  4);	}		else	{	nowsmp += (dat_code << 4 );	}
#define		UNPACK7_DATA4			GET_DAT_CODE7		if (dat_code & 0x40)	{	dat_code &= 0x3f;	nowsmp -= (dat_code <<  5);	}		else	{	nowsmp += (dat_code << 5 );	}
#define		UNPACK7_DATA5			GET_DAT_CODE7		if (dat_code & 0x40)	{	dat_code &= 0x3f;	nowsmp -= (dat_code <<  6);	}		else	{	nowsmp += (dat_code << 6 );	}
#define		UNPACK7_DATA6			GET_DAT_CODE7		if (dat_code & 0x40)	{	dat_code &= 0x3f;	nowsmp -= (dat_code <<  7);	}		else	{	nowsmp += (dat_code << 7 );	}
#define		UNPACK7_DATA7			GET_DAT_CODE8		if (dat_code & 0x80)	{	dat_code &= 0x7f;	nowsmp -= (dat_code <<  9);	}		else	{	nowsmp += (dat_code << 9 );	}

#define		UNPACK8_DATA1			GET_DAT_CODE8		if (dat_code & 0x80)	{	dat_code &= 0x7f;	nowsmp -= (dat_code <<  2);	}		else	{	nowsmp += (dat_code << 2 );	}
#define		UNPACK8_DATA2			GET_DAT_CODE8		if (dat_code & 0x80)	{	dat_code &= 0x7f;	nowsmp -= (dat_code <<  3);	}		else	{	nowsmp += (dat_code << 3 );	}
#define		UNPACK8_DATA3			GET_DAT_CODE8		if (dat_code & 0x80)	{	dat_code &= 0x7f;	nowsmp -= (dat_code <<  4);	}		else	{	nowsmp += (dat_code << 4 );	}
#define		UNPACK8_DATA4			GET_DAT_CODE8		if (dat_code & 0x80)	{	dat_code &= 0x7f;	nowsmp -= (dat_code <<  5);	}		else	{	nowsmp += (dat_code << 5 );	}
#define		UNPACK8_DATA5			GET_DAT_CODE8		if (dat_code & 0x80)	{	dat_code &= 0x7f;	nowsmp -= (dat_code <<  6);	}		else	{	nowsmp += (dat_code << 6 );	}
#define		UNPACK8_DATA6			GET_DAT_CODE8		if (dat_code & 0x80)	{	dat_code &= 0x7f;	nowsmp -= (dat_code <<  7);	}		else	{	nowsmp += (dat_code << 7 );	}
#define		UNPACK8_DATA7			GET_DAT_CODE8		if (dat_code & 0x80)	{	dat_code &= 0x7f;	nowsmp -= (dat_code <<  9);	}		else	{	nowsmp += (dat_code << 9 );	}

// 解凍マクロ
#define		UNPACK(MOD) \
			if (zero_cnt)	{	zero_cnt--;	} \
			else			{ \
				GET_MOD_CODE3 \
				if (mod_code < 4)	{ \
					if (mod_code == 0)		{ \
						if (header->zero_mod) {	GET_ZERO_COUNT	} \
					} \
					else if (mod_code == 1)	{	UNPACK##MOD##_DATA1	} \
					else if (mod_code == 2)	{	UNPACK##MOD##_DATA2	} \
					else					{   UNPACK##MOD##_DATA3	} \
				} \
				else			{ \
					if (mod_code == 4)		{	UNPACK##MOD##_DATA4	} \
					else if (mod_code == 5)	{	UNPACK##MOD##_DATA5	} \
					else if (mod_code == 6)	{	UNPACK##MOD##_DATA6	} \
					else					{ \
						GET_MOD_CODE1 \
						if (mod_code == 0)	{	UNPACK##MOD##_DATA7	} \
						else				{	nowsmp = 0;			} \
					} \
				} \
			} \
			if (i >= unpack_smp_top)	{ \
				if (unpack_smp_counter < unpack_smp_cnt)	{ \
					*dp++ = (short)nowsmp; \
					unpack_smp_counter++; \
				} \
				else	{	break;	} \
			}

// 解凍マクロ １６ビット・モノラル
#define		UNPACK16M(MOD) \
			for (i = 0; i < src_smp_cnt; i++){ \
				UNPACK(MOD) \
			}

// 解凍マクロ １６ビット・ステレオ
#define		UNPACK16S(MOD) \
			for (i = 0; i < src_smp_cnt; i++){ \
				nowsmp = *nowsmp_ptr[i&0x01]; \
				UNPACK(MOD) \
				*nowsmp_ptr[i&0x01] = nowsmp; \
			}


int nwa_unpack_unit_16m(BYTE* src_p, DWORD src_smp_cnt, DWORD unpack_smp_top, DWORD unpack_smp_cnt, BYTE *dst_p, NWA_HEADER_STRUCT* header);
int nwa_unpack_unit_16s(BYTE* src_p, DWORD src_smp_cnt, DWORD unpack_smp_top, DWORD unpack_smp_cnt, BYTE *dst_p, NWA_HEADER_STRUCT* header);

// ****************************************************************
// ＮＷＡユニット解凍 
// ================================================================
int nwa_unpack_unit(BYTE* src_p, DWORD src_smp_cnt, DWORD unpack_smp_top, DWORD unpack_smp_cnt, BYTE *dst_p, NWA_HEADER_STRUCT* header)
{
	if (header->bits_per_sample == 16)	{
		if (header->channels == 1)	{	return nwa_unpack_unit_16m(src_p, src_smp_cnt, unpack_smp_top, unpack_smp_cnt, dst_p, header);	}
		else						{	return nwa_unpack_unit_16s(src_p, src_smp_cnt, unpack_smp_top, unpack_smp_cnt, dst_p, header);	}
	}
	return 0;
}

// ****************************************************************
// ＮＷＡユニット解凍 １６ビット・モノラル
// ================================================================
int nwa_unpack_unit_16m(BYTE* src_p, DWORD src_smp_cnt, DWORD unpack_smp_top, DWORD unpack_smp_cnt, BYTE *dst_p, NWA_HEADER_STRUCT* header)
{
	BYTE*	sp = src_p;
	short*	dp = (short*)dst_p;
	DWORD	i, mod_code, dat_code;

	DWORD	bit_ind = 0;
	DWORD	bit_cnt = 0;

	DWORD	unpack_smp_counter = 0;
	DWORD	 zero_cnt = 0;

	// 圧縮モードの補正
	DWORD	pack_mod = header->pack_mod;
	switch (pack_mod)	{
		case 0:		pack_mod = 2;	break;
		case 1:		pack_mod = 1;	break;
		case 2:		pack_mod = 0;	break;
	}

	// 初期サンプル値
	int		nowsmp = *(short*)sp;	sp += 2;

	switch (pack_mod)	{
		case 0:		UNPACK16M(3);	break;
		case 1:		UNPACK16M(4);	break;
		case 2:		UNPACK16M(5);	break;
		case 3:		UNPACK16M(6);	break;
		case 4:		UNPACK16M(7);	break;
		case 5:		UNPACK16M(8);	break;
	}

	return unpack_smp_counter;
}

// ****************************************************************
// ＮＷＡユニット解凍 １６ビット・ステレオ
// ================================================================
int nwa_unpack_unit_16s(BYTE* src_p, DWORD src_smp_cnt, DWORD unpack_smp_top, DWORD unpack_smp_cnt, BYTE *dst_p, NWA_HEADER_STRUCT* header)
{
	BYTE*	sp = src_p;
	short*	dp = (short*)dst_p;
	DWORD	i, mod_code, dat_code;

	DWORD	bit_ind = 0;
	DWORD	bit_cnt = 0;

	DWORD	unpack_smp_counter = 0;
	DWORD	 zero_cnt = 0;

	// 圧縮モードの補正
	DWORD	pack_mod = header->pack_mod;
	switch (pack_mod)	{
		case 0:		pack_mod = 2;	break;
		case 1:		pack_mod = 1;	break;
		case 2:		pack_mod = 0;	break;
	}

	// 初期サンプル値
	int		nowsmp_l = *(short*)sp;	sp += 2;
	int		nowsmp_r = *(short*)sp;	sp += 2;
	int*	nowsmp_ptr[2] = {&nowsmp_l, &nowsmp_r};
	int		nowsmp;

	switch (pack_mod)	{
		case 0:		UNPACK16S(3);	break;
		case 1:		UNPACK16S(4);	break;
		case 2:		UNPACK16S(5);	break;
		case 3:		UNPACK16S(6);	break;
		case 4:		UNPACK16S(7);	break;
		case 5:		UNPACK16S(8);	break;
	}

	return unpack_smp_counter;
}