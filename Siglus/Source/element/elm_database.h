#pragma		once

#include	"element/elm_list.h"

struct S_tnm_database_row_header
{
	int call_no;		// �ďo���ԍ�
};

struct S_tnm_database_column_header
{
	int call_no;		// �ďo���ԍ�
	int data_type;		// �f�[�^�^�C�v
};

struct S_tnm_database_info
{
	int row_cnt;			// �s��
	int column_cnt;			// ��
	S_tnm_database_row_header*		p_row_header;		// �s�w�b�_�e�[�u���ւ̃|�C���^
	S_tnm_database_column_header*	p_column_header;	// ��w�b�_�e�[�u���ւ̃|�C���^
	DWORD* p_data;			// �f�[�^�e�[�u���ւ̃|�C���^
	char* p_str;			// ������f�[�^�e�[�u���ւ̃|�C���^
};

// ****************************************************************
// �G�������g�F�f�[�^�x�[�X
// ================================================================
class C_elm_database : public C_tnm_element
{
public:
	C_elm_database();

	void	init(S_element element, CTSTR& name, int ini_no);	// ������

	bool	load_dbs(CTSTR& file_path);
	bool	get_data(int item_call_no, int column_call_no, int* ret_value);
	bool	get_data(int item_call_no, int column_call_no, TSTR& ret_value);
	int		check_item_no(int item_call_no);
	int		check_column_no(int column_call_no);
	int		find_num(int column_call_no, int num);
	int		find_str(int column_call_no, CTSTR& str);
	int		find_str_real(int column_call_no, CTSTR& str);

private:
	TSTR	get_str(int str_offset);
	int		get_item_no(int item_call_no);
	int		get_column_no(int column_call_no);

	int					m_type;
	ARRAY<BYTE>			m_data;
	S_tnm_database_info	m_info;
};

// ****************************************************************
// �G�������g�F�f�[�^�x�[�X���X�g
// ================================================================
class C_elm_database_list : public C_elm_list<C_elm_database>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};
