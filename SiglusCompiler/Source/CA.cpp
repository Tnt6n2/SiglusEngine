#include	"pch.h"
#include	"CA.h"

// ****************************************************************
// �������r
// ================================================================
bool check_str(TSTR::const_iterator& p_ret, CTSTR& str)
{
	TSTR::const_iterator p = p_ret;

	for (TSTR::const_iterator q = str.begin(); q != str.end(); ++p, ++q)	{
		if (*p != *q)
			return false;
	}

	p_ret = p;

	return true;
}

bool check_word(TSTR::const_iterator& p_ret, TSTR& word_ret)
{
	TSTR::const_iterator p = p_ret;

	while (*p == ' ' || *p == '\t')
		p++;

	TSTR::const_iterator word_start_p = p;

	if (*p == '_' || *p == '@' || is_alpha(*p) || is_zenkaku(*p)) {
		p++;

		while (*p == '_' || *p == '@' || is_alpha(*p) || is_num(*p) || is_zenkaku(*p))
			p++;

		word_ret = TSTR(word_start_p, p);
		p_ret = p;

		return true;

	}
	else {

		return false;
	}
}

// ****************************************************************
// ������� - Character Analysis -
// ================================================================

bool C_tnms_character_analizer::analize_file_1(CTSTR& in_text_, TSTR& out_text)
{
	TSTR in_text = in_text_;

	// �ԕ��̑}��
	for (int i = 0; i < 256; i++)
		in_text.push_back(_T('\0'));

	// �o�͗p�o�b�t�@�̊m��
	out_text.reserve(in_text.size() * 5);		// �h�m�e�L�X�g�̂T�{���U�[�u

	// ��͗p�X�e�[�g
	enum STATE
	{
		STATE_NONE,
		STATE_SINGLE_QUOTE,			// �V���O���R�[�e�[�V����
		STATE_SINGLE_QUOTE_ESCAPE,	// �V���O���R�[�e�[�V�����G�X�P�[�v
		STATE_SINGLE_QUOTE_END,		// �V���O���R�[�e�[�V�����I��
		STATE_DOUBLE_QUOTE,			// �_�u���R�[�e�[�V����
		STATE_DOUBLE_QUOTE_ESCAPE,	// �_�u���R�[�e�[�V�����G�X�P�[�v
		STATE_COMMENT_LINE,			// �s�R�����g
		STATE_COMMENT_BLOCK,		// �u���b�N�R�����g
	};

	// ��͏���
	m_line = 1;							// �s�ԍ��i�P�s�ڂ���X�^�[�g�j
	int block_comment_start_line = 1;	// �u���b�N�R�����g�̊J�n�s
	STATE state = STATE_NONE;			// ���݂̃X�e�[�g

	// ��͊J�n�I
	for (TSTR::const_iterator p = in_text.begin(); *p != _T('\0'); ++p)	{

		TCHAR moji = *p;	// �o�͕���

		// ���s�̏ꍇ�̂ݓ��ꏈ��
		if (*p == _T('\n'))	{
			// �V���O���R�[�e�[�V�������Ȃ�G���[
			if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE || state == STATE_SINGLE_QUOTE_END)
				return error(m_line, _T("�V���O���R�[�e�[�V�������͉��s�ł��܂���B"));
			// �_�u���R�[�e�[�V�������Ȃ�G���[
			if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
				return error(m_line, _T("�_�u���R�[�e�[�V�������͉��s�ł��܂���B"));
			// �s�R�����g���Ȃ�s�R�����g�I��
			if (state == STATE_COMMENT_LINE)
				state = STATE_NONE;
			// �s���C���N�������g
			m_line++;
		}
		// �V���O���R�[�e�[�V������
		else if (state == STATE_SINGLE_QUOTE)	{
			// �G�X�P�[�v
			if (*p == _T('\\'))	{
				state = STATE_SINGLE_QUOTE_ESCAPE;
			}
			// �V���O���R�[�e�[�V����
			else if (*p == _T('\''))	{
				return error(m_line, _T("' �͕K���P�������͂��Ă��������B"));
			}
			else	{
				state = STATE_SINGLE_QUOTE_END;
			}
		}
		// �V���O���R�[�e�[�V�����G�X�P�[�v��
		else if (state == STATE_SINGLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('\''))	{
				state = STATE_SINGLE_QUOTE_END;
			}
			else if (*p == _T('n'))	{
				state = STATE_SINGLE_QUOTE_END;
			}
			else	{
				return error(m_line, _T("�s���ȃG�X�P�[�v�i\\�j�ł��B'\\' ���g���ɂ� '\\\\' �Ə����Ă��������B"));
			}
		}
		// �V���O���R�[�e�[�V�����I��
		else if (state == STATE_SINGLE_QUOTE_END)	{
			// �V���O���R�[�e�[�V�����I��
			if (*p == _T('\''))	{
				state = STATE_NONE;
			}
			else	{
				return error(m_line, _T("' �������Ă��Ȃ��A�܂��͂Q�����ȏ�̕������͂�ł��܂��B"));
			}
		}
		// �_�u���R�[�e�[�V������
		else if (state == STATE_DOUBLE_QUOTE)	{
			// �G�X�P�[�v
			if (*p == _T('\\'))	{
				state = STATE_DOUBLE_QUOTE_ESCAPE;
			}
			// �_�u���R�[�e�[�V�����I��
			else if (*p == _T('"'))	{
				state = STATE_NONE;
			}
		}
		// �_�u���R�[�e�[�V�����G�X�P�[�v��
		else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('"'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			else if (*p == _T('n'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			else	{
				return error(m_line, _T("�s���ȃG�X�P�[�v�i\\�j�ł��B'\\' ���g���ɂ� '\\\\' �Ə����Ă��������B"));
			}
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
			// �V���O���R�[�e�[�V�����J�n
			else if (*p == _T('\''))
				state = STATE_SINGLE_QUOTE;
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
				block_comment_start_line = m_line;
				state = STATE_COMMENT_BLOCK;
				continue;
			}
			// �A���t�@�x�b�g�啶�����������ɕϊ�
			else if (_T('A') <= *p && *p <= _T('Z'))
				moji += _T('a') - _T('A');
		}

		// �o��
		out_text.push_back(moji);
	}

	// �I�����̃X�e�[�g�ɂ���ăG���[
	if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE || state == STATE_SINGLE_QUOTE_END)
		return error(m_line, _T("�����Ă��Ȃ� ' ������܂��B"));
	if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
		return error(m_line, _T("�����Ă��Ȃ� \" ������܂��B"));
	if (state == STATE_COMMENT_BLOCK)
		return error(block_comment_start_line, _T("�����Ă��Ȃ� /* ������܂��B"));

	return true;
}

bool C_tnms_character_analizer::analize_file_2(CTSTR& in_text_, TSTR& out_text, TSTR& inc_text)
{
	TSTR in_text = in_text_;

	// �ԕ��̑}��
	for (int i = 0; i < 256; i++)
		in_text.push_back(_T('\0'));

	// �o�͗p�o�b�t�@�̊m��
	out_text.reserve(in_text.size() * 5);		// �h�m�e�L�X�g�̂T�{���U�[�u
	inc_text.reserve(in_text.size() * 5);		// �h�m�e�L�X�g�̂T�{���U�[�u

	// ��͗p�X�e�[�g
	enum STATE
	{
		STATE_NONE,
		STATE_SINGLE_QUOTE,			// �V���O���R�[�e�[�V����
		STATE_SINGLE_QUOTE_ESCAPE,	// �V���O���R�[�e�[�V�����G�X�P�[�v
		STATE_SINGLE_QUOTE_END,		// �V���O���R�[�e�[�V�����I��
		STATE_DOUBLE_QUOTE,			// �_�u���R�[�e�[�V����
		STATE_DOUBLE_QUOTE_ESCAPE,	// �_�u���R�[�e�[�V�����G�X�P�[�v
	};
	enum IF_STATE
	{
		IF_STATE_NONE = 0,
		IF_STATE_TRUE = 1,
		IF_STATE_FALSE = 2,
		IF_STATE_FALSE_MORE = 3,
	};

	// ��͏���
	m_line = 1;										// �s�ԍ��i�P�s�ڂ���X�^�[�g�j
	STATE state = STATE_NONE;						// ���݂̃X�e�[�g
	IF_STATE if_state[16] = { IF_STATE_NONE };		// ���݂� if �X�e�[�g
	int if_depth = 0;								// ���݂� if �̐[��
	bool inc_start = false;							// inc_start

	// ��͊J�n�I
	for (TSTR::const_iterator p = in_text.begin(); *p != _T('\0'); )	{

		// ���s�̏ꍇ�̂ݓ��ꏈ��
		if (*p == _T('\n'))	{
			// �V���O���R�[�e�[�V�������Ȃ�G���[
			if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE || state == STATE_SINGLE_QUOTE_END)
				return error(m_line, _T("�V���O���R�[�e�[�V�������͉��s�ł��܂���B"));
			// �_�u���R�[�e�[�V�������Ȃ�G���[
			if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
				return error(m_line, _T("�_�u���R�[�e�[�V�������͉��s�ł��܂���B"));
			// �s���C���N�������g
			m_line++;
		}
		// �V���O���R�[�e�[�V������
		else if (state == STATE_SINGLE_QUOTE)	{
			// �G�X�P�[�v
			if (*p == _T('\\'))	{
				state = STATE_SINGLE_QUOTE_ESCAPE;
			}
			// �V���O���R�[�e�[�V����
			else if (*p == _T('\''))	{
				return error(m_line, _T("' �͕K���P�������͂��Ă��������B"));
			}
			else	{
				state = STATE_SINGLE_QUOTE_END;
			}
		}
		// �V���O���R�[�e�[�V�����G�X�P�[�v��
		else if (state == STATE_SINGLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('\''))	{
				state = STATE_SINGLE_QUOTE_END;
			}
			else if (*p == _T('n'))	{
				state = STATE_SINGLE_QUOTE_END;
			}
			else	{
				return error(m_line, _T("�s���ȃG�X�P�[�v�i\\�j�ł��B'\\' ���g���ɂ� '\\\\' �Ə����Ă��������B"));
			}
		}
		// �V���O���R�[�e�[�V�����I��
		else if (state == STATE_SINGLE_QUOTE_END)	{
			// �V���O���R�[�e�[�V�����I��
			if (*p == _T('\''))	{
				state = STATE_NONE;
			}
			else	{
				return error(m_line, _T("' �������Ă��Ȃ��A�܂��͂Q�����ȏ�̕������͂�ł��܂��B"));
			}
		}
		// �_�u���R�[�e�[�V������
		else if (state == STATE_DOUBLE_QUOTE)	{
			// �G�X�P�[�v
			if (*p == _T('\\'))	{
				state = STATE_DOUBLE_QUOTE_ESCAPE;
			}
			// �_�u���R�[�e�[�V�����I��
			else if (*p == _T('"'))	{
				state = STATE_NONE;
			}
		}
		// �_�u���R�[�e�[�V�����G�X�P�[�v��
		else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('"'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			else if (*p == _T('n'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			else	{
				return error(m_line, _T("�s���ȃG�X�P�[�v�i\\�j�ł��B'\\' ���g���ɂ� '\\\\' �Ə����Ă��������B"));
			}
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

			// #ifdef
			else if (check_str(p, _T("#ifdef"))) {

				TSTR word;
				if (check_word(p, word)) {

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
					return error(m_line, _T("#ifdef �ɑ����P�ꂪ����܂���B"));
				}
			}
			// #elseifdef
			else if (check_str(p, _T("#elseifdef"))) {

				if (if_state[if_depth] > 0) {

					TSTR word;
					if (check_word(p, word)) {

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
						return error(m_line, _T("#elseifdef �ɑ����P�ꂪ����܂���B"));
					}
				}
				else {
					return error(m_line, _T("���� #elseifdef �ɑΉ����� #if ������܂���B"));
				}
			}
			// #else
			else if (check_str(p, _T("#else"))) {

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
					return error(m_line, _T("���� #else �ɑΉ����� #if ������܂���B"));
				}
			}
			// #endif
			else if (check_str(p, _T("#endif"))) {

				if (if_state[if_depth] > 0) {
					--if_depth;
					continue;
				}
				else {
					return error(m_line, _T("���� #endif �ɑΉ����� #if ������܂���B"));
				}
			}
			// #inc_start
			else if (check_str(p, _T("#inc_start"))) {
				inc_start = true;
				continue;
			}
			// #inc_end
			else if (check_str(p, _T("#inc_end"))) {

				if (inc_start) {
					inc_start = false;
					continue;
				}
				else {
					return error(m_line, _T("���� #inc_end �ɑΉ����� #inc_start ������܂���B"));
				}
			}
		}

		// �o��
		if (*p == '\n') {
			if (inc_start)	{ inc_text.push_back(*p); }
			out_text.push_back(*p);		// ���s�͕K���o��
		}
		else if (if_state[if_depth] == IF_STATE_NONE || if_state[if_depth] == IF_STATE_TRUE) {
			if (inc_start)	{ inc_text.push_back(*p); }
			else			{ out_text.push_back(*p); }
		}

		p++;
	}

	// �I�����̃X�e�[�g�ɂ���ăG���[
	if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE || state == STATE_SINGLE_QUOTE_END)
		return error(m_line, _T("�����Ă��Ȃ� ' ������܂��B"));
	if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
		return error(m_line, _T("�����Ă��Ȃ� \" ������܂��B"));
	if (inc_start)
		return error(m_line, _T("�����Ă��Ȃ� #inc_start ������܂��B"));
	if (if_depth > 0)
		return error(m_line, _T("�����Ă��Ȃ� #ifdef ������܂��B"));

	return true;
}


bool C_tnms_character_analizer::analize_file(CTSTR& in_text_, S_tnms_ia_data* piad, S_tnms_ca_data* pcad)
{
	TSTR in_text = in_text_;
	TSTR out_text;
	TSTR inc_text;

	// ������
	m_piad = piad;

	// �X�e�b�v�P�F�R�����g�J�b�g�A��������
	if (!analize_file_1(in_text, out_text))
		return false;

	// �e�L�X�g�̓���ւ�
	in_text = out_text;
	out_text.clear();

	// �X�e�b�v�Q�F�v���v���Z�b�T����
	if (!analize_file_2(in_text, out_text, inc_text))
		return false;

	// �e�L�X�g�̓���ւ�
	in_text = out_text;
	out_text.clear();

	// �ԕ��̑}��
	for (int z = 0; z < 256; z++)
		in_text.push_back(_T('\0'));

	// �o���オ���� inc_text �� inc analizer �ɒʂ�
	C_inc_file_analizer ia;
	S_tnms_ia_data_2 ia_data_2;
	if (!ia.analize_step_1(inc_text, FM_SCENE, piad, &ia_data_2))
		return error(ia.get_error_line(), _T("inc: ") + ia.get_error_str());
	if (!ia.analize_step_2(piad, &ia_data_2))
		return error(ia.get_error_line(), _T("inc: ") + ia.get_error_str());

	// inc �t�@�C���ɂ��u��
	m_line = 1;
	int rest_length_min = (int)in_text.size();
	int loop_replace_check_cnt = 0;
	for (TSTR::iterator p = in_text.begin(); *p != _T('\0');)	{

		// ���s
		if (*p == _T('\n'))	{
			m_line ++;
			p ++;
		}
		// �ʏ�̕���
		else	{
			// �R�}���h�ɂ��u��
			if (!std_replace(in_text, p, m_piad->replace_tree, S_replace_node()))
				return false;
		}

		// �c�蒷�������߂�i�u���Ȃǂɂ��c�蒷���͐L�т邱�Ƃ�����j
		int rest_length = in_text.end() - p;

		// ���������[�v�`�F�b�N
		if (rest_length >= rest_length_min)	{
			loop_replace_check_cnt ++;
			if (loop_replace_check_cnt > 10000)	{
				return error(m_line, _T("inc �t�@�C���ɂ��u�����ɖ������[�v���������܂����B"));
			}
		}
		else	{
			rest_length_min = rest_length;
			loop_replace_check_cnt = 0;
		}
	}

	// ���ʂ��쐬����
	pcad->scn_text = in_text;

	return true;
}

// ****************************************************************
// ������́F�u��
// ================================================================
bool C_tnms_character_analizer::std_replace(TSTR& text, TSTR::iterator& p, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree)
{
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
		TSTR::iterator define_start_pos = p;
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
		TSTR::iterator define_start_pos = p;
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
		TSTR::iterator macro_start_pos = p;
		int macro_start_pos_int = (int)(macro_start_pos - text.begin());

		// �u�����镶����̖��O�����̍Ō�Ɉړ�����
		p += name_len;
		// �}�N������͂���
		if (!analize_macro(p, p_replace.get(), default_replace_tree, added_replace_tree, result_text))
			return false;
		// ��͌�̕�����ɒu��
		text.replace(macro_start_pos, p, result_text);
		// ��̓|�C���^�̍Đݒ�
		p = text.begin() + macro_start_pos_int + result_text.size();
	}

	return true;
}

// ****************************************************************
// ������́F�}�N���̉��
// ================================================================
bool C_tnms_character_analizer::analize_macro(TSTR::iterator& p, S_tnms_ia_replace* p_macro, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text)
{
	ARRAY<TSTR> arg_list_real;			// ���������X�g
	int kakko_cnt = 0;					// ���݂̊��ʂ̐�
	int arg_cnt = 0;					// �����̌�

	// ���ʂ�����ꍇ
	if (*p == _T('('))	{
		p++;

		TSTR::iterator arg_start_pos = p;	// ���݂̈����J�n�ʒu

		// ���������X�g���쐬
		while (1)	{
			if (false);
			else if (*p == _T('\0'))	{
				return error(m_line, _T("�}�N���̉�͒��Ƀt�@�C���̏I�[��������܂����B"));
			}
			//else if (*p == _T('\n'))	{
			//	return error(m_line, _T("�}�N���̉�͒��ɉ��s��������܂����B"));
			//}
			else if (*p == _T('\''))	{
				for (++p; ; ++p)	{
					if (*p == _T('\''))	{	++p;	break;	}	// �I���� ' �����������̂Ŕ�����
					if (*p == _T('\\'))	{	++p;			}	// \ �����������̂łP������������
				}
			}
			else if (*p == _T('"'))	{
				for (++p; ; ++p)	{
					if (*p == _T('"'))	{	++p;	break;	}	// �I���� " �����������̂Ŕ�����
					if (*p == _T('\\'))	{	++p;			}	// \ �����������̂łP������������
				}
			}
			else if (*p == _T('('))	{
				kakko_cnt ++;
				++p;	// ( �̎��̕����܂Ői��
			}
			else if (*p == _T(','))	{
				if (kakko_cnt == 0)	{
					if (arg_start_pos == p)	{	// ��������Ȃ�G���[�I
						return error(m_line, _T("�}�N����") + tostr(arg_cnt) + _T("�ڂ̈�������ł��B"));
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
						return error(m_line, _T("�}�N����") + tostr(arg_cnt) + _T("�ڂ̈�������ł��B"));
					}
					// ����������Έ������X�^�b�N
					else	{
						arg_list_real.push_back(TSTR(arg_start_pos, p));
						++p;	// ) �̎��̕����܂Ői��
					}
					break;
				}
				else	{
					kakko_cnt --;
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
		return error(m_line, _T("�����̂���Ȃ��}�N���ɂ͊��ʂ͕K�v����܂���B"));
	if (p_macro->arg_list.size() < arg_list_real.size())
		return error(m_line, _T("�}�N���̈������������܂��B"));

	// �����W�J
	if (!analize_macro_replace(p_macro->after, p_macro->arg_list, arg_list_real, default_replace_tree, added_replace_tree, result_text))
		return false;

	return true;
}

// ****************************************************************
// ������́F�}�N���̒u��
// ================================================================
bool C_tnms_character_analizer::analize_macro_replace(CTSTR& src_text, ARRAY<S_tnms_ia_macro_arg>& tmp_arg_list, ARRAY<TSTR>& real_arg_list, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text)
{
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
				return error(m_line, _T("�}�N���̈���������܂���B"));
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
			std_replace((*itr)->after, p, default_replace_tree, added_replace_tree);

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
		if (!std_replace(result_text, p, default_replace_tree, arg_replace_tree))
			return false;
	}

	// �ԕ����폜
	result_text.erase(result_text.end() - 256, result_text.end());

	return true;
}

// ****************************************************************
// ������́F�P�s�����
// ================================================================
bool C_tnms_character_analizer::analize_line(CTSTR& in_text_, S_tnms_ia_data* piad, TSTR& out_text)
{
	TSTR in_text = in_text_;

	// ���̃R�s�[
	m_piad = piad;

	// �ԕ��̑}��
	for (int i = 0; i < 1; i++)
		in_text.push_back(_T('\0'));

	// inc �t�@�C���ɂ��u��
	m_line = 1;
	int loop_replace_check_cnt = 0;
	int rest_length_min = (int)in_text.size();
	for (TSTR::iterator p = in_text.begin(); *p != _T('\0');)	{
		TSTR::iterator last_p = p;

		// ���s
		if (*p == _T('\n'))	{
			m_line ++;
			p ++;
		}
		// �ʏ�̕���
		else	{
			// �R�}���h�ɂ��u��
			if (!std_replace(in_text, p, m_piad->replace_tree, S_replace_node()))
				return false;
		}

		// �c�蒷�������߂�i�u���Ȃǂɂ��c�蒷���͐L�т邱�Ƃ�����j
		int rest_length = in_text.end() - p;

		// ���������[�v�`�F�b�N
		if (rest_length >= rest_length_min)	{
			loop_replace_check_cnt ++;
			if (loop_replace_check_cnt > 10000)	{
				return error(m_line, _T("inc �t�@�C���ɂ��u�����ɖ������[�v���������܂����B"));
			}
		}
		else	{
			rest_length_min = rest_length;
			loop_replace_check_cnt = 0;
		}
	}

	// �ԕ����폜
	in_text.erase(in_text.end() - 1, in_text.end());

	// ���ʂ��쐬����
	out_text = in_text;

	return true;
}

// ****************************************************************
// ������́F�G���[�̃Z�b�g
// ================================================================
bool C_tnms_character_analizer::error(int line, CTSTR& str)
{
	error_line = line;
	error_str = str;

	return false;
}
