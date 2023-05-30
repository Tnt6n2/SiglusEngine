#pragma		once

struct		S_element;
class		C_tnm_prop;
class		C_tnm_prop_list;

// ****************************************************************
// �X�^�b�N�F�\����
// ================================================================
class C_tnm_stack
{
public:

	C_tnm_stack()
	{
		int_p = NULL;
		int_now_cnt = 0;
		int_alloc_cnt = 0;
	}

	// �����^�̃X�^�b�N
	int*				int_p;
	DWORD				int_now_cnt;
	DWORD				int_alloc_cnt;

	// ������^�̃X�^�b�N
	ARRAY<TSTR>			str_stack;

	// �G�������g�R�[�h�̐擪�ʒu���L������X�^�b�N
	// �G�������g�R�[�h���X�^�b�N���ŃR�s�[�����肷��̂Ɏg��
	ARRAY<int>			stack_point_list;
};

// ****************************************************************
// �X�^�b�N�F�C���^�[�t�F�[�X
// ================================================================
void		tnm_stack_realloc(int cnt);						// �X�^�b�N�m��
void		tnm_stack_push_int(int value);					// �X�^�b�N�� int �l���o��
int			tnm_stack_pop_int();							// �X�^�b�N���� int �l���擾
int			tnm_stack_back_int();							// �X�^�b�N���疖���� int �l���Q��
void		tnm_stack_push_str(CTSTR& str);					// �X�^�b�N�ɕ�������o��
TSTR		tnm_stack_pop_str();							// �X�^�b�N���當������擾
TSTR		tnm_stack_back_str();							// �X�^�b�N���疖���̕�������Q��
void		tnm_stack_push_element(S_element& element);		// �X�^�b�N�ɃG�������g���o��
int			tnm_stack_pop_element(S_element& element);		// �X�^�b�N����G�������g���擾
void		tnm_stack_copy_element(S_element& element);		// �X�^�b�N���ŃG�������g���R�s�[
void		tnm_stack_pop_arg_list(C_tnm_prop_list& arg_list, int arg_form_code);					// �X�^�b�N���������W�J�i�^�R�[�h���P�w�肷��j
int			tnm_stack_pop_arg_list(C_tnm_prop_list& arg_list, ARRAY<int>& arg_form_code_list);		// �X�^�b�N���������W�J�i�^�R�[�h�̓V�[������擾����j

