#include	"pch.h"
#include	"LA.h"

// ****************************************************************
// �����́F�A�g���e�L�X�g�̎擾
// ================================================================
TSTR C_tnms_lexcial_analizer::get_atom_text(S_tnms_atom atom, S_tnms_la_data* plad)
{
	switch (atom.type)	{
		case TNMS_LA_ATOM_ASSIGN:			return _T("=");
		case TNMS_LA_ATOM_PLUS:				return _T("+");
		case TNMS_LA_ATOM_MINUS:			return _T("-");
		case TNMS_LA_ATOM_MULTIPLE:			return _T("*");
		case TNMS_LA_ATOM_DIVIDE:			return _T("/");
		case TNMS_LA_ATOM_PERCENT:			return _T("%");
		case TNMS_LA_ATOM_PLUS_ASSIGN:		return _T("+=");
		case TNMS_LA_ATOM_MINUS_ASSIGN:		return _T("-=");
		case TNMS_LA_ATOM_MULTIPLE_ASSIGN:	return _T("*=");
		case TNMS_LA_ATOM_DIVIDE_ASSIGN:	return _T("/=");
		case TNMS_LA_ATOM_PERCENT_ASSIGN:	return _T("%=");
		case TNMS_LA_ATOM_EQUAL:			return _T("==");
		case TNMS_LA_ATOM_NOT_EQUAL:		return _T("!=");
		case TNMS_LA_ATOM_GREATER:			return _T(">" );
		case TNMS_LA_ATOM_GREATER_EQUAL:	return _T(">=");
		case TNMS_LA_ATOM_LESS:				return _T("<" );
		case TNMS_LA_ATOM_LESS_EQUAL:		return _T("<=");
		case TNMS_LA_ATOM_LOGICAL_AND:		return _T("&&");
		case TNMS_LA_ATOM_LOGICAL_OR:		return _T("||");
		case TNMS_LA_ATOM_DOT:				return _T(".");
		case TNMS_LA_ATOM_COMMA:			return _T(",");
		case TNMS_LA_ATOM_COLON:			return _T(":");
		case TNMS_LA_ATOM_OPEN_PAREN:		return _T("(");
		case TNMS_LA_ATOM_CLOSE_PAREN:		return _T(")");
		case TNMS_LA_ATOM_OPEN_BRACKET:		return _T("[");
		case TNMS_LA_ATOM_CLOSE_BRACKET:	return _T("]");
		case TNMS_LA_ATOM_OPEN_BRACE:		return _T("{");
		case TNMS_LA_ATOM_CLOSE_BRACE:		return _T("}");
		case TNMS_LA_ATOM_OPEN_SUMI:		return _T("�y");
		case TNMS_LA_ATOM_CLOSE_SUMI:		return _T("�z");
		case TNMS_LA_ATOM_VAL_INT:			return tostr(atom.opt);
		case TNMS_LA_ATOM_VAL_STR:			return plad->str_list[atom.opt];
		case TNMS_LA_ATOM_LABEL:			return _T("#") + plad->label_list[atom.opt].name;
		case TNMS_LA_ATOM_Z_LABEL:			return _T("#z") + tostr(atom.opt);
		case TNMS_LA_ATOM_COMMAND:			return _T("command");
		case TNMS_LA_ATOM_PROPERTY:			return _T("property");
		case TNMS_LA_ATOM_GOTO:				return _T("goto");
		case TNMS_LA_ATOM_GOSUB:			return _T("gosub");
		case TNMS_LA_ATOM_GOSUBSTR:			return _T("gosubstr");
		case TNMS_LA_ATOM_RETURN:			return _T("return");
		case TNMS_LA_ATOM_IF:				return _T("if");
		case TNMS_LA_ATOM_ELSEIF:			return _T("elseif");
		case TNMS_LA_ATOM_ELSE:				return _T("else");
		case TNMS_LA_ATOM_FOR:				return _T("for");
		case TNMS_LA_ATOM_WHILE:			return _T("while");
		case TNMS_LA_ATOM_CONTINUE:			return _T("continue");
		case TNMS_LA_ATOM_BREAK:			return _T("break");
		case TNMS_LA_ATOM_SWITCH:			return _T("switch");
		case TNMS_LA_ATOM_DEFAULT:			return _T("default");
		case TNMS_LA_ATOM_CASE:				return _T("case");
		case TNMS_LA_ATOM_UNKNOWN:			return plad->unknown_list[atom.opt];

		case TNMS_LA_ATOM_TILDE:			return _T("~");
		case TNMS_LA_ATOM_AND:				return _T("&");
		case TNMS_LA_ATOM_OR:				return _T("|");
		case TNMS_LA_ATOM_HAT:				return _T("^");
		case TNMS_LA_ATOM_SL:				return _T("<<");
		case TNMS_LA_ATOM_SR:				return _T(">>");
		case TNMS_LA_ATOM_SR3:				return _T(">>>");
		case TNMS_LA_ATOM_AND_ASSIGN:		return _T("&=");
		case TNMS_LA_ATOM_OR_ASSIGN:		return _T("|=");
		case TNMS_LA_ATOM_HAT_ASSIGN:		return _T("^=");
		case TNMS_LA_ATOM_SL_ASSIGN:		return _T("<<=");
		case TNMS_LA_ATOM_SR_ASSIGN:		return _T(">>=");
		case TNMS_LA_ATOM_SR3_ASSIGN:		return _T(">>>=");

		default:							return _T("");
	}
}

// ****************************************************************
// ������ - Lexical Analysis -
// ================================================================
bool C_tnms_lexcial_analizer::analize(S_tnms_ca_data* pcad, S_tnms_la_data* plad)
{
	m_ca_data = *pcad;		// ca �f�[�^
	m_cur_atom_id = 0;		// �A�g���h�c
	m_cur_line = 1;			// �s���i�P�s�ڂ���X�^�[�g�j

	// �ԕ��̑}��
	for (int z = 0; z < 256; z++)
		m_ca_data.scn_text.push_back(_T('\0'));

	// �����J�n
	TSTR::iterator p = m_ca_data.scn_text.begin();
	TSTR::iterator end = m_ca_data.scn_text.end();
	while (*p != _T('\0'))	{

		TCHAR moji = *p;

		// �Z�p���[�^���X�L�b�v
		if (!skip_separator(p, end))
			break;	// �t�@�C���I�[�ɂ��I��

		// �\���̏�����
		S_tnms_atom atom;
		atom.line = m_cur_line;
		atom.id = m_cur_atom_id++;

		// �P�����ڂɂ�蕪��
		if (false);

		// �P�����ڂ�����ȑS�p����
		else if (*p == _T('�y'))	{	atom.type = TNMS_LA_ATOM_OPEN_SUMI;		p++;	}
		else if (*p == _T('�z'))	{	atom.type = TNMS_LA_ATOM_CLOSE_SUMI;	p++;	}

		// �P�����ڂ��S�p����
		else if (is_zenkaku(*p))	{
			if (!get_zenkaku_string(p, end, &atom))			return false;
		}
		// �P�����ڂ��P��Ɏg���镶��
		else if (*p == _T('_') || *p == _T('$') || *p == _T('@') || (_T('a') <= *p && *p <= _T('z')))	{
			if (!get_word(p, end, &atom))					return false;
		}
		// �P�����ڂ�����
		else if (_T('0') <= *p && *p <= _T('9'))	{
			if (!get_integer(p, end, &atom))				return false;
		}
		// �V���O���R�[�e�[�V��������
		else if (*p == _T('\''))	{
			if (!get_single_quoted_string(p, end, &atom))	return false;
		}
		// �_�u���R�[�e�[�V����������
		else if (*p == _T('"'))	{
			if (!get_double_quoted_string(p, end, &atom))	return false;
		}
		// ���x��
		else if (*p == _T('#'))	{
			if (!get_label(p, end, &atom))					return false;
		}

		// ���Z�q
		else if (*p == _T('>') && *(p + 1) == _T('>') && *(p + 2) == _T('>') && *(p + 3) == _T('='))	{	atom.type = TNMS_LA_ATOM_SR3_ASSIGN;	p += 4;	}

		else if (*p == _T('>') && *(p + 1) == _T('>') && *(p + 2) == _T('>'))	{	atom.type = TNMS_LA_ATOM_SR3;		p += 3;	}
		else if (*p == _T('<') && *(p + 1) == _T('<') && *(p + 2) == _T('='))	{	atom.type = TNMS_LA_ATOM_SL_ASSIGN;	p += 3;	}
		else if (*p == _T('>') && *(p + 1) == _T('>') && *(p + 2) == _T('='))	{	atom.type = TNMS_LA_ATOM_SR_ASSIGN;	p += 3;	}

		else if (*p == _T('+') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_PLUS_ASSIGN;		p += 2;	}
		else if (*p == _T('-') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_MINUS_ASSIGN;		p += 2;	}
		else if (*p == _T('*') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_MULTIPLE_ASSIGN;	p += 2;	}
		else if (*p == _T('/') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_DIVIDE_ASSIGN;		p += 2;	}
		else if (*p == _T('%') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_PERCENT_ASSIGN;	p += 2;	}
		else if (*p == _T('&') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_AND_ASSIGN;		p += 2;	}
		else if (*p == _T('|') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_OR_ASSIGN;			p += 2;	}
		else if (*p == _T('^') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_HAT_ASSIGN;		p += 2;	}
		else if (*p == _T('<') && *(p + 1) == _T('<'))	{	atom.type = TNMS_LA_ATOM_SL;				p += 2;	}
		else if (*p == _T('>') && *(p + 1) == _T('>'))	{	atom.type = TNMS_LA_ATOM_SR;				p += 2;	}
		else if (*p == _T('=') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_EQUAL;				p += 2;	}
		else if (*p == _T('!') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_NOT_EQUAL;			p += 2;	}
		else if (*p == _T('>') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_GREATER_EQUAL;		p += 2;	}
		else if (*p == _T('<') && *(p + 1) == _T('='))	{	atom.type = TNMS_LA_ATOM_LESS_EQUAL;		p += 2;	}
		else if (*p == _T('&') && *(p + 1) == _T('&'))	{	atom.type = TNMS_LA_ATOM_LOGICAL_AND;		p += 2;	}
		else if (*p == _T('|') && *(p + 1) == _T('|'))	{	atom.type = TNMS_LA_ATOM_LOGICAL_OR;		p += 2;	}

		else if (*p == _T('='))							{	atom.type = TNMS_LA_ATOM_ASSIGN;			p++;	}
		else if (*p == _T('+'))							{	atom.type = TNMS_LA_ATOM_PLUS;				p++;	}
		else if (*p == _T('-'))							{	atom.type = TNMS_LA_ATOM_MINUS;				p++;	}
		else if (*p == _T('*'))							{	atom.type = TNMS_LA_ATOM_MULTIPLE;			p++;	}
		else if (*p == _T('/'))							{	atom.type = TNMS_LA_ATOM_DIVIDE;			p++;	}
		else if (*p == _T('%'))							{	atom.type = TNMS_LA_ATOM_PERCENT;			p++;	}
		else if (*p == _T('&'))							{	atom.type = TNMS_LA_ATOM_AND;				p++;	}
		else if (*p == _T('|'))							{	atom.type = TNMS_LA_ATOM_OR;				p++;	}
		else if (*p == _T('^'))							{	atom.type = TNMS_LA_ATOM_HAT;				p++;	}
		else if (*p == _T('>'))							{	atom.type = TNMS_LA_ATOM_GREATER;			p++;	}
		else if (*p == _T('<'))							{	atom.type = TNMS_LA_ATOM_LESS;				p++;	}
		else if (*p == _T('~'))							{	atom.type = TNMS_LA_ATOM_TILDE;				p++;	}

		// ���̑��̋L��
		else if (*p == _T('.'))		{atom.type = TNMS_LA_ATOM_DOT;				p++;}
		else if (*p == _T(','))		{atom.type = TNMS_LA_ATOM_COMMA;			p++;}
		else if (*p == _T(':'))		{atom.type = TNMS_LA_ATOM_COLON;			p++;}
		else if (*p == _T('('))		{atom.type = TNMS_LA_ATOM_OPEN_PAREN;		p++;}
		else if (*p == _T(')'))		{atom.type = TNMS_LA_ATOM_CLOSE_PAREN;		p++;}
		else if (*p == _T('['))		{atom.type = TNMS_LA_ATOM_OPEN_BRACKET;		p++;}
		else if (*p == _T(']'))		{atom.type = TNMS_LA_ATOM_CLOSE_BRACKET;	p++;}
		else if (*p == _T('{'))		{atom.type = TNMS_LA_ATOM_OPEN_BRACE;		p++;}
		else if (*p == _T('}'))		{atom.type = TNMS_LA_ATOM_CLOSE_BRACE;		p++;}

		// ���̑��̕���
		else
			return error(m_cur_line, _T("�����ȕ����ł��B\'") + tostr_moji(*p) + _T("\'"));
		
		// �P������X�g�ɒǉ�
		if (atom.type != TNMS_LA_ATOM_NONE)
			m_la_data.atom_list.push_back(atom);
	}

	// �I�[�̃A�g��������
	S_tnms_atom eof_atom;
	eof_atom.line = m_cur_line;
	eof_atom.id = m_cur_atom_id++;
	eof_atom.type = TNMS_LA_ATOM_EOF;
	m_la_data.atom_list.push_back(eof_atom);

	// �_�~�[�������ǉ�
	m_la_data.str_list.push_back(_T("dummy"));

	// ���ʂ�Ԃ�
	*plad = m_la_data;

	return true;
}

// ****************************************************************
// �����́F�Z�p���[�^���X�L�b�v
// ================================================================
bool C_tnms_lexcial_analizer::skip_separator(TSTR::iterator& p, TSTR::iterator end)
{
	while (1)	{
		if (false);

		// �I�[
		else if (*p == _T('\0'))
			return false;
		// ���s
		else if (*p == _T('\n'))	{
			p++;
			m_cur_line++;
		}
		// ���̑��̃Z�p���[�^
		else if (*p == ' ' || *p == '\t')
			p++;
		// �Z�p���[�^�ȊO�̕���
		else
			break;			
	}

	return true;
}

// ****************************************************************
// �����́F���x���̎擾
// ================================================================
bool C_tnms_lexcial_analizer::get_label(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom)
{
	// �ŏ��� '#' ���p�X
	p++;

	// ���݂̈ʒu���L��
	TSTR::iterator start = p;
	// �A���t�@�x�b�g�܂��͐����������Ԑi��
	for (; *p == _T('_') || (_T('a') <= *p && *p <= _T('z')) || (_T('0') <= *p && *p <= _T('9')); p++);
	// ���x�����̌���
	TSTR name = TSTR(start, p);

	// �y���x���̏ꍇ
	if ((int)name.length() == 2 && name[0] == _T('z') && ('0' <= name[1] && name[1] <= _T('9')))	{
		atom->type = TNMS_LA_ATOM_Z_LABEL;
		atom->opt = name[1] - _T('0');

		// �ʏ�̃��x���Ƃ��Ă��o�^����I

		// ���x�����̌���
		S_tnms_la_label label(name, m_cur_line);
		ARRAY<S_tnms_la_label>::iterator label_itr = std::find(m_la_data.label_list.begin(), m_la_data.label_list.end(), label);
		if (label_itr == m_la_data.label_list.end())	{
			// ������Ȃ������̂ŐV�K���x���Ƃ��ēo�^
			atom->subopt = (int)m_la_data.label_list.size();		// subopt ���g��
			m_la_data.label_list.push_back(label);
		}
		else	{
			// ���������̂Ń��x���ԍ���ݒ�
			atom->subopt = (int)(label_itr - m_la_data.label_list.begin());		// subopt ���g��
		}
	}
	else if ((int)name.length() == 3 && name[0] == _T('z') && ('0' <= name[1] && name[1] <= _T('9')) && (_T('0') <= name[2] && name[2] <= _T('9')))	{
		atom->type = TNMS_LA_ATOM_Z_LABEL;
		atom->opt = (name[1] - _T('0')) * 10 + (name[2] - _T('0'));

		// �ʏ�̃��x���Ƃ��Ă��o�^����I

		// ���x�����̌���
		S_tnms_la_label label(name, m_cur_line);
		ARRAY<S_tnms_la_label>::iterator label_itr = std::find(m_la_data.label_list.begin(), m_la_data.label_list.end(), label);
		if (label_itr == m_la_data.label_list.end())	{
			// ������Ȃ������̂ŐV�K���x���Ƃ��ēo�^
			atom->subopt = (int)m_la_data.label_list.size();		// subopt ���g��
			m_la_data.label_list.push_back(label);
		}
		else	{
			// ���������̂Ń��x���ԍ���ݒ�
			atom->subopt = (int)(label_itr - m_la_data.label_list.begin());		// subopt ���g��
		}
	}
	else if ((int)name.length() == 4 && name[0] == _T('z') && ('0' <= name[1] && name[1] <= _T('9')) && (_T('0') <= name[2] && name[2] <= _T('9')) && (_T('0') <= name[3] && name[3] <= _T('9')))	{
		atom->type = TNMS_LA_ATOM_Z_LABEL;
		atom->opt = (name[1] - _T('0')) * 100 + (name[2] - _T('0')) * 10 + (name[3] - _T('0'));

		// �ʏ�̃��x���Ƃ��Ă��o�^����I

		// ���x�����̌���
		S_tnms_la_label label(name, m_cur_line);
		ARRAY<S_tnms_la_label>::iterator label_itr = std::find(m_la_data.label_list.begin(), m_la_data.label_list.end(), label);
		if (label_itr == m_la_data.label_list.end())	{
			// ������Ȃ������̂ŐV�K���x���Ƃ��ēo�^
			atom->subopt = (int)m_la_data.label_list.size();		// subopt ���g��
			m_la_data.label_list.push_back(label);
		}
		else	{
			// ���������̂Ń��x���ԍ���ݒ�
			atom->subopt = (int)(label_itr - m_la_data.label_list.begin());		// subopt ���g��
		}
	}
	// �ʏ�̃��x���̏ꍇ
	else	{
		atom->type = TNMS_LA_ATOM_LABEL;

		// ���x�����̌���
		S_tnms_la_label label(name, m_cur_line);
		ARRAY<S_tnms_la_label>::iterator label_itr = std::find(m_la_data.label_list.begin(), m_la_data.label_list.end(), label);
		if (label_itr == m_la_data.label_list.end())	{
			// ������Ȃ������̂ŐV�K���x���Ƃ��ēo�^
			atom->opt  = (int)m_la_data.label_list.size();		// opt ���g��
			m_la_data.label_list.push_back(label);
		}
		else	{
			// ���������̂Ń��x���ԍ���ݒ�
			atom->opt  = (int)(label_itr - m_la_data.label_list.begin());			// opt ���g��
		}
	}

	return true;
}

// ****************************************************************
// �����́F���l�����̎擾
// ================================================================
bool C_tnms_lexcial_analizer::get_integer(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom)
{
	int value = 0;

	if (false);
	else if (*p == _T('0') && *(p + 1) == _T('b'))	{
		p += 2;

		// 2�i��
		for (; _T('0') <= *p && *p <= _T('1'); p++)	{
			value *= 2;
			value += (*p - _T('0'));
		}
	}
	else if (*p == _T('0') && *(p + 1) == _T('x'))	{
		p += 2;

		// 16�i��
		for (; (_T('0') <= *p && *p <= _T('9')) || (_T('a') <= *p && *p <= _T('f')); p++)	{
			value *= 16;
			value += is_num(*p) ? (*p - _T('0')) : (*p - _T('a') + 10);
		}
	}
	else	{
		// 10�i��
		for (; _T('0') <= *p && *p <= _T('9'); p++)	{
			value *= 10;
			value += *p - _T('0');
		}
	}

	atom->type = TNMS_LA_ATOM_VAL_INT;
	atom->opt  = value;
	return true;
}

// ****************************************************************
// �����́F�V���O���R�[�e�[�V����������̎擾
// ================================================================
bool C_tnms_lexcial_analizer::get_single_quoted_string(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom)
{
	// �����̒������擾�i�G�X�P�[�v���Ă�����Q�ɂȂ�܂��j
	int len = 1;
	if (*(p + 1) == _T('\\'))
		len = 2;

	// �����𐔒l�ɕϊ�
	atom->type = TNMS_LA_ATOM_VAL_INT;
	atom->opt  = *(p + len);

	// �V���O���R�[�e�[�V�����~�Q�{�����̒����������i��
	p += 2 + len;

	return true;
}

// ****************************************************************
// �����́F�_�u���R�[�e�[�V����������̎擾
// ================================================================
bool C_tnms_lexcial_analizer::get_double_quoted_string(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom)
{
	TSTR result;

	// �ŏ��� '"' ���p�X
	p++;

	// �I���̃_�u���R�[�e�[�V������������܂Ői��
	while (*p != _T('"'))	{
		// �G�X�P�[�v��W�J����
		if (*p == _T('\\'))	{
			if (*(p + 1) == _T('n'))	{
				result.push_back(_T('\n'));
				p += 2;
			}
			else	{
				result.push_back(*(p + 1));
				p += 2;
			}
		}
		// �ʏ�̕���
		else	{
			result.push_back(*p);
			p += 1;
		}
	}

	// �����񃊃X�g�ɓo�^
	atom->type = TNMS_LA_ATOM_VAL_STR;
	atom->opt  = (int)m_la_data.str_list.size();
	m_la_data.str_list.push_back(result);

	// �I�[�� '"' ���p�X
	p++;

	return true;
}

// ****************************************************************
// �����́F�S�p������̎擾
// ================================================================
bool C_tnms_lexcial_analizer::get_zenkaku_string(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom)
{
	// ���݂̈ʒu���L��
	TSTR::iterator start = p;
	// �S�p�����������Ԑi��
	for (; is_zenkaku(*p) && *p != _T('�y') && *p != _T('�z'); p++);
	// ��������쐬
	TSTR str = TSTR(start, p);
	
	// �����񃊃X�g�ɓo�^
	atom->type = TNMS_LA_ATOM_VAL_STR;
	atom->opt  = (int)m_la_data.str_list.size();
	m_la_data.str_list.push_back(str);

	return true;
}

// ****************************************************************
// �����́F�P��̎擾
// ================================================================
bool C_tnms_lexcial_analizer::get_word(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom)
{
	// ���݂̈ʒu���L��
	TSTR::iterator start = p;
	// �A���t�@�x�b�g�␔���������Ԑi��
	for (; (_T('a') <= *p && *p <= _T('z')) || (_T('0') <= *p && *p <= _T('9')) || *p == _T('_') || *p == _T('$') || *p == _T('@'); p++);
	// ��������쐬
	TSTR word = TSTR(start, p);

	// �\���
	if (false);
	else if (word == _T("command"))		atom->type = TNMS_LA_ATOM_COMMAND;
	else if (word == _T("property"))	atom->type = TNMS_LA_ATOM_PROPERTY;
	else if (word == _T("goto"))		atom->type = TNMS_LA_ATOM_GOTO;
	else if (word == _T("gosub"))		atom->type = TNMS_LA_ATOM_GOSUB;
	else if (word == _T("gosubstr"))	atom->type = TNMS_LA_ATOM_GOSUBSTR;
	else if (word == _T("return"))		atom->type = TNMS_LA_ATOM_RETURN;
	else if (word == _T("if"))			atom->type = TNMS_LA_ATOM_IF;
	else if (word == _T("elseif"))		atom->type = TNMS_LA_ATOM_ELSEIF;
	else if (word == _T("else"))		atom->type = TNMS_LA_ATOM_ELSE;
	else if (word == _T("for"))			atom->type = TNMS_LA_ATOM_FOR;
	else if (word == _T("while"))		atom->type = TNMS_LA_ATOM_WHILE;
	else if (word == _T("continue"))	atom->type = TNMS_LA_ATOM_CONTINUE;
	else if (word == _T("break"))		atom->type = TNMS_LA_ATOM_BREAK;
	else if (word == _T("switch"))		atom->type = TNMS_LA_ATOM_SWITCH;
	else if (word == _T("case"))		atom->type = TNMS_LA_ATOM_CASE;
	else if (word == _T("default"))		atom->type = TNMS_LA_ATOM_DEFAULT;

	// ���̑��Ȃ�A����`�ꃊ�X�g�Ƃ��Ēǉ�
	else	{
		atom->type = TNMS_LA_ATOM_UNKNOWN;
		atom->opt  = (int)m_la_data.unknown_list.size();
		m_la_data.unknown_list.push_back(word);
	}

	return true;
}

// ****************************************************************
// �����́F�G���[
// ================================================================
bool C_tnms_lexcial_analizer::error(int line, CTSTR& str)
{
	m_error_line = line;
	m_error_str = str;

	return false;
}