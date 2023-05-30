#pragma		once

// ****************************************************************
// �^�C�� ID�i���� TonaLib �ɓ���悤���ȁc�H�j
// ================================================================

// �������琶���ł��A���\�[�g�\�ȍ\����
// SYSTEMTIME ���Ɨj�������܂��Ă��邽�߃\�[�g�Ɍ����Ȃ�
// �֗��̂��� SYSTEMTIME ���琶���ł���悤�ɂ��Ƃ��܂��B

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
// �^�C�}�[
// ================================================================
class C_tnm_timer
{
public:
	int		real_time;				// �������i��ɓ����j
	int		real_time_past;			// �������i��ɓ����j
	__int64	global_real_time;		// ���v���C����
	int		local_real_time;		// ���[�J���������i�O����n�܂�A��ɓ����A�Z�[�u�����j
	int		local_real_time_past;	// ���[�J���������i�O����n�܂�A��ɓ����A�Z�[�u�����j
	int		local_real_time_amari;	// ���[�J���������i�O����n�܂�A��ɓ����A�Z�[�u�����j
	int		local_game_time;		// ���[�J���Q�[�������i������������A�Z�[�u�����j
	int		local_game_time_past;	// ���[�J���Q�[�������i������������A�Z�[�u�����j
	int		local_game_time_amari;	// ���[�J���Q�[�������i������������A�Z�[�u�����j
	int		local_wipe_time;		// ���[�J�����C�v�����i������������A�Z�[�u�����j
	int		local_wipe_time_past;	// ���[�J�����C�v�����i������������A�Z�[�u�����j
	int		local_wipe_time_amari;	// ���[�J�����C�v�����i������������A�Z�[�u�����j
};



