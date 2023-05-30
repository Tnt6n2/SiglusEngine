#include	<tona3.h>

using namespace NT3;

#include	"common_command_line.h"

// ****************************************************************
// �R�}���h���C����̓N���X
// ================================================================
//		str:		�R�}���h���C��������
//		cmd_list:	�g�[�N����������
// ================================================================
bool C_command_line_analizer::analize_command_line(TSTR str, ARRAY<S_command_line_analizer_struct>& cmd_list)
{
	C_str_analizer anl(str.begin(), str.end());

	enum STATE
	{
		EMPTY,
		SLASH,
		KEY,
		EQUAL,
		VALUE,
	}	state = EMPTY;

	int cmd_no = 0;
	TSTR key, value;
	ARRAY<TSTR> value_list;
	while (1)	{

		// �����̏ꍇ�͓��ꏈ��
		if (anl.is_end())	{
			if (state == EMPTY)	{
				return true;		// �����I
			}
			else if (state == VALUE || state == KEY)	{

				// ���݂̃R�}���h��o�^
				S_command_line_analizer_struct cmd;
				cmd.key = key;
				cmd.value_list = value_list;
				cmd_list.push_back(cmd);

				// �R�}���h���N���A
				key.clear();
				value_list.clear();

				// �����I
				return true;
			}
			else	{
				erorr_str = tostr(std::max(cmd_no, 1)) + _T("�ڂ̈����̉�͂Ɏ��s���܂����B");
				return false;		// �G���[
			}
		}
		// �܂���̏ꍇ
		else if (state == EMPTY)	{
			if (anl.check_moji(_T('/')))	{
				state = SLASH;	// �R�}���h�� '/' �����������i���̃R�}���h�ցj
				cmd_no ++;
			}
			else	{
				erorr_str = tostr(std::max(cmd_no, 1)) + _T("�ڂ̈����̉�͂Ɏ��s���܂����B�R�}���h�̍ŏ��� '/' �Ŏn�߂Ȃ���΂����܂���B�B");
				return false;		// �G���[
			}
		}
		// '/' ������������
		else if (state == SLASH)	{
			if (anl.get_word(key))	{
				state = KEY;	// �R�}���h�̃L�[����������
			}
			else	{
				erorr_str = _T("'/' �ɑ����P��̉�͂Ɏ��s���܂����B") + tostr(std::max(cmd_no, 1)) + _T("�ڂ̈����̉�͂Ɏ��s���܂����B");
				return false;		// �G���[
			}
		}
		// �L�[������������
		else if (state == KEY)	{
			if (anl.check_moji(_T('/')))	{
				state = SLASH;	// �R�}���h�� '/' �����������i���̃R�}���h�ցj
				cmd_no ++;

				// ���݂̃R�}���h��o�^
				S_command_line_analizer_struct cmd;
				cmd.key = key;
				cmd.value_list = value_list;
				cmd_list.push_back(cmd);

				// �R�}���h���N���A
				key.clear();
				value_list.clear();
			}
			else if (anl.check_moji(_T('=')))	{
				state = EQUAL;	// �R�}���h�� '=' ����������
			}
			else	{
				erorr_str = tostr(std::max(cmd_no, 1)) + _T("�ڂ̈����̉�͂Ɏ��s���܂����B�R�}���h�̍ŏ��� '/' �Ŏn�߂Ȃ���΂����܂���B");
				return false;		// �G���[
			}
		}
		// '=' ������������
		else if (state == EQUAL)	{
			if (anl.get_dquote_str(value, _T('\\')))	{
				state = VALUE;	// �R�}���h�̒l����������

				// �l��o�^
				value_list.push_back(value);
			}
			else if (anl.get_word(key))	{
				state = VALUE;	// �R�}���h�̒l����������

				// �l��o�^
				value_list.push_back(value);
			}
			else	{
				erorr_str = tostr(std::max(cmd_no, 1)) + _T("�ڂ̈����̉�͂Ɏ��s���܂����B'=' �ɑ����l��������܂���B");
				return false;		// �G���[
			}
		}
		// �l������������
		else if (state == VALUE)	{
			if (anl.check_moji(_T('/')))	{
				state = SLASH;	// �R�}���h�� '/' �����������i���̃R�}���h�ցj
				cmd_no ++;

				// ���݂̃R�}���h��o�^
				S_command_line_analizer_struct cmd;
				cmd.key = key;
				cmd.value_list = value_list;
				cmd_list.push_back(cmd);

				// �R�}���h���N���A
				key.clear();
				value_list.clear();
			}
			else if (anl.get_dquote_str(value, _T('\\')))	{
				state = VALUE;	// �R�}���h�̒l����������

				// �l��o�^
				value_list.push_back(value);
			}
			else if (anl.get_word(key))	{
				state = VALUE;	// �R�}���h�̒l����������

				// �l��o�^
				value_list.push_back(value);
			}
			else	{
				erorr_str = tostr(std::max(cmd_no, 1)) + _T("�ڂ̈����̉�͂Ɏ��s���܂����B'=' �ɑ����l��������܂���B");
				return false;		// �G���[
			}
		}
	}

	return true;
}
