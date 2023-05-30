#include	"pch.h"
#include	"data/tnm_guid.h"

// ****************************************************************
// �f�t�h�c�N���X�̃X�^�e�B�b�N�I�u�W�F�N�g
// ================================================================
C_tnm_guid_manager	C_tnm_guid::m_guid_manager;

// ****************************************************************
// �f�t�h�c�F�f�t�H���g�R���X�g���N�^
// ================================================================
C_tnm_guid::C_tnm_guid()
{
	// �t�H�[�}�b�g
	m_guid = -1;

	// ������
	init();
}

// ****************************************************************
// �f�t�h�c�F�R�s�[�R���X�g���N�^
// ================================================================
C_tnm_guid::C_tnm_guid(const C_tnm_guid& rhs)
{
	// �t�H�[�}�b�g
	m_guid = -1;

	// ������
	init();

	// GUID ���R�s�[����
	m_guid_manager.create_guid(this, rhs.m_guid);
}

// ****************************************************************
// �f�t�h�c�F������Z�q
// ================================================================
C_tnm_guid& C_tnm_guid::operator = (const C_tnm_guid& rhs)
{
	// ������
	init();

	// GUID ���R�s�[����
	m_guid_manager.create_guid(this, rhs.m_guid);

	return *this;
}

// ****************************************************************
// �f�t�h�c�F�f�X�g���N�^
// ================================================================
C_tnm_guid::~C_tnm_guid()
{
	release_guid();
}


// ****************************************************************
// �f�t�h�c�F������
// ================================================================
void C_tnm_guid::init()
{
	release_guid();
}

// ****************************************************************
// �f�t�h�c�F�f�t�h�c���쐬����
// ================================================================
void C_tnm_guid::create_guid()
{
	// GUID ������ĂȂ���������
	if (m_guid == -1)
		m_guid_manager.create_guid(this);
}

// ****************************************************************
// �f�t�h�c�F�f�t�h�c���������
// ================================================================
void C_tnm_guid::release_guid()
{
	// GUID ������Ă���Ή������
	if (m_guid >= 0)
		m_guid_manager.release_guid(this);
}


// ****************************************************************
// �f�t�h�c�}�l�[�W���F�R���X�g���N�^
// ================================================================
C_tnm_guid_manager::C_tnm_guid_manager()
{
	m_guid_cnt = 0;
}

// ****************************************************************
// �f�t�h�c�}�l�[�W���F�f�t�h�c���쐬
// ================================================================
void C_tnm_guid_manager::create_guid(C_tnm_guid* guid)
{
	// �h�c��\�񂷂�
	if (m_enable_guid_list.empty())	{
		guid->m_guid = m_guid_cnt;					// �V���� GUID �����蓖�Ă�
		m_ref_count_list.resize(m_guid_cnt + 1);	// �Q�ƃJ�E���g���X�g���g��
		m_ref_count_list[guid->m_guid] = 1;			// �Q�ƃJ�E���g��ݒ肷��
		m_guid_cnt ++;								// ���蓖�Ă� GUID ���𑝂₷
	}
	else	{
		guid->m_guid = m_enable_guid_list.back();	// �󂢂Ă��� GUID �����蓖�Ă�
		m_ref_count_list[guid->m_guid] ++;			// �Q�ƃJ�E���^���グ��
		m_enable_guid_list.pop_back();				// �g�p�ł��� GUID ���X�g����폜
	}
}

// ****************************************************************
// �f�t�h�c�}�l�[�W���F�f�t�h�c�����蓖�Ă�
// ================================================================
void C_tnm_guid_manager::create_guid(C_tnm_guid* guid, int id)
{
	if (id >= 0)	{
		guid->m_guid = id;							// GUID �����蓖�Ă�
		m_ref_count_list[guid->m_guid] ++;			// �Q�ƃJ�E���g���グ��
	}
}

// ****************************************************************
// �f�t�h�c�}�l�[�W���F�f�t�h�c�����
// ================================================================
void C_tnm_guid_manager::release_guid(C_tnm_guid* guid)
{
	// �h�c���������
	if (guid->m_guid >= 0)	{
		m_ref_count_list[guid->m_guid] --;			// �Q�ƃJ�E���g��������

		// �Q�ƃJ�E���^���O�ɂȂ����������X�g�ɒǉ�
		if (m_ref_count_list[guid->m_guid] == 0)	{
			m_enable_guid_list.push_back(guid->m_guid);
		}

		guid->m_guid = -1;							// GUID ���������
	}
}

