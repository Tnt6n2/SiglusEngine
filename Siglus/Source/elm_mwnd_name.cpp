#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_sorter.h"
#include	"data/tnm_config_data.h"

#include	"element/elm_mwnd_name.h"
#include	"element/elm_mwnd_moji.h"

#include	"engine/eng_config.h"
#include	"engine/eng_message.h"

// ****************************************************************
// �l�v�F���O�F������
// ================================================================
void C_elm_mwnd_name::init()
{
	// �e���v���[�g�ԍ�
	m_template_no = -1;

	// �e���v���[�g�̐ݒ�
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_size = 0;
	m_def.moji_space = C_size(0, 0);
	m_def.moji_cnt = 0;
	m_def.moji_align = 0;
	m_def.moji_color_no = 0;
	m_def.shadow_color_no = 0;
	m_def.fuchi_color_no = 0;

	// �����ϐ�������
	init_work_variable();
}

// ****************************************************************
// �l�v�F���O�F�e���v���[�g�̒l��ݒ�
// ================================================================
void C_elm_mwnd_name::set_template(int template_no)
{
	// �e���v���[�g�ԍ�
	m_template_no = template_no;

	// �e���v���[�g���č\�z
	restruct_template();

	// �����ϐ�������
	init_work_variable();
}

// ****************************************************************
// �l�v�F���O�F�e���v���[�g���č\�z
// ================================================================
void C_elm_mwnd_name::restruct_template()
{
	// �e���v���[�g�̐ݒ�
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_size = Gp_ini->mwnd.sub[m_template_no].name_moji_size;
	m_def.moji_space = Gp_ini->mwnd.sub[m_template_no].name_moji_space;
	m_def.moji_cnt = Gp_ini->mwnd.sub[m_template_no].name_moji_cnt;
	m_def.moji_align = Gp_ini->mwnd.sub[m_template_no].name_window_align;
	m_def.moji_color_no = Gp_ini->mwnd.sub[m_template_no].name_moji_color >= 0 ? Gp_ini->mwnd.sub[m_template_no].name_moji_color : Gp_ini->mwnd.moji_color;
	m_def.shadow_color_no = Gp_ini->mwnd.sub[m_template_no].name_shadow_color >= 0 ? Gp_ini->mwnd.sub[m_template_no].name_shadow_color : Gp_ini->mwnd.shadow_color;
	m_def.fuchi_color_no = Gp_ini->mwnd.sub[m_template_no].name_fuchi_color >= 0 ? Gp_ini->mwnd.sub[m_template_no].name_fuchi_color : Gp_ini->mwnd.fuchi_color;
}

// ****************************************************************
// �l�v�F���O�F�����ϐ�������
// ================================================================
void C_elm_mwnd_name::init_work_variable()
{
	m_cur = m_def;					// �f�t�H���g�p�����[�^�����݂̃p�����[�^�փR�s�[����
	m_cur.moji_color_no = -1;		// �u�f�t�H���g�ɏ]���v
	m_cur.shadow_color_no = -1;		// �u�f�t�H���g�ɏ]���v
	m_cur.fuchi_color_no = -1;		// �u�f�t�H���g�ɏ]���v

	init_work_variable_sub();
}

void C_elm_mwnd_name::init_work_variable_sub()
{
	m_chara_moji_color_no = -1;		// �f�t�H���g�ɏ]��
	m_chara_shadow_color_no = -1;	// �f�t�H���g�ɏ]��
	m_chara_fuchi_color_no = -1;	// �f�t�H���g�ɏ]��

	m_name.clear();
	m_moji_list.clear();
}

// ****************************************************************
// �l�v�F���O�F�Z�[�u
// ================================================================
void C_elm_mwnd_name::save(C_tnm_save_stream& stream)
{
	// �e���v���[�g�ԍ�
	stream.save(m_template_no);

	// ���݂̃p�����[�^
	stream.save(m_cur);
	stream.save(m_name);
	stream.save(m_msg_rect);

	// ����
	stream.save(m_moji_list);
}

// ****************************************************************
// �l�v�F���O�F���[�h
// ================================================================
void C_elm_mwnd_name::load(C_tnm_save_stream& stream)
{
	// �e���v���[�g�ԍ�
	stream.load(m_template_no);

	// �e���v���[�g���č\�z
	restruct_template();

	// ���݂̃p�����[�^
	stream.load(m_cur);
	stream.load(m_name);
	stream.load(m_msg_rect);

	// ����
	stream.load(m_moji_list);
}

// ****************************************************************
// �l�v�F���O�F�R�s�[
// ================================================================
void C_elm_mwnd_name::copy(C_elm_mwnd_name* src)
{
	// �e���v���[�g�ԍ�
	m_template_no = src->m_template_no;
		
	// �e���v���[�g���č\�z
	restruct_template();

	// ���݂̃p�����[�^
	m_cur = src->m_cur;
	m_name = src->m_name;
	m_msg_rect = src->m_msg_rect;
	
	// ����
	m_moji_list.resize(src->m_moji_list.size());
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i] = src->m_moji_list[i];
}

// ****************************************************************
// �l�v�F���O�F�t���[������
// ================================================================
void C_elm_mwnd_name::frame_init()
{
	m_spr_moji_root.rp.init();
	m_spr_shadow_root.rp.init();

	// ���ׂĂ̕����ɑ΂��ăt���[��������
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].frame_init();
}

void C_elm_mwnd_name::frame(const S_tnm_render_param* parent_trp, C_point pos_rep, bool draw_futoku, bool draw_shadow)
{
	// ���ׂĂ̕����ɑ΂��ăt���[������
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].frame(parent_trp, pos_rep, 0, 0, C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), draw_futoku, draw_shadow);
}

// ****************************************************************
// �l�v�F���O�F�X�v���C�g�c���[���\�z
// ================================================================
void C_elm_mwnd_name::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].get_sprite_tree(root, root);
}

// ****************************************************************
// �l�v�F���O�F�󂩂ǂ�������
// ================================================================
bool C_elm_mwnd_name::is_empty()
{
	return m_moji_list.empty();
}

// ****************************************************************
// �l�v�F���O�F���O���N���A
// ================================================================
void C_elm_mwnd_name::clear_name()
{
	m_cur.moji_pos = m_def.moji_pos;		// �f�t�H���g�̕����ʒu

	init_work_variable_sub();
}

// ****************************************************************
// �l�v�F���O�F���O�̎擾
// ================================================================
TSTR C_elm_mwnd_name::get_name()
{
	return m_name;
}

// ****************************************************************
// �l�v�F���O�F���O�̐ݒ�
// ================================================================

const int	NAME_CODE_MOJI = 0;
const int	NAME_CODE_SIZE = 1;
const int	NAME_CODE_SIZE_DEFAULT = 2;
const int	NAME_CODE_COLOR = 3;
const int	NAME_CODE_COLOR_DEFAULT = 4;
const int	NAME_CODE_X = 5;
const int	NAME_CODE_Y = 6;
const int	NAME_CODE_ADD_X = 7;
const int	NAME_CODE_ADD_Y = 8;
const int	NAME_CODE_UNKNOWN = 99;

struct S_name_next_code
{
	int		type;
	int		option;
};

static TSTR::const_iterator name_get_next_code(TSTR::const_iterator itr, TSTR::const_iterator end, S_name_next_code* ret_code)
{
	int num = 0;

	if (itr == end)
		return end;

	if (str_analize_check_moji(itr, end, _T('#')))	{

		if (itr == end)	{
			ret_code->type = NAME_CODE_MOJI;
			ret_code->option = _T('#');
		}
		else if (str_analize_check_moji(itr, end, _T('#')))	{
			ret_code->type = NAME_CODE_MOJI;
			ret_code->option = _T('#');
		}
		else if (str_analize_check_moji(itr, end, _T('S')))	{
			ret_code->type = NAME_CODE_SIZE_DEFAULT;
		}
		else if (str_analize_check_moji(itr, end, _T('C')))	{
			ret_code->type = NAME_CODE_COLOR_DEFAULT;
		}
		else if (str_analize_get_integer(itr, end, &num))	{

			if (itr == end)	{
				ret_code->type = NAME_CODE_MOJI;
				ret_code->option = _T('#');
			}
			else if (str_analize_check_moji(itr, end, _T('S')))	{
				ret_code->type = NAME_CODE_SIZE;
				ret_code->option = num;
			}
			else if (str_analize_check_moji(itr, end, _T('C')))	{
				ret_code->type = NAME_CODE_COLOR;
				ret_code->option = num;
			}
			else if (str_analize_check_moji(itr, end, _T('X')))	{
				ret_code->type = NAME_CODE_X;
				ret_code->option = num;
			}
			else if (str_analize_check_moji(itr, end, _T('Y')))	{
				ret_code->type = NAME_CODE_Y;
				ret_code->option = num;
			}
			else if (str_analize_check_str(itr, end, _T("RX")))	{
				ret_code->type = NAME_CODE_ADD_X;
				ret_code->option = num;
			}
			else if (str_analize_check_str(itr, end, _T("RY")))	{
				ret_code->type = NAME_CODE_ADD_Y;
				ret_code->option = num;
			}
		}
	}
	else	{
		ret_code->type = NAME_CODE_UNKNOWN;	// �C�e���[�^��i�߂Ȃ�
	}

	return itr;
}

void C_elm_mwnd_name::set_name(CTSTR& name)
{
	if (name.empty())
		return;

	// ����̕����J���[
	int cur_moji_size = m_cur.moji_size;
	int cur_moji_color_no = m_cur.moji_color_no;

	for (TSTR::const_iterator itr = name.begin(); itr != name.end();)	{
		int moji_type = TNM_MOJI_TYPE_MOJI;
		int cmd_code = 0;

		// �R�[�h���擾
		S_name_next_code next_code;
		itr = name_get_next_code(itr, name.end(), &next_code);

		// ����R�[�h�F�F�ύX�̏ꍇ
		if (next_code.type == NAME_CODE_SIZE)	{
			cur_moji_size = next_code.option;
		}
		else if (next_code.type == NAME_CODE_SIZE_DEFAULT)	{
			cur_moji_size = m_cur.moji_size;
		}
		else if (next_code.type == NAME_CODE_COLOR)	{
			cur_moji_color_no = next_code.option;
		}
		else if (next_code.type == NAME_CODE_COLOR_DEFAULT)	{
			cur_moji_color_no = m_cur.moji_color_no;
		}
		else if (next_code.type == NAME_CODE_X)	{
			m_cur.moji_pos.x = next_code.option;
		}
		else if (next_code.type == NAME_CODE_Y)	{
			m_cur.moji_pos.y = next_code.option;
		}
		else if (next_code.type == NAME_CODE_ADD_X)	{
			m_cur.moji_pos.x += next_code.option;
		}
		else if (next_code.type == NAME_CODE_ADD_Y)	{
			m_cur.moji_pos.y += next_code.option;
		}
		else	{

			// �s���ȃR�[�h�łȂ疼�O�t���O����
			if (next_code.type == NAME_CODE_UNKNOWN)	{
				next_code.type = NAME_CODE_MOJI;

				// ���O�t���O��W�J�i�C�e���[�^�i�݂܂��j
				tnm_msg_proc_expand_name_flag(itr, name.end(), &moji_type, &next_code.option);
			}

			// �����R�[�h�Ȃ當�����쐬�i## �̏ꍇ��������ʂ邱�Ƃɒ��Ӂj
			if (next_code.type == NAME_CODE_MOJI)	{

				// �������쐬
				C_elm_mwnd_moji moji;
				int moji_color_no = cur_moji_color_no != -1 ? cur_moji_color_no : (Gp_config->message_chrcolor_flag && m_chara_moji_color_no != -1) ? m_chara_moji_color_no : m_def.moji_color_no;
				int shadow_color_no = m_cur.shadow_color_no != -1 ? m_cur.shadow_color_no : (Gp_config->message_chrcolor_flag && m_chara_shadow_color_no != -1) ? m_chara_shadow_color_no : m_def.shadow_color_no;
				int fuchi_color_no = m_cur.fuchi_color_no != -1 ? m_cur.fuchi_color_no : (Gp_config->message_chrcolor_flag && m_chara_fuchi_color_no != -1) ? m_chara_fuchi_color_no : m_def.fuchi_color_no;

				moji.set_moji(moji_type, next_code.option, cur_moji_size, moji_color_no, shadow_color_no, fuchi_color_no, m_cur.moji_pos);
				moji.set_appear(true);	// �f�t�H���g�ŕ\��

				// ���̕����ʒu�̐ݒ�
				if (moji_type != TNM_MOJI_TYPE_MOJI)			m_cur.moji_pos.x += cur_moji_size + m_cur.moji_space.cx;
				else if (is_hankaku((TCHAR)next_code.option))	m_cur.moji_pos.x += (cur_moji_size + m_cur.moji_space.cx) / 2;
				else											m_cur.moji_pos.x += cur_moji_size + m_cur.moji_space.cx;

				// �������X�g�ɒǉ�
				m_moji_list.push_back(moji);
			}
		}
	}

	int name_width = m_cur.moji_pos.x - m_cur.moji_space.cx;

	// �ʒu�𑵂���
	if (false);
	else if (m_def.moji_align == 0)	{

		// �T�C�Y���v�Z
		m_msg_rect.left = 0;
		m_msg_rect.top = 0;
		m_msg_rect.right = name_width;
		m_msg_rect.bottom = m_cur.moji_size;
	}
	else if (m_def.moji_align == 1)	{	// ����
		for (int i = 0; i < (int)m_moji_list.size(); i++)	{
			m_moji_list[i].set_moji_pos(m_moji_list[i].get_moji_pos() - C_point(name_width / 2, 0));
		}

		// �T�C�Y���v�Z
		m_msg_rect.left = - name_width / 2;
		m_msg_rect.top = 0;
		m_msg_rect.right = name_width / 2;
		m_msg_rect.bottom = m_cur.moji_size;
	}
	else if (m_def.moji_align == 2)	{	// �E����
		for (int i = 0; i < (int)m_moji_list.size(); i++)	{
			m_moji_list[i].set_moji_pos(m_moji_list[i].get_moji_pos() - C_point(name_width, 0));
		}

		// �T�C�Y���v�Z
		m_msg_rect.left = - name_width;
		m_msg_rect.top = 0;
		m_msg_rect.right = 0;
		m_msg_rect.bottom = m_cur.moji_size;
	}

	// ���O���L��
	m_name = name;
}

// ****************************************************************
// �l�v�F���O�F�L�����N�^�����F��ݒ�
// ================================================================
void C_elm_mwnd_name::clear_chara_moji_color()
{
	m_chara_moji_color_no = -1;
}
void C_elm_mwnd_name::clear_chara_shadow_color()
{
	m_chara_shadow_color_no = -1;
}
void C_elm_mwnd_name::clear_chara_fuchi_color()
{
	m_chara_fuchi_color_no = -1;
}

// ****************************************************************
// �l�v�F���O�F�L�����N�^�����F��ݒ�
// ================================================================
void C_elm_mwnd_name::set_chara_moji_color(int color_no)
{
	m_chara_moji_color_no = color_no;
}
void C_elm_mwnd_name::set_chara_shadow_color(int color_no)
{
	m_chara_shadow_color_no = color_no;
}
void C_elm_mwnd_name::set_chara_fuchi_color(int color_no)
{
	m_chara_fuchi_color_no = color_no;
}

// ****************************************************************
// �l�v�F���O�F���b�Z�[�W�T�C�Y�̎擾
// ================================================================
C_rect C_elm_mwnd_name::get_msg_rect()
{
	return m_msg_rect;
}
