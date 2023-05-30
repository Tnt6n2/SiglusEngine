#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_moji.h"
#include	"data/tnm_moji_manager.h"
#include	"data/tnm_font.h"
#include	"engine/eng_msgbox.h"
#include	"engine/eng_config.h"
#include	"engine/eng_graphics.h"
#include	"dialog/cfg_wnd_modal_font.h"

#include	"localize/localize.h"

// ▲現在の処理では、文字が開放されるタイミングは「文字を表示した時」である。
// 　しかも１文字の表示につき最大で１つしか開放されない（開放されない場合もある）
// 　通常の文字サイズであれば、特に問題ではないが、巨大な文字の場合は問題かも。
// 　巨大な文字の場合、使用されなくなったならすぐに開放すべきである。
// 　回避策は、フレーム処理で常に監視するしかないだろう。
// 　サイズの大きな文字と、使用頻度の低い文字を開放する必要がある。

// ▲sel命令の文字が選択終了後も開放されないままである
// 　おそらく選択ウィンドウが消えた後も、実際のウィンドウからは文字自体が削除されていないからと考えられる。

// ****************************************************************
// 文字マネージャ
// ================================================================
C_tnm_moji_manager	G_moji_manager;

// ****************************************************************
// 文字マネージャ：文字テクスチャをロードする
// ================================================================
BSP<C_d3d_texture> C_tnm_moji_manager::load_moji(int moji_type, int moji, int moji_size, C_argb moji_color, MOJI_FACE_TYPE moji_face_type, bool tategaki, int buf_no)
{
	S_moji mojist;
	mojist.moji_type = moji_type;
	mojist.moji = moji;
	mojist.size = moji_size;
	mojist.color = moji_color;
	mojist.face_type = moji_face_type;

	// すでに存在するかどうか検索する
	MOJI_MAP::iterator itr = moji_map.find(mojist);

	// すでに存在する場合
	if (itr != moji_map.end())	{
		itr->second.load_counter = moji_load_counter;
		return itr->second.texture;
	}

	// 未使用のバッファを１つ探して開放する（開放しない場合もある）
	if (!moji_map.empty())	{

		int erase_moji_level = 0;
		DWORD counter;
		DWORD erase_moji_nouse_counter = 0;
		DWORD erase_moji_kanji_counter = 0;
		DWORD erase_moji_hiragana_counter = 0;

		MOJI_MAP::iterator end = moji_map.end();
		MOJI_MAP::iterator hit_itr = end;
		for (MOJI_MAP::iterator itr = moji_map.begin(); itr != end; ++itr)	{
			if (itr->second.texture.use_count() == 1)	{

				// moji_load_counter は文字が読み込まれるたびにカウントアップしていきます。（この関数の最後で処理している）
				// カウント値は作成された文字に記憶されます。（既に存在する場合の処理で再利用された場合にも記憶される）
				// 結果的にカウント値が大きいほど新しい文字と言う事になる。
				// ここでは、現在のカウント値から文字に記憶されたカウント値を引き、作成されてからの経過時間のような値を算出する事になる。
				// 算出された値が大きいほど古い文字と言う事になる。
				// 古い文字は優先的に開放される事になる。
				counter = (DWORD)(moji_load_counter - itr->second.load_counter);

				// サイズの大きな文字（４８以上：▲：実際には３３～６４のサイズの文字は６４×６４サイズのテクスチャーが生成されているので、３３以上とするべきかもしれない）
				if (itr->second.moji_size >= 48 && erase_moji_level < itr->second.moji_size)	{
					hit_itr = itr;
					erase_moji_level = itr->second.moji_size;	// 文字サイズをレベル判定に使用する（大きな文字ほど優先的に削除される）
				}

				// 使用されていない文字（１０００カウンター以上：この値が意味する事は、使用されなくなってから違う文字が１０００文字以上の文字が作成されたと言う事です。）
				else if (erase_moji_level <= 3 && counter > 1000)	{
					if (erase_moji_level < 3 || erase_moji_nouse_counter < counter)	{		// カウンターが大きいほど古い文字なので優先的に削除される
						hit_itr = itr;
						erase_moji_level = 3;
						erase_moji_nouse_counter = counter;
					}
				}

				// ５１２文字以上確保している
				else if (moji_map.size() >= 512)	{

					// 漢字などの文字
					if (erase_moji_level <= 2 && !itr->second.useful)	{
						if (erase_moji_level < 2 || erase_moji_kanji_counter < counter)	{		// カウンターが大きいほど古い文字なので優先的に削除される
							hit_itr = itr;
							erase_moji_level = 2;
							erase_moji_kanji_counter = counter;
						}
					}

					// ひらがななどの有益な文字
					else if (erase_moji_level <= 1)	{
						if (erase_moji_level < 1 || erase_moji_hiragana_counter < counter)	{	// カウンターが大きいほど古い文字なので優先的に削除される
							hit_itr = itr;
							erase_moji_level = 1;
							erase_moji_hiragana_counter = counter;
						}
					}
				}
			}
		}

		// 削除
		if (hit_itr != end)	{
			moji_map.erase(hit_itr);
		}
	}

	// 有益な文字
	bool useful =  (
		(_T('ぁ') <= moji && moji <= _T('ん')) ||
		_T('　') == moji ||
		_T('、') == moji ||
		_T('。') == moji ||
		_T('？') == moji ||
		_T('！') == moji ||
		_T('ー') == moji ||
		_T('…') == moji ||
		_T('‥') == moji ||
		_T('・') == moji ||
		_T('─') == moji ||		// 罫線
		_T('―') == moji ||		// ダッシュ
		_T('「') == moji ||
		_T('」') == moji ||
		_T('『') == moji ||
		_T('』') == moji ||
		_T('（') == moji ||
		_T('）') == moji ||
		_T('【') == moji ||
		_T('】') == moji ||
		_T('私') == moji ||
		_T('俺') == moji ||
		_T('僕') == moji
	) ? true : false;

	// 文字を作成する
	MOJI_PAIR pair;
	pair.first = mojist;
	pair.second.texture = create_moji(moji_type, moji, moji_size, moji_color, moji_face_type, tategaki);
	pair.second.moji_size = moji_size;
	pair.second.useful = useful;
	pair.second.load_counter = moji_load_counter;
	pair.second.texture_memory = 0;
	if (Gp_global->debug_flag)	{
		int texture_size;
		for (texture_size = 1; texture_size < moji_size; texture_size *= 2);
		pair.second.texture_memory = texture_size * texture_size * 4;
	}

	// マップに追加する
	moji_map.insert(pair);

	// 文字ロードカウンターを進める
	moji_load_counter++;

	// 最大マップ数の記憶
	int now_cnt = (int)moji_map.size();
	if (moji_map_max_cnt < now_cnt)	{
		moji_map_max_cnt = now_cnt;	
	}

	return pair.second.texture;
}

// ****************************************************************
// 文字マネージャ：文字テクスチャを作成する
// ================================================================
BSP<C_d3d_texture> C_tnm_moji_manager::create_moji(int moji_type, int moji_no, int moji_size, C_argb moji_color, MOJI_FACE_TYPE moji_face_type, bool tategaki)
{
	// 通常の文字
	if (moji_type == TNM_MOJI_TYPE_MOJI)	{

		// フォント名から文字コードセットを取得
		ARRAY<ENUMLOGFONT>::iterator itr = std::find_if(Gp_font_name_list->m_font_list.begin(), Gp_font_name_list->m_font_list.end(), C_font_name_functor(Gp_global->total_font_name));
		// フォントを作成する
		if (itr != Gp_font_name_list->m_font_list.end())	{
			Gp_font->select_font(Gp_global->total_font_name, itr->elfLogFont.lfCharSet, moji_size);
		}

		// フォントが存在しない
		static bool dont_warning_flag = false;
		while (!dont_warning_flag && Gp_font->get_h_dc() == NULL)	{
			int res = tnm_game_warning_box(LCL_STR_NOT_EXIST_FONT(Gp_global->total_font_name));
			if (res == IDNO)	{
				dont_warning_flag = true;
				break;
			}
			// ダイアログ
			C_cfg_wnd_modal_font cfg_wnd_modal_font;
			cfg_wnd_modal_font.open_modal();

			// フォント名から文字コードセットを取得
			ARRAY<ENUMLOGFONT>::iterator itr = std::find_if(Gp_font_name_list->m_font_list.begin(), Gp_font_name_list->m_font_list.end(), C_font_name_functor(Gp_global->total_font_name));
			// フォントを作成する
			if (itr != Gp_font_name_list->m_font_list.end())	{
				Gp_font->select_font(Gp_global->total_font_name, itr->elfLogFont.lfCharSet, moji_size);
			}
		}

		// テクスチャを作成する
		BSP<C_d3d_texture> texture(new C_d3d_texture);
		if (!texture->load_moji(moji_no, moji_size, moji_color, moji_face_type, tategaki, Gp_font->get_h_dc()))	{
			return BSP<C_d3d_texture>();
		}
		return texture;
	}

	// 絵文字
	else	{

		// サイズを選ぶ
		int ini_no = -1;
		int cur_size = INT_MAX;
		for (int i = 0; i < Gp_ini->emoji_cnt; i++)	{
			if (!Gp_ini->emoji[i].file_name.empty())	{

				// まだ見つかってない場合、できるだけ小さいのを選ぶ
				if (cur_size >= moji_size)	{
					if (Gp_ini->emoji[i].font_size < cur_size)	{
						ini_no = i;
						cur_size = Gp_ini->emoji[i].font_size;
					}
				}
				// 既に見つかっている場合、指定サイズ以下でできるだけ大きいものを探す
				else	{
					if (Gp_ini->emoji[i].font_size > cur_size && Gp_ini->emoji[i].font_size <= moji_size)	{
						ini_no = i;
						cur_size = Gp_ini->emoji[i].font_size;
					}
				}
			}
		}
		if (ini_no >= 0)	{
			BSP<C_d3d_album> album = tnm_load_pct_d3d(Gp_ini->emoji[ini_no].file_name, moji_face_type == MOJI_FACE_TYPE_9 ? true : false, false);
			if (!album || !album->get_texture(moji_no))	{
				return BSP<C_d3d_texture>();
			}
			else	{
				BSP<C_d3d_texture> texture = album->get_texture(moji_no);
				return texture;
			}
		}
		else	{
			return BSP<C_d3d_texture>();
		}
	}
}

// ****************************************************************
// 文字マネージャ：クリアする
// ================================================================
void C_tnm_moji_manager::clear()
{
	moji_map.clear();
}

// ****************************************************************
// 文字マネージャ：使用中の文字マップ数を取得する
// ================================================================
int C_tnm_moji_manager::get_moji_map_use_cnt()
{
	if (moji_map.empty())	{
		return 0;
	}

	int cnt = 0;
	MOJI_MAP::iterator end = moji_map.end();
	for (MOJI_MAP::iterator itr = moji_map.begin(); itr != end; ++itr)	{
		if (itr->second.texture.use_count() > 1)	{
			cnt++;
		}
	}
	return cnt;
}

// ****************************************************************
// 文字マネージャ：現在の文字マップ数を取得する
// ================================================================
int C_tnm_moji_manager::get_moji_map_now_cnt()
{
	return (int)moji_map.size();
}

// ****************************************************************
// 文字マネージャ：最大の文字マップ数を取得する
// ================================================================
int C_tnm_moji_manager::get_moji_map_max_cnt()
{
	return moji_map_max_cnt;
}

// ****************************************************************
// 文字マネージャ：使用中の文字メモリーサイズを取得する
// ================================================================
__int64 C_tnm_moji_manager::get_moji_memory_use_size()
{
	moji_memory_now_size = 0;
	moji_kind_useful_cnt = 0;
	moji_kind_unuseful_cnt = 0;

	if (moji_map.empty())	{
		return 0;
	}

	__int64 size = 0;
	MOJI_MAP::iterator end = moji_map.end();
	for (MOJI_MAP::iterator itr = moji_map.begin(); itr != end; ++itr)	{

		moji_memory_now_size += itr->second.texture_memory;

		if (itr->second.texture.use_count() > 1)	{
			size += itr->second.texture_memory;
		}

		if (itr->second.useful)	{
			moji_kind_useful_cnt++;
		}
		else	{
			moji_kind_unuseful_cnt++;
		}
	}

	// 最大メモリはココで処理する
	if (moji_memory_max_size < moji_memory_now_size)	{
		moji_memory_max_size = moji_memory_now_size;
	}

	return size;
}

// ****************************************************************
// 文字マネージャ：現在の文字メモリーサイズを取得する
// ================================================================
__int64 C_tnm_moji_manager::get_moji_memory_now_size()
{
	return moji_memory_now_size;
}

// ****************************************************************
// 文字マネージャ：最大の文字メモリーサイズを取得する
// ================================================================
__int64 C_tnm_moji_manager::get_moji_memory_max_size()
{
	return moji_memory_max_size;
}

// ****************************************************************
// 文字マネージャ：有益な文字数を取得する
// ================================================================
int C_tnm_moji_manager::get_moji_kind_useful_cnt()
{
	return moji_kind_useful_cnt;
}

// ****************************************************************
// 文字マネージャ：非有益な文字数を取得する
// ================================================================
int C_tnm_moji_manager::get_moji_kind_unuseful_cnt()
{
	return moji_kind_unuseful_cnt;
}

