#pragma		once

#include	"data/tnm_element.h"
#include	"engine/ifc_error.h"

class	C_tnm_save_stream;

// ****************************************************************
// 汎用リスト（ベース）
// ================================================================
//		直接インスタンスを作ることはできません。
//		継承した C_elm_list や C_elm_list_ex を使って下さい。
// ================================================================
template <typename TYPE>
class C_elm_list_base : public C_tnm_element
{
protected:
	C_elm_list_base() : m_def_extend_enable(true), m_def_size(0)	{}

public:
	// 通常操作
	int		get_size() const 	{	return (int)m_sub.size();	}	// サイズ取得
	bool	is_empty() const	{	return m_sub.size() == 0;	}	// 空判定

	// パラメータを取得
	ARRAY<TYPE>&		get_sub()			{	return m_sub;				}
	const ARRAY<TYPE>&	get_sub() const		{	return m_sub;				}
	TYPE*				get_ptr()			{	return m_sub.get();			}
	const TYPE*			get_ptr() const		{	return m_sub.get();			}
	TYPE*				get_end_ptr()		{	return m_sub.get_end();		}
	const TYPE*			get_end_ptr() const	{	return m_sub.get_end();		}

	TYPE*				get_sub(int index, bool disp_error);									// 範囲チェックあり版
	TYPE&				operator [] (int index)				{	return m_sub[index];		}	// 範囲チェックなし版
	const TYPE&			operator [] (int index) const		{	return m_sub[index];		}	// 範囲チェックなし版

protected:
	ARRAY<TYPE>		m_sub;
	int				m_def_size;
	bool			m_def_extend_enable;
};

// ****************************************************************
// 汎用リスト（通常）
// ================================================================
//		通常はこれを使って下さい。
// ================================================================
template <typename TYPE>
class C_elm_list : public C_elm_list_base<TYPE>
{
public:
	C_elm_list()	{}

	// エレメント基本操作
	void	init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent);	// 初期化
	void	reinit();								// 再初期化
	void	resize(int size);						// サイズ変更
	void	clear();								// クリア
	void	save(C_tnm_save_stream& stream);		// セーブ
	void	load(C_tnm_save_stream& stream);		// ロード

protected:

	// 以下の仮想関数は、C_elm_list を継承するクラスがオーバーライドする必要があります。
	virtual	void	_init(int begin, int end) = 0;
	virtual	void	_reinit(int begin, int end) = 0;
	virtual	void	_save(C_tnm_save_stream& stream, int begin, int end) = 0;
	virtual	void	_load(C_tnm_save_stream& stream, int begin, int end) = 0;
};

// ****************************************************************
// 汎用リスト（拡張）
// ================================================================
//		再構築や破壊などの処理を細かく制御する場合はこちらを使って下さい。
//		オブジェクト、ステージ、フレームアクションなど。
// ================================================================
template <typename TYPE>
class C_elm_list_ex : public C_elm_list_base<TYPE>
{
public:
	C_elm_list_ex()	{}

	// エレメント基本操作
	void	init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent);	// 初期化
	void	reinit(bool restruct_flag);				// 再初期化
	void	resize(int size);						// サイズ変更
	void	clear();								// クリア
	void	save(C_tnm_save_stream& stream);		// セーブ
	void	load(C_tnm_save_stream& stream);		// ロード

protected:

	// 以下の仮想関数は、C_elm_list を継承するクラスがオーバーライドする必要があります。
	virtual	void	_init(int begin, int end) = 0;
	virtual	void	_reinit(int begin, int end, bool restruct_flag) = 0;
	virtual	void	_save(C_tnm_save_stream& stream, int begin, int end) = 0;
	virtual	void	_load(C_tnm_save_stream& stream, int begin, int end) = 0;
};

// ****************************************************************
// 汎用リスト（ベース）：サブを取得
// ================================================================
template <typename TYPE>
inline TYPE* C_elm_list_base<TYPE>::get_sub(int index, bool disp_error)
{
	// 範囲チェック
	if (index < 0 || (int)m_sub.size() <= index)	{
		if (disp_error)
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("範囲外の ") + get_element_name() + _T("[") + tostr(index) + _T("] を参照しました。"));

		return NULL;
	}

	return &m_sub[index];
}

// ****************************************************************
// 汎用リスト（通常）：初期化
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, form, name, p_parent);

	// 固定パラメータの設定
	m_def_size = size;						// 初期サイズ
	m_def_extend_enable = extend_enable;	// 拡張可能フラグ

	// 初期サイズ
	m_sub.resize(size);

	// サブの初期化
	_init(0, size);
}

// ****************************************************************
// 汎用リスト（通常）：再初期化
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::reinit()
{
	// サイズ可変の場合
	if (m_def_extend_enable)	{
		int new_cnt = m_def_size;
		int old_cnt = (int)m_sub.size();
		m_sub.resize(new_cnt);

		// 維持されたサブは再初期化
		// 増えたサブは初期化
		if (new_cnt > old_cnt)	{
			_reinit(0, old_cnt);
			_init(old_cnt, new_cnt);
		}
		else	{
			_reinit(0, new_cnt);
		}
	}

	// サイズ固定の場合
	else	{

		// サブの初期化
		_reinit(0, m_def_size);
	}
}

// ****************************************************************
// 汎用リスト（通常）：クリア
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::clear()
{
	// サイズ可変の場合
	if (m_def_extend_enable)	{
		m_sub.clear();
	}

	// サイズ固定の場合
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, get_element_name() + _T(" はサイズを変更できません。"));
	}
}

// ****************************************************************
// 汎用リスト（通常）：サイズ変更
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::resize(int new_cnt)
{
	// サイズ可変の場合
	if (m_def_extend_enable)	{
		int old_cnt = (int)m_sub.size();

		// 個数を変更
		m_sub.resize(new_cnt);

		// 増えたサブは初期化
		if (new_cnt > old_cnt)	{
			_init(old_cnt, new_cnt);
		}
	}

	// サイズ固定の場合
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, get_element_name() + _T(" はサイズを変更できません。"));
	}
}

// ****************************************************************
// 汎用リスト（通常）：セーブ
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::save(C_tnm_save_stream& stream)
{
	int jump;

	// サイズ可変の場合
	if (m_def_extend_enable)	{
		stream.save(m_sub.size());
		_save(stream, 0, (int)m_sub.size());
	}

	// サイズ固定の場合
	else	{
		stream.save_fixed_array_begin(m_sub, &jump);
		_save(stream, 0, (int)m_sub.size());
		stream.save_fixed_array_end(&jump);
	}
}

// ****************************************************************
// 汎用リスト（通常）：ロード
// ================================================================
template <typename TYPE>
void C_elm_list<TYPE>::load(C_tnm_save_stream& stream)
{
	int cnt, jump;

	// サイズ可変の場合
	if (m_def_extend_enable)	{
		cnt = stream.load_ret<int>();
		resize(cnt);
		_load(stream, 0, cnt);
	}

	// サイズ固定の場合
	else	{
		cnt = stream.load_fixed_array_begin(m_sub, get_element_name(), &jump);
		_load(stream, 0, cnt);
		stream.load_fixed_array_end(&jump);
	}
}

// ****************************************************************
// 汎用リスト（拡張）：初期化
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, form, name, p_parent);

	// 固定パラメータの設定
	m_def_size = size;						// 初期サイズ
	m_def_extend_enable = extend_enable;	// 拡張可能フラグ

	// 初期サイズ
	m_sub.resize(size);

	// サブの初期化
	_init(0, size);
}

// ****************************************************************
// 汎用リスト（拡張）：再初期化
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::reinit(bool restruct_flag)
{
	// サイズ可変の場合
	if (m_def_extend_enable)	{
		int new_cnt = m_def_size;
		int old_cnt = (int)m_sub.size();
		m_sub.resize(new_cnt);

		// 維持されたサブは再初期化
		// 増えたサブは初期化
		if (new_cnt > old_cnt)	{
			_reinit(0, old_cnt, restruct_flag);
			_init(old_cnt, new_cnt);
		}
		else	{
			_reinit(0, new_cnt, restruct_flag);
		}
	}

	// サイズ固定の場合
	else	{

		// サブの初期化
		_reinit(0, m_def_size, restruct_flag);
	}
}

// ****************************************************************
// 汎用リスト（拡張）：クリア
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::clear()
{
	// サイズ可変の場合
	if (m_def_extend_enable)	{
		m_sub.clear();
	}

	// サイズ固定の場合
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, get_element_name() + _T(" はサイズを変更できません。"));
	}
}

// ****************************************************************
// 汎用リスト（拡張）：サイズ変更
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::resize(int new_cnt)
{
	// サイズ可変の場合
	if (m_def_extend_enable)	{
		int old_cnt = (int)m_sub.size();

		// 個数を変更
		m_sub.resize(new_cnt);

		// 増えたサブは初期化
		if (new_cnt > old_cnt)	{
			_init(old_cnt, new_cnt);
		}
	}

	// サイズ固定の場合
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, get_element_name() + _T(" はサイズを変更できません。"));
	}
}

// ****************************************************************
// 汎用リスト（拡張）：セーブ
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::save(C_tnm_save_stream& stream)
{
	int jump;

	// サイズ可変の場合
	if (m_def_extend_enable)	{
		stream.save(m_sub.size());
		_save(stream, 0, (int)m_sub.size());
	}

	// サイズ固定の場合
	else	{
		stream.save_fixed_array_begin(m_sub, &jump);
		_save(stream, 0, (int)m_sub.size());
		stream.save_fixed_array_end(&jump);
	}
}

// ****************************************************************
// 汎用リスト（拡張）：ロード
// ================================================================
template <typename TYPE>
void C_elm_list_ex<TYPE>::load(C_tnm_save_stream& stream)
{
	int cnt, jump;

	// サイズ可変の場合
	if (m_def_extend_enable)	{
		cnt = stream.load_ret<int>();
		resize(cnt);
		_load(stream, 0, cnt);
	}

	// サイズ固定の場合
	else	{
		cnt = stream.load_fixed_array_begin(m_sub, get_element_name(), &jump);
		_load(stream, 0, cnt);
		stream.load_fixed_array_end(&jump);
	}
}

// ****************************************************************
// 整数リスト
// ================================================================
class C_elm_int_list : public C_elm_list<int>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable);
	void	set_value(int index, int value);					// 値を設定する
	int		get_value(int index);								// 値を取得する

	// ビット操作
	void	set_bit_1(int index, int value);
	void	set_bit_2(int index, int value);
	void	set_bit_4(int index, int value);
	void	set_bit_8(int index, int value);
	void	set_bit_16(int index, int value);
	int		get_bit_1(int index);
	int		get_bit_2(int index);
	int		get_bit_4(int index);
	int		get_bit_8(int index);
	int		get_bit_16(int index);

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};

// ****************************************************************
// 整数リスト：値を設定
// ================================================================
inline void C_elm_int_list::set_value(int index, int value)
{
	int* p = get_sub(index, true);
	if (p)
		*p = value;
}

// ****************************************************************
// 整数リスト：値を取得
// ================================================================
inline int C_elm_int_list::get_value(int index)
{
	int* p = get_sub(index, true);
	if (p)
		return *p;
	else
		return 0;
}

// ****************************************************************
// 文字列リスト
// ================================================================
class C_elm_str_list : public C_elm_list<TSTR>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable);
	void	set_value(int index, CTSTR& value);					// 値を設定する
	TSTR	get_value(int index);								// 値を取得する

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};

// ****************************************************************
// 文字列リスト：値を設定
// ================================================================
inline void C_elm_str_list::set_value(int index, CTSTR& value)
{
	TSTR* p = get_sub(index, true);
	if (p)
		*p = value;
}

// ****************************************************************
// 文字列リスト：値を取得
// ================================================================
inline TSTR C_elm_str_list::get_value(int index)
{
	TSTR* p = get_sub(index, true);
	if (p)
		return *p;
	else
		return _T("");
}
