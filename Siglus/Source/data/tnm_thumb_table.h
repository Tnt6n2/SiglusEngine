#pragma		once

// ****************************************************************
// �T���l�C���e�[�u��
// ================================================================
class C_tnm_thumb_table
{
public:
	bool	create_table();			// �T���l�C���e�[�u��������������
	TSTR	calc_thumb_file_name();	// �ŏI�I�ɕ\������T���l�C���̃t�@�C�������擾����

	std::map<TSTR, TSTR>&	get_table()	{	return m_thumb_table;	}

private:
	std::map<TSTR, TSTR>	m_thumb_table;
};


