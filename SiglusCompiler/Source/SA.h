#pragma		once

#include	"IA.h"
#include	"LA.h"

// ****************************************************************
// �\����� - Syntax Analizer -
// ================================================================
//		�\���̉�͂��s���A�\���؂��\�z���܂��B
// ================================================================

/*
	SS			<SS>				:= <��*> "����"
	BLOCK		<�u���b�N>			:= "{" <��*> "}"
	SENTENCE	<��>				:= <���x����> | <goto ��> | <return ��> | <if ��> | <for ��> | <while ��> | <continue ��> | <break ��> | <switch ��> | <�R�}���h��> | <�����> | <�e�L�X�g��>
	LABEL		<���x����>			:= "���x����"
	Z_LABEL		<�y���x����>		:= "�y���x����"
	DEF_SUB		<���[�`����`��>	:= "sub" <���[�`����> "{" <��*> "}"
	DEF_CMD		<�R�}���h��`��>	:= "command" <�^��> <�R�}���h��> "{" <��*> "}"
	DEF_PROP	<�v���p�e�B��`��>	:= "property" <�^��> <�v���p�e�B��> "{" <��*> "}"
	GOTO		<goto ��>			:= ("goto" | "gosub") [ <�������X�g> ] "���x����"
	RETURN		<return ��>			:= "return"
	IF			<if ��>				:= "if" "(" <��> ")" "{" <��*> "}"
	FOR			<for ��>			:= "for" "(" <��*> "," <��> "," <��*> ")" "{" <��*> "}"
	WHILE		<while ��>			:= "while" "(" <��> ")" "{" <��*> "}"
	CONTINUE	<continue ��>		:= "continue"
	BREAK		<break ��>			:= "break"
	SWITCH		<switch ��>			:= "switch" "(" <��> ")" "{" (<case ��> | <default ��>)* "}"
	CASE		<case ��>			:= "case" "(" <��> ")" <��*>
	COMMAND		<�R�}���h��>		:= <�G�������g��>
	ASSIGN		<�����>			:= <�G�������g��> "=" <��>
	TEXT		<�e�L�X�g��>		:= "�����񃊃e����"
	EXP			<��>				:= <�P����> | <�P�����Z�q> <��> | <��> <�񍀉��Z�q> <��>
	EXP_LIST	<�����X�g>			:= "{" <��> "," <��> ... "}"
	SMP_EXP		<�P����>			:= <goto ��> | "(" <��> ")" | <�G�������g��> | <�����X�g> | <���e����>
	ELM_EXP		<�G�������g��>		:= <�G�������g���X�g> [ <�������X�g> ]
	ELM_LIST	<�G�������g���X�g>	:= <�G�������g> <�q���G�������g *>
	ELM			<�G�������g>		:= <�G�������g��>
	ELM_CHILD	<�q���G�������g>	:= ("." <�G�������g��>) | "[" <��> "]"
	ELM_NAME	<�G�������g��>		:= "����`��"
	ARG_LIST	<�������X�g>		:= "(" <����> "," <����> ... ")"
	ARG			<����>				:= <��> | "����`��" "=" <��>
	NAMED_ARG	<���O������>		:= "����`��" "=" <��>
	NAME		<���O>				:= "�y" "�����񃊃e����" "�z"
	LITERAL		<���e����>			:= "�������e����" | "�����񃊃e����"
	SINGLE_OPR	<�P�����Z�q>		:= "+" | "-"
	DOUBLE_OPR	<�񍀉��Z�q>		:= "+" | "-" | "*" | "/" | "%" | "==" | "!=" | ">" | ">=" | "<" | "<="
	ASSIGN_OPR	<������Z�q>		:= "=" | "+=" | "-=" | "*=" | "/=" | "%="
*/

// ****************************************************************
// �\����̓G���[
// ================================================================
enum TNMSERR_SA
{
	TNMSERR_SA_NONE,
	TNMSERR_SA_BLOCK_ILLEGAL_SENTENCE,
	TNMSERR_SA_BLOCK_NO_CLOSE_BRACE,
	TNMSERR_SA_SENTENCE_ILLEGAL,
	TNMSERR_SA_LABEL_NOT_EXIST,
	TNMSERR_SA_LABEL_OVERLAPPED,
	TNMSERR_SA_Z_LABEL_00_NOT_EXIST,
	TNMSERR_SA_Z_LABEL_OVERLAPPED,
	TNMSERR_SA_DEF_CMD_NOT_EXIST,
	TNMSERR_SA_DEF_CMD_ILLEGAL_FORM,
	TNMSERR_SA_DEF_CMD_ILLEGAL_NAME,
	TNMSERR_SA_DEF_CMD_UNKNOWN_COMMAND,
	TNMSERR_SA_DEF_CMD_ALREADY_DEFINED,
	TNMSERR_SA_DEF_CMD_TYPE_NO_MATCH,
	TNMSERR_SA_DEF_CMD_ARG_TYPE_NO_MATCH,
	TNMSERR_SA_DEF_CMD_NO_CLOSE_PAREN,
	TNMSERR_SA_DEF_CMD_ILLEGAL_ARG,
	TNMSERR_SA_DEF_CMD_NO_COMMA,
	TNMSERR_SA_DEF_CMD_NO_OPEN_BRACE,
	TNMSERR_SA_DEF_CMD_NO_CLOSE_BRACE,
	TNMSERR_SA_DEF_CMD_ILLEGAL_BLOCK,
	TNMSERR_SA_DEF_PROP_ILLEGAL_FORM,
	TNMSERR_SA_DEF_PROP_ILLEGAL_NAME,
	TNMSERR_SA_DEF_PROP_NO_CLOSE_BRACKET,
	TNMSERR_SA_GOTO_NO_LABEL,
	TNMSERR_SA_RETURN_ILLEGAL_EXP,
	TNMSERR_SA_RETURN_NO_CLOSE_PAREN,
	TNMSERR_SA_IF_NO_OPEN_PAREN,
	TNMSERR_SA_IF_NO_CLOSE_PAREN,
	TNMSERR_SA_IF_ILLEGAL_COND,
	TNMSERR_SA_IF_NO_OPEN_BRACE,
	TNMSERR_SA_IF_NO_CLOSE_BRACE,
	TNMSERR_SA_IF_ILLEGAL_BLOCK,
	TNMSERR_SA_FOR_NO_OPEN_PAREN,
	TNMSERR_SA_FOR_NO_CLOSE_PAREN,
	TNMSERR_SA_FOR_ILLEGAL_INIT,
	TNMSERR_SA_FOR_ILLEGAL_COND,
	TNMSERR_SA_FOR_ILLEGAL_LOOP,
	TNMSERR_SA_FOR_NO_INIT_COMMA,
	TNMSERR_SA_FOR_NO_COND_COMMA,
	TNMSERR_SA_FOR_NO_OPEN_BRACE,
	TNMSERR_SA_FOR_NO_CLOSE_BRACE,
	TNMSERR_SA_FOR_ILLEGAL_BLOCK,
	TNMSERR_SA_WHILE_ILLEGAL_COND,
	TNMSERR_SA_WHILE_NO_OPEN_PAREN,
	TNMSERR_SA_WHILE_NO_CLOSE_PAREN,
	TNMSERR_SA_WHILE_NO_OPEN_BRACE,
	TNMSERR_SA_WHILE_NO_CLOSE_BRACE,
	TNMSERR_SA_WHILE_ILLEGAL_BLOCK,
	TNMSERR_SA_SWITCH_NO_OPEN_PAREN,
	TNMSERR_SA_SWITCH_NO_CLOSE_PAREN,
	TNMSERR_SA_SWITCH_ILLEGAL_COND,
	TNMSERR_SA_SWITCH_NO_OPEN_BRACE,
	TNMSERR_SA_SWITCH_NO_CLOSE_BRACE,
	TNMSERR_SA_SWITCH_ILLEGAL_CASE,
	TNMSERR_SA_CASE_NO_OPEN_PAREN,
	TNMSERR_SA_CASE_NO_CLOSE_PAREN,
	TNMSERR_SA_CASE_ILLEGAL_VALUE,
	TNMSERR_SA_CASE_ILLEGAL_BLOCK,
	TNMSERR_SA_DEFAULT_REDEFINE,
	TNMSERR_SA_ASSIGN_NO_EQUAL,
	TNMSERR_SA_ASSIGN_ILLEGAL_RIGHT,
	TNMSERR_SA_ELEMENT_UNKNOWN,
	TNMSERR_SA_ELEMENT_NO_OPEN,
	TNMSERR_SA_ELEMENT_NO_CLOSE,
	TNMSERR_SA_ELEMENT_NO_CHILD,
	TNMSERR_SA_ELEMENT_ILLEGAL_EXP,
	TNMSERR_SA_ARG_LIST_NO_CLOSE_PAREN,
	TNMSERR_SA_ARG_LIST_NAMED_ARG_EXIST_BEFORE,
	TNMSERR_SA_EXP_ILLEGAL,
	TNMSERR_SA_EXP_LIST_NO_CLOSE_BRACKET,
	TNMSERR_SA_SMP_EXP_NO_CLOSE_PAREN,
	TNMSERR_SA_NAME_ILLEGAL_NAME,
	TNMSERR_SA_NAME_NO_CLOSE_SUMI,
};

// ****************************************************************
// �\����́F�m�[�h�^�C�v
// ================================================================
enum TNMS_SA_NODE_TYPE
{
	TNMS_SA_NODE_TYPE_SENTENCE_LABEL,
	TNMS_SA_NODE_TYPE_SENTENCE_Z_LABEL,
	TNMS_SA_NODE_TYPE_SENTENCE_DEF_SUB,
	TNMS_SA_NODE_TYPE_SENTENCE_DEF_CMD,
	TNMS_SA_NODE_TYPE_SENTENCE_DEF_PROP,
	TNMS_SA_NODE_TYPE_SENTENCE_GOTO,
	TNMS_SA_NODE_TYPE_SENTENCE_RETURN,
	TNMS_SA_NODE_TYPE_SENTENCE_IF,
	TNMS_SA_NODE_TYPE_SENTENCE_FOR,
	TNMS_SA_NODE_TYPE_SENTENCE_WHILE,
	TNMS_SA_NODE_TYPE_SENTENCE_CONTINUE,
	TNMS_SA_NODE_TYPE_SENTENCE_BREAK,
	TNMS_SA_NODE_TYPE_SENTENCE_SWITCH,
	TNMS_SA_NODE_TYPE_SENTENCE_ASSIGN,
	TNMS_SA_NODE_TYPE_SENTENCE_COMMAND,
	TNMS_SA_NODE_TYPE_SENTENCE_TEXT,
	TNMS_SA_NODE_TYPE_SENTENCE_NAME,
	TNMS_SA_NODE_TYPE_SENTENCE_EOF,
	TNMS_SA_NODE_TYPE_GOTO_GOTO,
	TNMS_SA_NODE_TYPE_GOTO_GOSUB,
	TNMS_SA_NODE_TYPE_GOTO_GOSUBSTR,
	TNMS_SA_NODE_TYPE_GOTO_LABEL,
	TNMS_SA_NODE_TYPE_GOTO_Z_LABEL,
	TNMS_SA_NODE_TYPE_RETURN_WITH_ARG,
	TNMS_SA_NODE_TYPE_RETURN_WITHOUT_ARG,
	TNMS_SA_NODE_TYPE_IF_IF,
	TNMS_SA_NODE_TYPE_IF_ELSEIF,
	TNMS_SA_NODE_TYPE_IF_ELSE,
	TNMS_SA_NODE_TYPE_COMMAND_WITHOUT_ARG,
	TNMS_SA_NODE_TYPE_COMMAND_WITH_ARG,
	TNMS_SA_NODE_TYPE_EXP_SIMPLE,
	TNMS_SA_NODE_TYPE_EXP_OPR_1,
	TNMS_SA_NODE_TYPE_EXP_OPR_2,
	TNMS_SA_NODE_TYPE_SMP_EXP_KAKKO,
	TNMS_SA_NODE_TYPE_SMP_EXP_GOTO,
	TNMS_SA_NODE_TYPE_SMP_EXP_ELM_EXP,
	TNMS_SA_NODE_TYPE_SMP_EXP_EXP_LIST,
	TNMS_SA_NODE_TYPE_SMP_EXP_LITERAL,
	TNMS_SA_NODE_TYPE_ELEMENT_ELEMENT,
	TNMS_SA_NODE_TYPE_ELEMENT_ARRAY,
	TNMS_SA_NODE_TYPE_ARG_NO_NAME,
	TNMS_SA_NODE_TYPE_ARG_WITH_NAME,
};

// ****************************************************************
// �\����́F�m�[�h�\����
// ================================================================
struct TNMS_SA_NODE_SS;
struct TNMS_SA_NODE_SENTENCE;
struct TNMS_SA_NODE_BLOCK;
struct TNMS_SA_NODE_LABEL;
struct TNMS_SA_NODE_Z_LABEL;
struct TNMS_SA_NODE_DEF_CMD;
struct TNMS_SA_NODE_DEF_PROP;
struct TNMS_SA_NODE_GOTO;
struct TNMS_SA_NODE_RETURN;
struct TNMS_SA_NODE_IF;
struct TNMS_SA_NODE_IF_SUB;
struct TNMS_SA_NODE_FOR;
struct TNMS_SA_NODE_WHILE;
struct TNMS_SA_NODE_CONTINUE;
struct TNMS_SA_NODE_BREAK;
struct TNMS_SA_NODE_SWITCH;
struct TNMS_SA_NODE_CASE;
struct TNMS_SA_NODE_DEFAULT;
struct TNMS_SA_NODE_COMMAND;
struct TNMS_SA_NODE_ASSIGN;
struct TNMS_SA_NODE_EXP;
struct TNMS_SA_NODE_EXP_LIST;
struct TNMS_SA_NODE_SMP_EXP;
struct TNMS_SA_NODE_FORM;
struct TNMS_SA_NODE_ELM_EXP;
struct TNMS_SA_NODE_ELM_LIST;
struct TNMS_SA_NODE_ELEMENT;
struct TNMS_SA_NODE_ARG_LIST;
struct TNMS_SA_NODE_ARG;
struct TNMS_SA_NODE_TEXT;
struct TNMS_SA_NODE_NAME;
struct TNMS_SA_NODE_LITERAL;
struct TNMS_SA_NODE_ATOM;

// �x�[�X
struct TNMS_SA_NODE
{
	int					node_line;		// �s
	int					node_form;		// �^
	TNMS_SA_NODE_TYPE	node_type;		// �m�[�h�^�C�v
	TNMS_SA_NODE_TYPE	node_sub_type;	// �m�[�h�^�C�v
};

// <SS>
struct TNMS_SA_NODE_SS : public TNMS_SA_NODE
{
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	sentense_list;
};

// <�u���b�N>
struct TNMS_SA_NODE_BLOCK : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>				open_b;
	BSP<TNMS_SA_NODE_ATOM>				close_b;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	sentense_list;
};

// <��>
struct TNMS_SA_NODE_SENTENCE : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_BLOCK>		block;
	BSP<TNMS_SA_NODE_LABEL>		label;
	BSP<TNMS_SA_NODE_Z_LABEL>	z_label;
	BSP<TNMS_SA_NODE_DEF_CMD>	def_cmd;
	BSP<TNMS_SA_NODE_DEF_PROP>	def_prop;
	BSP<TNMS_SA_NODE_GOTO>		Goto;
	BSP<TNMS_SA_NODE_RETURN>	Return;
	BSP<TNMS_SA_NODE_IF>		If;
	BSP<TNMS_SA_NODE_FOR>		For;
	BSP<TNMS_SA_NODE_WHILE>		While;
	BSP<TNMS_SA_NODE_CONTINUE>	Continue;
	BSP<TNMS_SA_NODE_BREAK>		Break;
	BSP<TNMS_SA_NODE_SWITCH>	Switch;
	BSP<TNMS_SA_NODE_ASSIGN>	assign;
	BSP<TNMS_SA_NODE_COMMAND>	command;
	BSP<TNMS_SA_NODE_NAME>		name;
	BSP<TNMS_SA_NODE_ATOM>		text;
	BSP<TNMS_SA_NODE_ATOM>		eof;

	// �I�����Ɋւ��镶�t���O
	bool						is_include_sel;

	TNMS_SA_NODE_SENTENCE()
	{
		is_include_sel = false;
	}
};

// <���x����>
struct TNMS_SA_NODE_LABEL : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		label;
};

// <�y���x����>
struct TNMS_SA_NODE_Z_LABEL : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		z_label;
};

// <�R�}���h��`��>
struct TNMS_SA_NODE_DEF_CMD : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>				command;
	BSP<TNMS_SA_NODE_ATOM>				name;
	BSP<TNMS_SA_NODE_ATOM>				open_p;
	BSP<TNMS_SA_NODE_ATOM>				close_p;
	ARRAY< BSP<TNMS_SA_NODE_DEF_PROP> >	prop_list;
	ARRAY< BSP<TNMS_SA_NODE_ATOM> >		comma_list;
	BSP<TNMS_SA_NODE_ATOM>				colon;
	BSP<TNMS_SA_NODE_FORM>				form;
	BSP<TNMS_SA_NODE_BLOCK>				block;
	int									cmd_id;
	int									form_code;	// �^
};

// <�v���p�e�B��`��>
struct TNMS_SA_NODE_DEF_PROP : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>				Property;
	BSP<TNMS_SA_NODE_FORM>				form;
	BSP<TNMS_SA_NODE_ATOM>				name;
	BSP<TNMS_SA_NODE_ATOM>				colon;
	int									prop_id;	// �f�o�b�O���̖��O�����Ɏg��
	int									form_code;	// �^
};

// <goto ��>
struct TNMS_SA_NODE_GOTO : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		Goto;
	BSP<TNMS_SA_NODE_ARG_LIST>	arg_list;
	BSP<TNMS_SA_NODE_ATOM>		label;
	BSP<TNMS_SA_NODE_ATOM>		z_label;
	MBSTR						arg_state;
};

// <return ��>
struct TNMS_SA_NODE_RETURN : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		Return;
	BSP<TNMS_SA_NODE_ATOM>		open_p;
	BSP<TNMS_SA_NODE_ATOM>		close_p;
	BSP<TNMS_SA_NODE_EXP>		exp;
};

// <if ��>
struct TNMS_SA_NODE_IF : public TNMS_SA_NODE
{
	ARRAY<TNMS_SA_NODE_IF_SUB>	sub;
};

struct TNMS_SA_NODE_IF_SUB
{
	BSP<TNMS_SA_NODE_ATOM>		If;
	BSP<TNMS_SA_NODE_ATOM>		open_p;
	BSP<TNMS_SA_NODE_ATOM>		close_p;
	BSP<TNMS_SA_NODE_EXP>		cond;
	BSP<TNMS_SA_NODE_ATOM>		open_b;
	BSP<TNMS_SA_NODE_ATOM>		close_b;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	block;
};

// <for ��>
struct TNMS_SA_NODE_FOR : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		For;
	BSP<TNMS_SA_NODE_ATOM>		open_p;
	BSP<TNMS_SA_NODE_ATOM>		close_p;
	BSP<TNMS_SA_NODE_EXP>		cond;
	BSP<TNMS_SA_NODE_ATOM>		comma[2];
	BSP<TNMS_SA_NODE_ATOM>		open_b;
	BSP<TNMS_SA_NODE_ATOM>		close_b;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	init;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	loop;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	block;
};

// <while ��>
struct TNMS_SA_NODE_WHILE : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		While;
	BSP<TNMS_SA_NODE_ATOM>		open_p;
	BSP<TNMS_SA_NODE_ATOM>		close_p;
	BSP<TNMS_SA_NODE_EXP>		cond;
	BSP<TNMS_SA_NODE_ATOM>		open_b;
	BSP<TNMS_SA_NODE_ATOM>		close_b;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	block;
};

// <continue ��>
struct TNMS_SA_NODE_CONTINUE : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		Continue;
};

// <break ��>
struct TNMS_SA_NODE_BREAK : public TNMS_SA_NODE	
{
	BSP<TNMS_SA_NODE_ATOM>		Break;
};

// <switch ��>
struct TNMS_SA_NODE_SWITCH : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>			Switch;
	BSP<TNMS_SA_NODE_ATOM>			open_p;
	BSP<TNMS_SA_NODE_ATOM>			close_p;
	BSP<TNMS_SA_NODE_EXP>			cond;
	BSP<TNMS_SA_NODE_ATOM>			open_b;
	BSP<TNMS_SA_NODE_ATOM>			close_b;
	ARRAY< BSP<TNMS_SA_NODE_CASE> >	Case;
	BSP<TNMS_SA_NODE_DEFAULT>		Default;
};

// <case ��>
struct TNMS_SA_NODE_CASE : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>				Case;
	BSP<TNMS_SA_NODE_ATOM>				open_p;
	BSP<TNMS_SA_NODE_ATOM>				close_p;
	BSP<TNMS_SA_NODE_EXP>				value;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	block;
};

// <default ��>
struct TNMS_SA_NODE_DEFAULT : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>				Default;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	block;
};

// <�R�}���h��>
struct TNMS_SA_NODE_COMMAND : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ELM_EXP>	command;
};

// <�����>
struct TNMS_SA_NODE_ASSIGN : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ELM_EXP>	left;
	BSP<TNMS_SA_NODE_ATOM>		equal;
	BSP<TNMS_SA_NODE_EXP>		right;
	int							equal_form;
	int							al_id;
	bool						set_flag;	// __set �R�}���h���g���t���O�B
};

// <��>
struct TNMS_SA_NODE_EXP : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_SMP_EXP>	smp_exp;
	BSP<TNMS_SA_NODE_ATOM>		opr;
	BSP<TNMS_SA_NODE_EXP>		exp_1;
	BSP<TNMS_SA_NODE_EXP>		exp_2;
	int							tmp_form;	// �������̌^�i�v������Ă���^�j

	// �擪�̃A�g�����擾
	S_tnms_atom*	get_first_atom();
};

// <�����X�g>
struct TNMS_SA_NODE_EXP_LIST : public TNMS_SA_NODE
{
	ARRAY< BSP<TNMS_SA_NODE_EXP> >	exp;
	ARRAY< BSP<TNMS_SA_NODE_ATOM> >	comma;
	BSP<TNMS_SA_NODE_ATOM>			open_b;
	BSP<TNMS_SA_NODE_ATOM>			close_b;
	ARRAY<int>						form_list;

	// �擪�̃A�g�����擾
	S_tnms_atom*	get_first_atom();
};

// <�P����>
struct TNMS_SA_NODE_SMP_EXP : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		open;
	BSP<TNMS_SA_NODE_ATOM>		close;
	BSP<TNMS_SA_NODE_EXP>		exp;
	BSP<TNMS_SA_NODE_GOTO>		Goto;
	BSP<TNMS_SA_NODE_ELM_EXP>	elm_exp;
	BSP<TNMS_SA_NODE_EXP_LIST>	exp_list;
	BSP<TNMS_SA_NODE_ATOM>		Literal;

	// �擪�̃A�g�����擾
	S_tnms_atom*	get_first_atom();
};

// <�^>
struct TNMS_SA_NODE_FORM : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		form;
	int							form_code;
	BSP<TNMS_SA_NODE_ATOM>		open_b;
	BSP<TNMS_SA_NODE_ATOM>		close_b;
	BSP<TNMS_SA_NODE_EXP>		index;
};

// <�G�������g��>
struct TNMS_SA_NODE_ELM_EXP : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ELM_LIST>		elm_list;
	int								element_type;

	// �擪�̃A�g�����擾
	S_tnms_atom*	get_first_atom();
};

// <�G�������g���X�g>
struct TNMS_SA_NODE_ELM_LIST : public TNMS_SA_NODE
{
	int									parent_form_code;	// �X�R�[�v�iMA �Ō��܂�j
	ARRAY< BSP<TNMS_SA_NODE_ELEMENT> >	element;
	int									element_type;

	// �擪�̃A�g�����擾
	S_tnms_atom*	get_first_atom();
	// �����̃A�g�����擾
	S_tnms_atom*	get_last_atom();
};

// <�G�������g>
struct TNMS_SA_NODE_ELEMENT : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		name;
	BSP<TNMS_SA_NODE_ATOM>		dot;			// �q���̏ꍇ
	BSP<TNMS_SA_NODE_ATOM>		open_b;			// �Y�����J��
	BSP<TNMS_SA_NODE_ATOM>		close_b;		// �Y������
	BSP<TNMS_SA_NODE_EXP>		exp;			// �Y����
	BSP<TNMS_SA_NODE_ARG_LIST>	arg_list;		// �������X�g

	// �G�������g�R�[�h�Ȃ�
	int							element_code;
	int							element_type;			// �v���p�e�B�^�R�}���h
	int							element_parent_form;
	int							arg_list_id;
};

// <�������X�g>
struct TNMS_SA_NODE_ARG_LIST : public TNMS_SA_NODE
{
	ARRAY< BSP<TNMS_SA_NODE_ARG> >			arg;
	ARRAY< BSP<TNMS_SA_NODE_ATOM> >			comma;
	BSP<TNMS_SA_NODE_ATOM>					open_p;
	BSP<TNMS_SA_NODE_ATOM>					close_p;
	int										named_arg_cnt;	// ���O�������̌�
};

// <����>
struct TNMS_SA_NODE_ARG : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>	name;
	BSP<TNMS_SA_NODE_ATOM>	equal;
	BSP<TNMS_SA_NODE_EXP>	exp;
	int						name_id;		// ���OID
};

// <���O>
struct TNMS_SA_NODE_NAME : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		open_s;
	BSP<TNMS_SA_NODE_ATOM>		close_s;
	BSP<TNMS_SA_NODE_ATOM>		name;
};

// <LA�A�g��>
struct TNMS_SA_NODE_ATOM : public TNMS_SA_NODE
{
	S_tnms_atom					atom;
};

// ****************************************************************
// �\����́F���x���\����
// ================================================================
struct S_tnms_sa_label
{
	S_tnms_sa_label() : line(-1), exist(false)	{}

	TSTR	name;	// ���x����
	int		line;	// ���o��̍s�ԍ�
	bool	exist;	// ���݂��Ă���
};

// ****************************************************************
// �\����́F�y���x���\����
// ================================================================
struct S_tnms_sa_z_label
{
	S_tnms_sa_z_label()
	{
		exist = false;
	}

	int		line;	// ���o��̍s�ԍ�
	bool	exist;	// ���݂��Ă���
};

// ****************************************************************
// �\����́F���[�U�R�}���h�\����
// ================================================================
struct S_tnms_sa_user_cmd
{
	int		offset;
};

// ****************************************************************
// �\����́F�\����͍\����
// ================================================================
struct S_tnms_sa_data
{
	BSP<TNMS_SA_NODE_SS>		root;
	ARRAY<S_tnms_sa_label>		z_label_list;			// �y���x�����X�g
	ARRAY<TSTR>					call_prop_name_list;	// �R�[���v���p�e�B�̖��O���X�g
	int							cur_call_prop_cnt;		// �R�[���v���p�e�B�̌��i���݂̒l�j
	int							total_call_prop_cnt;	// �R�[���v���p�e�B�̌��i�ʂ��̒l�j
	int							command_in;				// �R�}���h�̒�
};

// ****************************************************************
// �\����� - Sentence Analizer -
// ================================================================
class C_tnms_sentence_analizer
{
public:
	bool		analize(S_tnms_ia_data* p_ia_data, S_tnms_la_data* p_la_data, S_tnms_sa_data* p_sa_data);

	S_tnms_atom	get_error_atom();
	int			get_error_line();
	TSTR		get_error_str();

	struct SA_ERROR
	{
		TNMSERR_SA		type;
		S_tnms_atom		atom;
	};

private:

	typedef	ARRAY<S_tnms_atom>::iterator	ITR;

	bool	sa_ss(ITR& atom_itr, BSP<TNMS_SA_NODE_SS>& ss);
	bool	sa_block(ITR& atom_itr, BSP<TNMS_SA_NODE_BLOCK>& block);
	bool	sa_sentence(ITR& atom_itr, BSP<TNMS_SA_NODE_SENTENCE>& sentence);
	bool	sa_label(ITR& atom_itr, BSP<TNMS_SA_NODE_LABEL>& label);
	bool	sa_z_label(ITR& atom_itr, BSP<TNMS_SA_NODE_Z_LABEL>& z_label);
	bool	sa_def_cmd(ITR& atom_itr, BSP<TNMS_SA_NODE_DEF_CMD>& def_cmd);
	bool	sa_def_prop(ITR& atom_itr, BSP<TNMS_SA_NODE_DEF_PROP>& def_prop);
	bool	sa_goto(ITR& atom_itr, BSP<TNMS_SA_NODE_GOTO>& Goto);
	bool	sa_return(ITR& atom_itr, BSP<TNMS_SA_NODE_RETURN>& Return);
	bool	sa_if(ITR& atom_itr, BSP<TNMS_SA_NODE_IF>& If);
	bool	sa_for(ITR& atom_itr, BSP<TNMS_SA_NODE_FOR>& For);
	bool	sa_while(ITR& atom_itr, BSP<TNMS_SA_NODE_WHILE>& While);
	bool	sa_continue(ITR& atom_itr, BSP<TNMS_SA_NODE_CONTINUE>& Continue);
	bool	sa_break(ITR& atom_itr, BSP<TNMS_SA_NODE_BREAK>& Break);
	bool	sa_switch(ITR& atom_itr, BSP<TNMS_SA_NODE_SWITCH>& Switch);
	bool	sa_case(ITR& atom_itr, BSP<TNMS_SA_NODE_CASE>& Case);
	bool	sa_default(ITR& atom_itr, BSP<TNMS_SA_NODE_DEFAULT>& Default);
	bool	sa_command_or_assign(ITR& atom_itr, BSP<TNMS_SA_NODE_COMMAND>& command, BSP<TNMS_SA_NODE_ASSIGN>& assign);
	bool	sa_arg_list(ITR& atom_itr, BSP<TNMS_SA_NODE_ARG_LIST>& arg_list);
	bool	sa_arg(ITR& atom_itr, BSP<TNMS_SA_NODE_ARG>& arg);
	bool	sa_named_arg(ITR& atom_itr, BSP<TNMS_SA_NODE_ARG>& arg);
	bool	sa_exp(ITR& atom_itr, BSP<TNMS_SA_NODE_EXP>& exp, int last_priority);
	bool	sa_exp_list(ITR& atom_itr, BSP<TNMS_SA_NODE_EXP_LIST>& exp_list);
	bool	sa_smp_exp(ITR& atom_itr, BSP<TNMS_SA_NODE_SMP_EXP>& smp_exp);
	bool	sa_form(ITR& atom_itr, BSP<TNMS_SA_NODE_FORM>& form);
	bool	sa_elm_exp(ITR& atom_itr, BSP<TNMS_SA_NODE_ELM_EXP>& elm_exp);
	bool	sa_elm_list(ITR& atom_itr, BSP<TNMS_SA_NODE_ELM_LIST>& elm_list);
	bool	sa_element(ITR& atom_itr, BSP<TNMS_SA_NODE_ELEMENT>& element, bool top_element);
	bool	sa_name(ITR& atom_itr, BSP<TNMS_SA_NODE_NAME>& name);
	bool	sa_literal(ITR& atom_itr, BSP<TNMS_SA_NODE_ATOM>& Literal);
	bool	sa_operator_1(ITR& atom_itr, BSP<TNMS_SA_NODE_ATOM>& opr);
	bool	sa_operator_2(ITR& atom_itr, BSP<TNMS_SA_NODE_ATOM>& opr, int last_priority, int* new_priority);
	bool	sa_assign_operator(ITR& atom_itr, BSP<TNMS_SA_NODE_ATOM>& opr);
	bool	sa_atom(ITR& atom_itr, BSP<TNMS_SA_NODE_ATOM>& atom, TNMS_LA_ATOM_TYPE type);

	enum
	{
		TNMS_SA_OPERATOR_PRIORITY_MAX = 32,
	};

	S_tnms_ia_data*			piad;		// ia �f�[�^
	S_tnms_la_data*			plad;		// la �f�[�^
	S_tnms_sa_data*			psad;		// sa �f�[�^
	ARRAY<S_tnms_sa_label>	label_list;	// ���x�����X�g

	bool	error(TNMSERR_SA type);
	bool	error(TNMSERR_SA type, S_tnms_atom atom);
	void	clear_error();

	SA_ERROR	last_error;
};


