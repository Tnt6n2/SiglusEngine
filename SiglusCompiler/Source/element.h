#pragma		once

// ****************************************************************
// def �t�@�C�������荞�ݗp�̍\����
// ================================================================
struct S_tnms_element_def
{
	int		type;		// �v���p�e�B�A�R�}���h
	int		parent;		// �e�̌^
	int		form;		// �^
	TSTR	name;		// ���O
	BYTE	owner;		// �쐬��
	BYTE	group;		// �O���[�v
	WORD	code;		// �R�[�h
	TSTR	arg_info;	// �������
};

struct S_tnms_form_def
{
	TSTR	name;
	int		code;
};

// ****************************************************************
// �����\����
// ================================================================
struct S_tnms_arg
{
	S_tnms_arg() : def_int(0), def_exist(false)	{}

	int		id;			// ID
	int		form;		// �^
	TSTR	name;		// ���O
	int		def_int;	// �����^�̏ꍇ�̃f�t�H���g�l
	TSTR	def_str;	// ������^�̏ꍇ�̃f�t�H���g�l
	bool	def_exist;	// �f�t�H���g���������邩�ǂ���
};

// ****************************************************************
// �������X�g�\����
// ================================================================
struct S_tnms_arg_list
{
	ARRAY<S_tnms_arg>		arg_list;	// �������X�g
};

// ****************************************************************
// �G�������g�^�C�v
// ================================================================
enum TNMS_ELEMENT_TYPE
{
	TNMS_ELEMENT_TYPE_PROPERTY,	// �v���p�e�B
	TNMS_ELEMENT_TYPE_COMMAND,	// �R�}���h
};

// ****************************************************************
// �G�������g�\����
// ================================================================
struct S_tnms_element
{
	int		type;		// �v���p�e�B�A�R�}���h
	int		code;		// �R�[�h
	TSTR	name;		// ���O
	int		form;		// �^

	void	add_arg_list(int id, S_tnms_arg_list& arg_list);

	std::map<int, S_tnms_arg_list>	arg_map;	// �����}�b�v
};

// ****************************************************************
// �^�\����
// ================================================================
struct S_tnms_form
{
	int		code;		// �R�[�h
	TSTR	name;		// ���O

	void			clear();
	void			add_element(S_tnms_element* element);
	S_tnms_element*	get_element_by_name(CTSTR& name);
	S_tnms_element*	get_element_by_code(int code);

private:

	std::map<int, S_tnms_element>	element_map_by_code;	// �G�������g�}�b�v�i�R�[�h����j
	std::map<TSTR, S_tnms_element>	element_map_by_name;	// �G�������g�}�b�v�i���O����j
};

// ****************************************************************
// �^�e�[�u��
// ================================================================
class C_tnms_form_table
{
public:

	void				create_system_form_table();
	void				add_form(S_tnms_form* form);
	S_tnms_form*		get_form_by_name(CTSTR& name);
	S_tnms_form*		get_form_by_code(int code);
	S_tnms_element*		find_element_info(CTSTR& element_name, int *ret_parent_form_code);
	bool				check_element_info(CTSTR& element_name);
	bool				get_element_info(int parent_form_code, TSTR name, S_tnms_element** element_info);

	S_tnms_form		form_call;		// "call" �^�f�t�H���g�l

private:

	std::map<int, S_tnms_form>	form_map_by_code;	// �^�}�b�v�i�R�[�h����j
	std::map<TSTR, S_tnms_form>	form_map_by_name;	// �^�}�b�v�i���O����j
};

// ****************************************************************
// �^�� �� �^�ɕϊ�
// ================================================================
int get_form_code_by_name(CTSTR& name);
