#pragma		once

#include	"code.h"
#include	"element.h"

// ****************************************************************
// inc �t�@�C�����
//		*.inc �t�@�C������͂���N���X��錾���܂��B
// ================================================================

// ****************************************************************
// �}�N���̈���
// ================================================================
struct S_tnms_ia_macro_arg
{
	TSTR	name;			// ���O
	TSTR	def_value;		// �f�t�H���g�l
};

// ****************************************************************
// �u���^�C�v
// ================================================================
enum TNMS_IA_REPLACE_TYPE
{
	TNMS_IA_REPLACE_TYPE_NONE,
	TNMS_IA_REPLACE_TYPE_REPLACE,		// replace
	TNMS_IA_REPLACE_TYPE_DEFINE,		// define
	TNMS_IA_REPLACE_TYPE_MACRO,			// macro
};

// ****************************************************************
// �u���\����
// ================================================================
struct S_tnms_ia_replace
{
	TNMS_IA_REPLACE_TYPE		type;		// �u���^�C�v
	TSTR						name;		// ���O
	TSTR						after;		// �u����̕�����
	ARRAY<S_tnms_ia_macro_arg>	arg_list;	// �������X�g�i�}�N���̂݁j
};

// ****************************************************************
// �u������ёւ��邽�߂̔�r�֐�
// ================================================================
//		�����݂̂Ŕ��肵�܂��B
// ================================================================
inline bool G_tnms_ia_replace_cmp_func(BSP<S_tnms_ia_replace>& lhs, BSP<S_tnms_ia_replace>& rhs)
{
	return lhs->name.length() < rhs->name.length();
}

// ****************************************************************
// �u���c���[�p�m�[�h
// ================================================================
struct S_replace_node
{
	void	clear()
	{
		child_map.clear();
		p_replace.reset();
	}

	std::map<TCHAR, S_replace_node>	child_map;
	BSP<S_tnms_ia_replace>			p_replace;
};

// ****************************************************************
// �u���c���[
// ================================================================
class C_replace_tree
{
public:
	static	void	create_replace_tree(S_replace_node& root, ARRAY< BSP<S_tnms_ia_replace> >& replace_list);
	static	void	add_replace(S_replace_node& node, BSP<S_tnms_ia_replace> p_replace, int index);
	static	void	search_replace(S_replace_node& node, TSTR::const_iterator p, BSP<S_tnms_ia_replace>& ret_replace);
};

// ****************************************************************
// �v���p�e�B�\����
// ================================================================
struct S_tnms_ia_property
{
	int				id;				// �v���p�e�BID
	int				form;			// �^
	int				size;			// �z��̏ꍇ�̃T�C�Y
	TSTR			name;			// ���O
};

// ****************************************************************
// �R�}���h�\����
// ================================================================
struct S_tnms_ia_command
{
	S_tnms_ia_command() : is_defined(false)	{}

	int				id;				// �R�}���hID�i�ʂ��ԍ��j
	int				form;			// �^
	TSTR			name;			// ���O
	S_tnms_arg_list	arg_list;		// �������X�g
	bool			is_defined;		// ��`���ꂽ���ǂ���
};

// ****************************************************************
// �錾�^�C�v
// ================================================================
enum TNMS_IA_DECLARE_TYPE
{
	TNMS_IA_DECLARE_TYPE_NONE,
	TNMS_IA_DECLARE_TYPE_REPLACE,
	TNMS_IA_DECLARE_TYPE_DEFINE,
	TNMS_IA_DECLARE_TYPE_DEFINE_SPACE,
	TNMS_IA_DECLARE_TYPE_MACRO,
	TNMS_IA_DECLARE_TYPE_PROPERTY,
	TNMS_IA_DECLARE_TYPE_COMMAND,
	TNMS_IA_DECLARE_TYPE_EXPAND,
};

// ****************************************************************
// �錾�\����
// ================================================================
struct S_tnms_ia_declare
{
	TNMS_IA_DECLARE_TYPE	type;
	BSP<S_tnms_ia_replace>	replace;	// �u���ireplace, define, macro�j
	BSP<S_tnms_ia_property>	Property;	// �v���p�e�B
	BSP<S_tnms_ia_command>	command;	// �R�}���h
};

// ****************************************************************
// inc �G���[�萔
// ================================================================
enum TNSMERR_IA
{
	TNMSERR_IA_NONE,
	TNMSERR_IA_NEWLINE_IN_SQUOTE,
	TNMSERR_IA_NEWLINE_IN_DQUOTE,
	TNMSERR_IA_SQUOTE_NO_CLOSE,
	TNMSERR_IA_DQUOTE_NO_CLOSE,
	TNMSERR_IA_COMMENT_NO_CLOSE,
	TNMSERR_IA_MISS_DECLARE,
	TNMSERR_IA_MISS_REPLACE,
	TNMSERR_IA_MISS_DEFINE,
	TNMSERR_IA_MISS_MACRO,
	TNMSERR_IA_MISS_COMMAND,
	TNMSERR_IA_MISS_PROPERTY,
	TNMSERR_IA_MACRO_NO_NAME,
	TNMSERR_IA_MACRO_NO_ARG_LIST,
	TNMSERR_IA_MACRO_NO_AFTER,
	TNMSERR_IA_MACRO_ARG_LIST_NO_OPEN,
	TNMSERR_IA_MACRO_ARG_LIST_NO_CLOSE,
	TNMSERR_IA_MACRO_ARG_LIST_NO_ARG,
	TNMSERR_IA_MACRO_ARG_LIST_MISS_ARG,
	TNMSERR_IA_PROPERTY_MISS_FORM,
	TNMSERR_IA_COMMAND_MISS_FORM,
	TNMSERR_IA_RE_DECRARE,
	TNMSERR_IA_MISS_FORM,
};

// ****************************************************************
// �C���N�f�[�^�\����
// ================================================================
//		�S�C���N�f�[�^���ʂ̃f�[�^�ł��B
//		�S�C���N�f�[�^�ŋ��ʂ��ĂP�����쐬���ĉ������B
// ================================================================
struct S_tnms_ia_data
{
	void	init();

	S_replace_node						replace_tree;	// �u���c���[
	ARRAY< BSP<S_tnms_ia_property> >	property_list;	// �v���p�e�B
	ARRAY< BSP<S_tnms_ia_command> >		command_list;	// �R�}���h
	std::set<TSTR>						name_set;		// �d������p�̖��O�Z�b�g

	C_tnms_form_table	form_table;				// �^�e�[�u��
	int					property_cnt;			// �v���p�e�B���iinc + scn�j
	int					command_cnt;			// �R�}���h���iinc + scn�j
	int					inc_property_cnt;		// �v���p�e�B���iinc�j
	int					inc_command_cnt;		// �R�}���h���iinc�j
};

// ****************************************************************
// �C���N�f�[�^�\���̂Q
// ================================================================
//		�e�C���N�f�[�^���ʂ̃f�[�^�ł��B
//		�C���N�f�[�^���ƂɂP���쐬���ĉ������B
// ================================================================
struct S_tnms_ia_data_2
{
	void	init();

	ARRAY<TSTR>			property_temp_list;	// ��͑O�̃v���p�e�B���X�g
	ARRAY<int>			property_line_list;	// ��͑O�̃v���p�e�B���X�g
	ARRAY<TSTR>			command_temp_list;	// ��͑O�̃R�}���h���X�g
	ARRAY<int>			command_line_list;	// ��͑O�̃R�}���h���X�g
};

// ****************************************************************
// inc analizer
// ================================================================
class C_inc_file_analizer
{
public:
	bool	analize_step_1(CTSTR& text, int parent_form, S_tnms_ia_data* piad, S_tnms_ia_data_2* piad2);		// parent_form �ɂ́AFM_GLOBAL �� FM_SCENE ���w�肵�܂�
	bool	analize_step_2(S_tnms_ia_data* piad, S_tnms_ia_data_2* piad2);

	TSTR	get_error_str();
	int		get_error_line();

private:

	bool	ia_comment_cut();
	bool	ia_main_step_1();
	bool	ia_main_step_2();

	bool	ia_skip_separator(TSTR::const_iterator& p, int* line_ret);
	bool	ia_declare(TSTR::const_iterator& p, int* line_ret, BSP<S_tnms_ia_declare>& declare);
	bool	ia_declare_type(TSTR::const_iterator& p, int* line_ret, TNMS_IA_DECLARE_TYPE* type);
	bool	ia_define_name(TSTR::const_iterator& p, int* line_ret, TSTR& name);
	bool	ia_define_space_name(TSTR::const_iterator& p, int* line_ret, TSTR& name);
	bool	ia_define_after(TSTR::const_iterator& p, int* line_ret, TSTR& after);
	bool	ia_macro_name(TSTR::const_iterator& p, int* line_ret, TSTR& name);
	bool	ia_macro_arg_list(TSTR::const_iterator& p, int* line_ret, ARRAY<S_tnms_ia_macro_arg>& arg_list);
	bool	ia_macro_arg(TSTR::const_iterator& p, int* line_ret, S_tnms_ia_macro_arg& arg);
	bool	ia_macro_after(TSTR::const_iterator& p, int* line_ret, TSTR& after);
	bool	ia_property(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret);
	bool	ia_command(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret);

	bool	ia_property_name(TSTR::const_iterator& p, int* line_ret, TSTR& name);
	bool	ia_command_name(TSTR::const_iterator& p, int* line_ret, TSTR& name);
	bool	ia_command_arg_list(TSTR::const_iterator& p, int* line_ret, S_tnms_arg_list& arg_list);
	bool	ia_command_arg(TSTR::const_iterator& p, int* line_ret, S_tnms_arg& arg);
	bool	ia_declare_property_form(TSTR::const_iterator& p, int* line_ret, int* form_ret, int* size_ret);
	bool	ia_declare_form(TSTR::const_iterator& p, int* line_ret, int* form_ret);
	bool	ia_form(TSTR::const_iterator& p, int error_line, int* line_ret, int* form_ret);
	bool	ia_array_size(TSTR::const_iterator& p_ret, int* line_ret, int* size_ret);

	// �}�N���̉��
	bool	std_replace(TSTR& text, TSTR::const_iterator& p, int* line_ret, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree);
	bool	analize_macro(TSTR& text, TSTR::const_iterator& p, int* line_ret, S_tnms_ia_replace* p_macro, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text);
	bool	analize_macro_replace(TSTR& text, int* line_ret, CTSTR& src_text, ARRAY<S_tnms_ia_macro_arg>& tmp_arg_list, ARRAY<TSTR>& real_arg_list, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text);


	S_tnms_ia_data*			m_piad;
	S_tnms_ia_data_2*		m_piad2;
	TSTR					m_text;
	int						m_parent_form;

	bool		error(int line, CTSTR& error_str);
	int			m_error_line;
	TSTR		m_error_str;
};




