#pragma		once

#include	"element.h"
#include	"CA.h"

// ****************************************************************
// �����̓A�g���^�C�v
// ================================================================
enum TNMS_LA_ATOM_TYPE
{
	#define		DEFINE(str)		str,
	#include	"def_la_atom.h"
	#undef		DEFINE
};

const TSTR TNMS_ATOM_TYPE_STR[] =
{
	#define		DEFINE(str)		_T(#str),
	#include	"def_la_atom.h"
	#undef		DEFINE
};

// ****************************************************************
// �����̓A�g��
// ================================================================
struct S_tnms_atom
{
	int					id;			// �h�c�i�ʂ��ԍ��j
	int					line;		// �s
	TNMS_LA_ATOM_TYPE	type;		// �A�g���^�C�v
	int					opt;		// �I�v�V����
	int					subopt;		// �I�v�V����

	S_tnms_atom()
	{
		id     = 0;
		line   = 0;
		type   = TNMS_LA_ATOM_NONE;
		opt    = 0;
		subopt = 0;
	}
};

// ****************************************************************
// ���x���\����
// ================================================================
struct S_tnms_la_label
{
	TSTR	name;
	int		line;

	S_tnms_la_label(CTSTR& name_, int line_)
	{
		name = name_;
		line = line_;
	}

	bool	operator == (const S_tnms_la_label& rhs)
	{
		return name == rhs.name;
	}
};


// ****************************************************************
// �����͍\����
// ================================================================
struct S_tnms_la_data
{
	ARRAY<S_tnms_atom>		atom_list;		// �A�g�����X�g
	ARRAY<TSTR>				str_list;		// �����񃊃X�g
	ARRAY<S_tnms_la_label>	label_list;		// ���x�����X�g
	ARRAY<TSTR>				unknown_list;	// ����`�ꃊ�X�g
};

// ****************************************************************
// ������ - Lexcial Analizer -
// ================================================================
class C_tnms_lexcial_analizer
{
public:
	bool	analize(S_tnms_ca_data* pcad, S_tnms_la_data* plad);

	int		get_error_line()	{	return m_error_line;	}
	TSTR	get_error_str()		{	return m_error_str;		}

	static	TSTR	get_atom_text(S_tnms_atom atom, S_tnms_la_data* plad);

private:
	bool	skip_separator(TSTR::iterator& p, TSTR::iterator end);
	bool	get_label(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);
	bool	get_integer(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);
	bool	get_single_quoted_string(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);
	bool	get_double_quoted_string(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);
	bool	get_zenkaku_string(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);
	bool	get_word(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);

	int				m_cur_atom_id;	// ���݂̂h�c
	int				m_cur_line;		// ���݂̍s
	S_tnms_ca_data	m_ca_data;		// CA �f�[�^
	S_tnms_la_data	m_la_data;		// LA �f�[�^

	bool			error(int line, CTSTR& str);	// �G���[
	int				m_error_line;					// �G���[�s
	TSTR			m_error_str;					// �G���[���e
};
