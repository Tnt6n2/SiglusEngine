#pragma		once

// ****************************************************************
// �f�t�h�c�}�l�[�W��
// ================================================================
//		������I�u�W�F�N�g�Ɍʂh�c��U��܂��B
// ================================================================


// ****************************************************************
// �f�t�h�c�N���X�̑O���錾
// ================================================================
class	C_tnm_guid;

// ****************************************************************
// �f�t�h�c�}�l�[�W��
// ================================================================
class C_tnm_guid_manager
{
	friend	class	C_tnm_guid;

public:

	C_tnm_guid_manager();

private:

	void	create_guid(C_tnm_guid* guid);			// GUID �����
	void	create_guid(C_tnm_guid* guid, int id);	// GUID �����iID�w��j
	void	release_guid(C_tnm_guid* guid);

	ARRAY<int>	m_enable_guid_list;		// �g�p�ł��� GUID ���X�g
	ARRAY<int>	m_ref_count_list;		// �Q�ƃJ�E���g
	int			m_guid_cnt;				// �쐬���� GUID �̌�
};

// ****************************************************************
// �f�t�h�c
// ================================================================
class C_tnm_guid
{
	friend	class	C_tnm_guid_manager;

public:
	C_tnm_guid();
	~C_tnm_guid();
	C_tnm_guid(const C_tnm_guid& rhs);
	C_tnm_guid& operator = (const C_tnm_guid& rhs);

	void	init();								// ����������
	void	create_guid();						// GUID ���쐬����
	void	release_guid();						// GUID ���������
	int		get_guid()	{	return m_guid;	}	// GUID ���擾����

private:

	int							m_guid;
	static	C_tnm_guid_manager	m_guid_manager;
};




