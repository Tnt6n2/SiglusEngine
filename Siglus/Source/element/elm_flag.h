#pragma		once

#include	"elm_prop.h"

class	C_tnm_save_stream;

// ****************************************************************
// �t���O
// ================================================================
class C_elm_flag
{
public:
	void	init_global();
	void	init_local();
	void	save_global(C_tnm_save_stream& stream);
	void	save_local(C_tnm_save_stream& stream);
	void	load_global(C_tnm_save_stream& stream);
	void	load_local(C_tnm_save_stream& stream);

	TSTR*		get_namae_flag_by_name(CTSTR& name);
	S_element	get_element_by_name(CTSTR& name);
	bool		get_flag_by_name(CTSTR& name, C_elm_str_list** pp_flag, int* p_flag_no);

	C_elm_int_list		A;
	C_elm_int_list		B;
	C_elm_int_list		C;
	C_elm_int_list		D;
	C_elm_int_list		E;
	C_elm_int_list		F;
	C_elm_int_list		X;	// �f�o�b�O�p

	C_elm_int_list		G;
	C_elm_int_list		Z;

	C_elm_str_list		S;
	C_elm_str_list		M;

	C_elm_int_list		H;
	C_elm_int_list		I;
	C_elm_int_list		J;

	C_elm_str_list		global_namae;
	C_elm_str_list		local_namae;

	DWORD				dummy_check_id;				// �_�~�[�t�@�C���`�F�b�N
};

// ****************************************************************
// ���[�U�C���N�v���p�e�B
// ================================================================
class C_elm_user_inc_prop
{
public:
	void	init();
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	C_tnm_prop_list	prop_list;		// ���[�U�v���p�e�B
};

// ****************************************************************
// ���[�U�V�[���v���p�e�B
// ================================================================
class C_elm_user_scn_prop
{
public:
	void	init(int scn_no);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	C_tnm_prop_list	prop_list;		// ���[�U�v���p�e�B
};

// ****************************************************************
// ���[�U�V�[���v���p�e�B���X�g
// ================================================================
class C_elm_user_scn_prop_list : public C_elm_list<C_elm_user_scn_prop>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

// ****************************************************************
// ���[�U�R�[���v���p�e�B
// ================================================================
class C_elm_user_call_prop : public C_tnm_prop
{
public:
	C_elm_user_call_prop() : C_tnm_prop(), scn_no(0), prop_id(0)	{}

	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	int		scn_no;
	int		prop_id;
};

// ****************************************************************
// ���[�U�R�[���v���p�e�B���X�g
// ================================================================
class C_tnm_user_call_prop_list : public C_elm_list<C_elm_user_call_prop>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

