#pragma		once

#include	"IA.h"
#include	"LA.h"

// ****************************************************************
// 構文解析 - Syntax Analizer -
// ================================================================
//		構文の解析を行い、構文木を構築します。
// ================================================================

/*
	SS			<SS>				:= <文*> "文末"
	BLOCK		<ブロック>			:= "{" <文*> "}"
	SENTENCE	<文>				:= <ラベル文> | <goto 文> | <return 文> | <if 文> | <for 文> | <while 文> | <continue 文> | <break 文> | <switch 文> | <コマンド文> | <代入文> | <テキスト文>
	LABEL		<ラベル文>			:= "ラベル名"
	Z_LABEL		<Ｚラベル文>		:= "Ｚラベル名"
	DEF_SUB		<ルーチン定義文>	:= "sub" <ルーチン名> "{" <文*> "}"
	DEF_CMD		<コマンド定義文>	:= "command" <型名> <コマンド名> "{" <文*> "}"
	DEF_PROP	<プロパティ定義文>	:= "property" <型名> <プロパティ名> "{" <文*> "}"
	GOTO		<goto 文>			:= ("goto" | "gosub") [ <引数リスト> ] "ラベル名"
	RETURN		<return 文>			:= "return"
	IF			<if 文>				:= "if" "(" <式> ")" "{" <文*> "}"
	FOR			<for 文>			:= "for" "(" <文*> "," <式> "," <文*> ")" "{" <文*> "}"
	WHILE		<while 文>			:= "while" "(" <式> ")" "{" <文*> "}"
	CONTINUE	<continue 文>		:= "continue"
	BREAK		<break 文>			:= "break"
	SWITCH		<switch 文>			:= "switch" "(" <式> ")" "{" (<case 文> | <default 文>)* "}"
	CASE		<case 文>			:= "case" "(" <式> ")" <文*>
	COMMAND		<コマンド文>		:= <エレメント式>
	ASSIGN		<代入文>			:= <エレメント式> "=" <式>
	TEXT		<テキスト文>		:= "文字列リテラル"
	EXP			<式>				:= <単項式> | <単項演算子> <式> | <式> <二項演算子> <式>
	EXP_LIST	<式リスト>			:= "{" <式> "," <式> ... "}"
	SMP_EXP		<単項式>			:= <goto 式> | "(" <式> ")" | <エレメント式> | <式リスト> | <リテラル>
	ELM_EXP		<エレメント式>		:= <エレメントリスト> [ <引数リスト> ]
	ELM_LIST	<エレメントリスト>	:= <エレメント> <子供エレメント *>
	ELM			<エレメント>		:= <エレメント名>
	ELM_CHILD	<子供エレメント>	:= ("." <エレメント名>) | "[" <式> "]"
	ELM_NAME	<エレメント名>		:= "未定義語"
	ARG_LIST	<引数リスト>		:= "(" <引数> "," <引数> ... ")"
	ARG			<引数>				:= <式> | "未定義語" "=" <式>
	NAMED_ARG	<名前つき引数>		:= "未定義語" "=" <式>
	NAME		<名前>				:= "【" "文字列リテラル" "】"
	LITERAL		<リテラル>			:= "整数リテラル" | "文字列リテラル"
	SINGLE_OPR	<単項演算子>		:= "+" | "-"
	DOUBLE_OPR	<二項演算子>		:= "+" | "-" | "*" | "/" | "%" | "==" | "!=" | ">" | ">=" | "<" | "<="
	ASSIGN_OPR	<代入演算子>		:= "=" | "+=" | "-=" | "*=" | "/=" | "%="
*/

// ****************************************************************
// 構文解析エラー
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
// 構文解析：ノードタイプ
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
// 構文解析：ノード構造体
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

// ベース
struct TNMS_SA_NODE
{
	int					node_line;		// 行
	int					node_form;		// 型
	TNMS_SA_NODE_TYPE	node_type;		// ノードタイプ
	TNMS_SA_NODE_TYPE	node_sub_type;	// ノードタイプ
};

// <SS>
struct TNMS_SA_NODE_SS : public TNMS_SA_NODE
{
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	sentense_list;
};

// <ブロック>
struct TNMS_SA_NODE_BLOCK : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>				open_b;
	BSP<TNMS_SA_NODE_ATOM>				close_b;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	sentense_list;
};

// <文>
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

	// 選択肢に関する文フラグ
	bool						is_include_sel;

	TNMS_SA_NODE_SENTENCE()
	{
		is_include_sel = false;
	}
};

// <ラベル文>
struct TNMS_SA_NODE_LABEL : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		label;
};

// <Ｚラベル文>
struct TNMS_SA_NODE_Z_LABEL : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		z_label;
};

// <コマンド定義文>
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
	int									form_code;	// 型
};

// <プロパティ定義文>
struct TNMS_SA_NODE_DEF_PROP : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>				Property;
	BSP<TNMS_SA_NODE_FORM>				form;
	BSP<TNMS_SA_NODE_ATOM>				name;
	BSP<TNMS_SA_NODE_ATOM>				colon;
	int									prop_id;	// デバッグ時の名前解決に使う
	int									form_code;	// 型
};

// <goto 文>
struct TNMS_SA_NODE_GOTO : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		Goto;
	BSP<TNMS_SA_NODE_ARG_LIST>	arg_list;
	BSP<TNMS_SA_NODE_ATOM>		label;
	BSP<TNMS_SA_NODE_ATOM>		z_label;
	MBSTR						arg_state;
};

// <return 文>
struct TNMS_SA_NODE_RETURN : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		Return;
	BSP<TNMS_SA_NODE_ATOM>		open_p;
	BSP<TNMS_SA_NODE_ATOM>		close_p;
	BSP<TNMS_SA_NODE_EXP>		exp;
};

// <if 文>
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

// <for 文>
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

// <while 文>
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

// <continue 文>
struct TNMS_SA_NODE_CONTINUE : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		Continue;
};

// <break 文>
struct TNMS_SA_NODE_BREAK : public TNMS_SA_NODE	
{
	BSP<TNMS_SA_NODE_ATOM>		Break;
};

// <switch 文>
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

// <case 文>
struct TNMS_SA_NODE_CASE : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>				Case;
	BSP<TNMS_SA_NODE_ATOM>				open_p;
	BSP<TNMS_SA_NODE_ATOM>				close_p;
	BSP<TNMS_SA_NODE_EXP>				value;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	block;
};

// <default 文>
struct TNMS_SA_NODE_DEFAULT : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>				Default;
	ARRAY< BSP<TNMS_SA_NODE_SENTENCE> >	block;
};

// <コマンド文>
struct TNMS_SA_NODE_COMMAND : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ELM_EXP>	command;
};

// <代入文>
struct TNMS_SA_NODE_ASSIGN : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ELM_EXP>	left;
	BSP<TNMS_SA_NODE_ATOM>		equal;
	BSP<TNMS_SA_NODE_EXP>		right;
	int							equal_form;
	int							al_id;
	bool						set_flag;	// __set コマンドを使うフラグ。
};

// <式>
struct TNMS_SA_NODE_EXP : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_SMP_EXP>	smp_exp;
	BSP<TNMS_SA_NODE_ATOM>		opr;
	BSP<TNMS_SA_NODE_EXP>		exp_1;
	BSP<TNMS_SA_NODE_EXP>		exp_2;
	int							tmp_form;	// 仮引数の型（要求されている型）

	// 先頭のアトムを取得
	S_tnms_atom*	get_first_atom();
};

// <式リスト>
struct TNMS_SA_NODE_EXP_LIST : public TNMS_SA_NODE
{
	ARRAY< BSP<TNMS_SA_NODE_EXP> >	exp;
	ARRAY< BSP<TNMS_SA_NODE_ATOM> >	comma;
	BSP<TNMS_SA_NODE_ATOM>			open_b;
	BSP<TNMS_SA_NODE_ATOM>			close_b;
	ARRAY<int>						form_list;

	// 先頭のアトムを取得
	S_tnms_atom*	get_first_atom();
};

// <単項式>
struct TNMS_SA_NODE_SMP_EXP : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		open;
	BSP<TNMS_SA_NODE_ATOM>		close;
	BSP<TNMS_SA_NODE_EXP>		exp;
	BSP<TNMS_SA_NODE_GOTO>		Goto;
	BSP<TNMS_SA_NODE_ELM_EXP>	elm_exp;
	BSP<TNMS_SA_NODE_EXP_LIST>	exp_list;
	BSP<TNMS_SA_NODE_ATOM>		Literal;

	// 先頭のアトムを取得
	S_tnms_atom*	get_first_atom();
};

// <型>
struct TNMS_SA_NODE_FORM : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		form;
	int							form_code;
	BSP<TNMS_SA_NODE_ATOM>		open_b;
	BSP<TNMS_SA_NODE_ATOM>		close_b;
	BSP<TNMS_SA_NODE_EXP>		index;
};

// <エレメント式>
struct TNMS_SA_NODE_ELM_EXP : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ELM_LIST>		elm_list;
	int								element_type;

	// 先頭のアトムを取得
	S_tnms_atom*	get_first_atom();
};

// <エレメントリスト>
struct TNMS_SA_NODE_ELM_LIST : public TNMS_SA_NODE
{
	int									parent_form_code;	// スコープ（MA で決まる）
	ARRAY< BSP<TNMS_SA_NODE_ELEMENT> >	element;
	int									element_type;

	// 先頭のアトムを取得
	S_tnms_atom*	get_first_atom();
	// 末尾のアトムを取得
	S_tnms_atom*	get_last_atom();
};

// <エレメント>
struct TNMS_SA_NODE_ELEMENT : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		name;
	BSP<TNMS_SA_NODE_ATOM>		dot;			// 子供の場合
	BSP<TNMS_SA_NODE_ATOM>		open_b;			// 添え字開き
	BSP<TNMS_SA_NODE_ATOM>		close_b;		// 添え字閉じ
	BSP<TNMS_SA_NODE_EXP>		exp;			// 添え字
	BSP<TNMS_SA_NODE_ARG_LIST>	arg_list;		// 引数リスト

	// エレメントコードなど
	int							element_code;
	int							element_type;			// プロパティ／コマンド
	int							element_parent_form;
	int							arg_list_id;
};

// <引数リスト>
struct TNMS_SA_NODE_ARG_LIST : public TNMS_SA_NODE
{
	ARRAY< BSP<TNMS_SA_NODE_ARG> >			arg;
	ARRAY< BSP<TNMS_SA_NODE_ATOM> >			comma;
	BSP<TNMS_SA_NODE_ATOM>					open_p;
	BSP<TNMS_SA_NODE_ATOM>					close_p;
	int										named_arg_cnt;	// 名前つき引数の個数
};

// <引数>
struct TNMS_SA_NODE_ARG : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>	name;
	BSP<TNMS_SA_NODE_ATOM>	equal;
	BSP<TNMS_SA_NODE_EXP>	exp;
	int						name_id;		// 名前ID
};

// <名前>
struct TNMS_SA_NODE_NAME : public TNMS_SA_NODE
{
	BSP<TNMS_SA_NODE_ATOM>		open_s;
	BSP<TNMS_SA_NODE_ATOM>		close_s;
	BSP<TNMS_SA_NODE_ATOM>		name;
};

// <LAアトム>
struct TNMS_SA_NODE_ATOM : public TNMS_SA_NODE
{
	S_tnms_atom					atom;
};

// ****************************************************************
// 構文解析：ラベル構造体
// ================================================================
struct S_tnms_sa_label
{
	S_tnms_sa_label() : line(-1), exist(false)	{}

	TSTR	name;	// ラベル名
	int		line;	// 初登場の行番号
	bool	exist;	// 存在している
};

// ****************************************************************
// 構文解析：Ｚラベル構造体
// ================================================================
struct S_tnms_sa_z_label
{
	S_tnms_sa_z_label()
	{
		exist = false;
	}

	int		line;	// 初登場の行番号
	bool	exist;	// 存在している
};

// ****************************************************************
// 構文解析：ユーザコマンド構造体
// ================================================================
struct S_tnms_sa_user_cmd
{
	int		offset;
};

// ****************************************************************
// 構文解析：構文解析構造体
// ================================================================
struct S_tnms_sa_data
{
	BSP<TNMS_SA_NODE_SS>		root;
	ARRAY<S_tnms_sa_label>		z_label_list;			// Ｚラベルリスト
	ARRAY<TSTR>					call_prop_name_list;	// コールプロパティの名前リスト
	int							cur_call_prop_cnt;		// コールプロパティの個数（現在の値）
	int							total_call_prop_cnt;	// コールプロパティの個数（通しの値）
	int							command_in;				// コマンドの中
};

// ****************************************************************
// 構文解析 - Sentence Analizer -
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

	S_tnms_ia_data*			piad;		// ia データ
	S_tnms_la_data*			plad;		// la データ
	S_tnms_sa_data*			psad;		// sa データ
	ARRAY<S_tnms_sa_label>	label_list;	// ラベルリスト

	bool	error(TNMSERR_SA type);
	bool	error(TNMSERR_SA type, S_tnms_atom atom);
	void	clear_error();

	SA_ERROR	last_error;
};


