#pragma		once

// ****************************************************************
// タイム ID（これ TonaLib に入れようかな…？）
// ================================================================

// 時刻から生成でき、かつソート可能な構造体
// SYSTEMTIME だと曜日が挟まっているためソートに向かない
// 便利のため SYSTEMTIME から生成できるようにしときます。

struct S_tid
{
	WORD	year;
	WORD	month;
	WORD	day;
	WORD	hour;
	WORD	minute;
	WORD	second;
	WORD	mill_second;

	S_tid()
	{
		year = 0;
		month = 0;
		day = 0;
		hour = 0;
		minute = 0;
		second = 0;
		mill_second = 0;
	}

	S_tid& operator = (SYSTEMTIME sys_time)
	{
		year = sys_time.wYear;
		month = sys_time.wMonth;
		day = sys_time.wDay;
		hour = sys_time.wHour;
		minute = sys_time.wMinute;
		second = sys_time.wSecond;
		mill_second = sys_time.wMilliseconds;

		return *this;
	}
};

inline bool operator == (const S_tid& lhs, const S_tid& rhs)
{
	if (lhs.year != rhs.year)					return false;
	if (lhs.month != rhs.month)					return false;
	if (lhs.day != rhs.day)						return false;
	if (lhs.hour != rhs.hour)					return false;
	if (lhs.minute != rhs.minute)				return false;
	if (lhs.second != rhs.second)				return false;
	if (lhs.mill_second != rhs.mill_second)		return false;

	return true;
}

inline bool operator != (const S_tid& lhs, const S_tid& rhs)
{
	if (lhs.year != rhs.year)					return true;
	if (lhs.month != rhs.month)					return true;
	if (lhs.day != rhs.day)						return true;
	if (lhs.hour != rhs.hour)					return true;
	if (lhs.minute != rhs.minute)				return true;
	if (lhs.second != rhs.second)				return true;
	if (lhs.mill_second != rhs.mill_second)		return true;

	return false;
}

inline bool operator < (const S_tid& lhs, const S_tid& rhs)
{
	if (lhs.year < rhs.year)		return true;
	if (lhs.year > rhs.year)		return false;
	if (lhs.month < rhs.month)		return true;
	if (lhs.month > rhs.month)		return false;
	if (lhs.day < rhs.day)			return true;
	if (lhs.day > rhs.day)			return false;
	if (lhs.hour < rhs.hour)		return true;
	if (lhs.hour > rhs.hour)		return false;
	if (lhs.minute < rhs.minute)	return true;
	if (lhs.minute > rhs.minute)	return false;
	if (lhs.second < rhs.second)	return true;
	if (lhs.second > rhs.second)	return false;
	if (lhs.mill_second < rhs.mill_second)		return true;
	if (lhs.mill_second > rhs.mill_second)		return false;

	return false;
}

// ****************************************************************
// タイマー
// ================================================================
class C_tnm_timer
{
public:
	int		real_time;				// 実時刻（常に等速）
	int		real_time_past;			// 実時刻（常に等速）
	__int64	global_real_time;		// 総プレイ時間
	int		local_real_time;		// ローカル実時刻（０から始まる、常に等速、セーブされる）
	int		local_real_time_past;	// ローカル実時刻（０から始まる、常に等速、セーブされる）
	int		local_real_time_amari;	// ローカル実時刻（０から始まる、常に等速、セーブされる）
	int		local_game_time;		// ローカルゲーム時刻（加速減速する、セーブされる）
	int		local_game_time_past;	// ローカルゲーム時刻（加速減速する、セーブされる）
	int		local_game_time_amari;	// ローカルゲーム時刻（加速減速する、セーブされる）
	int		local_wipe_time;		// ローカルワイプ時刻（加速減速する、セーブされる）
	int		local_wipe_time_past;	// ローカルワイプ時刻（加速減速する、セーブされる）
	int		local_wipe_time_amari;	// ローカルワイプ時刻（加速減速する、セーブされる）
};



