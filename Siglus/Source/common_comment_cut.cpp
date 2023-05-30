#include	"pch.h"
#include	"common_comment_cut.h"


// ****************************************************************
// �R�����g�J�b�g
// ================================================================
bool C_ini_comment_cut::comment_cut(ARRAY<TSTR>& in_txt_data)
{
	// �G���[�̏�����
	error_line_no = -1;
	error_str.clear();

	enum STATE
	{
		STATE_NONE,
		STATE_SINGLE_QUOTE,			// �V���O���R�[�e�[�V����
		STATE_SINGLE_QUOTE_ESCAPE,	// �V���O���R�[�e�[�V�����G�X�P�[�v
		STATE_DOUBLE_QUOTE,			// �_�u���R�[�e�[�V����
		STATE_DOUBLE_QUOTE_ESCAPE,	// �_�u���R�[�e�[�V�����G�X�P�[�v
		STATE_COMMENT_LINE,			// �s�R�����g
		STATE_COMMENT_BLOCK,		// �u���b�N�R�����g
	};
	
	STATE state = STATE_NONE;				// ���݂̃X�e�[�g
	int block_comment_start_line_no = 1;	// �u���b�N�R�����g�̊J�n�s
	int line_no = 1;						// ���݂̍s�ԍ�

	ARRAY<TSTR> out_txt_data;
	for (int i = 0; i < (int)in_txt_data.size(); i++)	{
		line_no = i + 1;
		TSTR in_line_data = in_txt_data[i];
		TSTR out_line_data;
		for (TSTR::const_iterator p = in_line_data.begin(); ; ++p)	{

			// ���s�̏ꍇ�̂ݓ��ꏈ��
			if (p == in_line_data.end())	{
				// �V���O���R�[�e�[�V�������Ȃ�G���[
				if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE)
					return error(line_no, _T("�V���O���R�[�e�[�V���� (') ���ɉ��s��������܂����B"));
				// �_�u���R�[�e�[�V�������Ȃ�G���[
				if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
					return error(line_no, _T("�_�u���R�[�e�[�V���� (\") ���ɉ��s��������܂����B"));
				// �s�R�����g���Ȃ�s�R�����g�I��
				if (state == STATE_COMMENT_LINE)
					state = STATE_NONE;
				// ���̍s��
				break;
			}

			TCHAR moji = *p;	// �o�͕���

			if (false);
			// �V���O���R�[�e�[�V������
			else if (state == STATE_SINGLE_QUOTE)	{
				// �V���O���R�[�e�[�V�����I��
				if (*p == _T('\''))
					state = STATE_NONE;
				// �V���O���R�[�e�[�V�����G�X�P�[�v
				else if (*p == _T('\\'))
					state = STATE_SINGLE_QUOTE_ESCAPE;
			}
			// �V���O���R�[�e�[�V�����G�X�P�[�v��
			else if (state == STATE_SINGLE_QUOTE_ESCAPE)	{
				// �G�X�P�[�v�\�ȕ���
				if (*p == _T('\'') || *p == _T('\\'))
					state = STATE_SINGLE_QUOTE;
				else
					return error(line_no, _T("�s���ȃG�X�P�[�v�i\\�j�ł��B'\\' ���g���ɂ� '\\\\' �Ə����ĉ������B"));
			}
			// �_�u���R�[�e�[�V������
			else if (state == STATE_DOUBLE_QUOTE)	{
				// �_�u���R�[�e�[�V�����I��
				if (*p == _T('"'))
					state = STATE_NONE;
				// �_�u���R�[�e�[�V�����G�X�P�[�v
				else if (*p == _T('\\'))
					state = STATE_DOUBLE_QUOTE_ESCAPE;
			}
			// �_�u���R�[�e�[�V�����G�X�P�[�v��
			else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
				// �G�X�P�[�v�\�ȕ���
				if (*p == _T('"') || *p == _T('\\'))
					state = STATE_DOUBLE_QUOTE;
				else
					return error(line_no, _T("�s���ȃG�X�P�[�v�i\\�j�ł��B'\\' ���g���ɂ� '\\\\' �Ə����ĉ������B"));
			}
			// �s�R�����g��
			else if (state == STATE_COMMENT_LINE)	{
				continue;	// �������Ȃ�
			}
			// �u���b�N�R�����g��
			else if (state == STATE_COMMENT_BLOCK)	{
				// �u���b�N�R�����g�I��
				if (str_analize_check_str(p, in_line_data.end(), _T("*/"), false))	{
					state = STATE_NONE;
					p++;
				}
				continue;	// �������Ȃ�
			}
			// �ʏ폈��
			else if (state == STATE_NONE)	{

				if (false);
				// �V���O���R�[�e�[�V�����J�n
				else if (*p == _T('\''))
					state = STATE_SINGLE_QUOTE;
				// �_�u���R�[�e�[�V�����J�n
				else if (*p == _T('"'))
					state = STATE_DOUBLE_QUOTE;
				// �s�R�����g�J�n
				else if (str_analize_check_str(p, in_line_data.end(), _T("//"), false))	{
					state = STATE_COMMENT_LINE;
					continue;
				}
				// �u���b�N�R�����g�J�n
				else if (str_analize_check_str(p, in_line_data.end(), _T("/*"), false))	{
					block_comment_start_line_no = line_no;
					state = STATE_COMMENT_BLOCK;
					continue;
				}
			}

			// �o��
			out_line_data.push_back(moji);
		}
		// �o��
		out_txt_data.push_back(out_line_data);
	}

	// �I�����̃X�e�[�g�ɂ���ăG���[
	if (state == STATE_SINGLE_QUOTE)
		return error(line_no, _T("�V���O���R�[�e�[�V���� (') �������Ă��܂���B"));
	if (state == STATE_DOUBLE_QUOTE)
		return error(line_no, _T("�_�u���R�[�e�[�V���� (\") �������Ă��܂���B"));
	if (state == STATE_COMMENT_BLOCK)
		return error(block_comment_start_line_no, _T(" �R�����g (/*) �������Ă��܂���B"));

	// ����
	in_txt_data = out_txt_data;
	return true;
}

bool C_ini_comment_cut::error(int line_no, CTSTR& str)
{
	error_line_no = line_no;
	error_str = str;

	return false;
}

