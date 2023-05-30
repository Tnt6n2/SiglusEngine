#pragma		once

#include	"tnm_common_header.h"

class	C_tnm_save_stream;

// ****************************************************************
// �V�[�����
// ================================================================
//		�e�V�[���f�[�^�̏����i�[���܂��B
// ================================================================
struct S_scn_info
{
	BYTE*					p_data_top;
	S_tnm_scn_header*		p_header;					// �w�b�_
	BYTE*					p_scn;						// �V�[��
	int*					p_label_list;				// ���x�����X�g
	int*					p_z_label_list;				// �y���x�����X�g
	C_index*				p_str_index_list;			// �����񃊃X�g�g�b�v
	TCHAR*					p_str_list;					// �����񃊃X�g
	S_tnm_scn_scn_prop*		p_scn_prop_list;			// �V�[���v���p�e�B���X�g
	C_index*				p_scn_prop_name_index_list;	// �V�[���v���p�e�B���C���f�b�N�X���X�g
	TCHAR*					p_scn_prop_name_list;		// �V�[���v���p�e�B�����X�g
	C_index*				p_call_prop_name_index_list;// �R�[���v���p�e�B���C���f�b�N�X���X�g
	TCHAR*					p_call_prop_name_list;		// �R�[���v���p�e�B�����X�g
	S_tnm_scn_scn_cmd*		p_scn_cmd_list;				// �V�[���R�}���h���X�g
	C_index*				p_scn_cmd_name_index_list;	// �V�[���R�}���h���C���f�b�N�X���X�g
	TCHAR*					p_scn_cmd_name_list;		// �V�[���R�}���h�����X�g
	int*					p_namae_list;				// �y���O�z���X�g
	S_tnm_scn_read_flag*	p_read_flag_list;			// ���ǃt���O���X�g

	std::map<TSTR, int>		scn_prop_no_map;		// �V�[���v���p�e�B�� �� �V�[���v���p�e�B�ԍ��̃}�b�v
	std::map<TSTR, int>		scn_cmd_no_map;			// �V�[���R�}���h�� �� �V�[���R�}���h�ԍ��̃}�b�v
};

// ****************************************************************
// �p�b�N�V�[�����
// ================================================================
//		�p�b�N�V�[���f�[�^�̏����i�[���܂��B
// ================================================================
struct S_pack_scn_info
{
	BYTE*						p_data_top;
	S_tnm_pack_scn_header*		p_header;
	S_tnm_pack_scn_inc_prop*	p_inc_prop_list;
	C_index*					p_inc_prop_name_index_list;
	TCHAR*						p_inc_prop_name_list;
	C_index*					p_inc_cmd_name_index_list;
	TCHAR*						p_inc_cmd_name_list;
	S_tnm_pack_scn_inc_cmd*		p_inc_cmd_list;
	C_index*					p_scn_name_index_list;
	TCHAR*						p_scn_name_list;
	C_index*					p_scn_data_index_list;
	BYTE*						p_scn_data_list;

	std::map<TSTR, int>			scn_no_map;			// �V�[���� �� �V�[���ԍ��̃}�b�v
	std::map<TSTR, int>			inc_prop_no_map;	// inc �v���p�e�B�� �� inc �v���p�e�B�ԍ��̃}�b�v
	std::map<TSTR, int>			inc_cmd_no_map;		// inc �R�}���h�� �� inc �R�}���h�ԍ��̃}�b�v
};

// ****************************************************************
// �X�N���v�g���N�T�[
// ================================================================
class C_tnm_scene_lexer
{
public:
	C_tnm_scene_lexer();

	// ������
	void	init();

	// Scene.pck ��ǂݍ���
	bool	load_scn(CTSTR& scn_pack_name);

	// �I���W�i���\�[�X����
	bool	is_exist_org_source();

	// ���[�U�v���p�e�B
	int		get_user_inc_prop_cnt();
	int		get_user_scn_prop_cnt(int scn_no);
	int		get_user_inc_prop_form(int prop_id);
	int		get_user_scn_prop_form(int scn_no, int prop_id);
	int		get_user_inc_prop_size(int prop_id);
	int		get_user_scn_prop_size(int scn_no, int prop_id);
	TSTR	get_user_inc_prop_name(int prop_id);
	TSTR	get_user_scn_prop_name(int scn_no, int prop_id);
	TSTR	get_user_call_prop_name(int scn_no, int prop_id);

	int		get_user_prop_no(CTSTR& name);					// ���[�U�v���p�e�B�ԍ����擾
	int		get_user_prop_no(int scn_no, CTSTR& name);		// ���[�U�v���p�e�B�ԍ����擾

	// ���[�U�R�}���h
	int		get_user_cmd_cnt(int scn_no);					// ���[�U�R�}���h�����擾
	int		get_user_cmd_no(int scn_no, CTSTR& name);		// ���[�U�R�}���h�ԍ����擾
	TSTR	get_user_cmd_name(int scn_no, int user_cmd_no);	// ���[�U�R�}���h�̖��O���擾
	bool	is_exist_user_cmd(int scn_no, CTSTR& name);		// ���[�U�R�}���h���ݔ���

	// �V�[��
	int		get_scn_cnt();									// �V�[�������擾
	int		get_scn_no(CTSTR& name);						// �V�[���ԍ����擾
	TSTR	get_scn_name(int scn_no);						// �V�[���̖��O���擾
	int		get_cur_scn_no();								// ���݂̃V�[���ԍ����擾
	void	set_cur_scn_no(int scn_no);						// ���݂̃V�[���ԍ���ݒ�
	int		get_cur_line_no();								// ���݂̍s�ԍ����擾
	void	set_cur_line_no(int line_no);					// ���݂̍s�ԍ���ݒ�
	BYTE*	get_cut_ptr();									// ���݂̃|�C���^���擾
	int		get_prg_cntr();									// �v���O�����J�E���^���擾
	void	set_prg_cntr(int prg_cntr);						// �v���O�����J�E���^��ݒ�
	bool	is_exist_scn(CTSTR& name);						// �V�[�����ݔ���
	bool	is_exist_z(int scn_no, int z_no);				// �y���x�����ݔ���

	int		get_namae_cnt(int scn_no);						// �y���O�z�̌����擾
	TSTR	get_namae_str(int scn_no, int name_no);			// �y���O�z�̕�������擾

	int		get_read_flag_cnt(int scn_no);					// ���ǃt���O�����擾
	int		get_read_flag_line_no(int scn_no, int rf_no);	// ���ǃt���O�̍s�ԍ����擾

	bool	jump_to_scn(int scn_no, int z_no);				// �V�[���ɃW�����v
	bool	jump_to_label(int label);						// ���x���ɃW�����v
	bool	jump_to_user_cmd(int scn_no, int user_cmd_id);	// ���[�U�R�}���h�ɃW�����v

	// �Z�[�u�^���[�h
	bool	load_local(int scn_no, int line_no, int prg_cntr);
	bool	save_call(C_tnm_save_stream& stream);
	bool	load_call(C_tnm_save_stream& stream);

	template <typename TYPE>
	inline TYPE	pop_ret()
	{
		m_cur_ptr += sizeof(TYPE);
		return *(TYPE *)(m_cur_ptr - sizeof(TYPE));
	}

	TSTR	pop_str_ret()
	{
		int str_index = pop_ret<int>();
		return get_string(str_index);
	}

	template <typename TYPE>
	TYPE	pop_str_indirect()
	{
		typedef TYPE::value_type CHAR_TYPE;

		int size = pop_ret<int>() * sizeof(CHAR_TYPE);		// �f�[�^�����擾

		cur_ptr += size;
		return TYPE((CHAR_TYPE *)(m_cur_ptr - size), (CHAR_TYPE *)m_cur_ptr);
	}

	TSTR	get_string_with_scn_no(int scn_no, int str_index);
	TSTR	get_string(int str_index);

private:

	// �V�[���f�[�^
	BUFFER				m_scn_data;			// �V�[���f�[�^
	S_pack_scn_info		m_pack_scn_info;	// �p�b�N�V�[�����
	ARRAY<S_scn_info>	m_scn_info_list;	// �V�[����񃊃X�g

	BYTE*				m_cur_ptr;			// ���݂̃|�C���^
	int					m_cur_scn_no;		// ���݂̃V�[���ԍ�
	int					m_cur_line_no;		// ���݂̍s�ԍ�
};

// ****************************************************************
// �V�[�������擾
// ================================================================
inline int C_tnm_scene_lexer::get_scn_cnt()
{
	return m_pack_scn_info.p_header->scn_data_cnt;
}

// ****************************************************************
// ���݂̃V�[���ԍ����擾
// ================================================================
inline int C_tnm_scene_lexer::get_cur_scn_no()
{
	return m_cur_scn_no;
}

// ****************************************************************
// ���݂̃V�[���ԍ���ݒ�
// ================================================================
inline void C_tnm_scene_lexer::set_cur_scn_no(int cur_scn_no)
{
	m_cur_scn_no = cur_scn_no;
}

// ****************************************************************
// ���݂̍s�ԍ����擾
// ================================================================
inline int C_tnm_scene_lexer::get_cur_line_no()
{
	return m_cur_line_no;
}

// ****************************************************************
// ���݂̍s�ԍ���ݒ�
// ================================================================
inline void C_tnm_scene_lexer::set_cur_line_no(int cur_line_no)
{
	m_cur_line_no = cur_line_no;
}

// ****************************************************************
// ���݂̃|�C���^���擾
// ================================================================
inline BYTE* C_tnm_scene_lexer::get_cut_ptr()
{
	return m_cur_ptr;
}

// ****************************************************************
// ���[�U�C���N�v���p�e�B�̌����擾
// ================================================================
inline int C_tnm_scene_lexer::get_user_inc_prop_cnt()
{
	return m_pack_scn_info.p_header->inc_prop_cnt;
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B�̌����擾
// ================================================================
inline int C_tnm_scene_lexer::get_user_scn_prop_cnt(int scn_no)
{
	return m_scn_info_list[scn_no].p_header->scn_prop_cnt;
}

// ****************************************************************
// ���[�U�C���N�v���p�e�B�̌^���擾
// ================================================================
inline int C_tnm_scene_lexer::get_user_inc_prop_form(int prop_id)
{
	return m_pack_scn_info.p_inc_prop_list[prop_id].form;
}

// ****************************************************************
// ���[�U�C���N�v���p�e�B�̃T�C�Y���擾�i�z��̏ꍇ�j
// ================================================================
inline int C_tnm_scene_lexer::get_user_inc_prop_size(int prop_id)
{
	return m_pack_scn_info.p_inc_prop_list[prop_id].size;
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B�̌^���擾
// ================================================================
inline int C_tnm_scene_lexer::get_user_scn_prop_form(int scn_no, int prop_id)
{
	return m_scn_info_list[scn_no].p_scn_prop_list[prop_id].form;
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B�̃T�C�Y���擾�i�z��̏ꍇ�j
// ================================================================
inline int C_tnm_scene_lexer::get_user_scn_prop_size(int scn_no, int prop_id)
{
	return m_scn_info_list[scn_no].p_scn_prop_list[prop_id].size;
}
