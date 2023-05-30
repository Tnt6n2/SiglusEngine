#pragma		once

#include	"tnm_common_header.h"
#include	"SA.h"

extern	std::map<TSTR, int> G_name_cntr;

// ****************************************************************
// �o�C�i���Z�[�u�G���[
// ================================================================
enum TNMSERR_BS
{
	TNMSERR_BS_NONE,
	TNMSERR_BS_ILLEGAL_DEFAULT_ARG,
	TNMSERR_BS_CONTINUE_NO_LOOP,
	TNMSERR_BS_BREAK_NO_LOOP,
	TNMSERR_BS_NEED_REFERENCE,
	TNMSERR_BS_NEED_VALUE,
};

// ****************************************************************
// �o�C�i���Z�[�u�\����
// ================================================================
struct S_tnms_bs_data
{
	C_stream		out_scn;			// ���ۂɎg�p����p�b�N�V�[��
	C_stream		out_dbg;			// �f�o�b�O�p�̃f�[�^
	ARRAY<TSTR>		out_txt;			// �ꎞ�e�L�X�g
};

// ****************************************************************
// �o�C�i���Z�[�o�[
// ================================================================
class C_tnms_binary_saver
{
public:
	bool		compile(S_tnms_ia_data* piad, S_tnms_la_data* plad, S_tnms_sa_data* psad, S_tnms_bs_data* pbsd, bool is_test);

	S_tnms_atom	get_error_atom();
	int			get_error_line();
	TSTR		get_error_str();
	
private:
	bool	bs_ss(BSP<TNMS_SA_NODE_SS> ss);
	bool	bs_block(BSP<TNMS_SA_NODE_BLOCK> block);
	bool	bs_sentence(BSP<TNMS_SA_NODE_SENTENCE> sentence);
	bool	bs_sentence_sub(BSP<TNMS_SA_NODE_SENTENCE> sentence);
	bool	bs_label(BSP<TNMS_SA_NODE_LABEL> label);
	bool	bs_z_label(BSP<TNMS_SA_NODE_Z_LABEL> z_label);
	bool	bs_def_prop(BSP<TNMS_SA_NODE_DEF_PROP> def_prop);
	bool	bs_def_cmd(BSP<TNMS_SA_NODE_DEF_CMD> def_cmd);
	bool	bs_goto(BSP<TNMS_SA_NODE_GOTO> Goto);
	bool	bs_goto_exp(BSP<TNMS_SA_NODE_GOTO> Goto);
	bool	bs_return(BSP<TNMS_SA_NODE_RETURN> Return);
	bool	bs_if(BSP<TNMS_SA_NODE_IF> If);
	bool	bs_for(BSP<TNMS_SA_NODE_FOR> For);
	bool	bs_while(BSP<TNMS_SA_NODE_WHILE> While);
	bool	bs_continue(BSP<TNMS_SA_NODE_CONTINUE> Continue);
	bool	bs_break(BSP<TNMS_SA_NODE_BREAK> Break);
	bool	bs_switch(BSP<TNMS_SA_NODE_SWITCH> Switch);
	bool	bs_assign(BSP<TNMS_SA_NODE_ASSIGN> assign);
	bool	bs_command(BSP<TNMS_SA_NODE_COMMAND> command);
	bool	bs_text(BSP<TNMS_SA_NODE_ATOM> text);
	bool	bs_exp(BSP<TNMS_SA_NODE_EXP> exp, bool need_value);
	bool	bs_exp_list(BSP<TNMS_SA_NODE_EXP_LIST> exp_list);
	bool	bs_smp_exp(BSP<TNMS_SA_NODE_SMP_EXP> smp_exp, bool need_value);
	bool	bs_left(BSP<TNMS_SA_NODE_ELM_EXP> Property);
	bool	bs_elm_exp(BSP<TNMS_SA_NODE_ELM_EXP> elm_exp, bool need_value);
	bool	bs_elm_list(BSP<TNMS_SA_NODE_ELM_LIST> elm_list);
	bool	bs_element(BSP<TNMS_SA_NODE_ELEMENT> element);
	bool	bs_arg_list(BSP<TNMS_SA_NODE_ARG_LIST> arg_list, bool need_value);
	bool	bs_arg(BSP<TNMS_SA_NODE_ARG> arg, bool need_value);
	bool	bs_name(BSP<TNMS_SA_NODE_NAME> name);
	bool	bs_literal(BSP<TNMS_SA_NODE_ATOM> Literal);
	bool	bs_assign_operator(BSP<TNMS_SA_NODE_ATOM> opr);
	bool	bs_operator_1(BSP<TNMS_SA_NODE_ATOM> opr);
	bool	bs_operator_2(BSP<TNMS_SA_NODE_ATOM> opr);
	bool	bs_eof(BSP<TNMS_SA_NODE_ATOM> eof);

	void	bs_push_msg_block();

	TSTR	tostr_form(int form_code);

	// �f�o�b�O�p�e�L�X�g�o��
	void	add_out_txt(CTSTR& str);

	// ���[�v�p�̃��x���ʒu
	struct LOOP
	{
		int		Continue;	// continue ���̃W�����v��
		int		Break;		// break ���̃W�����v��
	};

	S_tnms_ia_data*	m_piad;
	S_tnms_la_data*	m_plad;
	S_tnms_sa_data*	m_psad;
	S_tnms_bs_data*	m_pbsd;

	S_tnm_scn_data		out_scn;			// �o�̓V�[���f�[�^
	ARRAY<TSTR>			out_txt;			// �o�̓e�L�X�g�f�[�^
	ARRAY<LOOP>			loop_label;			// ���[�v�p�̃V�X�e�����x��
	int					cur_read_flag_no;	// ���蓖�Ē��̊��ǃt���O�ԍ�
	bool				m_is_test;			// �e�X�g�t���O

	// �G���[�\����
	struct BS_ERROR
	{
		TNMSERR_BS		type;
		S_tnms_atom		atom;
	};

	// �G���[�֘A
	void		clear_error();
	bool		error(TNMSERR_BS type, S_tnms_atom atom);
	BS_ERROR	last_error;
};


inline void C_tnms_binary_saver::add_out_txt(CTSTR& str)
{
	if (m_is_test)
		out_txt.push_back(str);
}
