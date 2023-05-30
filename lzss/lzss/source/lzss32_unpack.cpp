#include	"pch.h"
#include	"lzss_unpack.h"

/*=============================================================================
	ＬＺＳＳ３２解凍
int lzss32_unpack(BYTE *src_data, BYTE *dst_buf);
	IN		BYTE	*src_data	: 圧縮データのポインタ
			BYTE	*dst_buf	: 解凍先のポインタ
	OUT		int					: 解凍データのサイズ 0=ERR
-----------------------------------------------------------------------------*/

#define		LZSS32_INDEX_BIT_COUNT		12
#define		LZSS32_BREAK_EVEN			0		// ※１個でもマッチすれば圧縮コードのほうが得です。
#define		LZSS32_LENGTH_BIT_COUNT		(16 - LZSS32_INDEX_BIT_COUNT)
#define		LZSS32_LENGTH_AND			((1 << LZSS32_LENGTH_BIT_COUNT) - 1)

int lzss32_unpack(BYTE *src_data, BYTE *dst_buf)
{
	if (src_data == NULL)	return 0;

	// サイズヘッダの取得
	DWORD* header = (DWORD *)src_data;
	int arc_size = *header++;
	int org_size = *header++;
	if (org_size == 0)		return 0;
	if (dst_buf == NULL)	return org_size;

	// 解凍設定
	DWORD edi_last_adr = (DWORD)(UINT_PTR)(dst_buf + org_size);

	_asm{
		cld
		mov		esi, header
		mov		edi, dst_buf
		xor		edx, edx
	__main_loop:
		mov		dl, byte ptr [esi]
		inc		esi
		mov		dh, 8
	__flag_loop:
		cmp		edi, edi_last_adr
		je		__loop_out		// jge で抜けると、破損ファイルの場合に少しでも強制終了を回避できる。
		test	dl, 0x01
		jz		__flag_step
		movsw
		movsb
		mov		[edi], 255		// ←マスクデータ２５５
		inc		edi
		jmp		__flag_check
	__flag_step:
		xor		eax, eax
		lodsw
		mov		ecx, eax
		shr		eax, LZSS32_LENGTH_BIT_COUNT
		shl		eax, 2
		and		ecx, LZSS32_LENGTH_AND
		add		ecx, (LZSS32_BREAK_EVEN + 1)
		mov		ebx, esi		// push
		mov		esi, edi
		sub		esi, eax
		rep		movsd
		mov		esi, ebx		// pop
	__flag_check:
		shr		dl, 1
		dec		dh
		jnz		__flag_loop
		jmp		__main_loop
	__loop_out:
	}
	return org_size;
}

