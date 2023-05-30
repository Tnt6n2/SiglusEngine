#pragma		once

// ****************************************************************
// Siglus �̃o�[�W����
// ================================================================
CTSTR	SIGLUS_VERSION = _T("1.1.134.0");

// ****************************************************************
// Z ���x���̌�
// ================================================================
const int	TNM_Z_LABEL_CNT = 1000;

// ****************************************************************
// Gameexe.dat�F�w�b�_�\����
// ================================================================
struct S_tnm_gameexe_dat_header
{
	int		version;
	int		exe_angou_mode;
};

// ****************************************************************
// �V�[���f�[�^�F�R�}���h���x���\����
// ================================================================
struct S_tnm_scn_cmd_label
{
	int		cmd_id;
	int		offset;
};

// ****************************************************************
// �V�[���f�[�^�F�V�[���v���p�e�B�\����
// ================================================================
struct S_tnm_scn_scn_prop
{
	int		form;
	int		size;
};

// ****************************************************************
// �V�[���f�[�^�F�V�[���R�}���h���x���\����
// ================================================================
struct S_tnm_scn_scn_cmd
{
	int		offset;
};

// ****************************************************************
// �V�[���f�[�^�F���ǃt���O�\����
// ================================================================
struct S_tnm_scn_read_flag
{
	int		line_no;	// �s�ԍ�
};

// ****************************************************************
// �V�[���f�[�^�F�w�b�_�\����
// ================================================================
struct S_tnm_scn_header
{
	int		header_size;					// �w�b�_�T�C�Y
	int		scn_ofs;						// �V�[��
	int		scn_size;						// �V�[���̃T�C�Y
	int		str_index_list_ofs;				// ������C���f�b�N�X���X�g
	int		str_index_cnt;					// ������C���f�b�N�X�̌�
	int		str_list_ofs;					// �����񃊃X�g
	int		str_cnt;						// ������̌�
	int		label_list_ofs;					// ���x�����X�g
	int		label_cnt;						// ���x���̌�
	int		z_label_list_ofs;				// �y���x�����X�g
	int		z_label_cnt;					// �y���x���̌�
	int		cmd_label_list_ofs;				// �R�}���h���x�����X�g
	int		cmd_label_cnt;					// �R�}���h���x���̌�
	int		scn_prop_list_ofs;				// �V�[���v���p�e�B���X�g
	int		scn_prop_cnt;					// �V�[���v���p�e�B�̌�
	int		scn_prop_name_index_list_ofs;	// �V�[���v���p�e�B���C���f�b�N�X���X�g
	int		scn_prop_name_index_cnt;		// �V�[���v���p�e�B���̌�
	int		scn_prop_name_list_ofs;			// �V�[���v���p�e�B�����X�g
	int		scn_prop_name_cnt;				// �V�[���v���p�e�B���̌�
	int		scn_cmd_list_ofs;				// �V�[���R�}���h���X�g
	int		scn_cmd_cnt;					// �V�[���R�}���h�̌�
	int		scn_cmd_name_index_list_ofs;	// �V�[���R�}���h���C���f�b�N�X���X�g
	int		scn_cmd_name_index_cnt;			// �V�[���R�}���h���C���f�b�N�X�̌�
	int		scn_cmd_name_list_ofs;			// �V�[���R�}���h�����X�g
	int		scn_cmd_name_cnt;				// �V�[���R�}���h���̌�
	int		call_prop_name_index_list_ofs;	// �R�[���v���p�e�B���C���f�b�N�X���X�g
	int		call_prop_name_index_cnt;		// �R�[���v���p�e�B���̌�
	int		call_prop_name_list_ofs;		// �R�[���v���p�e�B�����X�g
	int		call_prop_name_cnt;				// �R�[���v���p�e�B���̌�
	int		namae_list_ofs;					// �y���O�z���X�g
	int		namae_cnt;						// �y���O�z�̌�
	int		read_flag_list_ofs;				// ���ǃt���O���X�g
	int		read_flag_cnt;					// ���ǃt���O�̌�
};

// ****************************************************************
// �V�[���f�[�^�\����
// ================================================================
struct S_tnm_scn_data
{
	S_tnm_scn_header			header;						// �w�b�_
	C_stream					scn;						// �V�[��
	ARRAY<C_index>				str_index_list;				// ������C���f�b�N�X���X�g
	ARRAY<TSTR>					str_list;					// �����񃊃X�g
	ARRAY<int>					label_list;					// ���x�����X�g
	ARRAY<int>					z_label_list;				// �y���x�����X�g
	ARRAY<S_tnm_scn_cmd_label>	cmd_label_list;				// �R�}���h���x�����X�g�i�����J��p�j
	ARRAY<S_tnm_scn_scn_prop>	scn_prop_list;				// �V�[���v���p�e�B���X�g
	ARRAY<C_index>				scn_prop_name_index_list;	// �V�[���v���p�e�B���C���f�b�N�X���X�g
	ARRAY<TSTR>					scn_prop_name_list;			// �V�[���v���p�e�B�����X�g
	ARRAY<S_tnm_scn_scn_cmd>	scn_cmd_list;				// �V�[���R�}���h���X�g
	ARRAY<C_index>				scn_cmd_name_index_list;	// �V�[���R�}���h���C���f�b�N�X���X�g
	ARRAY<TSTR>					scn_cmd_name_list;			// �V�[���R�}���h�����X�g
	ARRAY<C_index>				call_prop_name_index_list;	// �R�[���v���p�e�B���C���f�b�N�X���X�g
	ARRAY<TSTR>					call_prop_name_list;		// �R�[���v���p�e�B�����X�g
	ARRAY<int>					namae_list;					// �y���O�z���X�g
	ARRAY<S_tnm_scn_read_flag>	read_flag_list;				// ���ǃt���O���X�g
};

// ****************************************************************
// �p�b�N�V�[���F�C���N�v���p�e�B�\����
// ================================================================
struct S_tnm_pack_scn_inc_prop
{
	int		form;
	int		size;
};

// ****************************************************************
// �p�b�N�V�[���F�C���N�R�}���h�\����
// ================================================================
struct S_tnm_pack_scn_inc_cmd
{
	int		scn_no;
	int		offset;
};

// ****************************************************************
// �p�b�N�V�[���F�w�b�_�\����
// ================================================================
struct S_tnm_pack_scn_header
{
	int		header_size;					// �w�b�_
	int		inc_prop_list_ofs;				// �C���N�v���p�e�B���X�g�ւ̃I�t�Z�b�g
	int		inc_prop_cnt;					// �C���N�v���p�e�B�̌�
	int		inc_prop_name_index_list_ofs;	// �C���N�v���p�e�B�����X�g
	int		inc_prop_name_index_cnt;		// �C���N�v���p�e�B�����X�g
	int		inc_prop_name_list_ofs;			// �C���N�v���p�e�B�����X�g
	int		inc_prop_name_cnt;				// �C���N�v���p�e�B�����X�g
	int		inc_cmd_list_ofs;				// �C���N�R�}���h���X�g�ւ̃I�t�Z�b�g
	int		inc_cmd_cnt;					// �C���N�R�}���h�̌�
	int		inc_cmd_name_index_list_ofs;	// �C���N�R�}���h�����X�g�ւ̃I�t�Z�b�g
	int		inc_cmd_name_index_cnt;			// �C���N�R�}���h���̌�
	int		inc_cmd_name_list_ofs;			// �C���N�R�}���h�����X�g�ւ̃I�t�Z�b�g
	int		inc_cmd_name_cnt;				// �C���N�R�}���h���̌�
	int		scn_name_index_list_ofs;		// �V�[�����C���f�b�N�X���X�g�ւ̃I�t�Z�b�g
	int		scn_name_index_cnt;				// �V�[�������X�g�̌�
	int		scn_name_list_ofs;				// �V�[�������X�g�ւ̃I�t�Z�b�g
	int		scn_name_cnt;					// �V�[�������X�g�̌�
	int		scn_data_index_list_ofs;		// �V�[���f�[�^�C���f�b�N�X���X�g�ւ̃I�t�Z�b�g
	int		scn_data_index_cnt;				// �V�[���f�[�^���X�g�̌�
	int		scn_data_list_ofs;				// �V�[���f�[�^���X�g�ւ̃I�t�Z�b�g
	int		scn_data_cnt;					// �V�[���f�[�^���X�g�̌�
	int		scn_data_exe_angou_mod;			// �V�[���f�[�^�̈Í������[�h
	int		original_source_header_size;	// �I���W�i���\�[�X�̃w�b�_�T�C�Y
};

// ****************************************************************
// �p�b�N�V�[���f�[�^�\����
// ================================================================
struct S_tnm_pack_scn_data
{
	S_tnm_pack_scn_header			header;						// �w�b�_
	ARRAY<S_tnm_pack_scn_inc_prop>	inc_prop_list;				// �C���N�v���p�e�B���X�g
	ARRAY<C_index>					inc_prop_name_index_list;	// �C���N�v���p�e�B�����X�g
	ARRAY<TSTR>						inc_prop_name_list;			// �C���N�v���p�e�B�����X�g
	ARRAY<S_tnm_pack_scn_inc_cmd>	inc_cmd_list;				// �C���N�R�}���h���X�g
	ARRAY<C_index>					inc_cmd_name_index_list;	// �C���N�R�}���h�����X�g
	ARRAY<TSTR>						inc_cmd_name_list;			// �C���N�R�}���h�����X�g
	ARRAY<C_index>					scn_name_index_list;		// �V�[�������X�g
	ARRAY<TSTR>						scn_name_list;				// �V�[�������X�g

	ARRAY<C_index>					nolzss_scn_data_index_list;		// �V�[���f�[�^���X�g
	ARRAY<BUFFER>					nolzss_scn_data_list;			// �V�[���f�[�^���X�g
	ARRAY<C_index>					lzss_scn_data_index_list;		// �V�[���f�[�^���X�g
	ARRAY<BUFFER>					lzss_scn_data_list;				// �V�[���f�[�^���X�g

	ARRAY<C_index>					noangou_scn_data_index_list;	// �Í����Ȃ��V�[���f�[�^���X�g
	ARRAY<BUFFER>					noangou_scn_data_list;			// �Í����Ȃ��V�[���f�[�^���X�g
	ARRAY<C_index>					angou_scn_data_index_list;		// �Í�������V�[���f�[�^���X�g
	ARRAY<BUFFER>					angou_scn_data_list;			// �Í�������V�[���f�[�^���X�g
};

// ****************************************************************
// �T���l�C���f�[�^�x�[�X�w�b�_�[
// ================================================================
struct S_tnm_thumbnail_database_header
{
	int		header_size;
	int		version;
	int		data_cnt;
};

