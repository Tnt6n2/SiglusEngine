#pragma		once

#include	"data/tnm_element.h"
#include	"engine/ifc_error.h"

class	C_tnm_save_stream;

// ****************************************************************
// �ėp���X�g�i�x�[�X�j
// ================================================================
//		���ڃC���X�^���X����邱�Ƃ͂ł��܂���B
//		�p������ C_elm_list �� C_elm_list_ex ���g���ĉ������B
// ================================================================
template <typename TYPE>
class C_elm_list_base : public C_tnm_element
{
protected:
	C_elm_list_base() : m_def_extend_enable(true), m_def_size(0)	{}

public:
	// �ʏ푀��
	int		get_size() const 	{	return (int)m_sub.size();	}	// �T�C�Y�擾
	bool	is_empty() const	{	return m_sub.size() == 0;	}	// �󔻒�

	// �p�����[�^���擾
	ARRAY<TYPE>&		get_sub()			{	return m_sub;				}
	const ARRAY<TYPE>&	get_sub() const		{	return m_sub;				}
	TYPE*				get_ptr()			{	return m_sub.get();			}
	const TYPE*			get_ptr() const		{	return m_sub.get();			}
	TYPE*				get_end_ptr()		{	return m_sub.get_end();		}
	const TYPE*			get_end_ptr() const	{	return m_sub.get_end();		}

	TYPE*				get_sub(int index, bool disp_error);									// �͈̓`�F�b�N�����
	TYPE&				operator [] (int index)				{	return m_sub[index];		}	// �͈̓`�F�b�N�Ȃ���
	const TYPE&			operator [] (int index) const		{	return m_sub[index];		}	// �͈̓`�F�b�N�Ȃ���

protected:
	ARRAY<TYPE>		m_sub;
	int				m_def_size;
	bool			m_def_extend_enable;
};

// ****************************************************************
// �ėp���X�g�i�ʏ�j
// ================================================================
//		�ʏ�͂�����g���ĉ������B
// ================================================================
template <typename TYPE>
class C_elm_list : public C_elm_list_base<TYPE>
{
public:
	C_elm_list()	{}

	// �G�������g��{����
	void	init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent);	// ������
	void	reinit();								// �ď�����
	void	resize(int size);						// �T�C�Y�ύX
	void	clear();								// �N���A
	void	save(C_tnm_save_stream& stream);		// �Z�[�u
	void	load(C_tnm_save_stream& stream);		// ���[�h

protected:

	// �ȉ��̉��z�֐��́AC_elm_list ���p������N���X���I�[�o�[���C�h����K�v������܂��B
	virtual	void	_init(int begin, int end) = 0;
	virtual	void	_reinit(int begin, int end) = 0;
	virtual	void	_save(C_tnm_save_stream& stream, int begin, int end) = 0;
	virtual	void	_load(C_tnm_save_stream& stream, int begin, int end) = 0;
};

// ****************************************************************
// �ėp���X�g�i�g���j
// ================================================================
//		�č\�z��j��Ȃǂ̏������ׂ������䂷��ꍇ�͂�������g���ĉ������B
//		�I�u�W�F�N�g�A�X�e�[�W�A�t���[���A�N�V�����ȂǁB
// ================================================================
template <typename TYPE>
class C_elm_list_ex : public C_elm_list_base<TYPE>
{
public:
	C_elm_list_ex()	{}

	// �G�������g��{����
	void	init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent);	// ������
	void	reinit(bool restruct_flag);				// �ď�����
	void	resize(int size);						// �T�C�Y�ύX
	void	clear();								// �N���A
	void	save(C_tnm_save_stream& stream);		// �Z�[�u
	void	load(C_tnm_save_stream& stream);		// ���[�h

protected:

	// �ȉ��̉��z�֐��́AC_elm_list ���p������N���X���I�[�o�[���C�h����K�v������܂��B
	virtual	void	_init(int begin, int end) = 0;
	virtual	void	_reinit(int begin, int end, bool restruct_flag) = 0;
	virtual	void	_save(C_tnm_save_stream& stream, int begin, int end) = 0;
	virtual	void	_load(C_tnm_save_stream& stream, int begin, int end) = 0;
};

// ****************************************************************
// �ėp���X�g�i�x�[�X�j�F�T�u���擾
// ================================================================
template <typename TYPE>
inline TYPE* C_elm_list_base<TYPE>::get_sub(int index, bool disp_error)
{
	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index)	{
		if (disp_error)
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T("[") + tostr(index) + _T("] ���Q�Ƃ��܂����B"));

		return NULL;
	}

	return &m_sub[index];
}

// ****************************************************************
// �ėp���X�g�i�ʏ�j�F������
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, form, name, p_parent);

	// �Œ�p�����[�^�̐ݒ�
	m_def_size = size;						// �����T�C�Y
	m_def_extend_enable = extend_enable;	// �g���\�t���O

	// �����T�C�Y
	m_sub.resize(size);

	// �T�u�̏�����
	_init(0, size);
}

// ****************************************************************
// �ėp���X�g�i�ʏ�j�F�ď�����
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::reinit()
{
	// �T�C�Y�ς̏ꍇ
	if (m_def_extend_enable)	{
		int new_cnt = m_def_size;
		int old_cnt = (int)m_sub.size();
		m_sub.resize(new_cnt);

		// �ێ����ꂽ�T�u�͍ď�����
		// �������T�u�͏�����
		if (new_cnt > old_cnt)	{
			_reinit(0, old_cnt);
			_init(old_cnt, new_cnt);
		}
		else	{
			_reinit(0, new_cnt);
		}
	}

	// �T�C�Y�Œ�̏ꍇ
	else	{

		// �T�u�̏�����
		_reinit(0, m_def_size);
	}
}

// ****************************************************************
// �ėp���X�g�i�ʏ�j�F�N���A
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::clear()
{
	// �T�C�Y�ς̏ꍇ
	if (m_def_extend_enable)	{
		m_sub.clear();
	}

	// �T�C�Y�Œ�̏ꍇ
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, get_element_name() + _T(" �̓T�C�Y��ύX�ł��܂���B"));
	}
}

// ****************************************************************
// �ėp���X�g�i�ʏ�j�F�T�C�Y�ύX
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::resize(int new_cnt)
{
	// �T�C�Y�ς̏ꍇ
	if (m_def_extend_enable)	{
		int old_cnt = (int)m_sub.size();

		// ����ύX
		m_sub.resize(new_cnt);

		// �������T�u�͏�����
		if (new_cnt > old_cnt)	{
			_init(old_cnt, new_cnt);
		}
	}

	// �T�C�Y�Œ�̏ꍇ
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, get_element_name() + _T(" �̓T�C�Y��ύX�ł��܂���B"));
	}
}

// ****************************************************************
// �ėp���X�g�i�ʏ�j�F�Z�[�u
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::save(C_tnm_save_stream& stream)
{
	int jump;

	// �T�C�Y�ς̏ꍇ
	if (m_def_extend_enable)	{
		stream.save(m_sub.size());
		_save(stream, 0, (int)m_sub.size());
	}

	// �T�C�Y�Œ�̏ꍇ
	else	{
		stream.save_fixed_array_begin(m_sub, &jump);
		_save(stream, 0, (int)m_sub.size());
		stream.save_fixed_array_end(&jump);
	}
}

// ****************************************************************
// �ėp���X�g�i�ʏ�j�F���[�h
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::load(C_tnm_save_stream& stream)
{
	int cnt, jump;

	// �T�C�Y�ς̏ꍇ
	if (m_def_extend_enable)	{
		cnt = stream.load_ret<int>();
		resize(cnt);
		_load(stream, 0, cnt);
	}

	// �T�C�Y�Œ�̏ꍇ
	else	{
		cnt = stream.load_fixed_array_begin(m_sub, get_element_name(), &jump);
		_load(stream, 0, cnt);
		stream.load_fixed_array_end(&jump);
	}
}

// ****************************************************************
// �ėp���X�g�i�g���j�F������
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, form, name, p_parent);

	// �Œ�p�����[�^�̐ݒ�
	m_def_size = size;						// �����T�C�Y
	m_def_extend_enable = extend_enable;	// �g���\�t���O

	// �����T�C�Y
	m_sub.resize(size);

	// �T�u�̏�����
	_init(0, size);
}

// ****************************************************************
// �ėp���X�g�i�g���j�F�ď�����
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::reinit(bool restruct_flag)
{
	// �T�C�Y�ς̏ꍇ
	if (m_def_extend_enable)	{
		int new_cnt = m_def_size;
		int old_cnt = (int)m_sub.size();
		m_sub.resize(new_cnt);

		// �ێ����ꂽ�T�u�͍ď�����
		// �������T�u�͏�����
		if (new_cnt > old_cnt)	{
			_reinit(0, old_cnt, restruct_flag);
			_init(old_cnt, new_cnt);
		}
		else	{
			_reinit(0, new_cnt, restruct_flag);
		}
	}

	// �T�C�Y�Œ�̏ꍇ
	else	{

		// �T�u�̏�����
		_reinit(0, m_def_size, restruct_flag);
	}
}

// ****************************************************************
// �ėp���X�g�i�g���j�F�N���A
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::clear()
{
	// �T�C�Y�ς̏ꍇ
	if (m_def_extend_enable)	{
		m_sub.clear();
	}

	// �T�C�Y�Œ�̏ꍇ
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, get_element_name() + _T(" �̓T�C�Y��ύX�ł��܂���B"));
	}
}

// ****************************************************************
// �ėp���X�g�i�g���j�F�T�C�Y�ύX
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::resize(int new_cnt)
{
	// �T�C�Y�ς̏ꍇ
	if (m_def_extend_enable)	{
		int old_cnt = (int)m_sub.size();

		// ����ύX
		m_sub.resize(new_cnt);

		// �������T�u�͏�����
		if (new_cnt > old_cnt)	{
			_init(old_cnt, new_cnt);
		}
	}

	// �T�C�Y�Œ�̏ꍇ
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, get_element_name() + _T(" �̓T�C�Y��ύX�ł��܂���B"));
	}
}

// ****************************************************************
// �ėp���X�g�i�g���j�F�Z�[�u
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::save(C_tnm_save_stream& stream)
{
	int jump;

	// �T�C�Y�ς̏ꍇ
	if (m_def_extend_enable)	{
		stream.save(m_sub.size());
		_save(stream, 0, (int)m_sub.size());
	}

	// �T�C�Y�Œ�̏ꍇ
	else	{
		stream.save_fixed_array_begin(m_sub, &jump);
		_save(stream, 0, (int)m_sub.size());
		stream.save_fixed_array_end(&jump);
	}
}

// ****************************************************************
// �ėp���X�g�i�g���j�F���[�h
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::load(C_tnm_save_stream& stream)
{
	int cnt, jump;

	// �T�C�Y�ς̏ꍇ
	if (m_def_extend_enable)	{
		cnt = stream.load_ret<int>();
		resize(cnt);
		_load(stream, 0, cnt);
	}

	// �T�C�Y�Œ�̏ꍇ
	else	{
		cnt = stream.load_fixed_array_begin(m_sub, get_element_name(), &jump);
		_load(stream, 0, cnt);
		stream.load_fixed_array_end(&jump);
	}
}

// ****************************************************************
// �������X�g
// ================================================================
class C_elm_int_list : public C_elm_list<int>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable);
	void	set_value(int index, int value);					// �l��ݒ肷��
	int		get_value(int index);								// �l���擾����

	// �r�b�g����
	void	set_bit_1(int index, int value);
	void	set_bit_2(int index, int value);
	void	set_bit_4(int index, int value);
	void	set_bit_8(int index, int value);
	void	set_bit_16(int index, int value);
	int		get_bit_1(int index);
	int		get_bit_2(int index);
	int		get_bit_4(int index);
	int		get_bit_8(int index);
	int		get_bit_16(int index);

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

// ****************************************************************
// �������X�g�F�l��ݒ�
// ================================================================
inline void C_elm_int_list::set_value(int index, int value)
{
	int* p = get_sub(index, true);
	if (p)
		*p = value;
}

// ****************************************************************
// �������X�g�F�l���擾
// ================================================================
inline int C_elm_int_list::get_value(int index)
{
	int* p = get_sub(index, true);
	if (p)
		return *p;
	else
		return 0;
}

// ****************************************************************
// �����񃊃X�g
// ================================================================
class C_elm_str_list : public C_elm_list<TSTR>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable);
	void	set_value(int index, CTSTR& value);					// �l��ݒ肷��
	TSTR	get_value(int index);								// �l���擾����

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

// ****************************************************************
// �����񃊃X�g�F�l��ݒ�
// ================================================================
inline void C_elm_str_list::set_value(int index, CTSTR& value)
{
	TSTR* p = get_sub(index, true);
	if (p)
		*p = value;
}

// ****************************************************************
// �����񃊃X�g�F�l���擾
// ================================================================
inline TSTR C_elm_str_list::get_value(int index)
{
	TSTR* p = get_sub(index, true);
	if (p)
		return *p;
	else
		return _T("");
}
