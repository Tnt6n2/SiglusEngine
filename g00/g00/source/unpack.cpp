#include	"g00_pch.h"
#include	"unpack.h"

/*=============================================================================
	ＬＺＳＳ解凍
int LzssUnPack(UC *data, HANDLE *orghd, void *orgp);
	IN		UC *data      : 圧縮データのポインタ
			HANDLE *orghd : 解凍データのハンドル(RET)
			void *p       : リターンポインタ
	OUT		UL            : 解凍データのサイズ 0=ERR or CANCEL
-----------------------------------------------------------------------------*/
#define		LZSS_INDEX_BIT_COUNT		12
#define		LZSS_LENGTH_BIT_COUNT		(16 - LZSS_INDEX_BIT_COUNT)
#define		LZSS_WINDOW_SIZE			(1 << LZSS_INDEX_BIT_COUNT)
#define		LZSS_LOW_LOOK_AHEAD_SIZE	(1 << LZSS_LENGTH_BIT_COUNT)
#define		LZSS_BREAK_EVEN				((1 + LZSS_INDEX_BIT_COUNT + LZSS_LENGTH_BIT_COUNT) / 9)		// 1

int LzssUnPack(BYTE* src_data, BYTE* dst_buf)
{
	if (src_data == 0)
		return 0;

	// サイズヘッダの取得
	BYTE* p1 = src_data;
	int arc_size = *(DWORD *)p1;	p1 += 4;
	int org_size = *(DWORD *)p1;	p1 += 4;
	if (org_size == 0)
		return 0;
	if (dst_buf == NULL)
		return org_size;

	// 解凍バッファの確保
	BYTE* p = dst_buf;
	DWORD edi_last_adr = (DWORD)(UINT_PTR)(p + org_size);

#if 0
	_asm{
		cld
		mov		esi, p1
		mov		edi, p
		xor		edx, edx
	while_loop:
		mov		dl, byte ptr [esi]
		inc		esi
		mov		dh, 8
	flag_loop:
		cmp		edi, edi_last_adr
		jge		loop_out_edi		// jge で抜けると、破損ファイルの場合に少しでも強制終了を回避できる。
		cmp		esi, esi_last_adr
		jge		loop_out_esi		// jge で抜けると、破損ファイルの場合に少しでも強制終了を回避できる。
		test	dl, 0x01
		jz		flag_loop_step1
		movsb
		jmp		flag_loop_check
	flag_loop_step1:
		xor		eax, eax
		lodsw
		mov		ecx, eax
		shr		eax, 4
		and		ecx, 0x0f
		add		ecx, (LZSS_BREAK_EVEN + 1)
		mov		ebx, esi		// push
		mov		esi, edi
		sub		esi, eax
		rep		movsb
		mov		esi, ebx		// pop
	flag_loop_check:
		shr		dl, 1
		dec		dh
		jnz		flag_loop
		jmp		while_loop
	loop_out_esi:
		cmp		esi, esi_last_adr
		je		loop_out
		mov		limit_flag, 1
		jmp		loop_out
	loop_out_edi:
		cmp		edi, edi_last_adr
		je		loop_out
		mov		limit_flag, 1
	loop_out:
	}

#else
	_asm{
		cld
		mov		esi, p1
		mov		edi, p
		xor		edx, edx
	while_loop:
		cmp		edi, edi_last_adr
		jge		loop_out			// jge で抜けると、破損ファイルの場合に少しでも強制終了を回避できる。
		mov		dl, byte ptr [esi]
		inc		esi
		mov		dh, 8
	flag_loop:
		cmp		edi, edi_last_adr
		jge		loop_out			// jge で抜けると、破損ファイルの場合に少しでも強制終了を回避できる。
		test	dl, 0x01
		jz		flag_loop_step1
		movsb
		jmp		flag_loop_check
	flag_loop_step1:
		xor		eax, eax
		lodsw
		mov		ecx, eax
		shr		eax, 4
		and		ecx, 0x0f
		add		ecx, (LZSS_BREAK_EVEN + 1)
		mov		ebx, esi		// push
		mov		esi, edi
		sub		esi, eax
		rep		movsb
		mov		esi, ebx		// pop
	flag_loop_check:
		shr		dl, 1
		dec		dh
		jnz		flag_loop
		jmp		while_loop
	loop_out:

		mov		p1, esi
	}
#endif

	return org_size;
}


/*=============================================================================
	ＬＺＳＳ３２解凍
int LzssUnPack32(UC *data, HANDLE *orghd, void *orgp);
	IN		UC *data      : 圧縮データのポインタ
			HANDLE *orghd : 解凍データのハンドル(RET)
			void *p       : リターンポインタ
	OUT		UL            : 解凍データのサイズ 0=ERR or CANCEL
-----------------------------------------------------------------------------*/
#define		LZSS32_INDEX_BIT_COUNT			12
#define		LZSS32_LENGTH_BIT_COUNT			(16 - LZSS32_INDEX_BIT_COUNT)
#define		LZSS32_WINDOW_SIZE				(1 << LZSS32_INDEX_BIT_COUNT)
#define		LZSS32_LOW_LOOK_AHEAD_SIZE		(1 << LZSS32_LENGTH_BIT_COUNT)
#define		LZSS32_BREAK_EVEN				0		// ※１個でもマッチすれば圧縮コードのほうが得です。

int LzssUnPack32(BYTE *srcdata, BYTE* buf)
{
	BYTE *p1;
	BYTE *p;

	int org_size;
	int arc_size;

	DWORD edi_last_adr;

	if (srcdata == 0)
		return 0;

/* サイズヘッダの取得 */
	p1 = srcdata;
	arc_size = *(DWORD *)p1;
	p1 += 4;
	org_size = *(DWORD *)p1;
	p1 += 4;

	if(org_size == 0){
		return 0;
	}

	if (buf == 0)	{
		return org_size;
	}

/* 解凍バッファの確保 */
	p = buf;
	edi_last_adr = (DWORD)(UINT_PTR)(p + org_size);

	_asm{
		cld
		mov		esi, p1
		mov		edi, p
		xor		edx, edx
	while_loop:
		cmp		edi, edi_last_adr
		je		loop_out			// jge で抜けると、破損ファイルの場合に少しでも強制終了を回避できる。
		mov		dl, byte ptr [esi]
		inc		esi
		mov		dh, 8
	flag_loop:
		cmp		edi, edi_last_adr
		je		loop_out			// jge で抜けると、破損ファイルの場合に少しでも強制終了を回避できる。
		test	dl, 0x01
		jz		flag_loop_step1
		movsw
		movsb
		mov		[edi], 255			// ←マスクデータ２５５
		inc		edi
		jmp		flag_loop_check
	flag_loop_step1:
		xor		eax, eax
		lodsw
		mov		ecx, eax
		shr		eax, 4
		shl		eax, 2
		and		ecx, 0x0f
		add		ecx, (LZSS32_BREAK_EVEN + 1)
		mov		ebx, esi		// push
		mov		esi, edi
		sub		esi, eax
		rep		movsd
		mov		esi, ebx		// pop
	flag_loop_check:
		shr		dl, 1
		dec		dh
		jnz		flag_loop
		jmp		while_loop
	loop_out:
	}

	return org_size;
}

