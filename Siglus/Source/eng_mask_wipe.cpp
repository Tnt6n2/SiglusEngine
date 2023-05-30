#include	"pch.h"
#include	"engine/eng_mask_wipe.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// エリア設定
// IN	int w         : 幅
// 		int h         : 高さ
// 		int *x1 - *y2 : エリア
// OUT	なし
// ================================================================
void tnm_set_area_func(int w, int h, int *x1, int *y1, int *x2, int *y2)
{
	if (*x1 == -1 && *y1 == -1 && *x2 == -1 && *y2 == -1)	{
		*x1 = 0;
		*y1 = 0;
		*x2 = w - 1;
		*y2 = h - 1;
	}
	else	{
		int tmp;
		if (*x1 > *x2)	{
			tmp = *x1;
			*x1 = *x2;
			*x2 = tmp;
		}
		if (*y1 > *y2)	{
			tmp = *y1;
			*y1 = *y2;
			*y2 = tmp;
		}
		if (*x1 < 0)	{	*x1 = 0;		}
		if (*y1 < 0)	{	*y1 = 0;		}
		if (*x2 >= w)	{	*x2 = w - 1;	}
		if (*y2 >= h)	{	*y2 = h - 1;	}
	}
}

// ****************************************************************
// クリッピングＦＵＮＣ
// ※第１引数　<=　第３引数
// ※第２引数、第４引数は大小関係無し
// ※負数は取り扱えない（このルーチンでは事前に下駄を履かせてある）
// ※因数の名前はあまり意味がない（ x1 が１点目のＸ座標とは限らない）
// IN	DWORD x1 - y2 : エリア
// 		DWORD clip : クリップ座標
// OUT	DWORD もう一方の座標
// ================================================================
DWORD tnm_clip_line_func(DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD clip)
{

	_asm{
		mov		eax, x1
		mov		ebx, x2
		mov		ecx, y1
		mov		edx, y2

		mov		esi, eax
		add		esi, ebx
		rcr		esi, 1			// キャリーを含むシフト
		mov		edi, ecx
		add		edi, edx
		rcr		edi, 1			// キャリーを含むシフト

	cliping_loop:
		cmp		esi, clip
		je		cliping_end
		ja		cliping_step1

	// 中点がクリップ座標より小さい
		mov		eax, esi		// 小さい座標の方を代える
		add		esi, ebx
		rcr		esi, 1			// キャリーを含むシフト
		cmp		esi, eax
		je		cliping_end
		mov		ecx, edi
		add		edi, edx
		rcr		edi, 1			// キャリーを含むシフト
		jmp		cliping_loop

	// 中点がクリップ座標より大きい
	cliping_step1:
		mov		ebx, esi		// 小さい座標の方を代える
		add		esi, eax
		rcr		esi, 1			// キャリーを含むシフト
		cmp		esi, ebx
		je		cliping_end
		mov		edx, edi
		add		edi, ecx
		rcr		edi, 1			// キャリーを含むシフト
		jmp		cliping_loop

	cliping_end:
		mov		x1, edi
	}

	return x1;
}

// ****************************************************************
// クリッピング
// IN	int *x1 - *y2 : 座標へのポインター
//		int ex1 - ey2 : クリップ範囲
// OUT	false = 不可視
//		true  = クリップされました
// ================================================================

const int TECL_CLIP_U = 0;
const int TECL_CLIP_D = 1;
const int TECL_CLIP_L = 2;
const int TECL_CLIP_R = 3;

bool tnm_clip_line(int *x1, int *y1, int *x2, int *y2, int ex1, int ey1, int ex2, int ey2)
{
	// エリアに下駄を履かせる
	DWORD gex1 = ex1 + 0x80000000;
	DWORD gey1 = ey1 + 0x80000000;
	DWORD gex2 = ex2 + 0x80000000;
	DWORD gey2 = ey2 + 0x80000000;

	// クリッピングコードのクリア
	BYTE clip_coad1[4];
	BYTE clip_coad2[4];
	fill_memory(clip_coad1, 4, 0);
	fill_memory(clip_coad2, 4, 0);

	// クリッピングコード作成
	if (*x1 < ex1)		{	clip_coad1[TECL_CLIP_L] = 1;}
	else if (*x1 > ex2)	{	clip_coad1[TECL_CLIP_R] = 1;}
	if (*y1 < ey1)		{	clip_coad1[TECL_CLIP_U] = 1;}
	else if (*y1 > ey2)	{	clip_coad1[TECL_CLIP_D] = 1;}
	if (*x2 < ex1)		{	clip_coad2[TECL_CLIP_L] = 1;}
	else if (*x2 > ex2)	{	clip_coad2[TECL_CLIP_R] = 1;}
	if (*y2 < ey1)		{	clip_coad2[TECL_CLIP_U] = 1;}
	else if (*y2 > ey2)	{	clip_coad2[TECL_CLIP_D] = 1;}

	// 完全不可視判定（同一側に点が存在する）
	for (int i = 0; i < 4; i++)	{
		if (clip_coad1[i] && clip_coad2[i])	{	return false;	}
	}

	// 座標に下駄を履かせる
	DWORD gx1 = *x1 + 0x80000000;
	DWORD gy1 = *y1 + 0x80000000;
	DWORD gx2 = *x2 + 0x80000000;
	DWORD gy2 = *y2 + 0x80000000;

	// 作業用座標
	DWORD tx1 = gx1;
	DWORD ty1 = gy1;
	DWORD tx2 = gx2;
	DWORD ty2 = gy2;

	DWORD tmp;

	// １点目可視判定
	if (clip_coad1[TECL_CLIP_L] || clip_coad1[TECL_CLIP_R] || clip_coad1[TECL_CLIP_U] || clip_coad1[TECL_CLIP_D])	{
		while (1)	{
			if (clip_coad1[TECL_CLIP_L])	{
				tmp = tnm_clip_line_func(gx1, gy1, gx2, gy2, gex1);
				if (tmp >= gey1 && tmp <= gey2)	{
					tx1 = gex1;
					ty1 = tmp;
					break;
				}
			}
			else if (clip_coad1[TECL_CLIP_R])	{
				tmp = tnm_clip_line_func(gx2, gy2, gx1, gy1, gex2);
				if (tmp >= gey1 && tmp <= gey2)	{
					tx1 = gex2;
					ty1 = tmp;
					break;
				}
			}
			if (clip_coad1[TECL_CLIP_U])	{
				tmp = tnm_clip_line_func(gy1, gx1, gy2, gx2, gey1);
				if (tmp >= gex1 && tmp <= gex2)	{
					tx1 = tmp;
					ty1 = gey1;
					break;
				}
			}
			else if (clip_coad1[TECL_CLIP_D])	{
				tmp = tnm_clip_line_func(gy2, gx2, gy1, gx1, gey2);
				if (tmp >= gex1 && tmp <= gex2)	{
					tx1 = tmp;
					ty1 = gey2;
					break;
				}
			}
			return false;
		}
	}

	// ２点目可視判定
	if (clip_coad2[TECL_CLIP_L] || clip_coad2[TECL_CLIP_R] || clip_coad2[TECL_CLIP_U] || clip_coad2[TECL_CLIP_D])	{
		while (1)	{
			if (clip_coad2[TECL_CLIP_L])	{
				tmp = tnm_clip_line_func(gx2, gy2, gx1, gy1, gex1);
				if (tmp >= gey1 && tmp <= gey2)	{
					tx2 = gex1;
					ty2 = tmp;
					break;
				}
			}
			else if (clip_coad2[TECL_CLIP_R])	{
				tmp = tnm_clip_line_func(gx1, gy1, gx2, gy2, gex2);
				if (tmp >= gey1 && tmp <= gey2)	{
					tx2 = gex2;
					ty2 = tmp;
					break;
				}
			}
			if (clip_coad2[TECL_CLIP_U])	{
				tmp = tnm_clip_line_func(gy2, gx2, gy1, gx1, gey1);
				if (tmp >= gex1 && tmp <= gex2)	{
					tx2 = tmp;
					ty2 = gey1;
					break;
				}
			}
			else if (clip_coad2[TECL_CLIP_D])	{
				tmp = tnm_clip_line_func(gy1, gx1, gy2, gx2, gey2);
				if (tmp >= gex1 && tmp <= gex2)	{
					tx2 = tmp;
					ty2 = gey2;
					break;
				}
			}
			return false;
		}
	}

	// 座標の下駄を脱がせる
	*x1 = tx1 - 0x80000000;
	*y1 = ty1 - 0x80000000;
	*x2 = tx2 - 0x80000000;
	*y2 = ty2 - 0x80000000;

	return true;
}

// ****************************************************************
// Ｙ軸基準のラインバッファ作成
// ================================================================
int tnm_create_y_line_buffer(int *buf, int buf_max, int x1, int y1, int x2, int y2, int mod)
{
	// 前処理
	int line_w = x2 - x1;
	int line_h = y2 - y1;
	int line_add_w = 1;
	int line_add_h = 1;
	if (line_w < 0)	{	line_w = -line_w;	line_add_w = -line_add_w;	}
	if (line_h < 0)	{	line_h = -line_h;	line_add_h = -line_add_h;	}

	if (line_h >= buf_max)	{	return 0;	}

	int x = x1;
	int y = y1;

	int *original_buf = buf;

	if (line_h == 0)	{
		// 点
		if (line_w == 0)	{
			*(buf++) = x;
			*(buf++) = y;
		}
		// 水平線
		else	{
			if (mod == 0)	{
				if (line_add_w < 0)	{
					*(buf++) = x - line_w;
					*(buf++) = y;
				}
				else	{
					*(buf++) = x;
					*(buf++) = y;
				}
			}
			else	{
				if (line_add_w < 0)	{
					*(buf++) = x;
					*(buf++) = y;
				}
				else	{
					*(buf++) = x + line_w;
					*(buf++) = y;
				}
			}
		}
	}
	// 垂直線
	else if (line_w == 0)	{
		if (line_add_h < 0)	{
			_asm	{
				mov		eax, x
				mov		ebx, y
				mov		edi, buf
				mov		ecx, line_h
				inc		ecx
			v1_loop:
				mov		[edi], eax
				add		edi, 4
				mov		[edi], ebx
				add		edi, 4
				dec		ebx
				loop	v1_loop
				mov		buf, edi
			}
		}
		else	{
			_asm	{
				mov		eax, x
				mov		ebx, y
				mov		edi, buf
				mov		ecx, line_h
				inc		ecx
			v2_loop:
				mov		[edi], eax
				add		edi, 4
				mov		[edi], ebx
				add		edi, 4
				inc		ebx
				loop	v2_loop
				mov		buf, edi
			}
		}
	}
	// Ｘ基準線
	else if (line_w > line_h)	{
		int ENs = -line_w;
		int ENa = line_h<<1;
		int ENh = line_w<<1;
		if ((mod == 0 && line_add_w < 0) || (mod == 1 && line_add_w > 0))	{
			if (line_add_w < 0)	{
				if (line_add_h < 0)	{
					_asm	{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, line_w
					x1_loop:
						add		eax, ebx
						cmp		eax, 0
						jl		x1_step
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
						dec		edx
						sub		eax, ENh
					x1_step:
						dec		esi			// ＊
						loop	x1_loop
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4	
						mov		buf, edi
					}
				}
				else	{
					_asm	{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, line_w
					x2_loop:
						add		eax, ebx
						cmp		eax, 0
						jl		x2_step
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
						inc		edx
						sub		eax, ENh
					x2_step:
						dec		esi			// ＊
						loop	x2_loop
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4	
						mov		buf, edi
					}
				}
			}
			else	{
				if (line_add_h < 0)	{
					_asm	{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, line_w
					x3_loop:
						add		eax, ebx
						cmp		eax, 0
						jl		x3_step
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
						dec		edx
						sub		eax, ENh
					x3_step:
						inc		esi			//＊
						loop	x3_loop
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4	
						mov		buf, edi
					}
				}
				else	{
					_asm	{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, line_w
					x4_loop:
						add		eax, ebx
						cmp		eax, 0
						jl		x4_step
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
						inc		edx
						sub		eax, ENh
					x4_step:
						inc		esi			//＊
						loop	x4_loop
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4	
						mov		buf, edi
					}
				}
			}
		}
		else	{
			if (line_add_w < 0)	{
				if (line_add_h < 0)	{
					_asm	{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, line_w
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x5_loop:
						dec		esi
						add		eax, ebx
						cmp		eax, 0
						jl		x5_step
						dec		edx
						sub		eax, ENh
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x5_step:
						loop	x5_loop
						mov		buf, edi
					}
				}
				else	{
					_asm	{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, line_w
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x6_loop:
						dec		esi
						add		eax, ebx
						cmp		eax, 0
						jl		x6_step
						inc		edx
						sub		eax, ENh
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x6_step:
						loop	x6_loop
						mov		buf, edi
					}
				}
			}
			else	{
				if (line_add_h < 0)	{
					_asm	{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, line_w
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x7_loop:
						inc		esi
						add		eax, ebx
						cmp		eax, 0
						jl		x7_step
						dec		edx
						sub		eax, ENh
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x7_step:
						loop	x7_loop
						mov		buf, edi
					}
				}
				else	{
					_asm	{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, line_w
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x8_loop:
						inc		esi
						add		eax, ebx
						cmp		eax, 0
						jl		x8_step
						inc		edx
						sub		eax, ENh
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x8_step:
						loop	x8_loop
						mov		buf, edi
					}
				}
			}
		}
	}
	// Ｙ基準線
	else	{
		int ENs = -line_h;
		int ENa = line_w<<1;
		int ENh = line_h<<1;
		if (line_add_h < 0)	{
			if (line_add_w < 0)	{
				_asm	{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, line_h
					inc		ecx
				y1_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					dec		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y1_step
					dec		esi
					sub		eax, ENh
				y1_step:
					loop	y1_loop
					mov		buf, edi
				}
			}
			else	{
				_asm	{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, line_h
					inc		ecx
				y2_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					dec		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y2_step
					inc		esi
					sub		eax, ENh
				y2_step:
					loop	y2_loop
					mov		buf, edi
				}
			}
		}
		else	{
			if (line_add_w < 0)	{
				_asm	{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, line_h
					inc		ecx
				y3_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					inc		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y3_step
					dec		esi
					sub		eax, ENh
				y3_step:
					loop	y3_loop
					mov		buf, edi
				}
			}
			else	{
				_asm	{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, line_h
					inc		ecx
				y4_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					inc		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y4_step
					inc		esi
					sub		eax, ENh
				y4_step:
					loop	y4_loop
					mov		buf, edi
				}
			}
		}
	}
	return ((DWORD)(PTR)buf - (DWORD)(PTR)original_buf) / 8;		// / 8 = １要素がint型のＸＹ座標だから
}

// ****************************************************************
// ３２ビットライン描画
// ================================================================
void tnm_draw_32bit_line(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2)
{
	if (dst == NULL)	{	return;	}

	// エリア設定
	tnm_set_area_func(dst_w, dst_h, &ex1, &ey1, &ex2, &ey2);

	// クリップ
	if (!tnm_clip_line(&x1, &y1, &x2, &y2, ex1, ey1, ex2, ey2)) {	return;	}

	// 前処理
	int line_w = x2 - x1;
	int line_h = y2 - y1;
	int line_add_w = 1 * 4;				// * 4 = 32bit化
	int line_add_h = dst_w * 4;			// * 4 = 32bit化
	if (line_w < 0)	{	line_w = -line_w;	line_add_w = -line_add_w;	}
	if (line_h < 0)	{	line_h = -line_h;	line_add_h = -line_add_h;	}

	// アドレス算出
	line_add_h = -line_add_h;
	dst += (x1 + (dst_w * y1)) * 4;		// * 4 = 32bit化

	// カラー
	if (gray_reverse)	{	gray = 255 - gray;	}
	DWORD color = (gray << 24) | (gray << 16) | (gray << 8) | gray;

	// 描画パラメータ
	int ENs, ENa, ENh;
	int ECNT, EALPHA, EBETA;
	if (line_w > line_h)	{
		ENs = -line_w;
		ENa = line_h * 2;
		ENh = line_w * 2;
		ECNT = line_w;
		line_add_h = -line_add_h;
		EALPHA = line_add_w;
		EBETA  = line_add_h;
	}
	else	{
		ENs = -line_h;
		ENa = line_w * 2;
		ENh = line_h * 2;
		ECNT = line_h;
		line_add_h = -line_add_h;
		EALPHA = line_add_h;
		EBETA  = line_add_w;
	}

	if (line_h == 0)	{
		// 点
		if (line_w == 0)	{
			*(DWORD *)dst = color;
		}
		// 水平線
		else	{
			_asm	{
				mov		edi, dst
				mov		ecx, line_w
				mov		eax, color
				mov		edx, line_add_w
				inc		ecx
			line_h_loop:
				mov		[edi], eax		// 32bit
				add		edi, edx
				loop	line_h_loop
			}
		}
	}
	else	{
		// 垂直線
		if (line_w == 0)	{
			_asm	{
				mov		edi, dst
				mov		eax, color
				mov		ecx, line_h
				inc		ecx
				mov		edx, line_add_h
			line_v_loop:
				mov		[edi], eax		// 32bit
				add		edi, edx
				loop	line_v_loop
			}
		}
		// 斜線
		else	{
			_asm	{
				mov		edi, dst
				mov		eax, color
				mov		ecx, ECNT
				inc		ecx
				mov		ebx, EALPHA
				mov		edx, ENs
				mov		esi, ENa
			line_x_loop:
				mov		[edi], eax		// 32bit
				add		edi, ebx
				add		edx, esi
				cmp		edx, 0
				jl		line_x_step
				add		edi, EBETA
				sub		edx, ENh
			line_x_step:
				loop	line_x_loop
			}
		}
	}
}

// ****************************************************************
// ３２ビットエンプティーボックス描画
// ================================================================
void tnm_draw_32bit_empty_box(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2)
{
	if (dst == NULL)	{	return;	}

	int tmp;
	if (y1 > y2)	{	tmp = y1;	y1 = y2;	y2 = tmp;	}

	tnm_draw_32bit_line(dst, dst_w, dst_h, x1, y1, x2, y1, gray, gray_reverse, ex1, ey1, ex2, ey2);

	if (y1 == y2)	{	return;	}
	tnm_draw_32bit_line(dst, dst_w, dst_h, x1, y2, x2, y2, gray, gray_reverse, ex1, ey1, ex2, ey2);

	if ((y1 + 1) == y2)	{	return;	}
	tnm_draw_32bit_line(dst, dst_w, dst_h, x1, (y1 + 1), x1, (y2 - 1), gray, gray_reverse, ex1, ey1, ex2, ey2);

	if ((y1 + 1) == (y2 - 1))	{	return;	}
	tnm_draw_32bit_line(dst, dst_w, dst_h, x2, (y1 + 1), x2, (y2 - 1), gray, gray_reverse, ex1, ey1, ex2, ey2);
}

// ****************************************************************
// ３２ビットフィルボックス描画
// ================================================================
void tnm_draw_32bit_fill_box(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2)
{
	if (dst == NULL)	{	return;	}

	// エリア設定
	tnm_set_area_func(dst_w, dst_h, &ex1, &ey1, &ex2, &ey2);

	// 座標補正
	int tmp;
	if (x1 > x2)	{	tmp = x1;	x1 = x2;	x2 = tmp;	}
	if (y1 > y2)	{	tmp = y1;	y1 = y2;	y2 = tmp;	}

	// 範囲外
	if (x1 > ex2 || x2 < ex1 || y1 > ey2 || y2 < ey1)	{	return;	}

	// 座標補正２
	if (x1 < ex1)	{	x1 = ex1;	}
	if (x2 > ex2)	{	x2 = ex2;	}
	if (y1 < ey1)	{	y1 = ey1;	}
	if (y2 > ey2)	{	y2 = ey2;	}

	// アドレス算出
	dst += (x1 + (dst_w * y1)) * 4;		// * 4 = 32bit化

	// カラー
	if (gray_reverse)	{	gray = 255 - gray;	}
	DWORD color = (gray << 24) | (gray << 16) | (gray << 8) | gray;

	// 描画パラメータ
	int draw_cnt_x = x2 - x1 + 1;
	int draw_cnt_y = y2 - y1 + 1;
	int add_dst_addr = (dst_w - draw_cnt_x) * sizeof(DWORD);

	_asm{
		cld
		mov		edi, dst
		mov		eax, color
		mov		ebx, draw_cnt_x
		mov		edx, add_dst_addr

		mov		ecx, draw_cnt_y
	loop_y:
		push	ecx
		mov		ecx, ebx
		rep		stosd
		pop		ecx
		add		edi, edx
		loop	loop_y
	}
}

// ****************************************************************
// ３２ビットフィルポリゴン描画
// ================================================================

// ３２ビットフィルポリゴン描画ＦＵＮＣ
void tnm_draw_32bit_fillpolygon_func(BYTE *dst, int dst_w, int dst_h, int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2)
{
	ARRAY<int> line_buf1;
	int a_len = (ay1 <= ay2) ? (ay2 - ay1) + 1 : (ay1 - ay2) + 1;
	line_buf1.resize((a_len * 2) + 1);
	int *p1 = line_buf1.get();
	tnm_create_y_line_buffer(p1, a_len, ax1, ay1, ax2, ay2, 0);

	ARRAY<int> line_buf2;
	int b_len = (by1 <= by2) ? (by2 - by1) + 1 : (by1 - by2) + 1;
	line_buf2.resize((b_len * 2) + 1);
	int *p2 = line_buf2.get();
	tnm_create_y_line_buffer(p2, b_len, bx1, by1, bx2, by2, 0);

	int len = (a_len <= b_len) ? a_len : b_len;

	// カラー
	if (gray_reverse)	{	gray = 255 - gray;	}
	DWORD color = (gray << 24) | (gray << 16) | (gray << 8) | gray;

	bool flag = true;
	for (int i = 0; i < len; i++)	{
		if (*p1 > *p2)		{
			break;
		}
		else if (*p1 < *p2)	{
			flag = false;
			break;
		}
		p1 += 2;
		p2 += 2;
	}

	if (flag)	{
		line_buf1.clear();
		line_buf1.resize((a_len * 2) + 1);
		p1 = line_buf2.get();
		p2 = line_buf1.get();
		tnm_create_y_line_buffer(p2, a_len, ax1, ay1, ax2, ay2, 1);
	}
	else	{
		line_buf2.clear();
		line_buf2.resize((b_len * 2) + 1);
		p1 = line_buf1.get();
		p2 = line_buf2.get();
		tnm_create_y_line_buffer(p2, b_len, bx1, by1, bx2, by2, 1);
	}

	int x1, x2, y, draw_cnt;
	BYTE *dp;
	for (int i = 0; i < len; i++)	{
		x1 = *p1;
		x2 = *p2;
		y = *(p1 + 1);
		if (x1 < dst_w && x2 >= 0 && y >= 0 && y < dst_h)	{
			if (x1 < 0)			{	x1 = 0;			}
			if (x2 >= dst_w)	{	x2 = dst_w - 1;	}
			draw_cnt = (x2 - x1) + 1;
			dp = dst + ((x1 + (dst_w * y)) * 4);		// * 4 = 32bit化
			_asm	{
				mov		eax, color
				mov		edi, dp
				mov		ecx, draw_cnt
				rep		stosd
			}
		}
		p1 += 2;
		p2 += 2;
	}

	// 使用したバッファをクリアする
	line_buf1.clear();
	line_buf2.clear();
}

// ３２ビットフィルポリゴン描画（本体）
void tnm_draw_32bit_fillpolygon(BYTE *dst, int dst_w, int dst_h, int x1, int y1, int x2, int y2, int x3, int y3, BYTE gray, bool gray_reverse, int ex1, int ey1, int ex2, int ey2)
{
	if (dst == NULL)	{	return;	}

	if(y1 == y2){
		tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x3, y3, x1, y1, x3, y3, x2, y2, gray, gray_reverse, ex1, ey1, ex2, ey2);
	}
	else if(y1 == y3){
		tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x2, y2, x1, y1, x2, y2, x3, y3, gray, gray_reverse, ex1, ey1, ex2, ey2);
	}
	else if(y2 == y3){
		tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x1, y1, x2, y2, x1, y1, x3, y3, gray, gray_reverse, ex1, ey1, ex2, ey2);
	}
	else{
		if(y1 > y2){
			if(y1 > y3){
				if(y2 > y3){
					// 1 2 3
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x1, y1, x2, y2, x1, y1, x3, y3, gray, gray_reverse, ex1, ey1, ex2, ey2);
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x3, y3, x2, y2, x3, y3, x1, y1, gray, gray_reverse, ex1, ey1, ex2, ey2);
				}
				else{
					// 1 3 2
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x1, y1, x3, y3, x1, y1, x2, y2, gray, gray_reverse, ex1, ey1, ex2, ey2);
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x2, y2, x3, y3, x2, y2, x1, y1, gray, gray_reverse, ex1, ey1, ex2, ey2);
				}
			}
			else{
					// 3 1 2
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x3, y3, x1, y1, x3, y3, x2, y2, gray, gray_reverse, ex1, ey1, ex2, ey2);
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x2, y2, x1, y1, x2, y2, x3, y3, gray, gray_reverse, ex1, ey1, ex2, ey2);
			}
		}
		else{
			if(y2 > y3){
				if(y1 > y3){
					// 2 1 3
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x2, y2, x1, y1, x2, y2, x3, y3, gray, gray_reverse, ex1, ey1, ex2, ey2);
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x3, y3, x1, y1, x3, y3, x2, y2, gray, gray_reverse, ex1, ey1, ex2, ey2);
				}
				else{
					// 2 3 1
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x2, y2, x3, y3, x2, y2, x1, y1, gray, gray_reverse, ex1, ey1, ex2, ey2);
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x1, y1, x3, y3, x1, y1, x2, y2, gray, gray_reverse, ex1, ey1, ex2, ey2);
				}
			}
			else{
					// 3 2 1
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x3, y3, x2, y2, x3, y3, x1, y1, gray, gray_reverse, ex1, ey1, ex2, ey2);
					tnm_draw_32bit_fillpolygon_func(dst, dst_w, dst_h, x1, y1, x2, y2, x1, y1, x3, y3, gray, gray_reverse, ex1, ey1, ex2, ey2);
			}
		}
	}
}

// ****************************************************************
// マスクバッファ確保
// ================================================================
bool tnm_alloc_mask_buffer(S_mask_buffer *buf, int mask_w, int mask_h, BYTE **dp, int *ex1, int *ey1, int *ex2, int *ey2)
{
	if (mask_w <= 0 || mask_h <= 0)	{	return false;	}
	buf->w = mask_w;
	buf->h = mask_h;
	buf->p.resize(buf->w * buf->h * 4);
	*ex1 = 0;
	*ey1 = 0;
	*ex2 = buf->w - 1;
	*ey2 = buf->h - 1;
	*dp = buf->p.get();
	return true;
}

// ****************************************************************
// パターン増殖
// ================================================================

bool tnm_duplicate_mask_func(S_mask_buffer *dst_buf, int dst_w, int dst_h, S_mask_buffer *src_buf)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(dst_buf, dst_w, dst_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	BYTE *sp = src_buf->p.get();
	int w_cnt = dst_buf->w / src_buf->w;
	if (dst_buf->w % src_buf->w)	{	w_cnt++;	}
	int h_cnt = dst_buf->h / src_buf->h;
	if (dst_buf->h % src_buf->h)	{	h_cnt++;	}

	int i, j, x, y;
	y = 0;
	for (i = 0; i < h_cnt; i++)	{
		x = 0;
		for (j = 0; j < w_cnt; j++)	{
			tc_draw_dib_easy(dp, dst_buf->w, dst_buf->h, sp, src_buf->w, src_buf->h, x, y);
			x += src_buf->w;
		}
		y += src_buf->h;
	}

	return true;
}

bool tnm_duplicate_mask(S_mask_buffer *dst_buf, int dst_w, int dst_h, S_mask_buffer *src_buf)
{
	S_mask_buffer tmp;
	if (!tnm_duplicate_mask_func(&tmp, dst_w, src_buf->h, src_buf))	{	return false;	}	// 横に複製
	return tnm_duplicate_mask_func(dst_buf, dst_w, dst_h, &tmp);							// 縦に複製
}

// ****************************************************************
// マスクテクスチャ作成
// ================================================================
bool tnm_make_mask_texture(BSP<C_d3d_texture> *texture, S_mask_buffer *buf)
{
	if (buf->p.empty() || buf->w == 0 || buf->h == 0)	{	return false;	}
	*texture = G_rsm.create_texture(_T("ﾏｽｸﾃｸｽﾁｬ"), buf->w, buf->h, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, false);
	C_d3d_texture *tex = texture->get();
	D3DLOCKED_RECT rect;
	tex->lock_rect(0, &rect, NULL, 0);
	tc_draw_dib_easy((BYTE *)rect.pBits, rect.Pitch / 4, tex->get_height_ex(), buf->p.get(), buf->w, buf->h, 0, 0);
	tex->unlock_rect(0);
	return true;
}

// ****************************************************************
// マスクテクスチャ作成（パターン増殖版）
// ================================================================
bool tnm_make_mask_texture_with_duplicate(BSP<C_d3d_texture> *texture, int target_w, int target_h, S_mask_buffer *buf)
{
	if (target_w != buf->w || target_h != buf->h)	{
		S_mask_buffer dup;
		if (!tnm_duplicate_mask(&dup, target_w, target_h, buf))	{	return false;	}
		return tnm_make_mask_texture(texture, &dup);
	}
	else	{
		return tnm_make_mask_texture(texture, buf);
	}
}
