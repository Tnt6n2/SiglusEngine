#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"element/elm_object.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// オブジェクト：数値パラメータの初期化
// ================================================================
void C_elm_object::init_number_param()
{
	m_op.number.number = 0;				// 数値
	m_op.number.keta_max = 0;			// 最大桁数
	m_op.number.disp_zero = false;		// 先頭の 0 を表示
	m_op.number.disp_sign = false;		// 符号を表示
	m_op.number.tumeru_sign = false;	// 符号をつめて表示
	m_op.number.space_mod = 0;			// 間隔タイプ
	m_op.number.space = 0;				// 間隔
}

// ****************************************************************
// オブジェクト：数値パラメータの設定
// ================================================================
void C_elm_object::set_number_param(int keta, int disp_zero, int disp_sign, int tumeru_sign, int space_mod, int space)
{
	m_op.number.keta_max = keta;			// 桁数
	m_op.number.disp_zero = disp_zero;		// ０を表示
	m_op.number.disp_sign = disp_sign;		// 符号を表示
	m_op.number.tumeru_sign = tumeru_sign;	// 符号を詰める
	m_op.number.space_mod = space_mod;		// スペースのモード
	m_op.number.space = space;				// スペース
}

// ****************************************************************
// オブジェクト：数値オブジェクトの作成
// ================================================================
bool C_elm_object::create_number(TSTR file_path)
{
	// 既に作られている場合は何もしない
	if (m_op.type == TNM_OBJECT_TYPE_NUMBER && m_op.file_path == file_path)
		return true;

	// タイプ別のパラメータを初期化する
	init_type(true);

	// パラメータを設定
	m_op.type = TNM_OBJECT_TYPE_NUMBER;
	m_op.file_path = file_path;

	// 数値オブジェクトを再構築する
	if (!restruct_number())
		return false;

	return true;
}

// ****************************************************************
// オブジェクト：数値オブジェクトを再構築
// ================================================================
bool C_elm_object::restruct_number()
{
	// テクスチャを読み込む
	m_album = tnm_load_pct_d3d(m_op.file_path, false, m_op_def.set_cg_table);
	if (!m_album)	{
		m_op.file_path.clear();		// 失敗したのでファイル名をクリア
		return false;
	}

	// スプライトを作成
	m_sprite_list.resize(m_sprite_cnt_number);
	for (int i = 0; i < m_sprite_cnt_number; i++)	{
		if (!m_sprite_list[i].create_polygon_no_init())
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("数値オブジェクトの作成に失敗しました。"));

		m_sprite_list[i].set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_sprite_list[i].set_index_buffer(G_rect_index_buffer);
	}

	return true;
}

// ****************************************************************
// オブジェクト：数値オブジェクトのフレーム処理
// ================================================================
void C_elm_object::number_frame()
{
	int pat_no[m_sprite_cnt_number];

	// スプライトの設定
	for (int i = 0; i < m_sprite_cnt_number; i++)	{
		trp_to_rp(&m_sprite_list[i].rpex, &m_sprite_list[i].rp);
		m_sprite_list[i].rp.disp = false;	// 一旦非表示にする
		pat_no[i] = 0;						// パターン番号を初期化
	}

	// 数値の設定
	if (m_trp.disp && m_album)	{
		int num = abs(m_op.number.number);
		int sign = get_sign(m_op.number.number);
		bool disp_sign = m_op.number.disp_sign || sign == -1;
		bool tumeru_sign = m_op.number.tumeru_sign && !m_op.number.disp_zero;
		int keta_max = m_op.number.keta_max;
		int keta = get_keta(num);			// 桁数を求める
		BSA<int> digit(new int[keta]);		// 桁分解用のバッファ
		get_keta(num, digit.get(), keta);	// 桁分解

		// 0 を表示する場合は、先に 0 で埋める
		if (m_op.number.disp_zero)	{
			for (int i = 0; i < keta_max; i++)	{
				pat_no[i] = 0;
				m_sprite_list[i].rp.disp = true;
			}
		}
		// 符号と数値の位置を求める
		int num_pos, sign_pos = -1;
		num_pos = std::max(keta_max - keta, 0);
		if (disp_sign)	{
			if (tumeru_sign)	sign_pos = std::max(num_pos - 1, 0);
			else				sign_pos = 0;
			// 数値の位置を補正
			num_pos = std::max(num_pos, sign_pos + 1);
		}
		// 数値スプライトの設定
		for (int i = 0; i < keta; i++)	{
			pat_no[num_pos + i] = digit[i];
			m_sprite_list[num_pos + i].rp.disp = true;
		}
		// 符号スプライトの設定
		if (disp_sign)	{
			if (false);
			else if (sign == 0)		pat_no[sign_pos] = 12;
			else if (sign == 1)		pat_no[sign_pos] = 11;
			else if (sign == -1)	pat_no[sign_pos] = 10;
			m_sprite_list[sign_pos].rp.disp = true;
		}
		// パターン番号を確定する
		for (int i = 0; i < m_sprite_cnt_number; i++)	{
			pat_no[i] += m_trp.pat_no;
		}
		// パターン番号からテクスチャを設定する
		for (int i = 0; i < m_sprite_cnt_number; i++)	{
			if (m_sprite_list[i].rp.disp)	{
				m_sprite_list[i].set_texture(m_album->get_texture(pat_no[i]));
				m_sprite_list[i].rp.size_fit_to_texture = 0;
			}
		}
		// スプライトの位置の設定
		int offset = 0;
		for (int i = 0; i < m_sprite_cnt_number; i++)	{
			m_sprite_list[i].rp.center.x -= offset;
			offset += m_op.number.space;
			if (m_op.number.space_mod == 0)	{		// 画像サイズを使う
				if (pat_no[i] >= 0 && m_sprite_list[i].get_texture(pat_no[i]))	{
					offset += m_album->get_texture(pat_no[i])->get_original_width();
				}
				else if (m_album->get_texture(0))	{
					offset += m_album->get_texture(0)->get_original_width();		// 表示しない場合は０番テクスチャのサイズを使う
				}
			}
		}
	}
}


