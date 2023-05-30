#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_dir.h"
#include	"data/tnm_def.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"

#include	"engine/eng_save.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// キャプチャーがあるかを判定する
// ================================================================
bool tnm_syscom_is_exist_capture()
{
	return Gp_global->capture_prior > TNM_CAPTURE_PRIOR_NONE;
}

void tnm_syscom_create_capture(int prior, bool disp)
{
	// キャプチャーの本体がなければキャプチャーする
	if (!Gp_wnd->m_capture_texture_for_save_thumb)
	{
		Gp_global->capture_type = TNM_CAPTURE_TYPE_SAVE_THUMB;		// キャプチャーフラグを立てる
		Gp_global->capture_sorter = S_tnm_sorter(INT_MAX, INT_MAX);	// キャプチャーソーター
		Gp_global->capture_prior = prior;							// 優先度を設定する
		tnm_push_proc(TNM_PROC_TYPE_DISP);							// フレームを抜ける
	}

	// 現在のキャプチャーより優先度が高いか同じならキャプチャーする
	else if (prior >= Gp_global->capture_prior)
	{
		Gp_global->capture_type = TNM_CAPTURE_TYPE_SAVE_THUMB;		// キャプチャーフラグを立てる
		Gp_global->capture_sorter = S_tnm_sorter(INT_MAX, INT_MAX);	// キャプチャーソーター
		Gp_global->capture_prior = prior;							// 優先度を設定する
		tnm_push_proc(TNM_PROC_TYPE_DISP);							// フレームを抜ける
	}
}

// キャプチャーを解放する
void tnm_syscom_free_capture(int prior)
{
	// 現在のキャプチャーより優先度が高いか同じならキャプチャーを解放する
	if (prior >= Gp_global->capture_prior)	{
		Gp_wnd->m_capture_texture_for_save_thumb.reset();			// キャプチャーを解放する
		Gp_global->capture_prior = TNM_CAPTURE_PRIOR_NONE;			// 優先度をリセット
	}
}

// ツイート用のキャプチャーを準備する
void tnm_syscom_create_capture_for_tweet()
{
	Gp_global->capture_type = TNM_CAPTURE_TYPE_TWEET;				// キャプチャーフラグを立てる
	Gp_global->capture_sorter = S_tnm_sorter(INT_MAX, INT_MAX);		// キャプチャーソーター
	tnm_push_proc(TNM_PROC_TYPE_DISP);								// フレームを抜ける
}

// ツイート用のキャプチャーを解放する
void tnm_syscom_free_capture_for_tweet()
{
	Gp_wnd->m_capture_texture_for_tweet.reset();					// キャプチャーを解放する
}

// オブジェクト用のキャプチャーを準備する
void tnm_syscom_create_capture_for_object(S_tnm_sorter sorter, bool disp)
{
	Gp_global->capture_type = TNM_CAPTURE_TYPE_OBJECT;	// キャプチャーフラグを立てる
	Gp_global->capture_sorter = sorter;					// キャプチャーソーター
	Gp_global->present_flag = disp;						// 画面更新
	tnm_push_proc(TNM_PROC_TYPE_DISP);					// フレームを抜ける
}

// オブジェクト用のキャプチャーを解放する
void tnm_syscom_free_capture_for_object()
{
	Gp_wnd->m_capture_texture_for_object.reset();
}

// ローカルセーブ用のキャプチャーを準備する
void tnm_syscom_create_capture_for_local_save(S_tnm_sorter sorter, int width, int height, bool disp)
{
	Gp_global->capture_type = TNM_CAPTURE_TYPE_THUMB;		// キャプチャーフラグを立てる
	Gp_global->capture_sorter = sorter;						// キャプチャーソーター
	Gp_global->present_flag = disp;							// 画面更新
	Gp_global->capture_time = 0;							// 時刻を初期化
	Gp_global->capture_w = width;
	Gp_global->capture_h = height;
	tnm_push_proc(TNM_PROC_TYPE_DISP);						// フレームを抜ける
}

// キャプチャーバッファを作成
void tnm_syscom_create_capture_buffer(int width, int height)
{
	Gp_wnd->create_capture_buffer(width, height);
}

// キャプチャーバッファを破壊
void tnm_syscom_destroy_capture_buffer()
{
	Gp_wnd->destroy_capture_buffer();
}

// バッファキャプチャーを行う
void tnm_syscom_buffer_capture(int x, int y, CTSTR& file_name)
{
	Gp_global->capture_type = TNM_CAPTURE_TYPE_BUFFER;			// キャプチャーフラグを立てる
	Gp_global->capture_sorter = S_tnm_sorter(INT_MAX, INT_MAX);	// キャプチャーソーター
	Gp_global->capture_x = x;									// キャプチャー先座標Ｘ
	Gp_global->capture_y = y;									// キャプチャー先座標Ｙ
	Gp_global->capture_file_name = file_name;					// キャプチャー先ファイル名
	Gp_global->present_flag = false;							// 画面を更新
	tnm_push_proc(TNM_PROC_TYPE_DISP);							// フレームを抜ける
}

// キャプチャーバッファ構造体
struct S_capture_buffer_option_header
{
	int header_size;
	int flag_offset;
	int flag_size;
	int flag_cnt;
	int str_flag_offset;
	int str_flag_size;
	int str_flag_cnt;
};

// キャプチャーバッファをファイルに保存
bool tnm_syscom_save_capture_buffer_to_file(CTSTR& file_name, TSTR extension, bool dialog, CTSTR& dialog_title, S_element* p_flag_element, int start_flag, int flag_cnt, S_element* p_str_flag_element, int start_str_flag, int str_flag_cnt)
{
	extension = str_to_lower_ret(extension);

	if (!Gp_wnd->m_capture_temp_buffer_texture)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーバッファーがありません。"));
		return false;
	}
	if (extension != _T("bmp") && extension != _T("png"))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーの拡張子は bmp または png のみです。"));
		return false;
	}
	if (extension == _T("png") && flag_cnt > 0)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーを png で保存する場合、フラグを保存することはできません。"));
		return false;
	}

	// 保存パス
	TSTR file_path;
	if (dialog)
	{
		if (extension == _T("bmp"))
		{
			C_select_file_dialog sfd;
			sfd.add_filter(_T("BMP Files"), _T("*.bmp"));
			file_path = sfd.select_save_file(dialog_title, file_name + _T(".bmp"));
		}
		if (extension == _T("png"))
		{
			C_select_file_dialog sfd;
			sfd.add_filter(_T("PNG Files"), _T("*.png"));
			file_path = sfd.select_save_file(dialog_title, file_name + _T(".png"));
		}
	}
	else
	{
		if (extension == _T("bmp"))
		{
			file_path = Gp_dir->save_dir + _T("\\") + file_name + _T(".bmp");
		}
		if (extension == _T("png"))
		{
			file_path = Gp_dir->save_dir + _T("\\") + file_name + _T(".png");
		}
	}

	// 保存パスがある場合
	if (!file_path.empty())
	{
		// 指定のファイルに保存
		D3DLOCKED_RECT locked_rect;
		if (!Gp_wnd->m_capture_temp_buffer_texture->lock_rect(0, &locked_rect, NULL, D3DLOCK_DISCARD))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーバッファのロックに失敗しました。\n") + get_last_error_msg());
		}
		else
		{
			// テクスチャサイズ
			int width = Gp_wnd->m_capture_temp_buffer_texture->get_width();
			int height = Gp_wnd->m_capture_temp_buffer_texture->get_height();

			if (extension == _T("bmp"))
			{
				BUFFER image_buffer;
				S_capture_buffer_option_header option_header = { 0 };
				C_stream option_body;

				option_header.header_size = sizeof(S_capture_buffer_option_header);

				// bmp を作成
				tnm_create_bmp_from_texture(image_buffer, width, height, &locked_rect);

				// ロックを解除
				Gp_wnd->m_capture_temp_buffer_texture->unlock_rect(0);

				// フラグを追加
				option_header.flag_offset = sizeof(S_capture_buffer_option_header) + option_body.size();
					
				if (p_flag_element && flag_cnt > 0)
				{
					C_elm_int_list* p_flag = (C_elm_int_list *)tnm_get_element_ptr(p_flag_element->begin(), p_flag_element->end(), true);

					int end_flag = start_flag + flag_cnt;

					start_flag = std::max(start_flag, 0);
					end_flag = std::min(end_flag, p_flag->get_size());
					flag_cnt = end_flag - start_flag;

					if (flag_cnt > 0)
					{
						option_body.push((void *)(p_flag->get_ptr() + start_flag), sizeof(int) * flag_cnt);

						option_header.flag_cnt = flag_cnt;
						option_header.flag_size = sizeof(S_capture_buffer_option_header) + option_body.size() - option_header.flag_offset;
					}
				}

				// 文字列フラグを追加
				option_header.str_flag_offset = sizeof(S_capture_buffer_option_header) + option_body.size();

				if (p_str_flag_element && str_flag_cnt > 0)
				{
					C_elm_str_list* p_flag = (C_elm_str_list *)tnm_get_element_ptr(p_str_flag_element->begin(), p_str_flag_element->end(), true);

					int end_str_flag = start_str_flag + str_flag_cnt;

					start_str_flag = std::max(start_str_flag, 0);
					end_str_flag = std::min(end_str_flag, p_flag->get_size());
					str_flag_cnt = end_str_flag - start_str_flag;

					if (str_flag_cnt > 0)
					{
						for (int i = 0; i < str_flag_cnt; i++)
						{
							option_body.push_str_null(p_flag->get_value(start_str_flag + i));
						}

						option_header.str_flag_cnt = str_flag_cnt;
						option_header.str_flag_size = sizeof(S_capture_buffer_option_header) + option_body.size() - option_header.str_flag_offset;
					}
				}

				// bmp にフラグをくっつける
				int old_size = (int)image_buffer.size();
				image_buffer.resize(old_size + sizeof(option_header) + option_body.size());
				memcpy(image_buffer.get() + old_size, &option_header, sizeof(option_header));
				memcpy(image_buffer.get() + old_size + sizeof(option_header), option_body.get(), option_body.size());

				// ファイルに保存
				if (!C_file::write_full_data(file_path, image_buffer))
				{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーバッファの保存に失敗しました。\n") + get_last_error_msg());
				}
				else
				{
					return true;		// 成功！
				}
			}
			if (extension == _T("png"))
			{
				// ファイルに保存
				if (!tnm_create_png_from_texture_and_save_to_file(file_path, width, height, &locked_rect, true))
				{
					Gp_wnd->m_capture_temp_buffer_texture->unlock_rect(0);

					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーバッファの保存に失敗しました。\n"));
				}
				else
				{
					Gp_wnd->m_capture_temp_buffer_texture->unlock_rect(0);

					return true;		// 成功！
				}
			}
		}
	}

	return false;
}

// フラグをキャプチャーファイルから取得
bool tnm_syscom_load_flag_from_capture_file(CTSTR& file_name, TSTR extension, bool dialog, CTSTR& dialog_title, S_element* p_flag_element, int start_flag, int flag_cnt, S_element* p_str_flag_element, int start_str_flag, int str_flag_cnt)
{
	extension = str_to_lower_ret(extension);

	if (extension != _T("bmp") && extension != _T("png"))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーの拡張子は bmp または png のみです。"));
		return false;
	}
	if (extension == _T("png"))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーフラグは bmp からしか読み取ることはできません。"));
		return false;
	}

	// 読み込みパス
	TSTR file_path;
	if (dialog)
	{
		if (extension == _T("bmp"))
		{
			C_select_file_dialog sfd;
			sfd.add_filter(_T("BMP Files"), _T("*.bmp"));
			file_path = sfd.select_open_file(dialog_title, file_name);
		}
	}
	else
	{
		if (extension == _T("bmp"))
		{
			file_path = Gp_dir->save_dir + _T("\\") + file_name + _T(".bmp");
		}
	}

	// パスを選択させる
	if (!file_path.empty())
	{
		C_file file;
		if (!file.open(file_path, _T("rb")))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("キャプチャーファイルが開けませんでした。\n") + get_last_error_msg());
		}
		else
		{
			UINT64 file_size = file.get_size();

			DWORD bmp_size = 0;
			file.seek(2, SEEK_SET);
			file.read((void *)&bmp_size, sizeof(DWORD));
			file.seek(bmp_size, SEEK_SET);

			int option_size = (int)(file_size - bmp_size);
			if (option_size > sizeof(S_capture_buffer_option_header))
			{
				S_capture_buffer_option_header option_header = { 0 };
				file.read(&option_header, sizeof(S_capture_buffer_option_header));

				int option_body_size = option_size - sizeof(S_capture_buffer_option_header);

				C_stream option_body;
				option_body.resize(option_body_size);
				file.read(option_body.get(), option_body_size);

				// フラグを取得
				if (p_flag_element && flag_cnt > 0)
				{
					C_elm_int_list* p_flag = (C_elm_int_list *)tnm_get_element_ptr(p_flag_element->begin(), p_flag_element->end(), true);

					flag_cnt = std::min(flag_cnt, option_header.flag_cnt);

					int end_flag = start_flag + flag_cnt;
					start_flag = std::max(start_flag, 0);
					end_flag = std::min(end_flag, p_flag->get_size());
					flag_cnt = end_flag - start_flag;
					if (flag_cnt > 0)
					{
						int read_size = std::min((int)sizeof(int) * flag_cnt, option_body_size);
						memcpy(p_flag->get_ptr() + start_flag, option_body.get(), read_size);
					}
				}

				// フラグ分を進める
				option_body.add_seek(sizeof(int) * flag_cnt);

				// 文字列フラグを取得
				if (p_str_flag_element && str_flag_cnt > 0)
				{
					C_elm_str_list* p_str_flag = (C_elm_str_list *)tnm_get_element_ptr(p_str_flag_element->begin(), p_str_flag_element->end(), true);

					str_flag_cnt = std::min(str_flag_cnt, option_header.str_flag_cnt);

					int end_str_flag = start_str_flag + str_flag_cnt;
					start_str_flag = std::max(start_str_flag, 0);
					end_str_flag = std::min(end_str_flag, p_str_flag->get_size());
					str_flag_cnt = end_str_flag - start_str_flag;
					if (str_flag_cnt > 0)
					{
						for (int i = 0; i < str_flag_cnt; i++)
						{
							p_str_flag->set_value(start_str_flag + i, option_body.pop_str_null_ret<TSTR>());
						}
					}
				}
			}

			return true;
		}
	}

	return false;
}





