#include	"pch.h"
#include	"ini.h"

// ****************************************************************
// ini �t�@�C�����
// ================================================================
bool C_ini_file_analizer::analize(TSTR src, TSTR& dst)
{
	TSTR result;

	if (!inia_comment_cut(src))
		return false;

	dst = src;
	return true;
}

bool C_ini_file_analizer::inia_comment_cut(TSTR& text)
{
	TSTR result;

	// �ԕ�������
	for (int i = 0; i < 256; i++)
		text.push_back(_T('\0'));

	enum STATE
	{
		STATE_NONE,
		STATE_DOUBLE_QUOTE,			// �_�u���R�[�e�[�V����
		STATE_DOUBLE_QUOTE_ESCAPE,	// �_�u���R�[�e�[�V�����G�X�P�[�v
		STATE_COMMENT_LINE,			// �s�R�����g
		STATE_COMMENT_BLOCK,		// �u���b�N�R�����g
	};
	
	int line = 1;						// �s�ԍ��i�P�s�ڂ���X�^�[�g�j
	int block_comment_start_line = 1;	// �u���b�N�R�����g�̊J�n�s
	STATE state = STATE_NONE;			// ���݂̃X�e�[�g

	for (TSTR::iterator p = text.begin(); *p != _T('\0'); ++p)	{

		TCHAR moji = *p;	// �o�͕���

		// ���s�̏ꍇ�̂ݓ��ꏈ��
		if (*p == _T('\n'))	{
			// �_�u���R�[�e�[�V�������Ȃ�G���[
			if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
				return error(line, _T("�_�u���R�[�e�[�V�������͉��s�ł��܂���B"));
			// �s�R�����g���Ȃ�s�R�����g�I��
			if (state == STATE_COMMENT_LINE)
				state = STATE_NONE;
			// �s���C���N�������g
			line ++;
		}
		// �_�u���R�[�e�[�V������
		else if (state == STATE_DOUBLE_QUOTE)	{
			// �G�X�P�[�v
			if (*p == _T('\\'))
				state = STATE_DOUBLE_QUOTE_ESCAPE;
			// �_�u���R�[�e�[�V�����I��
			if (*p == _T('"'))
				state = STATE_NONE;
		}
		// �_�u���R�[�e�[�V�����G�X�P�[�v��
		else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('"'))
				state = STATE_DOUBLE_QUOTE;
			else
				return error(line, _T("�s���ȃG�X�P�[�v�i\\�j�ł��B'\\' ���g���ɂ� '\\\\' �Ə����Ă��������B"));
		}
		// �s�R�����g��
		else if (state == STATE_COMMENT_LINE)	{
			continue;	// �������Ȃ�
		}
		// �u���b�N�R�����g��
		else if (state == STATE_COMMENT_BLOCK)	{
			// �u���b�N�R�����g�I��
			if (*p == _T('*') && *(p + 1) == _T('/'))	{
				state = STATE_NONE;
				p++;
			}
			continue;	// �������Ȃ�
		}
		// �ʏ폈��
		else if (state == STATE_NONE)	{

			if (false);
			// �_�u���R�[�e�[�V�����J�n
			else if (*p == _T('"'))
				state = STATE_DOUBLE_QUOTE;
			// �s�R�����g�J�n
			else if (*p == _T(';'))	{
				state = STATE_COMMENT_LINE;
				continue;
			}
			// �s�R�����g�J�n
			else if (*p == _T('/') && *(p + 1) == _T('/'))	{
				state = STATE_COMMENT_LINE;
				continue;
			}
			// �u���b�N�R�����g�J�n
			else if (*p == _T('/') && *(p + 1) == _T('*'))	{
				block_comment_start_line = line;
				state = STATE_COMMENT_BLOCK;
				continue;
			}
			// �A���t�@�x�b�g��������啶���ɕϊ�
			else if (_T('a') <= *p && *p <= _T('z'))
				moji += _T('A') - _T('a');
		}

		// ���ʂ̏o��
		result.push_back(moji);
	}

	// �I�����̃X�e�[�g�ɂ���ăG���[
	if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
		return error(line, _T("�����Ă��Ȃ� \" ������܂��B"));
	if (state == STATE_COMMENT_BLOCK)
		return error(block_comment_start_line, _T("�����Ă��Ȃ� /* ������܂��B"));

	// ����
	text = result;
	return true;
}

// ****************************************************************
// inc ��́F�G���[�̐ݒ�
// ================================================================
bool C_ini_file_analizer::error(int line, CTSTR& str)
{
	error_line = line;
	error_str = str;

	return false;
}

// ****************************************************************
// inc ��́F�G���[�̎擾
// ================================================================
int C_ini_file_analizer::get_error_line()
{
	return error_line;
}

TSTR C_ini_file_analizer::get_error_str()
{
	return error_str;
}

