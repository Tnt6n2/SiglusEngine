#include	"pch.h"
#include	"lzss_tree.h"
#include	"lzss_tree_find.h"
#include	"lzss_pack.h"

// ****************************************************************
// ＬＺＳＳ圧縮 使い方
// ================================================================
/*
	Clzss_pack lzss;

	// ★圧縮準備（ソースデータへのポインタとデータ数を指定。ループ抜けする為に処理数を指定。 0 の場合、バッファが足りないか、完了するまで返りません）
	lzss.ready(src_data, src_data_cnt, 1024*100);

	while (1)	{

		// ★圧縮開始（出力先バッファのポインタとサイズを指定）
		if (lzss.proc(dst_buf, dst_buf_size))	{
			// ★出力先バッファのサイズ調整
			lzss.pack_size

			break;	// ★圧縮完了
		}

		if (lzss.dst_buf_need_size > 0)	{
			// ★出力先バッファの再確保
		}

		// ★プログレスバーなどの処理
		// lzss.src_complate_cnt * 100％) / src_data_cnt
	}

	// ★圧縮完了後の処理

*/

// ****************************************************************
// ＬＺＳＳ圧縮 準備
// ================================================================

#define		LZSS_INDEX_BIT_COUNT		12
#define		LZSS_BREAK_EVEN				1

void Clzss_pack::ready(BYTE* src_data_, DWORD src_data_cnt_, DWORD break_out_cnt_)
{
	_src_data = src_data_;
	_src_data_cnt = src_data_cnt_;

	_sizeof_data_size = sizeof(BYTE);
	_window_size = 1 << LZSS_INDEX_BIT_COUNT;
	_break_even = LZSS_BREAK_EVEN;
	_length_bit_count = 16 - LZSS_INDEX_BIT_COUNT;
	_look_ahead_size = (1 << _length_bit_count) + _break_even;

	_pack_buf_size = 8;		// 圧縮バッファサイズ 8 = ORG_SIZE + ARC_SIZE
	_pack_bit_count = 0;	// 圧縮ビットカウント

	// 圧縮データ
	_pack_data_count = 1;	// １バイト目はBitFlag
	_pack_data[0] = 0;

	// ツリー検索準備
	_tree_find.ready(_src_data, _src_data_cnt, _window_size, _look_ahead_size);

	_replace_cnt = 0;

	_proc_step = __PACK;		// プロセスステップ

	_break_out_cnt = break_out_cnt_;		// ループ抜け処理数
	_break_out_last = 0;					// 前回のループ抜け値

	src_complate_cnt = 0;	// 処理完了数
	dst_buf_need_size = 0;	// 足りないバッファ数
	pack_size = 0;			// 圧縮サイズ
}

// ****************************************************************
// ＬＺＳＳ圧縮 プロセス
// ================================================================
bool Clzss_pack::proc(BYTE* dst_buf_, DWORD dst_buf_size_)
{
	_dst_buf = dst_buf_;
	_dst_buf_size = dst_buf_size_;
	dst_buf_need_size = 0;

	switch (_proc_step)	{
		case __COPY:
			if (!_proc_copy())		{	return false;	}		// 圧縮データをバッファにコピーする
			break;
		case __LAST_COPY:
			if (!_proc_last_copy())	{	return false;	}		// 最後の圧縮データをバッファにコピーする
			_proc_step = __END;
			return _proc_end();		// 終了処理
			break;
		case __END:
			return _proc_end();		// 終了処理
			break;
		case __COMPLATE:
			return true;
			break;
	}

	_proc_step = __PACK;
	while (1)	{
		if (_make_pack_data())	{	// 圧縮データを生成
			_proc_step = __COPY;
			if (!_proc_copy())		{	return false;	}		// 圧縮データをバッファにコピーする
			_proc_step = __PACK;

			// 処理完了数に応じてループアウトする
			if (_break_out_cnt > 0)	{
				DWORD tmp = src_complate_cnt / _break_out_cnt;
				if (_break_out_last != tmp)	{
					_break_out_last = tmp;
					return false;	// 一旦処理を返す
				}
			}
		}
		else	{
			_proc_step = __LAST_COPY;
			if (!_proc_last_copy())	{	return false;	}		// 最後の圧縮データをバッファにコピーする
			_proc_step = __END;
			return _proc_end();		// 終了処理
		}
	}
}

// ****************************************************************
// ＬＺＳＳ圧縮 プロセス 圧縮データをバッファにコピーする
// ================================================================
bool Clzss_pack::_proc_copy()
{
	if (!_copy_pack_buf(false))	{
		if(dst_buf_need_size > 0)	{
			return false;	// バッファサイズが足りません
		}
	}
	return true;	// コピー完了（もしくはコピーしていない）
}

// ****************************************************************
// ＬＺＳＳ圧縮 プロセス 最後の圧縮データをバッファにコピーする
// ================================================================
bool Clzss_pack::_proc_last_copy()
{
	_copy_pack_buf(true);
	if (dst_buf_need_size > 0)	{
		return false;	// バッファサイズが足りません
	}

	return true;	// コピー完了
}

// ****************************************************************
// ＬＺＳＳ圧縮 プロセス 終了処理
// ================================================================
bool Clzss_pack::_proc_end()
{
	if (_dst_buf_size < 8)	{
		dst_buf_need_size = 8 - _dst_buf_size;
		return false;	// バッファサイズが足りません
	}

	DWORD* p = (DWORD *)_dst_buf;
	*p++ = _pack_buf_size;
	*p++ = _src_data_cnt * _sizeof_data_size;
	pack_size = _pack_buf_size;		// 圧縮サイズ
	_proc_step = __COMPLATE;
	return true;	// 圧縮完了
}

// ****************************************************************
// ＬＺＳＳ圧縮 データ化
// ================================================================
bool Clzss_pack::_make_pack_data()
{
	static int bit[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

	// 終了
	if (_tree_find.src_data_index >= _src_data_cnt)	{	return false;	}

	// ツリー検索
	if (_replace_cnt > 0)	{	_tree_find.proc(_replace_cnt);	}

	// 処理完了数
	src_complate_cnt = _tree_find.src_data_index;

	// 終了
	if (_tree_find.src_data_index >= _src_data_cnt)	{	return false;	}

	// データ設定
	if (_tree_find.matching_size <= _break_even)	{
		_replace_cnt = 1;
		_pack_data[0] |= bit[_pack_bit_count];		// １文字出力のＢＩＴフラグＳＥＴ
		_pack_data[_pack_data_count] = *(_src_data + _tree_find.src_data_index);
		_pack_data_count++;
	}
	else	{
		_replace_cnt = _tree_find.matching_size;
		DWORD tmp = ((_tree_find.window_top_index - _tree_find.matching_target) % _window_size) << _length_bit_count;
		*(WORD *)&_pack_data[_pack_data_count] = (WORD)(tmp | (_tree_find.matching_size - _break_even - 1));
		_pack_data_count += 2;
	}

	_pack_bit_count++;

	return true;
}

// ****************************************************************
// ＬＺＳＳ圧縮 データをバッファにコピーする
// ================================================================
bool Clzss_pack::_copy_pack_buf(bool last_flag)
{
	dst_buf_need_size = 0;

	if (!last_flag && _pack_bit_count!=8)	{	return false;	}

	DWORD copy_size = _pack_buf_size + _pack_data_count;
	if (_dst_buf_size < copy_size)	{
		dst_buf_need_size = copy_size - _dst_buf_size;
		return false;
	}

	CopyMemory(_dst_buf + _pack_buf_size, _pack_data, _pack_data_count);
	_pack_buf_size += _pack_data_count;
	_pack_bit_count = 0;
	_pack_data_count = 1;		// １バイト目はBitFlag
	_pack_data[0]    = 0;

	return true;
}


