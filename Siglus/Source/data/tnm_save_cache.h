#pragma		once

#include	"tnm_def.h"

// ****************************************************************
// �G���W���F���ݒ�Z�[�u�w�b�_
// ================================================================
struct S_tnm_config_save_header
{
	int		major_version;
	int		minor_version;
	int		config_data_size;	
};

// ****************************************************************
// �G���W���F�O���[�o���Z�[�u�w�b�_
// ================================================================
struct S_tnm_global_save_header
{
	int		major_version;
	int		minor_version;
	int		global_data_size;	
};

// ****************************************************************
// �G���W���F���ǃZ�[�u�w�b�_
// ================================================================
struct S_tnm_read_save_header
{
	int		major_version;
	int		minor_version;
	int		read_data_size;
	int		scn_cnt;
};

// ****************************************************************
// �G���W���F�Z�[�u�w�b�_
// ================================================================
struct S_tnm_save_header
{
	int		major_version;
	int		minor_version;
	int		year;
	int		month;
	int		day;
	int		weekday;
	int		hour;
	int		minute;
	int		second;
	int		millisecond;
	TCHAR	append_dir[TNM_SAVE_APPEND_DIR_MAX_LEN];
	TCHAR	append_name[TNM_SAVE_APPEND_NAME_MAX_LEN];
	TCHAR	title[TNM_SAVE_TITLE_MAX_LEN];
	TCHAR	message[TNM_SAVE_MESSAGE_MAX_LEN];
	TCHAR	full_message[TNM_SAVE_FULL_MESSAGE_MAX_LEN];
	TCHAR	comment[TNM_SAVE_COMMENT_MAX_LEN];
	TCHAR	comment2[TNM_SAVE_COMMENT2_MAX_LEN];
	int		flag[TNM_SAVE_FLAG_MAX_CNT];
	int		data_size;		// �f�[�^�̃T�C�Y

	bool operator == (const S_tnm_save_header& rhs) const	{
		return
			major_version == rhs.major_version &&
			minor_version == rhs.minor_version &&
			year == rhs.year &&
			month == rhs.month &&
			day == rhs.day &&
			weekday == rhs.weekday &&
			hour == rhs.hour &&
			minute == rhs.minute &&
			second == rhs.second &&
			millisecond == rhs.millisecond &&
			_tcscmp(title, rhs.title) == 0;
	}

	bool operator != (const S_tnm_save_header& rhs) const	{
		return
			major_version != rhs.major_version ||
			minor_version != rhs.minor_version ||
			year != rhs.year ||
			month != rhs.month ||
			day != rhs.day ||
			weekday != rhs.weekday ||
			hour != rhs.hour ||
			minute != rhs.minute ||
			second != rhs.second ||
			millisecond != rhs.millisecond ||
			_tcscmp(title, rhs.title) != 0;
	}

	bool operator < (const S_tnm_save_header& rhs) const
	{
		if (year < rhs.year)					return true;
		if (year > rhs.year)					return false;
		if (month < rhs.month)					return true;
		if (month > rhs.month)					return false;
		if (day < rhs.day)						return true;
		if (day > rhs.day)						return false;
		if (hour < rhs.hour)					return true;
		if (hour > rhs.hour)					return false;
		if (minute < rhs.minute)				return true;
		if (minute > rhs.minute)				return false;
		if (second < rhs.second)				return true;
		if (second > rhs.second)				return false;
		if (millisecond < rhs.millisecond)		return true;
		if (millisecond > rhs.millisecond)		return false;
		if (_tcscmp(title, rhs.title) == -1)	return true;
		if (_tcscmp(title, rhs.title) == 1)		return false;
		return true;
	}
};

// ****************************************************************
// �Z�[�u�f�[�^�L���b�V��
// ================================================================
class C_tnm_save_cache
{

public:

	void		init();			// ������
	bool		clear_cache(int save_no);								// �N���A
	bool		clear_new_cache();										// �ŐV�̃Z�[�u�f�[�^�ԍ��N���A
	bool		save_cache(int save_no, S_tnm_save_header* p_header);	// �Z�[�u
	bool		load_cache(int save_no, S_tnm_save_header* p_header);	// ���[�h
	bool		check_file(int save_no);								// �`�F�b�N
	int			get_new_save_data_no();									// �ŐV�̃Z�[�u�f�[�^�ԍ����擾
	int			get_new_quick_save_data_no();							// �ŐV�̃N�C�b�N�Z�[�u�f�[�^�ԍ����擾

private:

	struct S_cache
	{
		BSP<S_tnm_save_header>	save_header;
		bool					data_none_flag;

		S_cache()	{	data_none_flag = false;		}		// �R���X�g���N�^
	};
	ARRAY<S_cache>	cache_list;

	// �ŐV�̃Z�[�u�f�[�^�ԍ�
	int		new_save_data_no;
	int		new_quick_save_data_no;

	bool	memory_cache(int save_no, S_tnm_save_header* p_header);	// �L��

};





