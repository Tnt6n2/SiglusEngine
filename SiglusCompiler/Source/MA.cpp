#include	"pch.h"
#include	"code.h"
#include	"MA.h"

// ****************************************************************
// 意味解析 - Semantic Analizer -
// ================================================================
bool C_tnms_semantic_analizer::analize(S_tnms_ia_data* piad_, S_tnms_la_data* plad_, S_tnms_sa_data* psad_)
{
	piad = piad_;
	plad = plad_;
	psad = psad_;

	// 初期化
	psad->command_in = 0;

	// ss
	if (!ma_ss(psad->root))
		return false;

	return true;
}

// ****************************************************************
// 意味解析：<ss>
// ================================================================
bool C_tnms_semantic_analizer::ma_ss(BSP<TNMS_SA_NODE_SS> ss)
{
	// <文*>
	for (int s = 0; s < (int)ss->sentense_list.size(); s++)	{
		if (!ma_sentence(ss->sentense_list[s]))
			return false;
	}

	// 型を決定
	ss->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<ブロック>
// ================================================================
//		２重にできません。（コールプロパティの破棄のタイミングの問題）
//		とりあえずは２重にならないコマンドの宣言などでのみ使用してください。
// ================================================================
bool C_tnms_semantic_analizer::ma_block(BSP<TNMS_SA_NODE_BLOCK> block)
{
	// <文*>
	for (int s = 0; s < (int)block->sentense_list.size(); s++)	{
		if (!ma_sentence(block->sentense_list[s]))
			return false;
	}

	// コールプロパティをクリア（初期状態に戻す）
	*piad->form_table.get_form_by_code(FM_CALL) = piad->form_table.form_call;
	psad->cur_call_prop_cnt = 0;

	// 型を決定
	block->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<文>
// ================================================================
bool C_tnms_semantic_analizer::ma_sentence(BSP<TNMS_SA_NODE_SENTENCE> sentence)
{
	bool ret = false;
	bool is_exist_sel = false;

	// <文>
	switch (sentence->node_type)	{

		case TNMS_SA_NODE_TYPE_SENTENCE_LABEL:
			ret = ma_label(sentence->label);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_Z_LABEL:
			ret = ma_z_label(sentence->z_label);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_DEF_PROP:
			ret = ma_def_prop(sentence->def_prop);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_DEF_CMD:
			ret = ma_def_cmd(sentence->def_cmd);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_GOTO:
			ret = ma_goto(sentence->Goto);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_RETURN:
			ret = ma_return(sentence->Return, &is_exist_sel);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_IF:
			ret = ma_if(sentence->If);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_FOR:
			ret = ma_for(sentence->For);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_WHILE:
			ret = ma_while(sentence->While);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_CONTINUE:
			ret = ma_continue(sentence->Continue);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_BREAK:
			ret = ma_break(sentence->Break);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_SWITCH:
			ret = ma_switch(sentence->Switch);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_ASSIGN:
			ret = ma_assign(sentence->assign, &is_exist_sel);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_COMMAND:
			ret = ma_command(sentence->command, &is_exist_sel);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_TEXT:
			ret = ma_text(sentence->text);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_NAME:
			ret = ma_name(sentence->name);
			break;
		case TNMS_SA_NODE_TYPE_SENTENCE_EOF:
			ret = ma_eof(sentence->eof);
			break;
	}

	// 成功した場合の処理
	if (ret)	{

		// 型を決定
		sentence->node_form = FM_VOID;
		// 選択肢を含む文
		sentence->is_include_sel = is_exist_sel;
		//if (is_exist_sel)	{
		//	info_box(_T("MA"));
		//}
	}

	return ret;
}

// ****************************************************************
// 意味解析：<ラベル>
// ================================================================
bool C_tnms_semantic_analizer::ma_label(BSP<TNMS_SA_NODE_LABEL> label)
{
	// 型を決定
	label->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<Ｚラベル>
// ================================================================
bool C_tnms_semantic_analizer::ma_z_label(BSP<TNMS_SA_NODE_Z_LABEL> z_label)
{
	// 型を決定
	z_label->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<プロパティ定義文>
// ================================================================
bool C_tnms_semantic_analizer::ma_def_prop(BSP<TNMS_SA_NODE_DEF_PROP> def_prop)
{
	// コマンドの中じゃなければエラー
	if (psad->command_in == 0)
	{
		return error(TNMSERR_MA_PROPERTY_OUT_OF_COMMAND, def_prop->Property->atom);
	}

	// 型を決定
	def_prop->node_form = FM_VOID;

	// 型
	if (def_prop->form)	{
		if (!ma_form(def_prop->form))
			return false;
	}

	// コールプロパティを追加
	S_tnms_element element;
	element.type = TNMS_ELEMENT_TYPE_PROPERTY;
	element.code = create_elm_code(ELM_OWNER_CALL_PROP, 0, psad->cur_call_prop_cnt);
	element.form = def_prop->form_code;
	element.name = plad->unknown_list[def_prop->name->atom.opt];
	piad->form_table.get_form_by_code(FM_CALL)->add_element(&element);

	// コールプロパティＩＤ決定（デバッグ時の名前解決に使う）
	def_prop->prop_id = psad->total_call_prop_cnt;
	psad->call_prop_name_list.push_back(element.name);

	// プロパティ数をインクリメント
	psad->cur_call_prop_cnt ++;
	psad->total_call_prop_cnt ++;

	return true;
}

// ****************************************************************
// 意味解析：<コマンド定義文>
// ================================================================
bool C_tnms_semantic_analizer::ma_def_cmd(BSP<TNMS_SA_NODE_DEF_CMD> def_cmd)
{
	// コマンドの中に入る（プロパティが宣言できるようになる）
	psad->command_in = 1;

	// <プロパティ宣言>
	for (int p = 0; p < (int)def_cmd->prop_list.size(); p++)	{
		if (!ma_def_prop(def_cmd->prop_list[p]))
			return false;
	}

	// <ブロック>
	if (!ma_block(def_cmd->block))
		return false;

	// コマンドから脱出
	psad->command_in = 0;

	// 型を決定
	def_cmd->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<goto 文／gosub 文>
// ================================================================
bool C_tnms_semantic_analizer::ma_goto(BSP<TNMS_SA_NODE_GOTO> Goto)
{
	bool is_include_sel = false;

	// gosub 文
	if (Goto->node_type != TNMS_SA_NODE_TYPE_GOTO_GOTO)	{

		// gosub 式
		if (!ma_goto_exp(Goto, &is_include_sel))
			return false;

		// gosub の中では選択肢は使えない
		if (is_include_sel)
			return error(TNMSERR_MA_SEL_CANNOT_USE_IN_GOTO, Goto->Goto->atom);
	}

	// 型を決定
	Goto->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<goto／gosub 式>
// ================================================================
bool C_tnms_semantic_analizer::ma_goto_exp(BSP<TNMS_SA_NODE_GOTO> Goto, bool* ret_exist_sel)
{
	// 引数
	if (!ma_arg_list(Goto->arg_list, ret_exist_sel))
		return false;

	// 型を決定
	if (false);
	else if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOTO)		{
		Goto->node_form = FM_VOID;		// goto の戻り値は void
	}
	else if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOSUB)	{
		Goto->node_form = FM_INT;		// gosub の戻り値は int
	}
	else if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOSUBSTR)	{
		Goto->node_form = FM_STR;		// gosubstr の戻り値は str
	}
	else
		return false;

	return true;
}

// ****************************************************************
// 意味解析：<return 文>
// ================================================================
bool C_tnms_semantic_analizer::ma_return(BSP<TNMS_SA_NODE_RETURN> Return, bool* ret_exist_sel)
{
	// 引数ありの場合
	if (Return->node_type == TNMS_SA_NODE_TYPE_RETURN_WITH_ARG)	{

		if (!ma_exp(Return->exp, ret_exist_sel))
			return false;
	}

	// 型を決定
	Return->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<if 文>
// ================================================================
bool C_tnms_semantic_analizer::ma_if(BSP<TNMS_SA_NODE_IF> If)
{
	bool is_include_sel = false;

	int if_cnt = (int)If->sub.size();
	for (int i = 0; i < if_cnt; i++)	{

		// "if" | "elseif"
		if (If->sub[i].If->atom.type == TNMS_LA_ATOM_IF || If->sub[i].If->atom.type == TNMS_LA_ATOM_ELSEIF)	{

			// <条件式>
			if (!ma_exp(If->sub[i].cond, &is_include_sel))
				return false;

			// 条件式は整数
			if (If->sub[i].cond->node_form != FM_INT && If->sub[i].cond->node_form != FM_INTREF)
				return error(TNMSERR_MA_IF_COND_IS_NOT_INT, If->sub[i].If->atom);

			// 条件式の中では選択肢は使えない
			if (is_include_sel)
				return error(TNMSERR_MA_SEL_CANNOT_USE_IN_COND, If->sub[i].If->atom);
		}

		// <ブロック>
		for (int s = 0; s < (int)If->sub[i].block.size(); s++)	{
			if (!ma_sentence(If->sub[i].block[s]))
				return false;
		}
	}

	// 型を決定
	If->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<for 文>
// ================================================================
bool C_tnms_semantic_analizer::ma_for(BSP<TNMS_SA_NODE_FOR> For)
{
	bool is_include_sel = false;

	// <初期化文>
	for (int s = 0; s < (int)For->init.size(); s++)	{
		if (!ma_sentence(For->init[s]))
			return false;
	}

	// <条件式>
	if (!ma_exp(For->cond, &is_include_sel))
		return false;

	// 条件式の中では選択肢は使えない
	if (is_include_sel)
		return error(TNMSERR_MA_SEL_CANNOT_USE_IN_COND, For->For->atom);

	// 条件式は整数
	if (For->cond->node_form != FM_INT && For->cond->node_form != FM_INTREF)
		return error(TNMSERR_MA_FOR_COND_IS_NOT_INT, For->For->atom);

	// <ループ文>
	for (int s = 0; s < (int)For->loop.size(); s++)	{
		if (!ma_sentence(For->loop[s]))
			return false;
	}

	// <ブロック>
	for (int s = 0; s < (int)For->block.size(); s++)	{
		if (!ma_sentence(For->block[s]))
			return false;
	}

	// 型を決定
	For->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<while 文>
// ================================================================
bool C_tnms_semantic_analizer::ma_while(BSP<TNMS_SA_NODE_WHILE> While)
{
	bool is_include_sel = false;

	// <条件式>
	if (!ma_exp(While->cond, &is_include_sel))
		return false;

	// 条件式の中では選択肢は使えない
	if (is_include_sel)
		return error(TNMSERR_MA_SEL_CANNOT_USE_IN_COND, While->While->atom);

	// 条件式は整数
	if (While->cond->node_form != FM_INT && While->cond->node_form != FM_INTREF)
		return error(TNMSERR_MA_WHILE_COND_IS_NOT_INT, While->While->atom);

	// <ブロック>
	for (int s = 0; s < (int)While->block.size(); s++)	{
		if (!ma_sentence(While->block[s]))
			return false;
	}

	// 型を決定
	While->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<continue 文>
// ================================================================
bool C_tnms_semantic_analizer::ma_continue(BSP<TNMS_SA_NODE_CONTINUE> Continue)
{
	// 型を決定
	Continue->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<break 文>
// ================================================================
bool C_tnms_semantic_analizer::ma_break(BSP<TNMS_SA_NODE_BREAK> Break)
{
	// 型を決定
	Break->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<switch 文>
// ================================================================
bool C_tnms_semantic_analizer::ma_switch(BSP<TNMS_SA_NODE_SWITCH> Switch)
{
	bool is_include_sel = false;

	// <条件式>
	if (!ma_exp(Switch->cond, &is_include_sel))
		return false;

	// 条件式の中では選択肢は使えない
	if (is_include_sel)
		return error(TNMSERR_MA_SEL_CANNOT_USE_IN_COND, Switch->Switch->atom);

	// <case 文*>
	for (int c = 0; c < (int)Switch->Case.size(); c++)	{

		// <比較値>
		if (!ma_exp(Switch->Case[c]->value, &is_include_sel))
			return false;

		// 条件式の中では選択肢は使えない
		if (is_include_sel)
			return error(TNMSERR_MA_SEL_CANNOT_USE_IN_COND, Switch->Case[c]->Case->atom);

		// case 文の比較値と switch 文の条件式の型は等しい
		if (false);
		else if (Switch->cond->node_form == FM_INT || Switch->cond->node_form == FM_INTREF)	{
			if (Switch->Case[c]->value->node_form != FM_INT && Switch->Case[c]->value->node_form != FM_INTREF)	{
				return error(TNMSERR_MA_CASE_TYPE_MISMATCH, Switch->Case[c]->Case->atom);
			}
		}
		else if (Switch->cond->node_form == FM_STR || Switch->cond->node_form == FM_STRREF)	{
			if (Switch->Case[c]->value->node_form != FM_STR && Switch->Case[c]->value->node_form != FM_STRREF)	{
				return error(TNMSERR_MA_CASE_TYPE_MISMATCH, Switch->Case[c]->Case->atom);
			}
		}
		else	{
			return error(TNMSERR_MA_CASE_TYPE_MISMATCH, Switch->Case[c]->Case->atom);
		}

		// <ブロック>
		for (int s = 0; s < (int)Switch->Case[c]->block.size(); s++)	{
			if (!ma_sentence(Switch->Case[c]->block[s]))
				return false;
		}
	}

	// <default 文>
	if (Switch->Default)	{

		// <ブロック>
		for (int s = 0; s < (int)Switch->Default->block.size(); s++)	{
			if (!ma_sentence(Switch->Default->block[s]))
				return false;
		}
	}

	// 型を決定
	Switch->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<代入文>
// ================================================================
// ★型についての注意
//		a = b	→	assign->left->form = (a), assign->right->form = (b)
//		a += b	→	assign->left->form = (a), assign->right->form = (b), assign->equal_form = (a + b)
// ================================================================
bool C_tnms_semantic_analizer::ma_assign(BSP<TNMS_SA_NODE_ASSIGN> assign, bool* ret_exist_sel)
{
	// 左辺
	if (!ma_left(assign->left))
		return false;

	// 右辺
	if (!ma_exp(assign->right, ret_exist_sel))
		return false;

	// 型チェック
	assign->equal_form = assign->right->node_form;
	if (assign->equal->atom.opt != OP_NONE)		// 演算代入（+= など）なら演算後の型を調べる
		assign->equal_form = check_operate_2(assign->left->node_form, assign->right->node_form, assign->equal->atom.opt);

	// 左右の型が一致していなければエラー
	if (false);
	else if (assign->left->node_form == FM_INTREF)	{
		if (assign->equal_form != FM_INT && assign->equal_form != FM_INTREF)
			return error(TNMSERR_MA_ASSIGN_TYPE_NO_MATCH, assign->equal->atom);

		assign->set_flag = false;	// 値代入
		assign->al_id = 1;			// 使わない場合、代入 = 1
	}
	else if (assign->left->node_form == FM_STRREF)	{
		if (assign->equal_form != FM_STR && assign->equal_form != FM_STRREF)
			return error(TNMSERR_MA_ASSIGN_TYPE_NO_MATCH, assign->equal->atom);

		assign->set_flag = false;	// 値代入
		assign->al_id = 1;			// 使わない場合、代入 = 1
	}
	else if (assign->left->node_form == FM_VOID)	{
		return error(TNMSERR_MA_ASSIGN_LEFT_NEED_REFERENCE, assign->equal->atom);
	}
	else if (assign->left->node_form == FM_INT)	{
		return error(TNMSERR_MA_ASSIGN_LEFT_NEED_REFERENCE, assign->equal->atom);
	}
	else if (assign->left->node_form == FM_STR)	{
		return error(TNMSERR_MA_ASSIGN_LEFT_NEED_REFERENCE, assign->equal->atom);
	}
	else	{
		if (assign->left->node_form != assign->equal_form)
			return error(TNMSERR_MA_ASSIGN_TYPE_NO_MATCH, assign->equal->atom);

		assign->set_flag = true;	// 参照代入
		assign->al_id = 1;			// 使わない場合、代入 = 1
	}
#if 0
	else	{

		// __set コマンドを調べる
		// __set(right_form) というコマンドがあれば代入成功なのです。

		// エレメントの情報を取得する
		S_tnms_element* element_info = NULL;
		TSTR element_name = _T("__set");
		if (!piad->form_table.get_element_info(assign->left->node_form, element_name, &element_info))
			return error(TNMSERR_MA_ASSIGN_TYPE_NO_MATCH, assign->equal->atom);

		std::map<int, S_tnms_arg_list>::iterator itr = element_info->arg_map.begin();
		for (; itr != element_info->arg_map.end(); ++itr)	{
			if (itr->second.arg_list.size() == 1 && itr->second.arg_list[0].form == assign->right_form)	{
				assign->set_flag = true;		// __set コマンドを使う
				assign->al_id = itr->first;		// 引数リストID
				break;
			}
		}
		if (!assign->set_flag)
			return error(TNMSERR_MA_ASSIGN_TYPE_NO_MATCH, assign->equal->atom);
	}
#elif 0
	else	{
		return error(TNMSERR_MA_ASSIGN_TYPE_NO_MATCH, assign->equal->atom);
	}
#endif

	// 型を決定
	assign->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<コマンド文>
// ================================================================
bool C_tnms_semantic_analizer::ma_command(BSP<TNMS_SA_NODE_COMMAND> command, bool* ret_exist_sel)
{
	// エレメント式
	if (!ma_elm_exp(command->command, ret_exist_sel))
		return false;

	// 式がプロパティならエラー
	if (command->command->element_type == TNMS_ELEMENT_TYPE_PROPERTY)
		return error(TNMSERR_MA_ELEMENT_IS_PROPERTY, *command->command->elm_list->get_last_atom());

	// 型を決定
	command->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<テキスト文>
// ================================================================
bool C_tnms_semantic_analizer::ma_text(BSP<TNMS_SA_NODE_ATOM> text)
{
	// 型を決定
	text->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<式>
// ================================================================
bool C_tnms_semantic_analizer::ma_exp(BSP<TNMS_SA_NODE_EXP> exp, bool* ret_exist_sel)
{
	if (!ma_exp_sub(exp, ret_exist_sel))	{

		// ここで半角文字列の変換を行う

		// 式の解析に失敗…
		// もし引数なしの単一エレメントなら、無理やり文字列をでっちあげる！
		if (last_error.type != TNMSERR_MA_ELEMENT_UNKNOWN)
			return false;
		if (exp->node_type != TNMS_SA_NODE_TYPE_EXP_SIMPLE)
			return false;
		if (exp->smp_exp->node_type != TNMS_SA_NODE_TYPE_SMP_EXP_ELM_EXP)
			return false;
		if (exp->smp_exp->elm_exp->elm_list->element.size() != 1)
			return false;
		if (!exp->smp_exp->elm_exp->elm_list->element[0]->arg_list->arg.empty())
			return false;

		// エレメントの情報を取得する
		S_tnms_element* element_info = NULL;
		TSTR element_name = plad->unknown_list[exp->smp_exp->elm_exp->elm_list->element[0]->name->atom.opt];

		// ファイル名制限
		for (TSTR::const_iterator itr = element_name.begin(); itr != element_name.end(); ++itr)	{
			if (*itr == _T('@') || *itr == _T('$'))
				return false;
		}

		// LA アトムをでっちあげる
		S_tnms_atom la_atom;
		la_atom.type = TNMS_LA_ATOM_VAL_STR;
		la_atom.opt  = (int)plad->str_list.size();
		plad->str_list.push_back(element_name);

		// SA ノードをでっちあげる（リテラル）
		BSP<TNMS_SA_NODE_ATOM> sa_node_literal(new TNMS_SA_NODE_ATOM);
		sa_node_literal->atom = la_atom;
		sa_node_literal->node_form = FM_STR;

		// SA ノードをでっちあげる（単項式）
		BSP<TNMS_SA_NODE_SMP_EXP> sa_node_smp_exp(new TNMS_SA_NODE_SMP_EXP);
		sa_node_smp_exp->node_type = TNMS_SA_NODE_TYPE_SMP_EXP_LITERAL;
		sa_node_smp_exp->Literal = sa_node_literal;
		sa_node_smp_exp->node_form = FM_STR;

		// SA ノードをでっちあげる（式）
		exp->node_type = TNMS_SA_NODE_TYPE_EXP_SIMPLE;
		exp->smp_exp = sa_node_smp_exp;
		exp->node_form = FM_STR;
		exp->tmp_form = exp->node_form;
	}

	return true;
}

bool C_tnms_semantic_analizer::ma_exp_sub(BSP<TNMS_SA_NODE_EXP> exp, bool* ret_exist_sel)
{
	if (false);
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_SIMPLE)	{	// 単項式
		if (!ma_smp_exp(exp->smp_exp, ret_exist_sel))
			return false;

		// 型を決定
		exp->node_form = exp->smp_exp->node_form;
		exp->tmp_form = exp->node_form;
	}
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_OPR_1)	{		// 単項演算
		if (!ma_exp(exp->exp_1, ret_exist_sel))				// 左式
			return false;

		// 型を決定
		exp->node_form = check_operate_1(exp->exp_1->node_form, exp->opr->atom.opt);
		exp->tmp_form = exp->node_form;

		// 式が型を持たなければエラー
		if (exp->node_form == FM_VOID)
			return error(TNMSERR_MA_EXP_TYPE_NO_MATCH, exp->opr->atom);
	}
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_OPR_2)	{		// 二項演算
		if (!ma_exp(exp->exp_1, ret_exist_sel))				// 左式
			return false;
		if (!ma_exp(exp->exp_2, ret_exist_sel))				// 右式
			return false;

		// 型を決定
		exp->node_form = check_operate_2(exp->exp_1->node_form, exp->exp_2->node_form, exp->opr->atom.opt);
		exp->tmp_form = exp->node_form;

		// 式が型を持たなければエラー
		if (exp->node_form == FM_VOID)
			return error(TNMSERR_MA_EXP_TYPE_NO_MATCH, exp->opr->atom);
	}

	return true;
}

// ****************************************************************
// 意味解析：<式リスト>
// ================================================================
//		[式の個数]	: int
//		[式]		: すべての式
// ================================================================
bool C_tnms_semantic_analizer::ma_exp_list(BSP<TNMS_SA_NODE_EXP_LIST> exp_list, bool* ret_exist_sel)
{
	int exp_cnt = (int)exp_list->exp.size();
	for (int e = 0; e < exp_cnt; e ++)	{
		if (!ma_exp(exp_list->exp[e], ret_exist_sel))
			return false;

		// 個々の型を追加
		exp_list->form_list.push_back(exp_list->exp[e]->node_form);
	}

	// 型を決定
	exp_list->node_form = FM_LIST;

	return true;
}

// ****************************************************************
// 意味解析：<単項式>
// ================================================================
bool C_tnms_semantic_analizer::ma_smp_exp(BSP<TNMS_SA_NODE_SMP_EXP> smp_exp, bool* ret_exist_sel)
{
	if (false);
	// '(' <式> ')'
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_KAKKO)	{
		if (!ma_exp(smp_exp->exp, ret_exist_sel))
			return false;

		// 型を決定
		smp_exp->node_form = smp_exp->exp->node_form;
	}
	// <式リスト>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_EXP_LIST)	{
		if (!ma_exp_list(smp_exp->exp_list, ret_exist_sel))
			return false;

		// 型を決定
		smp_exp->node_form = smp_exp->exp_list->node_form;
	}
	// <goto 式>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_GOTO)	{
		if (!ma_goto_exp(smp_exp->Goto, ret_exist_sel))
			return false;

		// 型を決定
		smp_exp->node_form = smp_exp->Goto->node_form;
	}
	// <エレメント式>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_ELM_EXP)	{
		if (!ma_elm_exp(smp_exp->elm_exp, ret_exist_sel))
			return false;

		// 型を決定
		smp_exp->node_form = smp_exp->elm_exp->node_form;
	}
	// <リテラル>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_LITERAL)	{
		if (!ma_literal(smp_exp->Literal))
			return false;

		// 型を決定
		smp_exp->node_form = smp_exp->Literal->node_form;
	}

	return true;
}

// ****************************************************************
// 意味解析：<左辺>
// ================================================================
bool C_tnms_semantic_analizer::ma_left(BSP<TNMS_SA_NODE_ELM_EXP> left)
{
	// エレメントリスト
	if (!ma_elm_list(left->elm_list, NULL))
		return false;

	// エレメントタイプ
	left->element_type = left->elm_list->element_type;

	// 型を決定
	left->node_form = left->elm_list->node_form;

	return true;
}

// ****************************************************************
// 意味解析：<エレメント式>
// ================================================================
bool C_tnms_semantic_analizer::ma_elm_exp(BSP<TNMS_SA_NODE_ELM_EXP> elm_exp, bool* ret_exist_sel)
{
	// エレメントリストは、
	// ・コマンドの場合：末尾のコマンドエレメントにより、直値に展開されています。
	// ・プロパティの場合：直値が求められている場合は、ここで直値に展開します。

	// 本当は、コマンドもエレメントで返すこともできるようにした方が望ましいです。
	// その場合はリファレンスの概念をしっかり組まないといけません。

	// エレメントリストを解析
	if (!ma_elm_list(elm_exp->elm_list, ret_exist_sel))
		return false;
	
	// エレメント式のタイプの決定
	elm_exp->element_type = elm_exp->elm_list->element_type;

	// 型を決定
	elm_exp->node_form = elm_exp->elm_list->node_form;

	return true;
}

// ****************************************************************
// 意味解析：<エレメントリスト>
// ================================================================
bool C_tnms_semantic_analizer::ma_elm_list(BSP<TNMS_SA_NODE_ELM_LIST> elm_list, bool* ret_exist_sel)
{
	// 先頭エレメントの情報を探す
	int parent_form_code;
	TSTR element_name = plad->unknown_list[elm_list->element[0]->name->atom.opt];
	S_tnms_element* element_info = piad->form_table.find_element_info(element_name, &parent_form_code);
	if (element_info == NULL)
		return error(TNMSERR_MA_ELEMENT_UNKNOWN, elm_list->element[0]->name->atom);

	// スコープ決定（グローバル／シーン／コール）
	elm_list->parent_form_code = parent_form_code;

	// 全てのエレメントを解析
	for (int i = 0; i < (int)elm_list->element.size(); i++)	{
		if (!ma_element(parent_form_code, elm_list->element[i], ret_exist_sel))
			return false;

		// 末尾のエレメントの型がエレメントリストの型になる
		elm_list->node_form = elm_list->element[i]->node_form;
		elm_list->element_type = elm_list->element[i]->element_type;

		// 親の型を変更
		parent_form_code = elm_list->node_form;
	}

	// ★プロパティはデフォルトで参照型
	if (elm_list->element_type == TNMS_ELEMENT_TYPE_PROPERTY)	{
		if (elm_list->node_form == FM_INT)
			elm_list->node_form = FM_INTREF;
		if (elm_list->node_form == FM_STR)
			elm_list->node_form = FM_STRREF;
		if (elm_list->node_form == FM_INTLIST)
			elm_list->node_form = FM_INTLISTREF;
		if (elm_list->node_form == FM_STRLIST)
			elm_list->node_form = FM_STRLISTREF;
	}

	return true;
}

// ****************************************************************
// 意味解析：<エレメント>
// ================================================================
bool C_tnms_semantic_analizer::ma_element(int parent_form_code, BSP<TNMS_SA_NODE_ELEMENT> element, bool* ret_exist_sel)
{
	if (false);

	// 通常のエレメントの場合
	else if (element->node_type == TNMS_SA_NODE_TYPE_ELEMENT_ELEMENT)	{

		// エレメントの情報を取得する
		S_tnms_element* element_info = NULL;
		TSTR element_name = plad->unknown_list[element->name->atom.opt];

		if (!piad->form_table.get_element_info(parent_form_code, element_name, &element_info))
			return error(TNMSERR_MA_ELEMENT_UNKNOWN, element->name->atom);

		// 型を決定
		element->node_form = element_info->form;

		// その他のエレメントの情報を記憶しておく
		element->element_code = element_info->code;
		element->element_type = element_info->type;
		element->element_parent_form = parent_form_code;

		// コマンドの場合
		if (element_info->type == TNMS_ELEMENT_TYPE_COMMAND)	{
			bool is_include_sel = false;	// 引数解析用。ret_*** と混じらないように！

			// 引数リスト
			if (!ma_arg_list(element->arg_list, &is_include_sel))
				return false;

			// 引数の中では選択肢は使えない
			if (is_include_sel)
				return error(TNMSERR_MA_SEL_CANNOT_USE_IN_ARG, element->name->atom);

			// 引数リストの型チェック
			element->arg_list_id = check_arg_list(element, element_info, element->arg_list);
			if (element->arg_list_id < 0)
				return error(TNMSERR_MA_ARG_TYPE_NO_MATCH, element->name->atom);	// エラー：引数の型が合わない

			// ★選択肢ブロックの判定
			if ((element->element_parent_form == FM_GLOBAL && element->element_code == ELM_GLOBAL_SEL)
				|| (element->element_parent_form == FM_GLOBAL && element->element_code == ELM_GLOBAL_SEL_CANCEL)
				|| (element->element_parent_form == FM_GLOBAL && element->element_code == ELM_GLOBAL_SELMSG)
				|| (element->element_parent_form == FM_GLOBAL && element->element_code == ELM_GLOBAL_SELMSG_CANCEL)
				|| (element->element_parent_form == FM_GLOBAL && element->element_code == ELM_GLOBAL_SEL_IMAGE)
				)
			{
				if (ret_exist_sel)	{
					*ret_exist_sel = true;
				}
			}
		}
	}

	// 添え字の場合
	else if (element->node_type == TNMS_SA_NODE_TYPE_ELEMENT_ARRAY)	{
		bool is_include_sel = false;

		// エレメントの情報を取得する
		S_tnms_element* element_info = NULL;
		TSTR element_name = _T("array");
		if (!piad->form_table.get_element_info(parent_form_code, element_name, &element_info))
			return error(TNMSERR_MA_ELEMENT_ILLEGAL_ARRAY, element->open_b->atom);

		// 型を決定
		element->node_form = element_info->form;

		// その他のエレメントの情報を記憶しておく
		element->element_code = element_info->code;
		element->element_type = element_info->type;
		element->element_parent_form = parent_form_code;

		// 添え字を解析
		if (!ma_exp(element->exp, &is_include_sel))
			return false;

		// 添え字の中では選択肢は使えない
		if (is_include_sel)
			return error(TNMSERR_MA_SEL_CANNOT_USE_IN_INDEX, *element->exp->get_first_atom());

		// 添え字は整数
		if (element->exp->node_form != FM_INT && element->exp->node_form != FM_INTREF)
			return error(TNMSERR_MA_INDEX_NOT_INT, *element->exp->get_first_atom());
	}

	return true;
}

// ****************************************************************
// 意味解析：<型>
// ================================================================
bool C_tnms_semantic_analizer::ma_form(BSP<TNMS_SA_NODE_FORM> form)
{
	// 添え字
	if (form->index)	{

		if (!ma_exp(form->index, NULL))
			return false;

		// 添え字は整数でなければいけない！
		if (form->index->node_form != FM_INT)
			return error(TNMSERR_MA_DEF_PROP_NOT_INT, form->open_b->atom);
	}

	// 型を決定
	form->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<引数リスト>
// ================================================================
//	ex.	(1, "abc")
//		<式>		// 1
//		<式>		// "abc"
// ================================================================
bool C_tnms_semantic_analizer::ma_arg_list(BSP<TNMS_SA_NODE_ARG_LIST> arg_list, bool* ret_exist_sel)
{
	int arg_cnt = (int)arg_list->arg.size();

	// 引数リスト
	for (int a = 0; a < arg_cnt; a ++)	{
		if (!ma_arg(arg_list->arg[a], ret_exist_sel))
			return false;
	}

	// 型を決定
	arg_list->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<引数>
// ================================================================
//	ex.	(type=10, time=1000)
// ================================================================
bool C_tnms_semantic_analizer::ma_arg(BSP<TNMS_SA_NODE_ARG> arg, bool* ret_exist_sel)
{
	if (!ma_exp(arg->exp, ret_exist_sel))
		return false;

	// 型を決定
	arg->node_form = arg->exp->node_form;

	return true;
}

// ****************************************************************
// 意味解析：引数リストの型チェック
// ================================================================
int C_tnms_semantic_analizer::check_arg_list(BSP<TNMS_SA_NODE_ELEMENT> element, S_tnms_element* element_info, BSP<TNMS_SA_NODE_ARG_LIST> real_arg_list)
{
	for (std::map<int, S_tnms_arg_list>::iterator itr = element_info->arg_map.begin(); itr != element_info->arg_map.end(); ++itr)	{

		// まずは名前なし引数の解析を行う
		if (itr->first != -1 && check_no_named_arg_list(itr->second, real_arg_list))	{

			// 名前なし引数の解析に成功したので、引数リストIDはここで確定

			// 名前つき引数を解析
			if (!check_named_arg_list(element, element_info, real_arg_list))
				return -1;	// 失敗

			return itr->first;	// 引数リストIDを返す
		}
	}

	// 失敗
	return -1;
}

// ****************************************************************
// 意味解析：引数リストの型チェック
// ================================================================
bool C_tnms_semantic_analizer::check_no_named_arg_list(S_tnms_arg_list& temp_arg_list, BSP<TNMS_SA_NODE_ARG_LIST> real_arg_list)
{
	// 書き換え用の引数リストを作る
	ARRAY<int> real_arg_form_list(real_arg_list->arg.size());
	for (int i = 0; i < (int)real_arg_list->arg.size(); i++)
		real_arg_form_list[i] = real_arg_list->arg[i]->exp->tmp_form;

	ARRAY<S_tnms_arg>::iterator t_itr = temp_arg_list.arg_list.begin();							// 仮引数
	ARRAY<S_tnms_arg>::iterator t_itr_end = temp_arg_list.arg_list.end();						// 仮引数
	ARRAY<int>::iterator r_itr = real_arg_form_list.begin();									// 実引数
	ARRAY<int>::iterator r_itr_end = real_arg_form_list.end() - real_arg_list->named_arg_cnt;	// 実引数

	while (1)	{

		// 仮引数が終了した場合
		if (t_itr == t_itr_end)	{

			// 実引数も終了したなら成功
			if (r_itr == r_itr_end)
				break;

			// 仮引数が終了したのに実引数がまだあれば失敗
			if (r_itr != r_itr_end)
				return false;
		}

		// 仮引数がまだある場合
		else	{

			// 仮引数が __args なら成功
			if (t_itr->form == FM___ARGS)	{

				// 残りの実引数を全て値にする（値になれるかどうかの判定は BS で行う）
				for (ARRAY<int>::iterator itr = r_itr; itr != r_itr_end; ++itr)	{
					if (*itr == FM_INTREF)
						*itr = FM_INT;
					else if (*itr == FM_STRREF)
						*itr = FM_STR;
				}
				break;
			}

			// 仮引数が __argsref なら成功
			else if (t_itr->form == FM___ARGSREF)	{

				// 残りの実引数を全て参照にする（参照になれるかどうかの判定は BS で行う）
				// 参照になれない例：100、"abc" などの直値
				for (ARRAY<int>::iterator itr = r_itr; itr != r_itr_end; ++itr)	{
					if (*itr == FM_INT)
						*itr = FM_INTREF;
					else if (*itr == FM_STR)
						*itr = FM_STRREF;
				}
				break;
			}

			// 実引数が終了した場合
			else if (r_itr == r_itr_end)	{

				// 仮引数がデフォルト引数を持っているなら成功
				if (t_itr->def_exist)
					break;

				// 仮引数がまだあるので失敗
				else
					return false;
			}

			// 仮引数も実引数もまだある場合
			else	{

				// 仮引数と実引数の整合性チェック
				if (t_itr->form != *r_itr)	{

					// 仮引数として値が求められているのに参照だった場合
					if (false);
					else if (t_itr->form == FM_INT && *r_itr == FM_INTREF)	{
						*r_itr = FM_INT;		// 実引数を値にする（値になれるかどうかの判定は BS で行う）
					}
					else if (t_itr->form == FM_STR && *r_itr == FM_STRREF)	{
						*r_itr = FM_STR;		// 実引数を値にする（値になれるかどうかの判定は BS で行う）
					}
					// それ以外で型が違えば失敗
					else	{
						return false;
					}
				}
			}
		}

		++ t_itr;
		++ r_itr;
	}

	// 成功した引数リストに更新（参照などが変わっている可能性がある）
	for (int i = 0; i < (int)real_arg_list->arg.size(); i++)
		real_arg_list->arg[i]->exp->tmp_form = real_arg_form_list[i];

	return true;
}

// ****************************************************************
// 意味解析：名前引数リストの型チェック
// ================================================================
bool C_tnms_semantic_analizer::check_named_arg_list(BSP<TNMS_SA_NODE_ELEMENT> element, S_tnms_element* element_info, BSP<TNMS_SA_NODE_ARG_LIST> real_arg_list)
{
	if (real_arg_list->named_arg_cnt == 0)
		return true;	// 名前つき引数が存在しないので問題なし

	// 名前つき引数があった場合は、名前つき仮引数があることを確認する
	std::map<int, S_tnms_arg_list>::iterator tmp_arg_list = element_info->arg_map.find(-1);	// -1: 名前つき引数リスト
	if (tmp_arg_list == element_info->arg_map.end())
		return error(TNMSERR_MA_CMD_NO_NAMED_ARG_LIST, element->name->atom);	// 名前つきの仮引数リストが見つからない

	int real_named_arg_cnt = real_arg_list->named_arg_cnt;
	ARRAY< BSP<TNMS_SA_NODE_ARG> >::iterator real_named_arg_itr_begin = real_arg_list->arg.end() - real_named_arg_cnt;
	ARRAY< BSP<TNMS_SA_NODE_ARG> >::iterator real_named_arg_itr_end = real_arg_list->arg.end();

	// 実引数の型リスト（書き換え用）を作る
	ARRAY<int> real_arg_form_list(real_named_arg_cnt);
	for (int i = 0; i < real_named_arg_cnt; i++)
		real_arg_form_list[i] = (*(real_named_arg_itr_begin + i))->exp->tmp_form;

	ARRAY< BSP<TNMS_SA_NODE_ARG> >::iterator real_named_arg_itr = real_named_arg_itr_begin;
	ARRAY<int>::iterator real_named_arg_form_itr = real_arg_form_list.begin();
	for (; real_named_arg_itr < real_named_arg_itr_end; ++real_named_arg_itr, ++real_named_arg_form_itr)	{

		TSTR real_arg_name = plad->unknown_list[(*real_named_arg_itr)->name->atom.opt];

		// 実引数で与えられた名前を仮引数から探す
		int tmp_arg_no = -1;
		int tmp_arg_cnt = (int)tmp_arg_list->second.arg_list.size();
		for (int ta = 0; ta < tmp_arg_cnt; ta++)	{
			if (real_arg_name == tmp_arg_list->second.arg_list[ta].name)	{
				tmp_arg_no = ta;
				break;
			}
		}
		if (tmp_arg_no == -1)
			return error(TNMSERR_MA_CMD_ILLEGAL_NAMED_ARG, (*real_named_arg_itr)->name->atom);	// 指定された名前が見つからない

		int t_form = tmp_arg_list->second.arg_list[tmp_arg_no].form;	// 仮引数の型
		int r_form = *real_named_arg_form_itr;							// 実引数の型

		// 型チェック
		if (r_form != t_form)	{

			// 仮引数として値が求められているのに参照だった場合
			if (false);
			else if (t_form == FM_INT && r_form == FM_INTREF)	{
				*real_named_arg_form_itr = FM_INT;		// 実引数を値にする（値になれるかどうかの判定は BS で行う）
			}
			else if (t_form == FM_STR && r_form == FM_STRREF)	{
				*real_named_arg_form_itr = FM_STR;		// 実引数を値にする（値になれるかどうかの判定は BS で行う）
			}
			// それ以外で型が違えば失敗
			else	{
				return error(TNMSERR_MA_ARG_TYPE_NO_MATCH, (*real_named_arg_itr)->name->atom);	// 型が合わない
			}
		}

		// 引数 id が確定
		(*real_named_arg_itr)->name_id = tmp_arg_list->second.arg_list[tmp_arg_no].id;
	}

	// 成功した引数リストに更新（参照などが変わっている可能性がある）
	for (int i = 0; i < real_named_arg_cnt; i++)
		(*(real_named_arg_itr_begin + i))->exp->tmp_form = real_arg_form_list[i];

	return true;
}

// ****************************************************************
// 意味解析：<名前>
// ================================================================
bool C_tnms_semantic_analizer::ma_name(BSP<TNMS_SA_NODE_NAME> name)
{
	if (!ma_literal(name->name))
		return false;

	// 型を決定
	name->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<リテラル>
// ================================================================
bool C_tnms_semantic_analizer::ma_literal(BSP<TNMS_SA_NODE_ATOM> Literal)
{
	// 型を決定
	if (false);
	else if (Literal->atom.type == TNMS_LA_ATOM_VAL_INT)	Literal->node_form = FM_INT;
	else if (Literal->atom.type == TNMS_LA_ATOM_VAL_STR)	Literal->node_form = FM_STR;
	else if (Literal->atom.type == TNMS_LA_ATOM_LABEL)		Literal->node_form = FM_LABEL;

	return true;
}

// ****************************************************************
// 意味解析：<代入演算子>
// ================================================================
bool C_tnms_semantic_analizer::ma_assign_operator(BSP<TNMS_SA_NODE_ATOM> opr)
{
	// 型を決定
	opr->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<単項演算子>
// ================================================================
bool C_tnms_semantic_analizer::ma_operator_1(BSP<TNMS_SA_NODE_ATOM> opr)
{
	// 型を決定
	opr->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<二項演算子>
// ================================================================
bool C_tnms_semantic_analizer::ma_operator_2(BSP<TNMS_SA_NODE_ATOM> opr)
{
	// 型を決定
	opr->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// 意味解析：<終端>
// ================================================================
bool C_tnms_semantic_analizer::ma_eof(BSP<TNMS_SA_NODE_ATOM> eof)
{
	// 型を決定
	eof->node_form = FM_VOID;

	return true;
}


// ****************************************************************
// 意味解析：単項演算の型をチェック
// ================================================================
int C_tnms_semantic_analizer::check_operate_1(int r_form, BYTE opr)
{
	int ret_form = FM_VOID;

	// 単項演算は int 型に対してしか定義されていない
	// 演算後は必ず int 型になる
	if (r_form == FM_INT || r_form == FM_INTREF)		ret_form = FM_INT;

	return ret_form;
}

// ****************************************************************
// 意味解析：二項演算の型をチェック
// ================================================================
int C_tnms_semantic_analizer::check_operate_2(int l_form, int r_form, BYTE opr)
{
	int ret_form = FM_VOID;

	if (false);
	else if ((l_form == FM_INT || l_form == FM_INTREF) && (r_form == FM_INT || r_form == FM_INTREF))	{	// 整数 op 整数
		if (false);
		else if (opr == OP_PLUS)			ret_form = FM_INT;		// (整数 +  整数) = 整数
		else if (opr == OP_MINUS)			ret_form = FM_INT;		// (整数 -  整数) = 整数
		else if (opr == OP_MULTIPLE)		ret_form = FM_INT;		// (整数 *  整数) = 整数
		else if (opr == OP_DIVIDE)			ret_form = FM_INT;		// (整数 /  整数) = 整数
		else if (opr == OP_AMARI)			ret_form = FM_INT;		// (整数 %  整数) = 整数
		else if (opr == OP_EQUAL)			ret_form = FM_INT;		// (整数 == 整数) = 整数
		else if (opr == OP_NOT_EQUAL)		ret_form = FM_INT;		// (整数 != 整数) = 整数
		else if (opr == OP_GREATER)			ret_form = FM_INT;		// (整数 >  整数) = 整数
		else if (opr == OP_GREATER_EQUAL)	ret_form = FM_INT;		// (整数 >= 整数) = 整数
		else if (opr == OP_LESS)			ret_form = FM_INT;		// (整数 <  整数) = 整数
		else if (opr == OP_LESS_EQUAL)		ret_form = FM_INT;		// (整数 <= 整数) = 整数
		else if (opr == OP_LOGICAL_AND)		ret_form = FM_INT;		// (整数 && 整数) = 整数
		else if (opr == OP_LOGICAL_OR)		ret_form = FM_INT;		// (整数 || 整数) = 整数

		else if (opr == OP_AND)				ret_form = FM_INT;		// (整数 &  整数) = 整数
		else if (opr == OP_OR)				ret_form = FM_INT;		// (整数 |  整数) = 整数
		else if (opr == OP_HAT)				ret_form = FM_INT;		// (整数 ^  整数) = 整数
		else if (opr == OP_SL)				ret_form = FM_INT;		// (整数 << 整数) = 整数
		else if (opr == OP_SR)				ret_form = FM_INT;		// (整数 >> 整数) = 整数
		else if (opr == OP_SR3)				ret_form = FM_INT;		// (整数 >>> 整数) = 整数
	}
	else if ((l_form == FM_STR || l_form == FM_STRREF) && (r_form == FM_STR || r_form == FM_STRREF))	{	// 文字列 op 文字列
		if (false);
		else if (opr == OP_PLUS)			ret_form = FM_STR;		// (文字列 +  文字列) = 文字列
		else if (opr == OP_EQUAL)			ret_form = FM_INT;		// (文字列 == 文字列) = 整数
		else if (opr == OP_NOT_EQUAL)		ret_form = FM_INT;		// (文字列 != 文字列) = 整数
		else if (opr == OP_GREATER)			ret_form = FM_INT;		// (文字列 >  文字列) = 整数
		else if (opr == OP_GREATER_EQUAL)	ret_form = FM_INT;		// (文字列 >= 文字列) = 整数
		else if (opr == OP_LESS)			ret_form = FM_INT;		// (文字列 <  文字列) = 整数
		else if (opr == OP_LESS_EQUAL)		ret_form = FM_INT;		// (文字列 <= 文字列) = 整数
	}
	else if ((l_form == FM_STR || l_form == FM_STRREF) && (r_form == FM_INT || r_form == FM_INTREF))	{	// 文字列 op 整数
		if (false);
		else if (opr == OP_MULTIPLE)		ret_form = FM_STR;		// (文字列 *  整数) = 文字列
	}

	return ret_form;
}

// ****************************************************************
// 意味解析エラー
// ================================================================
bool C_tnms_semantic_analizer::error(TNMSERR_MA type, S_tnms_atom atom)
{
	last_error.type = type;
	last_error.atom = atom;

	return false;
}

void C_tnms_semantic_analizer::clear_error()
{
	last_error.type = TNMSERR_MA_NONE;
}

S_tnms_atom C_tnms_semantic_analizer::get_error_atom()
{
	return last_error.atom;
}

int C_tnms_semantic_analizer::get_error_line()
{
	return last_error.atom.line;
}

TSTR C_tnms_semantic_analizer::get_error_str()
{
	// エラーの設定
	switch (last_error.type)	{
		case TNMSERR_MA_DEF_PROP_NOT_INT:			return _T("添え字の型は整数でなければいけません。");
		case TNMSERR_MA_IF_COND_IS_NOT_INT:			return _T("if 文の条件式は整数でなければいけません。");
		case TNMSERR_MA_FOR_COND_IS_NOT_INT:		return _T("for 文の条件式は整数でなければいけません。");
		case TNMSERR_MA_WHILE_COND_IS_NOT_INT:		return _T("while 文の条件式は整数でなければいけません。");
		case TNMSERR_MA_CASE_TYPE_MISMATCH:			return _T("case 文の型が switch 文の条件式の型と一致しません。");
		case TNMSERR_MA_ASSIGN_LEFT_NEED_REFERENCE:	return _T("代入文の左辺は整数か文字列の変数である必要があります。");
		case TNMSERR_MA_ASSIGN_TYPE_NO_MATCH:		return _T("代入文の左右の型が一致しません。");
		case TNMSERR_MA_EXP_TYPE_NO_MATCH:			return _T("演算の型が一致しません。");
		case TNMSERR_MA_ELEMENT_UNKNOWN:			return _T("未定義のエレメントです。");
		case TNMSERR_MA_ELEMENT_IS_PROPERTY:		return _T("このエレメントはコマンドではありません。");
		case TNMSERR_MA_ELEMENT_ILLEGAL_ARRAY:		return _T("'[' が見つかりましたが、このエレメントは配列ではありません。");
		case TNMSERR_MA_INDEX_NOT_INT:				return _T("配列の添え字は整数でなければいけません。");
		case TNMSERR_MA_ARG_TYPE_NO_MATCH:			return _T("コマンドの引数の型が合いません。");
		case TNMSERR_MA_CMD_NO_NAMED_ARG_LIST:		return _T("このコマンドは名前つき引数を受け取りません。");
		case TNMSERR_MA_CMD_ILLEGAL_NAMED_ARG:		return _T("不明な名前つき引数です。");
		case TNMSERR_MA_SEL_CANNOT_USE_IN_COND:		return _T("条件式の中では sel コマンドは使えません。");
		case TNMSERR_MA_SEL_CANNOT_USE_IN_ARG:		return _T("コマンドの引数に sel コマンドを使うことはできません。");
		case TNMSERR_MA_SEL_CANNOT_USE_IN_GOTO:		return _T("gosub の引数に sel コマンドを使うことはできません。");
		case TNMSERR_MA_SEL_CANNOT_USE_IN_RETURN:	return _T("return の引数に sel コマンドを使うことはできません。");
		case TNMSERR_MA_SEL_CANNOT_USE_IN_INDEX:	return _T("配列の添え字に sel コマンドを使うことはできません。");
		case TNMSERR_MA_PROPERTY_OUT_OF_COMMAND:	return _T("ユーザープロパティがユーザーコマンドの外で定義されています。");
	}

	return _T("意味解析：不明なエラーです。");
}

