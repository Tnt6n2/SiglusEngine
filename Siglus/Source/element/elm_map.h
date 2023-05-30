#pragma		once

#include	"data/tnm_element.h"

class	C_tnm_save_stream;

// ****************************************************************
// マップ
// ================================================================
class C_elm_map : public C_tnm_element
{
public:
	// エレメントの基本動作
	void	init(S_element element, CTSTR& name);					// 初期化
	void	reinit();												// 再初期化
	void	save(C_tnm_save_stream& stream);						// セーブ
	void	load(C_tnm_save_stream& stream);						// ロード

	// パラメータ
	int		get_size()	{ return m_map.size(); }

private:
	std::map<TSTR, int>		m_map;
};

