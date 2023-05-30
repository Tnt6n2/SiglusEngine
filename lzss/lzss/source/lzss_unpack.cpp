#include	"pch.h"
#include	"lzss_unpack.h"

// ****************************************************************
// �k�y�r�r��
//		src_data	: ���k�f�[�^�̃|�C���^
// 		dst_buf		: �𓀐�̃|�C���^�iNULL = �𓀌�̃T�C�Y���߂�܂��j
// ================================================================

#define		LZSS_INDEX_BIT_COUNT		12
#define		LZSS_BREAK_EVEN				1
#define		LZSS_LENGTH_BIT_COUNT		(16 - LZSS_INDEX_BIT_COUNT)
#define		LZSS_LENGTH_AND				((1 << LZSS_LENGTH_BIT_COUNT) - 1)

int lzss_unpack(BYTE *src_data, BYTE *dst_buf)
{
	if (src_data == NULL)	return 0;

	// �T�C�Y�w�b�_�̎擾
	DWORD* header = (DWORD *)src_data;
	int arc_size = *header++;
	int org_size = *header++;
	if (org_size == 0)		return 0;
	if (dst_buf == NULL)	return org_size;

	// �𓀐ݒ�
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
		je		__loop_out		// jge �Ŕ�����ƁA�j���t�@�C���̏ꍇ�ɏ����ł������I��������ł���B
		test	dl, 0x01
		jz		__flag_step
		movsb
		jmp		__flag_check
	__flag_step:
		xor		eax, eax
		lodsw
		mov		ecx, eax
		shr		eax, LZSS_LENGTH_BIT_COUNT
		and		ecx, LZSS_LENGTH_AND
		add		ecx, (LZSS_BREAK_EVEN + 1)
		mov		ebx, esi		// push
		mov		esi, edi
		sub		esi, eax
		rep		movsb
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

