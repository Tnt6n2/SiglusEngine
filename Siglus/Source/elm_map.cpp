#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_element.h"
#include	"element/elm_map.h"

// ****************************************************************
// �}�b�v�F������
// ================================================================
void C_elm_map::init(S_element element, CTSTR& name)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_VOID, name, NULL);

	// �p�����[�^��������
	m_map.clear();
}

// ****************************************************************
// �}�b�v�F������
// ================================================================
void C_elm_map::reinit()
{
	// �p�����[�^��������
	m_map.clear();
}

// ****************************************************************
// �}�b�v�F�Z�[�u
// ================================================================
void C_elm_map::save(C_tnm_save_stream& stream)
{
	stream.save(m_map.size());
	for (std::map<TSTR, int>::iterator itr = m_map.begin(); itr != m_map.end(); ++itr)
	{
		stream.save(itr->first);
		stream.save(itr->second);
	}
}

// ****************************************************************
// �}�b�v�F���[�h
// ================================================================
void C_elm_map::load(C_tnm_save_stream& stream)
{
	int num = stream.load_ret<int>();
	for (int i = 0; i < num; i++)
	{
		TSTR key = stream.load_ret<TSTR>();
		int value = stream.load_ret<int>();
		m_map.insert(std::pair<TSTR, int>(key, value));
	}

}





