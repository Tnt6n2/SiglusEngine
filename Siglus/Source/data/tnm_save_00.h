#pragma		once

#include	"tnm_array.h"

// ****************************************************************
// セーブストリーム
// ================================================================
class C_tnm_save_stream : private C_stream
{
public:

	using	C_stream::empty;
	using	C_stream::size;
	using	C_stream::clear;
	using	C_stream::resize;
	using	C_stream::set_seek;
	using	C_stream::begin;
	using	C_stream::end;
	using	C_stream::get;
	using	C_stream::get_end;
	using	C_stream::get_cur_ptr;
	using	C_stream::add_seek;
	using	C_stream::get_buffer;

	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	template <typename TYPE>	void save(TYPE data)					{	C_stream::push(data);				}
								void save(const void* ptr, int size)	{	C_stream::push(ptr, size);			}
								void save(TSTR str)						{	C_stream::push_str_len(str);		}

	template <typename TYPE>	void load(TYPE& data)					{	C_stream::pop(data);				}
								void load(void* ptr, int size)			{	C_stream::pop(ptr, size);			}
								void load(TSTR& str)					{	C_stream::pop_str_len(str);			}
	template <typename TYPE>	TYPE load_ret()							{	return C_stream::pop_ret<TYPE>();			}
	template <>					TSTR load_ret<TSTR>()					{	return C_stream::pop_str_len_ret<TSTR>();	}

	template <typename TYPE1, typename TYPE2>	void save(C_tnm_pod_array<TYPE1, TYPE2>& data);
	template <typename TYPE1, typename TYPE2>	void save(C_tnm_base_array<TYPE1, TYPE2>& data);
	template <typename TYPE1, typename TYPE2>	void save(C_tnm_class_array<TYPE1, TYPE2>& data);
	template <typename TYPE1, typename TYPE2>	void load(C_tnm_pod_array<TYPE1, TYPE2>& data);
	template <typename TYPE1, typename TYPE2>	void load(C_tnm_base_array<TYPE1, TYPE2>& data);
	template <typename TYPE1, typename TYPE2>	void load(C_tnm_class_array<TYPE1, TYPE2>& data);

	// 固定長の配列
	template <typename TYPE1>	inline void	save_fixed_array_begin(ARRAY<TYPE1>& data, int* jump);
								inline void	save_fixed_array_end(int* jump);
	template <typename TYPE1>	inline int	load_fixed_array_begin(ARRAY<TYPE1>& data, CTSTR& name, int* jump);
								inline void	load_fixed_array_end(int* jump);
								inline void	skip_fixed_array();
};









