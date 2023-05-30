#include	"pch.h"
#include	"BS.h"
#include	"code.h"
#include	"element.h"
#include	"tnm_common_header.h"

#define		add_out_txt(s)	if (m_is_test)	add_out_txt(s)

std::map<TSTR, int> G_name_cntr;

// ****************************************************************
// �o�C�i���Z�[�o�[ - Bynary Saver -
// ================================================================

// �l����
inline bool is_value(int form)
{
	switch (form)	{
		case FM_VOID:
		case FM_INT:
		case FM_STR:
		case FM_INTLIST:
		case FM_STRLIST:
			return true;
		default:
			return false;
	};
}

// �Q�Ɣ���
inline bool is_reference(int form)
{
	return !is_value(form);
}

// �Q�ƊO��
int dereference(int form)
{
	if (form == FM_INTREF)		return FM_INT;
	if (form == FM_STRREF)		return FM_STR;
	if (form == FM_INTLISTREF)	return FM_INTLIST;
	if (form == FM_STRLISTREF)	return FM_STRLIST;
	return form;
}


// ****************************************************************
// �R���p�C�����C��
// ================================================================
bool C_tnms_binary_saver::compile(S_tnms_ia_data* piad, S_tnms_la_data* plad, S_tnms_sa_data* psad, S_tnms_bs_data* pbsd, bool is_test)
{
	int offset = 0;

	m_piad = piad;
	m_plad = plad;
	m_psad = psad;
	m_pbsd = pbsd;
	m_is_test = is_test;

	// �o�C�i���Z�[�o�[�f�[�^�̃N���A
	m_pbsd->out_scn.clear();
	m_pbsd->out_dbg.clear();
	m_pbsd->out_txt.clear();

	// �o�̓f�[�^�̃N���A
	out_scn.scn.clear();
	out_scn.str_list.clear();
	out_scn.label_list.clear();
	out_scn.z_label_list.clear();
	out_scn.cmd_label_list.clear();
	out_scn.scn_prop_list.clear();
	out_scn.scn_prop_name_list.clear();
	out_scn.call_prop_name_list.clear();
	out_scn.scn_cmd_list.clear();
	out_scn.scn_cmd_name_list.clear();
	out_scn.namae_list.clear();
	out_scn.read_flag_list.clear();
	out_txt.clear();
	loop_label.clear();
	cur_read_flag_no = 0;

	// �����񃊃X�g���쐬

	// �\�[�X����ǂ��ɂ������邽�߁A���������ёւ���
	int str_cnt = (int)m_plad->str_list.size();
	ARRAY<int> str_sort_index(str_cnt);
	for (int i = 0; i < str_cnt; i++)	{
		str_sort_index[i] = i;
	}
	std::random_shuffle(str_sort_index.begin(), str_sort_index.end());

	// ���������ёւ��l�ߍ���ł���
	offset = 0;
	out_scn.str_list.resize(str_cnt);
	out_scn.str_index_list.resize(str_cnt);
	for (int i = 0; i < str_cnt; i++)	{
		out_scn.str_list[i] = m_plad->str_list[str_sort_index[i]];
		out_scn.str_index_list[str_sort_index[i]].offset = offset;
		out_scn.str_index_list[str_sort_index[i]].size = (int)m_plad->str_list[str_sort_index[i]].size();
		offset += (int)m_plad->str_list[str_sort_index[i]].size();
	}

	//offset = 0;
	//out_scn.str_list.resize((int)m_plad->str_list.size());
	//out_scn.str_index_list.resize((int)m_plad->str_list.size());
	//for (int i = 0; i < (int)m_plad->str_list.size(); i++)	{
	//	out_scn.str_list[i] = m_plad->str_list[i];
	//	out_scn.str_index_list[i].offset = offset;
	//	out_scn.str_index_list[i].size = (int)m_plad->str_list[i].size();
	//	offset += (int)m_plad->str_list[i].size();
	//}

	// ���x�����X�g�A�y���x�����X�g���m�ہi�R���p�C�����ɍ\�z�����j
	out_scn.label_list.resize(m_plad->label_list.size());
	out_scn.z_label_list.resize(TNM_Z_LABEL_CNT, 0);

	// �V�[���v���p�e�B���X�g���쐬
	out_scn.scn_prop_list.resize((int)m_piad->property_list.size() - m_piad->inc_property_cnt);
	for (int i = 0; i < (int)m_piad->property_list.size() - m_piad->inc_property_cnt; i++)	{
		int property_id = i + m_piad->inc_property_cnt;
		out_scn.scn_prop_list[i].form = m_piad->property_list[property_id]->form;
		out_scn.scn_prop_list[i].size = m_piad->property_list[property_id]->size;
	}

	// �V�[���v���p�e�B�����X�g���쐬
	offset = 0;
	out_scn.scn_prop_name_index_list.resize((int)m_piad->property_list.size() - m_piad->inc_property_cnt);
	out_scn.scn_prop_name_list.resize((int)m_piad->property_list.size() - m_piad->inc_property_cnt);
	for (int i = 0; i < (int)m_piad->property_list.size() - m_piad->inc_property_cnt; i++)	{
		int property_id = i + m_piad->inc_property_cnt;
		out_scn.scn_prop_name_list[i] = m_piad->property_list[property_id]->name;
		out_scn.scn_prop_name_index_list[i].offset = offset;
		out_scn.scn_prop_name_index_list[i].size = (int)m_piad->property_list[property_id]->name.size();
		offset += (int)m_piad->property_list[property_id]->name.size();
	}

	// �V�[���R�}���h���X�g���m�ہi�R���p�C�����ɍ\�z�����j
	out_scn.scn_cmd_list.resize((int)m_piad->command_list.size() - m_piad->inc_command_cnt);

	// �V�[���R�}���h�����X�g���쐬
	offset = 0;
	out_scn.scn_cmd_name_index_list.resize((int)m_piad->command_list.size() - m_piad->inc_command_cnt);
	out_scn.scn_cmd_name_list.resize((int)m_piad->command_list.size() - m_piad->inc_command_cnt);
	for (int i = 0; i < (int)m_piad->command_list.size() - m_piad->inc_command_cnt; i++)	{
		int command_id = i + m_piad->inc_command_cnt;
		out_scn.scn_cmd_name_list[i] = m_piad->command_list[command_id]->name;
		out_scn.scn_cmd_name_index_list[i].offset = offset;
		out_scn.scn_cmd_name_index_list[i].size = (int)m_piad->command_list[command_id]->name.size();
		offset += (int)m_piad->command_list[command_id]->name.size();
	}

	// �R�[���v���p�e�B�����X�g���쐬
	offset = 0;
	out_scn.call_prop_name_index_list.resize((int)m_psad->call_prop_name_list.size());
	out_scn.call_prop_name_list.resize((int)m_psad->call_prop_name_list.size());
	for (int i = 0; i < (int)m_psad->call_prop_name_list.size(); i++)	{
		out_scn.call_prop_name_list[i] = m_psad->call_prop_name_list[i];
		out_scn.call_prop_name_index_list[i].offset = offset;
		out_scn.call_prop_name_index_list[i].size = (int)m_psad->call_prop_name_list[i].size();
		offset += (int)m_psad->call_prop_name_list[i].size();
	}

	// ss �R���p�C��
	if (!bs_ss(m_psad->root))
		return false;

	// ================================================================
	// �V�[���f�[�^������
	// ----------------------------------------------------------------

	// �w�b�_
	out_scn.header.header_size = sizeof(S_tnm_scn_header);
	m_pbsd->out_scn.push(out_scn.header);
	// ������C���f�b�N�X���X�g
	out_scn.header.str_index_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.str_index_cnt = (int)out_scn.str_index_list.size();
	m_pbsd->out_scn.push_array(out_scn.str_index_list.get(), (int)out_scn.str_index_list.size());
	// �����񃊃X�g
	out_scn.header.str_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.str_cnt = (int)out_scn.str_list.size();
	for (int i = 0; i < (int)out_scn.str_list.size(); i++)	{
		TSTR str = m_plad->str_list[str_sort_index[i]];
		int str_len = (int)str.size();
		for (int j = 0; j < str_len; j++)	{
			WORD moji = ((WORD)str[j]) ^ (28807 * str_sort_index[i]);
			m_pbsd->out_scn.push(moji);
		}
		//m_pbsd->out_scn.push_str(out_scn.str_list[i]);
	}
	// �V�[��
	out_scn.header.scn_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_size = out_scn.scn.size();
	m_pbsd->out_scn.push_array(out_scn.scn.get(), (int)out_scn.scn.size());
	// ���x�����X�g
	out_scn.header.label_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.label_cnt = (int)out_scn.label_list.size();
	m_pbsd->out_scn.push_array(out_scn.label_list.get(), (int)out_scn.label_list.size());
	// �y���x�����X�g
	out_scn.header.z_label_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.z_label_cnt = (int)out_scn.z_label_list.size();
	m_pbsd->out_scn.push_array(out_scn.z_label_list.get(), (int)out_scn.z_label_list.size());
	// �R�}���h���x�����X�g
	out_scn.header.cmd_label_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.cmd_label_cnt = (int)out_scn.cmd_label_list.size();
	m_pbsd->out_scn.push_array(out_scn.cmd_label_list.get(), (int)out_scn.cmd_label_list.size());
	// �V�[���v���p�e�B���X�g
	out_scn.header.scn_prop_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_prop_cnt = (int)out_scn.scn_prop_list.size();
	m_pbsd->out_scn.push_array(out_scn.scn_prop_list.get(), (int)out_scn.scn_prop_list.size());
	// �V�[���v���p�e�B���C���f�b�N�X���X�g
	out_scn.header.scn_prop_name_index_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_prop_name_index_cnt = (int)out_scn.scn_prop_name_index_list.size();
	m_pbsd->out_scn.push_array(out_scn.scn_prop_name_index_list.get(), (int)out_scn.scn_prop_name_index_list.size());
	// �V�[���v���p�e�B�����X�g
	out_scn.header.scn_prop_name_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_prop_name_cnt = (int)out_scn.scn_prop_name_list.size();
	for (int i = 0; i < (int)out_scn.scn_prop_name_list.size(); i++)
		m_pbsd->out_scn.push_str(out_scn.scn_prop_name_list[i]);
	// �V�[���R�}���h���X�g
	out_scn.header.scn_cmd_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_cmd_cnt = (int)out_scn.scn_cmd_list.size();
	m_pbsd->out_scn.push_array(out_scn.scn_cmd_list.get(), (int)out_scn.scn_cmd_list.size());
	// �V�[���R�}���h���C���f�b�N�X���X�g
	out_scn.header.scn_cmd_name_index_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_cmd_name_index_cnt = (int)out_scn.scn_cmd_name_index_list.size();
	m_pbsd->out_scn.push_array(out_scn.scn_cmd_name_index_list.get(), (int)out_scn.scn_cmd_name_index_list.size());
	// �V�[���R�}���h�����X�g
	out_scn.header.scn_cmd_name_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_cmd_name_cnt = (int)out_scn.scn_cmd_name_list.size();
	for (int i = 0; i < (int)out_scn.scn_cmd_name_list.size(); i++)
		m_pbsd->out_scn.push_str(out_scn.scn_cmd_name_list[i]);
	// �R�[���v���p�e�B���C���f�b�N�X���X�g
	out_scn.header.call_prop_name_index_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.call_prop_name_index_cnt = (int)out_scn.call_prop_name_index_list.size();
	m_pbsd->out_scn.push_array(out_scn.call_prop_name_index_list.get(), (int)out_scn.call_prop_name_index_list.size());
	// �R�[���v���p�e�B�����X�g
	out_scn.header.call_prop_name_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.call_prop_name_cnt = (int)out_scn.call_prop_name_list.size();
	for (int i = 0; i < (int)out_scn.call_prop_name_list.size(); i++)
		m_pbsd->out_scn.push_str(out_scn.call_prop_name_list[i]);

	// �y���O�z���X�g
	out_scn.header.namae_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.namae_cnt = (int)out_scn.namae_list.size();
	m_pbsd->out_scn.push_array(out_scn.namae_list.get(), (int)out_scn.namae_list.size());
	// ���ǃt���O���
	out_scn.header.read_flag_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.read_flag_cnt = (int)out_scn.read_flag_list.size();
	m_pbsd->out_scn.push_array(out_scn.read_flag_list.get(), (int)out_scn.read_flag_list.size());
	// �w�b�_���C��
	*(S_tnm_scn_header *)m_pbsd->out_scn.get() = out_scn.header;


	// ================================================================
	// �V�[���e�L�X�g�f�[�^������
	// ----------------------------------------------------------------
	m_pbsd->out_txt = out_txt;

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<ss>
// ================================================================
bool C_tnms_binary_saver::bs_ss(BSP<TNMS_SA_NODE_SS> ss)
{
	// <��*>
	for (int s = 0; s < (int)ss->sentense_list.size(); s++)	{
		if (!bs_sentence(ss->sentense_list[s]))
			return false;
	}

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�u���b�N>
// ================================================================
bool C_tnms_binary_saver::bs_block(BSP<TNMS_SA_NODE_BLOCK> block)
{
	// <��*>
	for (int s = 0; s < (int)block->sentense_list.size(); s++)	{
		if (!bs_sentence(block->sentense_list[s]))
			return false;
	}

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<��>
// ================================================================
bool C_tnms_binary_saver::bs_sentence(BSP<TNMS_SA_NODE_SENTENCE> sentence)
{
	// �s�ԍ�
	out_scn.scn.push(CD_NL);
	out_scn.scn.push(sentence->node_line);
	add_out_txt(_T("CD_NL: ") + tostr(sentence->node_line));

	// �I�������J�n
	if (sentence->is_include_sel)	{
		out_scn.scn.push(CD_SEL_BLOCK_START);
	}

	// �{��
	if (!bs_sentence_sub(sentence))	{
		return false;
	}

	// �I�������I��
	if (sentence->is_include_sel)	{
		out_scn.scn.push(CD_SEL_BLOCK_END);
	}

	return true;
}
bool C_tnms_binary_saver::bs_sentence_sub(BSP<TNMS_SA_NODE_SENTENCE> sentence)
{
	// ��
	switch (sentence->node_type)	{
		case TNMS_SA_NODE_TYPE_SENTENCE_LABEL:
			return bs_label(sentence->label);
		case TNMS_SA_NODE_TYPE_SENTENCE_Z_LABEL:
			return bs_z_label(sentence->z_label);
		case TNMS_SA_NODE_TYPE_SENTENCE_DEF_PROP:
			return bs_def_prop(sentence->def_prop);
		case TNMS_SA_NODE_TYPE_SENTENCE_DEF_CMD:
			return bs_def_cmd(sentence->def_cmd);
		case TNMS_SA_NODE_TYPE_SENTENCE_GOTO:
			return bs_goto(sentence->Goto);
		case TNMS_SA_NODE_TYPE_SENTENCE_RETURN:
			return bs_return(sentence->Return);
		case TNMS_SA_NODE_TYPE_SENTENCE_IF:
			return bs_if(sentence->If);
		case TNMS_SA_NODE_TYPE_SENTENCE_FOR:
			return bs_for(sentence->For);
		case TNMS_SA_NODE_TYPE_SENTENCE_WHILE:
			return bs_while(sentence->While);
		case TNMS_SA_NODE_TYPE_SENTENCE_CONTINUE:
			return bs_continue(sentence->Continue);
		case TNMS_SA_NODE_TYPE_SENTENCE_BREAK:
			return bs_break(sentence->Break);
		case TNMS_SA_NODE_TYPE_SENTENCE_SWITCH:
			return bs_switch(sentence->Switch);
		case TNMS_SA_NODE_TYPE_SENTENCE_ASSIGN:
			return bs_assign(sentence->assign);
		case TNMS_SA_NODE_TYPE_SENTENCE_COMMAND:
			return bs_command(sentence->command);
		case TNMS_SA_NODE_TYPE_SENTENCE_TEXT:
			return bs_text(sentence->text);
		case TNMS_SA_NODE_TYPE_SENTENCE_NAME:
			return bs_name(sentence->name);
		case TNMS_SA_NODE_TYPE_SENTENCE_EOF:
			return bs_eof(sentence->eof);
	}

	return false;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<���x��>
// ================================================================
bool C_tnms_binary_saver::bs_label(BSP<TNMS_SA_NODE_LABEL> label)
{
	out_scn.label_list[label->label->atom.opt] = (int)out_scn.scn.size();
	add_out_txt(_T("#label, ") + tostr(label->label->atom.opt));

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�y���x��>
// ================================================================
bool C_tnms_binary_saver::bs_z_label(BSP<TNMS_SA_NODE_Z_LABEL> z_label)
{
	// �y���x���̓��x���Ƃ��Ă��o�^����Ă���I
	out_scn.label_list[z_label->z_label->atom.subopt] = (int)out_scn.scn.size();	// ���x���ԍ��� subopt ���g��
	out_scn.z_label_list[z_label->z_label->atom.opt] = (int)out_scn.scn.size();		// �y���x���ԍ��� opt ���g��
	add_out_txt(_T("#z-label, ") + tostr(z_label->z_label->atom.opt));

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�v���p�e�B��`��>
// ================================================================
bool C_tnms_binary_saver::bs_def_prop(BSP<TNMS_SA_NODE_DEF_PROP> def_prop)
{
	// �Y����
	if (def_prop->form_code == FM_INTLIST || def_prop->form_code == FM_STRLIST)	{

		// �Y����������ΓY����
		if (def_prop->form->index)	{
			if (!bs_exp(def_prop->form->index, true))	// �l��v������
				return false;
		}
		// �Y�������Ȃ���� 0
		else	{
			out_scn.scn.push(CD_PUSH);
			out_scn.scn.push(FM_INT);
			out_scn.scn.push(0);
		}
	}

	out_scn.scn.push(CD_DEC_PROP);
	out_scn.scn.push(def_prop->form_code);	// �^
	out_scn.scn.push(def_prop->prop_id);	// �v���p�e�B�h�c

	// �R�[���v���p�e�B��
//	out_dbg.call_prop_name_list


	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�R�}���h��`��>
// ================================================================
bool C_tnms_binary_saver::bs_def_cmd(BSP<TNMS_SA_NODE_DEF_CMD> def_cmd)
{
	// ���x���̉��쐬
	int label_no_end = (int)out_scn.label_list.size();
	out_scn.label_list.push_back(0);

	// �R�}���h�O���炱���֓˂����񂾏ꍇ�̓R�}���h��փW�����v����
	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no_end);

	// �R�}���h���x���̐ݒ�i�����J�� inc �R�}���h��g�ݗ��Ă�Ƃ��ɕK�v�ɂȂ�܂��j
	S_tnm_scn_cmd_label cmd_label;
	cmd_label.cmd_id = def_cmd->cmd_id;
	cmd_label.offset = (int)out_scn.scn.size();
	out_scn.cmd_label_list.push_back(cmd_label);

	// <�v���p�e�B�錾>
	for (int p = 0; p < (int)def_cmd->prop_list.size(); p++)	{
		if (!bs_def_prop(def_cmd->prop_list[p]))
			return false;
	}

	// <�����̓W�J>
	out_scn.scn.push(CD_ARG);

	// <�u���b�N>
	if (!bs_block(def_cmd->block))
		return false;

	// �_�~�[�� return ����u���Ă���
	// return �����Ȃ������ꍇ�͂��� return ���ɂ���ăR�}���h���甲����
	out_scn.scn.push(CD_RETURN);
	out_scn.scn.push(0);	// �����̌� = 0
	add_out_txt(_T("CD_RETURN"));

	// ���x���̐ݒ�
	out_scn.label_list[label_no_end] = (int)out_scn.scn.size();

	// �V�[���R�}���h���X�g
	if (def_cmd->cmd_id >= m_piad->inc_command_cnt)
		out_scn.scn_cmd_list[def_cmd->cmd_id - m_piad->inc_command_cnt].offset = cmd_label.offset;

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<goto ���^gosub ��>
// ================================================================
bool C_tnms_binary_saver::bs_goto(BSP<TNMS_SA_NODE_GOTO> Goto)
{
	// goto ��
	if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOTO)	{

		// goto ���x��
		if (Goto->node_sub_type == TNMS_SA_NODE_TYPE_GOTO_LABEL)	{
			out_scn.scn.push(CD_GOTO);
			out_scn.scn.push(Goto->label->atom.opt);		// opt ���g��
			add_out_txt(_T("CD_GOTO: ") + tostr(Goto->label->atom.opt));
		}
		// goto �y���x��
		else	{
			out_scn.scn.push(CD_GOTO);
			out_scn.scn.push(Goto->z_label->atom.subopt);	// subopt ���g��
			add_out_txt(_T("CD_GOTO: ") + tostr(Goto->z_label->atom.subopt));
		}
	}
	// gosub ��
	else {
		// gosub ��
		if (!bs_goto_exp(Goto))
			return false;

		// �߂�l���̂Ă�
		if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOSUB)	{
			out_scn.scn.push(CD_POP);
			out_scn.scn.push(FM_INT);
			add_out_txt(_T("CD_POP, ") + tostr_form(FM_INT));
		}
		else	{
			out_scn.scn.push(CD_POP);
			out_scn.scn.push(FM_STR);
			add_out_txt(_T("CD_POP, ") + tostr_form(FM_STR));
		}
	}

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<goto�^gosub ��>
// ================================================================
bool C_tnms_binary_saver::bs_goto_exp(BSP<TNMS_SA_NODE_GOTO> Goto)
{
	// ����
	if (!bs_arg_list(Goto->arg_list, true))		// true: �l��v��
		return false;

	// gosub / gosubstr
	if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOSUB)	{

		// ���x���ԍ����擾�i���x���̏ꍇ�� opt�A�y���x���̏ꍇ�� subopt �ɓ����Ă���j
		int label_no = Goto->node_sub_type == TNMS_SA_NODE_TYPE_GOTO_LABEL ? Goto->label->atom.opt : Goto->z_label->atom.subopt;

		out_scn.scn.push(CD_GOSUB);
		out_scn.scn.push(label_no);		// ���x���ԍ�

		// �����̐��ƌ^����ׂĂ���
		out_scn.scn.push((int)Goto->arg_list->arg.size());
		for (int i = 0; i < (int)Goto->arg_list->arg.size(); i++)	{
			int form = dereference(Goto->arg_list->arg[i]->exp->tmp_form);
			out_scn.scn.push(form);
		}

		add_out_txt(_T("CD_GOSUB, ") + tostr(label_no) + _T(", ") + tostr((int)Goto->arg_list->arg.size()));
	}
	else	{

		// ���x���ԍ����擾�i���x���̏ꍇ�� opt�A�y���x���̏ꍇ�� subopt �ɓ����Ă���j
		int label_no = Goto->node_sub_type == TNMS_SA_NODE_TYPE_GOTO_LABEL ? Goto->label->atom.opt : Goto->z_label->atom.subopt;

		out_scn.scn.push(CD_GOSUBSTR);
		out_scn.scn.push(label_no);		// ���x���ԍ�

		// �����̐��ƌ^����ׂĂ���
		out_scn.scn.push((int)Goto->arg_list->arg.size());
		for (int i = 0; i < (int)Goto->arg_list->arg.size(); i++)	{
			int form = dereference(Goto->arg_list->arg[i]->exp->tmp_form);
			out_scn.scn.push(form);
		}

		add_out_txt(_T("CD_GOSUBSTR, ") + tostr(label_no) + _T(", ") + tostr((int)Goto->arg_list->arg.size()));
	}


	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<return ��>
// ================================================================
bool C_tnms_binary_saver::bs_return(BSP<TNMS_SA_NODE_RETURN> Return)
{
	// ��������̏ꍇ
	if (Return->node_type == TNMS_SA_NODE_TYPE_RETURN_WITH_ARG)	{
		if (!bs_exp(Return->exp, true))		// �l��v������
			return false;
	}

	// return
	out_scn.scn.push(CD_RETURN);
	add_out_txt(_T("CD_RETURN"));

	// �����̐��ƌ^����ׂĂ����i���x���ԍ��̕���Y�ꂸ�Ɂj
	if (Return->node_type == TNMS_SA_NODE_TYPE_RETURN_WITH_ARG)	{
		int form = dereference(Return->exp->node_form);
		out_scn.scn.push(1);	// �����P��
		out_scn.scn.push(form);
	}
	else	{
		out_scn.scn.push(0);	// �����O��
	}

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<if ��>
// ================================================================
bool C_tnms_binary_saver::bs_if(BSP<TNMS_SA_NODE_IF> If)
{
	// �G���h���x���̉��쐬
	int label_no_end = (int)out_scn.label_list.size();
	out_scn.label_list.push_back(0);

	int if_cnt = (int)If->sub.size();
	for (int i = 0; i < if_cnt; i++)	{

		// "if" | "elseif"
		if (If->sub[i].If->atom.type == TNMS_LA_ATOM_IF || If->sub[i].If->atom.type == TNMS_LA_ATOM_ELSEIF)	{
			// �������x���̉��쐬
			int label_no_if = (int)out_scn.label_list.size();
			out_scn.label_list.push_back(0);
			// <������>
			if (!bs_exp(If->sub[i].cond, true))		// �l��v������
				return false;
			// ���������U�Ȃ�������x���ɃW�����v
			out_scn.scn.push(CD_GOTO_FALSE);
			out_scn.scn.push(label_no_if);
			// <�u���b�N>
			for (int s = 0; s < (int)If->sub[i].block.size(); s++)	{
				if (!bs_sentence(If->sub[i].block[s]))
					return false;
			}
			// �G���h���x���ɃW�����v
			out_scn.scn.push(CD_GOTO);
			out_scn.scn.push(label_no_end);
			// �������x���̐ݒ�
			out_scn.label_list[label_no_if] = (int)out_scn.scn.size();
		}
		// "else"
		else	{
			// <�u���b�N>
			for (int s = 0; s < (int)If->sub[i].block.size(); s++)	{
				if (!bs_sentence(If->sub[i].block[s]))
					return false;
			}
		}
	}

	// �G���h���x���̐ݒ�
	out_scn.label_list[label_no_end] = (int)out_scn.scn.size();

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<for ��>
// ================================================================
bool C_tnms_binary_saver::bs_for(BSP<TNMS_SA_NODE_FOR> For)
{
	// �R�̃��x�������쐬
	int label_size = (int)out_scn.label_list.size();
	int label_no_init = label_size;			// [���������x��]
	int label_no_loop = label_size + 1;		// [���[�v���x��]
	int label_no_out = label_size + 2;		// [�E�o���x��]
	out_scn.label_list.push_back(0);
	out_scn.label_list.push_back(0);
	out_scn.label_list.push_back(0);

	// ���[�v���x���̒ǉ�
	LOOP loop = {label_no_loop, label_no_out};
	loop_label.push_back(loop);

	// <��������>
	for (int s = 0; s < (int)For->init.size(); s++)	{
		if (!bs_sentence(For->init[s]))
			return false;
	}
	// ���������x���ɃW�����v
	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no_init);
	// ���x���̐ݒ� [���[�v���x��]
	out_scn.label_list[label_no_loop] = (int)out_scn.scn.size();
	// <���[�v��>
	for (int s = 0; s < (int)For->loop.size(); s++)	{
		if (!bs_sentence(For->loop[s]))
			return false;
	}
	// ���x���̐ݒ� [���������x��]
	out_scn.label_list[label_no_init] = (int)out_scn.scn.size();
	// <������>
	if (!bs_exp(For->cond, true))		// �l��v������
		return false;
	// ���������U�Ȃ�E�o���x���ɃW�����v
	out_scn.scn.push(CD_GOTO_FALSE);
	out_scn.scn.push(label_no_out);
	// <�u���b�N>
	for (int s = 0; s < (int)For->block.size(); s++)	{
		if (!bs_sentence(For->block[s]))
			return false;
	}
	// ���[�v���x���ɃW�����v
	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no_loop);
	// ���x���̐ݒ� [�E�o���x��]
	out_scn.label_list[label_no_out] = (int)out_scn.scn.size();

	// ���[�v���x���̍폜
	loop_label.pop_back();

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<while ��>
// ================================================================
bool C_tnms_binary_saver::bs_while(BSP<TNMS_SA_NODE_WHILE> While)
{
	// �Q�̃��x�������쐬
	int label_size = (int)out_scn.label_list.size();
	int label_no_loop = label_size;		// [���[�v���x��]
	int label_no_out = label_size + 1;	// [�E�o���x��]
	out_scn.label_list.push_back(0);
	out_scn.label_list.push_back(0);

	// ���[�v���x���̒ǉ�
	LOOP loop = {label_no_loop, label_no_out};
	loop_label.push_back(loop);

	// ���x���̐ݒ� [���[�v���x��]
	out_scn.label_list[label_no_loop] = (int)out_scn.scn.size();
	// <������>
	if (!bs_exp(While->cond, true))		// �l��v������
		return false;
	// ���������U�Ȃ�E�o���x���ɃW�����v
	out_scn.scn.push(CD_GOTO_FALSE);
	out_scn.scn.push(label_no_out);
	// <�u���b�N>
	for (int s = 0; s < (int)While->block.size(); s++)	{
		if (!bs_sentence(While->block[s]))
			return false;
	}
	// ���[�v���x���ɃW�����v
	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no_loop);
	// ���x���̐ݒ� [�E�o���x��]
	out_scn.label_list[label_no_out] = (int)out_scn.scn.size();

	// ���[�v���x���̍폜
	loop_label.pop_back();

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<continue ��>
// ================================================================
bool C_tnms_binary_saver::bs_continue(BSP<TNMS_SA_NODE_CONTINUE> Continue)
{
	if (loop_label.empty())
		return error(TNMSERR_BS_CONTINUE_NO_LOOP, Continue->Continue->atom);

	// ���x�������o��
	int label_no = loop_label.back().Continue;

	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no);

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<break ��>
// ================================================================
bool C_tnms_binary_saver::bs_break(BSP<TNMS_SA_NODE_BREAK> Break)
{
	if (loop_label.empty())
		return error(TNMSERR_BS_BREAK_NO_LOOP, Break->Break->atom);

	// ���x�������o��
	int label_no = loop_label.back().Break;

	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no);

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<switch ��>
// ================================================================
bool C_tnms_binary_saver::bs_switch(BSP<TNMS_SA_NODE_SWITCH> Switch)
{
	// �Q�ƊO��
	int form_l = dereference(Switch->cond->node_form);

	// ���x�������쐬
	int label_size = (int)out_scn.label_list.size();
	int label_no_out = label_size;										// [�E�o���x��]
	int label_no_case = label_size + 1;									// [case���x��]
	int label_no_default = label_size + 1 + (int)Switch->Case.size();	// [default���x��]
	for (int i = 0; i < (int)Switch->Case.size() + 1; i++)	{
		out_scn.label_list.push_back(0);
	}
	if (Switch->Default)	{
		out_scn.label_list.push_back(0);
	}

	// <������>
	if (!bs_exp(Switch->cond, true))		// �l��v������
		return false;
	// <case ��*>
	for (int c = 0; c < (int)Switch->Case.size(); c++)	{

		// �Q�ƊO��
		int form_r = dereference(Switch->Case[c]->value->node_form);

		// ���������R�s�[
		out_scn.scn.push(CD_COPY);
		out_scn.scn.push(form_l);
		// <��r�l>
		if (!bs_exp(Switch->Case[c]->value, true))		// �l��v������
			return false;

		// ��r
		out_scn.scn.push(CD_OPERATE_2);			// �񍀉��Z
		out_scn.scn.push(form_l);				// �����̌^
		out_scn.scn.push(form_r);				// �E���̌^
		out_scn.scn.push(OP_EQUAL);				// ����
		// ���������^�Ȃ�case���x���ɃW�����v
		out_scn.scn.push(CD_GOTO_TRUE);
		out_scn.scn.push(label_no_case + c);
	}
	// ��������j��
	out_scn.scn.push(CD_POP);
	out_scn.scn.push(form_l);
	// �E�o���x���܂��̓f�t�H���g���x���ɃW�����v
	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(Switch->Default ? label_no_default : label_no_out);

	// <case ��*>
	for (int c = 0; c < (int)Switch->Case.size(); c++)	{
		// ���x����ݒu [case���x��]
		out_scn.label_list[label_no_case + c] = (int)out_scn.scn.size();
		// ��������j��
		out_scn.scn.push(CD_POP);
		out_scn.scn.push(form_l);
		// <�u���b�N>
		for (int s = 0; s < (int)Switch->Case[c]->block.size(); s++)	{
			if (!bs_sentence(Switch->Case[c]->block[s]))
				return false;
		}
		// �E�o���x���ɃW�����v
		out_scn.scn.push(CD_GOTO);
		out_scn.scn.push(label_no_out);
	}
	// <default ��>
	if (Switch->Default)	{
		// ���x����ݒu [default���x��]
		out_scn.label_list[label_no_default] = (int)out_scn.scn.size();
		// <�u���b�N>
		for (int s = 0; s < (int)Switch->Default->block.size(); s++)	{
			if (!bs_sentence(Switch->Default->block[s]))
				return false;
		}
		// �E�o���x���ɃW�����v
		out_scn.scn.push(CD_GOTO);
		out_scn.scn.push(label_no_out);
	}
	// ���x����ݒu [�E�o���x��]
	out_scn.label_list[label_no_out] = (int)out_scn.scn.size();

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�����>
// ================================================================
// ���^�ɂ��Ă̒���
//		a = b	��	assign->left->form = (a), assign->right->form = (b)
//		a += b	��	assign->left->form = (a), assign->right->form = (b), assign->equal_form = (a + b)
// ================================================================
bool C_tnms_binary_saver::bs_assign(BSP<TNMS_SA_NODE_ASSIGN> assign)
{
	// ����
	if (!bs_left(assign->left))
		return false;

#if 0
	// __set �R�}���h���g���ꍇ�̓G�������g�Ƃ��Đς�ł���
	if (assign->set_flag)	{
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(FM_INT);
		out_scn.scn.push(ELM___SET);
	}
#endif

	// += �ȂǁA���Z���K�v�ȏꍇ�A
	// ���ӂ̃G�������g���X�g���R�s�[���ăv���p�e�B�W�J�����Ă���
	if (assign->equal->atom.opt != OP_NONE)	{
		out_scn.scn.push(CD_COPY_ELM);
		out_scn.scn.push(CD_PROPERTY);
		add_out_txt(_T("CD_COPY_EL"));
		add_out_txt(_T("CD_PROPERTY"));
	}

	// �E��
	if (!bs_exp(assign->right, assign->set_flag ? false : true))		// �l��v������
		return false;

	// += �Ȃǂ̉��Z���s��
	int form_l = dereference(assign->left->node_form);
	int form_r = dereference(assign->right->node_form);
	if (assign->equal->atom.opt != OP_NONE)	{
		out_scn.scn.push(CD_OPERATE_2);			// �񍀉��Z
		out_scn.scn.push(form_l);				// �����̌^
		out_scn.scn.push(form_r);				// �E���̌^
		add_out_txt(_T("CD_OPERATE_2, ") + tostr_form(form_l) + _T(", ") + tostr_form(form_r));
		bs_assign_operator(assign->equal);		// ���Z
	}

	// ���
	int form_r_2 = dereference(assign->equal_form);
	out_scn.scn.push(CD_ASSIGN);
	out_scn.scn.push(assign->left->node_form);	// �����̌^�͎Q�ƌ^�̂܂�
	out_scn.scn.push(form_r_2);					// �E���̌^�͌v�Z���ʂ̌^���g��
	out_scn.scn.push(assign->al_id);			// �E���̌^�͌v�Z���ʂ̌^���g��
	add_out_txt(_T("CD_ASSIGN, ") + tostr_form(assign->left->node_form) + _T(", ") + tostr_form(assign->node_form));

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�R�}���h��>
// ================================================================
bool C_tnms_binary_saver::bs_command(BSP<TNMS_SA_NODE_COMMAND> command)
{
	// �R�}���h
	if (!bs_elm_exp(command->command, true))	// �l��v������
		return false;

	// �߂�l���̂Ă�
	out_scn.scn.push(CD_POP);
	out_scn.scn.push(command->command->node_form);
	add_out_txt(_T("CD_POP, ") + tostr_form(command->command->node_form));

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�e�L�X�g��>
// ================================================================
bool C_tnms_binary_saver::bs_text(BSP<TNMS_SA_NODE_ATOM> text)
{
	// �����b�Z�[�W�u���b�N�����蓖�Ă����ȃR�}���h
	bs_push_msg_block();

	// �����i������h�c�j
	out_scn.scn.push(CD_PUSH);
	out_scn.scn.push(FM_STR);
	out_scn.scn.push(text->atom.opt);

	// �e�L�X�g�R�}���h
	out_scn.scn.push(CD_TEXT);

	// �e�L�X�g���Ɋ��ǃt���O�����蓖�Ă�
	out_scn.scn.push(cur_read_flag_no ++);

	// ���ǃt���O���
	S_tnm_scn_read_flag read_flag;
	read_flag.line_no = text->atom.line;
	out_scn.read_flag_list.push_back(read_flag);

	// �e�L�X�g
	add_out_txt(_T("CD_TEXT"));

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<��>
// ================================================================
bool C_tnms_binary_saver::bs_exp(BSP<TNMS_SA_NODE_EXP> exp, bool need_value)
{
	if (false);
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_SIMPLE)	{	// �P����
		return bs_smp_exp(exp->smp_exp, need_value);
	}
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_OPR_1)	{		// �P�����Z

		// �Q�Ƃ͉��Z�ł��Ȃ�
		if (!need_value)
			return error(TNMSERR_BS_NEED_REFERENCE, *exp->get_first_atom());

		if (!bs_exp(exp->exp_1, true))			// �����A�l��v������
			return false;

		int form = dereference(exp->exp_1->node_form);		// �Q�ƊO��
		out_scn.scn.push(CD_OPERATE_1);						// �񍀉��Z
		out_scn.scn.push(form);								// �����̌^
		add_out_txt(_T("CD_OPERATE_1"));
		if (!bs_operator_1(exp->opr))						// ���Z�q�^�C�v
			return false;
	}
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_OPR_2)	{		// �񍀉��Z

		// �Q�Ƃ͉��Z�ł��Ȃ�
		if (!need_value)
			return error(TNMSERR_BS_NEED_REFERENCE, *exp->get_first_atom());

		if (!bs_exp(exp->exp_1, true))				// �����A�l��v������
			return false;
		if (!bs_exp(exp->exp_2, true))				// �E���A�l��v������
			return false;

		int form_l = dereference(exp->exp_1->node_form);	// �Q�ƊO��
		int form_r = dereference(exp->exp_1->node_form);	// �Q�ƊO��
		out_scn.scn.push(CD_OPERATE_2);						// �񍀉��Z
		out_scn.scn.push(form_l);							// �����̌^
		out_scn.scn.push(form_r);							// �E���̌^
		add_out_txt(_T("CD_OPERATE_2"));
		if (!bs_operator_2(exp->opr))						// ���Z�q�^�C�v
			return false;
	}

	// ���ۂ̌^�Ɨv������Ă���^���Ⴄ�ꍇ�͕ϊ��R�}���h�����s
	// �������AFM_INTREF �� FM_INT �� FM_STRREF �� FM_STR �͎Q�ƊO���̎d���B�����I����Ă�̂ł����ł͍s��Ȃ�
	// �P�ӏ��ɂ܂Ƃ߂�ׂ��I�I�I
#if 0
	if (exp->node_form != exp->tmp_form)	{
		if (!((exp->node_form == FM_INTREF && exp->tmp_form == FM_INT) || (exp->node_form == FM_STRREF && exp->tmp_form == FM_STR)))	{

			out_scn.scn.push(CD_PUSH);
			out_scn.scn.push(FM_INT);
			out_scn.scn.push(ELM___TRANS);	// �ϊ�
			out_scn.scn.push(CD_COMMAND);	// �ϊ��R�}���h�����s
			out_scn.scn.push(0);			// �������X�g�ԍ�
			out_scn.scn.push(0);			// �������̌�
			out_scn.scn.push(0);			// ���O�������̌�
			out_scn.scn.push(FM_VOID);		// �߂�l�̌^
		}
	}
#endif

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�����X�g>
// ================================================================
//		[���̌�]	: int
//		[��]		: ���ׂĂ̎�
// ================================================================
bool C_tnms_binary_saver::bs_exp_list(BSP<TNMS_SA_NODE_EXP_LIST> exp_list)
{
	int exp_cnt = (int)exp_list->exp.size();
	for (int e = 0; e < exp_cnt; e ++)	{
		if (!bs_exp(exp_list->exp[e], true))	// �l��v������
			return false;
	}

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�P����>
// ================================================================
bool C_tnms_binary_saver::bs_smp_exp(BSP<TNMS_SA_NODE_SMP_EXP> smp_exp, bool need_value)
{
	if (false);
	// '(' <��> ')'
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_KAKKO)	{
		return bs_exp(smp_exp->exp, need_value);
	}
	// <�����X�g>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_EXP_LIST)	{

		// �����X�g�͎Q�ƂɂȂ�Ȃ�
		if (!need_value)
			return error(TNMSERR_BS_NEED_REFERENCE, *smp_exp->get_first_atom());

		return bs_exp_list(smp_exp->exp_list);
	}
	// <goto ��>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_GOTO)	{

		// goto �͎Q�ƂɂȂ�Ȃ�
		if (!need_value)
			return error(TNMSERR_BS_NEED_REFERENCE, *smp_exp->get_first_atom());

		return bs_goto_exp(smp_exp->Goto);
	}
	// <�G�������g��>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_ELM_EXP)	{
		return bs_elm_exp(smp_exp->elm_exp, need_value);
	}
	// <���e����>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_LITERAL)	{

		// ���e�����͎Q�ƂɂȂ�Ȃ�
		if (!need_value)
			return error(TNMSERR_BS_NEED_REFERENCE, *smp_exp->get_first_atom());

		return bs_literal(smp_exp->Literal);
	}

	return false;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<����>
// ================================================================
bool C_tnms_binary_saver::bs_left(BSP<TNMS_SA_NODE_ELM_EXP> left)
{
	// �G�������g�������
	if (!bs_elm_list(left->elm_list))	// ���ӂ͒l��v�����Ȃ�
		return false;

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�G�������g��>
// ================================================================
//	�R�}���h�̏ꍇ
//		<�G�������g���X�g>
//	�v���p�e�B�̏ꍇ
//		<�G�������g���X�g>
//		CD_PROPERTY
// ================================================================
bool C_tnms_binary_saver::bs_elm_exp(BSP<TNMS_SA_NODE_ELM_EXP> elm_exp, bool need_value)
{
	// �G�������g���X�g�́A
	// �E�R�}���h�̏ꍇ�F�����̃R�}���h�G�������g�ɂ��A���l�ɓW�J����Ă��܂��B
	// �E�v���p�e�B�̏ꍇ�F���l�����߂��Ă���ꍇ�́A�����Œ��l�ɓW�J���܂��B

	// �{���́A�R�}���h���G�������g�ŕԂ����Ƃ��ł���悤�ɂ��������]�܂����ł��B
	// ���̏ꍇ�̓��t�@�����X�̊T�O����������g�܂Ȃ��Ƃ����܂���B

	if (false);

	// �R�}���h�̏ꍇ
	else if (elm_exp->element_type == TNMS_ELEMENT_TYPE_COMMAND)	{

		// �R�}���h�͎Q�Ƃɂ͂Ȃ�Ȃ�
//		if (!need_value)
//			return error(TNMSERR_BS_NEED_REFERENCE, *elm_exp->elm_list->get_last_atom());

		// �R�}���h�̃R�[�h���擾���Ă���
		int parent_form_code = elm_exp->elm_list->element.back()->element_parent_form;
		int element_code = elm_exp->elm_list->element.back()->element_code;

		// �����b�Z�[�W�u���b�N�����蓖�Ă����ȃR�}���h
		if (
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_KOE) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SET_FACE) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SET_NAMAE) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_PRINT) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_RUBY) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_NL) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_NLI) ||
			//(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SEL) || 
			//(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SEL_CANCEL) || 
			//(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELBTN) || 
			//(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELBTN_CANCEL) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_KOE) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SET_FACE) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SET_NAMAE) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_PRINT) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_RUBY) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_NL) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_NLI) || 
			//(parent_form_code == FM_MWND && element_code == ELM_MWND_SEL) ||
			//(parent_form_code == FM_MWND && element_code == ELM_MWND_SEL_CANCEL) ||
			false
			)
		{
			bs_push_msg_block();
		}

		// �G�������g���X�g�����
		if (!bs_elm_list(elm_exp->elm_list))
			return false;

		// �����ǃt���O�����蓖�Ă����ȃR�}���h
		if (
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_PRINT) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_KOE) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_KOE_PLAY_WAIT) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_KOE_PLAY_WAIT_KEY) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SEL) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SEL_CANCEL) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELMSG) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELMSG_CANCEL) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELBTN) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELBTN_CANCEL) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELBTN_START) ||	// ready �ł͂Ȃ� start �Ɋ��蓖�Ă�
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SEL_IMAGE) ||		// kinetic ��p
			(parent_form_code == FM_MWND && element_code == ELM_MWND_PRINT) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_KOE) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_KOE_PLAY_WAIT) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_KOE_PLAY_WAIT_KEY) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SEL) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SEL_CANCEL) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SELMSG) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SELMSG_CANCEL) ||
			false
			)
		{
			out_scn.scn.push(cur_read_flag_no);
			add_out_txt(_T("\tread_flag_no = ") + tostr(cur_read_flag_no));
			cur_read_flag_no ++;

			// ���ǃt���O���
			S_tnm_scn_read_flag read_flag;
			read_flag.line_no = elm_exp->elm_list->element.back()->node_line;
			out_scn.read_flag_list.push_back(read_flag);
		}

		// �l��v������ꍇ�͎Q�ƊO��
		if (need_value)	{

			// �l�Ȃ牽�����Ȃ�
			if (is_value(elm_exp->node_form))	{}
			// �Q�ƊO���ł���Ȃ�Q�ƊO��
			else if (elm_exp->node_form == FM_INTREF || elm_exp->node_form == FM_STRREF || elm_exp->node_form == FM_INTLISTREF || elm_exp->node_form == FM_STRLISTREF)	{
				out_scn.scn.push(CD_PROPERTY);
				add_out_txt(_T("CD_PROPERTY"));
			}
			// �Q�ƊO���ł��Ȃ��Ȃ�G���[
			else
				return error(TNMSERR_BS_NEED_VALUE, *elm_exp->elm_list->get_last_atom());
		}
	}

	// �v���p�e�B�̏ꍇ
	else if (elm_exp->element_type == TNMS_ELEMENT_TYPE_PROPERTY)	{

		// �G�������g���X�g�����
		if (!bs_elm_list(elm_exp->elm_list))
			return false;

		// �l���K�v�ȏꍇ
		if (need_value)	{
			
			// �l�Ȃ牽�����Ȃ�
			if (is_value(elm_exp->node_form))	{}
			// �Q�ƊO���ł���Ȃ�Q�ƊO��
			else if (elm_exp->node_form == FM_INTREF || elm_exp->node_form == FM_STRREF || elm_exp->node_form == FM_INTLISTREF || elm_exp->node_form == FM_STRLISTREF)	{
				out_scn.scn.push(CD_PROPERTY);
				add_out_txt(_T("CD_PROPERTY"));
			}
			// �Q�ƊO���ł��Ȃ��Ȃ�G���[
			else
				return error(TNMSERR_BS_NEED_VALUE, *elm_exp->elm_list->get_last_atom());
		}
	}

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�G�������g���X�g>
// ================================================================
bool C_tnms_binary_saver::bs_elm_list(BSP<TNMS_SA_NODE_ELM_LIST> elm_list)
{
	// �G�������g�J�n�|�C���g��ݒ�
	out_scn.scn.push(CD_ELM_POINT);
	add_out_txt(_T("CD_ELM_POINT"));

	// �X�R�[�v��ς�
	if (elm_list->parent_form_code == FM_CALL)	{
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(FM_INT);
		out_scn.scn.push(ELM_GLOBAL_CUR_CALL);
		add_out_txt(_T("CD_PUSH, ") + tostr_form(FM_INT) + _T(", ") + tostr(ELM_GLOBAL_CUR_CALL));
	}

	// �G�������g�����ɐς�
	int element_cnt = (int)elm_list->element.size();
	for (int i = 0; i < element_cnt; i++)	{
		if (!bs_element(elm_list->element[i]))
			return false;

		// �Q�ƂȂ�Q�ƊO��
		if (get_elm_owner(elm_list->element[i]->element_code) == ELM_OWNER_CALL_PROP)	{
			if (is_reference(elm_list->element[i]->node_form))	{
				out_scn.scn.push(CD_PROPERTY);
				add_out_txt(_T("CD_PROPERTY"));
			}
		}
	}

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�G�������g>
// ================================================================
//	ex.	load("abc")
//		CD_PUSH		10			// load �̃G�������g�R�[�h
//		<�������X�g>			// �������X�g
//		CD_COMMAND				// �R�}���h�����s
//		CD_PUSH		0			// �������X�g�ԍ�
//		CD_PUSH		1			// �����̌�
// ================================================================
bool C_tnms_binary_saver::bs_element(BSP<TNMS_SA_NODE_ELEMENT> element)
{
	if (false);

	// �ʏ�̃G�������g�̏ꍇ
	else if (element->node_type == TNMS_SA_NODE_TYPE_ELEMENT_ELEMENT)	{

		// �G�������g�R�[�h��ς�
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(FM_INT);
		out_scn.scn.push(element->element_code);
		add_out_txt(_T("CD_PUSH, ") + tostr_form(FM_INT) + _T(", ") + tostr(element->element_code));

		// �R�}���h�̏ꍇ
		if (element->element_type == TNMS_ELEMENT_TYPE_COMMAND)	{

			int arg_cnt = (int)element->arg_list->arg.size();		// �������̌�

			// �������X�g
			if (!bs_arg_list(element->arg_list, false))		// false: �l��v�����Ȃ�
				return false;

			// �ȗ����ꂽ�����������œW�J���Ă���

			// �R�}���h���擾
			S_tnms_form* form_info = m_piad->form_table.get_form_by_code(element->element_parent_form);
			S_tnms_element* element_info = form_info->get_element_by_code(element->element_code);
			ARRAY<S_tnms_arg>* temp_arg_list = &element_info->arg_map[element->arg_list_id].arg_list;
			if (element->arg_list->arg.size() < temp_arg_list->size())	{	// ����������������菭�Ȃ��ꍇ
				for (ARRAY<S_tnms_arg>::iterator temp_arg = temp_arg_list->begin() + element->arg_list->arg.size(); temp_arg != temp_arg_list->end(); ++temp_arg)	{

					// "*" �Ȃ�I��
					if (temp_arg->form == FM___ARGS || temp_arg->form == FM___ARGSREF)
						break;

					// �f�t�H���g����������
					out_scn.scn.push(CD_PUSH);
					out_scn.scn.push(temp_arg->form);
					switch (temp_arg->form)	{		// �f�t�H���g�l������
						case FM_INT:	out_scn.scn.push(temp_arg->def_int);	break;
						//case FM_STR:	out_scn.scn.push(temp_arg->def_str);	break;
						default:		
							return error(TNMSERR_BS_ILLEGAL_DEFAULT_ARG, element->name->atom);
					}
					arg_cnt ++;		// �f�t�H���g�������������Ƃ��Đς񂾂̂ŃJ�E���g�𑝂₷
				}
			}

			// �R�}���h
			out_scn.scn.push(CD_COMMAND);
			out_scn.scn.push(element->arg_list_id);		// �������X�g�ԍ�
			out_scn.scn.push(arg_cnt);					// �������̌�

			// �����̌^����ׂĂ����i�ϒ������Ȃǂ̑΍�j
			// �t���ɕ��ׂ�I�i�����̓X�^�b�N�ɋt���ɐς܂�邽�߁j
			TSTR arg_form_text;

			// �܂��͏ȗ����ꂽ����
			if (element->arg_list->arg.size() < temp_arg_list->size())	{	// ����������������菭�Ȃ��ꍇ
				for (S_tnms_arg* temp_arg = temp_arg_list->get_end() - 1; temp_arg >= temp_arg_list->get() + element->arg_list->arg.size(); --temp_arg)	{

					// "*" �Ȃ�I��
					if (temp_arg->form == FM___ARGS || temp_arg->form == FM___ARGSREF)
						break;

					out_scn.scn.push(temp_arg->form);
					arg_form_text += tostr_form(temp_arg->form) + _T(", ");
				}
			}

			// ���Ɏ��ۂɗ^����ꂽ�������̕ϊ���̌^
			for (int i = (int)element->arg_list->arg.size() - 1; i >= 0; i--)	{
				out_scn.scn.push(element->arg_list->arg[i]->exp->tmp_form);
				arg_form_text += tostr_form(element->arg_list->arg[i]->exp->tmp_form) + _T(", ");
				// �^�� FM_LIST �̏ꍇ�́A�^���X�g��ǉ�
				if (element->arg_list->arg[i]->exp->tmp_form == FM_LIST)	{
					out_scn.scn.push((int)element->arg_list->arg[i]->exp->smp_exp->exp_list->form_list.size());	// ���X�g�̃T�C�Y
					// �������t���ɕ��ׂ�
					for (int j = (int)element->arg_list->arg[i]->exp->smp_exp->exp_list->form_list.size() - 1; j >= 0; j--)	{
						int form = dereference(element->arg_list->arg[i]->exp->smp_exp->exp_list->form_list[j]);
						out_scn.scn.push(form);
					}
				}
			}

			// ���O�������̌�
			out_scn.scn.push(element->arg_list->named_arg_cnt);

			// ���O�������̈���ID
			for (int i = (int)element->arg_list->arg.size() - 1; i >= 0; i--)	{
				if (element->arg_list->arg[i]->node_type == TNMS_SA_NODE_TYPE_ARG_WITH_NAME)	{
					out_scn.scn.push(element->arg_list->arg[i]->name_id);
				}
			}

			// �߂�l�̌^
			out_scn.scn.push(element->node_form);

			// �f�o�b�O�e�L�X�g
			add_out_txt(_T("CD_COMMAND, al_id = ") + tostr(element->arg_list_id) + _T(", al_size = ") + tostr((int)element->arg_list->arg.size()));
			add_out_txt(_T("\targ_form_list = ") + arg_form_text + _T("ret_form = ") + tostr_form(element->node_form));
		}
	}

	// �Y�����̏ꍇ
	else if (element->node_type == TNMS_SA_NODE_TYPE_ELEMENT_ARRAY)	{

		// �G�������g�R�[�h��ς�
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(FM_INT);
		out_scn.scn.push(ELM_ARRAY);
		add_out_txt(_T("CD_PUSH, ") + tostr_form(FM_INT) + _T(", ELM_ARRAY"));

		// �Y���������
		bs_exp(element->exp, true);		// �l��v������
	}

	return true;
}


// ****************************************************************
// �o�C�i���Z�[�o�[�F<�������X�g>
// ================================================================
//	ex.	(1, "abc")
//		<��>		// 1
//		<��>		// "abc"
// ================================================================
//		need_value: �l��v������i�Q�ƊO����v�������j
// ================================================================
bool C_tnms_binary_saver::bs_arg_list(BSP<TNMS_SA_NODE_ARG_LIST> arg_list, bool need_value)
{
	int arg_cnt = (int)arg_list->arg.size();
	for (int a = 0; a < arg_cnt; a ++)	{
		bool need_value_arg = need_value;

		if (arg_list->arg[a]->exp->tmp_form == FM_INT || arg_list->arg[a]->exp->tmp_form == FM_STR || arg_list->arg[a]->exp->tmp_form == FM_LIST)
			need_value_arg = true;		// int �� str �� list �^�͒l���K�v�i�Q�ƊO����v������j

		if (!bs_arg(arg_list->arg[a], need_value_arg))
			return false;
	}

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<����>
// ================================================================
bool C_tnms_binary_saver::bs_arg(BSP<TNMS_SA_NODE_ARG> arg, bool need_value)
{
	// <��>
	if (!bs_exp(arg->exp, need_value))
		return false;

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<���O>
// ================================================================
bool C_tnms_binary_saver::bs_name(BSP<TNMS_SA_NODE_NAME> name)
{
	// �����b�Z�[�W�u���b�N�����蓖�Ă����ȃR�}���h
	bs_push_msg_block();

	// ���O
	if (!bs_literal(name->name))
		return false;

	out_scn.scn.push(CD_NAME);
	add_out_txt(_T("CD_NAME, ") + tostr(name->name->atom.opt));

	// �y���O�z����o�^
	bool new_name = true;
	if (!out_scn.namae_list.empty())	{
		ARRAY<int>::iterator end = out_scn.namae_list.end();
		for (ARRAY<int>::iterator itr = out_scn.namae_list.begin(); itr != end; ++itr)	{
			if (out_scn.str_list[*itr] == out_scn.str_list[name->name->atom.opt])	{
				new_name = false;
				break;
			}
		}
	}
	if (new_name)
		out_scn.namae_list.push_back(name->name->atom.opt);

#if 0
	G_name_cntr[out_scn.str_list[name->name->atom.opt]] = G_name_cntr[out_scn.str_list[name->name->atom.opt]] + 1;
#endif

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<���e����>
// ================================================================
bool C_tnms_binary_saver::bs_literal(BSP<TNMS_SA_NODE_ATOM> Literal)
{
	// ���x���� FM_INT �ɕϊ����đ���
	if (Literal->node_form == FM_LABEL)	{
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(FM_INT);
		out_scn.scn.push(Literal->atom.opt);
		add_out_txt(_T("CD_PUSH, ") + tostr_form(FM_INT) + _T(", ") + tostr(Literal->atom.opt));
	}
	else	{
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(Literal->node_form);
		out_scn.scn.push(Literal->atom.opt);
		add_out_txt(_T("CD_PUSH, ") + tostr_form(Literal->node_form) + _T(", ") + tostr(Literal->atom.opt));
	}

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<������Z�q>
// ================================================================
bool C_tnms_binary_saver::bs_assign_operator(BSP<TNMS_SA_NODE_ATOM> opr)
{
	out_scn.scn.push((BYTE)opr->atom.opt);

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�P�����Z�q>
// ================================================================
bool C_tnms_binary_saver::bs_operator_1(BSP<TNMS_SA_NODE_ATOM> opr)
{
	out_scn.scn.push((BYTE)opr->atom.opt);	// ���Z�q�^�C�v

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�񍀉��Z�q>
// ================================================================
bool C_tnms_binary_saver::bs_operator_2(BSP<TNMS_SA_NODE_ATOM> opr)
{
	out_scn.scn.push((BYTE)opr->atom.opt);	// ���Z�q�^�C�v

	return true;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F<�I�[>
// ================================================================
bool C_tnms_binary_saver::bs_eof(BSP<TNMS_SA_NODE_ATOM> eof)
{
	out_scn.scn.push(CD_EOF);

	return true;
}

// ****************************************************************
// �^�R�[�h �� �^�̖��O
// ================================================================
TSTR C_tnms_binary_saver::tostr_form(int form_code)
{
	return m_piad->form_table.get_form_by_code(form_code)->name;
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F���b�Z�[�W�u���b�N
// ================================================================
void C_tnms_binary_saver::bs_push_msg_block()
{
	// ���b�Z�[�W�u���b�N
	out_scn.scn.push(CD_ELM_POINT);
	out_scn.scn.push(CD_PUSH);
	out_scn.scn.push(FM_INT);
	out_scn.scn.push(ELM_GLOBAL_MSG_BLOCK);		// ���b�Z�[�W�u���b�N
	out_scn.scn.push(CD_COMMAND);				// �R�}���h
	out_scn.scn.push(0);						// �������X�g�ԍ�
	out_scn.scn.push(0);						// �����̌�
	out_scn.scn.push(0);						// ���O�������̌�
	out_scn.scn.push(FM_VOID);					// �߂�l�̌^
}

// ****************************************************************
// �o�C�i���Z�[�o�[�F�G���[
// ================================================================
bool C_tnms_binary_saver::error(TNMSERR_BS type, S_tnms_atom atom)
{
	last_error.type = type;
	last_error.atom = atom;

	return false;
}

void C_tnms_binary_saver::clear_error()
{
	last_error.type = TNMSERR_BS_NONE;
}

S_tnms_atom C_tnms_binary_saver::get_error_atom()
{
	return last_error.atom;
}

int C_tnms_binary_saver::get_error_line()
{
	return last_error.atom.line;
}

TSTR C_tnms_binary_saver::get_error_str()
{
	// �G���[�̐ݒ�
	switch (last_error.type)	{
		case TNMSERR_BS_ILLEGAL_DEFAULT_ARG:		return _T("�Ή����Ă��Ȃ��^�̃f�t�H���g�����ł��B");
		case TNMSERR_BS_CONTINUE_NO_LOOP:			return _T("continue �̓��[�v�̒��ł����g���܂���B");
		case TNMSERR_BS_BREAK_NO_LOOP:				return _T("break �̓��[�v�̒��ł����g���܂���B");
		case TNMSERR_BS_NEED_REFERENCE:				return _T("���Ӓl���K�v�ł��B");
		case TNMSERR_BS_NEED_VALUE:					return _T("�E�Ӓl���K�v�ł��B");
	}

	return _T("�o�C�i���Z�[�u�F�s���ȃG���[�ł��B");
}

