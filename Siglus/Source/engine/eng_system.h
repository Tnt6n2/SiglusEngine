#pragma		once

// ****************************************************************
// システム情報
// ================================================================
//		ＰＣの情報を表す構造体
// ================================================================

// ****************************************************************
// 解像度構造体
// ================================================================
struct S_tnm_resolution
{
	C_size	resolution;
};

// 解像度の比較演算子
inline bool operator == (S_tnm_resolution& lhs, S_tnm_resolution& rhs)
{
	return lhs.resolution.cx == rhs.resolution.cx && lhs.resolution.cy == rhs.resolution.cy;
}

// 解像度の比較演算子
inline bool operator < (S_tnm_resolution& lhs, S_tnm_resolution& rhs)
{
	if (lhs.resolution.cx < rhs.resolution.cx)	return true;
	if (lhs.resolution.cx > rhs.resolution.cx)	return false;
	if (lhs.resolution.cy < rhs.resolution.cy)	return true;
	if (lhs.resolution.cy > rhs.resolution.cy)	return false;
	return false;
}

// 解像度リスト
struct S_tnm_resolution_list
{
	ARRAY<S_tnm_resolution>	resolution_list;
	int						default_resolution_no;	// デフォルトの解像度
};

// ****************************************************************
// システム情報の構造体
// ================================================================
class C_tnm_system_info
{
public:
	ARRAY<D3DADAPTER_IDENTIFIER9>	display_list;
	ARRAY<S_tnm_resolution_list>	resolution_table;
};

// ****************************************************************
// ＰＣ関連の処理
// ================================================================
bool	tnm_get_os_name(TSTR& name);					// OSの名前を取得する
TSTR	tnm_get_cpu_name();								// CPUの名前を取得する

// ****************************************************************
// デバイス関係の処理
// ================================================================
bool	tnm_check_device();								// デバイスをチェックする
TCHAR	tnm_search_disc_drive();						// ディスクドライブを検索する

