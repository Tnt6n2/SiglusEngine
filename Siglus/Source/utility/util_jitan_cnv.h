#pragma		once

// ****************************************************************
// 時短コンバータ
// ================================================================
class C_jitan_cnv
{

public:

	// 本体
	bool	convert(ARRAY<BYTE>& dst_wave_ary, BYTE *src_wave, int wave_size, int rate, int channel, int bit, int parcent);

private:

	// ファンクション
	bool	convert_func(ARRAY<BYTE>& dst_wave_ary, BYTE *src_wave, int wave_size, int rate, int channel, int bit, int play_block_smp, int delete_block_smp, int before_smp, int after_smp, int zero_flag, int smooth_flag);

	// 小さなサンプル位置でベクトルの合ったサンプルのアドレスを取得
	short *		convert_func_16bit_rep(short *dst_last, short *src, int work_base_smp, DWORD dst_top_offset, DWORD dst_end_offset, DWORD src_top_offset, DWORD src_end_offset);

	// ベクトルが一致する適正値のサンプルを探す（add = 1 or -1）
	int		convert_func_16bit_rep_func(short *src, int limit_smp_cnt1, int limit_smp_cnt2, DWORD top_offset, DWORD end_offset, int vector_flag, int target_smp, int add);

	// ベクトルを調べる
	int		convert_func_16bit_vector(short *src, DWORD top_offset, DWORD end_offset);

	// サンプル値が０レベルに近い最後を探してサイズを戻す
	int		convert_func_16bit_copy_size(short *src, int work_base_byte, int work_base_smp, DWORD top_offset, DWORD end_offset);

	// サンプルの繋ぎ目を滑らかに繋ぐ（補正値を戻す）
	int		convert_func_smooth(short *src, int before_smp_cnt, int after_smp_cnt, DWORD top_offset, DWORD end_offset);

	// サンプル合成
	void	convert_func_gousei(short *dst, int work_base_smp, DWORD top_offset, DWORD end_offset, short *gousei_p, int gousei_cnt1, int gousei_cnt2, int smooth_leng);

};

