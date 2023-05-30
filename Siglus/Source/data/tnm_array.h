#pragma		once

// ****************************************************************
// �z��
// ================================================================
//		pod:	memcpy �ŕۑ��ł���f�[�^�ł��B
//		base:	C_tnm_save_stream::save(data) �ŕۑ��ł���f�[�^�ł��B�����������ł��B
//		class:	class::save(save) �ŕۑ��ł���f�[�^�ł��B
// ================================================================
template <typename data_type, class allocator_type = std::allocator<data_type> >
class C_tnm_pod_array : public ARRAY<data_type, allocator_type>
{
public:
	// ������₷�����邽�߁A�^���`���Ă����܂��B
	typedef ARRAY<data_type, allocator_type>			base_type;		// ���̌^
	typedef C_tnm_pod_array<data_type, allocator_type>	this_type;		// ���g�̌^

	// �R���X�g���N�^�Q
	C_tnm_pod_array() : ARRAY()	{}
	C_tnm_pod_array(base_type _Base) : ARRAY(_Base) 	{}
	explicit C_tnm_pod_array(size_type _Count) : ARRAY(_Count)	{}
	C_tnm_pod_array(size_type _Count, const data_type& _Val) : ARRAY(_Count, _Val)	{}
	C_tnm_pod_array(const this_type& _Right) : ARRAY(_Right)	{}

	// �C�e���[�^����̃R���X�g���N�^
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
	// �R���X�g���N�^�Q
	C_tnm_class_array() : ARRAY()	{}

	// �C�e���[�^����̃R���X�g���N�^
	template<class _Iter> C_tnm_class_array(_Iter _First, _Iter _Last) : ARRAY(_First, _Last)	{}
};

