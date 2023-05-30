#include	"pch.h"
#include	"IA.h"
#include	"CA.h"

// ****************************************************************
// �������́F�����񂩂�P����擾�i�g���Łj
// ================================================================
bool str_analize_get_word_ex(TSTR::const_iterator& itr_, TSTR::const_iterator end, TSTR& word)
{
	TSTR::const_iterator itr = itr_;

	// �P�����ڂ͉p���܂��� '_'
	if (itr == end || (!is_alpha(*itr) && !is_zenkaku(*itr) && *itr != _T('_') && *itr != _T('@')))
		return false;

	++itr;

	// �Q�����ڈȍ~�͉p�����܂��� '_'
	for (; itr != end; ++itr)	{
		if (!is_alpha(*itr) && !is_num(*itr) && !is_zenkaku(*itr) && *itr != _T('_') && *itr != _T('@'))
			break;
	}

	word = TSTR(itr_, itr);
	itr_ = itr;
	return true;
}


// ****************************************************************
// �u���c���[�F�u���c���[���쐬
// ================================================================
void C_replace_tree::create_replace_tree(S_replace_node& root, ARRAY< BSP<S_tnms_ia_replace> >& replace_list)
{
	for (ARRAY< BSP<S_tnms_ia_replace> >::iterator itr = replace_list.begin(); itr != replace_list.end(); ++itr)
		add_replace(root, *itr, 0);
}

// ****************************************************************
// �u���c���[�F�u����ǉ�
// ================================================================
void C_replace_tree::add_replace(S_replace_node& node, BSP<S_tnms_ia_replace> p_replace, int index)
{
	// ���O�̏I�[�ɗ����ꍇ�A�f�[�^��ݒ肷��
	if (index == p_replace->name.size())	{
		node.p_replace = p_replace;
	}
	// ���O���܂������ꍇ�A�q���}�b�v�ɐi��
	else	{
		TCHAR moji = p_replace->name[index];
		add_replace(node.child_map[moji], p_replace, index + 1);
	}
}

// ****************************************************************
// �u���c���[�F�u��������
// ================================================================
void C_replace_tree::search_replace(S_replace_node& node, TSTR::const_iterator p, BSP<S_tnms_ia_replace>& ret_replace)
{
	// ������
	if (*p == _T('\0'))	{
		ret_replace = node.p_replace;
	}
	else	{

		std::map<TCHAR, S_replace_node>::iterator itr = node.child_map.find(*p);
		if (itr != node.child_map.end())	{
			search_replace(itr->second, p + 1, ret_replace);
			if (!ret_replace)	{
				ret_replace = node.p_replace;
			}
		}
		else	{
			ret_replace = node.p_replace;
		}
	}
}

// ****************************************************************
// inc �t�@�C����̓f�[�^�F������
// ================================================================
void S_tnms_ia_data::init()
{
	replace_tree.clear();
	property_list.clear();
	command_list.clear();
	name_set.clear();

	// �f�t�H���g�̌^�e�[�u�����쐬
	form_table.create_system_form_table();

	// �G�������g���̏�����
	property_cnt = 0;
	command_cnt = 0;
	inc_property_cnt = 0;
	inc_command_cnt = 0;
}

// ****************************************************************
// inc �t�@�C����̓f�[�^�Q�F������
// ================================================================
void S_tnms_ia_data_2::init()
{
	property_temp_list.clear();
	property_line_list.clear();
	command_temp_list.clear();
	command_line_list.clear();
}

// ****************************************************************
// inc �t�@�C�����
// ================================================================
bool C_inc_file_analizer::analize_step_1(CTSTR& text, int parent_form, S_tnms_ia_data* piad, S_tnms_ia_data_2* piad2)
{
	m_text = text;
	m_parent_form = parent_form;
	m_piad = piad;
	m_piad2 = piad2;

	// �R�����g�J�b�g�i����ɏ������ɕϊ����܂��j
	if (!ia_comment_cut())
		return false;

	// ���C���X�e�b�v�P
	if (!ia_main_step_1())
		return false;

	return true;
}

bool C_inc_file_analizer::analize_step_2(S_tnms_ia_data* piad, S_tnms_ia_data_2* piad2)
{
	m_piad = piad;
	m_piad2 = piad2;

	// ���C���X�e�b�v�Q
	if (!ia_main_step_2())
		return false;

	return true;
}

// ****************************************************************
// inc ��́F�R�����g�J�b�g
// ================================================================
bool C_inc_file_analizer::ia_comment_cut()
{
	TSTR out_text;

	// �G���[�̏�����
	m_error_line = -1;
	m_error_str.clear();

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
	
	STATE state = STATE_NONE;			// ���݂̃X�e�[�g
	int block_comment_start_line = 1;	// �u���b�N�R�����g�̊J�n�s
	int line = 1;						// ���݂̍s��

	for (TSTR::const_iterator p = m_text.begin(); p != m_text.end(); ++p)	{

		TCHAR moji = *p;	// �o�͕���

		// ���s�̏ꍇ�̂ݓ��ꏈ��
		if (*p == _T('\n'))	{
			// �V���O���R�[�e�[�V�������Ȃ�G���[
			if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE)
				return error(line, _T("�V���O���R�[�e�[�V���� (') ���ɉ��s��������܂����B"));
			// �_�u���R�[�e�[�V�������Ȃ�G���[
			if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
				return error(line, _T("�_�u���R�[�e�[�V���� (\") ���ɉ��s��������܂����B"));
			// �s�R�����g���Ȃ�s�R�����g�I��
			if (state == STATE_COMMENT_LINE)
				state = STATE_NONE;
			// �s���C���N�������g
			line ++;
		}
		// �V���O���R�[�e�[�V������
		else if (state == STATE_SINGLE_QUOTE)	{
			// �V���O���R�[�e�[�V�����I��
			if (*p == _T('\''))	{
				state = STATE_NONE;
			}
			// �V���O���R�[�e�[�V�����G�X�P�[�v
			else if (*p == _T('\\'))	{
				state = STATE_SINGLE_QUOTE_ESCAPE;
			}
		}
		// �V���O���R�[�e�[�V�����G�X�P�[�v��
		else if (state == STATE_SINGLE_QUOTE_ESCAPE)	{
			// �G�X�P�[�v�\�ȕ���
			if (*p == _T('"') || *p == _T('\\') || *p == _T('n'))	{
				state = STATE_SINGLE_QUOTE;
			}
			else	{
				return error(line, _T("�s���ȃG�X�P�[�v�i\\�j�ł��B'\\' ���g���ɂ� '\\\\' �Ə����Ă��������B"));
			}
		}
		// �_�u���R�[�e�[�V������
		else if (state == STATE_DOUBLE_QUOTE)	{
			// �_�u���R�[�e�[�V�����I��
			if (*p == _T('"'))	{
				state = STATE_NONE;
			}
			// �_�u���R�[�e�[�V�����G�X�P�[�v
			else if (*p == _T('\\'))	{
				state = STATE_DOUBLE_QUOTE_ESCAPE;
			}
		}
		// �_�u���R�[�e�[�V�����G�X�P�[�v��
		else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
			// �G�X�P�[�v�\�ȕ���
			if (*p == _T('"') || *p == _T('\\') || *p == _T('n'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			else	{
				return error(line, _T("�s���ȃG�X�P�[�v�i\\�j�ł��B'\\' ���g���ɂ� '\\\\' �Ə����Ă��������B"));
			}
		}
		// �s�R�����g��
		else if (state == STATE_COMMENT_LINE)	{
			continue;	// �������Ȃ�
		}
		// �u���b�N�R�����g��
		else if (state == STATE_COMMENT_BLOCK)	{
			// �u���b�N�R�����g�I��
			if (p + 1 != m_text.end() && *p == _T('*') && *(p + 1) == _T('/'))	{
				state = STATE_NONE;
				p++;
			}
			continue;	// �������Ȃ�
		}
		// �ʏ폈��
		else if (state == STATE_NONE)	{

			if (false);
			// �V���O���R�[�e�[�V�����J�n
			else if (*p == _T('\''))	{
				state = STATE_SINGLE_QUOTE;
			}
			// �_�u���R�[�e�[�V�����J�n
			else if (*p == _T('"'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			// �s�R�����g�J�n
			else if (*p == _T(';'))	{
				state = STATE_COMMENT_LINE;
				continue;
			}
			// �s�R�����g�J�n
			else if (p + 1 != m_text.end() && *p == _T('/') && *(p + 1) == _T('/'))	{
				state = STATE_COMMENT_LINE;
				continue;
			}
			// �u���b�N�R�����g�J�n
			else if (p + 1 != m_text.end() && *p == _T('/') && *(p + 1) == _T('*'))	{
				block_comment_start_line = line;
				state = STATE_COMMENT_BLOCK;
				continue;
			}
			else	{
				if (_T('A') <= moji && moji <= _T('Z'))
					moji -= _T('A') - _T('a');
			}
		}

		// �o��
		out_text.push_back(moji);
	}

	// �I�����̃X�e�[�g�ɂ���ăG���[
	if (state == STATE_SINGLE_QUOTE)
		return error(line, _T("�V���O���R�[�e�[�V���� (') �������Ă��܂���B"));
	if (state == STATE_DOUBLE_QUOTE)
		return error(line, _T("�_�u���R�[�e�[�V���� (\") �������Ă��܂���B"));
	if (state == STATE_COMMENT_BLOCK)
		return error(block_comment_start_line, _T(" �R�����g (/*) �������Ă��܂���B"));

	// ����
	m_text = out_text;
	return true;
}

// ****************************************************************
// inc ��́F���C��
// ================================================================
bool C_inc_file_analizer::ia_main_step_1()
{
	// �p�����[�^�̏�����
	m_error_line = -1;
	m_error_str.clear();
	int line = 1;

	// ��͊J�n
	for (TSTR::const_iterator p = m_text.begin(); p != m_text.end();)	{

		// �Z�p���[�^���X�L�b�v
		if (!ia_skip_separator(p, &line))
			break;	// �I�[�Ȃ�I��

		// �錾��T��
		BSP<S_tnms_ia_declare> declare(new S_tnms_ia_declare);
		if (!ia_declare(p, &line, declare))
			return false;
	}

	return true;
}

// ****************************************************************
// inc ��́F�Z�p���[�^���X�L�b�v
// ================================================================
bool C_inc_file_analizer::ia_skip_separator(TSTR::const_iterator& p_ret, int* line_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	while (1)	{
		if (false);

		// �I�[
		else if (p == m_text.end())
			return false;
		// ���s
		else if (*p == _T('\n'))	{
			p++;
			line++;
		}
		// ���̑��̃Z�p���[�^
		else if (*p == ' ' || *p == '\t')
			p++;
		// �Z�p���[�^�ȊO�̕���
		else
			break;
	}

	// ����
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F�錾�̎擾
// ================================================================
bool C_inc_file_analizer::ia_declare(TSTR::const_iterator& p_ret, int* line_ret, BSP<S_tnms_ia_declare>& declare_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	TSTR::const_iterator p_org = p_ret;
	int p_org_int = (int)(p_org - m_text.begin());

	BSP<S_tnms_ia_declare> declare(new S_tnms_ia_declare);

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);
	// �R�}���h�^�C�v
	if (!ia_declare_type(p, &line, &declare->type))
		return false;

	TSTR declare_name;			// �錾�̖��O
	int declare_line = line;	// �錾�̍s

	if (false);

	// #replace �̏���
	else if (declare->type == TNMS_IA_DECLARE_TYPE_REPLACE)	{
		declare->replace = BSP<S_tnms_ia_replace>(new S_tnms_ia_replace);
		declare->replace->type = TNMS_IA_REPLACE_TYPE_REPLACE;
		if (!ia_define_name(p, &line, declare->replace->name))				return false;
		if (!ia_define_after(p, &line, declare->replace->after))			return false;
		declare_name = declare->replace->name;		// �d������p�̖��O

		// �}�N���͂Q�����ȏ�K�{�i�P�����n�j�ɂȂ�܂��� 2013/01/17�j
		if (declare_name.length() < 1)
			return error(declare_line, _T("#replace �̖��O�͂P�����ȏ�K�v�ł��B"));

		// �d������
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(declare_line, declare_name + _T(" ����d�ɐ錾����Ă��܂��B"));
		m_piad->name_set.insert(declare_name);

		// �u���c���[��ǉ�
		C_replace_tree::add_replace(m_piad->replace_tree, declare->replace, 0);
	}

	// #define �̏���
	else if (declare->type == TNMS_IA_DECLARE_TYPE_DEFINE)	{
		declare->replace = BSP<S_tnms_ia_replace>(new S_tnms_ia_replace);
		declare->replace->type = TNMS_IA_REPLACE_TYPE_DEFINE;
		if (!ia_define_name(p, &line, declare->replace->name))				return false;
		if (!ia_define_after(p, &line, declare->replace->after))			return false;
		declare_name = declare->replace->name;		// �d������p�̖��O

		// �}�N���͂Q�����ȏ�K�{�i�P�����n�j�ɂȂ�܂��� 2013/01/17�j
		if (declare_name.length() < 1)
			return error(declare_line, _T("#define �̖��O�͂P�����ȏ�K�v�ł��B"));

		// �d������
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(declare_line, declare_name + _T(" ����d�ɐ錾����Ă��܂��B"));
		m_piad->name_set.insert(declare_name);

		// �u���c���[��ǉ�
		C_replace_tree::add_replace(m_piad->replace_tree, declare->replace, 0);
	}

	// #define_s �̏���
	else if (declare->type == TNMS_IA_DECLARE_TYPE_DEFINE_SPACE)	{
		declare->replace = BSP<S_tnms_ia_replace>(new S_tnms_ia_replace);
		declare->replace->type = TNMS_IA_REPLACE_TYPE_DEFINE;
		if (!ia_define_space_name(p, &line, declare->replace->name))		return false;
		if (!ia_define_after(p, &line, declare->replace->after))			return false;
		declare_name = declare->replace->name;		// �d������p�̖��O

		// �}�N���͂Q�����ȏ�K�{�i�P�����n�j�ɂȂ�܂��� 2013/01/17�j
		if (declare_name.length() < 1)
			return error(declare_line, _T("#define �̖��O�͂P�����ȏ�K�v�ł��B"));

		// �d������
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(declare_line, declare_name + _T(" ����d�ɐ錾����Ă��܂��B"));
		m_piad->name_set.insert(declare_name);

		// �u���c���[��ǉ�
		C_replace_tree::add_replace(m_piad->replace_tree, declare->replace, 0);
	}

	// #macro �̏���
	else if (declare->type == TNMS_IA_DECLARE_TYPE_MACRO)	{
		declare->replace = BSP<S_tnms_ia_replace>(new S_tnms_ia_replace);
		declare->replace->type = TNMS_IA_REPLACE_TYPE_MACRO;
		if (!ia_macro_name(p, &line, declare->replace->name))				return false;
		if (!ia_macro_arg_list(p, &line, declare->replace->arg_list))		return false;
		if (!ia_macro_after(p, &line, declare->replace->after))				return false;
		declare_name = declare->replace->name;		// �d������p�̖��O

		// �}�N���͂P�����ȏ�K�{�i�P�����n�j�ɂȂ�܂��� 2013/01/17�j
		if (declare_name.length() < 1)
			return error(declare_line, _T("#macro �̖��O�͂P�����ȏ�K�v�ł��B"));

		// �}�N���̐擪�� '@'
		if (declare_name[0] != _T('@'))
			return error(declare_line, _T("#macro �̖��O�� '@' �Ŏn�߂Ȃ���΂����܂���B"));

		// �d������
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(declare_line, declare_name + _T(" ����d�ɐ錾����Ă��܂��B"));
		m_piad->name_set.insert(declare_name);

		// �u���c���[��ǉ�
		C_replace_tree::add_replace(m_piad->replace_tree, declare->replace, 0);
	}

	// #property �̏���
	else if (declare->type == TNMS_IA_DECLARE_TYPE_PROPERTY)	{

		// �Z�p���[�^���X�L�b�v
		ia_skip_separator(p, &line);
		int name_line = line;

		// �ڍׂȉ�͂͂܂��s��Ȃ��i���̃X�e�b�v�ōs���j
		TSTR text;
		ia_property(p, &line, text);
		m_piad2->property_temp_list.push_back(text);
		m_piad2->property_line_list.push_back(name_line);
	}

	// #command �̏���
	else if (declare->type == TNMS_IA_DECLARE_TYPE_COMMAND)	{

		// �Z�p���[�^���X�L�b�v
		ia_skip_separator(p, &line);
		int name_line = line;

		// �ڍׂȉ�͂͂܂��s��Ȃ��i���̃X�e�b�v�ōs���j
		TSTR text;
		ia_command(p, &line, text);
		m_piad2->command_temp_list.push_back(text);
		m_piad2->command_line_list.push_back(name_line);
	}

	// #expand �̏���
	else if (declare->type == TNMS_IA_DECLARE_TYPE_EXPAND)	{
		TSTR after;
		if (!ia_macro_after(p, &line, after))				return false;

		// �ԕ��̑}��
		for (int i = 0; i < 256; i++)
			after.push_back(_T('\0'));

		// �u��
		for (TSTR::iterator p2 = after.begin(); *p2 != _T('\0');)
			std_replace(after, p2, &line, m_piad->replace_tree, S_replace_node());

		// �ԕ����폜
		after.erase(after.end() - 256, after.end());

		// ���e�L�X�g�ɒǉ�����Ƃ������\���i�s�ԍ�����܂����c�j
		TSTR left = m_text.substr(0, (int)(p_org - m_text.begin()));
		TSTR right = m_text.substr((int)(p - m_text.begin()));
		m_text = left + after + right;

		// �ēW�J���邽�߂Ɉʒu��߂�
		p = m_text.begin() + p_org_int;
	}

	// ����
	declare_ret = declare;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F�錾�^�C�v�̎擾
// ================================================================
bool C_inc_file_analizer::ia_declare_type(TSTR::const_iterator& p_ret, int* line_ret, TNMS_IA_DECLARE_TYPE* type_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	TNMS_IA_DECLARE_TYPE type = TNMS_IA_DECLARE_TYPE_NONE;

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// �R�}���h�����擾
	if (false);
	else if (str_analize_check_str(p, m_text.end(), _T("#replace")))
		type = TNMS_IA_DECLARE_TYPE_REPLACE;
	else if (str_analize_check_str(p, m_text.end(), _T("#define_s")))
		type = TNMS_IA_DECLARE_TYPE_DEFINE_SPACE;
	else if (str_analize_check_str(p, m_text.end(), _T("#define")))
		type = TNMS_IA_DECLARE_TYPE_DEFINE;
	else if (str_analize_check_str(p, m_text.end(), _T("#macro")))
		type = TNMS_IA_DECLARE_TYPE_MACRO;
	else if (str_analize_check_str(p, m_text.end(), _T("#property")))
		type = TNMS_IA_DECLARE_TYPE_PROPERTY;
	else if (str_analize_check_str(p, m_text.end(), _T("#command")))
		type = TNMS_IA_DECLARE_TYPE_COMMAND;
	else if (str_analize_check_str(p, m_text.end(), _T("#expand")))
		type = TNMS_IA_DECLARE_TYPE_EXPAND;
	else
		return error(*line_ret, _T("�s���Ȑ錾������܂��B���x���̏ꍇ�́A'#' �� '##' �ɕύX���Ă��������B"));

	// ����
	*type_ret = type;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#define �̖��O
// ================================================================
bool C_inc_file_analizer::ia_define_name(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ���݂̈ʒu���L��
	TSTR::const_iterator name_start_pos = p;

	// ��؂�ȊO�������Ԑi��
	for (; !(p == m_text.end() || *p == _T(' ') || *p == _T('\t') || *p == _T('\n')); ++p);

	// ���O���쐬
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// ����
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#define �̖��O
// ================================================================
bool C_inc_file_analizer::ia_define_space_name(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ���݂̈ʒu���L��
	TSTR::const_iterator name_start_pos = p;

	// ��؂�ȊO�������Ԑi��
	for (; !(p == m_text.end() || *p == _T('\t') || *p == _T('\n')); ++p);

	// ���O���쐬
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// ����
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#define �̒u����
// ================================================================
bool C_inc_file_analizer::ia_define_after(TSTR::const_iterator& p_ret, int* line_ret, TSTR& after_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	TSTR after;

	enum IF_STATE
	{
		IF_STATE_NONE = 0,
		IF_STATE_TRUE = 1,
		IF_STATE_FALSE = 2,
		IF_STATE_FALSE_MORE = 3,
	};

	IF_STATE if_state[16] = { IF_STATE_NONE };		// ���݂� if �X�e�[�g
	int if_depth = 0;								// ���݂� if �̐[��

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ��͊J�n
	for (; p != m_text.end();)	{
		if (false);

		// ����
		else if (p == m_text.end())	{
			break;
		}
		// �G�X�P�[�v
		else if (str_analize_check_str(p, m_text.end(), _T("##")))	{
			after += _T('#');
		}
		// #ifdef
		else if (str_analize_check_str(p, m_text.end(), _T("#ifdef")))	{

			// �Z�p���[�^���X�L�b�v
			if (!ia_skip_separator(p, &line))
				return error(line, _T("#ifdef �ɑ����P�ꂪ����܂���B"));

			TSTR word;
			if (str_analize_get_word_ex(p, m_text.end(), word)) {

				std::set<TSTR>::iterator itr = m_piad->name_set.find(word);
				if (itr != m_piad->name_set.end()) {
					if_state[++if_depth] = IF_STATE_TRUE;
					continue;
				}
				else {
					if_state[++if_depth] = IF_STATE_FALSE;
					continue;
				}
			}
			else {
				return error(line, _T("#ifdef �ɑ����P�ꂪ����܂���B"));
			}
		}
		// #elseifdef
		else if (str_analize_check_str(p, m_text.end(), _T("#elseifdef"))) {

			if (if_state[if_depth] > 0) {

				// �Z�p���[�^���X�L�b�v
				if (!ia_skip_separator(p, &line))
					return error(line, _T("#elseifdef �ɑ����P�ꂪ����܂���B"));

				TSTR word;
				if (str_analize_get_word_ex(p, m_text.end(), word)) {

					if (if_state[if_depth] == IF_STATE_FALSE_MORE)	{
						continue;
					}
					else if (if_state[if_depth] == IF_STATE_TRUE)	{
						if_state[if_depth] = IF_STATE_FALSE_MORE;
						continue;
					}
					else {

						std::set<TSTR>::iterator itr = m_piad->name_set.find(word);
						if (itr != m_piad->name_set.end()) {
							if_state[if_depth] = IF_STATE_TRUE;
							continue;
						}
						else {
							if_state[if_depth] = IF_STATE_FALSE;
							continue;
						}
					}
				}
				else {
					return error(line, _T("#elseifdef �ɑ����P�ꂪ����܂���B"));
				}
			}
			else {
				return error(line, _T("���� #elseifdef �ɑΉ����� #if ������܂���B"));
			}
		}
		// #else
		else if (str_analize_check_str(p, m_text.end(), _T("#else"))) {

			if (if_state[if_depth] > 0) {

				if (if_state[if_depth] == IF_STATE_FALSE_MORE)	{
					continue;
				}
				else if (if_state[if_depth] == IF_STATE_TRUE)	{
					if_state[if_depth] = IF_STATE_FALSE_MORE;
					continue;
				}
				else {
					if_state[if_depth] = IF_STATE_TRUE;
					continue;
				}
			}
			else {
				return error(line, _T("���� #else �ɑΉ����� #if ������܂���B"));
			}
		}
		// #endif
		else if (str_analize_check_str(p, m_text.end(), _T("#endif"))) {

			if (if_state[if_depth] > 0) {
				--if_depth;
				continue;
			}
			else {
				return error(line, _T("���� #endif �ɑΉ����� #if ������܂���B"));
			}
		}
		// ���s
		else if (*p == _T('\n'))	{
			after += _T(' ');
			line++;
			p++;
		}
		// if false ���͂����܂�
		else if (if_state[if_depth] == IF_STATE_FALSE || if_state[if_depth] == IF_STATE_FALSE_MORE) {
			p++;
		}
		// # ������ΏI��
		else if (*p == _T('#'))	{
			break;
		}
		// ���̑��̕����͂��̂܂�
		else	{
			after += *p;
			p++;
		}
	}

	// �Ō�̃X�y�[�X�Ȃǂ�؂�
	int not_space_pos = (int)after.find_last_not_of(_T(" \t"));
	if (not_space_pos != TSTR::npos)	{
		after.erase(not_space_pos + 1, after.size());
	}

	// ����
	after_ret = after;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#macro �̖��O
// ================================================================
bool C_inc_file_analizer::ia_macro_name(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ���݂̈ʒu���L��
	TSTR::const_iterator name_start_pos = p;

	// ��؂�ȊO�������Ԑi��
	for (; !(p == m_text.end() || *p == _T(' ') || *p == _T('\t') || *p == _T('\n') || *p == _T('(')); ++p);

	// ���O���쐬
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// ����
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#macro �̈������X�g
// ================================================================
bool C_inc_file_analizer::ia_macro_arg_list(TSTR::const_iterator& p_ret, int* line_ret, ARRAY<S_tnms_ia_macro_arg>& arg_list_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	ARRAY<S_tnms_ia_macro_arg> arg_list;

	// �Z�p���[�^���X�L�b�v
	if (!ia_skip_separator(p, &line))
		return error(line, _T("�������X�g�̈����̉�͂Ɏ��s���܂����B"));

	// '('
	if (*p == _T('('))	{
		p++;

		// �������X�g
		while (1)	{

			// �������擾
			S_tnms_ia_macro_arg arg;
			if (!ia_macro_arg(p, &line, arg))
				return error(line, _T("�������X�g�̈����̉�͂Ɏ��s���܂����B"));

			arg_list.push_back(arg);

			// �Z�p���[�^���X�L�b�v
			if (!ia_skip_separator(p, &line))
				return error(line, _T("�������X�g�� '(' �������Ă��܂���B"));

			// ',' �łȂ��Ȃ�I��
			if (*p != _T(','))
				break;

			// ���̕�����
			p++;
		}

		// �Z�p���[�^���X�L�b�v
		if (!ia_skip_separator(p, &line))
			return error(line, _T("�������X�g�� '(' �������Ă��܂���B"));

		// ')'
		if (*p++ != ')')
			return error(line, _T("�������X�g�� '(' �������Ă��܂���B"));
	}

	// ����
	arg_list_ret = arg_list;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#macro �̈���
// ================================================================
bool C_inc_file_analizer::ia_macro_arg(TSTR::const_iterator& p_ret, int* line_ret, S_tnms_ia_macro_arg& arg_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	S_tnms_ia_macro_arg arg;

	// �Z�p���[�^���X�L�b�v
	if (!ia_skip_separator(p, &line))
		return error(line, _T("�������X�g�̈�����������܂���B"));

	// ���݂̈ʒu���L��
	TSTR::const_iterator arg_start_pos = p;

	// ��؂�ȊO�������Ԑi��
	for (; !(p == m_text.end() || *p == _T(' ') || *p == _T('\t') || *p == _T('\n') || *p == _T(',') || *p == _T('(') || *p == _T(')') || *p == _T('"') || *p == _T('\'')); ++p);

	// �����̖��O�m��
	arg.name = TSTR(arg_start_pos, p);
	if (arg.name.empty())
		return error(line, _T("�������X�g�̈�����������܂���B"));

	// �Z�p���[�^���X�L�b�v
	if (!ia_skip_separator(p, &line))
		return error(line, _T("�������X�g�I���� ')' ��������܂���B"));

	// �f�t�H���g�l
	if (*p == _T('('))	{
		p++;

		// ���݂̈ʒu���L��
		TSTR::const_iterator def_start_pos = p;

		// �f�t�H���g�l�I����������܂Ői��
		for (; *p != _T(')'); ++p)	{

			if (p == m_text.end() || *p == _T('\t') || *p == _T('\n'))
				return error(line, _T("�����̃f�t�H���g�l����͒��ɕs���ȕ�����������܂����B"));
		}

		arg.def_value = TSTR(def_start_pos, p);
		p++;
	}

	// ����
	arg_ret = arg;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#macro �̒u����
// ================================================================
bool C_inc_file_analizer::ia_macro_after(TSTR::const_iterator& p_ret, int* line_ret, TSTR& after_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	TSTR after;

	enum IF_STATE
	{
		IF_STATE_NONE = 0,
		IF_STATE_TRUE = 1,
		IF_STATE_FALSE = 2,
		IF_STATE_FALSE_MORE = 3,
	};

	IF_STATE if_state[16] = { IF_STATE_NONE };		// ���݂� if �X�e�[�g
	int if_depth = 0;								// ���݂� if �̐[��

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ��͊J�n
	for (; p != m_text.end();)	{
		if (false);

		// ����
		else if (p == m_text.end())	{
			break;
		}
		// �G�X�P�[�v
		else if (str_analize_check_str(p, m_text.end(), _T("##"), false))	{
			after += _T('#');
			p += 2;
		}
		// #ifdef
		else if (str_analize_check_str(p, m_text.end(), _T("#ifdef")))	{

			// �Z�p���[�^���X�L�b�v
			if (!ia_skip_separator(p, &line))
				return error(line, _T("#ifdef �ɑ����P�ꂪ����܂���B"));

			TSTR word;
			if (str_analize_get_word_ex(p, m_text.end(), word)) {

				std::set<TSTR>::iterator itr = m_piad->name_set.find(word);
				if (itr != m_piad->name_set.end()) {
					if_state[++if_depth] = IF_STATE_TRUE;
					continue;
				}
				else {
					if_state[++if_depth] = IF_STATE_FALSE;
					continue;
				}
			}
			else {
				return error(line, _T("#ifdef �ɑ����P�ꂪ����܂���B"));
			}
		}
		// #elseifdef
		else if (str_analize_check_str(p, m_text.end(), _T("#elseifdef"))) {

			if (if_state[if_depth] > 0) {

				// �Z�p���[�^���X�L�b�v
				if (!ia_skip_separator(p, &line))
					return error(line, _T("#elseifdef �ɑ����P�ꂪ����܂���B"));

				TSTR word;
				if (str_analize_get_word_ex(p, m_text.end(), word)) {

					if (if_state[if_depth] == IF_STATE_FALSE_MORE)	{
						continue;
					}
					else if (if_state[if_depth] == IF_STATE_TRUE)	{
						if_state[if_depth] = IF_STATE_FALSE_MORE;
						continue;
					}
					else {

						std::set<TSTR>::iterator itr = m_piad->name_set.find(word);
						if (itr != m_piad->name_set.end()) {
							if_state[if_depth] = IF_STATE_TRUE;
							continue;
						}
						else {
							if_state[if_depth] = IF_STATE_FALSE;
							continue;
						}
					}
				}
				else {
					return error(line, _T("#elseifdef �ɑ����P�ꂪ����܂���B"));
				}
			}
			else {
				return error(line, _T("���� #elseifdef �ɑΉ����� #if ������܂���B"));
			}
		}
		// #else
		else if (str_analize_check_str(p, m_text.end(), _T("#else"))) {

			if (if_state[if_depth] > 0) {

				if (if_state[if_depth] == IF_STATE_FALSE_MORE)	{
					continue;
				}
				else if (if_state[if_depth] == IF_STATE_TRUE)	{
					if_state[if_depth] = IF_STATE_FALSE_MORE;
					continue;
				}
				else {
					if_state[if_depth] = IF_STATE_TRUE;
					continue;
				}
			}
			else {
				return error(line, _T("���� #else �ɑΉ����� #if ������܂���B"));
			}
		}
		// #endif
		else if (str_analize_check_str(p, m_text.end(), _T("#endif"))) {

			if (if_state[if_depth] > 0) {
				--if_depth;
				continue;
			}
			else {
				return error(line, _T("���� #endif �ɑΉ����� #if ������܂���B"));
			}
		}
		// ���s
		else if (*p == _T('\n'))	{
			after += _T(' ');
			line++;
			p++;
		}
		// if false ���͂����܂�
		else if (if_state[if_depth] == IF_STATE_FALSE || if_state[if_depth] == IF_STATE_FALSE_MORE) {
			p++;
		}
		// # ������ΏI��
		else if (*p == _T('#'))	{
			break;
		}
		// ���̑��̕����͂��̂܂�
		else	{
			after += *p;
			p ++;
		}
	}

	// �Ō�̃X�y�[�X�Ȃǂ�؂�
	int not_space_pos = (int)after.find_last_not_of(_T(" \t"));
	if (not_space_pos != TSTR::npos)
		after.erase(not_space_pos + 1, after.size());

	// ����
	after_ret = after;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#property�i�u���O�j
// ================================================================
bool C_inc_file_analizer::ia_property(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	// �I��肩�� # ��������܂Ői��
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	
	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ���݂̈ʒu���L��
	TSTR::const_iterator name_start_pos = p;

	// ��؂�ȊO�������Ԑi��
	while (1)	{
		if (false);

		// ����
		else if (p == m_text.end() || *p == _T('#'))	{
			break;
		}
		// ���s
		else if (*p == _T('\n'))	{
			line ++;
			p ++;
		}
		// ���̑��̕����͂��̂܂�
		else	{
			p ++;
		}
	}

	// ���O���쐬
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// ����
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#command�i�u���O�j
// ================================================================
bool C_inc_file_analizer::ia_command(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	// �I��肩�� # ��������܂Ői��
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	
	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ���݂̈ʒu���L��
	TSTR::const_iterator name_start_pos = p;

	// ��؂�ȊO�������Ԑi��
	while (1)	{
		if (false);

		// ����
		else if (p == m_text.end() || *p == _T('#'))	{
			break;
		}
		// ���s
		else if (*p == _T('\n'))	{
			line ++;
			p ++;
		}
		// ���̑��̕����͂��̂܂�
		else	{
			p ++;
		}
	}

	// ���O���쐬
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// ����
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#property �̖��O
// ================================================================
bool C_inc_file_analizer::ia_property_name(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ���݂̈ʒu���L��
	TSTR::const_iterator name_start_pos = p;

	// ��؂�ȊO�������Ԑi��
	for (; !(p == m_text.end() || *p == _T(' ') || *p == _T(':') || *p == _T('\t') || *p == _T('\n')); ++p);

	// ���O���쐬
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// ����
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#command �̖��O
// ================================================================
bool C_inc_file_analizer::ia_command_name(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ���݂̈ʒu���L��
	TSTR::const_iterator name_start_pos = p;

	// ��؂�ȊO�������Ԑi��
	for (; !(p == m_text.end() || *p == _T(' ') || *p == _T('(') || *p == _T(':') || *p == _T('\t') || *p == _T('\n')); ++p);

	// ���O���쐬
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// ����
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#command �̈������X�g
// ================================================================
bool C_inc_file_analizer::ia_command_arg_list(TSTR::const_iterator& p_ret, int* line_ret, S_tnms_arg_list& arg_list_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	S_tnms_arg_list arg_list;

	// �Z�p���[�^���X�L�b�v
	if (!ia_skip_separator(p, &line))
		return true;	// �|�C���^��i�߂��ɐ����i�����Ȃ��j

	// '('
	if (*p++ != _T('('))
		return true;	// �|�C���^��i�߂��ɐ����i�����Ȃ��j

	// �������X�g
	bool def_value_exist = false;
	for (int i = 0; ; i++)	{

		// �������擾
		S_tnms_arg arg;
		if (!ia_command_arg(p, &line, arg))
			return error(line, _T("�������X�g�̈����̉�͂Ɏ��s���܂����B"));

		arg_list.arg_list.push_back(arg);

		// �f�t�H���g�����̔���
		if (def_value_exist)	{
			if (!arg.def_exist)	{
				return error(line, tostr(i + 1) + _T(" �ڂ̈����Ƀf�t�H���g�������K�v�ł��B"));
			}
		}

		if (arg.def_exist)
			def_value_exist = true;

		// �Z�p���[�^���X�L�b�v
		if (!ia_skip_separator(p, &line))
			return error(line, _T("�������X�g�� '(' �������Ă��܂���B"));

		// ',' �łȂ��Ȃ�I��
		if (*p != _T(','))
			break;

		// ���̕�����
		p++;
	}

	// �Z�p���[�^���X�L�b�v
	if (!ia_skip_separator(p, &line))
		return error(line, _T("�������X�g�� '(' �������Ă��܂���B"));

	// ')'
	if (*p++ != ')')
		return error(line, _T("�������X�g�� '(' �������Ă��܂���B"));

	// ����
	arg_list_ret = arg_list;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#command �̈���
// ================================================================
bool C_inc_file_analizer::ia_command_arg(TSTR::const_iterator& p_ret, int* line_ret, S_tnms_arg& arg_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	S_tnms_arg arg;

	// �����̌^�����
	if (!ia_form(p, line, line_ret, &arg.form))
		return false;

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// �f�t�H���g�����i���������j
	if (str_analize_check_moji(p, m_text.end(), _T('(')))	{

		// �Z�p���[�^���X�L�b�v
		ia_skip_separator(p, &line);

		// �������擾
		if (false);
		else if (arg.form == FM_INT)	{
			int num = 0;
			if (!str_analize_get_integer(p, m_text.end(), &num))
				return error(line, _T("int �^�̃f�t�H���g����"));

			arg.def_exist = true;
			arg.def_int = num;
		}
		else if (arg.form == FM_STR)	{
			TSTR str;
			if (!str_analize_get_dquote_str(p, m_text.end(), str, _T('\n')))
				return error(line, _T("str �^�̃f�t�H���g����"));

			arg.def_exist = true;
			arg.def_str = str;
		}

		// �Z�p���[�^���X�L�b�v
		ia_skip_separator(p, &line);

		// ')'
		if (!str_analize_check_moji(p, m_text.end(), _T(')')))
			return false;
	}

	// ����
	arg_ret = arg;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F#property �̌^
// ================================================================
bool C_inc_file_analizer::ia_declare_property_form(TSTR::const_iterator& p_ret, int* line_ret, int* form_ret, int* size_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	int form = FM_INT;		// �f�t�H���g�ł� int �^
	int size = 0;			// �f�t�H���g�ł̓T�C�Y�O

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ':'
	if (str_analize_check_moji(p, m_text.end(), _T(':')))	{
		int colon_line = line;

		// �^
		if (!ia_form(p, colon_line, &line, &form))
			return false;

		// �Z�p���[�^���X�L�b�v
		ia_skip_separator(p, &line);

		// '['
		if (str_analize_check_moji(p, m_text.end(), _T('[')))	{
			int kakko_line = line;

			// �Z�p���[�^���X�L�b�v
			ia_skip_separator(p, &line);

			// ����
			if (!str_analize_get_num(p, m_text.end(), &size))
				return error(kakko_line, _T("�z��̓Y�����������ł���܂���B"));

			// �Z�p���[�^���X�L�b�v
			ia_skip_separator(p, &line);

			// ']'
			if (!str_analize_check_moji(p, m_text.end(), ']'))
				return error(kakko_line, _T("�z��̓Y������ ] �ŕ����Ă��܂���B"));

			// �z��ɂȂ��͓̂���̌^�̂�
			if (form != FM_INTLIST && form != FM_STRLIST)
				return error(kakko_line, _T("intlist ����� strlist �ȊO�͔z��ɂȂ�܂���B"));
		}
	}

	// ����
	*form_ret = form;
	*size_ret = size;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F�錾�̌^
// ================================================================
bool C_inc_file_analizer::ia_declare_form(TSTR::const_iterator& p_ret, int* line_ret, int* form_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	int form = FM_INT;		// �f�t�H���g�ł� int �^

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// ':'
	if (*p == ':')	{
		p++;

		// �^
		if (!ia_form(p, line, &line, &form))
			return false;
	}

	// ����
	*form_ret = form;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc ��́F�^
// ================================================================
bool C_inc_file_analizer::ia_form(TSTR::const_iterator& p_ret, int error_line, int* line_ret, int* form_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	int form = FM_INT;		// �f�t�H���g�ł� int �^

	// �Z�p���[�^���X�L�b�v
	ia_skip_separator(p, &line);

	// �^�����擾
	TSTR form_name;
	str_analize_get_word(p, m_text.end(), form_name);
	form = get_form_code_by_name(form_name);
	if (form == -1)
		return error(error_line, _T("�s���Ȍ^�ł��B"));	// �I���W�i���̍s���ŃG���[���o��

	// ����
	*form_ret = form;
	*line_ret = line;
	p_ret = p;
	return true;
}


// ****************************************************************
// ������́F�u��
// ================================================================
bool C_inc_file_analizer::std_replace(TSTR& text, TSTR::const_iterator& p_ret, int* line_ret, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	BSP<S_tnms_ia_replace> p_default_replace;
	BSP<S_tnms_ia_replace> p_added_replace;
	BSP<S_tnms_ia_replace> p_replace;

	// �f�t�H���g�u���c���[����u������������
	C_replace_tree::search_replace(default_replace_tree, p, p_default_replace);

	// �ǉ��u���c���[����u������������
	if (!added_replace_tree.child_map.empty())
		C_replace_tree::search_replace(added_replace_tree, p, p_added_replace);

	// �u�����Ȃ������ꍇ�͂P�����i��
	if (!p_default_replace && !p_added_replace)	{
		++p;
		*line_ret = line;
		p_ret = p;
		return true;
	}
	// �u�����������ꍇ�͒u����I��
	else if (p_default_replace && p_added_replace)	{
		if (p_default_replace->name > p_added_replace->name)	{
			p_replace = p_default_replace;
		}
		else	{
			p_replace = p_added_replace;
		}
	}
	else if (p_default_replace)	{
		p_replace = p_default_replace;
	}
	else	{
		p_replace = p_added_replace;
	}

	// �u���J�n
	if (false);

	// #replace
	else if (p_replace->type == TNMS_IA_REPLACE_TYPE_REPLACE)	{
		int name_len = (int)p_replace->name.size();
		TSTR::const_iterator define_start_pos = p;
		int define_start_pos_int = (int)(define_start_pos - text.begin());

		// �u�����镶����̍Ō�Ɉړ�����
		p += name_len;
		// ��͌�̕�����ɒu��
		text.replace(define_start_pos, p, p_replace->after);
		// ��̓|�C���^�̍Đݒ�
		p = text.begin() + define_start_pos_int + p_replace->after.size();
	}

	// #define
	else if (p_replace->type == TNMS_IA_REPLACE_TYPE_DEFINE)	{
		int name_len = (int)p_replace->name.size();
		TSTR::const_iterator define_start_pos = p;
		int define_start_pos_int = (int)(define_start_pos - text.begin());

		// �u�����镶����̍Ō�Ɉړ�����
		p += name_len;
		// ��͌�̕�����ɒu��
		text.replace(define_start_pos, p, p_replace->after);
		// ��̓|�C���^�̍Đݒ�
		p = text.begin() + define_start_pos_int;
	}

	// #macro
	else if (p_replace->type == TNMS_IA_REPLACE_TYPE_MACRO)	{
		int name_len = (int)p_replace->name.size();

		// �}�N���̉��
		TSTR result_text;
		TSTR::const_iterator macro_start_pos = p;
		int macro_start_pos_int = (int)(macro_start_pos - text.begin());

		// �u�����镶����̖��O�����̍Ō�Ɉړ�����
		p += name_len;
		// �}�N������͂���
		if (!analize_macro(text, p, &line, p_replace.get(), default_replace_tree, added_replace_tree, result_text))
			return false;
		// ��͌�̕�����ɒu��
		text.replace(macro_start_pos, p, result_text);
		// ��̓|�C���^�̍Đݒ�
		p = text.begin() + macro_start_pos_int + result_text.size();
	}

	// ����
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// ������́F�}�N���̉��
// ================================================================
bool C_inc_file_analizer::analize_macro(TSTR& text, TSTR::const_iterator& p_ret, int* line_ret, S_tnms_ia_replace* p_macro, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	ARRAY<TSTR> arg_list_real;			// ���������X�g
	int kakko_cnt = 0;					// ���݂̊��ʂ̐�
	int arg_cnt = 0;					// �����̌�

	// ���ʂ�����ꍇ
	if (*p == _T('('))	{
		p++;

		TSTR::const_iterator arg_start_pos = p;	// ���݂̈����J�n�ʒu

		// ���������X�g���쐬
		while (1)	{
			if (false);
			else if (*p == _T('\0'))	{
				return error(line, _T("�}�N���̉�͒��Ƀt�@�C���̏I�[��������܂����B"));
			}
			//else if (*p == _T('\n'))	{
			//	return error(m_line, _T("�}�N���̉�͒��ɉ��s��������܂����B"));
			//}
			else if (*p == _T('\''))	{
				for (++p;; ++p)	{
					if (*p == _T('\''))	{ ++p;	break; }	// �I���� ' �����������̂Ŕ�����
					if (*p == _T('\\'))	{ ++p; }	// \ �����������̂łP������������
				}
			}
			else if (*p == _T('"'))	{
				for (++p;; ++p)	{
					if (*p == _T('"'))	{ ++p;	break; }	// �I���� " �����������̂Ŕ�����
					if (*p == _T('\\'))	{ ++p; }	// \ �����������̂łP������������
				}
			}
			else if (*p == _T('('))	{
				kakko_cnt++;
				++p;	// ( �̎��̕����܂Ői��
			}
			else if (*p == _T(','))	{
				if (kakko_cnt == 0)	{
					if (arg_start_pos == p)	{	// ��������Ȃ�G���[�I
						return error(line, _T("�}�N����") + tostr(arg_cnt) + _T("�ڂ̈�������ł��B"));
					}
					arg_list_real.push_back(TSTR(arg_start_pos, p));
					arg_start_pos = p + 1;
					++p;	// , �̎��̕����܂Ői��
				}
				else	{
					++p;	// , �̎��̕����܂Ői��
				}
			}
			else if (*p == _T(')'))	{
				if (kakko_cnt == 0)	{

					// �P�ڂ̈�������Ȃ�����Ȃ�
					if (arg_start_pos == p && arg_list_real.size() == 0)	{
						++p;	// ) �̎��̕����܂Ői��
					}
					// �Q�ڈȍ~�̈�������Ȃ�G���[
					else if (arg_start_pos == p)	{
						return error(line, _T("�}�N����") + tostr(arg_cnt) + _T("�ڂ̈�������ł��B"));
					}
					// ����������Έ������X�^�b�N
					else	{
						arg_list_real.push_back(TSTR(arg_start_pos, p));
						++p;	// ) �̎��̕����܂Ői��
					}
					break;
				}
				else	{
					kakko_cnt--;
					++p;	// ) �̎��̕����܂Ői��
				}
			}
			else	{
				++p;
			}
		}
	}

	// �����̌��̃`�F�b�N
	if (p_macro->arg_list.size() == 0 && arg_list_real.size() > 0)
		return error(line, _T("�����̂���Ȃ��}�N���ɂ͊��ʂ͕K�v����܂���B"));
	if (p_macro->arg_list.size() < arg_list_real.size())
		return error(line, _T("�}�N���̈������������܂��B"));

	// �����W�J
	if (!analize_macro_replace(text, &line, p_macro->after, p_macro->arg_list, arg_list_real, default_replace_tree, added_replace_tree, result_text))
		return false;

	// ����
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// ������́F�}�N���̒u��
// ================================================================
bool C_inc_file_analizer::analize_macro_replace(TSTR& text, int* line_ret, CTSTR& src_text, ARRAY<S_tnms_ia_macro_arg>& tmp_arg_list, ARRAY<TSTR>& real_arg_list, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text)
{
	int line = *line_ret;
	result_text = src_text;

	// �����̒u�����X�g�����
	ARRAY< BSP<S_tnms_ia_replace> > arg_replace_list;
	for (int a = 0; a < (int)tmp_arg_list.size(); a++)	{
		BSP<S_tnms_ia_replace> replace(new S_tnms_ia_replace);
		replace->type = TNMS_IA_REPLACE_TYPE_REPLACE;
		replace->name = tmp_arg_list[a].name;

		// ���������Ȃ��ꍇ�f�t�H���g������^����
		if (a >= (int)real_arg_list.size())	{
			if (tmp_arg_list[a].def_value.empty())
				return error(line, _T("�}�N���̈���������܂���B"));
			replace->after = tmp_arg_list[a].def_value;
		}
		else	{
			replace->after = real_arg_list[a];
		}

		// ���X�g�ɉ�����
		arg_replace_list.push_back(replace);
	}

	// ���������ɒu������
	// �f�t�H���g����������̂ŁA�������̊m�肵�������ł��Ȃ��Ƃ����Ȃ��̂ł��B
	for (ARRAY< BSP<S_tnms_ia_replace> >::iterator itr = arg_replace_list.begin(); itr != arg_replace_list.end(); ++itr)	{

		// �ԕ��̑}��
		for (int i = 0; i < 256; i++)
			(*itr)->after.push_back(_T('\0'));

		// �u��
		for (TSTR::iterator p = (*itr)->after.begin(); *p != _T('\0');)
			std_replace((*itr)->after, p, &line, default_replace_tree, added_replace_tree);

		// �ԕ����폜
		(*itr)->after.erase((*itr)->after.end() - 256, (*itr)->after.end());
	}

	// �����̒u�����X�g����ёւ���
	std::sort(arg_replace_list.begin(), arg_replace_list.end(), G_tnms_ia_replace_cmp_func);
	std::reverse(arg_replace_list.begin(), arg_replace_list.end());

	// �����̒u���c���[�����
	S_replace_node arg_replace_tree;
	C_replace_tree::create_replace_tree(arg_replace_tree, arg_replace_list);

	// �ԕ��̑}��
	for (int z = 0; z < 256; z++)
		result_text.push_back(_T('\0'));

	// �}�N�����̃}�N���̒u�����s��
	for (TSTR::iterator p = result_text.begin(); *p != _T('\0');)	{
		if (!std_replace(result_text, p, &line, default_replace_tree, arg_replace_tree))
			return false;
	}

	// �ԕ����폜
	result_text.erase(result_text.end() - 256, result_text.end());

	// ����
	*line_ret = line;
	return true;
}



// ****************************************************************
// inc ��́F���C���F�X�e�b�v�Q
// ================================================================
bool C_inc_file_analizer::ia_main_step_2()
{
	// �p�����[�^�̏�����
	m_error_line = -1;
	m_error_str.clear();
	int line = 1;

	// �v���p�e�B
	int property_cnt = (int)m_piad2->property_temp_list.size();
	for (int i = 0; i < property_cnt; i++)	{
		m_text = m_piad2->property_temp_list[i];
		line = m_piad2->property_line_list[i];

		// �܂��̓v���p�e�B�ɑ΂��Ēu�����s��
		C_tnms_character_analizer ca;
		TSTR out_text;
		if (!ca.analize_line(m_text, m_piad, m_text))
			return error(line, ca.get_error_str());

		// �u����̃v���p�e�B�����
		TSTR::iterator p = m_text.begin();
		BSP<S_tnms_ia_declare> declare(new S_tnms_ia_declare);
		declare->Property = BSP<S_tnms_ia_property>(new S_tnms_ia_property);
		if (!ia_property_name(p, &line, declare->Property->name))										return false;
		if (!ia_declare_property_form(p, &line, &declare->Property->form, &declare->Property->size))	return false;
		TSTR declare_name = declare->Property->name;		// �d������p�̖��O

		// �d������
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(line, declare_name + _T(" ����d�ɐ錾����Ă��܂��B"));
		m_piad->name_set.insert(declare_name);

		// �v���p�e�B�� void �^�Ȃ�G���[
		if (declare->Property->form == FM_VOID)
			return error(line, _T("void �^�̃v���p�e�B�͐錾�ł��܂���B"));

		// �v���p�e�B�� id ������
		declare->Property->id = m_piad->property_cnt ++;

		// �v���p�e�B���X�g�ɒǉ�
		m_piad->property_list.push_back(declare->Property);

		// ���[�U�v���p�e�B��ǉ�
		S_tnms_element element;
		element.type = TNMS_ELEMENT_TYPE_PROPERTY;
		element.code = create_elm_code(ELM_OWNER_USER_PROP, 0, declare->Property->id);
		element.name = declare->Property->name;
		element.form = declare->Property->form;

		// �G�������g��ǉ�
		S_tnms_form* form_global = m_piad->form_table.get_form_by_code(m_parent_form);
		form_global->add_element(&element);

		// �O���[�o���̏ꍇ�͌����L�����Ă���
		if (m_parent_form == FM_GLOBAL)
			m_piad->inc_property_cnt ++;
	}

	// �R�}���h
	int command_cnt = (int)m_piad2->command_temp_list.size();
	for (int i = 0; i < command_cnt; i++)	{
		m_text = m_piad2->command_temp_list[i];
		line = m_piad2->command_line_list[i];
		int org_line = line;

		// �܂��̓R�}���h�ɑ΂��Ēu�����s��
		C_tnms_character_analizer ca;
		TSTR out_text;
		if (!ca.analize_line(m_text, m_piad, m_text))
			return error(org_line, ca.get_error_str());

		// �u����̃v���p�e�B�����
		TSTR::iterator p = m_text.begin();
		BSP<S_tnms_ia_declare> declare(new S_tnms_ia_declare);
		declare->command = BSP<S_tnms_ia_command>(new S_tnms_ia_command);
		if (!ia_command_name(p, &line, declare->command->name))				return false;
		if (!ia_command_arg_list(p, &line, declare->command->arg_list))		return false;
		if (!ia_declare_form(p, &line, &declare->command->form))			return false;
		TSTR declare_name = declare->command->name;		// �d������p�̖��O

		// �d������
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(org_line, declare_name + _T(" ����d�ɐ錾����Ă��܂��B"));
		m_piad->name_set.insert(declare_name);

		// �R�}���h�� id ������
		declare->command->id = m_piad->command_cnt ++;
		declare->command->is_defined = false;

		// �R�}���h���X�g�ɒǉ�
		m_piad->command_list.push_back(declare->command);

		// ���[�U�R�}���h��ǉ�
		S_tnms_element element;
		element.type = TNMS_ELEMENT_TYPE_COMMAND;
		element.code = create_elm_code(ELM_OWNER_USER_CMD, 0, declare->command->id);
		element.name = declare->command->name;
		element.form = declare->command->form;
		element.add_arg_list(0, declare->command->arg_list);

		// �G�������g��ǉ�
		S_tnms_form* form_global = m_piad->form_table.get_form_by_code(m_parent_form);
		form_global->add_element(&element);

		// �O���[�o���̏ꍇ�͌����L�����Ă���
		if (m_parent_form == FM_GLOBAL)
			m_piad->inc_command_cnt ++;
	}

	return true;
}

// ****************************************************************
// inc ��́F�G���[���Z�b�g
// ================================================================
bool C_inc_file_analizer::error(int line, CTSTR& str)
{
	if (m_error_str.empty())	{
		m_error_line = line;
		m_error_str = str;
	}

	return false;
}

// ****************************************************************
// inc ��́F�G���[���擾
// ================================================================
TSTR C_inc_file_analizer::get_error_str()
{
	return m_error_str;
}

int C_inc_file_analizer::get_error_line()
{
	return m_error_line;
}



