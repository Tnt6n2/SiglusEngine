#pragma		once

// ****************************************************************
// 配列
// ================================================================
//		pod:	memcpy で保存できるデータです。
//		base:	C_tnm_save_stream::save(data) で保存できるデータです。文字列もこれです。
//		class:	class::save(save) で保存できるデータです。
// ================================================================
template <typename data_type, class allocator_type = std::allocator<data_type> >
class C_tnm_pod_array : public ARRAY<data_type, allocator_type>
{
public:
	// 分かりやすくするため、型を定義しておきます。
	typedef ARRAY<data_type, allocator_type>			base_type;		// 基底の型
	typedef C_tnm_pod_array<data_type, allocator_type>	this_type;		// 自身の型

	// コンストラクタ群
	C_tnm_pod_array() : ARRAY()	{}
	C_tnm_pod_array(base_type _Base) : ARRAY(_Base) 	{}
	explicit C_tnm_pod_array(size_type _Count) : ARRAY(_Count)	{}
	C_tnm_pod_array(size_type _Count, const data_type& _Val) : ARRAY(_Count, _Val)	{}
	C_tnm_pod_array(const this_type& _Right) : ARRAY(_Right)	{}

	// イテレータからのコンストラクタ
	template<class _Iter> C_tnm_pod_array(_Iter _First, _Iter _Last) : ARRAY(_First, _Last)	{}
};

template <typename data_type, class allocator_type = std::allocator<data_type> >
class C_tnm_base_array : public ARRAY<data_type, allocator_type>
{
};

template <typename data_type, class allocator_type = std::allocator<data_type> >
class C_tnm_class_array : public ARRAY<data_type, allocator_type>
{
public:
	// コンストラクタ群
	C_tnm_class_array() : ARRAY()	{}

	// イテレータからのコンストラクタ
	template<class _Iter> C_tnm_class_array(_Iter _First, _Iter _Last) : ARRAY(_First, _Last)	{}
};

