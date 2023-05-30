#include	"pch.h"
#include	"nwa_unpack.h"

// ****************************************************************
// ＮＷＡ解凍 コンストラクタ
// ================================================================
Cnwa_unpack::Cnwa_unpack()
{
	_init();
}

// ****************************************************************
// ＮＷＡ解凍 デストラクタ
// ================================================================
Cnwa_unpack::~Cnwa_unpack()
{
	close();
}

// ****************************************************************
// ＮＷＡ解凍 初期化
// ================================================================
void Cnwa_unpack::_init()
{
	_pm.fp = NULL;
	_pm.read_sample_pos = 0;
	_unpack.unit_no = -1;
}

// ****************************************************************
// ＮＷＡ解凍 ファイルオープン
// ================================================================
bool Cnwa_unpack::open(const TCHAR* file_path, DWORD offset)
{
	// クローズ
	close();

	// ファイルを開く
	_tfopen_s(&_pm.fp, file_path, _T("rb"));
	if (!_pm.fp)	{	return false;	}

	// ファイルオフセット
	_pm.offset = offset;
	if (_pm.offset)	{	_fseeki64(_pm.fp, _pm.offset, SEEK_SET);	}

	// ＮＷＡヘッダ読み込み
	DWORD size = (DWORD)fread(&_info.header, 1, sizeof(NWA_HEADER_STRUCT), _pm.fp);
	if (size != sizeof(NWA_HEADER_STRUCT))	{	close();	return false;	}

	// ユニットオフセットテーブル読み込み
	if (_info.header.pack_mod == -1)	{	// 無圧縮モード
		_info.unit_ofs_tbl.clear();
	}
	else								{	// 圧縮モード
		_info.unit_ofs_tbl.resize(_info.header.unit_cnt);
		size = (DWORD)fread(&_info.unit_ofs_tbl[0], 1, _info.header.unit_cnt * 4, _pm.fp);
		if (size != _info.header.unit_cnt * 4)	{	close();	return false;	}
	}

	// １サンプルのバイト数
	_info.one_sample_byte_size = _info.header.bits_per_sample / 8;

	return true;
}

// ****************************************************************
// ＮＷＡ解凍 ファイルクローズ
// ================================================================
void Cnwa_unpack::close()
{
	if (_pm.fp)	{	fclose(_pm.fp);	}
	_info.unit_ofs_tbl.clear();
	_unpack.buf.clear();
	_init();
}

// ****************************************************************
// ＮＷＡ解凍 読み込み位置を設定（サンプル値：ステレオは左右で１サンプルです）
// ================================================================
void Cnwa_unpack::set_read_sample_pos(DWORD sample_pos)
{
	// 補足：ＮＷＡはチャンネルごとにサンプルを数えているので、チャンネル数を掛ける
	_pm.read_sample_pos = sample_pos * _info.header.channels;
}

// ****************************************************************
// ＮＷＡ解凍 読み込み位置を取得（サンプル値：ステレオは左右で１サンプルです）
// ================================================================
DWORD Cnwa_unpack::get_read_sample_pos()
{
	// 補足：ＮＷＡはチャンネルごとにサンプルを数えているので、チャンネル数を掛ける
	if (_info.header.channels <= 0)	{	return 0;	}
	return _pm.read_sample_pos / _info.header.channels;
}

// ****************************************************************
// ＮＷＡ解凍 ユニットを読み込んで解凍する
// ================================================================
bool Cnwa_unpack::_read_unit(DWORD unit_no)
{
	// すでに解凍されている
	if (_unpack.unit_no == unit_no)	{	return true;	}

	// ファイルの読み込み位置
	_fseeki64(_pm.fp, _pm.offset + _info.unit_ofs_tbl[unit_no], SEEK_SET);

	// ファイルの読み込みサイズ
	DWORD read_size;
	if (unit_no == (_info.header.unit_cnt - 1))	{
		read_size = _info.header.last_sample_pack_size;
		_unpack.unit_sample_cnt = _info.header.last_sample_cnt;
	}
	else	{
		read_size = _info.unit_ofs_tbl[unit_no + 1] - _info.unit_ofs_tbl[unit_no];
		_unpack.unit_sample_cnt = _info.header.unit_sample_cnt;
	}

	// 読み込みバッファ確保
	std::vector<BYTE> read_buf;
	read_buf.resize(read_size);

	// 読み込み
	DWORD size = (DWORD)fread(&read_buf[0], 1, read_size, _pm.fp);
	if (size != read_size)	{	return false;	}

	// 解凍
	_unpack.buf.resize(_unpack.unit_sample_cnt * (_info.header.bits_per_sample / 8));
	nwa_unpack_unit(&read_buf[0], _unpack.unit_sample_cnt, 0, _unpack.unit_sample_cnt, &_unpack.buf[0], &_info.header);

	_unpack.unit_no = unit_no;
	return true;
}

// ****************************************************************
// ＮＷＡ解凍 無圧縮データの読み込み
// ================================================================
bool Cnwa_unpack::_read_no_pack_data(DWORD need_byte_size, BYTE* dp, DWORD* copy_byte_size)
{
	*copy_byte_size = 0;

//	DWORD alpha = _info.header.channels * _info.one_sample_byte_size;
	DWORD alpha = _info.one_sample_byte_size;

	// ファイルの読み込み位置
	DWORD data_ofs = _pm.read_sample_pos * alpha;
	DWORD seek_ofs = sizeof(NWA_HEADER_STRUCT) + data_ofs;
	_fseeki64(_pm.fp, _pm.offset + seek_ofs, SEEK_SET);

	// データのサンプル値を超えて読んじゃ駄目
	int nokori_byte_size = (_info.header.sample_cnt * _info.one_sample_byte_size) - data_ofs;
	if (nokori_byte_size <= 0)	{	return false;	}
	if (need_byte_size > (DWORD)nokori_byte_size)	{	need_byte_size = nokori_byte_size;	}

	// 読み込み
	DWORD size = (DWORD)fread(dp, 1, need_byte_size, _pm.fp);
	if (size == 0)	{	return false;	}

	// 読み取れたバイト数をサンプル補正する
	*copy_byte_size = (size / alpha) * alpha;

	return true;
}

// ****************************************************************
// ＮＷＡ解凍 読み込み（サンプル数：ステレオは左右で１サンプルです）
// ================================================================
DWORD Cnwa_unpack::read_sample(BYTE* buf, DWORD sample_cnt)
{
	// ヘッダ情報がおかしい場合は、ここで戻る
	if (_info.header.channels != 1 && _info.header.channels != 2)					{	return 0;	}
	if (_info.header.bits_per_sample != 8 && _info.header.bits_per_sample != 16)	{	return 0;	}

	// 補足：ＮＷＡはチャンネルごとにサンプルを数えているので、チャンネル数も掛ける
	DWORD	need_byte_size = sample_cnt * _info.header.channels * _info.one_sample_byte_size;

	BYTE*	dp = buf;
	while (1)	{
		if (need_byte_size <= 0)							{	break;	}	// 必要なだけ読み込めました。
		if (_pm.read_sample_pos >= _info.header.sample_cnt)	{	break;	}	// それ以上のサンプルはありません。

		DWORD	copy_byte_size = 0;

		if (_info.header.pack_mod == -1)	{	// 無圧縮モード
			if(!Cnwa_unpack::_read_no_pack_data(need_byte_size, dp, &copy_byte_size))	{	break;	}		// エラー
		}
		else								{	// 圧縮モード
			// ユニットを読み込んで解凍する
			if (!_read_unit(_pm.read_sample_pos / _info.header.unit_sample_cnt))		{	break;	}		// エラー

			// コピー
			DWORD	ofs  = (_pm.read_sample_pos % _info.header.unit_sample_cnt) * _info.one_sample_byte_size;
			copy_byte_size = (_unpack.unit_sample_cnt * _info.one_sample_byte_size) - ofs;
			if (copy_byte_size > need_byte_size)	{	copy_byte_size = need_byte_size;	}
			CopyMemory(dp, &_unpack.buf[0] + ofs, copy_byte_size);
		}

		if (copy_byte_size == 0)	{	break;	}		// データが解凍できていない、もしくは読み取れていない

		dp += copy_byte_size;
		need_byte_size -= copy_byte_size;

		// 読み込みサンプル位置を進める
		_pm.read_sample_pos += (copy_byte_size / _info.one_sample_byte_size);
	}

	return (sample_cnt - (need_byte_size / _info.header.channels / _info.one_sample_byte_size));
}

// ****************************************************************
// ＮＷＡ解凍 情報取得 チャンネル数取得
// ================================================================
WORD Cnwa_unpack::get_info_channels()
{
	return _info.header.channels;
}

// ****************************************************************
// ＮＷＡ解凍 情報取得 ビット値取得
// ================================================================
WORD Cnwa_unpack::get_info_bits_per_sample()
{
	return _info.header.bits_per_sample;
}

// ****************************************************************
// ＮＷＡ解凍 情報取得 周波数取得
// ================================================================
DWORD Cnwa_unpack::get_info_samples_per_sec()
{
	return _info.header.samples_per_sec;
}

// ****************************************************************
// ＮＷＡ解凍 情報取得 総サンプル数取得（サンプル数：ステレオは左右で１サンプルです）
// ================================================================
DWORD Cnwa_unpack::get_info_sample_cnt()
{
	if (_info.header.channels <= 0)	{	return 0;	}
	return _info.header.sample_cnt / _info.header.channels;
}

// ****************************************************************
// ＮＷＡ解凍 情報取得 オリジナルサイズ取得
// ================================================================
DWORD Cnwa_unpack::get_info_original_size()
{
	return _info.header.original_size;
}

// ****************************************************************
// ＮＷＡ解凍 情報取得 圧縮サイズ取得
// ================================================================
DWORD Cnwa_unpack::get_info_pack_size()
{
	return _info.header.pack_size;
}

// ****************************************************************
// ＮＷＡ解凍 情報取得 圧縮モード取得（-1=無圧縮  0=低品質  1=中品質  2=高品質  3=より高品質  4=よりより高品質  5=よりよりより高品質）
// ================================================================
int Cnwa_unpack::get_info_pack_mod()
{
	return _info.header.pack_mod;
}

// ****************************************************************
// ＮＷＡ解凍 情報取得 ＺＥＲＯ圧縮モード取得（0=何もしない  1=無音部分を個数圧縮する）
// ================================================================
int Cnwa_unpack::get_info_zero_mod()
{
	return _info.header.zero_mod;
}



