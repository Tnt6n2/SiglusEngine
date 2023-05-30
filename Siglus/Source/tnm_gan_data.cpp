#include	"pch.h"
#include	"data/tnm_gan_data.h"

// ****************************************************************
// 読み込み
// ================================================================
bool C_gan_data::load(CTSTR& file_path)
{
	ARRAY<BYTE> buf;
	if (!C_file::read_full_data(file_path, buf))	{	return false;	}
	return analize(buf);
}

// ****************************************************************
// 解析
// ================================================================

#define		GANCODE_VERSION							10000
#define		GANCODE_G00NAME							10100

#define		GANCODE_SET_COUNT						20000

#define		GANCODE_PAT_COUNT						30000

#define		GANCODE_PAT_NO							30100
#define		GANCODE_PAT_X							30101
#define		GANCODE_PAT_Y							30102
#define		GANCODE_PAT_WAIT						30103
#define		GANCODE_PAT_CLR							30104
#define		GANCODE_PAT_Z							30105

#define		GANCODE_PAT_END							999999

#define		GANVERSION_10000						10000

bool C_gan_data::analize(ARRAY<BYTE>& buf)
{
	if (buf.size() < 8)	{	return false;	}
	if (!analize_func(buf.get()))	{
		set_list.clear();	// 失敗したのでクリアする
		return false;
	}
	return true;
}

// 解析ファンクション
bool C_gan_data::analize_func(BYTE* sp)
{
	// バージョン確認
	if (*(int *)sp != GANCODE_VERSION)	{	return false;	}
	sp += 4;
	int version = *(int *)sp;
	if (version != GANVERSION_10000)	{	return false;	}
	sp += 4;

	int code;
	while (1)	{
		code = *(int *)sp;
		sp += 4;

		// G00 ファイル名
		if (code == GANCODE_G00NAME)	{
			int len = *(int *)sp;
			sp += 4;
			g00_file_name = MBSTR_to_TSTR(sp, len);
			sp += len;
		}
		// セット解析
		else if (code == GANCODE_SET_COUNT)	{
			int set_cnt = *(int *)sp;
			sp += 4;
			if(set_cnt > 0){
				for(int i = 0; i < set_cnt; i++)	{
					S_gan_set_data set;
					set.total_time = 0;
					if (!analize_func_set(sp, &sp, &set, &set.total_time))	{	return false;	}
					set_list.push_back(set);
				}
			}
			return true;		// ここで戻ればＯＫ
		}
		// 未定義のコード
		else	{
			return false;
		}
	}
}

// セット解析
bool C_gan_data::analize_func_set(BYTE* sp, BYTE** ret_sp, S_gan_set_data* set, int* keika_time)
{
	if (*(int *)sp != GANCODE_PAT_COUNT)	{	return false;	}
	sp += 4;
	int pat_cnt = *(int *)sp;
	sp += 4;

	for(int i = 0; i < pat_cnt; i++)	{
		S_gan_pat_data pat;
		if (!analize_func_pat(sp, &sp, &pat, keika_time))	{	return false;	}
		set->pat_list.push_back(pat);
	}
	*ret_sp = sp;
	return true;
}

// パターン解析
bool C_gan_data::analize_func_pat(BYTE* sp, BYTE** ret_sp, S_gan_pat_data* pat, int* keika_time)
{
	int code;
	while (1)	{
		code = *(int *)sp;
		sp += 4;
		if (code == GANCODE_PAT_END)	{	break;	}
		switch (code)	{
			case GANCODE_PAT_NO:	pat->pat_no = *(int *)sp;	sp += 4;	break;
			case GANCODE_PAT_X:		pat->x = *(int *)sp;		sp += 4;	break;
			case GANCODE_PAT_Y:		pat->y = *(int *)sp;		sp += 4;	break;
			case GANCODE_PAT_WAIT:	pat->wait = *(int *)sp;		sp += 4;	break;
			case GANCODE_PAT_CLR:	pat->tr = *(int *)sp;		sp += 4;	break;
			case GANCODE_PAT_Z:		pat->z = *(int *)sp;		sp += 4;	break;
			default:				return false;							break;		// 未定義のコード
		}
	}
	*keika_time += pat->wait;
	pat->keika_time = *keika_time;
	*ret_sp = sp;
	return true;
}

