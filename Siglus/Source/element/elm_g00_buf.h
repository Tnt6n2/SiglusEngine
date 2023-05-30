#pragma		once

#include	"element/elm_list.h"

// ****************************************************************
// �f�O�O�o�b�t�@
// ================================================================
class C_elm_g00_buf : public C_tnm_element
{
public:

	// �G�������g�̊�{����
	void	init(S_element element, CTSTR& name);	// ������
	void	reinit();								// �ď�����
	void	save(C_tnm_save_stream& stream);		// �Z�[�u
	void	load(C_tnm_save_stream& stream);		// ���[�h

	// �t�@�C���̓ǂݍ��݂Ɖ��
	bool	load_file(CTSTR& file_name);
	void	free();

	// ��Ԏ擾
	bool	is_load()		{ return m_album ? true : false; }
	TSTR	get_file_name()	{ return m_file_name; }

private:
	TSTR				m_file_name;
	BSP<C_d3d_album>	m_album;
};

// ****************************************************************
// �f�O�O�o�b�t�@���X�g
// ================================================================
class C_elm_g00_buf_list : public C_elm_list<C_elm_g00_buf>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};
