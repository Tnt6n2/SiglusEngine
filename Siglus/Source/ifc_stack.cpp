#include	"pch.h"

#include	"tnm_form_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_lexer.h"
#include	"element/elm_prop.h"
#include	"engine/ifc_stack.h"

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�X�^�b�N�Ċm��
// ================================================================
void tnm_stack_realloc(int cnt)
{
	if (cnt >= (int)Gp_stack->int_alloc_cnt)	{
		while (cnt >= (int)Gp_stack->int_alloc_cnt)	{
			Gp_stack->int_alloc_cnt += 1024;
		}
		Gp_stack->int_p = (int *)::HeapReAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, Gp_stack->int_p, Gp_stack->int_alloc_cnt * sizeof(int));
	}
}

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�X�^�b�N�� int �l���o��
// ================================================================
void tnm_stack_push_int(int value)
{
	if (Gp_stack->int_now_cnt >= Gp_stack->int_alloc_cnt)	{
		while (Gp_stack->int_now_cnt >= Gp_stack->int_alloc_cnt)	{
			Gp_stack->int_alloc_cnt += 1024;
		}
		Gp_stack->int_p = (int *)::HeapReAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, Gp_stack->int_p, Gp_stack->int_alloc_cnt * sizeof(int));
	}
	*(Gp_stack->int_p + Gp_stack->int_now_cnt) = value;
	Gp_stack->int_now_cnt++;
}

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�X�^�b�N���� int �l���擾
// ================================================================
int tnm_stack_pop_int()
{
	Gp_stack->int_now_cnt--;
	return *(Gp_stack->int_p + Gp_stack->int_now_cnt);
}

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�X�^�b�N���疖���� int �l���Q��
// ================================================================
int tnm_stack_back_int()
{
	return *(Gp_stack->int_p + Gp_stack->int_now_cnt - 1);
}

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�X�^�b�N�ɕ�������o��
// ================================================================
void tnm_stack_push_str(CTSTR& str)
{
	Gp_stack->str_stack.push_back(str);
}

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�X�^�b�N���當������擾
// ================================================================
TSTR tnm_stack_pop_str()
{
	TSTR ret_value = Gp_stack->str_stack.back();
	Gp_stack->str_stack.pop_back();
	return ret_value;
}

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�X�^�b�N���疖���̕�������Q��
// ================================================================
TSTR tnm_stack_back_str()
{
	return Gp_stack->str_stack.back();
}

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�X�^�b�N�ɃG�������g���o��
// ================================================================
void tnm_stack_push_element(S_element& element)
{
	// �q�[�v���g��
	if (Gp_stack->int_now_cnt + element.code_cnt + 1 >= Gp_stack->int_alloc_cnt)	{
		Gp_stack->int_alloc_cnt += 1024 + element.code_cnt + 1;
		Gp_stack->int_p = (int *)::HeapReAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, Gp_stack->int_p, Gp_stack->int_alloc_cnt * sizeof(int));
	}

	// �X�^�b�N�|�C���g���X�g�Ƀ|�C���g��ǉ�
	Gp_stack->stack_point_list.push_back(Gp_stack->int_now_cnt);

	// �G�������g�R�[�h�𐳏��ɏo��
	int *dp = Gp_stack->int_p + Gp_stack->int_now_cnt;
	for (int i = 0; i < element.code_cnt; i++)
		*dp++ = element.code[i];
	// �X�^�b�N�𒲐�
	Gp_stack->int_now_cnt += element.code_cnt;// + 1;
}

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�X�^�b�N����G�������g���擾
// ================================================================
int tnm_stack_pop_element(S_element& element)
{
	// �G�������g�R�[�h�̒������擾
	int elm_cnt = Gp_stack->int_now_cnt - Gp_stack->stack_point_list.back();
	Gp_stack->stack_point_list.pop_back();

	// �R�s�[�|�C���g��ݒ�
	int *sp = Gp_stack->int_p + Gp_stack->int_now_cnt - elm_cnt;
	int *dp = element.code;

	// �R�s�[
	for (int i = 0; i < elm_cnt; i++)
		*dp++ = *sp++;

	// �G�������g��
	element.code_cnt = elm_cnt;

	// �X�^�b�N�𒲐�
	Gp_stack->int_now_cnt -= elm_cnt;// + 1;

	return elm_cnt;
}

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�X�^�b�N���ŃG�������g���R�s�[
// ================================================================
void tnm_stack_copy_element(S_element& element)
{
	// �G�������g�̒������擾
	int elm_cnt = Gp_stack->int_now_cnt - Gp_stack->stack_point_list.back();

	// �q�[�v���g��
	if (Gp_stack->int_now_cnt + elm_cnt + 1 >= Gp_stack->int_alloc_cnt)	{
		Gp_stack->int_alloc_cnt += 1024 + elm_cnt + 1;
		Gp_stack->int_p = (int *)::HeapReAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, Gp_stack->int_p, Gp_stack->int_alloc_cnt * sizeof(int));
	}

	// �R�s�[�|�C���g��ݒ�
	int* sp = Gp_stack->int_p + Gp_stack->int_now_cnt - elm_cnt;
	int* dp = Gp_stack->int_p + Gp_stack->int_now_cnt;

	// �R�s�[
	for (int i = 0; i < elm_cnt; i++)
		*dp++ = *sp++;

	// �X�^�b�N�|�C���g���X�g�Ƀ|�C���g��ǉ�
	Gp_stack->stack_point_list.push_back(Gp_stack->int_now_cnt);

	// �X�^�b�N���𒲐�
	Gp_stack->int_now_cnt += elm_cnt;
}

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X�F�������X�g���X�^�b�N����擾
// ================================================================
//		�X�^�b�N�ɂ́A���������珇�� push ����Ă��܂��B
//		�Ō�̈���������o���Ă������ƂɂȂ邽�߁A�t���ɕ��ёւ��܂��B
// ================================================================
void tnm_stack_pop_arg_list(C_tnm_prop_list& arg_list, int arg_form_code)
{
	if (false);
	else if (arg_form_code == FM_INT)	{
		arg_list[0].form = FM_INT;
		arg_list[0].Int = tnm_stack_pop_int();
	}
	else if (arg_form_code == FM_STR)	{
		arg_list[0].form = FM_STR;
		arg_list[0].str = tnm_stack_pop_str();
	}
	else	{
		arg_list[0].form = arg_form_code;
		tnm_stack_pop_element(arg_list[0].element);
	}
}

int tnm_stack_pop_arg_list(C_tnm_prop_list& arg_list, ARRAY<int>& arg_form_code_list)
{
	// �����̌^�R�[�h�̌����V�[������擾
	int arg_cnt = Gp_lexer->pop_ret<int>();

	// ���������Ɏ擾����
	for (int i = arg_cnt - 1; i >= 0; i--)	{

		// ����ID�͂܂�������Ȃ��̂ŏ��������Ă���
		arg_list[i].id = -1;

		// �����̌^���擾����
		arg_form_code_list[i] = Gp_lexer->pop_ret<int>();

		// �����̌^�𗊂�Ɉ������擾����
		if (false);
		else if (arg_form_code_list[i] == FM_INT)	{
			arg_list[i].form = FM_INT;
			arg_list[i].Int = tnm_stack_pop_int();
		}
		else if (arg_form_code_list[i] == FM_STR)	{
			arg_list[i].form = FM_STR;
			arg_list[i].str = tnm_stack_pop_str();
		}
		else if (arg_form_code_list[i] == FM_LABEL)	{
			arg_list[i].form = FM_INT;
			arg_list[i].Int = tnm_stack_pop_int();
		}
		else if (arg_form_code_list[i] == FM_LIST)	{
			ARRAY<int> exp_form_code_list(64);	// ��������ł���ΊO���Ő錾�������Ƃ���c

			arg_list[i].form = FM_LIST;
			arg_list[i].exp_cnt = tnm_stack_pop_arg_list(arg_list[i].exp_list, exp_form_code_list);
		}
		else	{
			arg_list[i].form = arg_form_code_list[i];
			tnm_stack_pop_element(arg_list[i].element);
		}
	}

	return arg_cnt;
}

