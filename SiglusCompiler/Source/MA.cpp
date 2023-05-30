#include	"pch.h"
#include	"code.h"
#include	"MA.h"

// ****************************************************************
// �Ӗ���� - Semantic Analizer -
// ================================================================
bool C_tnms_semantic_analizer::analize(S_tnms_ia_data* piad_, S_tnms_la_data* plad_, S_tnms_sa_data* psad_)
{
	piad = piad_;
	plad = plad_;
	psad = psad_;

	// ������
	psad->command_in = 0;

	// ss
	if (!ma_ss(psad->root))
		return false;

	return true;
}

// ****************************************************************
// �Ӗ���́F<ss>
// ================================================================
bool C_tnms_semantic_analizer::ma_ss(BSP<TNMS_SA_NODE_SS> ss)
{
	// <��*>
	for (int s = 0; s < (int)ss->sentense_list.size(); s++)	{
		if (!ma_sentence(ss->sentense_list[s]))
			return false;
	}

	// �^������
	ss->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�u���b�N>
// ================================================================
//		�Q�d�ɂł��܂���B�i�R�[���v���p�e�B�̔j���̃^�C�~���O�̖��j
//		�Ƃ肠�����͂Q�d�ɂȂ�Ȃ��R�}���h�̐錾�Ȃǂł̂ݎg�p���Ă��������B
// ================================================================
bool C_tnms_semantic_analizer::ma_block(BSP<TNMS_SA_NODE_BLOCK> block)
{
	// <��*>
	for (int s = 0; s < (int)block->sentense_list.size(); s++)	{
		if (!ma_sentence(block->sentense_list[s]))
			return false;
	}

	// �R�[���v���p�e�B���N���A�i������Ԃɖ߂��j
	*piad->form_table.get_form_by_code(FM_CALL) = piad->form_table.form_call;
	psad->cur_call_prop_cnt = 0;

	// �^������
	block->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<��>
// ================================================================
bool C_tnms_semantic_analizer::ma_sentence(BSP<TNMS_SA_NODE_SENTENCE> sentence)
{
	bool ret = false;
	bool is_exist_sel = false;

	// <��>
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

	// ���������ꍇ�̏���
	if (ret)	{

		// �^������
		sentence->node_form = FM_VOID;
		// �I�������܂ޕ�
		sentence->is_include_sel = is_exist_sel;
		//if (is_exist_sel)	{
		//	info_box(_T("MA"));
		//}
	}

	return ret;
}

// ****************************************************************
// �Ӗ���́F<���x��>
// ================================================================
bool C_tnms_semantic_analizer::ma_label(BSP<TNMS_SA_NODE_LABEL> label)
{
	// �^������
	label->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�y���x��>
// ================================================================
bool C_tnms_semantic_analizer::ma_z_label(BSP<TNMS_SA_NODE_Z_LABEL> z_label)
{
	// �^������
	z_label->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�v���p�e�B��`��>
// ================================================================
bool C_tnms_semantic_analizer::ma_def_prop(BSP<TNMS_SA_NODE_DEF_PROP> def_prop)
{
	// �R�}���h�̒�����Ȃ���΃G���[
	if (psad->command_in == 0)
	{
		return error(TNMSERR_MA_PROPERTY_OUT_OF_COMMAND, def_prop->Property->atom);
	}

	// �^������
	def_prop->node_form = FM_VOID;

	// �^
	if (def_prop->form)	{
		if (!ma_form(def_prop->form))
			return false;
	}

	// �R�[���v���p�e�B��ǉ�
	S_tnms_element element;
	element.type = TNMS_ELEMENT_TYPE_PROPERTY;
	element.code = create_elm_code(ELM_OWNER_CALL_PROP, 0, psad->cur_call_prop_cnt);
	element.form = def_prop->form_code;
	element.name = plad->unknown_list[def_prop->name->atom.opt];
	piad->form_table.get_form_by_code(FM_CALL)->add_element(&element);

	// �R�[���v���p�e�B�h�c����i�f�o�b�O���̖��O�����Ɏg���j
	def_prop->prop_id = psad->total_call_prop_cnt;
	psad->call_prop_name_list.push_back(element.name);

	// �v���p�e�B�����C���N�������g
	psad->cur_call_prop_cnt ++;
	psad->total_call_prop_cnt ++;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�R�}���h��`��>
// ================================================================
bool C_tnms_semantic_analizer::ma_def_cmd(BSP<TNMS_SA_NODE_DEF_CMD> def_cmd)
{
	// �R�}���h�̒��ɓ���i�v���p�e�B���錾�ł���悤�ɂȂ�j
	psad->command_in = 1;

	// <�v���p�e�B�錾>
	for (int p = 0; p < (int)def_cmd->prop_list.size(); p++)	{
		if (!ma_def_prop(def_cmd->prop_list[p]))
			return false;
	}

	// <�u���b�N>
	if (!ma_block(def_cmd->block))
		return false;

	// �R�}���h����E�o
	psad->command_in = 0;

	// �^������
	def_cmd->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<goto ���^gosub ��>
// ================================================================
bool C_tnms_semantic_analizer::ma_goto(BSP<TNMS_SA_NODE_GOTO> Goto)
{
	bool is_include_sel = false;

	// gosub ��
	if (Goto->node_type != TNMS_SA_NODE_TYPE_GOTO_GOTO)	{

		// gosub ��
		if (!ma_goto_exp(Goto, &is_include_sel))
			return false;

		// gosub �̒��ł͑I�����͎g���Ȃ�
		if (is_include_sel)
			return error(TNMSERR_MA_SEL_CANNOT_USE_IN_GOTO, Goto->Goto->atom);
	}

	// �^������
	Goto->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<goto�^gosub ��>
// ================================================================
bool C_tnms_semantic_analizer::ma_goto_exp(BSP<TNMS_SA_NODE_GOTO> Goto, bool* ret_exist_sel)
{
	// ����
	if (!ma_arg_list(Goto->arg_list, ret_exist_sel))
		return false;

	// �^������
	if (false);
	else if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOTO)		{
		Goto->node_form = FM_VOID;		// goto �̖߂�l�� void
	}
	else if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOSUB)	{
		Goto->node_form = FM_INT;		// gosub �̖߂�l�� int
	}
	else if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOSUBSTR)	{
		Goto->node_form = FM_STR;		// gosubstr �̖߂�l�� str
	}
	else
		return false;

	return true;
}

// ****************************************************************
// �Ӗ���́F<return ��>
// ================================================================
bool C_tnms_semantic_analizer::ma_return(BSP<TNMS_SA_NODE_RETURN> Return, bool* ret_exist_sel)
{
	// ��������̏ꍇ
	if (Return->node_type == TNMS_SA_NODE_TYPE_RETURN_WITH_ARG)	{

		if (!ma_exp(Return->exp, ret_exist_sel))
			return false;
	}

	// �^������
	Return->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<if ��>
// ================================================================
bool C_tnms_semantic_analizer::ma_if(BSP<TNMS_SA_NODE_IF> If)
{
	bool is_include_sel = false;

	int if_cnt = (int)If->sub.size();
	for (int i = 0; i < if_cnt; i++)	{

		// "if" | "elseif"
		if (If->sub[i].If->atom.type == TNMS_LA_ATOM_IF || If->sub[i].If->atom.type == TNMS_LA_ATOM_ELSEIF)	{

			// <������>
			if (!ma_exp(If->sub[i].cond, &is_include_sel))
				return false;

			// �������͐���
			if (If->sub[i].cond->node_form != FM_INT && If->sub[i].cond->node_form != FM_INTREF)
				return error(TNMSERR_MA_IF_COND_IS_NOT_INT, If->sub[i].If->atom);

			// �������̒��ł͑I�����͎g���Ȃ�
			if (is_include_sel)
				return error(TNMSERR_MA_SEL_CANNOT_USE_IN_COND, If->sub[i].If->atom);
		}

		// <�u���b�N>
		for (int s = 0; s < (int)If->sub[i].block.size(); s++)	{
			if (!ma_sentence(If->sub[i].block[s]))
				return false;
		}
	}

	// �^������
	If->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<for ��>
// ================================================================
bool C_tnms_semantic_analizer::ma_for(BSP<TNMS_SA_NODE_FOR> For)
{
	bool is_include_sel = false;

	// <��������>
	for (int s = 0; s < (int)For->init.size(); s++)	{
		if (!ma_sentence(For->init[s]))
			return false;
	}

	// <������>
	if (!ma_exp(For->cond, &is_include_sel))
		return false;

	// �������̒��ł͑I�����͎g���Ȃ�
	if (is_include_sel)
		return error(TNMSERR_MA_SEL_CANNOT_USE_IN_COND, For->For->atom);

	// �������͐���
	if (For->cond->node_form != FM_INT && For->cond->node_form != FM_INTREF)
		return error(TNMSERR_MA_FOR_COND_IS_NOT_INT, For->For->atom);

	// <���[�v��>
	for (int s = 0; s < (int)For->loop.size(); s++)	{
		if (!ma_sentence(For->loop[s]))
			return false;
	}

	// <�u���b�N>
	for (int s = 0; s < (int)For->block.size(); s++)	{
		if (!ma_sentence(For->block[s]))
			return false;
	}

	// �^������
	For->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<while ��>
// ================================================================
bool C_tnms_semantic_analizer::ma_while(BSP<TNMS_SA_NODE_WHILE> While)
{
	bool is_include_sel = false;

	// <������>
	if (!ma_exp(While->cond, &is_include_sel))
		return false;

	// �������̒��ł͑I�����͎g���Ȃ�
	if (is_include_sel)
		return error(TNMSERR_MA_SEL_CANNOT_USE_IN_COND, While->While->atom);

	// �������͐���
	if (While->cond->node_form != FM_INT && While->cond->node_form != FM_INTREF)
		return error(TNMSERR_MA_WHILE_COND_IS_NOT_INT, While->While->atom);

	// <�u���b�N>
	for (int s = 0; s < (int)While->block.size(); s++)	{
		if (!ma_sentence(While->block[s]))
			return false;
	}

	// �^������
	While->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<continue ��>
// ================================================================
bool C_tnms_semantic_analizer::ma_continue(BSP<TNMS_SA_NODE_CONTINUE> Continue)
{
	// �^������
	Continue->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<break ��>
// ================================================================
bool C_tnms_semantic_analizer::ma_break(BSP<TNMS_SA_NODE_BREAK> Break)
{
	// �^������
	Break->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<switch ��>
// ================================================================
bool C_tnms_semantic_analizer::ma_switch(BSP<TNMS_SA_NODE_SWITCH> Switch)
{
	bool is_include_sel = false;

	// <������>
	if (!ma_exp(Switch->cond, &is_include_sel))
		return false;

	// �������̒��ł͑I�����͎g���Ȃ�
	if (is_include_sel)
		return error(TNMSERR_MA_SEL_CANNOT_USE_IN_COND, Switch->Switch->atom);

	// <case ��*>
	for (int c = 0; c < (int)Switch->Case.size(); c++)	{

		// <��r�l>
		if (!ma_exp(Switch->Case[c]->value, &is_include_sel))
			return false;

		// �������̒��ł͑I�����͎g���Ȃ�
		if (is_include_sel)
			return error(TNMSERR_MA_SEL_CANNOT_USE_IN_COND, Switch->Case[c]->Case->atom);

		// case ���̔�r�l�� switch ���̏������̌^�͓�����
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

		// <�u���b�N>
		for (int s = 0; s < (int)Switch->Case[c]->block.size(); s++)	{
			if (!ma_sentence(Switch->Case[c]->block[s]))
				return false;
		}
	}

	// <default ��>
	if (Switch->Default)	{

		// <�u���b�N>
		for (int s = 0; s < (int)Switch->Default->block.size(); s++)	{
			if (!ma_sentence(Switch->Default->block[s]))
				return false;
		}
	}

	// �^������
	Switch->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�����>
// ================================================================
// ���^�ɂ��Ă̒���
//		a = b	��	assign->left->form = (a), assign->right->form = (b)
//		a += b	��	assign->left->form = (a), assign->right->form = (b), assign->equal_form = (a + b)
// ================================================================
bool C_tnms_semantic_analizer::ma_assign(BSP<TNMS_SA_NODE_ASSIGN> assign, bool* ret_exist_sel)
{
	// ����
	if (!ma_left(assign->left))
		return false;

	// �E��
	if (!ma_exp(assign->right, ret_exist_sel))
		return false;

	// �^�`�F�b�N
	assign->equal_form = assign->right->node_form;
	if (assign->equal->atom.opt != OP_NONE)		// ���Z����i+= �Ȃǁj�Ȃ牉�Z��̌^�𒲂ׂ�
		assign->equal_form = check_operate_2(assign->left->node_form, assign->right->node_form, assign->equal->atom.opt);

	// ���E�̌^����v���Ă��Ȃ���΃G���[
	if (false);
	else if (assign->left->node_form == FM_INTREF)	{
		if (assign->equal_form != FM_INT && assign->equal_form != FM_INTREF)
			return error(TNMSERR_MA_ASSIGN_TYPE_NO_MATCH, assign->equal->atom);

		assign->set_flag = false;	// �l���
		assign->al_id = 1;			// �g��Ȃ��ꍇ�A��� = 1
	}
	else if (assign->left->node_form == FM_STRREF)	{
		if (assign->equal_form != FM_STR && assign->equal_form != FM_STRREF)
			return error(TNMSERR_MA_ASSIGN_TYPE_NO_MATCH, assign->equal->atom);

		assign->set_flag = false;	// �l���
		assign->al_id = 1;			// �g��Ȃ��ꍇ�A��� = 1
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

		assign->set_flag = true;	// �Q�Ƒ��
		assign->al_id = 1;			// �g��Ȃ��ꍇ�A��� = 1
	}
#if 0
	else	{

		// __set �R�}���h�𒲂ׂ�
		// __set(right_form) �Ƃ����R�}���h������Α�������Ȃ̂ł��B

		// �G�������g�̏����擾����
		S_tnms_element* element_info = NULL;
		TSTR element_name = _T("__set");
		if (!piad->form_table.get_element_info(assign->left->node_form, element_name, &element_info))
			return error(TNMSERR_MA_ASSIGN_TYPE_NO_MATCH, assign->equal->atom);

		std::map<int, S_tnms_arg_list>::iterator itr = element_info->arg_map.begin();
		for (; itr != element_info->arg_map.end(); ++itr)	{
			if (itr->second.arg_list.size() == 1 && itr->second.arg_list[0].form == assign->right_form)	{
				assign->set_flag = true;		// __set �R�}���h���g��
				assign->al_id = itr->first;		// �������X�gID
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

	// �^������
	assign->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�R�}���h��>
// ================================================================
bool C_tnms_semantic_analizer::ma_command(BSP<TNMS_SA_NODE_COMMAND> command, bool* ret_exist_sel)
{
	// �G�������g��
	if (!ma_elm_exp(command->command, ret_exist_sel))
		return false;

	// �����v���p�e�B�Ȃ�G���[
	if (command->command->element_type == TNMS_ELEMENT_TYPE_PROPERTY)
		return error(TNMSERR_MA_ELEMENT_IS_PROPERTY, *command->command->elm_list->get_last_atom());

	// �^������
	command->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�e�L�X�g��>
// ================================================================
bool C_tnms_semantic_analizer::ma_text(BSP<TNMS_SA_NODE_ATOM> text)
{
	// �^������
	text->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<��>
// ================================================================
bool C_tnms_semantic_analizer::ma_exp(BSP<TNMS_SA_NODE_EXP> exp, bool* ret_exist_sel)
{
	if (!ma_exp_sub(exp, ret_exist_sel))	{

		// �����Ŕ��p������̕ϊ����s��

		// ���̉�͂Ɏ��s�c
		// ���������Ȃ��̒P��G�������g�Ȃ�A������蕶������ł���������I
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

		// �G�������g�̏����擾����
		S_tnms_element* element_info = NULL;
		TSTR element_name = plad->unknown_list[exp->smp_exp->elm_exp->elm_list->element[0]->name->atom.opt];

		// �t�@�C��������
		for (TSTR::const_iterator itr = element_name.begin(); itr != element_name.end(); ++itr)	{
			if (*itr == _T('@') || *itr == _T('$'))
				return false;
		}

		// LA �A�g�����ł���������
		S_tnms_atom la_atom;
		la_atom.type = TNMS_LA_ATOM_VAL_STR;
		la_atom.opt  = (int)plad->str_list.size();
		plad->str_list.push_back(element_name);

		// SA �m�[�h���ł���������i���e�����j
		BSP<TNMS_SA_NODE_ATOM> sa_node_literal(new TNMS_SA_NODE_ATOM);
		sa_node_literal->atom = la_atom;
		sa_node_literal->node_form = FM_STR;

		// SA �m�[�h���ł���������i�P�����j
		BSP<TNMS_SA_NODE_SMP_EXP> sa_node_smp_exp(new TNMS_SA_NODE_SMP_EXP);
		sa_node_smp_exp->node_type = TNMS_SA_NODE_TYPE_SMP_EXP_LITERAL;
		sa_node_smp_exp->Literal = sa_node_literal;
		sa_node_smp_exp->node_form = FM_STR;

		// SA �m�[�h���ł���������i���j
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
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_SIMPLE)	{	// �P����
		if (!ma_smp_exp(exp->smp_exp, ret_exist_sel))
			return false;

		// �^������
		exp->node_form = exp->smp_exp->node_form;
		exp->tmp_form = exp->node_form;
	}
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_OPR_1)	{		// �P�����Z
		if (!ma_exp(exp->exp_1, ret_exist_sel))				// ����
			return false;

		// �^������
		exp->node_form = check_operate_1(exp->exp_1->node_form, exp->opr->atom.opt);
		exp->tmp_form = exp->node_form;

		// �����^�������Ȃ���΃G���[
		if (exp->node_form == FM_VOID)
			return error(TNMSERR_MA_EXP_TYPE_NO_MATCH, exp->opr->atom);
	}
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_OPR_2)	{		// �񍀉��Z
		if (!ma_exp(exp->exp_1, ret_exist_sel))				// ����
			return false;
		if (!ma_exp(exp->exp_2, ret_exist_sel))				// �E��
			return false;

		// �^������
		exp->node_form = check_operate_2(exp->exp_1->node_form, exp->exp_2->node_form, exp->opr->atom.opt);
		exp->tmp_form = exp->node_form;

		// �����^�������Ȃ���΃G���[
		if (exp->node_form == FM_VOID)
			return error(TNMSERR_MA_EXP_TYPE_NO_MATCH, exp->opr->atom);
	}

	return true;
}

// ****************************************************************
// �Ӗ���́F<�����X�g>
// ================================================================
//		[���̌�]	: int
//		[��]		: ���ׂĂ̎�
// ================================================================
bool C_tnms_semantic_analizer::ma_exp_list(BSP<TNMS_SA_NODE_EXP_LIST> exp_list, bool* ret_exist_sel)
{
	int exp_cnt = (int)exp_list->exp.size();
	for (int e = 0; e < exp_cnt; e ++)	{
		if (!ma_exp(exp_list->exp[e], ret_exist_sel))
			return false;

		// �X�̌^��ǉ�
		exp_list->form_list.push_back(exp_list->exp[e]->node_form);
	}

	// �^������
	exp_list->node_form = FM_LIST;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�P����>
// ================================================================
bool C_tnms_semantic_analizer::ma_smp_exp(BSP<TNMS_SA_NODE_SMP_EXP> smp_exp, bool* ret_exist_sel)
{
	if (false);
	// '(' <��> ')'
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_KAKKO)	{
		if (!ma_exp(smp_exp->exp, ret_exist_sel))
			return false;

		// �^������
		smp_exp->node_form = smp_exp->exp->node_form;
	}
	// <�����X�g>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_EXP_LIST)	{
		if (!ma_exp_list(smp_exp->exp_list, ret_exist_sel))
			return false;

		// �^������
		smp_exp->node_form = smp_exp->exp_list->node_form;
	}
	// <goto ��>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_GOTO)	{
		if (!ma_goto_exp(smp_exp->Goto, ret_exist_sel))
			return false;

		// �^������
		smp_exp->node_form = smp_exp->Goto->node_form;
	}
	// <�G�������g��>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_ELM_EXP)	{
		if (!ma_elm_exp(smp_exp->elm_exp, ret_exist_sel))
			return false;

		// �^������
		smp_exp->node_form = smp_exp->elm_exp->node_form;
	}
	// <���e����>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_LITERAL)	{
		if (!ma_literal(smp_exp->Literal))
			return false;

		// �^������
		smp_exp->node_form = smp_exp->Literal->node_form;
	}

	return true;
}

// ****************************************************************
// �Ӗ���́F<����>
// ================================================================
bool C_tnms_semantic_analizer::ma_left(BSP<TNMS_SA_NODE_ELM_EXP> left)
{
	// �G�������g���X�g
	if (!ma_elm_list(left->elm_list, NULL))
		return false;

	// �G�������g�^�C�v
	left->element_type = left->elm_list->element_type;

	// �^������
	left->node_form = left->elm_list->node_form;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�G�������g��>
// ================================================================
bool C_tnms_semantic_analizer::ma_elm_exp(BSP<TNMS_SA_NODE_ELM_EXP> elm_exp, bool* ret_exist_sel)
{
	// �G�������g���X�g�́A
	// �E�R�}���h�̏ꍇ�F�����̃R�}���h�G�������g�ɂ��A���l�ɓW�J����Ă��܂��B
	// �E�v���p�e�B�̏ꍇ�F���l�����߂��Ă���ꍇ�́A�����Œ��l�ɓW�J���܂��B

	// �{���́A�R�}���h���G�������g�ŕԂ����Ƃ��ł���悤�ɂ��������]�܂����ł��B
	// ���̏ꍇ�̓��t�@�����X�̊T�O����������g�܂Ȃ��Ƃ����܂���B

	// �G�������g���X�g�����
	if (!ma_elm_list(elm_exp->elm_list, ret_exist_sel))
		return false;
	
	// �G�������g���̃^�C�v�̌���
	elm_exp->element_type = elm_exp->elm_list->element_type;

	// �^������
	elm_exp->node_form = elm_exp->elm_list->node_form;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�G�������g���X�g>
// ================================================================
bool C_tnms_semantic_analizer::ma_elm_list(BSP<TNMS_SA_NODE_ELM_LIST> elm_list, bool* ret_exist_sel)
{
	// �擪�G�������g�̏���T��
	int parent_form_code;
	TSTR element_name = plad->unknown_list[elm_list->element[0]->name->atom.opt];
	S_tnms_element* element_info = piad->form_table.find_element_info(element_name, &parent_form_code);
	if (element_info == NULL)
		return error(TNMSERR_MA_ELEMENT_UNKNOWN, elm_list->element[0]->name->atom);

	// �X�R�[�v����i�O���[�o���^�V�[���^�R�[���j
	elm_list->parent_form_code = parent_form_code;

	// �S�ẴG�������g�����
	for (int i = 0; i < (int)elm_list->element.size(); i++)	{
		if (!ma_element(parent_form_code, elm_list->element[i], ret_exist_sel))
			return false;

		// �����̃G�������g�̌^���G�������g���X�g�̌^�ɂȂ�
		elm_list->node_form = elm_list->element[i]->node_form;
		elm_list->element_type = elm_list->element[i]->element_type;

		// �e�̌^��ύX
		parent_form_code = elm_list->node_form;
	}

	// ���v���p�e�B�̓f�t�H���g�ŎQ�ƌ^
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
// �Ӗ���́F<�G�������g>
// ================================================================
bool C_tnms_semantic_analizer::ma_element(int parent_form_code, BSP<TNMS_SA_NODE_ELEMENT> element, bool* ret_exist_sel)
{
	if (false);

	// �ʏ�̃G�������g�̏ꍇ
	else if (element->node_type == TNMS_SA_NODE_TYPE_ELEMENT_ELEMENT)	{

		// �G�������g�̏����擾����
		S_tnms_element* element_info = NULL;
		TSTR element_name = plad->unknown_list[element->name->atom.opt];

		if (!piad->form_table.get_element_info(parent_form_code, element_name, &element_info))
			return error(TNMSERR_MA_ELEMENT_UNKNOWN, element->name->atom);

		// �^������
		element->node_form = element_info->form;

		// ���̑��̃G�������g�̏����L�����Ă���
		element->element_code = element_info->code;
		element->element_type = element_info->type;
		element->element_parent_form = parent_form_code;

		// �R�}���h�̏ꍇ
		if (element_info->type == TNMS_ELEMENT_TYPE_COMMAND)	{
			bool is_include_sel = false;	// ������͗p�Bret_*** �ƍ�����Ȃ��悤�ɁI

			// �������X�g
			if (!ma_arg_list(element->arg_list, &is_include_sel))
				return false;

			// �����̒��ł͑I�����͎g���Ȃ�
			if (is_include_sel)
				return error(TNMSERR_MA_SEL_CANNOT_USE_IN_ARG, element->name->atom);

			// �������X�g�̌^�`�F�b�N
			element->arg_list_id = check_arg_list(element, element_info, element->arg_list);
			if (element->arg_list_id < 0)
				return error(TNMSERR_MA_ARG_TYPE_NO_MATCH, element->name->atom);	// �G���[�F�����̌^������Ȃ�

			// ���I�����u���b�N�̔���
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

	// �Y�����̏ꍇ
	else if (element->node_type == TNMS_SA_NODE_TYPE_ELEMENT_ARRAY)	{
		bool is_include_sel = false;

		// �G�������g�̏����擾����
		S_tnms_element* element_info = NULL;
		TSTR element_name = _T("array");
		if (!piad->form_table.get_element_info(parent_form_code, element_name, &element_info))
			return error(TNMSERR_MA_ELEMENT_ILLEGAL_ARRAY, element->open_b->atom);

		// �^������
		element->node_form = element_info->form;

		// ���̑��̃G�������g�̏����L�����Ă���
		element->element_code = element_info->code;
		element->element_type = element_info->type;
		element->element_parent_form = parent_form_code;

		// �Y���������
		if (!ma_exp(element->exp, &is_include_sel))
			return false;

		// �Y�����̒��ł͑I�����͎g���Ȃ�
		if (is_include_sel)
			return error(TNMSERR_MA_SEL_CANNOT_USE_IN_INDEX, *element->exp->get_first_atom());

		// �Y�����͐���
		if (element->exp->node_form != FM_INT && element->exp->node_form != FM_INTREF)
			return error(TNMSERR_MA_INDEX_NOT_INT, *element->exp->get_first_atom());
	}

	return true;
}

// ****************************************************************
// �Ӗ���́F<�^>
// ================================================================
bool C_tnms_semantic_analizer::ma_form(BSP<TNMS_SA_NODE_FORM> form)
{
	// �Y����
	if (form->index)	{

		if (!ma_exp(form->index, NULL))
			return false;

		// �Y�����͐����łȂ���΂����Ȃ��I
		if (form->index->node_form != FM_INT)
			return error(TNMSERR_MA_DEF_PROP_NOT_INT, form->open_b->atom);
	}

	// �^������
	form->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�������X�g>
// ================================================================
//	ex.	(1, "abc")
//		<��>		// 1
//		<��>		// "abc"
// ================================================================
bool C_tnms_semantic_analizer::ma_arg_list(BSP<TNMS_SA_NODE_ARG_LIST> arg_list, bool* ret_exist_sel)
{
	int arg_cnt = (int)arg_list->arg.size();

	// �������X�g
	for (int a = 0; a < arg_cnt; a ++)	{
		if (!ma_arg(arg_list->arg[a], ret_exist_sel))
			return false;
	}

	// �^������
	arg_list->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<����>
// ================================================================
//	ex.	(type=10, time=1000)
// ================================================================
bool C_tnms_semantic_analizer::ma_arg(BSP<TNMS_SA_NODE_ARG> arg, bool* ret_exist_sel)
{
	if (!ma_exp(arg->exp, ret_exist_sel))
		return false;

	// �^������
	arg->node_form = arg->exp->node_form;

	return true;
}

// ****************************************************************
// �Ӗ���́F�������X�g�̌^�`�F�b�N
// ================================================================
int C_tnms_semantic_analizer::check_arg_list(BSP<TNMS_SA_NODE_ELEMENT> element, S_tnms_element* element_info, BSP<TNMS_SA_NODE_ARG_LIST> real_arg_list)
{
	for (std::map<int, S_tnms_arg_list>::iterator itr = element_info->arg_map.begin(); itr != element_info->arg_map.end(); ++itr)	{

		// �܂��͖��O�Ȃ������̉�͂��s��
		if (itr->first != -1 && check_no_named_arg_list(itr->second, real_arg_list))	{

			// ���O�Ȃ������̉�͂ɐ��������̂ŁA�������X�gID�͂����Ŋm��

			// ���O�����������
			if (!check_named_arg_list(element, element_info, real_arg_list))
				return -1;	// ���s

			return itr->first;	// �������X�gID��Ԃ�
		}
	}

	// ���s
	return -1;
}

// ****************************************************************
// �Ӗ���́F�������X�g�̌^�`�F�b�N
// ================================================================
bool C_tnms_semantic_analizer::check_no_named_arg_list(S_tnms_arg_list& temp_arg_list, BSP<TNMS_SA_NODE_ARG_LIST> real_arg_list)
{
	// ���������p�̈������X�g�����
	ARRAY<int> real_arg_form_list(real_arg_list->arg.size());
	for (int i = 0; i < (int)real_arg_list->arg.size(); i++)
		real_arg_form_list[i] = real_arg_list->arg[i]->exp->tmp_form;

	ARRAY<S_tnms_arg>::iterator t_itr = temp_arg_list.arg_list.begin();							// ������
	ARRAY<S_tnms_arg>::iterator t_itr_end = temp_arg_list.arg_list.end();						// ������
	ARRAY<int>::iterator r_itr = real_arg_form_list.begin();									// ������
	ARRAY<int>::iterator r_itr_end = real_arg_form_list.end() - real_arg_list->named_arg_cnt;	// ������

	while (1)	{

		// ���������I�������ꍇ
		if (t_itr == t_itr_end)	{

			// ���������I�������Ȃ琬��
			if (r_itr == r_itr_end)
				break;

			// ���������I�������̂Ɏ��������܂�����Ύ��s
			if (r_itr != r_itr_end)
				return false;
		}

		// ���������܂�����ꍇ
		else	{

			// �������� __args �Ȃ琬��
			if (t_itr->form == FM___ARGS)	{

				// �c��̎�������S�Ēl�ɂ���i�l�ɂȂ�邩�ǂ����̔���� BS �ōs���j
				for (ARRAY<int>::iterator itr = r_itr; itr != r_itr_end; ++itr)	{
					if (*itr == FM_INTREF)
						*itr = FM_INT;
					else if (*itr == FM_STRREF)
						*itr = FM_STR;
				}
				break;
			}

			// �������� __argsref �Ȃ琬��
			else if (t_itr->form == FM___ARGSREF)	{

				// �c��̎�������S�ĎQ�Ƃɂ���i�Q�ƂɂȂ�邩�ǂ����̔���� BS �ōs���j
				// �Q�ƂɂȂ�Ȃ���F100�A"abc" �Ȃǂ̒��l
				for (ARRAY<int>::iterator itr = r_itr; itr != r_itr_end; ++itr)	{
					if (*itr == FM_INT)
						*itr = FM_INTREF;
					else if (*itr == FM_STR)
						*itr = FM_STRREF;
				}
				break;
			}

			// ���������I�������ꍇ
			else if (r_itr == r_itr_end)	{

				// ���������f�t�H���g�����������Ă���Ȃ琬��
				if (t_itr->def_exist)
					break;

				// ���������܂�����̂Ŏ��s
				else
					return false;
			}

			// �����������������܂�����ꍇ
			else	{

				// �������Ǝ������̐������`�F�b�N
				if (t_itr->form != *r_itr)	{

					// �������Ƃ��Ēl�����߂��Ă���̂ɎQ�Ƃ������ꍇ
					if (false);
					else if (t_itr->form == FM_INT && *r_itr == FM_INTREF)	{
						*r_itr = FM_INT;		// ��������l�ɂ���i�l�ɂȂ�邩�ǂ����̔���� BS �ōs���j
					}
					else if (t_itr->form == FM_STR && *r_itr == FM_STRREF)	{
						*r_itr = FM_STR;		// ��������l�ɂ���i�l�ɂȂ�邩�ǂ����̔���� BS �ōs���j
					}
					// ����ȊO�Ō^���Ⴆ�Ύ��s
					else	{
						return false;
					}
				}
			}
		}

		++ t_itr;
		++ r_itr;
	}

	// ���������������X�g�ɍX�V�i�Q�ƂȂǂ��ς���Ă���\��������j
	for (int i = 0; i < (int)real_arg_list->arg.size(); i++)
		real_arg_list->arg[i]->exp->tmp_form = real_arg_form_list[i];

	return true;
}

// ****************************************************************
// �Ӗ���́F���O�������X�g�̌^�`�F�b�N
// ================================================================
bool C_tnms_semantic_analizer::check_named_arg_list(BSP<TNMS_SA_NODE_ELEMENT> element, S_tnms_element* element_info, BSP<TNMS_SA_NODE_ARG_LIST> real_arg_list)
{
	if (real_arg_list->named_arg_cnt == 0)
		return true;	// ���O�����������݂��Ȃ��̂Ŗ��Ȃ�

	// ���O���������������ꍇ�́A���O�������������邱�Ƃ��m�F����
	std::map<int, S_tnms_arg_list>::iterator tmp_arg_list = element_info->arg_map.find(-1);	// -1: ���O���������X�g
	if (tmp_arg_list == element_info->arg_map.end())
		return error(TNMSERR_MA_CMD_NO_NAMED_ARG_LIST, element->name->atom);	// ���O���̉��������X�g��������Ȃ�

	int real_named_arg_cnt = real_arg_list->named_arg_cnt;
	ARRAY< BSP<TNMS_SA_NODE_ARG> >::iterator real_named_arg_itr_begin = real_arg_list->arg.end() - real_named_arg_cnt;
	ARRAY< BSP<TNMS_SA_NODE_ARG> >::iterator real_named_arg_itr_end = real_arg_list->arg.end();

	// �������̌^���X�g�i���������p�j�����
	ARRAY<int> real_arg_form_list(real_named_arg_cnt);
	for (int i = 0; i < real_named_arg_cnt; i++)
		real_arg_form_list[i] = (*(real_named_arg_itr_begin + i))->exp->tmp_form;

	ARRAY< BSP<TNMS_SA_NODE_ARG> >::iterator real_named_arg_itr = real_named_arg_itr_begin;
	ARRAY<int>::iterator real_named_arg_form_itr = real_arg_form_list.begin();
	for (; real_named_arg_itr < real_named_arg_itr_end; ++real_named_arg_itr, ++real_named_arg_form_itr)	{

		TSTR real_arg_name = plad->unknown_list[(*real_named_arg_itr)->name->atom.opt];

		// �������ŗ^����ꂽ���O������������T��
		int tmp_arg_no = -1;
		int tmp_arg_cnt = (int)tmp_arg_list->second.arg_list.size();
		for (int ta = 0; ta < tmp_arg_cnt; ta++)	{
			if (real_arg_name == tmp_arg_list->second.arg_list[ta].name)	{
				tmp_arg_no = ta;
				break;
			}
		}
		if (tmp_arg_no == -1)
			return error(TNMSERR_MA_CMD_ILLEGAL_NAMED_ARG, (*real_named_arg_itr)->name->atom);	// �w�肳�ꂽ���O��������Ȃ�

		int t_form = tmp_arg_list->second.arg_list[tmp_arg_no].form;	// �������̌^
		int r_form = *real_named_arg_form_itr;							// �������̌^

		// �^�`�F�b�N
		if (r_form != t_form)	{

			// �������Ƃ��Ēl�����߂��Ă���̂ɎQ�Ƃ������ꍇ
			if (false);
			else if (t_form == FM_INT && r_form == FM_INTREF)	{
				*real_named_arg_form_itr = FM_INT;		// ��������l�ɂ���i�l�ɂȂ�邩�ǂ����̔���� BS �ōs���j
			}
			else if (t_form == FM_STR && r_form == FM_STRREF)	{
				*real_named_arg_form_itr = FM_STR;		// ��������l�ɂ���i�l�ɂȂ�邩�ǂ����̔���� BS �ōs���j
			}
			// ����ȊO�Ō^���Ⴆ�Ύ��s
			else	{
				return error(TNMSERR_MA_ARG_TYPE_NO_MATCH, (*real_named_arg_itr)->name->atom);	// �^������Ȃ�
			}
		}

		// ���� id ���m��
		(*real_named_arg_itr)->name_id = tmp_arg_list->second.arg_list[tmp_arg_no].id;
	}

	// ���������������X�g�ɍX�V�i�Q�ƂȂǂ��ς���Ă���\��������j
	for (int i = 0; i < real_named_arg_cnt; i++)
		(*(real_named_arg_itr_begin + i))->exp->tmp_form = real_arg_form_list[i];

	return true;
}

// ****************************************************************
// �Ӗ���́F<���O>
// ================================================================
bool C_tnms_semantic_analizer::ma_name(BSP<TNMS_SA_NODE_NAME> name)
{
	if (!ma_literal(name->name))
		return false;

	// �^������
	name->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<���e����>
// ================================================================
bool C_tnms_semantic_analizer::ma_literal(BSP<TNMS_SA_NODE_ATOM> Literal)
{
	// �^������
	if (false);
	else if (Literal->atom.type == TNMS_LA_ATOM_VAL_INT)	Literal->node_form = FM_INT;
	else if (Literal->atom.type == TNMS_LA_ATOM_VAL_STR)	Literal->node_form = FM_STR;
	else if (Literal->atom.type == TNMS_LA_ATOM_LABEL)		Literal->node_form = FM_LABEL;

	return true;
}

// ****************************************************************
// �Ӗ���́F<������Z�q>
// ================================================================
bool C_tnms_semantic_analizer::ma_assign_operator(BSP<TNMS_SA_NODE_ATOM> opr)
{
	// �^������
	opr->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�P�����Z�q>
// ================================================================
bool C_tnms_semantic_analizer::ma_operator_1(BSP<TNMS_SA_NODE_ATOM> opr)
{
	// �^������
	opr->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�񍀉��Z�q>
// ================================================================
bool C_tnms_semantic_analizer::ma_operator_2(BSP<TNMS_SA_NODE_ATOM> opr)
{
	// �^������
	opr->node_form = FM_VOID;

	return true;
}

// ****************************************************************
// �Ӗ���́F<�I�[>
// ================================================================
bool C_tnms_semantic_analizer::ma_eof(BSP<TNMS_SA_NODE_ATOM> eof)
{
	// �^������
	eof->node_form = FM_VOID;

	return true;
}


// ****************************************************************
// �Ӗ���́F�P�����Z�̌^���`�F�b�N
// ================================================================
int C_tnms_semantic_analizer::check_operate_1(int r_form, BYTE opr)
{
	int ret_form = FM_VOID;

	// �P�����Z�� int �^�ɑ΂��Ă�����`����Ă��Ȃ�
	// ���Z��͕K�� int �^�ɂȂ�
	if (r_form == FM_INT || r_form == FM_INTREF)		ret_form = FM_INT;

	return ret_form;
}

// ****************************************************************
// �Ӗ���́F�񍀉��Z�̌^���`�F�b�N
// ================================================================
int C_tnms_semantic_analizer::check_operate_2(int l_form, int r_form, BYTE opr)
{
	int ret_form = FM_VOID;

	if (false);
	else if ((l_form == FM_INT || l_form == FM_INTREF) && (r_form == FM_INT || r_form == FM_INTREF))	{	// ���� op ����
		if (false);
		else if (opr == OP_PLUS)			ret_form = FM_INT;		// (���� +  ����) = ����
		else if (opr == OP_MINUS)			ret_form = FM_INT;		// (���� -  ����) = ����
		else if (opr == OP_MULTIPLE)		ret_form = FM_INT;		// (���� *  ����) = ����
		else if (opr == OP_DIVIDE)			ret_form = FM_INT;		// (���� /  ����) = ����
		else if (opr == OP_AMARI)			ret_form = FM_INT;		// (���� %  ����) = ����
		else if (opr == OP_EQUAL)			ret_form = FM_INT;		// (���� == ����) = ����
		else if (opr == OP_NOT_EQUAL)		ret_form = FM_INT;		// (���� != ����) = ����
		else if (opr == OP_GREATER)			ret_form = FM_INT;		// (���� >  ����) = ����
		else if (opr == OP_GREATER_EQUAL)	ret_form = FM_INT;		// (���� >= ����) = ����
		else if (opr == OP_LESS)			ret_form = FM_INT;		// (���� <  ����) = ����
		else if (opr == OP_LESS_EQUAL)		ret_form = FM_INT;		// (���� <= ����) = ����
		else if (opr == OP_LOGICAL_AND)		ret_form = FM_INT;		// (���� && ����) = ����
		else if (opr == OP_LOGICAL_OR)		ret_form = FM_INT;		// (���� || ����) = ����

		else if (opr == OP_AND)				ret_form = FM_INT;		// (���� &  ����) = ����
		else if (opr == OP_OR)				ret_form = FM_INT;		// (���� |  ����) = ����
		else if (opr == OP_HAT)				ret_form = FM_INT;		// (���� ^  ����) = ����
		else if (opr == OP_SL)				ret_form = FM_INT;		// (���� << ����) = ����
		else if (opr == OP_SR)				ret_form = FM_INT;		// (���� >> ����) = ����
		else if (opr == OP_SR3)				ret_form = FM_INT;		// (���� >>> ����) = ����
	}
	else if ((l_form == FM_STR || l_form == FM_STRREF) && (r_form == FM_STR || r_form == FM_STRREF))	{	// ������ op ������
		if (false);
		else if (opr == OP_PLUS)			ret_form = FM_STR;		// (������ +  ������) = ������
		else if (opr == OP_EQUAL)			ret_form = FM_INT;		// (������ == ������) = ����
		else if (opr == OP_NOT_EQUAL)		ret_form = FM_INT;		// (������ != ������) = ����
		else if (opr == OP_GREATER)			ret_form = FM_INT;		// (������ >  ������) = ����
		else if (opr == OP_GREATER_EQUAL)	ret_form = FM_INT;		// (������ >= ������) = ����
		else if (opr == OP_LESS)			ret_form = FM_INT;		// (������ <  ������) = ����
		else if (opr == OP_LESS_EQUAL)		ret_form = FM_INT;		// (������ <= ������) = ����
	}
	else if ((l_form == FM_STR || l_form == FM_STRREF) && (r_form == FM_INT || r_form == FM_INTREF))	{	// ������ op ����
		if (false);
		else if (opr == OP_MULTIPLE)		ret_form = FM_STR;		// (������ *  ����) = ������
	}

	return ret_form;
}

// ****************************************************************
// �Ӗ���̓G���[
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
	// �G���[�̐ݒ�
	switch (last_error.type)	{
		case TNMSERR_MA_DEF_PROP_NOT_INT:			return _T("�Y�����̌^�͐����łȂ���΂����܂���B");
		case TNMSERR_MA_IF_COND_IS_NOT_INT:			return _T("if ���̏������͐����łȂ���΂����܂���B");
		case TNMSERR_MA_FOR_COND_IS_NOT_INT:		return _T("for ���̏������͐����łȂ���΂����܂���B");
		case TNMSERR_MA_WHILE_COND_IS_NOT_INT:		return _T("while ���̏������͐����łȂ���΂����܂���B");
		case TNMSERR_MA_CASE_TYPE_MISMATCH:			return _T("case ���̌^�� switch ���̏������̌^�ƈ�v���܂���B");
		case TNMSERR_MA_ASSIGN_LEFT_NEED_REFERENCE:	return _T("������̍��ӂ͐�����������̕ϐ��ł���K�v������܂��B");
		case TNMSERR_MA_ASSIGN_TYPE_NO_MATCH:		return _T("������̍��E�̌^����v���܂���B");
		case TNMSERR_MA_EXP_TYPE_NO_MATCH:			return _T("���Z�̌^����v���܂���B");
		case TNMSERR_MA_ELEMENT_UNKNOWN:			return _T("����`�̃G�������g�ł��B");
		case TNMSERR_MA_ELEMENT_IS_PROPERTY:		return _T("���̃G�������g�̓R�}���h�ł͂���܂���B");
		case TNMSERR_MA_ELEMENT_ILLEGAL_ARRAY:		return _T("'[' ��������܂������A���̃G�������g�͔z��ł͂���܂���B");
		case TNMSERR_MA_INDEX_NOT_INT:				return _T("�z��̓Y�����͐����łȂ���΂����܂���B");
		case TNMSERR_MA_ARG_TYPE_NO_MATCH:			return _T("�R�}���h�̈����̌^�������܂���B");
		case TNMSERR_MA_CMD_NO_NAMED_ARG_LIST:		return _T("���̃R�}���h�͖��O���������󂯎��܂���B");
		case TNMSERR_MA_CMD_ILLEGAL_NAMED_ARG:		return _T("�s���Ȗ��O�������ł��B");
		case TNMSERR_MA_SEL_CANNOT_USE_IN_COND:		return _T("�������̒��ł� sel �R�}���h�͎g���܂���B");
		case TNMSERR_MA_SEL_CANNOT_USE_IN_ARG:		return _T("�R�}���h�̈����� sel �R�}���h���g�����Ƃ͂ł��܂���B");
		case TNMSERR_MA_SEL_CANNOT_USE_IN_GOTO:		return _T("gosub �̈����� sel �R�}���h���g�����Ƃ͂ł��܂���B");
		case TNMSERR_MA_SEL_CANNOT_USE_IN_RETURN:	return _T("return �̈����� sel �R�}���h���g�����Ƃ͂ł��܂���B");
		case TNMSERR_MA_SEL_CANNOT_USE_IN_INDEX:	return _T("�z��̓Y������ sel �R�}���h���g�����Ƃ͂ł��܂���B");
		case TNMSERR_MA_PROPERTY_OUT_OF_COMMAND:	return _T("���[�U�[�v���p�e�B�����[�U�[�R�}���h�̊O�Œ�`����Ă��܂��B");
	}

	return _T("�Ӗ���́F�s���ȃG���[�ł��B");
}

