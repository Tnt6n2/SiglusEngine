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

// �����݂̏����ł́A�������J�������^�C�~���O�́u������\���������v�ł���B
// �@�������P�����̕\���ɂ��ő�łP�����J������Ȃ��i�J������Ȃ��ꍇ������j
// �@�ʏ�̕����T�C�Y�ł���΁A���ɖ��ł͂Ȃ����A����ȕ����̏ꍇ�͖�肩���B
// �@����ȕ����̏ꍇ�A�g�p����Ȃ��Ȃ����Ȃ炷���ɊJ�����ׂ��ł���B
// �@�����́A�t���[�������ŏ�ɊĎ����邵���Ȃ����낤�B
// �@�T�C�Y�̑傫�ȕ����ƁA�g�p�p�x�̒Ⴂ�������J������K�v������B

// ��sel���߂̕������I���I������J������Ȃ��܂܂ł���
// �@�����炭�I���E�B���h�E������������A���ۂ̃E�B���h�E����͕������̂��폜����Ă��Ȃ�����ƍl������B

// ****************************************************************
// �����}�l�[�W��
// ================================================================
C_tnm_moji_manager	G_moji_manager;

// ****************************************************************
// �����}�l�[�W���F�����e�N�X�`�������[�h����
// ================================================================
BSP<C_d3d_texture> C_tnm_moji_manager::load_moji(int moji_type, int moji, int moji_size, C_argb moji_color, MOJI_FACE_TYPE moji_face_type, bool tategaki, int buf_no)
{
	S_moji mojist;
	mojist.moji_type = moji_type;
	mojist.moji = moji;
	mojist.size = moji_size;
	mojist.color = moji_color;
	mojist.face_type = moji_face_type;

	// ���łɑ��݂��邩�ǂ�����������
	MOJI_MAP::iterator itr = moji_map.find(mojist);

	// ���łɑ��݂���ꍇ
	if (itr != moji_map.end())	{
		itr->second.load_counter = moji_load_counter;
		return itr->second.texture;
	}

	// ���g�p�̃o�b�t�@���P�T���ĊJ������i�J�����Ȃ��ꍇ������j
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

				// moji_load_counter �͕������ǂݍ��܂�邽�тɃJ�E���g�A�b�v���Ă����܂��B�i���̊֐��̍Ō�ŏ������Ă���j
				// �J�E���g�l�͍쐬���ꂽ�����ɋL������܂��B�i���ɑ��݂���ꍇ�̏����ōė��p���ꂽ�ꍇ�ɂ��L�������j
				// ���ʓI�ɃJ�E���g�l���傫���قǐV���������ƌ������ɂȂ�B
				// �����ł́A���݂̃J�E���g�l���當���ɋL�����ꂽ�J�E���g�l�������A�쐬����Ă���̌o�ߎ��Ԃ̂悤�Ȓl���Z�o���鎖�ɂȂ�B
				// �Z�o���ꂽ�l���傫���قǌÂ������ƌ������ɂȂ�B
				// �Â������͗D��I�ɊJ������鎖�ɂȂ�B
				counter = (DWORD)(moji_load_counter - itr->second.load_counter);

				// �T�C�Y�̑傫�ȕ����i�S�W�ȏ�F���F���ۂɂ͂R�R�`�U�S�̃T�C�Y�̕����͂U�S�~�U�S�T�C�Y�̃e�N�X�`���[����������Ă���̂ŁA�R�R�ȏ�Ƃ���ׂ���������Ȃ��j
				if (itr->second.moji_size >= 48 && erase_moji_level < itr->second.moji_size)	{
					hit_itr = itr;
					erase_moji_level = itr->second.moji_size;	// �����T�C�Y�����x������Ɏg�p����i�傫�ȕ����قǗD��I�ɍ폜�����j
				}

				// �g�p����Ă��Ȃ������i�P�O�O�O�J�E���^�[�ȏ�F���̒l���Ӗ����鎖�́A�g�p����Ȃ��Ȃ��Ă���Ⴄ�������P�O�O�O�����ȏ�̕������쐬���ꂽ�ƌ������ł��B�j
				else if (erase_moji_level <= 3 && counter > 1000)	{
					if (erase_moji_level < 3 || erase_moji_nouse_counter < counter)	{		// �J�E���^�[���傫���قǌÂ������Ȃ̂ŗD��I�ɍ폜�����
						hit_itr = itr;
						erase_moji_level = 3;
						erase_moji_nouse_counter = counter;
					}
				}

				// �T�P�Q�����ȏ�m�ۂ��Ă���
				else if (moji_map.size() >= 512)	{

					// �����Ȃǂ̕���
					if (erase_moji_level <= 2 && !itr->second.useful)	{
						if (erase_moji_level < 2 || erase_moji_kanji_counter < counter)	{		// �J�E���^�[���傫���قǌÂ������Ȃ̂ŗD��I�ɍ폜�����
							hit_itr = itr;
							erase_moji_level = 2;
							erase_moji_kanji_counter = counter;
						}
					}

					// �Ђ炪�ȂȂǂ̗L�v�ȕ���
					else if (erase_moji_level <= 1)	{
						if (erase_moji_level < 1 || erase_moji_hiragana_counter < counter)	{	// �J�E���^�[���傫���قǌÂ������Ȃ̂ŗD��I�ɍ폜�����
							hit_itr = itr;
							erase_moji_level = 1;
							erase_moji_hiragana_counter = counter;
						}
					}
				}
			}
		}

		// �폜
		if (hit_itr != end)	{
			moji_map.erase(hit_itr);
		}
	}

	// �L�v�ȕ���
	bool useful =  (
		(_T('��') <= moji && moji <= _T('��')) ||
		_T('�@') == moji ||
		_T('�A') == moji ||
		_T('�B') == moji ||
		_T('�H') == moji ||
		_T('�I') == moji ||
		_T('�[') == moji ||
		_T('�c') == moji ||
		_T('�d') == moji ||
		_T('�E') == moji ||
		_T('��') == moji ||		// �r��
		_T('�\') == moji ||		// �_�b�V��
		_T('�u') == moji ||
		_T('�v') == moji ||
		_T('�w') == moji ||
		_T('�x') == moji ||
		_T('�i') == moji ||
		_T('�j') == moji ||
		_T('�y') == moji ||
		_T('�z') == moji ||
		_T('��') == moji ||
		_T('��') == moji ||
		_T('�l') == moji
	) ? true : false;

	// �������쐬����
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

	// �}�b�v�ɒǉ�����
	moji_map.insert(pair);

	// �������[�h�J�E���^�[��i�߂�
	moji_load_counter++;

	// �ő�}�b�v���̋L��
	int now_cnt = (int)moji_map.size();
	if (moji_map_max_cnt < now_cnt)	{
		moji_map_max_cnt = now_cnt;	
	}

	return pair.second.texture;
}

// ****************************************************************
// �����}�l�[�W���F�����e�N�X�`�����쐬����
// ================================================================
BSP<C_d3d_texture> C_tnm_moji_manager::create_moji(int moji_type, int moji_no, int moji_size, C_argb moji_color, MOJI_FACE_TYPE moji_face_type, bool tategaki)
{
	// �ʏ�̕���
	if (moji_type == TNM_MOJI_TYPE_MOJI)	{

		// �t�H���g�����當���R�[�h�Z�b�g���擾
		ARRAY<ENUMLOGFONT>::iterator itr = std::find_if(Gp_font_name_list->m_font_list.begin(), Gp_font_name_list->m_font_list.end(), C_font_name_functor(Gp_global->total_font_name));
		// �t�H���g���쐬����
		if (itr != Gp_font_name_list->m_font_list.end())	{
			Gp_font->select_font(Gp_global->total_font_name, itr->elfLogFont.lfCharSet, moji_size);
		}

		// �t�H���g�����݂��Ȃ�
		static bool dont_warning_flag = false;
		while (!dont_warning_flag && Gp_font->get_h_dc() == NULL)	{
			int res = tnm_game_warning_box(LCL_STR_NOT_EXIST_FONT(Gp_global->total_font_name));
			if (res == IDNO)	{
				dont_warning_flag = true;
				break;
			}
			// �_�C�A���O
			C_cfg_wnd_modal_font cfg_wnd_modal_font;
			cfg_wnd_modal_font.open_modal();

			// �t�H���g�����當���R�[�h�Z�b�g���擾
			ARRAY<ENUMLOGFONT>::iterator itr = std::find_if(Gp_font_name_list->m_font_list.begin(), Gp_font_name_list->m_font_list.end(), C_font_name_functor(Gp_global->total_font_name));
			// �t�H���g���쐬����
			if (itr != Gp_font_name_list->m_font_list.end())	{
				Gp_font->select_font(Gp_global->total_font_name, itr->elfLogFont.lfCharSet, moji_size);
			}
		}

		// �e�N�X�`�����쐬����
		BSP<C_d3d_texture> texture(new C_d3d_texture);
		if (!texture->load_moji(moji_no, moji_size, moji_color, moji_face_type, tategaki, Gp_font->get_h_dc()))	{
			return BSP<C_d3d_texture>();
		}
		return texture;
	}

	// �G����
	else	{

		// �T�C�Y��I��
		int ini_no = -1;
		int cur_size = INT_MAX;
		for (int i = 0; i < Gp_ini->emoji_cnt; i++)	{
			if (!Gp_ini->emoji[i].file_name.empty())	{

				// �܂��������ĂȂ��ꍇ�A�ł��邾���������̂�I��
				if (cur_size >= moji_size)	{
					if (Gp_ini->emoji[i].font_size < cur_size)	{
						ini_no = i;
						cur_size = Gp_ini->emoji[i].font_size;
					}
				}
				// ���Ɍ������Ă���ꍇ�A�w��T�C�Y�ȉ��łł��邾���傫�����̂�T��
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
// �����}�l�[�W���F�N���A����
// ================================================================
void C_tnm_moji_manager::clear()
{
	moji_map.clear();
}

// ****************************************************************
// �����}�l�[�W���F�g�p���̕����}�b�v�����擾����
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
// �����}�l�[�W���F���݂̕����}�b�v�����擾����
// ================================================================
int C_tnm_moji_manager::get_moji_map_now_cnt()
{
	return (int)moji_map.size();
}

// ****************************************************************
// �����}�l�[�W���F�ő�̕����}�b�v�����擾����
// ================================================================
int C_tnm_moji_manager::get_moji_map_max_cnt()
{
	return moji_map_max_cnt;
}

// ****************************************************************
// �����}�l�[�W���F�g�p���̕����������[�T�C�Y���擾����
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

	// �ő僁�����̓R�R�ŏ�������
	if (moji_memory_max_size < moji_memory_now_size)	{
		moji_memory_max_size = moji_memory_now_size;
	}

	return size;
}

// ****************************************************************
// �����}�l�[�W���F���݂̕����������[�T�C�Y���擾����
// ================================================================
__int64 C_tnm_moji_manager::get_moji_memory_now_size()
{
	return moji_memory_now_size;
}

// ****************************************************************
// �����}�l�[�W���F�ő�̕����������[�T�C�Y���擾����
// ================================================================
__int64 C_tnm_moji_manager::get_moji_memory_max_size()
{
	return moji_memory_max_size;
}

// ****************************************************************
// �����}�l�[�W���F�L�v�ȕ��������擾����
// ================================================================
int C_tnm_moji_manager::get_moji_kind_useful_cnt()
{
	return moji_kind_useful_cnt;
}

// ****************************************************************
// �����}�l�[�W���F��L�v�ȕ��������擾����
// ================================================================
int C_tnm_moji_manager::get_moji_kind_unuseful_cnt()
{
	return moji_kind_unuseful_cnt;
}

