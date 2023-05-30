#pragma		once

#include	<tnm_element_code.h>

// ****************************************************************
// エレメントコード
// ================================================================
const int TNM_ELEMENT_CODE_MAX = 31;

struct	S_element
{
	int		code[TNM_ELEMENT_CODE_MAX];
	int		code_cnt;

	// コンストラクタ
	S_element() : code_cnt(0)	{}
	explicit S_element(int code_00) : code_cnt(1)	
	{
		code[0] = code_00;	
	}
	S_element(int code_00, int code_01) : code_cnt(2)
	{
		code[0] = code_00;
		code[1] = code_01;
	}
	S_element(int code_00, int code_01, int code_02) : code_cnt(3)
	{
		code[0] = code_00;
		code[1] = code_01;
		code[2] = code_02;
	}
	S_element(int code_00, int code_01, int code_02, int code_03) : code_cnt(4)
	{
		code[0] = code_00;
		code[1] = code_01;
		code[2] = code_02;
		code[3] = code_03;
	}
	S_element(int code_00, int code_01, int code_02, int code_03, int code_04) : code_cnt(5)
	{
		code[0] = code_00;
		code[1] = code_01;
		code[2] = code_02;
		code[3] = code_03;
		code[4] = code_04;
	}
	S_element(int code_00, int code_01, int code_02, int code_03, int code_04, int code_05) : code_cnt(6)
	{
		code[0] = code_00;
		code[1] = code_01;
		code[2] = code_02;
		code[3] = code_03;
		code[4] = code_04;
		code[5] = code_05;
	}
	S_element(const int* begin, const int* end)
	{
		code_cnt = (int)(end - begin);
		memcpy(code, begin, code_cnt * sizeof(int));
	}
	
	// 初期化
	void	clear()				{	code_cnt = 0;			}
	bool	is_empty() const	{	return code_cnt == 0;	}

	// ポインタ
	int*	begin()	{	return code;				}
	int*	end()	{	return code + code_cnt;		}
	int		back()	{	return code[code_cnt - 1];	}	// エラーチェックなし！
};

inline S_element operator + (const S_element& lhs, const S_element& rhs)
{
	S_element result = lhs;

	memcpy(&result.code[result.code_cnt], &rhs.code[0], rhs.code_cnt * sizeof(int));
	result.code_cnt += rhs.code_cnt;

	return result;
}

inline bool operator == (const S_element& lhs, const S_element& rhs)
{
	if (lhs.code_cnt != rhs.code_cnt)
		return false;

	return memcmp(lhs.code, rhs.code, lhs.code_cnt * sizeof(int)) == 0;
}

inline bool operator != (const S_element& lhs, const S_element& rhs)
{
	return !(lhs == rhs);
}

// ****************************************************************
// エレメントコードに ELM_UP をつける
// ================================================================
inline S_element tnm_set_element_down(const S_element& element)
{
	if (element.is_empty())	{
		return element;
	}
	else if (element.code[0] == ELM_UP)	{
		return S_element(ELM_UP) + element;
	}
	else if (element.code[0] == ELM_CURRENT)	{
		return S_element(ELM_CURRENT, ELM_UP) + S_element(element.code + 1, element.code + element.code_cnt);
	}
	else	{
		return element;
	}
}

// ****************************************************************
// エレメント
// ================================================================
//		全てエレメントの基底クラスです。
//		エレメントコード、型、名前を持ちます。
// ================================================================
class C_tnm_element
{
public:
	S_element		get_element() const			{	return m_element;		}
	int				get_element_form() const	{	return m_element_form;	}
	TSTR			get_element_name() const	{	return m_element_name;	}
	C_tnm_element*	get_parent() const			{	return m_p_parent;		}

	// エレメント基本操作
	void init(S_element element, int form, CTSTR& name, C_tnm_element* p_parent)
	{
		m_element = element;
		m_element_form = form;
		m_element_name = name;
		m_p_parent = p_parent;
	}

private:
	S_element		m_element;
	int				m_element_form;
	TSTR			m_element_name;
	C_tnm_element*	m_p_parent;
};

// ****************************************************************
// エレメントコードから、エレメントへのポインタを取得する
// ================================================================
C_tnm_element*	tnm_get_element_ptr(int* elm_begin, int* elm_end, bool disp_out_of_range_error);


