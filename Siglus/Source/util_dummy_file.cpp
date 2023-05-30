#include	"pch.h"

#include	"utility/util_dummy_file.h"

// ****************************************************************
// ダミーファイルチェック
//		version: 1=バージョン１、2=バージョン２以上
// ================================================================
bool C_dummy_file_checker::check_dummy_file(int version, CTSTR& dummy_dir, CTSTR& dummy_file_name, int key, CTSTR& code)
{
	// パラメータを準備
	TSTR dummy_file_path[16];
	int code_len = 8, dummy_file_cnt = 0;
	MBSTR mb_code = TSTR_to_MBSTR(code);
	BYTE script_code[256] = {0};	// スクリプトに埋め込まれていたコード
	BYTE file_code[256] = {0};		// ファイルに埋め込まれていたコード

	// ダミーファイルを探す。ファイルは最大16個
	for (int i = 0; i < 16; i ++)	{
		dummy_file_path[i] = dummy_dir + _T("\\") + dummy_file_name + str_format(_T(".%03d"), i + 1);
		if (file_get_type(dummy_file_path[i]) == 1)
			dummy_file_cnt ++;
		else
			break;
	}

	// ダミーファイルを１つ以上見つけた場合
	if (dummy_file_cnt > 0)	{

		// 全てのダミーファイルのコードを結合する
		for (int dummy_file_no = 0; dummy_file_no < dummy_file_cnt; dummy_file_no++)	{

			// ファイルを開く
			C_file file;
			if (!file.open(dummy_file_path[dummy_file_no], _T("rb")))
				return false;

			// バージョン２以上の場合はバージョンを読み込む
			if (version >= 2)
				file.read(&version, sizeof(int));

			// バージョンによってコードの長さは違う
			if (version >= 2)
				code_len = 16;

			// コードの埋め込み位置を取得
			int code_pos = 0;
			file.read(&code_pos, sizeof(int));
			BYTE* p = (BYTE *)&code_pos;
			for (int i = 0; i < sizeof(int); i++)
				*p++ ^= key;			// １バイトずつ復号

			// コードを取得
			file.seek(code_pos, SEEK_SET);
			for (int i = dummy_file_no; i < code_len; i += dummy_file_cnt)	{
				file.read(&file_code[i], 1);
				file_code[i] ^= key;	// １バイトずつ復号
			}
		}

		// スクリプトで指定されたコードを取得
		memcpy(script_code, mb_code.c_str(), mb_code.size());

		// バージョン２以上の場合はスクリプトコードをMD5変換
		if (version >= 2)	{
			MD5_STRUCT md5st;
			MD5init(&md5st, code_len);
			MD5funk(&md5st, script_code, code_len);
			memcpy(script_code, md5st.code, 16);	// MD5変換で出来上がったコードは16文字
			code_len = 16;							// コードの長さを変換後のコードに補正
		}

		// コードチェック
		if (memcmp(script_code, file_code, code_len) == 0)
			return true;
	}

	return false;
}



