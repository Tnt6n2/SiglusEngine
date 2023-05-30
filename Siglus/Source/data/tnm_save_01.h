#pragma		once

#include	"data/tnm_save_00.h"

// ****************************************************************
// セーブストリーム：セーブ：ＰＯＤ配列
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::save(C_tnm_pod_array<TYPE1, TYPE2>& data)
{
	// サイズを書き込む
	save<int>((int)data.size());

	// セーブする
	save((const void *)data.get(), (int)data.size() * sizeof(TYPE1));
}

// ****************************************************************
// セーブストリーム：セーブ：ベース配列
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::save(C_tnm_base_array<TYPE1, TYPE2>& data)
{
	// サイズを書き込む
	save<int>((int)data.size());

	// セーブする
	for (C_tnm_base_array<TYPE1, TYPE2>::iterator itr = data.begin(); itr != data.end(); ++itr)
		save(*itr);
}

// ****************************************************************
// セーブストリーム：セーブ：クラス配列
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::save(C_tnm_class_array<TYPE1, TYPE2>& data)
{
	// サイズを書き込む
	save<int>((int)data.size());

	// セーブする
	for (C_tnm_class_array<TYPE1, TYPE2>::iterator itr = data.begin(); itr != data.end(); ++itr)
		itr->save(*this);
}

// ****************************************************************
// セーブストリーム：ロード：ＰＯＤ配列
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::load(C_tnm_pod_array<TYPE1, TYPE2>& data)
{
	// サイズを読み込む
	int cnt = load_ret<int>();

	// サイズを変更する
	data.resize(cnt);

	// 全部読み込む
	pop((void *)data.get(), (int)data.size() * sizeof(TYPE1));
}

// ****************************************************************
// セーブストリーム：ロード：ベース配列
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::load(C_tnm_base_array<TYPE1, TYPE2>& data)
{
	// サイズを読み込む
	int cnt = load_ret<int>();

	// サイズを変更する
	data.resize(cnt);

	// 全部読み込む
	for (C_tnm_base_array<TYPE1, TYPE2>::iterator itr = data.begin(); itr != data.end(); ++itr)
		load(*itr);
}

// ****************************************************************
// セーブストリーム：ロード：クラス配列
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::load(C_tnm_class_array<TYPE1, TYPE2>& data)
{
	// サイズを読み込む
	int cnt = load_ret<int>();

	// サイズを変更する
	data.resize(cnt);

	// データを読み込む
	for (C_tnm_class_array<TYPE1, TYPE2>::iterator itr = data.begin(); itr != data.end(); ++itr)
		itr->load(*this);
}

// ****************************************************************
// セーブストリーム：セーブ：配列（サイズ固定）
// ================================================================
template <typename TYPE1> inline void C_tnm_save_stream::save_fixed_array_begin(ARRAY<TYPE1>& data, int* jump)
{
	// カットするためのジャンプ先を記憶する位置を予約しておく
	*jump = get_seek();
	save<int>(0);

	// サイズチェックを行うためにサイズを書き込む
	save<int>((int)data.size());
}

inline void C_tnm_save_stream::save_fixed_array_end(int* jump)
{
	// カットするためのジャンプ先を書き込む
	*(int *)get(*jump) = get_seek();
}

// ****************************************************************
// セーブストリーム：ロード：配列（サイズ固定）
// ================================================================
template <typename TYPE1> inline int C_tnm_save_stream::load_fixed_array_begin(ARRAY<TYPE1>& data, CTSTR& name, int* jump)
{
	// カットするためのジャンプ先を読み込む
	*jump = load_ret<int>();

	// サイズチェックを行うためのサイズを読み込む
	int cnt = load_ret<int>();

	// 正常
	if (cnt == (int)data.size())	{}

	// データ多すぎる！
	else if (cnt > (int)data.size())	{
		tnm_set_error(TNM_ERROR_TYPE_DEBUG, str_format(_T("前回のデータより %s が減っています。\n設定ファイルが書き換えられた可能性があります。\n%d 個のデータは読み込まずに切り捨てます。"), name.c_str(), cnt - data.size()));

		// 読み込むべきサイズを修正！
		cnt = (int)data.size();
	}

	// データが少なすぎる！
	else if (cnt < (int)data.size())	{
		tnm_set_error(TNM_ERROR_TYPE_DEBUG, str_format(_T("前回のデータより %s が増えています。\n設定ファイルが書き換えられた可能性があります。\n%d 個のデータを構築します。"), name.c_str(), data.size() - cnt));
	}

	// 読み込める個数を返す
	return cnt;
}

inline void C_tnm_save_stream::load_fixed_array_end(int* jump)
{
	// 残りデータをカットする
	set_seek(*jump);
}

// ****************************************************************
// セーブストリーム：スキップ：配列（サイズ固定）
// ================================================================
inline void C_tnm_save_stream::skip_fixed_array()
{
	// カットするためのジャンプ先を読み込む
	int jump = load_ret<int>();

	// データをカットする
	set_seek(jump);
}
