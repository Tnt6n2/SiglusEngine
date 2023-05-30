#include	"pch.h"

#include	"utility/util_jitan_cnv.h"

// ****************************************************************
// �{��
// ================================================================
bool C_jitan_cnv::convert(ARRAY<BYTE>& dst_wave_ary, BYTE *src_wave, int wave_size, int rate, int channel, int bit, int parcent)
{
	int play_block_smp, delete_block_smp, before_smp, after_smp;

	if (wave_size <= 0)	{	return false;	}

	limit(100, parcent, 400);
	parcent -= 100;

	play_block_smp = rate / 80;		// ���̂R�őS�Ă����܂�
	before_smp     = rate / 110;	// ���̂R�őS�Ă����܂�
	after_smp      = rate / 110;	// ���̂R�őS�Ă����܂�

	delete_block_smp = (play_block_smp * parcent) / 100;

	return convert_func(
		dst_wave_ary, src_wave, wave_size,
		rate, channel, bit,
		play_block_smp, delete_block_smp, before_smp, after_smp,
		1, 1
	);
}

// ****************************************************************
// �t�@���N�V����
// ================================================================
bool C_jitan_cnv::convert_func(ARRAY<BYTE>& dst_wave_ary, BYTE *src_wave, int wave_size, int rate, int channel, int bit, int play_block_smp, int delete_block_smp, int before_smp, int after_smp, int zero_flag, int smooth_flag)
{
	if (!(channel == 1 && bit == 16))	{	return false;	}

	int size_check_byte = (rate / 10) << 1;		// �P�O���̂P�b
	if (wave_size < size_check_byte)	{	return false;	}

	// �Đ�����T���v����
//	int work_base_smp = rate / 40;			// �w���̂P�b
	int work_base_smp = play_block_smp;
	int work_base_byte = work_base_smp << 1;	// 16bit�␳

	// �Đ����Ȃ��T���v����
	int jump_smp = delete_block_smp;
//	int jump_smp = work_base_smp;
//	int jump_smp = (work_base_smp >> 1);
//	int jump_smp = (work_base_smp >> 1) + (work_base_smp >> 2);

	// �o�͐惁�������m�ۂ���
	dst_wave_ary.resize(wave_size + rate * (bit / 8));	// ���S��łP�b�قǗ]���Ɋm�ہB�҂�����ɂ����痎�����i2011/05/10�j
	BYTE *dst_wave = dst_wave_ary.get();

	// �����p�������m��
	int gousei_smp1_max;
	int gousei_smp2_max;
//	gousei_smp1_max = work_base_smp >> 2;	// work_base_smp�̂w���̂P
//	gousei_smp2_max = work_base_smp >> 2;	// work_base_smp�̂w���̂P
	gousei_smp1_max = before_smp;
	gousei_smp2_max = after_smp;
	int alloc_size = (gousei_smp1_max + gousei_smp2_max) << 1;	// 16bit�␳
	ARRAY<BYTE> gousei_ary;
	short *gousei_p = 0;
	short *gousei_p1 = 0;
	short *gousei_p2 = 0;
	if (alloc_size > 0)	{
		gousei_ary.resize(alloc_size);
		gousei_p = (short *)gousei_ary.get();
		gousei_p1 = gousei_p + gousei_smp1_max - 1;
		gousei_p2 = gousei_p + gousei_smp1_max;
	}
	int gousei_smp1_cnt = 0;		// �������̕K�v�͂Ȃ����A�O�̂��ߏ�����
	int gousei_smp2_cnt = 0;		// �������K�v

	// �����J�n
	short *dst = (short *)dst_wave;
	short *src = (short *)src_wave;

	DWORD dst_top_offset = (DWORD)(PTR)dst;
	DWORD dst_end_offset = dst_top_offset + wave_size;
	DWORD src_top_offset = (DWORD)(PTR)src;
	DWORD src_end_offset = src_top_offset + wave_size;

	int new_wave_size = 0;
	int before_copy_smp = 0;

	int copy_smp, copy_byte, smooth_leng, i;
	short *sp, *dp;

	while (1)	{
		if ((DWORD)(PTR)src >= src_end_offset)	{	break;	}

		// �����ȃT���v���ʒu�Ńx�N�g���̍������T���v���̃A�h���X���擾
		if ((DWORD)(PTR)dst != (DWORD)(PTR)dst_wave && zero_flag)	{
			src = convert_func_16bit_rep(dst-1, src, work_base_smp, dst_top_offset, dst_end_offset, src_top_offset, src_end_offset);
		}

		if ((DWORD)(PTR)src >= src_end_offset)	{	break;	}

		// �T���v���l���O���x���ɋ߂��Ō��T���ăT�C�Y��߂�
		if (zero_flag)	{
			copy_byte = convert_func_16bit_copy_size(src, work_base_byte, work_base_smp, src_top_offset, src_end_offset);
		}
		else{
			copy_byte = work_base_byte;
		}

		// �R�s�[�T�C�Y��␳����
		if ((DWORD)(PTR)src + copy_byte >= src_end_offset)	{
			copy_byte = src_end_offset - (DWORD)(PTR)src;
		}
		if (copy_byte < 0)	{	copy_byte = 0;	}
		copy_smp = copy_byte >> 1;	// 16bit�␳

		// �r�q�b�̑O�̃T���v�����擾
		gousei_smp1_cnt = 0;
		sp = src - 1;
		dp = gousei_p1;
		for (i = 0; i < gousei_smp1_max; i++)	{
			if ((DWORD)(PTR)sp < src_top_offset || (DWORD)(PTR)sp >= src_end_offset)	{	break;	}
			*dp-- = *sp--;	gousei_smp1_cnt++;
		}

		// �T���v�����R�s�[
		if (copy_byte > 0)	{	CopyMemory((BYTE *)dst, (BYTE *)src, copy_byte);	}

		// �T���v���̌q���ڂ����炩�Ɍq��
		smooth_leng = 0;
		if ((DWORD)(PTR)dst != (DWORD)(PTR)dst_wave && smooth_flag)	{
			smooth_leng = convert_func_smooth(dst, before_copy_smp, copy_smp, dst_top_offset, dst_end_offset);
		}
		before_copy_smp = copy_smp;

		// �T���v������
		if (gousei_smp1_cnt > 0 || gousei_smp2_cnt > 0)	{
			convert_func_gousei(dst, work_base_smp, dst_top_offset, dst_end_offset, gousei_p2, gousei_smp1_cnt, gousei_smp2_cnt, smooth_leng);
		}

		// �r�q�b�̌�̃T���v�����擾�i����̍��������P�O�̂r�q�b�̌�̃T���v���j
		gousei_smp2_cnt = 0;
		sp = src + copy_smp;
		dp = gousei_p2;
		for (i = 0; i < gousei_smp2_max; i++)	{
			if ((DWORD)(PTR)sp < src_top_offset || (DWORD)(PTR)sp >= src_end_offset)	{	break;	}
			*dp++ = *sp++;	gousei_smp2_cnt++;
		}

		// �A�h���X��i�߂�
		dst += copy_smp;
		src += copy_smp;

		// �V�����E�F�[�u�T�C�Y
		new_wave_size += copy_byte;

		// �Đ����Ȃ��T���v�������΂�
		src += jump_smp;
	}

	// �����p�������J��
	gousei_ary.clear();

	// �Ō���t�F�[�h���Ă���
	dp = (short *)dst_wave + ((new_wave_size >> 1) - 100);
	for (i = 100; i > 0; i--)	{
		if ((DWORD)(PTR)dp < dst_top_offset || (DWORD)(PTR)dp >= dst_end_offset)	{	break;	}
		*dp++ = (*dp * i) / 100;
	}

	// �o�͐惁�����𒲐�����
	dst_wave_ary.resize(new_wave_size);
	return true;
}

// ****************************************************************
// �����ȃT���v���ʒu�Ńx�N�g���̍������T���v���̃A�h���X���擾
// ================================================================
short * C_jitan_cnv::convert_func_16bit_rep(short *dst_last, short *src, int work_base_smp, DWORD dst_top_offset, DWORD dst_end_offset, DWORD src_top_offset, DWORD src_end_offset)
{
	int res;

	int limit_smp_cnt1 = work_base_smp >> 2;		// work_base_smp�̂S���̂P
	int limit_smp_cnt2 = work_base_smp >> 2;		// work_base_smp�̂S���̂P

	if (limit_smp_cnt1 == 0 || limit_smp_cnt2 == 0)	{	return src;	}

	// �O�̃T���v�����擾���x�N�g���𒲂ׂ�
	int vector_flag, target_smp;
	if ((DWORD)(PTR)dst_last >= dst_top_offset && (DWORD)(PTR)dst_last < dst_end_offset)	{
		vector_flag = convert_func_16bit_vector(dst_last, dst_top_offset, dst_end_offset);
		target_smp = *dst_last;
	}
	else{
		vector_flag = 0;
		target_smp = 0;
	}

	// �x�N�g��������Ȃ�A�����x�N�g����T��
	if (vector_flag != 0)	{
		res = convert_func_16bit_rep_func(src, limit_smp_cnt1, limit_smp_cnt2, src_top_offset, src_end_offset, vector_flag, target_smp, -1);
		if (res != -1)	{	src -= res;		return src;	}
		else	{
			res = convert_func_16bit_rep_func(src, limit_smp_cnt1, limit_smp_cnt2, src_top_offset, src_end_offset, vector_flag, target_smp, 1);
			if (res != -1)	{	src += res;		return src;	}
		}
	}

	// �K���ȃx�N�g���̃T���v����������Ȃ������i�������͐����x�N�g���j
	res = convert_func_16bit_rep_func(src, limit_smp_cnt1, limit_smp_cnt2, src_top_offset, src_end_offset, 0, target_smp, -1);
	if (res != -1)	{	src -= res;		return src;	}
	else	{
		res = convert_func_16bit_rep_func(src, limit_smp_cnt1, limit_smp_cnt2, src_top_offset, src_end_offset, 0, target_smp, 1);
		if (res != -1)	{	src += res;		return src;	}
	}
	return src;
}

// ****************************************************************
// �x�N�g������v����K���l�̃T���v����T���iadd = 1 or -1�j
// ================================================================
int C_jitan_cnv::convert_func_16bit_rep_func(short *src, int limit_smp_cnt1, int limit_smp_cnt2, DWORD top_offset, DWORD end_offset, int vector_flag, int target_smp, int add)
{
	int tmp, res;

	short *xp = 0;
	int xp_smp_cnt = -1;	// -1 �͌�����Ȃ������ꍇ�̃��^�[���l
	int test_smp_cnt = 0;
	int get_flag = 0;
	int min_len = 999999;

	// �����x�N�g��
	if (vector_flag == 0)	{
		while (1)	{
			if ((DWORD)(PTR)src < top_offset || (DWORD)(PTR)src >= end_offset)	{	break;	}
			tmp = target_smp - *src;
			if (tmp < 0)	{	tmp = -tmp;	}
			if (tmp < min_len)	{	min_len = tmp;	xp = src;	xp_smp_cnt = test_smp_cnt;	}
			if (xp && min_len <= 100)	{	break;	}
			test_smp_cnt++;
			src += add;
			if (test_smp_cnt >= limit_smp_cnt1)	{	break;	}
			if (test_smp_cnt >= limit_smp_cnt2)	{	if (xp)	{	break;	}	}
		}
	}
	// ���オ��x�N�g���i��菬�����T���v����T���j
	else if (vector_flag == 1)	{
		while (1)	{
			if ((DWORD)(PTR)src < top_offset || (DWORD)(PTR)src >= end_offset)	{	break;	}
			res = convert_func_16bit_vector(src, top_offset, end_offset);
			if (res == vector_flag)	{		// �x�N�g����v
				if (*src <= target_smp)	{
					tmp = target_smp - *src;
					if (get_flag == 0 || tmp < min_len)	{	min_len = tmp;	xp = src;	xp_smp_cnt = test_smp_cnt;	get_flag = 1;	}
				}
				else if (get_flag == 0)	{
					tmp = *src - target_smp;
					if (tmp < min_len)	{	min_len = tmp;	xp = src;	xp_smp_cnt = test_smp_cnt;	}
				}
			}
			if (xp && min_len <= 100)	{	break;	}
			test_smp_cnt++;
			src += add;
			if (test_smp_cnt >= limit_smp_cnt1)	{	break;	}
			if (test_smp_cnt >= limit_smp_cnt2)	{	if (get_flag)	{	break;	}	}
		}
	}
	// �E�オ��x�N�g���i���傫���T���v����T���j
	else	{
		while (1)	{
			if ((DWORD)(PTR)src < top_offset || (DWORD)(PTR)src >= end_offset)	{	break;	}
			res = convert_func_16bit_vector(src, top_offset, end_offset);
			if (res == vector_flag)	{		// �x�N�g����v
				if (*src >= target_smp)	{
					tmp = *src - target_smp;
					if (get_flag == 0 || tmp < min_len)	{	min_len = tmp;	xp = src;	xp_smp_cnt = test_smp_cnt;	get_flag = 1;	}
				}
				else if (get_flag == 0)	{
					tmp = target_smp - *src;
					if (tmp < min_len)	{	min_len = tmp;	xp = src;	xp_smp_cnt = test_smp_cnt;	}
				}
			}
			if (xp && min_len <= 100)	{	break;	}
			test_smp_cnt++;
			src += add;
			if (test_smp_cnt >= limit_smp_cnt1)	{	break;	}
			if (test_smp_cnt >= limit_smp_cnt2)	{	if (get_flag)	{	break;	}	}
		}
	}
	return xp_smp_cnt;
}

// ****************************************************************
// �x�N�g���𒲂ׂ�
// ================================================================
int C_jitan_cnv::convert_func_16bit_vector(short *src, DWORD top_offset, DWORD end_offset)
{
	int i;
	int base_smp = *src;
	int vector_flag = 0;
	short *wp = src - 1;
	for (i = 0; i < 10; i++)	{
		if ((DWORD)(PTR)wp < top_offset || (DWORD)(PTR)wp >= end_offset)	{	break;	}
		if (*wp < base_smp)	{
			vector_flag--;
			if (vector_flag <= -3)	{	break;	}
		}
		else if (*wp > base_smp)	{
			vector_flag++;
			if (vector_flag >=  3)	{	break;	}
		}
		wp--;
	}

	if (vector_flag >= 3)		{	vector_flag = 1;	}
	else if (vector_flag <= -3)	{	vector_flag = -1;	}
	else						{	vector_flag = 0;	}

	return(vector_flag);
}

// ****************************************************************
// �T���v���l���O���x���ɋ߂��Ō��T���ăT�C�Y��߂�
// ================================================================
int C_jitan_cnv::convert_func_16bit_copy_size(short *src, int work_base_byte, int work_base_smp, DWORD top_offset, DWORD end_offset)
{
	int total_copy_byte = 0;

	int copy_byte = end_offset - (DWORD)(PTR)src;
	if (copy_byte <= work_base_byte)	{
		if (copy_byte > 0)	{	total_copy_byte = copy_byte;	}
		return total_copy_byte;
	}

	copy_byte = work_base_byte;

	int min_smp, sign_flag, test_smp;

	// �P�O�T���v���O�܂ł��������A��ԏ����ȃT���v����������
	short *wp = src + (copy_byte>>1) - 1;	// 16bit�␳
	if ((DWORD)(PTR)wp >= top_offset && (DWORD)(PTR)wp<end_offset)	{

		min_smp = *wp;
		if(min_smp < 0){min_smp = -min_smp;}

		wp--;		// �P�O�̃A�h���X����
		int min_i = 0;
		for (int i = 0; i < 10; i++)	{
			if ((DWORD)(PTR)wp < top_offset || (DWORD)(PTR)wp >= end_offset)	{	break;	}
			test_smp = *wp;
			if (test_smp < 0)	{	test_smp = -test_smp;	}
			if (test_smp < min_smp)	{
				min_smp = test_smp;
				min_i = i + 1;
			}
			wp--;
		}

		// ��菬���ȃT���v�������������̂ŁA�R�s�[�o�C�g����␳����
		if (min_i)	{	copy_byte -= (min_i << 1);	}	// 16bit�␳

		// �Ȃ�ׂ��O���x����T������
		wp = src + (copy_byte>>1) - 1;	// 16bit�␳
		if ((DWORD)(PTR)wp >= top_offset && (DWORD)(PTR)wp < end_offset)	{
			min_smp = *wp;
			if (min_smp < 0)	{
				min_smp = -min_smp;
				sign_flag = 1;
			}
			else	{
				sign_flag = 0;
			}
		}
		else	{
			min_smp = 0;
			sign_flag = 0;
		}
	}
	else	{
		min_smp = 0;
	}

	if (min_smp > 0)	{
		int limit_smp_cnt = copy_byte >> 1;	// 16bit�␳
		limit_smp_cnt >>= 2;				// �S���̂P

		// ��_���������
		int min_smpA = min_smp;
		int copy_smp_cnt = 0;
		int test_smp_cnt = 1;
		wp = src + (copy_byte>>1);	// 16bit�␳
		while (1)	{
			if ((DWORD)(PTR)wp < top_offset || (DWORD)(PTR)wp >= end_offset)	{	break;	}
			test_smp = *wp;
			if (test_smp < 0)	{
				if (sign_flag == 0)	{	break;	}	// ���������]������I���
				test_smp = -test_smp;
			}
			else	{
				if (sign_flag == 1)	{	break;	}	// ���������]������I���
			}
			if (test_smp < min_smpA)	{
				copy_smp_cnt = test_smp_cnt;
				min_smpA = test_smp;
				if (min_smpA == 0)	{	break;	}	// �O�T���v��������������I���
			}
			test_smp_cnt++;
			if (test_smp_cnt >= limit_smp_cnt)	{	break;	}
			wp++;
		}
		int copy_smp_cntA = copy_smp_cnt;

		// ��_���O������
		int min_smpB = min_smp;
		copy_smp_cnt = 0;
		test_smp_cnt = 1;
		wp = src + (copy_byte>>1) - 2;	// 16bit�␳
		while (1)	{
			if ((DWORD)(PTR)wp < top_offset || (DWORD)(PTR)wp >= end_offset)	{	break;	}
			test_smp = *wp;
			if(test_smp < 0){
				if (sign_flag == 0)	{	break;	}	// ���������]������I���
				test_smp = -test_smp;
			}
			else	{
				if (sign_flag == 1)	{	break;	}	// ���������]������I���
			}
			if (test_smp < min_smpB)	{
				copy_smp_cnt = test_smp_cnt;
				min_smpB = test_smp;
				if (min_smpB == 0)	{	break;	}	// �O�T���v��������������I���
			}
			test_smp_cnt++;
			if (test_smp_cnt >= limit_smp_cnt)	{	break;	}
			wp--;
		}
		int copy_smp_cntB = copy_smp_cnt;

		// �`�Ƃa�̏����������̗p
		if (min_smpA <= min_smpB)	{
			total_copy_byte = copy_byte + (copy_smp_cntA << 1);		// 16bit�␳
		}
		else	{
			total_copy_byte = copy_byte - (copy_smp_cntB << 1);		// 16bit�␳
		}
	}
	else	{
		total_copy_byte = copy_byte;
	}

	return total_copy_byte;
}

// ****************************************************************
// �T���v���̌q���ڂ����炩�Ɍq���i�␳�l��߂��j
// ================================================================
int C_jitan_cnv::convert_func_smooth(short *src, int before_smp_cnt, int after_smp_cnt, DWORD top_offset, DWORD end_offset)
{
	short *wp1 = src - 1;
	short *wp2 = src;
	if ((DWORD)(PTR)wp1 < top_offset || (DWORD)(PTR)wp1 >= end_offset || (DWORD)(PTR)wp2 < top_offset || (DWORD)(PTR)wp2 >= end_offset)	{	return(0);	}

	// �␳�l
	int smooth_leng = (*wp2 - *wp1) >> 1;


	if (before_smp_cnt > 0)	{
		int xxx;
		for (int i = before_smp_cnt; i > 0; i--)	{
			if ((DWORD)(PTR)wp1 < top_offset || (DWORD)(PTR)wp1 >= end_offset)	{	break;	}
			xxx = (int)(((double)smooth_leng / (double)before_smp_cnt) * (double)i);
			if (xxx == 0)	{	break;	}
			xxx = *wp1 + xxx;
			if(xxx < -32760)		{	xxx = -32760;	}
			else if (xxx > 32760)	{	xxx = 32760;	}
			*wp1-- = xxx;
		}
	}
	if(after_smp_cnt > 0){
		int xxx;
		for (int i = after_smp_cnt; i > 0; i--)	{
			if ((DWORD)(PTR)wp2 < top_offset || (DWORD)(PTR)wp2 >= end_offset)	{	break;	}
			xxx = (int)(((double)smooth_leng / (double)after_smp_cnt) * (double)i);
			if (xxx == 0)	{	break;	}
			xxx = *wp2 - xxx;
			if (xxx < -32760)		{	xxx = -32760;	}
			else if (xxx > 32760)	{	xxx = 32760;	}
			*wp2++ = xxx;
		}
	}

	return smooth_leng;
}

// ****************************************************************
// �T���v������
// ================================================================
void C_jitan_cnv::convert_func_gousei(short *dst, int work_base_smp, DWORD top_offset, DWORD end_offset, short *gousei_p, int gousei_cnt1, int gousei_cnt2, int smooth_leng)
{
	if (gousei_cnt1 > 0)	{
		// �q���ڂ����炩�ɂ��鏈��
		if (smooth_leng != 0)	{
			int xxx;
			short *dp = gousei_p - 1;
			for (int i = 0; i < gousei_cnt1; i++)	{
				xxx = *dp - smooth_leng;
				if(xxx < -32760){xxx = -32760;} else if(xxx > 32760){xxx = 32760;}
				*dp-- = xxx;
			}
		}
		// �����T�C�Y
		short *dp = dst - gousei_cnt1;
		if ((DWORD)(PTR)dp >= end_offset)	{
			gousei_cnt1 = 0;
		}
		else if ((DWORD)(PTR)dp < top_offset)	{
			gousei_cnt1 -= (top_offset - (DWORD)(PTR)dp) >> 1;	// 16bit�␳
		}
	}
	if (gousei_cnt1 < 0)	{	gousei_cnt1 = 0;	}

	if (gousei_cnt2 > 0)	{
		// �q���ڂ����炩�ɂ��鏈��
		if (smooth_leng != 0)	{
			int xxx;
			short *dp = gousei_p;
			for (int i = 0; i < gousei_cnt2; i++)	{
				xxx = *dp + smooth_leng;
				if (xxx < -32760)		{	xxx = -32760;	}
				else if (xxx > 32760)	{	xxx = 32760;	}
				*dp++ = xxx;
			}
		}
		// �����T�C�Y
		short *dp = dst + (gousei_cnt2 - 1);
		if ((DWORD)(PTR)dp < top_offset)	{
			gousei_cnt2 = 0;
		}
		else if ((DWORD)(PTR)dp >= end_offset)	{
			gousei_cnt2 -= (((DWORD)(PTR)dp - end_offset) >> 1) + 1;	// 16bit�␳
		}
	}
	if (gousei_cnt2 < 0)	{	gousei_cnt2 = 0;	}

	// �X���b�v
	if (gousei_cnt2 > 0)	{
		short *sp = gousei_p;
		short *dp = dst;
		short swap_tmp;
		for (int i = 0; i < gousei_cnt2; i++)	{
			swap_tmp = *dp;
			*dp++ = *sp;
			*sp++ = swap_tmp;
		}
	}

	// ����
	int proc_cnt = gousei_cnt1 + gousei_cnt2;
	if(proc_cnt > 0){
		short *sp = gousei_p - gousei_cnt1;
		short *dp = dst      - gousei_cnt1;
		int aaa = proc_cnt - 1;
		int xxx;
		for(int i = 0; i < proc_cnt; i++)	{
			xxx = (int)(
				(((double)(*sp) / (double)aaa) * (double)i) +
				(((double)(*dp) / (double)aaa) * (double)(aaa-i))
			);
			if (xxx < -32760)		{	xxx = -32760;	}
			else if (xxx > 32760)	{	xxx = 32760;	}
			*dp = xxx; sp++; dp++;
		}
	}
}



