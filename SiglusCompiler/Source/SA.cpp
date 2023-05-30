#include	"pch.h"
#include	"code.h"
#include	"element.h"
#include	"tnm_common_header.h"
#include	"SA.h"

// ****************************************************************
// �\����� - Syntax Analizer -
// ================================================================
bool C_tnms_sentence_analizer::analize(S_tnms_ia_data* piad_, S_tnms_la_data* plad_, S_tnms_sa_data* psad_)
{
	// �f�[�^�̏�����
	piad = piad_;
	plad = plad_;
	psad = psad_;

	// ������
	psad->cur_call_prop_cnt = 0;
	psad->total_call_prop_cnt = 0;

	// �G���[�̏�����
	clear_error();

	// ���x���f�[�^�̈ڐA
	label_list.resize(plad->label_list.size());
	for (int i = 0; i < (int)label_list.size(); i++)	{
		label_list[i].name = plad->label_list[i].name;
		label_list[i].line = plad->label_list[i].line;
	}
	// �y���x���f�[�^
	psad->z_label_list.resize(TNM_Z_LABEL_CNT);

	// �A�g�����X�g�ɔԕ�������
	S_tnms_atom atom;
	for (int z = 0; z < 256; z++)	{
		atom.id = (int)plad->atom_list.size();
		plad->atom_list.push_back(atom);
	}

	// �A�g�����X�g�̏����ʒu���Z�b�g
	ITR atom_itr = plad->atom_list.begin();

	// <SS>
	if (!sa_ss(atom_itr, psad->root))
		return false;

	// ��U�G���[�̃N���A
	clear_error();

	// ���x���̑��ݔ���
	// ���ꂽ�S���x���ɑ΂��āA�{�̂��Ȃ���΃G���[�ɂȂ�
	// �i�Ⴆ�΁Agoto #LABEL ������̂ɁA#LABEL ���Ȃ��A�Ȃǁj
	for (int i = 0; i < (int)label_list.size(); i++)	{
		if (!label_list[i].exist)	{
			// �G���[��\�����邽�߂ɃA�g���̂ł�������
			S_tnms_atom atom;
			atom.type = TNMS_LA_ATOM_LABEL;
			atom.line = label_list[i].line;
			atom.opt = i;
			return error(TNMSERR_SA_LABEL_NOT_EXIST, atom);
		}
	}

	// #Z00 �̑��ݔ���
	// #Z00 ���Ȃ���΃G���[�ɂȂ�
	if (!psad->z_label_list[0].exist)
		return error(TNMSERR_SA_Z_LABEL_00_NOT_EXIST);

	// �R�}���h�̒�`���ݔ���
	// �V�[�����R�}���h�̒�`��������Ȃ���΃G���[�ɂȂ�
	for (int i = piad->inc_command_cnt; i < piad->command_cnt; i++)	{
		if (!piad->command_list[i]->is_defined)
			return error(TNMSERR_SA_DEF_CMD_NOT_EXIST);
	}

	return true;
}

// ****************************************************************
// �\����́F<SS>
// ================================================================
bool C_tnms_sentence_analizer::sa_ss(ITR& start, BSP<TNMS_SA_NODE_SS>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_SS> ss(new TNMS_SA_NODE_SS);
	ss->node_line = p->line;

	// <��*>
	while (p->type != TNMS_LA_ATOM_NONE)	{
		BSP<TNMS_SA_NODE_SENTENCE> sentence;
		if (!sa_sentence(p, sentence))
			return false;

		ss->sentense_list.push_back(sentence);
	}

	// <����>
	if (p->type != TNMS_LA_ATOM_NONE)
		return false;

	// ��Ԃ̍X�V
	last_error = err;
	parent = ss;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�u���b�N>
// ================================================================
bool C_tnms_sentence_analizer::sa_block(ITR& start, BSP<TNMS_SA_NODE_BLOCK>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_BLOCK> block(new TNMS_SA_NODE_BLOCK);
	block->node_line = p->line;

	// "{"
	if (!sa_atom(p, block->open_b, TNMS_LA_ATOM_OPEN_BRACE))
		return false;

	// <��*>
	while (p->type != TNMS_LA_ATOM_NONE && p->type != TNMS_LA_ATOM_CLOSE_BRACE)	{
		BSP<TNMS_SA_NODE_SENTENCE> sentence;
		if (!sa_sentence(p, sentence))
			return error(TNMSERR_SA_BLOCK_ILLEGAL_SENTENCE, *p);
		block->sentense_list.push_back(sentence);
	}

	// '}'
	if (!sa_atom(p, block->close_b, TNMS_LA_ATOM_CLOSE_BRACE))
		return error(TNMSERR_SA_BLOCK_NO_CLOSE_BRACE, block->open_b->atom);

	// ��Ԃ̍X�V
	last_error = err;
	parent = block;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<��>
// ================================================================
bool C_tnms_sentence_analizer::sa_sentence(ITR& start, BSP<TNMS_SA_NODE_SENTENCE>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_SENTENCE> sentence(new TNMS_SA_NODE_SENTENCE);
	sentence->node_line = p->line;

	if (false);

	// <���x����>
	else if (sa_label(p, sentence->label))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_LABEL;
	}
	// <�y���x����>
	else if (sa_z_label(p, sentence->z_label))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_Z_LABEL;
	}
	// <�R�}���h��`��>
	else if (sa_def_cmd(p, sentence->def_cmd))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_DEF_CMD;
	}
	// <�v���p�e�B��`��>
	else if (sa_def_prop(p, sentence->def_prop))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_DEF_PROP;
	}
	// <goto ��>
	else if (sa_goto(p, sentence->Goto))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_GOTO;
	}
	// <return ��>
	else if (sa_return(p, sentence->Return))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_RETURN;
	}
	// <if ��>
	else if (sa_if(p, sentence->If))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_IF;
	}
	// <for ��>
	else if (sa_for(p, sentence->For))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_FOR;
	}
	// <while ��>
	else if (sa_while(p, sentence->While))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_WHILE;
	}
	// <continue ��>
	else if (sa_continue(p, sentence->Continue))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_CONTINUE;
	}
	// <break ��>
	else if (sa_break(p, sentence->Break))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_BREAK;
	}
	// <switch ��>
	else if (sa_switch(p, sentence->Switch))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_SWITCH;
	}
	// <�R�}���h��> / <�����>
	else if (sa_command_or_assign(p, sentence->command, sentence->assign))	{
		if (sentence->command)	sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_COMMAND;
		else					sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_ASSIGN;
	}
	// <���O>
	else if (sa_name(p, sentence->name))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_NAME;
	}
	// <�e�L�X�g>
	else if (sa_atom(p, sentence->text, TNMS_LA_ATOM_VAL_STR))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_TEXT;
	}
	// <����>
	else if (sa_atom(p, sentence->eof, TNMS_LA_ATOM_EOF))	{
		sentence->node_type = TNMS_SA_NODE_TYPE_SENTENCE_EOF;
	}
	// ���s
	else
		return error(TNMSERR_SA_SENTENCE_ILLEGAL, *p);		// �G���[�F�s���ȕ�

	// ��Ԃ̍X�V
	last_error = err;
	parent = sentence;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<���x����>
// ================================================================
bool C_tnms_sentence_analizer::sa_label(ITR& start, BSP<TNMS_SA_NODE_LABEL>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_LABEL> label(new TNMS_SA_NODE_LABEL);
	label->node_line = p->line;

	// <���x��>
	if (!sa_atom(p, label->label, TNMS_LA_ATOM_LABEL))
		return false;

	// �������x�������݂��Ă�����G���[
	if (label_list[label->label->atom.opt].exist)
		return error(TNMSERR_SA_LABEL_OVERLAPPED, label->label->atom);

	// ���x�����̐ݒ�
	label_list[label->label->atom.opt].line = label->label->node_line;
	label_list[label->label->atom.opt].exist = true;

	// ��Ԃ̍X�V
	last_error = err;
	parent = label;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�y���x����>
// ================================================================
bool C_tnms_sentence_analizer::sa_z_label(ITR& start, BSP<TNMS_SA_NODE_Z_LABEL>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_Z_LABEL> z_label(new TNMS_SA_NODE_Z_LABEL);
	z_label->node_line = p->line;

	// <�y���x��>
	if (!sa_atom(p, z_label->z_label, TNMS_LA_ATOM_Z_LABEL))
		return false;

	// �����y���x�������݂��Ă�����G���[
	if (psad->z_label_list[z_label->z_label->atom.opt].exist)
		return error(TNMSERR_SA_Z_LABEL_OVERLAPPED, z_label->z_label->atom);

	// �y���x�����̐ݒ�
	psad->z_label_list[z_label->z_label->atom.opt].line = z_label->z_label->node_line;
	psad->z_label_list[z_label->z_label->atom.opt].exist = true;

	// ���x���Ƃ��Ă��o�^���Ă���̂ŁA���x�����̐ݒ�
	label_list[z_label->z_label->atom.subopt].line = z_label->z_label->node_line;
	label_list[z_label->z_label->atom.subopt].exist = true;

	// ��Ԃ̍X�V
	last_error = err;
	parent = z_label;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�R�}���h��`��>
// ================================================================
bool C_tnms_sentence_analizer::sa_def_cmd(ITR& start, BSP<TNMS_SA_NODE_DEF_CMD>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_DEF_CMD> def_cmd(new TNMS_SA_NODE_DEF_CMD);
	def_cmd->node_line = p->line;

	// "command"
	if (!sa_atom(p, def_cmd->command, TNMS_LA_ATOM_COMMAND))
		return false;

	// <�G�������g��>
	if (!sa_atom(p, def_cmd->name, TNMS_LA_ATOM_UNKNOWN))
		return error(TNMSERR_SA_DEF_CMD_ILLEGAL_NAME, *p);

	// "("
	if (sa_atom(p, def_cmd->open_p, TNMS_LA_ATOM_OPEN_PAREN))	{

		// ")"	�i������ ")" ��������Έ����Ȃ��j
		if (!sa_atom(p, def_cmd->close_p, TNMS_LA_ATOM_CLOSE_PAREN))	{

			while (1)	{

				// <�ϐ��錾>
				BSP<TNMS_SA_NODE_DEF_PROP> prop;
				if (!sa_def_prop(p, prop))
					return error(TNMSERR_SA_DEF_CMD_ILLEGAL_ARG, *p);

				def_cmd->prop_list.push_back(prop);

				// ")"	// �����I��
				if (sa_atom(p, def_cmd->close_p, TNMS_LA_ATOM_CLOSE_PAREN))
					break;

				// ","	// ��������
				BSP<TNMS_SA_NODE_ATOM> comma;
				if (!sa_atom(p, comma, TNMS_LA_ATOM_COMMA))
					return error(TNMSERR_SA_DEF_CMD_NO_COMMA, *p);

				def_cmd->comma_list.push_back(comma);
			}
		}
	}

	// ":"
	if (sa_atom(p, def_cmd->colon, TNMS_LA_ATOM_COLON))	{

		// <�^>
		if (!sa_form(p, def_cmd->form))
			return error(TNMSERR_SA_DEF_CMD_ILLEGAL_FORM, *p);
	}

	// ���̃R�}���h�̌^������i�ȗ����� int �^�Ƃ݂Ȃ����j
	def_cmd->form_code = def_cmd->form ? def_cmd->form->form_code : FM_INT;

	// <�u���b�N>
	if (!sa_block(p, def_cmd->block))
		return error(TNMSERR_SA_DEF_CMD_NO_OPEN_BRACE, def_cmd->name->atom);

	// ���[�U�R�}���h�̐錾��T��
	BSP<S_tnms_ia_command> command;
	for (int i = 0; i < (int)piad->command_list.size(); i++)	{
		if (piad->command_list[i]->name == plad->unknown_list[def_cmd->name->atom.opt])	{
			command = piad->command_list[i];
			break;
		}
	}

	// ������Ȃ�������V�K�R�}���h
	if (!command)	{

		int command_id = piad->command_cnt ++;
		def_cmd->cmd_id = command_id;

		// �������X�g���쐬
		S_tnms_arg_list arg_list;
		arg_list.arg_list.resize((int)def_cmd->prop_list.size());
		for (int i = 0; i < (int)def_cmd->prop_list.size(); i++)	{
			arg_list.arg_list[i].form = def_cmd->prop_list[i]->form_code;
			arg_list.arg_list[i].def_exist = false;	// �f�t�H���g�����Ȃ�
		}

		// inc �ɃR�}���h��ǉ�
		BSP<S_tnms_ia_command> command(new S_tnms_ia_command);
		command->id = command_id;
		command->form = def_cmd->form_code;
		command->name = plad->unknown_list[def_cmd->name->atom.opt];
		command->arg_list = arg_list;
		command->is_defined = true;		// ��`���ꂽ
		piad->command_list.push_back(command);
		piad->name_set.insert(command->name);

		// ���[�U�R�}���h�� �^ FM_SCENE �ɒǉ�
		S_tnms_element element;
		element.type = TNMS_ELEMENT_TYPE_COMMAND;
		element.code = create_elm_code(ELM_OWNER_USER_CMD, 0, command_id);
		element.name = plad->unknown_list[def_cmd->name->atom.opt];
		element.form = def_cmd->form ? def_cmd->form->form_code : FM_INT;
		element.add_arg_list(0, arg_list);
		S_tnms_form* form_global = piad->form_table.get_form_by_code(FM_SCENE);
		form_global->add_element(&element);
	}
	else	{

		// ���������̂ŃR�}���hID����
		def_cmd->cmd_id = command->id;

		// ��`�ς݂Ȃ�G���[
		if (command->is_defined)
			return error(TNMSERR_SA_DEF_CMD_ALREADY_DEFINED, def_cmd->name->atom);

		// �C���N�R�}���h�̏ꍇ�A�^�𒲂ׂ܂��B
		if (command->id < piad->inc_command_cnt)	{

			// �R�}���h�̌^
			if (piad->command_list[command->id]->form != def_cmd->form_code)	{
				return error(TNMSERR_SA_DEF_CMD_TYPE_NO_MATCH, def_cmd->name->atom);
			}
			// �����̌�
			if (piad->command_list[command->id]->arg_list.arg_list.size() != def_cmd->prop_list.size())	{
				return error(TNMSERR_SA_DEF_CMD_ARG_TYPE_NO_MATCH, def_cmd->name->atom);
			}
			// �����̌^
			for (int i = 0; i < (int)piad->command_list[command->id]->arg_list.arg_list.size(); i++)	{
				if (piad->command_list[command->id]->arg_list.arg_list[i].form != def_cmd->prop_list[i]->form_code)	{
					return error(TNMSERR_SA_DEF_CMD_ARG_TYPE_NO_MATCH, def_cmd->name->atom);
				}
			}
		}

		// �V�[���R�}���h�̏ꍇ�͒�`�ς݂ɂ���i�C���N�R�}���h�̏ꍇ�̓����N���Ƀ`�F�b�N����̂ł��j
		if (command->id >= piad->inc_command_cnt)
			command->is_defined = true;
	}

	// ��Ԃ̍X�V
	last_error = err;
	parent = def_cmd;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�v���p�e�B��`��>
// ================================================================
bool C_tnms_sentence_analizer::sa_def_prop(ITR& start, BSP<TNMS_SA_NODE_DEF_PROP>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_DEF_PROP> def_prop(new TNMS_SA_NODE_DEF_PROP);
	def_prop->node_line = p->line;

	// "property"
	if (!sa_atom(p, def_prop->Property, TNMS_LA_ATOM_PROPERTY))
		return false;
	// <�G�������g��>
	if (!sa_atom(p, def_prop->name, TNMS_LA_ATOM_UNKNOWN))
		return error(TNMSERR_SA_DEF_PROP_ILLEGAL_NAME, *p);

	// ":"
	if (sa_atom(p, def_prop->colon, TNMS_LA_ATOM_COLON))	{

		// <�^>
		if (!sa_form(p, def_prop->form))
			return false;
	}

	// �錾���ꂽ�v���p�e�B�̌^������
	// �^���ȗ������ꍇ�� int �^�Ƃ݂Ȃ����
	def_prop->form_code = def_prop->form ? def_prop->form->form_code : FM_INT;

	// ��Ԃ̍X�V
	last_error = err;
	parent = def_prop;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<goto ��>
// ================================================================
bool C_tnms_sentence_analizer::sa_goto(ITR& start, BSP<TNMS_SA_NODE_GOTO>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_GOTO> Goto(new TNMS_SA_NODE_GOTO);
	Goto->node_line = p->line;

	// "goto" / "gosub" / "gosubstr"
	if (false);
	else if (sa_atom(p, Goto->Goto, TNMS_LA_ATOM_GOTO))		{
		Goto->node_type = TNMS_SA_NODE_TYPE_GOTO_GOTO;
	}
	else if (sa_atom(p, Goto->Goto, TNMS_LA_ATOM_GOSUB))		{
		Goto->node_type = TNMS_SA_NODE_TYPE_GOTO_GOSUB;
	}
	else if (sa_atom(p, Goto->Goto, TNMS_LA_ATOM_GOSUBSTR))		{
		Goto->node_type = TNMS_SA_NODE_TYPE_GOTO_GOSUBSTR;
	}
	else
		return false;

	// gosub / gosubstr �̏ꍇ�������X�g
	if (Goto->Goto->atom.type == TNMS_LA_ATOM_GOSUB || Goto->Goto->atom.type == TNMS_LA_ATOM_GOSUBSTR)	{
		if (!sa_arg_list(p, Goto->arg_list))
			return false;
	}

	// "���x����"
	if (false);
	else if (sa_atom(p, Goto->label, TNMS_LA_ATOM_LABEL))	{
		Goto->node_sub_type = TNMS_SA_NODE_TYPE_GOTO_LABEL;
	}
	else if (sa_atom(p, Goto->z_label, TNMS_LA_ATOM_Z_LABEL))	{
		Goto->node_sub_type = TNMS_SA_NODE_TYPE_GOTO_Z_LABEL;
	}
	else
		return error(TNMSERR_SA_GOTO_NO_LABEL, Goto->Goto->atom);

	// ��Ԃ̍X�V
	last_error = err;
	parent = Goto;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<return ��>
// ================================================================
bool C_tnms_sentence_analizer::sa_return(ITR& start, BSP<TNMS_SA_NODE_RETURN>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_RETURN> Return(new TNMS_SA_NODE_RETURN);
	Return->node_line = p->line;
	Return->node_type = TNMS_SA_NODE_TYPE_RETURN_WITHOUT_ARG;

	// 'return'
	if (!sa_atom(p, Return->Return, TNMS_LA_ATOM_RETURN))
		return false;
	// '('
	if (sa_atom(p, Return->open_p, TNMS_LA_ATOM_OPEN_PAREN))	{

		// <������>
		if (!sa_exp(p, Return->exp, 0))
			return error(TNMSERR_SA_RETURN_ILLEGAL_EXP, Return->Return->atom);
		// ')'
		if (!sa_atom(p, Return->close_p, TNMS_LA_ATOM_CLOSE_PAREN))
			return error(TNMSERR_SA_RETURN_NO_CLOSE_PAREN, Return->Return->atom);
		// ��������
		Return->node_type = TNMS_SA_NODE_TYPE_RETURN_WITH_ARG;
	}

	// ��Ԃ̍X�V
	last_error = err;
	parent = Return;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<if ��>
// ================================================================
bool C_tnms_sentence_analizer::sa_if(ITR& start, BSP<TNMS_SA_NODE_IF>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_IF> If(new TNMS_SA_NODE_IF);
	If->node_line = p->line;

	// ���[�v
	bool loop_out = false;
	for (int i = 0; !loop_out; i++)	{

		TNMS_SA_NODE_IF_SUB sub;
		bool need_cond = false;

		// ����� "if" 
		if (i == 0)	{
			if (sa_atom(p, sub.If, TNMS_LA_ATOM_IF))		need_cond = true;	// if �͏��������K�v
			else										return false;		// ���s
		}
		// ����ȊO�� "elseif" / "else"
		else	{
			if (false);
			else if (sa_atom(p, sub.If, TNMS_LA_ATOM_ELSEIF))	need_cond = true;	// elseif �͏��������K�v
			else if (sa_atom(p, sub.If, TNMS_LA_ATOM_ELSE))		loop_out = true;	// else �̓��[�v�A�E�g
			else												break;				// �I��
		}

		// ���������K�v�ȏꍇ
		if (need_cond)	{
			// '('
			if (!sa_atom(p, sub.open_p, TNMS_LA_ATOM_OPEN_PAREN))
				return error(TNMSERR_SA_IF_NO_OPEN_PAREN, sub.If->atom);
			// <������>
			if (!sa_exp(p, sub.cond, 0))
				return error(TNMSERR_SA_IF_ILLEGAL_COND, sub.If->atom);
			// ')'
			if (!sa_atom(p, sub.close_p, TNMS_LA_ATOM_CLOSE_PAREN))
				return error(TNMSERR_SA_IF_NO_CLOSE_PAREN, sub.open_p->atom);
		}
		// '{'
		if (!sa_atom(p, sub.open_b, TNMS_LA_ATOM_OPEN_BRACE))
			return error(TNMSERR_SA_IF_NO_OPEN_BRACE, sub.If->atom);
		// <�u���b�N>
		while (p->type != TNMS_LA_ATOM_NONE && p->type != TNMS_LA_ATOM_CLOSE_BRACE)	{
			BSP<TNMS_SA_NODE_SENTENCE> sentence;
			if (!sa_sentence(p, sentence))
				return error(TNMSERR_SA_IF_ILLEGAL_BLOCK, sub.If->atom);
			sub.block.push_back(sentence);
		}
		// '}'
		if (!sa_atom(p, sub.close_b, TNMS_LA_ATOM_CLOSE_BRACE))
			return error(TNMSERR_SA_IF_NO_CLOSE_BRACE, sub.open_b->atom);

		If->sub.push_back(sub);
	}

	// ��Ԃ̍X�V
	last_error = err;
	parent = If;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<for ��>
// ================================================================
bool C_tnms_sentence_analizer::sa_for(ITR& start, BSP<TNMS_SA_NODE_FOR>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_FOR> For(new TNMS_SA_NODE_FOR);
	For->node_line = p->line;

	// "for"
	if (!sa_atom(p, For->For, TNMS_LA_ATOM_FOR))
		return false;
	// '('
	if (!sa_atom(p, For->open_p, TNMS_LA_ATOM_OPEN_PAREN))
		return error(TNMSERR_SA_FOR_NO_OPEN_PAREN, For->For->atom);
	// <��������>
	while (p->type != TNMS_LA_ATOM_NONE && p->type != TNMS_LA_ATOM_COMMA)	{
		BSP<TNMS_SA_NODE_SENTENCE> sentence;
		if (!sa_sentence(p, sentence))
			return error(TNMSERR_SA_FOR_ILLEGAL_INIT, For->For->atom);
		For->init.push_back(sentence);
	}
	// ','
	if (!sa_atom(p, For->comma[0], TNMS_LA_ATOM_COMMA))
		return error(TNMSERR_SA_FOR_NO_INIT_COMMA, For->For->atom);
	// <������>
	if (!sa_exp(p, For->cond, 0))
		return error(TNMSERR_SA_FOR_ILLEGAL_COND, For->For->atom);
	// ','
	if (!sa_atom(p, For->comma[1], TNMS_LA_ATOM_COMMA))
		return error(TNMSERR_SA_FOR_NO_COND_COMMA, For->For->atom);
	// <���[�v��>
	while (p->type != TNMS_LA_ATOM_NONE && p->type != TNMS_LA_ATOM_CLOSE_PAREN)	{
		BSP<TNMS_SA_NODE_SENTENCE> sentence;
		if (!sa_sentence(p, sentence))
			return error(TNMSERR_SA_FOR_ILLEGAL_LOOP, For->For->atom);
		For->loop.push_back(sentence);
	}
	// ')'
	if (!sa_atom(p, For->close_p, TNMS_LA_ATOM_CLOSE_PAREN))
		return error(TNMSERR_SA_FOR_NO_CLOSE_PAREN, For->For->atom);
	// '{'
	if (!sa_atom(p, For->open_b, TNMS_LA_ATOM_OPEN_BRACE))
		return error(TNMSERR_SA_FOR_NO_OPEN_BRACE, For->For->atom);
	// <�u���b�N>
	while (p->type != TNMS_LA_ATOM_NONE && p->type != TNMS_LA_ATOM_CLOSE_BRACE)	{
		BSP<TNMS_SA_NODE_SENTENCE> sentence;
		if (!sa_sentence(p, sentence))
			return error(TNMSERR_SA_FOR_ILLEGAL_BLOCK, For->For->atom);
		For->block.push_back(sentence);
	}
	// '}'
	if (!sa_atom(p, For->close_b, TNMS_LA_ATOM_CLOSE_BRACE))
		return error(TNMSERR_SA_FOR_NO_CLOSE_BRACE, For->For->atom);

	// ��Ԃ̍X�V
	last_error = err;
	parent = For;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<while ��>
// ================================================================
bool C_tnms_sentence_analizer::sa_while(ITR& start, BSP<TNMS_SA_NODE_WHILE>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_WHILE> While(new TNMS_SA_NODE_WHILE);
	While->node_line = p->line;

	// "while"
	if (!sa_atom(p, While->While, TNMS_LA_ATOM_WHILE))
		return false;
	// '('
	if (!sa_atom(p, While->open_p, TNMS_LA_ATOM_OPEN_PAREN))
		return error(TNMSERR_SA_WHILE_NO_OPEN_PAREN, While->While->atom);
	// <������>
	if (!sa_exp(p, While->cond, 0))
		return error(TNMSERR_SA_WHILE_ILLEGAL_COND, While->While->atom);
	// ')'
	if (!sa_atom(p, While->close_p, TNMS_LA_ATOM_CLOSE_PAREN))
		return error(TNMSERR_SA_WHILE_NO_CLOSE_PAREN, While->While->atom);
	// '{'
	if (!sa_atom(p, While->open_b, TNMS_LA_ATOM_OPEN_BRACE))
		return error(TNMSERR_SA_WHILE_NO_OPEN_BRACE, While->While->atom);
	// <��*>
	while (p->type != TNMS_LA_ATOM_NONE && p->type != TNMS_LA_ATOM_CLOSE_BRACE)	{
		BSP<TNMS_SA_NODE_SENTENCE> sentence;
		if (!sa_sentence(p, sentence))
			return error(TNMSERR_SA_WHILE_ILLEGAL_BLOCK, While->While->atom);
		While->block.push_back(sentence);
	}
	// '}'
	if (!sa_atom(p, While->close_b, TNMS_LA_ATOM_CLOSE_BRACE))
		return error(TNMSERR_SA_WHILE_NO_CLOSE_BRACE, While->While->atom);

	// ��Ԃ̍X�V
	last_error = err;
	parent = While;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<continue ��>
// ================================================================
bool C_tnms_sentence_analizer::sa_continue(ITR& start, BSP<TNMS_SA_NODE_CONTINUE>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_CONTINUE> Continue(new TNMS_SA_NODE_CONTINUE);
	Continue->node_line = p->line;

	if (!sa_atom(p, Continue->Continue, TNMS_LA_ATOM_CONTINUE))
		return false;

	// ��Ԃ̍X�V
	last_error = err;
	parent = Continue;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<break ��>
// ================================================================
bool C_tnms_sentence_analizer::sa_break(ITR& start, BSP<TNMS_SA_NODE_BREAK>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_BREAK> Break(new TNMS_SA_NODE_BREAK);
	Break->node_line = p->line;

	if (!sa_atom(p, Break->Break, TNMS_LA_ATOM_BREAK))
		return false;

	// ��Ԃ̍X�V
	last_error = err;
	parent = Break;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<switch ��>
// ================================================================
bool C_tnms_sentence_analizer::sa_switch(ITR& start, BSP<TNMS_SA_NODE_SWITCH>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_SWITCH> Switch(new TNMS_SA_NODE_SWITCH);
	Switch->node_line = p->line;

	// "switch"
	if (!sa_atom(p, Switch->Switch, TNMS_LA_ATOM_SWITCH))
		return false;
	// '('
	if (!sa_atom(p, Switch->open_p, TNMS_LA_ATOM_OPEN_PAREN))
		return error(TNMSERR_SA_SWITCH_NO_OPEN_PAREN, Switch->Switch->atom);
	// <������>
	if (!sa_exp(p, Switch->cond, 0))
		return error(TNMSERR_SA_SWITCH_ILLEGAL_COND, Switch->Switch->atom);
	// ')'
	if (!sa_atom(p, Switch->close_p, TNMS_LA_ATOM_CLOSE_PAREN))
		return error(TNMSERR_SA_SWITCH_NO_CLOSE_PAREN, Switch->Switch->atom);
	// '{'
	if (!sa_atom(p, Switch->open_b, TNMS_LA_ATOM_OPEN_BRACE))
		return error(TNMSERR_SA_SWITCH_NO_OPEN_BRACE, Switch->Switch->atom);
	// <(case �� | default ��)*>
	while (p->type != TNMS_LA_ATOM_NONE && p->type != TNMS_LA_ATOM_CLOSE_BRACE)	{
		BSP<TNMS_SA_NODE_CASE> Case;
		BSP<TNMS_SA_NODE_DEFAULT> Default;

		if (false);
		else if (sa_case(p, Case))	{
			Switch->Case.push_back(Case);
		}
		else if (sa_default(p, Default))	{
			// ���� default �����������ꍇ�G���[
			if (Switch->Default)
				return error(TNMSERR_SA_DEFAULT_REDEFINE, Default->Default->atom);

			Switch->Default = Default;
		}

		else
			return error(TNMSERR_SA_SWITCH_ILLEGAL_CASE, Switch->Switch->atom);
	}
	// '}'
	if (!sa_atom(p, Switch->close_b, TNMS_LA_ATOM_CLOSE_BRACE))
		return error(TNMSERR_SA_SWITCH_NO_CLOSE_BRACE, Switch->Switch->atom);

	// ��Ԃ̍X�V
	last_error = err;
	parent = Switch;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<case ��>
// ================================================================
bool C_tnms_sentence_analizer::sa_case(ITR& start, BSP<TNMS_SA_NODE_CASE>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_CASE> Case(new TNMS_SA_NODE_CASE);
	Case->node_line = p->line;

	// "case"
	if (!sa_atom(p, Case->Case, TNMS_LA_ATOM_CASE))
		return false;
	// '('
	if (!sa_atom(p, Case->open_p, TNMS_LA_ATOM_OPEN_PAREN))
		return error(TNMSERR_SA_CASE_NO_OPEN_PAREN, Case->Case->atom);
	// <��>
	if (!sa_exp(p, Case->value, 0))
		return error(TNMSERR_SA_CASE_ILLEGAL_VALUE, Case->Case->atom);
	// ')'
	if (!sa_atom(p, Case->close_p, TNMS_LA_ATOM_CLOSE_PAREN))
		return error(TNMSERR_SA_CASE_NO_CLOSE_PAREN, Case->Case->atom);
	// <��*>
	while (p->type != TNMS_LA_ATOM_NONE && p->type != TNMS_LA_ATOM_CASE && p->type != TNMS_LA_ATOM_DEFAULT && p->type != TNMS_LA_ATOM_CLOSE_BRACE)	{
		BSP<TNMS_SA_NODE_SENTENCE> sentence;
		if (!sa_sentence(p, sentence))
			return false;
		Case->block.push_back(sentence);
	}

	// ��Ԃ̍X�V
	last_error = err;
	parent = Case;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<default ��>
// ================================================================
bool C_tnms_sentence_analizer::sa_default(ITR& start, BSP<TNMS_SA_NODE_DEFAULT>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_DEFAULT> Default(new TNMS_SA_NODE_DEFAULT);
	Default->node_line = p->line;

	// "default"
	if (!sa_atom(p, Default->Default, TNMS_LA_ATOM_DEFAULT))
		return false;
	// <��*>
	while (p->type != TNMS_LA_ATOM_NONE && p->type != TNMS_LA_ATOM_CASE && p->type != TNMS_LA_ATOM_DEFAULT && p->type != TNMS_LA_ATOM_CLOSE_BRACE)	{
		BSP<TNMS_SA_NODE_SENTENCE> sentence;
		if (!sa_sentence(p, sentence))
			return false;
		Default->block.push_back(sentence);
	}

	// ��Ԃ̍X�V
	last_error = err;
	parent = Default;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�R�}���h��> / <�����>
// ================================================================
//		�R�}���h���Ƒ�����͂ǂ�����擪�� <�G�������g��> �Ȃ̂ŁA
//		�ꏏ�ɉ�͂�i�߂čs���܂��B
// ================================================================
bool C_tnms_sentence_analizer::sa_command_or_assign(ITR& start, BSP<TNMS_SA_NODE_COMMAND>& command_parent, BSP<TNMS_SA_NODE_ASSIGN>& assign_parent)
{
	ITR p = start;
	SA_ERROR err = last_error;

	// <�G�������g��>
	BSP<TNMS_SA_NODE_ELM_EXP> elm_exp(new TNMS_SA_NODE_ELM_EXP);
	if (!sa_elm_exp(p, elm_exp))
		return false;

	// <������Z�q>
	BSP<TNMS_SA_NODE_ATOM> equal(new TNMS_SA_NODE_ATOM);
	if (sa_assign_operator(p, equal))	{

		// ������Z�q���������̂ő�����̑������s��
		BSP<TNMS_SA_NODE_ASSIGN> assign(new TNMS_SA_NODE_ASSIGN);
		assign->node_line = p->line;
		assign->left = elm_exp;
		assign->equal = equal;

		// <�E��>
		if (!sa_exp(p, assign->right, 0))
			return error(TNMSERR_SA_ASSIGN_ILLEGAL_RIGHT, *p);

		// ��Ԃ̍X�V
		last_error = err;
		assign_parent = assign;
		start = p;
		return true;
	}

	else	{

		// ������Z�q���Ȃ������̂ŃR�}���h��
		BSP<TNMS_SA_NODE_COMMAND> command(new TNMS_SA_NODE_COMMAND);
		command->node_line = p->line;
		command->command = elm_exp;

		// ��Ԃ̍X�V
		last_error = err;
		command_parent = command;
		start = p;
		return true;
	}
}

// ****************************************************************
// �\����́F<��>
// ================================================================
bool C_tnms_sentence_analizer::sa_exp(ITR& start, BSP<TNMS_SA_NODE_EXP>& parent, int last_prioroty)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_EXP> exp(new TNMS_SA_NODE_EXP);
	exp->node_line = p->line;

	if (false);

	// <�O�u�P�����Z�q>
	else if (sa_operator_1(p, exp->opr))	{
		// <��>
		if (!sa_exp(p, exp->exp_1, TNMS_SA_OPERATOR_PRIORITY_MAX))
			return error(TNMSERR_SA_EXP_ILLEGAL, *p);
		// ���̃^�C�v�F�P�����Z
		exp->node_type = TNMS_SA_NODE_TYPE_EXP_OPR_1;
	}
	// <�P����>
	else if (sa_smp_exp(p, exp->smp_exp))	{
		// ���̃^�C�v�F�P����
		exp->node_type = TNMS_SA_NODE_TYPE_EXP_SIMPLE;
	}
	else
		return false;

	// <�񍀉��Z�q>
	BSP<TNMS_SA_NODE_ATOM> opr;
	int new_priority;
	while (sa_operator_2(p, opr, last_prioroty, &new_priority))	{
		// �D��x�̒Ⴂ���Z�q�Ȃ炱���ŏI��

		// �D��x�̍������Z�q�Ȃ�q���ς�
		BSP<TNMS_SA_NODE_EXP> exp_new(new TNMS_SA_NODE_EXP);
		exp_new->node_line = p->line;
		exp_new->exp_1 = exp;
		exp_new->opr = opr;
		exp = exp_new;

		// <��>
		if (!sa_exp(p, exp->exp_2, new_priority))
			return error(TNMSERR_SA_EXP_ILLEGAL, *p);

		// ���̃^�C�v�F�񍀉��Z
		exp->node_type = TNMS_SA_NODE_TYPE_EXP_OPR_2;
	}

	// ��Ԃ̍X�V
	last_error = err;
	parent = exp;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�����X�g>
// ================================================================
bool C_tnms_sentence_analizer::sa_exp_list(ITR& start, BSP<TNMS_SA_NODE_EXP_LIST>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_EXP_LIST> exp_list(new TNMS_SA_NODE_EXP_LIST);
	exp_list->node_line = p->line;

	// '['
	if (!sa_atom(p, exp_list->open_b, TNMS_LA_ATOM_OPEN_BRACKET))
		return false;

	while (1)	{

		// <��>
		BSP<TNMS_SA_NODE_EXP> exp;
		if (!sa_exp(p, exp, 0))
			return error(TNMSERR_SA_EXP_ILLEGAL, *p);				// �G���[�F�s���Ȏ�

		exp_list->exp.push_back(exp);

		// ']'
		if (sa_atom(p, exp_list->close_b, TNMS_LA_ATOM_CLOSE_BRACKET))
			break;

		// ','
		BSP<TNMS_SA_NODE_ATOM> comma;
		if (!sa_atom(p, comma, TNMS_LA_ATOM_COMMA))
			return error(TNMSERR_SA_EXP_LIST_NO_CLOSE_BRACKET, *p);	// �G���[�F'}' ���Ȃ�

		exp_list->comma.push_back(comma);
	}

	// ��Ԃ̍X�V
	last_error = err;
	parent = exp_list;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�P����>
// ================================================================
//		() �ň͂܂ꂽ��		// (a[0] + b[0])
//		�����X�g			// {1, 2, 3}
//		goto ��				// gosub #label
//		���e����			// 1, "abc"
//		�G�������g��		// object[0].x, object[0].load("abc", 1, 0, 0)
// ================================================================
bool C_tnms_sentence_analizer::sa_smp_exp(ITR& start, BSP<TNMS_SA_NODE_SMP_EXP>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_SMP_EXP> smp_exp(new TNMS_SA_NODE_SMP_EXP);
	smp_exp->node_line = p->line;

	if (false);
	// '('
	else if (sa_atom(p, smp_exp->open, TNMS_LA_ATOM_OPEN_PAREN))	{
		// <��>
		if (!sa_exp(p, smp_exp->exp, 0))
			return error(TNMSERR_SA_EXP_ILLEGAL, *p);						// �G���[�F�s���Ȏ�
		// ')'
		if (!sa_atom(p, smp_exp->close, TNMS_LA_ATOM_CLOSE_PAREN))
			return error(TNMSERR_SA_SMP_EXP_NO_CLOSE_PAREN, smp_exp->open->atom);	// �G���[�F')' ���Ȃ�

		smp_exp->node_type = TNMS_SA_NODE_TYPE_SMP_EXP_KAKKO;
	}
	// <�����X�g>
	else if (sa_exp_list(p, smp_exp->exp_list))	{
		smp_exp->node_type = TNMS_SA_NODE_TYPE_SMP_EXP_EXP_LIST;
	}
	// <goto ��>
	else if (sa_goto(p, smp_exp->Goto))	{
		smp_exp->node_type = TNMS_SA_NODE_TYPE_SMP_EXP_GOTO;
	}
	// <���e����>
	else if (sa_literal(p, smp_exp->Literal))	{
		smp_exp->node_type = TNMS_SA_NODE_TYPE_SMP_EXP_LITERAL;
	}
	// <�G�������g��>
	else if (sa_elm_exp(p, smp_exp->elm_exp))	{
		smp_exp->node_type = TNMS_SA_NODE_TYPE_SMP_EXP_ELM_EXP;
	}
	// ���s
	else
		return false;

	// ��Ԃ̍X�V
	last_error = err;
	parent = smp_exp;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�^��>
// ================================================================
bool C_tnms_sentence_analizer::sa_form(ITR& start, BSP<TNMS_SA_NODE_FORM>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_FORM> form(new TNMS_SA_NODE_FORM);
	form->node_line = p->line;

	// �^
	if (!sa_atom(p, form->form, TNMS_LA_ATOM_UNKNOWN))
		return false;

	form->form_code = get_form_code_by_name(plad->unknown_list[form->form->atom.opt]);
	if (form->form_code == -1)
		return error(TNMSERR_SA_DEF_PROP_ILLEGAL_FORM, *p);

	// �T�C�Y
	if (sa_atom(p, form->open_b, TNMS_LA_ATOM_OPEN_BRACKET))	{

		// �Y����
		if (!sa_exp(p, form->index, 0))
			return false;

		// ']'
		if (!sa_atom(p, form->close_b, TNMS_LA_ATOM_CLOSE_BRACKET))
			return error(TNMSERR_SA_DEF_PROP_NO_CLOSE_BRACKET);
	}

	// ��Ԃ̍X�V
	last_error = err;
	parent = form;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�G�������g��>
// ================================================================
//		ex. front.object[0].x
//		ex. front.object[0].load("abc", 1, 0, 0)
// ================================================================
bool C_tnms_sentence_analizer::sa_elm_exp(ITR& start, BSP<TNMS_SA_NODE_ELM_EXP>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_ELM_EXP> elm_exp(new TNMS_SA_NODE_ELM_EXP);
	elm_exp->node_line = p->line;

	// <�G�������g���X�g>
	if (!sa_elm_list(p, elm_exp->elm_list))
		return false;

	// ��Ԃ̍X�V
	last_error = err;
	parent = elm_exp;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�G�������g���X�g>
// ================================================================
//		ex. front.object[0].x
//		ex. front.object[0].load("abc", 1, 0, 0)
// ================================================================
bool C_tnms_sentence_analizer::sa_elm_list(ITR& start, BSP<TNMS_SA_NODE_ELM_LIST>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_ELM_LIST> elm_list(new TNMS_SA_NODE_ELM_LIST);
	elm_list->node_line = p->line;

	// <�G�������g>
	BSP<TNMS_SA_NODE_ELEMENT> element;
	if (!sa_element(p, element, true))
		return false;

	elm_list->element.push_back(element);		// �G�������g��ǉ�

	// '[' �� '.' �������ꍇ���q���G�������g
	while (p->type == TNMS_LA_ATOM_OPEN_BRACKET || p->type == TNMS_LA_ATOM_DOT)	{

		// <�q���G�������g>
		BSP<TNMS_SA_NODE_ELEMENT> element;
		if (!sa_element(p, element, false))
			return false;

		elm_list->element.push_back(element);		// �G�������g��ǉ�
	}

	// ��Ԃ̍X�V
	last_error = err;
	parent = elm_list;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�G�������g>
// ================================================================
//		ex.		object			// �v���p�e�B		: top_element == true �̏ꍇ�̂�
//		ex.		load("BG001")	// �R�}���h			: top_element == true �̏ꍇ�̂�
//		ex.		[0]				// �Y����			: top_element == false �̏ꍇ�̂�
//		ex.		. <�G�������g>	// �q���G�������g	: top_element == false �̏ꍇ�̂�
// ================================================================
bool C_tnms_sentence_analizer::sa_element(ITR& start, BSP<TNMS_SA_NODE_ELEMENT>& parent, bool top_element)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_ELEMENT> element(new TNMS_SA_NODE_ELEMENT);
	element->node_line = p->line;

	if (false);

	// '['	// �Y�����i�ǉ��G�������g�̂݁j
	else if (!top_element && sa_atom(p, element->open_b, TNMS_LA_ATOM_OPEN_BRACKET))	{
		
		// <��>
		if (!sa_exp(p, element->exp, 0))
			return error(TNMSERR_SA_ELEMENT_ILLEGAL_EXP, *p);

		// ']'
		if (!sa_atom(p, element->close_b, TNMS_LA_ATOM_CLOSE_BRACKET))
			return error(TNMSERR_SA_ELEMENT_NO_CLOSE, *p);

		element->node_type = TNMS_SA_NODE_TYPE_ELEMENT_ARRAY;	// �Y����
	}

	// '.'	// �q���G�������g�i�ǉ��G�������g�̂݁j
	else if (!top_element && sa_atom(p, element->dot, TNMS_LA_ATOM_DOT))	{

		// <�G�������g>
		if (!sa_element(p, element, true))
			return error(TNMSERR_SA_ELEMENT_NO_CHILD, *p);
	}

	// <�G�������g��>�i�擪�G�������g�̂݁j
	else if (top_element && sa_atom(p, element->name, TNMS_LA_ATOM_UNKNOWN))	{

		// �G�������g�̏���ݒ�
		element->node_type = TNMS_SA_NODE_TYPE_ELEMENT_ELEMENT;	// �G�������g

		// <�������X�g>
		if (!sa_arg_list(p, element->arg_list))
			return false;
	}

	// ���̑�
	else
		return false;

	// ��Ԃ̍X�V
	last_error = err;
	parent = element;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<�������X�g>
// ================================================================
bool C_tnms_sentence_analizer::sa_arg_list(ITR& start, BSP<TNMS_SA_NODE_ARG_LIST>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_ARG_LIST> arg_list(new TNMS_SA_NODE_ARG_LIST);
	arg_list->node_line = p->line;
	arg_list->named_arg_cnt = 0;

	// '('
	if (sa_atom(p, arg_list->open_p, TNMS_LA_ATOM_OPEN_PAREN))	{

		// ')'
		if (sa_atom(p, arg_list->close_p, TNMS_LA_ATOM_CLOSE_PAREN))	{

			// �����Ȃ��ŏI���I
		}
		else	{

			while (1)	{

				// <����>
				BSP<TNMS_SA_NODE_ARG> arg;
				if (!sa_arg(p, arg))
					return error(TNMSERR_SA_EXP_ILLEGAL, *p);				// �G���[�F�s���Ȏ�

				arg_list->arg.push_back(arg);

				if (arg->node_type == TNMS_SA_NODE_TYPE_ARG_WITH_NAME)
					arg_list->named_arg_cnt ++;

				// ')'
				if (sa_atom(p, arg_list->close_p, TNMS_LA_ATOM_CLOSE_PAREN))
					break;	// �������X�g�I��

				// ','
				BSP<TNMS_SA_NODE_ATOM> comma;
				if (!sa_atom(p, comma, TNMS_LA_ATOM_COMMA))
					return error(TNMSERR_SA_ARG_LIST_NO_CLOSE_PAREN, *p);	// �G���[�F')' ���Ȃ�

				arg_list->comma.push_back(comma);
			}
		}
	}

	// ���O�t�����������ɕ��ёւ���
	ARRAY< BSP<TNMS_SA_NODE_ARG> > normal;
	ARRAY< BSP<TNMS_SA_NODE_ARG> > with_name;
	for (int a = 0; a < (int)arg_list->arg.size(); a++)	{
		if (arg_list->arg[a]->node_type == TNMS_SA_NODE_TYPE_ARG_WITH_NAME)	{
			with_name.push_back(arg_list->arg[a]);
		}
		else	{
			normal.push_back(arg_list->arg[a]);
		}
	}
	arg_list->arg.clear();
	arg_list->arg.assign(normal.begin(), normal.end());
	arg_list->arg.insert(arg_list->arg.end(), with_name.begin(), with_name.end());

	// ���O�����������ɂ��邩�ǂ������`�F�b�N
#if 0
	bool find_named_arg = false;
	for (int a = 0; a < (int)arg_list->arg.size(); a++)	{

		if (arg_list->arg[a]->node_type == TNMS_SA_NODE_TYPE_ARG_WITH_NAME)	{
			find_named_arg = true;
		}
		else	{
			if (find_named_arg)	{
				if (arg_list->arg[a]->name)	{
					return error(TNMSERR_SA_ARG_LIST_NAMED_ARG_EXIST_BEFORE, arg_list->arg[a]->name->atom);
				}
				else	{
					return error(TNMSERR_SA_ARG_LIST_NAMED_ARG_EXIST_BEFORE, arg_list->open_p->atom);
				}
			}
		}
	}
#endif

	// ��Ԃ̍X�V
	last_error = err;
	parent = arg_list;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<����>
// ================================================================
bool C_tnms_sentence_analizer::sa_arg(ITR& start, BSP<TNMS_SA_NODE_ARG>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_ARG> arg(new TNMS_SA_NODE_ARG);
	arg->node_line = p->line;

	// <���O������>
	if (sa_named_arg(p, arg))	{

		arg->node_type = TNMS_SA_NODE_TYPE_ARG_WITH_NAME;
	}
	else	{

		// ���O�������Ƃ��Ď��s�����̂ŁA���O�Ȃ������Ƃ��ĉ�͂��Ă݂�

		// <��>
		if (!sa_exp(p, arg->exp, 0))
			return error(TNMSERR_SA_EXP_ILLEGAL, *p);				// �G���[�F�s���Ȏ�;

		arg->node_type = TNMS_SA_NODE_TYPE_ARG_NO_NAME;
	}

	// ��Ԃ̍X�V
	last_error = err;
	parent = arg;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<���O������>
// ================================================================
bool C_tnms_sentence_analizer::sa_named_arg(ITR& start, BSP<TNMS_SA_NODE_ARG>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_ARG> arg(new TNMS_SA_NODE_ARG);
	arg->node_line = p->line;

	// ���O
	if (!sa_atom(p, arg->name, TNMS_LA_ATOM_UNKNOWN))
		return false;

	// '='
	if (!sa_atom(p, arg->equal, TNMS_LA_ATOM_ASSIGN))
		return false;

	// <��>
	if (!sa_exp(p, arg->exp, 0))
		return error(TNMSERR_SA_EXP_ILLEGAL, *p);				// �G���[�F�s���Ȏ�;

	// ��Ԃ̍X�V
	last_error = err;
	parent = arg;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<���O>
// ================================================================
bool C_tnms_sentence_analizer::sa_name(ITR& start, BSP<TNMS_SA_NODE_NAME>& parent)
{
	ITR p = start;
	SA_ERROR err = last_error;
	BSP<TNMS_SA_NODE_NAME> name(new TNMS_SA_NODE_NAME);
	name->node_line = p->line;

	// '�y'
	if (!sa_atom(p, name->open_s, TNMS_LA_ATOM_OPEN_SUMI))
		return false;

	// <�����񃊃e����>
	if (!sa_atom(p, name->name, TNMS_LA_ATOM_VAL_STR))
		return error(TNMSERR_SA_NAME_ILLEGAL_NAME, *p);

	// '�z'
	if (!sa_atom(p, name->close_s, TNMS_LA_ATOM_CLOSE_SUMI))
		return error(TNMSERR_SA_NAME_NO_CLOSE_SUMI, *p);


	// ��Ԃ̍X�V
	last_error = err;
	parent = name;
	start = p;
	return true;
}

// ****************************************************************
// �\����́F<���e����>
// ================================================================
bool C_tnms_sentence_analizer::sa_literal(ITR& p, BSP<TNMS_SA_NODE_ATOM>& Literal)
{
	if (false);
	else if (sa_atom(p, Literal, TNMS_LA_ATOM_VAL_INT));
	else if (sa_atom(p, Literal, TNMS_LA_ATOM_VAL_STR));
	else if (sa_atom(p, Literal, TNMS_LA_ATOM_LABEL));
	else
		return false;

	return true;
}

// ****************************************************************
// �\����́F<�P�����Z�q>
// ================================================================
bool C_tnms_sentence_analizer::sa_operator_1(ITR& p, BSP<TNMS_SA_NODE_ATOM>& opr)
{
	if (false);
	else if (sa_atom(p, opr, TNMS_LA_ATOM_PLUS))	{	opr->atom.opt = OP_PLUS;	}
	else if (sa_atom(p, opr, TNMS_LA_ATOM_MINUS))	{	opr->atom.opt = OP_MINUS;	}
	else if (sa_atom(p, opr, TNMS_LA_ATOM_TILDE))	{	opr->atom.opt = OP_TILDE;	}
	else
		return false;

	return true;
}

// ****************************************************************
// �\����́F<�񍀉��Z�q>
// ================================================================
bool C_tnms_sentence_analizer::sa_operator_2(ITR& p, BSP<TNMS_SA_NODE_ATOM>& opr, int last_priority, int* new_priority)
{
	switch (last_priority)	{
		case 0:
			if (sa_atom(p, opr, TNMS_LA_ATOM_LOGICAL_OR))		{	*new_priority = 1;	opr->atom.opt = OP_LOGICAL_OR;		break;	}
		case 1:
			if (sa_atom(p, opr, TNMS_LA_ATOM_LOGICAL_AND))		{	*new_priority = 2;	opr->atom.opt = OP_LOGICAL_AND;		break;	}
		case 2:
			if (sa_atom(p, opr, TNMS_LA_ATOM_OR))				{	*new_priority = 3;	opr->atom.opt = OP_OR;				break;	}
		case 3:
			if (sa_atom(p, opr, TNMS_LA_ATOM_HAT))				{	*new_priority = 4;	opr->atom.opt = OP_HAT;				break;	}
		case 4:
			if (sa_atom(p, opr, TNMS_LA_ATOM_AND))				{	*new_priority = 5;	opr->atom.opt = OP_AND;				break;	}
		case 5:
			if (sa_atom(p, opr, TNMS_LA_ATOM_EQUAL))			{	*new_priority = 6;	opr->atom.opt = OP_EQUAL;			break;	}
			if (sa_atom(p, opr, TNMS_LA_ATOM_NOT_EQUAL))		{	*new_priority = 6;	opr->atom.opt = OP_NOT_EQUAL;		break;	}
		case 6:
			if (sa_atom(p, opr, TNMS_LA_ATOM_GREATER))			{	*new_priority = 7;	opr->atom.opt = OP_GREATER;			break;	}
			if (sa_atom(p, opr, TNMS_LA_ATOM_GREATER_EQUAL))	{	*new_priority = 7;	opr->atom.opt = OP_GREATER_EQUAL;	break;	}
			if (sa_atom(p, opr, TNMS_LA_ATOM_LESS))				{	*new_priority = 7;	opr->atom.opt = OP_LESS;			break;	}
			if (sa_atom(p, opr, TNMS_LA_ATOM_LESS_EQUAL))		{	*new_priority = 7;	opr->atom.opt = OP_LESS_EQUAL;		break;	}
		case 7:
			if (sa_atom(p, opr, TNMS_LA_ATOM_SL))				{	*new_priority = 8;	opr->atom.opt = OP_SL;				break;	}
			if (sa_atom(p, opr, TNMS_LA_ATOM_SR))				{	*new_priority = 8;	opr->atom.opt = OP_SR;				break;	}
			if (sa_atom(p, opr, TNMS_LA_ATOM_SR3))				{	*new_priority = 8;	opr->atom.opt = OP_SR3;				break;	}
		case 8:
			if (sa_atom(p, opr, TNMS_LA_ATOM_PLUS))				{	*new_priority = 9;	opr->atom.opt = OP_PLUS;			break;	}
			if (sa_atom(p, opr, TNMS_LA_ATOM_MINUS))			{	*new_priority = 9;	opr->atom.opt = OP_MINUS;			break;	}
		case 9:
			if (sa_atom(p, opr, TNMS_LA_ATOM_MULTIPLE))			{	*new_priority = 10;	opr->atom.opt = OP_MULTIPLE;		break;	}
			if (sa_atom(p, opr, TNMS_LA_ATOM_DIVIDE))			{	*new_priority = 10;	opr->atom.opt = OP_DIVIDE;			break;	}
			if (sa_atom(p, opr, TNMS_LA_ATOM_PERCENT))			{	*new_priority = 10;	opr->atom.opt = OP_AMARI;			break;	}
		default:
			return false;
	}

	return true;
}

// ****************************************************************
// �\����́F<������Z�q>
// ================================================================
bool C_tnms_sentence_analizer::sa_assign_operator(ITR& p, BSP<TNMS_SA_NODE_ATOM>& opr)
{
	if (false);
	else if (sa_atom(p, opr, TNMS_LA_ATOM_ASSIGN))			opr->atom.opt = OP_NONE;
	else if (sa_atom(p, opr, TNMS_LA_ATOM_PLUS_ASSIGN))		opr->atom.opt = OP_PLUS;
	else if (sa_atom(p, opr, TNMS_LA_ATOM_MINUS_ASSIGN))	opr->atom.opt = OP_MINUS;
	else if (sa_atom(p, opr, TNMS_LA_ATOM_MULTIPLE_ASSIGN))	opr->atom.opt = OP_MULTIPLE;
	else if (sa_atom(p, opr, TNMS_LA_ATOM_DIVIDE_ASSIGN))	opr->atom.opt = OP_DIVIDE;
	else if (sa_atom(p, opr, TNMS_LA_ATOM_PERCENT_ASSIGN))	opr->atom.opt = OP_AMARI;

	else if (sa_atom(p, opr, TNMS_LA_ATOM_AND_ASSIGN))		opr->atom.opt = OP_AND;
	else if (sa_atom(p, opr, TNMS_LA_ATOM_OR_ASSIGN))		opr->atom.opt = OP_OR;
	else if (sa_atom(p, opr, TNMS_LA_ATOM_HAT_ASSIGN))		opr->atom.opt = OP_HAT;
	else if (sa_atom(p, opr, TNMS_LA_ATOM_SL_ASSIGN))		opr->atom.opt = OP_SL;
	else if (sa_atom(p, opr, TNMS_LA_ATOM_SR_ASSIGN))		opr->atom.opt = OP_SR;
	else if (sa_atom(p, opr, TNMS_LA_ATOM_SR3_ASSIGN))		opr->atom.opt = OP_SR3;
	else
		return false;

	return true;
}

// ****************************************************************
// �\����́F<LA�A�g��>
// ================================================================
bool C_tnms_sentence_analizer::sa_atom(ITR& start, BSP<TNMS_SA_NODE_ATOM>& parent, TNMS_LA_ATOM_TYPE type)
{
	if (start->type != type)	// �������̂��߂ɐ�ɍs��
		return false;

	//BSP<TNMS_SA_NODE_ATOM> atom(new TNMS_SA_NODE_ATOM);
	BSP<TNMS_SA_NODE_ATOM> atom(new_alloc<TNMS_SA_NODE_ATOM>(), delete_free<TNMS_SA_NODE_ATOM>);
	atom->node_line = start->line;
	atom->atom = *start++;

	// ��Ԃ̍X�V
	parent = atom;
	return true;
}

// ****************************************************************
// �\����́F�G���[�̐ݒ�
// ================================================================
bool C_tnms_sentence_analizer::error(TNMSERR_SA type, S_tnms_atom atom)
{
	// �����ɃG���[�����݂���ꍇ�́A�G���[�ݒ���s��Ȃ�
	if (last_error.type == TNMSERR_SA_NONE || last_error.atom.id < atom.id)	{
		last_error.type = type;
		last_error.atom = atom;
	}

	return false;
}

bool C_tnms_sentence_analizer::error(TNMSERR_SA type)
{
	last_error.type = type;

	return false;
}

// ****************************************************************
// �\����́F�G���[�̃N���A
// ================================================================
void C_tnms_sentence_analizer::clear_error()
{
	last_error.type = TNMSERR_SA_NONE;
}

// ****************************************************************
// �\����́F�G���[�̎擾
// ================================================================
S_tnms_atom C_tnms_sentence_analizer::get_error_atom()
{
	return last_error.atom;
}

int C_tnms_sentence_analizer::get_error_line()
{
	return last_error.atom.line;
}

TSTR C_tnms_sentence_analizer::get_error_str()
{
	// �G���[�̐ݒ�
	switch (last_error.type)	{
		case TNMSERR_SA_BLOCK_ILLEGAL_SENTENCE:		return _T("���̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_BLOCK_NO_CLOSE_BRACE:		return _T("�u���b�N�̏I���� } ������܂���B");
		case TNMSERR_SA_SENTENCE_ILLEGAL:			return _T("���̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_LABEL_NOT_EXIST:			return _T("���݂��Ȃ����x���ł��B");
		case TNMSERR_SA_LABEL_OVERLAPPED:			return _T("�������x������������܂��B");
		case TNMSERR_SA_Z_LABEL_OVERLAPPED:			return _T("�������x������������܂��B");
		case TNMSERR_SA_Z_LABEL_00_NOT_EXIST:		return _T("#z00 �����݂��܂���B");
		case TNMSERR_SA_DEF_CMD_NOT_EXIST:			return _T("�錾����Ă��Ȃ��R�}���h������܂��B");
		case TNMSERR_SA_DEF_CMD_ILLEGAL_FORM:		return _T("��`����Ă��Ȃ��^�ł��B");
		case TNMSERR_SA_DEF_CMD_ILLEGAL_NAME:		return _T("�R�}���h��`���̖��O��������܂���B");
		case TNMSERR_SA_DEF_CMD_ALREADY_DEFINED:	return _T("���̃R�}���h�͊��ɒ�`����Ă��܂��B");
		case TNMSERR_SA_DEF_CMD_TYPE_NO_MATCH:		return _T("inc �t�@�C���ł̐錾�ƃR�}���h�̌^����v���܂���B");
		case TNMSERR_SA_DEF_CMD_ARG_TYPE_NO_MATCH:	return _T("inc �t�@�C���ł̐錾�ƃR�}���h�̈����̌^����v���܂���B");
		case TNMSERR_SA_DEF_CMD_UNKNOWN_COMMAND:	return _T("���̃R�}���h�̐錾�� inc �t�@�C���Ɍ�����܂���B");
		case TNMSERR_SA_DEF_CMD_NO_CLOSE_PAREN:		return _T("command ���� '(' �������Ă��܂���B");
		case TNMSERR_SA_DEF_CMD_ILLEGAL_ARG:		return _T("command ���̈����J�n�� property ��������܂���ł����B");
		case TNMSERR_SA_DEF_CMD_NO_COMMA:			return _T("command ���̈����� ',' �܂��� ')' ��������܂���B");
		case TNMSERR_SA_DEF_CMD_NO_OPEN_BRACE:		return _T("command ���� '{' ��������܂���B");
		case TNMSERR_SA_DEF_CMD_NO_CLOSE_BRACE:		return _T("command ���� '{' �������Ă܂���B");
		case TNMSERR_SA_DEF_CMD_ILLEGAL_BLOCK:		return _T("command ���̃u���b�N�̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_DEF_PROP_ILLEGAL_FORM:		return _T("��`����Ă��Ȃ��^�ł��B");
		case TNMSERR_SA_DEF_PROP_ILLEGAL_NAME:		return _T("�v���p�e�B�̖��O�̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_DEF_PROP_NO_CLOSE_BRACKET:	return _T("�z��̓Y�����������Ă��܂���B");
		case TNMSERR_SA_GOTO_NO_LABEL:				return _T("goto/gosub �ɑ������x����������܂���B");
		case TNMSERR_SA_RETURN_ILLEGAL_EXP:			return _T("return ���̎��̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_RETURN_NO_CLOSE_PAREN:		return _T("return ���� '(' �������Ă��܂���B");
		case TNMSERR_SA_IF_NO_OPEN_PAREN:			return _T("if ���� '(' ��������܂���B");
		case TNMSERR_SA_IF_NO_CLOSE_PAREN:			return _T("if ���� '(' �������Ă��܂���B");
		case TNMSERR_SA_IF_ILLEGAL_COND:			return _T("if ���̏������̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_IF_NO_OPEN_BRACE:			return _T("if ���� '{' ��������܂���B");
		case TNMSERR_SA_IF_NO_CLOSE_BRACE:			return _T("if ���� '{' �������Ă��܂���B");
		case TNMSERR_SA_IF_ILLEGAL_BLOCK:			return _T("if ���̃u���b�N�̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_FOR_NO_OPEN_PAREN:			return _T("for ���� '(' ��������܂���B");
		case TNMSERR_SA_FOR_NO_CLOSE_PAREN:			return _T("for ���� '(' �������Ă��܂���B");
		case TNMSERR_SA_FOR_ILLEGAL_INIT:			return _T("for ���̏��������̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_FOR_ILLEGAL_COND:			return _T("for ���̏������̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_FOR_ILLEGAL_LOOP:			return _T("for ���̃��[�v���̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_FOR_NO_INIT_COMMA:			return _T("for ���̂P�ڂ̃R���}��������܂���B");
		case TNMSERR_SA_FOR_NO_COND_COMMA:			return _T("for ���̂Q�ڂ̃R���}��������܂���B");
		case TNMSERR_SA_FOR_NO_OPEN_BRACE:			return _T("for ���� '{' ��������܂���B");
		case TNMSERR_SA_FOR_NO_CLOSE_BRACE:			return _T("for ���� '{' �������Ă��܂���B");
		case TNMSERR_SA_FOR_ILLEGAL_BLOCK:			return _T("for ���̃u���b�N�̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_WHILE_NO_OPEN_PAREN:		return _T("while ���� '(' ��������܂���B");
		case TNMSERR_SA_WHILE_NO_CLOSE_PAREN:		return _T("while ���� '(' �������Ă��܂���B");
		case TNMSERR_SA_WHILE_ILLEGAL_COND:			return _T("while ���̏������̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_WHILE_NO_OPEN_BRACE:		return _T("while ���� '{' ��������܂���B");
		case TNMSERR_SA_WHILE_NO_CLOSE_BRACE:		return _T("while ���� '{' �������Ă��܂���B");
		case TNMSERR_SA_WHILE_ILLEGAL_BLOCK:		return _T("while ���̃u���b�N�̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_SWITCH_NO_OPEN_PAREN:		return _T("switch ���� '(' ��������܂���B");
		case TNMSERR_SA_SWITCH_NO_CLOSE_PAREN:		return _T("switch ���� '(' �������Ă��܂���B");
		case TNMSERR_SA_SWITCH_ILLEGAL_COND:		return _T("switch ���̏������̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_SWITCH_NO_OPEN_BRACE:		return _T("switch ���� '{' ��������܂���B");
		case TNMSERR_SA_SWITCH_NO_CLOSE_BRACE:		return _T("switch ���� '{' �������Ă��܂���B");
		case TNMSERR_SA_SWITCH_ILLEGAL_CASE:		return _T("switch ���� case ���̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_CASE_NO_OPEN_PAREN:			return _T("case ���� '(' ��������܂���B");
		case TNMSERR_SA_CASE_NO_CLOSE_PAREN:		return _T("case ���� '(' �������Ă��܂���B");
		case TNMSERR_SA_CASE_ILLEGAL_VALUE:			return _T("case ���̒l�̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_CASE_ILLEGAL_BLOCK:			return _T("case ���̃u���b�N�̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_DEFAULT_REDEFINE:			return _T("default �����Q�ȏ㌩����܂����B");
		case TNMSERR_SA_ASSIGN_NO_EQUAL:			return _T("������� '=' '+=' �Ȃǉ��Z�q��������܂���B");
		case TNMSERR_SA_ASSIGN_ILLEGAL_RIGHT:		return _T("������̉E�ӂ̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_ARG_LIST_NO_CLOSE_PAREN:	return _T("�R�}���h�� '(' �������Ă��܂���B");
		case TNMSERR_SA_ARG_LIST_NAMED_ARG_EXIST_BEFORE:	return _T("���O�������́A�ʏ�̈��������ɒu���ĉ������B");
		case TNMSERR_SA_ELEMENT_UNKNOWN:			return _T("����`�̃G�������g�ł��B");
		case TNMSERR_SA_ELEMENT_NO_OPEN:			return _T("�z��^�̃G�������g�� '[' ������܂���B");
		case TNMSERR_SA_ELEMENT_NO_CLOSE:			return _T("�G�������g�� '[' �����Ă��܂���B");
		case TNMSERR_SA_ELEMENT_NO_CHILD:			return _T("�G�������g�� '.' �ɑ����G�������g��������܂���B");
		case TNMSERR_SA_ELEMENT_ILLEGAL_EXP:		return _T("�G�������g�̓Y�����̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_EXP_ILLEGAL:				return _T("���̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_EXP_LIST_NO_CLOSE_BRACKET:	return _T("�����X�g�� '[' �������Ă��܂���B");
		case TNMSERR_SA_SMP_EXP_NO_CLOSE_PAREN:		return _T("���� '(' �������Ă��܂���B");
		case TNMSERR_SA_NAME_ILLEGAL_NAME:			return _T("'�y' �ɑ������O�̉�͂Ɏ��s���܂����B");
		case TNMSERR_SA_NAME_NO_CLOSE_SUMI:			return _T("���O�� '�y' �������Ă��܂���B");
	}

	return _T("�\����́F�s���ȃG���[�ł��B");
}

// ���F�擪�̃A�g�����擾
S_tnms_atom* TNMS_SA_NODE_EXP::get_first_atom()
{
	switch (node_type)	{
		case TNMS_SA_NODE_TYPE_EXP_SIMPLE:
			return smp_exp->get_first_atom();
		case TNMS_SA_NODE_TYPE_EXP_OPR_1:
			return exp_1->get_first_atom();
		case TNMS_SA_NODE_TYPE_EXP_OPR_2:
			return exp_1->get_first_atom();
	}

	return NULL;
}

// �P�����F�擪�̃A�g�����擾
S_tnms_atom* TNMS_SA_NODE_SMP_EXP::get_first_atom()
{
	switch (node_type)	{
		case TNMS_SA_NODE_TYPE_SMP_EXP_KAKKO:
			return &open->atom;
		case TNMS_SA_NODE_TYPE_SMP_EXP_GOTO:
			return &Goto->Goto->atom;
		case TNMS_SA_NODE_TYPE_SMP_EXP_ELM_EXP:
			return elm_exp->get_first_atom();
		case TNMS_SA_NODE_TYPE_SMP_EXP_EXP_LIST:
			return exp_list->get_first_atom();
		case TNMS_SA_NODE_TYPE_SMP_EXP_LITERAL:
			return &Literal->atom;
	}

	return NULL;
}

// �����X�g�F�擪�̃A�g�����擾
S_tnms_atom* TNMS_SA_NODE_EXP_LIST::get_first_atom()
{
	return exp[0]->get_first_atom();
}

// �G�������g���F�擪�̃A�g�����擾
S_tnms_atom* TNMS_SA_NODE_ELM_EXP::get_first_atom()
{
	return elm_list->get_first_atom();
}

// �G�������g���X�g�F�擪�̃A�g�����擾
S_tnms_atom* TNMS_SA_NODE_ELM_LIST::get_first_atom()
{
	if (!element.empty())	{
		if (element[0]->name)	{
			return &element[0]->name->atom;
		}
	}

	return NULL;
}

// �G�������g���X�g�F�����̃A�g�����擾
S_tnms_atom* TNMS_SA_NODE_ELM_LIST::get_last_atom()
{
	for (int i = (int)element.size() - 1; i >= 0; i--)	{
		if (element[i]->name)
			return &element[i]->name->atom;
	}

	return NULL;
}

