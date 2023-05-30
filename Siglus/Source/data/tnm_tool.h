#pragma		once

// ****************************************************************
// •Ö—˜ŠÖ”
// ================================================================
class C_tnm_tool
{
public:

	template <typename TIME_TYPE, typename VALUE_TYPE>
	static VALUE_TYPE time_table(TIME_TYPE now_time, TIME_TYPE start_time, VALUE_TYPE start_value, TIME_TYPE end_time, VALUE_TYPE end_value)
	{
		if (now_time >= end_time)			return end_value;
		else if (now_time <= start_time)	return start_value;
		else								return (end_value - start_value) * (VALUE_TYPE)(now_time - start_time) / (VALUE_TYPE)(end_time - start_time) + start_value;
	}
};

inline int speed_up_limit(int i_ct, int i_st, int i_sv, int i_et, int i_ev)
{
	if (i_st == i_et)
		return i_ev;

	double ct = (double)i_ct;
	double st = (double)i_st;
	double et = (double)i_et;
	double sv = (double)i_sv;
	double ev = (double)i_ev;

	if (st - et < 0)	ct = limit(st, ct, et);
	if (st - et > 0)	ct = limit(et, ct, st);

	return (int)((ct - st) * (ct - st) * (ev - sv) / (et - st) / (et - st) + sv);
}

inline int speed_down_limit(int i_ct, int i_st, int i_sv, int i_et, int i_ev)
{
	if (i_st == i_et)
		return i_ev;

	double ct = (double)i_ct;
	double st = (double)i_st;
	double et = (double)i_et;
	double sv = (double)i_sv;
	double ev = (double)i_ev;

	if (st - et < 0)	ct = limit(st, ct, et);
	if (st - et > 0)	ct = limit(et, ct, st);

	return (int)(- (ct - et) * (ct - et) * (ev - sv) / (et - st) / (et - st) + ev);
}


