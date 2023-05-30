#pragma		once

// ****************************************************************
// ���Z�R���o�[�^
// ================================================================
class C_jitan_cnv
{

public:

	// �{��
	bool	convert(ARRAY<BYTE>& dst_wave_ary, BYTE *src_wave, int wave_size, int rate, int channel, int bit, int parcent);

private:

	// �t�@���N�V����
	bool	convert_func(ARRAY<BYTE>& dst_wave_ary, BYTE *src_wave, int wave_size, int rate, int channel, int bit, int play_block_smp, int delete_block_smp, int before_smp, int after_smp, int zero_flag, int smooth_flag);

	// �����ȃT���v���ʒu�Ńx�N�g���̍������T���v���̃A�h���X���擾
	short *		convert_func_16bit_rep(short *dst_last, short *src, int work_base_smp, DWORD dst_top_offset, DWORD dst_end_offset, DWORD src_top_offset, DWORD src_end_offset);

	// �x�N�g������v����K���l�̃T���v����T���iadd = 1 or -1�j
	int		convert_func_16bit_rep_func(short *src, int limit_smp_cnt1, int limit_smp_cnt2, DWORD top_offset, DWORD end_offset, int vector_flag, int target_smp, int add);

	// �x�N�g���𒲂ׂ�
	int		convert_func_16bit_vector(short *src, DWORD top_offset, DWORD end_offset);

	// �T���v���l���O���x���ɋ߂��Ō��T���ăT�C�Y��߂�
	int		convert_func_16bit_copy_size(short *src, int work_base_byte, int work_base_smp, DWORD top_offset, DWORD end_offset);

	// �T���v���̌q���ڂ����炩�Ɍq���i�␳�l��߂��j
	int		convert_func_smooth(short *src, int before_smp_cnt, int after_smp_cnt, DWORD top_offset, DWORD end_offset);

	// �T���v������
	void	convert_func_gousei(short *dst, int work_base_smp, DWORD top_offset, DWORD end_offset, short *gousei_p, int gousei_cnt1, int gousei_cnt2, int smooth_leng);

};

