#pragma		once

#include	"data/tnm_element.h"

class	C_tnm_save_stream;

// ****************************************************************
// �}�b�v
// ================================================================
class C_elm_map : public C_tnm_element
{
public:
	// �G�������g�̊�{����
	void	init(S_element element, CTSTR& name);					// ������
	void	reinit();												// �ď�����
	void	save(C_tnm_save_stream& stream);						// �Z�[�u
	void	load(C_tnm_save_stream& stream);						// ���[�h

	// �p�����[�^
	int		get_size()	{ return m_map.size(); }

private:
	std::map<TSTR, int>		m_map;
};

