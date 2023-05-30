#pragma		once

#include	"IA.h"
#include	"LA.h"
#include	"SA.h"

// ****************************************************************
// 意味解析 - Semantic Analizer -
// ================================================================
//		各ノードの型が決定されます。
//		主な仕事は、コマンドの引数や演算の型チェックです。
//		参照を値に変換する処理もここで行います。
//		その他、「添え字の中身は整数」、「条件式は整数」などの判定もここで行います。
//		非常に重要な処理です。
// ================================================================


// ****************************************************************
// 意味解析エラー
// ================================================================
enum TNMSERR_MA
{
	TNMSERR_MA_NONE,
	TNMSERR_MA_DEF_PROP_NOT_INT,
	TNMSERR_MA_IF_COND_IS_NOT_INT,
	TNMSERR_MA_FOR_COND_IS_NOT_INT,
	TNMSERR_MA_WHILE_COND_IS_NOT_INT,
	TNMSERR_MA_CASE_TYPE_MISMATCH,
	TNMSERR_MA_ASSIGN_LEFT_NEED_REFERENCE,
	TNMSERR_MA_ASSIGN_TYPE_NO_MATCH,
	TNMSERR_MA_EXP_TYPE_NO_MATCH,
	TNMSERR_MA_ELEMENT_IS_PROPERTY,
	TNMSERR_MA_ELEMENT_UNKNOWN,
	TNMSERR_MA_ELEMENT_ILLEGAL_ARRAY,
	TNMSERR_MA_INDEX_NOT_INT,
	TNMSERR_MA_ARG_TYPE_NO_MATCH,
	TNMSERR_MA_CMD_NO_NAMED_ARG_LIST,
	TNMSERR_MA_CMD_ILLEGAL_NAMED_ARG,
	TNMSERR_MA_SEL_CANNOT_USE_IN_COND,
	TNMSERR_MA_SEL_CANNOT_USE_IN_ARG,
	TNMSERR_MA_SEL_CANNOT_USE_IN_GOTO,
	TNMSERR_MA_SEL_CANNOT_USE_IN_RETURN,
	TNMSERR_MA_SEL_CANNOT_USE_IN_INDEX,
	TNMSERR_MA_PROPERTY_OUT_OF_COMMAND,
};

// ****************************************************************
// 意味解析 - Semantic Analizer -
// ================================================================
class C_tnms_semantic_analizer
{
public:
	bool		analize(S_tnms_ia_data* p_ia_data, S_tnms_la_data* p_la_data, S_tnms_sa_data* p_sa_data);

	S_tnms_atom	get_error_atom();
	int			get_error_line();
	TSTR		get_error_str();

	struct MA_ERROR
	{
		TNMSERR_MA		type;
		S_tnms_atom		atom;
	};

	bool		find_element_info_from_outside(CTSTR& element_name);								// エレメント検索（※外部からの呼び出し用）

private:

	typedef	ARRAY<S_tnms_atom>::iterator	ITR;

	bool	ma_ss(BSP<TNMS_SA_NODE_SS> ss);
	bool	ma_block(BSP<TNMS_SA_NODE_BLOCK> block);
	bool	ma_sentence(BSP<TNMS_SA_NODE_SENTENCE> sentence);
	bool	ma_label(BSP<TNMS_SA_NODE_LABEL> label);
	bool	ma_z_label(BSP<TNMS_SA_NODE_Z_LABEL> z_label);
	bool	ma_def_prop(BSP<TNMS_SA_NODE_DEF_PROP> def_prop);
	bool	ma_def_cmd(BSP<TNMS_SA_NODE_DEF_CMD> def_cmd);
	bool	ma_goto(BSP<TNMS_SA_NODE_GOTO> Goto);
	bool	ma_goto_exp(BSP<TNMS_SA_NODE_GOTO> Goto, bool* ret_exist_sel);
	bool	ma_return(BSP<TNMS_SA_NODE_RETURN> Return, bool* ret_exist_sel);
	bool	ma_if(BSP<TNMS_SA_NODE_IF> If);
	bool	ma_for(BSP<TNMS_SA_NODE_FOR> For);
	bool	ma_while(BSP<TNMS_SA_NODE_WHILE> While);
	bool	ma_continue(BSP<TNMS_SA_NODE_CONTINUE> Continue);
	bool	ma_break(BSP<TNMS_SA_NODE_BREAK> Break);
	bool	ma_switch(BSP<TNMS_SA_NODE_SWITCH> Switch);
	bool	ma_case(BSP<TNMS_SA_NODE_CASE> Case);
	bool	ma_default(BSP<TNMS_SA_NODE_DEFAULT> Default);
	bool	ma_command(BSP<TNMS_SA_NODE_COMMAND> command, bool* ret_exist_sel);
	bool	ma_text(BSP<TNMS_SA_NODE_ATOM> text);
	bool	ma_assign(BSP<TNMS_SA_NODE_ASSIGN> assign, bool* ret_exist_sel);
	bool	ma_exp(BSP<TNMS_SA_NODE_EXP> exp, bool* ret_exist_sel);
	bool	ma_exp_sub(BSP<TNMS_SA_NODE_EXP> exp, bool* ret_exist_sel);
	bool	ma_left(BSP<TNMS_SA_NODE_ELM_EXP> Property);
	bool	ma_exp_list(BSP<TNMS_SA_NODE_EXP_LIST> exp_list, bool* ret_exist_sel);
	bool	ma_smp_exp(BSP<TNMS_SA_NODE_SMP_EXP> smp_exp, bool* ret_exist_sel);
	bool	ma_form(BSP<TNMS_SA_NODE_FORM> form);
	bool	ma_elm_exp(BSP<TNMS_SA_NODE_ELM_EXP> elm_exp, bool* ret_exist_sel);
	bool	ma_elm_list(BSP<TNMS_SA_NODE_ELM_LIST> elm_list, bool* ret_exist_sel);
	bool	ma_element(int parent_form_code, BSP<TNMS_SA_NODE_ELEMENT> element, bool* ret_exist_sel);
	bool	ma_arg_list(BSP<TNMS_SA_NODE_ARG_LIST> arg_list, bool* ret_exist_sel);
	bool	ma_arg(BSP<TNMS_SA_NODE_ARG> arg, bool* ret_exist_sel);
	bool	ma_name(BSP<TNMS_SA_NODE_NAME> name);
	bool	ma_literal(BSP<TNMS_SA_NODE_ATOM> Literal);
	bool	ma_operator_1(BSP<TNMS_SA_NODE_ATOM> opr);
	bool	ma_operator_2(BSP<TNMS_SA_NODE_ATOM> opr);
	bool	ma_assign_operator(BSP<TNMS_SA_NODE_ATOM> opr);
	bool	ma_atom(BSP<TNMS_SA_NODE_ATOM> atom, TNMS_LA_ATOM_TYPE type);
	bool	ma_eof(BSP<TNMS_SA_NODE_ATOM> eof);

	// 意味解析（型チェックなど）
	int		check_arg_list(BSP<TNMS_SA_NODE_ELEMENT> element, S_tnms_element* element_info, BSP<TNMS_SA_NODE_ARG_LIST> real_arg_list);
	bool	check_no_named_arg_list(S_tnms_arg_list& temp_arg_list, BSP<TNMS_SA_NODE_ARG_LIST> real_arg_list);
	bool	check_named_arg_list(BSP<TNMS_SA_NODE_ELEMENT> element, S_tnms_element* element_info, BSP<TNMS_SA_NODE_ARG_LIST> real_arg_list);
	int		check_operate_1(int r_form, BYTE opr);
	int		check_operate_2(int l_form, int r_form, BYTE opr);

	S_tnms_ia_data*			piad;		// ia データ
	S_tnms_la_data*			plad;		// la データ
	S_tnms_sa_data*			psad;		// sa データ

	bool	error(TNMSERR_MA type, S_tnms_atom atom);
	void	clear_error();

	MA_ERROR	last_error;
};
