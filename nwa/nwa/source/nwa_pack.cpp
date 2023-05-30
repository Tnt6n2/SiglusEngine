#include	"pch.h"
#include	"nwa_pack.h"

// ****************************************************************
// ＮＷＡ圧縮 使い方
// ================================================================
/*
	Cnwa_pack nwa;

	// ★圧縮準備（ソースデータへのポインタとデータサイズ、チャンネル、ビット数、周波数、圧縮モード、ＺＥＲＯモード、を指定。ループ抜けする為に処理数を指定。 0 の場合、バッファが足りないか、完了するまで返りません）
	nwa.ready((DWORD*)src_data, src_data_size, channels, bits_per_sample, samples_per_sec, pack_mod[0 ～ 5], zero_mod[ 0 | 1 ], 1024*100);

	while (1)	{

		// ★圧縮開始（出力先バッファのポインタとサイズを指定）
		if (nwa.proc(dst_buf, dst_buf_size))	{
			// ★出力先バッファのサイズ調整
			nwa.pack_size

			break;	// ★圧縮完了
		}

		if (nwa.dst_buf_need_size > 0)	{
			// ★出力先バッファの再確保
		}

		// ★プログレスバーなどの処理
		// nwa.src_complate_size * 100％) / src_data_size
	}

	// ★圧縮完了後の処理

*/

// ****************************************************************
// ＮＷＡ圧縮 コンストラクタ
// ================================================================
Cnwa_pack::Cnwa_pack()
{
	// 関数テーブル
	_pack_func_dat_set[0] = &Cnwa_pack::_pack_func_dat_set0;
	_pack_func_dat_set[1] = &Cnwa_pack::_pack_func_dat_set1;
	_pack_func_dat_set[2] = &Cnwa_pack::_pack_func_dat_set2;
	_pack_func_dat_set[3] = &Cnwa_pack::_pack_func_dat_set3;
	_pack_func_dat_set[4] = &Cnwa_pack::_pack_func_dat_set4;
	_pack_func_dat_set[5] = &Cnwa_pack::_pack_func_dat_set5;
}

// ****************************************************************
// ＮＷＡ圧縮 準備
// ================================================================
bool Cnwa_pack::ready(BYTE* src_data_, DWORD src_data_size, WORD channels, WORD bits_per_sample, DWORD samples_per_sec, int pack_mod, int zero_mod, DWORD break_out_size_)
{
	_proc_step = __COMPLATE;	// プロセスステップ
	src_complate_size = 0;		// 処理完了サイズ
	dst_buf_need_size = 0;		// 足りないバッファ数
	pack_size = 0;				// 圧縮サイズ

	// 圧縮モードチェック（-1=無圧縮  0=低品質  1=中品質  2=高品質  3=より高品質  4=よりより高品質  5=よりよりより高品質）
	if (pack_mod < -1 || pack_mod > 5)	{	return false;	}
	// １６ビットのみ対応
	if (bits_per_sample != 16)			{	return false;	}
	// モノラル／ステレオのみ対応
	if (channels != 1 && channels != 2)	{	return false;	}

	// １サンプルのバイトサイズ
	_byte_per_sample = bits_per_sample / 8;

	// ソースデータサイズを補正（ステレオで奇数サンプルだった場合、、最後の１サンプルは余分です）
	if (channels == 2)	{	src_data_size = ((src_data_size / 2) & 0xfffffe) * 2;	}

	// ソースデータが無い
	if (src_data_size <= 0)				{	return false;	}

	// ヘッダ設定
	_header.channels        = channels;				// チャンネル（1=ﾓﾉﾗﾙ / 2=ｽﾃﾚｵ）
	_header.bits_per_sample = bits_per_sample;		// ビット（8bit / 16bit）
	_header.samples_per_sec = samples_per_sec;		// 周波数（11025 / 22050 / 44100）

	_header.pack_mod = pack_mod;					// 圧縮モード
	_header.zero_mod = zero_mod;					// 無音圧縮モード

	_header.original_size   = src_data_size;		// オリジナルサイズ
	_header.pack_size       = 0;					// 圧縮後のサイズ（★圧縮完了後に設定）
	_header.sample_cnt      = src_data_size / _byte_per_sample;	// サンプル数

	_header.unit_sample_cnt = NWA_UNIT_SAMPLE_CNT;							// 圧縮単位のサンプル数
	_header.unit_cnt        = _header.sample_cnt / _header.unit_sample_cnt;	// ユニット数
	_header.last_sample_cnt = _header.sample_cnt % _header.unit_sample_cnt;	// 最後の圧縮のサンプル数
	_header.last_sample_pack_size = 0;										// 最後の圧縮サイズ（★圧縮完了後に設定）

	// 最後の圧縮補正
	if (_header.last_sample_cnt)	{	_header.unit_cnt++;								}
	else							{	_header.last_sample_cnt = NWA_UNIT_SAMPLE_CNT;	}

	// 出力先の必要サイズ
	if (_header.pack_mod == -1)		{	_dst_buf_additional = sizeof(NWA_HEADER_STRUCT);										}	// 無圧縮モード
	else							{	_dst_buf_additional = sizeof(NWA_HEADER_STRUCT) + (_header.unit_cnt * sizeof(DWORD));	}	// 圧縮モード

	// ユニット情報
	_unit_index     = 0;											// ユニットのインデックス
	_unit_size      = NWA_UNIT_SAMPLE_CNT * _byte_per_sample;		// ユニットのサイズ
	_last_unit_size = _header.last_sample_cnt * _byte_per_sample;	// 最後のユニットのサイズ

	_src_data       = src_data_;		// ソースデータのポインタ
	_pack_buf_size  = 0;				// 圧縮バッファサイズ
	_break_out_size = break_out_size_;	// ループ抜け処理サイズ
	_break_out_last = 0;				// 前回のループ抜け値
	_proc_step      = __START;			// プロセスステップ

	return true;
}

// ****************************************************************
// ＮＷＡ圧縮 プロセス
// ================================================================
bool Cnwa_pack::proc(BYTE* dst_buf_, DWORD dst_buf_size_)
{
	_dst_buf = dst_buf_;
	_dst_buf_size = dst_buf_size_;
	dst_buf_need_size = 0;

	switch (_proc_step)	{
		case __START:
			if (!_proc_start())			{	return false;	}			// スタート
			_proc_step = __PACK;
			break;
		case __COPY:
			if (!_proc_copy())			{	return false;	}			// コピー
			_proc_step = __PACK;
			break;
		case __LAST_COPY:
			if (!_proc_copy())			{	return false;	}			// コピー
			_proc_step = __END;
			return _proc_end();											// 終了処理
			break;
		case __END:
			return _proc_end();											// 終了処理
			break;
		case __COMPLATE:
			return true;
			break;
	}

	while (1)	{

		// ラストフラグ
		int last_flag = _unit_index == (_header.unit_cnt - 1) ? true : false;

		// ユニットサイズ
		DWORD unit_size = last_flag ? _last_unit_size : _unit_size;

		if (_header.pack_mod == -1)	{	// 無圧縮モード
			_pack.dst_p = _src_data + src_complate_size;
			_dst_buf_additional = unit_size;
		}
		else						{	// 圧縮モード
			// オフセット設定
			*((DWORD*)(_dst_buf + sizeof(NWA_HEADER_STRUCT)) + _unit_index) = _pack_buf_size;

			// 圧縮
			_dst_buf_additional = _pack_func_pack(_src_data + src_complate_size, unit_size);

			// 最後の圧縮サイズ
			if (last_flag)	{	_header.last_sample_pack_size = _dst_buf_additional;	}
		}

		// 次へ
		_unit_index++;
		src_complate_size += unit_size;

		// コピー
		if (last_flag)	{
			_proc_step = __LAST_COPY;
			if (!_proc_copy())	{	return false;	}
			_proc_step = __END;
			return _proc_end();		// 終了処理
		}
		else	{
			_proc_step = __COPY;
			if (!_proc_copy())	{	return false;	}
			_proc_step = __PACK;
		}

		// 処理完了サンプル数に応じてループアウトする
		if (_break_out_size > 0)	{
			DWORD tmp = src_complate_size / _break_out_size;
			if (_break_out_last != tmp)	{
				_break_out_last = tmp;
				return false;	// 一旦処理を返す
			}
		}
	}
}

// ****************************************************************
// ＮＷＡ圧縮 プロセス スタート
// ================================================================
bool Cnwa_pack::_proc_start()
{
	if (!_proc_check_dst_buf_size())	{	return false;	}		// 出力先サイズを判定

	_pack_buf_size += _dst_buf_additional;
	_dst_buf_additional = 0;
	pack_size = _pack_buf_size;
	return true;
}

// ****************************************************************
// ＮＷＡ圧縮 プロセス コピー
// ================================================================
bool Cnwa_pack::_proc_copy()
{
	if (!_proc_check_dst_buf_size())	{	return false;	}		// 出力先サイズを判定

	CopyMemory(_dst_buf + _pack_buf_size, _pack.dst_p, _dst_buf_additional);

	_pack_buf_size += _dst_buf_additional;
	_dst_buf_additional = 0;
	pack_size = _pack_buf_size;
	return true;
}

// ****************************************************************
// ＮＷＡ圧縮 プロセス 終了処理
// ================================================================
bool Cnwa_pack::_proc_end()
{
	_header.pack_size = pack_size;		// 圧縮後のサイズ

	// 圧縮モードの補正
	switch (_header.pack_mod)	{
		case -1:								// 無圧縮モード（不要なメンバの初期化）
			_header.unit_sample_cnt = 0;		// 圧縮単位のサンプル数
			_header.unit_cnt        = 0;		// ユニット数
			_header.last_sample_cnt = 0;		// 最後の圧縮のサンプル数
			_header.last_sample_pack_size = 0;	// 最後の圧縮サイズ（★圧縮完了後に設定）
			break;

		case 0:		_header.pack_mod = 2;	break;
		case 1:		_header.pack_mod = 1;	break;
		case 2:		_header.pack_mod = 0;	break;
	}

	CopyMemory(_dst_buf, &_header, sizeof(NWA_HEADER_STRUCT));
	_proc_step = __COMPLATE;
	return true;	// 圧縮完了
}

// ****************************************************************
// ＮＷＡ圧縮 プロセス 出力先サイズを判定
// ================================================================
bool Cnwa_pack::_proc_check_dst_buf_size()
{
	DWORD need_size = _pack_buf_size + _dst_buf_additional + 1024;		// 余裕を見て１Ｋ確保
	if (_dst_buf_size < need_size)	{
		dst_buf_need_size = need_size - _dst_buf_size;
		return false;	// バッファサイズが足りません
	}
	return true;
}

