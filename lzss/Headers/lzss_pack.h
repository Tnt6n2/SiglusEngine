#pragma		once

// ****************************************************************
// ＬＺＳＳ圧縮
// ================================================================

#define		LZSS_BUF_REALLOC_SIZE		(1024*256)
#define		LZSS_PROG_CHECK_COUNT		(1024 * 10)

class Clzss_pack
{

public:
	void		ready(BYTE* src_data_, DWORD src_data_cnt_, DWORD break_out_cnt_);	// 準備
	bool		proc(BYTE* dst_buf_, DWORD dst_buf_size_);		// 圧縮プロセス

	DWORD		src_complate_cnt;					// 処理完了数
	DWORD		dst_buf_need_size;					// 足りないバッファ数
	DWORD		pack_size;							// 圧縮サイズ

private:

	bool		_proc_copy();						// プロセス 圧縮データをバッファにコピーする
	bool		_proc_last_copy();					// プロセス 最後の圧縮データをバッファにコピーする
	bool		_proc_end();						// プロセス 終了処理
	bool		_make_pack_data();					// データ化
	bool		_copy_pack_buf(bool last_flag);		// データをバッファにコピーする

	// プロセスステップ
	enum PROC_STEP{
		__PACK,
		__COPY,
		__LAST_COPY,
		__END,
		__COMPLATE,
	};
	PROC_STEP	_proc_step;					// プロセスステップ

	DWORD		_break_out_cnt;				// ループ抜け処理数
	DWORD		_break_out_last;			// 前回のループ抜け値

	BYTE*		_src_data;					// ソースデータ
	DWORD		_src_data_cnt;				// ソースデータサイズ

	BYTE*		_dst_buf;					// 出力先バッファ
	DWORD		_dst_buf_size;				// 出力先バッファサイズ

	DWORD		_sizeof_data_size;
	DWORD		_window_size;
	DWORD		_length_bit_count;
	DWORD		_break_even;
	DWORD		_look_ahead_size;

	DWORD		_pack_buf_size;				// 圧縮バッファサイズ
	DWORD		_pack_bit_count;			// 圧縮ビットカウント

	DWORD		_pack_data_count;			// 圧縮データカウント
	BYTE		_pack_data[1 + (2 * 8)];	// 圧縮データ：最大 = flag(１ビット × ８) + coad(２バイト × ８)

	Clzss_tree_find<BYTE>	_tree_find;		// ツリー検索

	DWORD		_replace_cnt;				// 入れ替え数（処理用変数）

};

