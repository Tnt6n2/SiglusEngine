#include	"g00_pch.h"
#include	"unpack.h"

/*=============================================================================
	�k�y�r�r��
int LzssUnPack(UC *data, HANDLE *orghd, void *orgp);
	IN		UC *data      : ���k�f�[�^�̃|�C���^
			HANDLE *orghd : �𓀃f�[�^�̃n���h��(RET)
			void *p       : ���^�[���|�C���^
	OUT		UL            : �𓀃f�[�^�̃T�C�Y 0=ERR or CANCEL
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

	// �T�C�Y�w�b�_�̎擾
	BYTE* p1 = src_data;
	int arc_size = *(DWORD *)p1;	p1 += 4;
	int org_size = *(DWORD *)p1;	p1 += 4;
	if (org_size == 0)
		return 0;
	if (dst_buf == NULL)
		return org_size;

	// �𓀃o�b�t�@�̊m��
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
		jge		loop_out_edi		// jge �Ŕ�����ƁA�j���t�@�C���̏ꍇ�ɏ����ł������I��������ł���B
		cmp		esi, esi_last_adr
		jge		loop_out_esi		// jge �Ŕ�����ƁA�j���t�@�C���̏ꍇ�ɏ����ł������I��������ł���B
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
		jge		loop_out			// jge �Ŕ�����ƁA�j���t�@�C���̏ꍇ�ɏ����ł������I��������ł���B
		mov		dl, byte ptr [esi]
		inc		esi
		mov		dh, 8
	flag_loop:
		cmp		edi, edi_last_adr
		jge		loop_out			// jge �Ŕ�����ƁA�j���t�@�C���̏ꍇ�ɏ����ł������I��������ł���B
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
	�k�y�r�r�R�Q��
int LzssUnPack32(UC *data, HANDLE *orghd, void *orgp);
	IN		UC *data      : ���k�f�[�^�̃|�C���^
			HANDLE *orghd : �𓀃f�[�^�̃n���h��(RET)
			void *p       : ���^�[���|�C���^
	OUT		UL            : �𓀃f�[�^�̃T�C�Y 0=ERR or CANCEL
-----------------------------------------------------------------------------*/
#define		LZSS32_INDEX_BIT_COUNT			12
#define		LZSS32_LENGTH_BIT_COUNT			(16 - LZSS32_INDEX_BIT_COUNT)
#define		LZSS32_WINDOW_SIZE				(1 << LZSS32_INDEX_BIT_COUNT)
#define		LZSS32_LOW_LOOK_AHEAD_SIZE		(1 << LZSS32_LENGTH_BIT_COUNT)
#define		LZSS32_BREAK_EVEN				0		// ���P�ł��}�b�`����Έ��k�R�[�h�̂ق������ł��B

int LzssUnPack32(BYTE *srcdata, BYTE* buf)
{
	BYTE *p1;
	BYTE *p;

	int org_size;
	int arc_size;

	DWORD edi_last_adr;

	if (srcdata == 0)
		return 0;

/* �T�C�Y�w�b�_�̎擾 */
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

/* �𓀃o�b�t�@�̊m�� */
	p = buf;
	edi_last_adr = (DWORD)(UINT_PTR)(p + org_size);

	_asm{
		cld
		mov		esi, p1
		mov		edi, p
		xor		edx, edx
	while_loop:
		cmp		edi, edi_last_adr
		je		loop_out			// jge �Ŕ�����ƁA�j���t�@�C���̏ꍇ�ɏ����ł������I��������ł���B
		mov		dl, byte ptr [esi]
		inc		esi
		mov		dh, 8
	flag_loop:
		cmp		edi, edi_last_adr
		je		loop_out			// jge �Ŕ�����ƁA�j���t�@�C���̏ꍇ�ɏ����ł������I��������ł���B
		test	dl, 0x01
		jz		flag_loop_step1
		movsw
		movsb
		mov		[edi], 255			// ���}�X�N�f�[�^�Q�T�T
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

