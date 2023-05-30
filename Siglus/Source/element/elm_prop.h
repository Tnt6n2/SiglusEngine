#pragma		once

#include	"tnm_form_code.h"
#include	"element/elm_list.h"

class	C_tnm_save_stream;
class	C_tnm_prop;

// ****************************************************************
// �v���p�e�B���X�g
// ================================================================
class C_tnm_prop_list : public C_elm_list<C_tnm_prop>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

// ****************************************************************
// �v���p�e�B
// ================================================================
class C_tnm_prop
{
public:
	C_tnm_prop() : id(0), form(FM_INT), Int(0), exp_cnt(0)	{}

	void	init();								// ������
	void	reinit();							// �ď�����
	void	save(C_tnm_save_stream& stream);	// �Z�[�u
	void	load(C_tnm_save_stream& stream);	// ���[�h

	int					id;				// ID�i���O�������Ɏg���j
	int					form;			// �^
	int					Int;			// �����^�̏ꍇ�̃f�[�^
	TSTR				str;			// ������^�̏ꍇ�f�[�^
	S_element			element;		// �Q�ƌ^�̏ꍇ�f�[�^
	C_tnm_prop_list		exp_list;		// �����X�g�̏ꍇ�̃f�[�^
	int					exp_cnt;		// �����X�g�̏ꍇ�̃f�[�^�̌�
	boost::any			any;			// ���̑��̃f�[�^
};

