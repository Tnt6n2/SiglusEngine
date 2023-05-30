#include	"pch.h"

#include	"tnm_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"

#include	"element/elm_prop.h"
#include	"element/elm_flag.h"
#include	"element/elm_call.h"

#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_flag.h"
#include	"engine/eng_message.h"

#include	"engine/eng_scene.h"
#include	"engine/flow_command.h"
#include	"engine/flow_script.h"

#include	<list>


// ****************************************************************
// �v���g�^�C�v�錾
// ================================================================
void	tnm_calculate_1(int form, BYTE opr);							// �P�����Z
void	tnm_calculate_2(int form_l, int form_r, BYTE opr);				// �񍀉��Z
void	tnm_calculate_2_str_int(BYTE opr);								// �񍀉��Z�F������ op ����
void	tnm_calculate_2_str_str(BYTE opr);								// �񍀉��Z�F������ op ������
bool	tnm_declare_call_prop(int form_code, int prop_id, int size);	// �R�[���v���p�e�B��錾
bool	tnm_expand_arg_into_call_flag();								// �������R�[���v���p�e�B�ɓW�J

// ****************************************************************
// �t���[�F�X�N���v�g����
// ================================================================
//		�X�N���v�g����͂��Ă����܂��B
// ================================================================

// ================================================================
// ���d�v�I�I�I�I�I
// �X�N���v�g���s�p�̃f�[�^���m�ۂ���̂ɂ��Ȃ�̎��Ԃ�������܂��B
// �g���܂킷���Ƃō�������}��܂��B
// �������A�G���h�A�N�V�����ɂ��ċA�I�ɌĂ΂��\��������܂��B
// �ċA�̐[�����L�����Ă����A�K�؂Ɋm�ۂ��g���܂��B
// ----------------------------------------------------------------

bool tnm_proc_script_sub();

// �X�N���v�g�v���Z�X���i�ċA�I�Ɂj�Ăяo������
static int _ex_call_cnt = 0;

// �X�N���v�g�v���Z�X�����s���邽�߂̃f�[�^�\����
struct S_script_proc_data
{
	// �G�������g
	S_element		element_set;

	// �����̌^�R�[�h���X�g
	ARRAY<int>		arg_form_code_set;

	// �������X�g
	C_tnm_prop_list	arg_set;
};

// �X�N���v�g�v���Z�X�����s���邽�߂̃f�[�^
// �m�ۂɎ��Ԃ�������̂Ŏg���܂킷
// �|�C���^�ŕێ������̂� vector �ł͓����Ȃ��Blist ���g�����ƁB
static std::list<S_script_proc_data>	_ex_script_proc_data;

// ****************************************************************
// �t���[�F�X�N���v�g�����{��
// ================================================================
bool tnm_proc_script()
{
	// �Q�[���I�����͉������Ȃ��i�t���[���A�N�V�����������ݒ肳�ꂽ�Ƃ��͂����Ŕ����邱�Ƃ������I�j
	if (Gp_global->game_end_flag)
		return true;

	// �ċA�I�ɌĂ΂��̂ŌĂ΂ꂽ�񐔂��J�E���g
	_ex_call_cnt ++;

	// �Ă΂ꂽ�񐔕��������o���Ă��Ȃ���Ώ���
	if ((int)_ex_script_proc_data.size() < _ex_call_cnt)	{
	
		const int ARG_CNT_MAX = 1024;

		// �X�N���v�g���s�p�̃f�[�^���m��
		_ex_script_proc_data.resize(_ex_call_cnt);

		// �e�����o����������
		_ex_script_proc_data.back().arg_form_code_set.resize(ARG_CNT_MAX);
		_ex_script_proc_data.back().arg_set.resize(ARG_CNT_MAX);
		for (int i = 0; i < ARG_CNT_MAX; i++)	{
			_ex_script_proc_data.back().arg_set[i].exp_list.resize(8);			// �����C�v�I�v�V�����̍ő吔���W�Ȃ̂� 4 ���� 8 �ɕύX���܂����B
		}
	}

	// �{�̂��Ă�
	bool ret = tnm_proc_script_sub();

	// �Ă΂ꂽ�񐔂����炷
	_ex_call_cnt --;

	return ret;
}

bool tnm_proc_script_sub()
{
	// ����g���X�N���v�g���s�p�̃f�[�^�����o��
	std::list<S_script_proc_data>::iterator itr = _ex_script_proc_data.begin();
	for (int i = 0; i < _ex_call_cnt - 1; i++)
		++ itr;
	S_element& _ex_element = itr->element_set;
	ARRAY<int>& _ex_arg_form_code_list = itr->arg_form_code_set;
	C_tnm_prop_list& _ex_arg_list = itr->arg_set;

	// ���ԈႢ
	//S_element& _ex_element = _ex_script_proc_data.back().element_set;
	//ARRAY<int>& _ex_arg_form_code_list = _ex_script_proc_data.back().arg_form_code_set;
	//C_tnm_prop_list& _ex_arg_list = _ex_script_proc_data.back().arg_set;

	// ----------------------------------------------------------------

	// �X�N���v�g�������s��
	// ��������}�邽�߁A�悭�ʂ���̂��珇�ɕ��ׂ܂��B

	while (Gp_local->cur_proc.type == TNM_PROC_TYPE_SCRIPT)	{
		T_tnm_code code = Gp_lexer->pop_ret<BYTE>();		// �X�N���v�g�R�[�h

		if (code == CD_PUSH)	{			// stack push

			// �X�^�b�N�ɒl�� push ���܂��B

			int form_code = Gp_lexer->pop_ret<int>();
			if (form_code == FM_INT)		tnm_stack_push_int(Gp_lexer->pop_ret<int>());
			else if (form_code == FM_STR)	tnm_stack_push_str(Gp_lexer->pop_str_ret());
		}
		else if (code == CD_PROPERTY)	{		// �v���p�e�B�l���擾

			// �X�^�b�N�̖����̃G�������g��l�ɕϊ����܂��B
			// ��Ffront.object[0].x �� 100

			// �G�������g���X�^�b�N����擾
			int elm_cnt = tnm_stack_pop_element(_ex_element);
			// �R�}���h�����s
			if (!tnm_command_proc(_ex_element.begin(), _ex_element.end()))
				return true;
		}
		else if (code == CD_OPERATE_2)	{		// �񍀉��Z

			// �X�^�b�N���œ񍀉��Z���s���܂��B

			int form_l = Gp_lexer->pop_ret<int>();
			int form_r = Gp_lexer->pop_ret<int>();
			BYTE opr = Gp_lexer->pop_ret<BYTE>();
			tnm_calculate_2(form_l, form_r, opr);
		}
		else if (code == CD_ELM_POINT)	{		// �G�������g�|�C���g�𗧂Ă�
			Gp_stack->stack_point_list.push_back(Gp_stack->int_now_cnt);
		}
		else if (code == CD_ASSIGN)	{			// �v���p�e�B�l��ݒ�

			// ����������s���܂��B

			// �^���擾
			int left_form = Gp_lexer->pop_ret<int>();
			int right_form = Gp_lexer->pop_ret<int>();
			int al_id = Gp_lexer->pop_ret<int>();
			// �X�^�b�N����������擾
			tnm_stack_pop_arg_list(_ex_arg_list, right_form);
			// �G�������g���X�^�b�N����擾
			int elm_cnt = tnm_stack_pop_element(_ex_element);
			// �R�}���h�����s�i�����͂P�ŌŒ�Ȃ̂ŁA_ex_arg_list.begin() �` _ex_arg_list.begin() + 1 �܂ł�n���j
			tnm_command_proc(_ex_element.begin(), _ex_element.end(), al_id, _ex_arg_list.get_sub().get(), _ex_arg_list.get_sub().get() + 1, NULL, FM_VOID);
		}
		else if (code == CD_NL)	{				// ���s

			// ���s�R�[�h��������܂����B
			// �X�e�b�v���s���̏ꍇ�́A�������񏈗����~�߂܂��B

			// �X�e�b�v���s���̏ꍇ
			int old_line_no = Gp_lexer->get_cur_line_no();
			int line_no = Gp_lexer->pop_ret<int>();
			Gp_lexer->set_cur_line_no(line_no);
			if (Gp_global->is_breaking && Gp_global->break_step_flag)	{
				// �X�V���ꂽ�ꍇ�A�����𔲂���
				if (Gp_lexer->get_cur_line_no() != old_line_no)	{
					Gp_global->break_step_flag = false;		// �X�e�b�v���s�I��
					Gp_global->disp_flag = false;			// �X�e�b�v���s�ɂ�鏈�������͕`�悵�Ȃ�
					return true;
				}
			}
		}
		else if (code == CD_COMMAND)	{		// �R�}���h�����s

			// �������X�g�h�c���擾
			int arg_list_id = Gp_lexer->pop_ret<int>();
			// �X�^�b�N����������擾
			int arg_cnt = tnm_stack_pop_arg_list(_ex_arg_list, _ex_arg_form_code_list);
			// �G�������g���X�^�b�N����擾
			int elm_cnt = tnm_stack_pop_element(_ex_element);
			// ���O�������̌�
			int named_arg_cnt = Gp_lexer->pop_ret<int>();
			// ���O���������X�g�̈����h�c���擾
			for (int a = 0; a < named_arg_cnt; a++)	{
				_ex_arg_list[arg_cnt - a - 1].id = Gp_lexer->pop_ret<int>();
			}
			// �߂�l�̌^���擾
			int ret_form_code = Gp_lexer->pop_ret<int>();
			// �R�}���h�����s
			tnm_command_proc(_ex_element.begin(), _ex_element.end(), arg_list_id, 
				_ex_arg_list.get_sub().get(), 
				_ex_arg_list.get_sub().get() + arg_cnt - named_arg_cnt, 
				_ex_arg_list.get_sub().get() + arg_cnt,
				ret_form_code);
		}
		else	{

			switch (code)	{

				case CD_GOTO:			// goto
				{
					// �������Ń��x���ɃW�����v���܂��B

					int label_no = Gp_lexer->pop_ret<int>();
					Gp_lexer->jump_to_label(label_no);
				}
				break;

				case CD_GOTO_FALSE:		// goto false
				{
					// ���ʂ��U�̏ꍇ�̂݃��x���ɃW�����v���܂��B

					int cond = tnm_stack_pop_int();
					int label_no = Gp_lexer->pop_ret<int>();
					if (cond == 0)	// �U�̏ꍇ
						Gp_lexer->jump_to_label(label_no);
				}
				break;

				case CD_GOTO_TRUE:		// goto true
				{
					// ���ʂ��^�̏ꍇ�̂݃��x���ɃW�����v���܂��B

					int cond = tnm_stack_pop_int();
					int label_no = Gp_lexer->pop_ret<int>();
					if (cond != 0)	// �^�̏ꍇ
						Gp_lexer->jump_to_label(label_no);
				}
				break;

				case CD_GOSUB:			// gosub
				{
					// gosub �̏������s���܂��B�߂�l�� int �^�ł��B

					tnm_command_proc_gosub(_ex_arg_list, _ex_arg_form_code_list, FM_INT);
				}
				break;

				case CD_GOSUBSTR:		// gosub str
				{
					// gosub �̏������s���܂��B�߂�l�� str �^�ł��B

					tnm_command_proc_gosub(_ex_arg_list, _ex_arg_form_code_list, FM_STR);
				}
				break;

				case CD_RETURN:			// return
				{
					// return �̏������s���܂��B

					if (!tnm_command_proc_return(_ex_arg_list, _ex_arg_form_code_list))
						return true;
				}
				break;

				case CD_POP:			// stack pop
				{
					// �X�^�b�N����l�� pop ���܂��B

					int form_code = Gp_lexer->pop_ret<int>();
					switch (form_code)	{
						case FM_INT:	tnm_stack_pop_int();	break;
						case FM_STR:	tnm_stack_pop_str();	break;
					}
				}
				break;

				case CD_COPY:			// stack copy
				{
					// �X�^�b�N�̖����̒l�𕡐����܂��B+= �Ȃǂ̏����Ɏg���܂��B

					int form_code = Gp_lexer->pop_ret<int>();
					switch (form_code)	{
						case FM_INT:	tnm_stack_push_int(tnm_stack_back_int());		break;
						case FM_STR:	tnm_stack_push_str(tnm_stack_back_str());		break;
					}
				}
				break;

				case CD_COPY_ELM:		// stack copy element
				{
					// �X�^�b�N�̖����̃G�������g�𕡐����܂��B

					tnm_stack_copy_element(_ex_element);
				}
				break;

				case CD_DEC_PROP:		// �v���p�e�B���`
				{
					// �ϐ���`���s���܂��B

					int form_code = Gp_lexer->pop_ret<int>();
					int prop_id = Gp_lexer->pop_ret<int>();
					int size = 0;
					if (form_code == FM_INTLIST || form_code == FM_STRLIST)	{
						size = tnm_stack_pop_int();
					}

					tnm_declare_call_prop(form_code, prop_id, size);
				}
				break;

				case CD_ARG:			// ������W�J����
				{
					// �������R�[���t���O�ɓW�J���܂��B

					tnm_expand_arg_into_call_flag();
				}
				break;

				case CD_OPERATE_1:		// �P�����Z
				{
					// �X�^�b�N���ŒP�����Z���s���܂��B

					int form = Gp_lexer->pop_ret<int>();
					BYTE opr = Gp_lexer->pop_ret<BYTE>();
					tnm_calculate_1(form, opr);
				}
				break;

				case CD_NAME:			// ���O
				{
					tnm_msg_proc_name(tnm_get_cur_mwnd(), tnm_stack_pop_str());
				}
				break;

				case CD_TEXT:			// �e�L�X�g
				{
					int rf_flag_no = Gp_lexer->pop_ret<int>();	// ���ǃt���O�̃t���O�ԍ����擾
					int rf_scn_no = Gp_lexer->get_cur_scn_no();	// ���ǃt���O�̃V�[���ԍ��͌��݂̃V�[���ԍ�
					TSTR msg = tnm_stack_pop_str();				// ������

					tnm_msg_proc_print(tnm_get_cur_mwnd(), msg, rf_scn_no, rf_flag_no, true);
				}
				break;

				case CD_NONE:			// �G���[
				{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�X�N���v�g�̉�͂Ɏ��s���܂����B"));
					tnm_push_proc(TNM_PROC_TYPE_NONE);
				}
				break;

				case CD_EOF:			// �t�@�C���I�[
				{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�t�@�C���̏I�[�ɓ��B���܂����B"));
					tnm_push_proc(TNM_PROC_TYPE_NONE);
				}
			}
		}
	}

	return true;
}


// ****************************************************************
// �P�����Z
// ================================================================
//		�P�����Z���s���A���ʂ��X�^�b�N�ɐς݂܂��B
//			[+, -]
// ================================================================
void tnm_calculate_1(int form, BYTE opr)
{
	if (false);
	else if (form == FM_INT)	{
		int rhs = tnm_stack_pop_int();			// �E�l

		switch (opr)	{
			case OP_PLUS:		tnm_stack_push_int(+ rhs);		break;
			case OP_MINUS:		tnm_stack_push_int(- rhs);		break;
			case OP_TILDE:		tnm_stack_push_int(~ rhs);		break;
		}
	}
}

// ****************************************************************
// �񍀉��Z
// ================================================================
//		�񍀉��Z���s���A���ʂ��X�^�b�N�ɐς݂܂��B
//			[+, -, *, /, %, ==, !=, >, >=, <, <=, &&, ||]
// ================================================================
void tnm_calculate_2(int form_l, int form_r, BYTE opr)
{
	if (false);

	// ���� op ����
	else if (form_l == FM_INT && form_r == FM_INT)	{
		int rhs = tnm_stack_pop_int();			// �E�l�����o��
		int lhs = tnm_stack_pop_int();			// ���l�����o��
		switch (opr)	{
			case OP_PLUS:			tnm_stack_push_int(lhs + rhs);				break;
			case OP_MINUS:			tnm_stack_push_int(lhs - rhs);				break;
			case OP_MULTIPLE:		tnm_stack_push_int(lhs * rhs);				break;
			case OP_EQUAL:			tnm_stack_push_int(lhs == rhs ? 1 : 0);		break;
			case OP_NOT_EQUAL:		tnm_stack_push_int(lhs != rhs ? 1 : 0);		break;
			case OP_GREATER:		tnm_stack_push_int(lhs >  rhs ? 1 : 0);		break;
			case OP_GREATER_EQUAL:	tnm_stack_push_int(lhs >= rhs ? 1 : 0);		break;
			case OP_LESS:			tnm_stack_push_int(lhs <  rhs ? 1 : 0);		break;
			case OP_LESS_EQUAL:		tnm_stack_push_int(lhs <= rhs ? 1 : 0);		break;
			case OP_LOGICAL_AND:	tnm_stack_push_int((lhs && rhs) ? 1 : 0);	break;
			case OP_LOGICAL_OR:		tnm_stack_push_int((lhs || rhs) ? 1 : 0);	break;

			case OP_AND:			tnm_stack_push_int(lhs & rhs);				break;
			case OP_OR:				tnm_stack_push_int(lhs | rhs);				break;
			case OP_HAT:			tnm_stack_push_int(lhs ^ rhs);				break;
			case OP_SL:				tnm_stack_push_int(lhs << rhs);				break;
			case OP_SR:				tnm_stack_push_int(lhs >> rhs);				break;

			// �E�_���V�t�g
			// int �̉E�V�t�g�͎Z�p�V�t�g�����AUINT �ɃL���X�g���邱�ƂŁA�_���V�t�g�ɂȂ�B
			// ���ʂ� int �ɖ߂��B
			case OP_SR3:			tnm_stack_push_int((int)((UINT)lhs >> (UINT)rhs));		break;

			// ����Z�͂O���Z�����邽�ߓ��ꏈ��
			case OP_DIVIDE:
				if (rhs == 0)	{
					tnm_set_error(TNM_ERROR_TYPE_DIVIDED_BY_ZERO, _T("0 ���Z���s���܂����I'/'"));
					tnm_stack_push_int(0);	// �����I�ɉ��Z���ʂ��O�Ƃ���
				}
				else	{
					tnm_stack_push_int(lhs / rhs);
				}
				break;

			// �]��Z�͂O���Z�����邽�ߓ��ꏈ��
			case OP_AMARI:
				if (rhs == 0)	{
					tnm_set_error(TNM_ERROR_TYPE_DIVIDED_BY_ZERO, _T("0 ���Z���s���܂����I'%'"));
					tnm_stack_push_int(0);	// �����I�ɉ��Z���ʂ��O�Ƃ���
				}
				else	{
					tnm_stack_push_int(lhs % rhs);
				}
				break;
		}
	}

	// ������ op ����
	else if (form_l == FM_STR && form_r == FM_INT)	{	tnm_calculate_2_str_int(opr);	}

	// ������ op ������
	else if (form_l == FM_STR && form_r == FM_STR)	{	tnm_calculate_2_str_str(opr);	}
}

// �񍀉��Z�F������ op ����
void tnm_calculate_2_str_int(BYTE opr)
{
	int rhs = tnm_stack_pop_int();		// �E�l�����o��
	TSTR lhs = tnm_stack_pop_str();		// ���l�����o��
	switch (opr)	{
		case OP_MULTIPLE:		tnm_stack_push_str(lhs * rhs);			break;
	}
}

// �񍀉��Z�F������ op ������
void tnm_calculate_2_str_str(BYTE opr)
{
	TSTR rhs = tnm_stack_pop_str();		// �E�l�����o��
	TSTR lhs = tnm_stack_pop_str();		// ���l�����o��
	switch (opr)	{
		case OP_PLUS:			tnm_stack_push_str(lhs + rhs);			break;
		case OP_EQUAL:			tnm_stack_push_int(str_to_lower_ret(lhs) == str_to_lower_ret(rhs) ? 1 : 0);	break;
		case OP_NOT_EQUAL:		tnm_stack_push_int(str_to_lower_ret(lhs) != str_to_lower_ret(rhs) ? 1 : 0);	break;
		case OP_GREATER:		tnm_stack_push_int(str_to_lower_ret(lhs) >  str_to_lower_ret(rhs) ? 1 : 0);	break;
		case OP_GREATER_EQUAL:	tnm_stack_push_int(str_to_lower_ret(lhs) >= str_to_lower_ret(rhs) ? 1 : 0);	break;
		case OP_LESS:			tnm_stack_push_int(str_to_lower_ret(lhs) <  str_to_lower_ret(rhs) ? 1 : 0);	break;
		case OP_LESS_EQUAL:		tnm_stack_push_int(str_to_lower_ret(lhs) <= str_to_lower_ret(rhs) ? 1 : 0);	break;
	}
}

// ****************************************************************
// gosub ����
// ================================================================
bool tnm_command_proc_gosub(C_tnm_prop_list& arg_list, ARRAY<int>& arg_form_code_list, int ret_form_code)
{
	// �V�[�����烉�x���ԍ����擾
	int label_no = Gp_lexer->pop_ret<int>();

	// �X�^�b�N����������X�g���擾
	int arg_cnt = tnm_stack_pop_arg_list(arg_list, arg_form_code_list);

	// �R�[��
	if (!tnm_scene_proc_gosub(label_no, ret_form_code))
		return false;

	// �������R�[���ϐ��ɔz�u
	C_elm_call* p_cur_call = tnm_get_cur_call();
	int l_cnt = 0;
	int k_cnt = 0;
	for (int i = 0; i < arg_cnt; i++)	{
		if (arg_list[i].form == FM_INT)		p_cur_call->L.set_value(l_cnt++, arg_list[i].Int);
		if (arg_list[i].form == FM_STR)		p_cur_call->K.set_value(k_cnt++, arg_list[i].str);
	}

	return true;
}

// ****************************************************************
// farcall ����
// ================================================================
bool tnm_command_proc_farcall(int al_id, C_tnm_prop* al_begin, C_tnm_prop* al_end, int ret_form_code)
{
	TSTR scn_name = al_begin[0].str;	// �V�[����
	int z_no = 0;
	switch (al_id)	{
		case 1:		z_no = al_begin[1].Int;
		case 0:		scn_name = al_begin[0].str;
	}

	// �R�[��
	if (!tnm_scene_proc_farcall(scn_name, z_no, ret_form_code, false, false))
		return false;

	// �������R�[���ϐ��ɔz�u
	if (al_id == 1)	{
		C_elm_call* p_cur_call = tnm_get_cur_call();
		int l_cnt = 0;
		int k_cnt = 0;
		for (C_tnm_prop* arg = al_begin + 2; arg != al_end; ++arg)	{	// 2 = �R�[���t���O�ȊO�̈���
			if (arg->form == FM_INT)	p_cur_call->L.set_value(l_cnt++, arg->Int);
			if (arg->form == FM_STR)	p_cur_call->K.set_value(k_cnt++, arg->str);
		}
	}

	return true;
}

// ****************************************************************
// farcall_ex ����
// ================================================================
bool tnm_command_proc_farcall_ex(int al_id, C_tnm_prop* al_begin, C_tnm_prop* al_end, int ret_form_code)
{
	TSTR scn_name = al_begin[0].str;	// �V�[����
	int z_no = 0;
	switch (al_id)	{
		case 1:		z_no = al_begin[1].Int;
		case 0:		scn_name = al_begin[0].str;
	}

	// �R�[��
	if (!tnm_scene_proc_farcall(scn_name, z_no, ret_form_code, true, false))
		return false;

	// �������R�[���ϐ��ɔz�u
	if (al_id == 1)	{
		C_elm_call* p_cur_call = tnm_get_cur_call();
		int l_cnt = 0;
		int k_cnt = 0;
		for (C_tnm_prop* arg = al_begin + 2; arg != al_end; ++arg)	{	// 2 = �R�[���t���O�ȊO�̈���
			if (arg->form == FM_INT)	p_cur_call->L.set_value(l_cnt++, arg->Int);
			if (arg->form == FM_STR)	p_cur_call->K.set_value(k_cnt++, arg->str);
		}
	}

	return true;
}

// ****************************************************************
// return ����
// ================================================================
bool tnm_command_proc_return(C_tnm_prop_list& arg_list, ARRAY<int>& arg_form_code_list)
{
	// �X�^�b�N����������X�g���擾
	int arg_cnt = tnm_stack_pop_arg_list(arg_list, arg_form_code_list);

	// ���^�[�����̃R�[�������L�����Ă����i���[�h�������_�ŏ����Ă��܂����߁j
	C_elm_call* p_src_call = tnm_get_cur_call();
	int call_type = p_src_call->call_type;
	int frame_action_flag = p_src_call->frame_action_flag;

	// ���^�[��
	if (!tnm_scene_proc_return())
		return false;

	// ���^�[����̃R�[�������擾
	C_elm_call* p_dst_call = tnm_get_cur_call();
	int ret_form_code = p_dst_call->ret_form_code;

	// �߂�l�̌^�Ǝ��߂�l�̌^���`�F�b�N
	// �^���Ⴄ�ꍇ�̓_�~�[�̖߂�l������
	if (false);
	else if (ret_form_code == FM_INT)	{	// ������Ԃ�
		if (arg_cnt == 1 && arg_list[0].form == FM_INT)		tnm_stack_push_int(arg_list[0].Int);
		else												tnm_stack_push_int(0);
	}
	else if (ret_form_code == FM_STR)	{	// �������Ԃ�
		if (arg_cnt == 1 && arg_list[0].form == FM_STR)		tnm_stack_push_str(arg_list[0].str);
		else												tnm_stack_push_str(_T(""));
	}

	// �t���[���A�N�V�����̏ꍇ�̓��[�v�𔲂���I
	if (frame_action_flag == 1)
		return false;

	return true;
}

// ****************************************************************
// ���[�U�R�}���h����
// ================================================================
bool tnm_command_proc_user_cmd(int user_cmd_id, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	// ���[�U�R�}���h����
	if (!tnm_scene_proc_call_user_cmd(Gp_lexer->get_cur_scn_no(), user_cmd_id, p_ai->ret_form_code, false, false))
		return false;

	// �������X�^�b�N�ɐς�
	for (C_tnm_prop* arg = p_ai->al_begin; arg < p_ai->al_end; arg++)	{
		if (false);
		else if (arg->form == FM_INT)	{
			tnm_stack_push_int(arg->Int);
		}
		else if (arg->form == FM_STR)	{
			tnm_stack_push_str(arg->str);
		}
		else	{
			tnm_stack_push_element(arg->element);
		}
	}

	return true;
}

// ****************************************************************
// ���[�U�v���p�e�B����
// ================================================================
bool tnm_command_proc_user_prop(int user_prop_id, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{	
	C_tnm_prop* p_prop;

	// �C���N�t���O�̏ꍇ
	if (user_prop_id < Gp_lexer->get_user_inc_prop_cnt())	{
		p_prop = &Gp_user_inc_prop->prop_list[user_prop_id];
	}
	// �V�[���t���O�̏ꍇ
	else	{
		user_prop_id -= Gp_lexer->get_user_inc_prop_cnt();
		p_prop = &(*Gp_user_scn_prop_list)[Gp_lexer->get_cur_scn_no()].prop_list[user_prop_id];
	}

	// �v���p�e�B����
	if (!tnm_command_proc_prop(p_prop, elm_top, elm_begin, elm_end, p_ai))
		return false;

	return true;
}

// ****************************************************************
// �R�[���t���O����
// ================================================================
bool tnm_command_proc_call_prop(int call_prop_id, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	C_elm_call* p_cur_call = tnm_get_cur_call();
	C_tnm_prop* p_prop = &p_cur_call->user_prop_list[call_prop_id];

	// �v���p�e�B����
	if (!tnm_command_proc_prop(p_prop, elm_top, elm_begin, elm_end, p_ai))
		return false;


	return true;
}

// ****************************************************************
// �v���p�e�B����
// ================================================================
bool tnm_command_proc_prop(C_tnm_prop* p_prop, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	// �^�����߂�
	int form = p_prop->form;

	// �^�ɂ���ď����𕪊�
	if (false);
	else if (form == FM_INT && al_id == 0)	{
		tnm_stack_push_int(p_prop->Int);
	}
	else if (form == FM_INT && al_id == 1)	{
		p_prop->Int = p_ai->al_begin[0].Int;
	}
	else if (form == FM_STR)	{
		tnm_command_proc_str(p_prop->str, elm_top, elm_begin, elm_end, p_ai);
	}
	else if (form == FM_INTLIST)	{
		tnm_command_proc_int_list(boost::any_cast< BSP<C_elm_int_list> >(p_prop->any).get(), 32, elm_top, elm_begin, elm_end, p_ai);
	}
	else if (form == FM_STRLIST)	{
		tnm_command_proc_str_list(boost::any_cast< BSP<C_elm_str_list> >(p_prop->any).get(), elm_top, elm_begin, elm_end, p_ai);
	}
	else if (form == FM_INTREF || form == FM_STRREF)	{	// �Q�ƊO���i$a = 0 -> a[0] = 0�j
		tnm_stack_push_element(p_prop->element);
	}
	else if (form == FM_INTLISTREF || form == FM_STRLISTREF)	{	// �Q�ƊO���i$a = 0 -> a[0] = 0�j
		tnm_stack_push_element(p_prop->element);
	}
	else if (elm_begin != elm_end)	{						// �Q�ƊO���i$obj.x -> front.object[0].x�j
		tnm_stack_push_element(p_prop->element);
	}
	else if (al_id == 0)	{								// �Q�Ƃ��擾
		tnm_stack_push_element(p_prop->element);
	}
	else if (al_id == 1)	{								// �Q�Ƃ���
		p_prop->element = p_ai->al_begin[0].element;
	}

	return true;
}

// ****************************************************************
// �R�[���v���p�e�B���`
// ================================================================
bool tnm_declare_call_prop(int form_code, int prop_id, int size)
{
	// ���݂̃R�[�����擾
	C_elm_call* p_cur_call = tnm_get_cur_call();

	// �R�[���v���p�e�B���쐬
	int old_cnt = p_cur_call->user_prop_list.get_size();
	p_cur_call->user_prop_list.resize(old_cnt + 1);
	C_elm_user_call_prop* p_prop = p_cur_call->user_prop_list.get_sub(old_cnt, true);

	// �R�[���v���p�e�B��������
	p_prop->scn_no = Gp_lexer->get_cur_scn_no();
	p_prop->prop_id = prop_id;
	p_prop->form = form_code;

	// �z��̏ꍇ�͏�����
	if (false);
	else if (form_code == FM_INTLIST)	{
		BSP<C_elm_int_list> int_list(new C_elm_int_list);
		TSTR name = Gp_lexer->get_user_call_prop_name(p_prop->scn_no, p_prop->prop_id);
		int_list->init(S_element(), name, size, true);
		p_prop->any = int_list;
	}
	else if (form_code == FM_STRLIST)	{
		BSP<C_elm_str_list> str_list(new C_elm_str_list);
		TSTR name = Gp_lexer->get_user_call_prop_name(p_prop->scn_no, p_prop->prop_id);
		str_list->init(S_element(), name, size, true);
		p_prop->any = str_list;
	}

	return true;
}


// ****************************************************************
// �������R�[���t���O�ɓW�J����
// ================================================================
bool tnm_expand_arg_into_call_flag()
{
	// ���݂̃R�[�����擾
	C_elm_call* p_cur_call = tnm_get_cur_call();

	// �t���[���A�N�V�����̏ꍇ�͈����`�F�b�N
	// ���{���͌^�`�F�b�N���s���K�v������I
	if (p_cur_call->frame_action_flag == 1)	{

		// �����`�F�b�N
		if (p_cur_call->user_prop_list.get_size() == 0 || p_cur_call->user_prop_list[0].form != FM_FRAMEACTION)	{
			TSTR str = _T("�t���[���A�N�V�����̈����̑������� frameaction �^�łȂ���΂����܂���B�i�V�����@�\�j");
			TSTR cmd_name = Gp_lexer->get_user_cmd_name(p_cur_call->scn_no, p_cur_call->cmd_no);
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str + _T("\n(") + cmd_name + _T(")"));
			return false;
		}
		// �����̌����Ⴄ
		if (p_cur_call->arg_cnt != p_cur_call->user_prop_list.get_size())	{
			TSTR str = _T("�t���[���A�N�V�����̈����̌����s���ł��B");
			TSTR cmd_name = Gp_lexer->get_user_cmd_name(p_cur_call->scn_no, p_cur_call->cmd_no);
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str + _T("\n(") + cmd_name + _T(")"));
			return false;
		}
	}

	// �t���ɐς܂�Ă���̂ŋt���ɓǂݍ���
	for (int i = (int)p_cur_call->user_prop_list.get_size() - 1; i >= 0; i--)	{

		if (false);
		else if (p_cur_call->user_prop_list[i].form == FM_INT)	{
			p_cur_call->user_prop_list[i].Int = tnm_stack_pop_int();
		}
		else if (p_cur_call->user_prop_list[i].form == FM_STR)	{
			p_cur_call->user_prop_list[i].str = tnm_stack_pop_str();
		}
		else	{	// �Q�ƌ^
			tnm_stack_pop_element(p_cur_call->user_prop_list[i].element);
		}
	}

	return true;
}
