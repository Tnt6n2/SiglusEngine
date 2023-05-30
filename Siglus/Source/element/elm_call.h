#pragma		once

#include	"data/tnm_element.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_flag.h"

class	C_tnm_save_stream;

// ****************************************************************
// �R�[���̃^�C�v
// ================================================================
enum E_tnm_call_type
{
	E_tnm_call_type_none = 0,
	E_tnm_call_type_gosub = 1,
	E_tnm_call_type_farcall = 2,
	E_tnm_call_type_user_cmd = 3,
};

// ****************************************************************
// �R�[���F�Z�[�u�\����
// ================================================================
struct S_elm_call_save_struct
{
	TSTR	lexer_scn_name;
	int		lexer_line_no;
	int		lexer_prg_cntr;
};

// ****************************************************************
// �R�[��
// ================================================================
class C_elm_call : public C_tnm_element
{
public:
	void	init(S_element element, CTSTR& name);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	// �R�[���v���p�e�B
	C_tnm_user_call_prop_list	user_prop_list;
	C_elm_int_list				L;
	C_elm_str_list				K;

	// ���̑��̏��
	E_tnm_call_type		call_type;
	bool				excall_flag;
	bool				frame_action_flag;
	int					ret_form_code;
	int					arg_cnt;		// �����̌��i�t���[���A�N�V�������̂ݐݒ肳��܂��j
	int					scn_no;			// ���[�U�R�}���h�̃V�[���ԍ��i�t���[���A�N�V�������̂ݐݒ肳��܂��j
	int					cmd_no;			// ���[�U�R�}���h�̃R�}���h�ԍ��i�t���[���A�N�V�������̂ݐݒ肳��܂��j

	S_elm_call_save_struct	m_call_save;		// �v���O�����J�E���^�ȂǁA�O���������Ă���f�[�^��ۑ����邽�߂̍\����

	// �f�o�b�O���
	int		src_scn_no;
	int		dst_scn_no;
	int		dst_label_no;
};

// ****************************************************************
// �R�[�����X�g
// ================================================================
class C_elm_call_list : public C_elm_list<C_elm_call>
{
public:
	void	init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent);	// ������
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	int		get_call_cnt()	{	return m_call_cnt;	}
	void	set_call_cnt(int call_cnt);
	void	add_call();
	void	sub_call();

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h

private:
	int		m_call_cnt;		// �������̂��߁A�R�[���̐������͂Ő��䂵�܂��B
};

