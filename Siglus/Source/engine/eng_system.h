#pragma		once

// ****************************************************************
// �V�X�e�����
// ================================================================
//		�o�b�̏���\���\����
// ================================================================

// ****************************************************************
// �𑜓x�\����
// ================================================================
struct S_tnm_resolution
{
	C_size	resolution;
};

// �𑜓x�̔�r���Z�q
inline bool operator == (S_tnm_resolution& lhs, S_tnm_resolution& rhs)
{
	return lhs.resolution.cx == rhs.resolution.cx && lhs.resolution.cy == rhs.resolution.cy;
}

// �𑜓x�̔�r���Z�q
inline bool operator < (S_tnm_resolution& lhs, S_tnm_resolution& rhs)
{
	if (lhs.resolution.cx < rhs.resolution.cx)	return true;
	if (lhs.resolution.cx > rhs.resolution.cx)	return false;
	if (lhs.resolution.cy < rhs.resolution.cy)	return true;
	if (lhs.resolution.cy > rhs.resolution.cy)	return false;
	return false;
}

// �𑜓x���X�g
struct S_tnm_resolution_list
{
	ARRAY<S_tnm_resolution>	resolution_list;
	int						default_resolution_no;	// �f�t�H���g�̉𑜓x
};

// ****************************************************************
// �V�X�e�����̍\����
// ================================================================
class C_tnm_system_info
{
public:
	ARRAY<D3DADAPTER_IDENTIFIER9>	display_list;
	ARRAY<S_tnm_resolution_list>	resolution_table;
};

// ****************************************************************
// �o�b�֘A�̏���
// ================================================================
bool	tnm_get_os_name(TSTR& name);					// OS�̖��O���擾����
TSTR	tnm_get_cpu_name();								// CPU�̖��O���擾����

// ****************************************************************
// �f�o�C�X�֌W�̏���
// ================================================================
bool	tnm_check_device();								// �f�o�C�X���`�F�b�N����
TCHAR	tnm_search_disc_drive();						// �f�B�X�N�h���C�u����������

