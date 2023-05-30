#include	"pch.h"
#include	"tpc_lzss.h"

/*=============================================================================
	ＴＰＣ ＬＺＳＳ解凍
int tpc_lzss_unpack(BYTE *src_data, BYTE *dst_buf);
	IN		BYTE	*src_data	: 圧縮データのポインタ
			BYTE	*dst_buf	: 解凍先のポインタ
	OUT		int					: 解凍データのサイズ 0=ERR
-----------------------------------------------------------------------------*/

struct TPC_LZSS_HEADER{
	BYTE id[8];
	int org_size;
	int arc_size;
};

int tpc_lzss_unpack(BYTE *src_data, BYTE *dst_buf)
{
	if(src_data == NULL)	return 0;

	// サイズヘッダの取得
	TPC_LZSS_HEADER *header = (TPC_LZSS_HEADER *)src_data;
	if (header->id[0] == 'P' && header->id[0] == 'A' && header->id[0] == 'C' && header->id[0] == 'K')	return 0;
	int arc_size = header->arc_size;
	int org_size = header->org_size;
	if (org_size == 0)		return 0;
	if (dst_buf == NULL)	return org_size;

	// 解凍設定
	BYTE *p = src_data + sizeof(TPC_LZSS_HEADER);
	DWORD esi_last_adr = (DWORD)(UINT_PTR)(src_data + arc_size);

	_asm{
		cld
		mov		esi, p
		mov		ebx, esi_last_adr
		mov		edi, dst_buf
		xor		edx, edx
	__loop:
		cmp		esi, ebx
		jl		__continue
		jmp		__loop_out
	__continue:
		or		dh, dh
		jnz		__not_zero
		mov		dl, byte ptr [esi]
		inc		esi
		mov		dh, 8
		jmp		__loop
	__not_zero:
		test	dl, 0x80
		jz		__step
		movsb
		jmp		__next
	__step:
		xor		eax, eax
		mov		ax, word ptr [esi]
		add		esi, 2
		mov		ecx, eax
		and		ecx, 0x0f
		shr		eax, 4
		inc		eax
		add		ecx, 2
		push	esi
		mov		esi, edi
		sub		esi, eax
		rep		movsb
		pop		esi
	__next:
		shl		dl, 1
		dec		dh
		jmp		__loop
	__loop_out:
	}

	return org_size;
}

