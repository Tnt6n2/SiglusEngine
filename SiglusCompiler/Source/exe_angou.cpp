#include	"pch.h"

#include	"exe_angou.h"

// ****************************************************************
// 暗号の挿入位置を探す
// ================================================================

BYTE exe_angou_find_code[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_CODE_SIZE] =
{
	TNM_EXE_ANGOU_FIND_CODE00,
	TNM_EXE_ANGOU_FIND_CODE01,
	TNM_EXE_ANGOU_FIND_CODE02,
	TNM_EXE_ANGOU_FIND_CODE03,
	TNM_EXE_ANGOU_FIND_CODE04,
	TNM_EXE_ANGOU_FIND_CODE05,
	TNM_EXE_ANGOU_FIND_CODE06,
	TNM_EXE_ANGOU_FIND_CODE07,
};

const int TNM_EXE_ANGOU_ERRCODE_OK						= 0;		// ＯＫ
const int TNM_EXE_ANGOU_ERRCODE_CANCEL					= 1;		// キャンセル
const int TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_NOT_FIND		= 2;		// 暗号を挿入する場所が見つかりません
const int TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_FIND_OVER	= 3;		// 暗号を挿入する場所が見つかり過ぎです
const int TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_SHORT			= 4;		// 暗号の文字列が短過ぎます
const int TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_LONG			= 5;		// 暗号の文字列が長過ぎます
const int TNM_EXE_ANGOU_ERRCODE_FILENONE				= 6;		// ＥＸＥファイルが存在しません
const int TNM_EXE_ANGOU_ERRCODE_FILEOPEN				= 7;		// ＥＸＥファイルが書き込みモードで開けませんでした
const int TNM_EXE_ANGOU_ERRCODE_FILEREAD				= 8;		// ＥＸＥファイルが読み込めませんでした
const int TNM_EXE_ANGOU_ERRCODE_FILESIZE				= 9;		// ＥＸＥファイルのサイズがありません
const int TNM_EXE_ANGOU_ERRCODE_FILESAVE				= 10;		// ＥＸＥファイルが保存できませんでした

// ＦＵＮＣ
int C_tnms_exe_angou::find_angou_insert_address_func(BYTE *exe_p, int exe_size, int *ret_find_code_count, BYTE *ap[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_MAX_CNT])
{
	int ap_cnt[TNM_EXE_ANGOU_FIND_CODE_CNT];
//	int find_code_count = (exe_size >= (1024 * 1024 * 9)) ? 2 : 1;		// ９Ｍバイト以上はデバッグ版のＥＸＥと認識（デバッグ版には２箇所ある）
	int find_code_count = 1;											// デバッグ版のＥＸＥも１箇所しか見つからない…なぜだろう？
	for (int i = 0; i < TNM_EXE_ANGOU_FIND_CODE_CNT; i++)	{
		for (int j = 0; j < TNM_EXE_ANGOU_FIND_MAX_CNT; j++)	{			// TNM_EXE_ANGOU_FIND_MAX_CNT(10)個分のバッファを用意しているが２つ目でエラー（デバッグ版は３つ目でエラー）
			ap[i][j] = 0;
		}
		ap_cnt[i] = find_angou_insert_address_funcfunc(exe_p, exe_size, find_code_count, exe_angou_find_code[i], ap[i]);
		if (ap_cnt[i] <= 0)	{
			return TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_NOT_FIND;		// 暗号を挿入する場所が見つかりません
		}
		else if (ap_cnt[i] != find_code_count)	{
			return TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_FIND_OVER;		// 暗号を挿入する場所が見つかり過ぎです
		}
	}
	if (ret_find_code_count)	{	*ret_find_code_count = find_code_count;	}
	return TNM_EXE_ANGOU_ERRCODE_OK;
}

// ＦＵＮＣＦＵＮＣ
int C_tnms_exe_angou::find_angou_insert_address_funcfunc(BYTE *exe_p, int exe_size, int find_code_count, BYTE *find_code, BYTE **ap)
{
	int end_size = exe_size - TNM_EXE_ANGOU_FIND_CODE_SIZE;
	BYTE *wp = exe_p;
	int ap_cnt = 0;
	while (1)	{
		if (*wp == *find_code)	{
			BYTE *sp = find_code;
			BYTE *dp = wp;
			int i;
			for (i = 0; i < TNM_EXE_ANGOU_FIND_CODE_SIZE; i++)	{
				if (*sp != *dp)	{	break;	}	sp++;	dp++;
			}
			// コードが見つかった
			if (i == TNM_EXE_ANGOU_FIND_CODE_SIZE)	{
				*ap++ = wp;		// オフセットを記憶
				ap_cnt++;
				if (ap_cnt > find_code_count)	{	break;	}		// １０個分のバッファを用意しているが２つ目でエラー（デバッグ版は３つ目）
			}
		}
		wp++;
		end_size--;
		if (end_size < 0)	{	break;	}	// 検索完了
	}
	return ap_cnt;
}

// ****************************************************************
// ＥＸＥが暗号化されているか判定する
// ================================================================
bool C_tnms_exe_angou::check(CTSTR& exe_path)
{
	// 読み込み
	BUFFER exe_data;
	if (!C_file::read_full_data(exe_path, exe_data))	{
		return false;
	}

	// 暗号の挿入位置を探す
	BYTE *ap[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_MAX_CNT];		// 暗号挿入位置のオフセットを取得するバッファ
	int err_code = find_angou_insert_address_func(exe_data.get(), (int)exe_data.size(), NULL, ap);
	if (err_code != TNM_EXE_ANGOU_ERRCODE_OK)	{
		return false;
	}

	return true;
}

// ****************************************************************
// ＥＸＥを暗号化する
// ================================================================

// 本体
bool C_tnms_exe_angou::angou(HWND h_wnd, CTSTR& exe_path, CTSTR& angou_str)
{
	int err_code = angou_func(h_wnd, exe_path, angou_str);
	err_code_msgbox(h_wnd, exe_path, angou_str, err_code);
	return (err_code == TNM_EXE_ANGOU_ERRCODE_OK) ? true : false;
}

// ＦＵＮＣ
int C_tnms_exe_angou::angou_func(HWND h_wnd, CTSTR& exe_path, CTSTR& angou_str)
{
	MBSTR mb_angou_str = TSTR_to_MBSTR(angou_str);
	int angou_str_size = (int)mb_angou_str.size();
	if (angou_str_size < TNM_EXE_ANGOU_STR_MIN_LEN)			{	return TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_SHORT;	}	// 暗号の文字列が短過ぎます
//	else if (angou_str_size > TNM_EXE_ANGOU_STR_MAX_LEN)	{	return TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_LONG;	}	// 暗号の文字列が長過ぎます

	if (file_get_type(exe_path) != 1)	{	return TNM_EXE_ANGOU_ERRCODE_FILENONE;	}	// ＥＸＥファイルが存在しません

	// 書き込みモードで開けるかテストする
	C_file file;
	if (!file.open(exe_path, _T("rb")))	{	return TNM_EXE_ANGOU_ERRCODE_FILEOPEN;	}	// ＥＸＥファイルが書き込みモードで開けませんでした
	file.close();

	// 読み込み
	BUFFER exe_data;
	if (!C_file::read_full_data(exe_path, exe_data))	{	return TNM_EXE_ANGOU_ERRCODE_FILEREAD;	}		// ＥＸＥファイルが読み込めませんでした
	if (exe_data.size() <= 0)	{	return TNM_EXE_ANGOU_ERRCODE_FILESIZE;	}	// ＥＸＥファイルのサイズがありません

	// 暗号の挿入位置を探す
	int find_code_count;
	BYTE *ap[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_MAX_CNT];		// 暗号挿入位置のアドレスを取得するバッファ
	int err_code = find_angou_insert_address_func(exe_data.get(), (int)exe_data.size(), &find_code_count, ap);
	if (err_code != TNM_EXE_ANGOU_ERRCODE_OK)	{	return err_code;	}

	// 最終確認
	TSTR mess =
		_T("暗号化のエラーチェックを完了しました。\n\n")
		_T("ＥＸＥを暗号化できます。\n\n")
		_T("実行してもよろしいですか？\n\n")
		_T("　") + exe_path + _T("　　\n");
	int res = MessageBox(h_wnd, mess.c_str(), _T("確認"), MB_OKCANCEL | MB_ICONINFORMATION);
	if (res == IDCANCEL)	{	return TNM_EXE_ANGOU_ERRCODE_CANCEL;	}

	// 暗号要素作成
	BYTE angou_element[TNM_EXE_ANGOU_ELEMENT_CNT];
	make_angou_element((BYTE *)mb_angou_str.c_str(), (int)mb_angou_str.size(), angou_element);

	// 少し乱数を混ぜてゼロクリアする
	BYTE insert_code[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_CODE_SIZE];
	for (int i = 0; i < TNM_EXE_ANGOU_FIND_CODE_CNT; i++)	{
		for (int j = 0; j < TNM_EXE_ANGOU_FIND_CODE_SIZE; j++)	{
			insert_code[i][j] = ((rand() % 5) != 0) ? 0 : (rand() % 0xff);
		}
	}

	// 暗号Ａ
	insert_code[0][TNM_EXE_ANGOU_OFFSET00A] = angou_element[TNM_EXE_ANGOU_ELEMENT00A];
	insert_code[1][TNM_EXE_ANGOU_OFFSET01A] = angou_element[TNM_EXE_ANGOU_ELEMENT01A];
	insert_code[2][TNM_EXE_ANGOU_OFFSET02A] = angou_element[TNM_EXE_ANGOU_ELEMENT02A];
	insert_code[3][TNM_EXE_ANGOU_OFFSET03A] = angou_element[TNM_EXE_ANGOU_ELEMENT03A];
	insert_code[4][TNM_EXE_ANGOU_OFFSET04A] = angou_element[TNM_EXE_ANGOU_ELEMENT04A];
	insert_code[5][TNM_EXE_ANGOU_OFFSET05A] = angou_element[TNM_EXE_ANGOU_ELEMENT05A];
	insert_code[6][TNM_EXE_ANGOU_OFFSET06A] = angou_element[TNM_EXE_ANGOU_ELEMENT06A];
	insert_code[7][TNM_EXE_ANGOU_OFFSET07A] = angou_element[TNM_EXE_ANGOU_ELEMENT07A];

	// 暗号Ｂ
	insert_code[0][TNM_EXE_ANGOU_OFFSET00B] = angou_element[TNM_EXE_ANGOU_ELEMENT00B];
	insert_code[1][TNM_EXE_ANGOU_OFFSET01B] = angou_element[TNM_EXE_ANGOU_ELEMENT01B];
	insert_code[2][TNM_EXE_ANGOU_OFFSET02B] = angou_element[TNM_EXE_ANGOU_ELEMENT02B];
	insert_code[3][TNM_EXE_ANGOU_OFFSET03B] = angou_element[TNM_EXE_ANGOU_ELEMENT03B];
	insert_code[4][TNM_EXE_ANGOU_OFFSET04B] = angou_element[TNM_EXE_ANGOU_ELEMENT04B];
	insert_code[5][TNM_EXE_ANGOU_OFFSET05B] = angou_element[TNM_EXE_ANGOU_ELEMENT05B];
	insert_code[6][TNM_EXE_ANGOU_OFFSET06B] = angou_element[TNM_EXE_ANGOU_ELEMENT06B];
	insert_code[7][TNM_EXE_ANGOU_OFFSET07B] = angou_element[TNM_EXE_ANGOU_ELEMENT07B];

	// 暗号挿入
	for (int i = 0; i < TNM_EXE_ANGOU_FIND_CODE_CNT; i++)	{
		for (int j = 0; j < find_code_count; j++)	{
			CopyMemory(ap[i][j], insert_code[i], TNM_EXE_ANGOU_FIND_CODE_SIZE);
		}
	}

	// 保存
	if (!C_file::write_full_data(exe_path, exe_data))	{
		err_code = TNM_EXE_ANGOU_ERRCODE_FILESAVE;		// ＥＸＥファイルが保存できませんでした
	}

	return TNM_EXE_ANGOU_ERRCODE_OK;
}

// ****************************************************************
// エラーコードメッセージボックス
// ================================================================
void C_tnms_exe_angou::err_code_msgbox(HWND h_wnd, CTSTR& exe_path, CTSTR& angou_str, int err_code)
{
	if (err_code == TNM_EXE_ANGOU_ERRCODE_OK)	{
		MessageBox(h_wnd, _T("ＥＸＥを暗号化しました。\n"), _T("確認"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (err_code == TNM_EXE_ANGOU_ERRCODE_CANCEL)	{
		MessageBox(h_wnd, _T("キャンセルしました。\n"), _T("確認"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	// その他エラー
	TSTR mess;
	bool add_angou_str_err = false;
	bool add_exe_path_err = false;
	bool add_exe_lock_err = false;
	switch (err_code)	{
		case TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_NOT_FIND:		// 暗号を挿入する場所が見つかりません
			mess =
				_T("暗号を挿入する場所が見つかりません。\n\n")
				_T("★このＥＸＥは既に暗号化されている可能性があります。\n\n")
				_T("★１度暗号化されたＥＸＥを再暗号化する事はできません。\n\n")
				_T("　暗号を元に戻したい場合 や 違う暗号にしたい場合は、\n")
				_T("　暗号化されていない元のＥＸＥを持ってきてコピーして下さい。\n");
			break;
		case TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_FIND_OVER:	// 暗号を挿入する場所が見つかり過ぎです
			mess =
				_T("暗号を挿入する場所が見つかり過ぎです。\n\n")
				_T("　未知のエラーが発生しました。\n\n")
				_T("　※お手数ですが、ビジュアルアーツのプログラム担当にお問い合わせ下さい。\n");
			break;
		case TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_SHORT:			// 暗号の文字列が短過ぎます
			mess = _T("暗号の文字列が短過ぎます。\n\n");
			add_angou_str_err = true;
			break;
		case TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_LONG:			// 暗号の文字列が長過ぎます
			mess = _T("暗号の文字列が長過ぎます。\n\n");
			add_angou_str_err = true;
			break;
		case TNM_EXE_ANGOU_ERRCODE_FILENONE:				// ＥＸＥファイルが存在しません
			mess = _T("ＥＸＥファイルの読み込みに失敗しました。\n\n");
			if (exe_path.size() == 0)	{
				mess += _T("「実行ファイル」 にＥＸＥを指定して下さい。\n");
			}
			else	{
				add_exe_path_err = true;
			}
			break;
		case TNM_EXE_ANGOU_ERRCODE_FILEOPEN:				// ＥＸＥファイルが書き込みモードで開けませんでした
			mess = _T("ＥＸＥファイルが書き込みモードで開けませんでした。\n\n");
			add_exe_lock_err = true;
			break;
		case TNM_EXE_ANGOU_ERRCODE_FILEREAD:				// ＥＸＥファイルが読み込めませんでした
			mess = _T("ＥＸＥファイルが読み込めませんでした。\n\n");
			add_exe_lock_err = true;
			break;
		case TNM_EXE_ANGOU_ERRCODE_FILESIZE:				// ＥＸＥファイルのサイズがありません
			mess = _T("ＥＸＥファイルのサイズがありません。\n\n");
			add_exe_path_err = true;
			break;
		case TNM_EXE_ANGOU_ERRCODE_FILESAVE:				// ＥＸＥファイルが保存できませんでした
			mess = _T("ＥＸＥファイルが保存できませんでした。\n\n");
			add_exe_lock_err = true;
			break;
	}

	if (add_angou_str_err)	{
		mess +=
			_T("　≪暗号の文字列≫\n\n")
			_T("　　") + angou_str + _T("\n\n")
			_T("　半角文字なら８文字以上６４文字以下を入力して下さい。\n\n")
			_T("　※全角文字は半角文字の２文字分です。\n");
	}

	if (add_exe_path_err)	{
		mess +=
			_T("「実行ファイル」 に指定されているＥＸＥのパスを確認して下さい。\n\n")
			_T("　≪ＥＸＥのパス≫\n\n")
			_T("　　") + exe_path + _T("\n");
	}

	if (add_exe_lock_err)	{
		mess +=
			_T("■ＥＸＥが起動している場合は、終了して下さい。\n\n")
			_T("　終了している場合は、バックタスクで動いていないかタスクマネージャーで確認して下さい。\n\n")
			_T("　もしくは Windows を再起動して下さい。\n\n")
			_T("■読み取り専用の属性になっていませんか？\n\n")
			_T("　読み取り専用の属性を外して下さい。\n\n")
			_T("■ＥＸＥファイルが他のプロセスによってロックされている可能性があります。\n\n")
			_T("　ＥＸＥファイルをロックしている可能性があるツールを終了して下さい。\n\n")
			_T("　もしくは Windows を再起動して下さい。\n");
	}

	MessageBox(h_wnd, mess.c_str(), _T("エラー"), MB_OK | MB_ICONWARNING);
}

// ****************************************************************
// 暗号要素作成
// ================================================================

BYTE exe_angou_original_element[TNM_EXE_ANGOU_ELEMENT_CNT] = {
	0x36, 0x59, 0xc9, 0x73, 0x2e, 0xb5, 0x09, 0xba, 0xe4, 0x4c, 0xf2, 0x6a, 0xa2, 0x34, 0xec, 0x7c,
};

// バイト文字列から作成
void C_tnms_exe_angou::make_angou_element(BYTE *angou_str, int angou_str_size, BYTE *ret_angou_element)
{
	CopyMemory(ret_angou_element, exe_angou_original_element, TNM_EXE_ANGOU_ELEMENT_CNT);

	if (angou_str_size <= 0)	{	return;	}

	int cnt = (angou_str_size < TNM_EXE_ANGOU_ELEMENT_CNT) ? TNM_EXE_ANGOU_ELEMENT_CNT : angou_str_size;

	int aaa = 0;
	int bbb = 0;
	for (int i = 0; i < cnt; i++)	{
		*(ret_angou_element + bbb) ^= *(angou_str + aaa);
		aaa++;	if (aaa == angou_str_size)			{	aaa = 0;	}
		bbb++;	if (bbb == TNM_EXE_ANGOU_ELEMENT_CNT)	{	bbb = 0;	}
	}
}

// TSTR型文字列から作成
void C_tnms_exe_angou::make_angou_element(CTSTR& angou_str, BYTE *ret_angou_element)
{
	MBSTR mb_angou_str = TSTR_to_MBSTR(angou_str);
	make_angou_element((BYTE *)mb_angou_str.c_str(), (int)mb_angou_str.size(), ret_angou_element);
}
