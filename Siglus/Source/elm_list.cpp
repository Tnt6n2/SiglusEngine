#include	"pch.h"

#include	"tnm_form_code.h"
#include	"element/elm_list.h"
#include	"data/tnm_save_00.h"

// ****************************************************************
// 整数リスト：初期化
// ================================================================
void C_elm_int_list::init(S_element element, CTSTR& name, int size, bool extend_enable)
{
	// 基底クラスの初期化
	C_elm_list<int>::init(element, FM_INTLIST, name, size, extend_enable, NULL);
}

// ****************************************************************
// 整数リスト：ビットを設定
// ================================================================
void C_elm_int_list::set_bit_1(int index, int value)
{
	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T(".b1[") + tostr(index) + _T("] を参照しました。"));
		return;
	}

	int* dst = &m_sub[index / 32];	// ターゲットのフラグ
	DWORD mask = 0x00000001 << index;		// ターゲットのビットのみ 1 のマスクを作る
	value  = (value << index) & mask;		// 値をターゲットの位置に移動してマスクをかける
	*dst = (*dst & ~mask) | value;			// ターゲットを 0 クリアしてから値を代入
}

void C_elm_int_list::set_bit_2(int index, int value)
{
	index *= 2;		// 2倍しておくと処理が簡単

	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T(".b2[") + tostr(index / 2) + _T("] を参照しました。"));
		return;
	}

	int* dst = &m_sub[index / 32];	// ターゲット
	DWORD mask = 0x00000003 << index;		// ターゲットのビットのみ 1 のマスクを作る
	value  = (value << index) & mask;		// 値をターゲットの位置に移動してマスクをかける
	*dst = (*dst & ~mask) | value;			// ターゲットを 0 クリアしてから値を代入
}

void C_elm_int_list::set_bit_4(int index, int value)
{
	index *= 4;		// 4倍しておくと処理が簡単

	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T(".b4[") + tostr(index / 4) + _T("] を参照しました。"));
		return;
	}

	int* dst = &m_sub[index / 32];	// ターゲットのフラグ
	DWORD mask = 0x0000000F << index;		// ターゲットのビットのみ 1 のマスクを作る
	value  = (value << index) & mask;		// 値をターゲットの位置に移動してマスクをかける
	*dst = (*dst & ~mask) | value;			// ターゲットを 0 クリアしてから値を代入
}

void C_elm_int_list::set_bit_8(int index, int value)
{
	index *= 8;		// 8倍しておくと処理が簡単

	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T(".b8[") + tostr(index / 8) + _T("] を参照しました。"));
		return;
	}

	int* dst = &m_sub[index / 32];	// ターゲット
	DWORD mask = 0x000000FF << index;		// ターゲットのビットのみ 1 のマスクを作る
	value  = (value << index) & mask;		// 値をターゲットの位置に移動してマスクをかける
	*dst = (*dst & ~mask) | value;			// ターゲットを 0 クリアしてから値を代入
}

void C_elm_int_list::set_bit_16(int index, int value)
{
	index *= 16;		// 16倍しておくと処理が簡単

	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T(".b16[") + tostr(index / 16) + _T("] を参照しました。"));
		return;
	}

	int* dst = &m_sub[index / 32];	// ターゲット
	DWORD mask = 0x0000FFFF << index;		// ターゲットのビットのみ 1 のマスクを作る
	value  = (value << index) & mask;		// 値をターゲットの位置に移動してマスクをかける
	*dst = (*dst & ~mask) | value;			// ターゲットを 0 クリアしてから値を代入
}

// ****************************************************************
// 整数リスト：ビットを取得
// ================================================================
int C_elm_int_list::get_bit_1(int index)
{
	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T(".b1[") + tostr(index) + _T("] を参照しました。"));
		return 0;
	}

	int value = m_sub[index / 32];	// ターゲットのフラグ
	return (value >> index) & 0x00000001;	// ターゲットのビットを返す
}

int C_elm_int_list::get_bit_2(int index)
{
	index *= 2;		// 2倍しておくと処理が簡単

	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T(".b2[") + tostr(index / 2) + _T("] を参照しました。"));
		return 0;
	}

	int value = m_sub[index / 32];	// ターゲットのフラグ
	return (value >> index) & 0x00000003;	// ターゲットのビットを返す
}

int C_elm_int_list::get_bit_4(int index)
{
	index *= 4;		// 4倍しておくと処理が簡単

	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T(".b4[") + tostr(index / 4) + _T("] を参照しました。"));
		return 0;
	}

	int value = m_sub[index / 32];	// ターゲットのフラグ
	return (value >> index) & 0x0000000F;	// ターゲットのビットを返す
}

int C_elm_int_list::get_bit_8(int index)
{
	index *= 8;		// 8倍しておくと処理が簡単

	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T(".b8[") + tostr(index / 8) + _T("] を参照しました。"));
		return 0;
	}

	int value = m_sub[index / 32];	// ターゲットのフラグ
	return (value >> index) & 0x000000FF;	// ターゲットのビットを返す
}

int C_elm_int_list::get_bit_16(int index)
{
	index *= 16;	// 16倍しておくと処理が簡単

	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T(".b16[") + tostr(index / 16) + _T("] を参照しました。"));
		return 0;
	}

	int value = m_sub[index / 32];	// ターゲットのフラグ
	return (value >> index) & 0x0000FFFF;	// ターゲットのビットを返す
}

// ****************************************************************
// 整数リスト：サブの初期化
// ================================================================
void C_elm_int_list::_init(int begin, int end)
{
	ZeroMemory(m_sub.get() + begin, (end - begin) * sizeof(int));
}

// ****************************************************************
// 整数リスト：サブの再初期化
// ================================================================
void C_elm_int_list::_reinit(int begin, int end)
{
	ZeroMemory(m_sub.get() + begin, (end - begin) * sizeof(int));
}

// ****************************************************************
// 整数リスト：サブのセーブ
// ================================================================
void C_elm_int_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	stream.save(m_sub.get() + begin, (end - begin) * sizeof(int));
}

// ****************************************************************
// 整数リスト：サブのロード
// ================================================================
void C_elm_int_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	stream.load(m_sub.get() + begin, (end - begin) * sizeof(int));
}

// ****************************************************************
// 文字列リスト：初期化
// ================================================================
void C_elm_str_list::init(S_element element, CTSTR& name, int size, bool extend_enable)
{
	// 基底クラスの初期化
	C_elm_list<TSTR>::init(element, FM_STRLIST, name, size, extend_enable, NULL);
}

// ****************************************************************
// 文字列リスト：サブの初期化
// ================================================================
void C_elm_str_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].clear();
}

// ****************************************************************
// 文字列リスト：サブの再初期化
// ================================================================
void C_elm_str_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].clear();
}

// ****************************************************************
// 文字列リスト：サブのセーブ
// ================================================================
void C_elm_str_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		stream.save(m_sub[i]);
}

// ****************************************************************
// 文字列リスト：サブのロード
// ================================================================
void C_elm_str_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		stream.load(m_sub[i]);
}

