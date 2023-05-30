#pragma		once

#include	"data/tnm_timer.h"
#include	"data/tnm_proc_def.h"
#include	"element/elm_prop.h"

class	C_tnm_save_stream;

// ****************************************************************
// �v���Z�X
// ================================================================
class C_tnm_proc
{
public:
	C_tnm_proc()
	{
		type = TNM_PROC_TYPE_NONE;
		arg_list_id = 0;
		key_skip_enable_flag = false;
		skip_disable_flag = false;
		return_value_flag = false;
		option = 0;
	}

	explicit C_tnm_proc(TNM_PROC_TYPE type_)
	{
		type = type_;
		arg_list_id = 0;
		key_skip_enable_flag = false;
		skip_disable_flag = false;
		return_value_flag = false;
		option = 0;
	}

	C_tnm_proc(TNM_PROC_TYPE type_, int option_)
	{
		type = type_;
		arg_list_id = 0;
		key_skip_enable_flag = false;
		skip_disable_flag = false;
		return_value_flag = false;
		option = option_;
	}

	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	TNM_PROC_TYPE				type;			// �v���Z�X�̃^�C�v
	S_element					element;		// �v���Z�X�Ɋւ���G�������g
	int							arg_list_id;	// �v���Z�X�Ɋւ�������h�c
	C_tnm_prop_list				arg_list;		// �v���Z�X�Ɋւ������

	bool						key_skip_enable_flag;	// �L�[�ŃX�L�b�v�ł���
	bool						skip_disable_flag;		// ������΂��Ȃ�
	bool						return_value_flag;		// �I�����ɒl��߂�
	int							option;					// �I�v�V����
	S_tid						option_time_id;			// �I�v�V�����ƂȂ�ID
};

