#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"element/elm_stage.h"
#include	"engine/eng_etc.h"

// ****************************************************************
// �X�e�[�W�F������
// ================================================================
void C_elm_stage::init(S_element element, CTSTR& name, bool use_ini)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_STAGE, name, NULL);

	// �O���[�v���X�g��������
	m_group_list.init(element + S_element(ELM_STAGE_OBJBTNGROUP), name + _T(".group"), Gp_ini->button.group_cnt, false, element + S_element(ELM_STAGE_OBJECT));

	// �I�u�W�F�N�g���X�g��������
	m_object_list.init(element + S_element(ELM_STAGE_OBJECT), name + _T(".object"), Gp_ini->object_cnt, false, use_ini, true, true, this, S_element(ELM_UP, ELM_STAGE_OBJBTNGROUP), S_element(ELM_UP, ELM_STAGE_WORLD));

	// ���b�Z�[�W�E�B���h�E��������
	m_mwnd_list.init(element + S_element(ELM_STAGE_MWND), name + _T(".mwnd"), Gp_ini->mwnd.sub_cnt, false, this, S_element(ELM_UP, ELM_STAGE_OBJBTNGROUP));

	// �{�^���I������������
	m_btn_sel.init(element + S_element(ELM_STAGE_BTNSELITEM), name + _T(".btnselitem"));
	
	// ���[���h��������
	m_world_list.init(element + S_element(ELM_STAGE_WORLD), FM_WORLDLIST, name + _T(".world"), Gp_ini->world_cnt, false, NULL);

	// �G�t�F�N�g��������
	m_effect_list.init(element + S_element(ELM_STAGE_EFFECT), FM_EFFECTLIST, name + _T(".effect"), Gp_ini->effect_cnt, false, NULL);

	// �N�G�C�N��������
	m_quake_list.init(element + S_element(ELM_STAGE_QUAKE), FM_QUAKELIST, name + _T(".quake"), Gp_ini->quake_cnt, false, NULL);

	// �ď�����
	reinit(true);
}

// ****************************************************************
// �X�e�[�W�F�ď�����
// ================================================================
void C_elm_stage::reinit(bool restruct_flag)
{
	// �O���[�v���X�g���ď�����
	m_group_list.reinit();

	// �I�u�W�F�N�g���X�g���ď�����
	m_object_list.reinit(restruct_flag);

	// ���b�Z�[�W�E�B���h�E���ď�����
	m_mwnd_list.reinit(restruct_flag);

	// �{�^���I�������ď�����
	m_btn_sel.reinit();

	// ���[���h���ď�����
	m_world_list.reinit();

	// �G�t�F�N�g���ď�����
	m_effect_list.reinit();

	// �N�G�C�N���ď�����
	m_quake_list.reinit();
}

// ****************************************************************
// �X�e�[�W�F�I��
// ================================================================
void C_elm_stage::finish()
{
	// �I�u�W�F�N�g
	m_object_list.finish();

	// ���b�Z�[�W�E�B���h�E
	m_mwnd_list.finish();

	// �{�^���I����
	m_btn_sel.finish();
}

// ****************************************************************
// �X�e�[�W�F�Z�[�u
// ================================================================
void C_elm_stage::save(C_tnm_save_stream& stream)
{
	// �O���[�v
	m_group_list.save(stream);

	// �I�u�W�F�N�g
	m_object_list.save(stream);

	// ���b�Z�[�W�E�B���h�E
	m_mwnd_list.save(stream);

	// �{�^���I����
	m_btn_sel.save(stream);

	// ���[���h
	m_world_list.save(stream);

	// �G�t�F�N�g
	m_effect_list.save(stream);

	// �N�G�C�N
	m_quake_list.save(stream);
}

// ****************************************************************
// �X�e�[�W�F���[�h
// ================================================================
void C_elm_stage::load(C_tnm_save_stream& stream)
{
	// �O���[�v
	m_group_list.load(stream);

	// �I�u�W�F�N�g
	m_object_list.load(stream);

	// ���b�Z�[�W�E�B���h�E
	m_mwnd_list.load(stream);

	// �{�^���I����
	m_btn_sel.load(stream);

	// ���[���h
	m_world_list.load(stream);

	// �G�t�F�N�g
	m_effect_list.load(stream);

	// �N�G�C�N
	m_quake_list.load(stream);
}

// ****************************************************************
// �X�e�[�W�F�t���[������������
// ================================================================
void C_elm_stage::frame_init()
{
	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
	{
		if (m_object_list[i].is_use())
			m_object_list[i].frame_init();
	}

	// ���b�Z�[�W�E�B���h�E
	int mwnd_cnt = (int)m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)
		m_mwnd_list[i].frame_init();

	// �{�^���I����
	m_btn_sel.frame_init();
}

// ****************************************************************
// �X�e�[�W�F���Ԃ��X�V
// ================================================================
void C_elm_stage::update_time(int past_game_time, int past_real_time)
{
	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
	{
		if (m_object_list[i].is_use())
			m_object_list[i].update_time(past_game_time, past_real_time, 0);
	}

	// ���b�Z�[�W�E�B���h�E
	int mwnd_cnt = m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)
		m_mwnd_list[i].update_time(past_game_time, past_real_time);

	// �{�^���I����
	m_btn_sel.update_time(past_game_time, past_real_time);

	// ���[���h
	int world_cnt = m_world_list.get_size();
	for (int i = 0; i < world_cnt; i++)
		m_world_list[i].update_time(past_game_time, past_real_time);

	// �G�t�F�N�g
	int effect_cnt = m_effect_list.get_size();
	for (int i = 0; i < effect_cnt; i++)
		m_effect_list[i].update_time(past_game_time, past_real_time);

	// �N�G�C�N
	int quake_cnt = m_quake_list.get_size();
	for (int i = 0; i < quake_cnt; i++)
		m_quake_list[i].update_time(past_game_time);
}

// ****************************************************************
// �X�e�[�W�F�t���[������
// ================================================================
bool C_elm_stage::frame(bool draw_futoku, bool draw_shadow, int order)
{
	// ���[���h�i�K���I�u�W�F�N�g����I�j
	int world_cnt = m_world_list.get_size();
	for (int i = 0; i < world_cnt; i++)
		m_world_list[i].frame();

	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
	{
		if (m_object_list[i].is_use())
			m_object_list[i].frame(NULL, order, 0);
	}

	// ���b�Z�[�W�E�B���h�E
	int mwnd_cnt = m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)
	{
		if (!m_mwnd_list[i].frame(draw_futoku, draw_shadow))
			return false;
	}

	// �{�^���I����
	m_btn_sel.frame(draw_futoku, draw_shadow);

	// �G�t�F�N�g
	int effect_cnt = m_effect_list.get_size();
	for (int i = 0; i < effect_cnt; i++)
		m_effect_list[i].frame();

	// �N�G�C�N
	int quake_cnt = m_quake_list.get_size();
	for (int i = 0; i < quake_cnt; i++)
		m_quake_list[i].frame();

	return true;
}

// ****************************************************************
// �X�e�[�W�F�t���[���A�N�V��������
// ================================================================
void C_elm_stage::frame_action()
{
	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		if (m_object_list[i].is_use())
			m_object_list[i].do_frame_action();
	}

	// ���b�Z�[�W�E�B���h�E
	int mwnd_cnt = m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)
		m_mwnd_list[i].do_frame_action();
}

// ****************************************************************
// �X�e�[�W�F�{�^���t���[������������
// ================================================================
void C_elm_stage::button_frame_init()
{
	// �O���[�v
	int group_cnt = m_group_list.get_size();
	for (int i = 0; i < group_cnt; i++)
		m_group_list[i].button_frame_init();
}

// ****************************************************************
// �X�e�[�W�F�{�^���o�^����
// ================================================================
void C_elm_stage::regist_button(int min_order)
{
	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
	{
		if (m_object_list[i].is_use())
		{
			m_object_list[i].regist_button(min_order);
		}
	}

	// ���b�Z�[�W�E�B���h�E
	if (!tnm_is_hide_mwnd())
	{
		int mwnd_cnt = m_mwnd_list.get_size();
		for (int i = 0; i < mwnd_cnt; i++)
		{
			m_mwnd_list[i].regist_button();
		}
	}

	// �{�^���I����
	if (!tnm_is_hide_mwnd())
	{
		m_btn_sel.regist_button();
	}
}

// ****************************************************************
// �X�e�[�W�F�{�^���C�x���g����
// ================================================================
void C_elm_stage::button_event(C_elm_mwnd* p_sel_target_mwnd)
{
	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		if (m_object_list[i].is_use())
			m_object_list[i].button_event();
	}

	// ���b�Z�[�W�E�B���h�E
	if (!tnm_is_hide_mwnd())	{
		int mwnd_cnt = m_mwnd_list.get_size();
		for (int i = 0; i < mwnd_cnt; i++)
			m_mwnd_list[i].button_event(p_sel_target_mwnd == &m_mwnd_list[i]);
	}

	// �{�^���I����
	if (!tnm_is_hide_mwnd())
		m_btn_sel.button_event();
}

// ****************************************************************
// �X�e�[�W�F�R�s�[���쐬
// ================================================================
void C_elm_stage::copy(C_elm_stage* src)
{
	// �O���[�v
	int group_cnt = m_group_list.get_size();
	for (int i = 0; i < group_cnt; i++)
		m_group_list[i].copy(&src->m_group_list[i]);

	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		if (m_object_list[i].is_use())
			m_object_list[i].copy(&src->m_object_list[i], true);
	}

	// ���b�Z�[�W�E�B���h�E
	int mwnd_cnt = m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)
		m_mwnd_list[i].copy(&src->m_mwnd_list[i]);

	// �{�^���I����
	m_btn_sel.copy(&src->m_btn_sel);

	// ���[���h
	int world_cnt = m_world_list.get_size();
	for (int i = 0; i < world_cnt; i++)
		m_world_list[i].copy(&src->m_world_list[i]);

	// �G�t�F�N�g
	int effect_cnt = m_effect_list.get_size();
	for (int i = 0; i < effect_cnt; i++)
		m_effect_list[i].copy(&src->m_effect_list[i]);

	// �N�G�C�N
	int quake_cnt = m_quake_list.get_size();
	for (int i = 0; i < quake_cnt; i++)
		m_quake_list[i].copy(&src->m_quake_list[i]);
}

// ****************************************************************
// �X�e�[�W�F���C�v����
// ================================================================
//		�R�s�[���āA�R�s�[��������������
// ================================================================
void C_elm_stage_list::wipe(S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter)
{
	C_elm_stage* p_back = &m_sub[TNM_STAGE_BACK];
	C_elm_stage* p_front = &m_sub[TNM_STAGE_FRONT];
	C_elm_stage* p_next = &m_sub[TNM_STAGE_NEXT];

	// �I�u�W�F�N�g
	int object_cnt = p_front->m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		C_elm_object* p_back_object = &p_back->m_object_list[i];
		C_elm_object* p_front_object = &p_front->m_object_list[i];
		C_elm_object* p_next_object = &p_next->m_object_list[i];

		if (p_front_object->is_use())	{
			S_tnm_sorter back_sorter = p_back_object->get_sorter();
			S_tnm_sorter front_sorter = p_front_object->get_sorter();

			// �t�����g���I�[�_�[���Ȃ烏�C�v����
			// �o�b�N����������Ă����烏�C�v����
			if ((begin_sorter <= front_sorter && front_sorter <= end_sorter) || p_back_object->get_type() != TNM_OBJECT_TYPE_NONE || p_back_object->get_child_cnt() > 0)	{
				p_next_object->copy(p_front_object, true);

				// �t�����g�Ƀ��C�v�R�s�[�������Ă��Ȃ���΃��C�v����
				// �o�b�N����������Ă����烏�C�v����
				// �o�b�N�Ƀ��C�v�����������Ă���΃��C�v����
				if (p_front_object->get_wipe_copy() == 0 || p_back_object->get_type() != TNM_OBJECT_TYPE_NONE || p_back_object->get_child_cnt() > 0 || p_back_object->get_wipe_erase() == 1)	{
					p_front_object->copy(p_back_object, true);
					p_back_object->reinit(true);
				}
			}
		}
	}

	// ���b�Z�[�W�E�B���h�E
	int mwnd_cnt = p_front->m_mwnd_list.get_size();
	for (int i = 0; i < mwnd_cnt; i++)	{
		S_tnm_sorter front_sorter = p_front->m_mwnd_list[i].get_sorter();
		if (begin_sorter <= front_sorter && front_sorter <= end_sorter)	{
			p_next->m_mwnd_list[i].copy(&p_front->m_mwnd_list[i]);
			p_front->m_mwnd_list[i].copy(&p_back->m_mwnd_list[i]);
			p_back->m_mwnd_list[i].reinit(true);
		}
	}

	// �O���[�v
	int group_cnt = p_front->m_group_list.get_size();
	for (int i = 0; i < group_cnt; i++)	{
		S_tnm_sorter front_sorter = p_front->m_group_list[i].get_sorter();
		if (begin_sorter <= front_sorter && front_sorter <= end_sorter)	{
			p_next->m_group_list[i].copy(&p_front->m_group_list[i]);
			p_front->m_group_list[i].copy(&p_back->m_group_list[i]);
			p_back->m_group_list[i].reinit();
		}
	}

	// �{�^���I����
	p_next->m_btn_sel.copy(&p_front->m_btn_sel);
	p_front->m_btn_sel.copy(&p_back->m_btn_sel);
	p_back->m_btn_sel.reinit();

	// ���[���h
	int world_cnt = p_front->m_world_list.get_size();
	for (int i = 0; i < world_cnt; i++)	{
		C_elm_world* p_back_world = &p_back->m_world_list[i];
		C_elm_world* p_front_world = &p_front->m_world_list[i];
		C_elm_world* p_next_world = &p_next->m_world_list[i];

		S_tnm_sorter back_sorter = p_back_world->get_sorter();
		S_tnm_sorter front_sorter = p_front_world->get_sorter();

		// �t�����g���I�[�_�[���Ȃ烏�C�v����
		if ((begin_sorter <= front_sorter && front_sorter <= end_sorter))	{
			p_next_world->copy(p_front_world);

			// �t�����g�Ƀ��C�v�R�s�[�������Ă��Ȃ���΃��C�v����
			// �o�b�N�Ƀ��C�v�����������Ă���΃��C�v����
			if (p_front_world->get_wipe_copy() == 0 || p_back_world->get_wipe_erase() == 1)	{
				p_front_world->copy(p_back_world);
				p_back_world->reinit();
			}
		}
	}

	// �G�t�F�N�g
	int effect_cnt = p_front->m_effect_list.get_size();
	for (int i = 0; i < effect_cnt; i++)	{
		C_elm_effect* p_back_effect = &p_back->m_effect_list[i];
		C_elm_effect* p_front_effect = &p_front->m_effect_list[i];
		C_elm_effect* p_next_effect = &p_next->m_effect_list[i];

		p_next_effect->copy(p_front_effect);

		// �t�����g�Ƀ��C�v�R�s�[�������Ă��Ȃ���΃��C�v����
		// �o�b�N�Ƀ��C�v�����������Ă���΃��C�v����
		if (p_front_effect->get_wipe_copy() == 0 || p_back_effect->get_wipe_erase() == 1)	{
			p_front_effect->copy(p_back_effect);
			p_back_effect->reinit();
		}
	}

	// �N�G�C�N
	int quake_cnt = p_front->m_quake_list.get_size();
	for (int i = 0; i < quake_cnt; i++)	{
		p_next->m_quake_list[i].copy(&p_front->m_quake_list[i]);
		p_front->m_quake_list[i].copy(&p_back->m_quake_list[i]);
		p_back->m_quake_list[i].reinit();
	}
}

// ****************************************************************
// �X�e�[�W�F�X�v���C�g�c���[�̎擾
// ================================================================
void C_elm_stage::get_sprite_tree(C_tree2<C_tnm_sprite *>& root, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter)
{
	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		if (m_object_list[i].is_use())	{
			S_tnm_sorter sorter = m_object_list[i].get_total_sorter();	// �\�[�^�[�̍��v�l���擾
			if (begin_sorter <= sorter && sorter <= end_sorter)	{		// �g�b�v���x���̐e�����̓\�[�^�[���Q�Ƃ���
				m_object_list[i].get_sprite_tree(root);
			}
		}
	}

	// ���b�Z�[�W�E�B���h�E
	int mwnd_cnt = m_mwnd_list.get_size();
	if (!tnm_is_hide_mwnd())	{
		for (int i = 0; i < mwnd_cnt; i++)	{
			S_tnm_sorter sorter = m_mwnd_list[i].get_sorter();			// ���������v�l�ɂ��Ȃ��Ɓc
			if (begin_sorter <= sorter && sorter <= end_sorter)	{
				m_mwnd_list[i].get_sprite_tree(root);
			}
		}
	}

	// �{�^���I����
	if (!tnm_is_hide_mwnd())	{
		S_tnm_sorter sorter(Gp_ini->mwnd.order, 0);
		if (begin_sorter <= sorter && sorter <= end_sorter)	{			// ���������v�l�ɂ��Ȃ��Ɓc
			m_btn_sel.get_sprite_tree(root);
		}
	}
}

// ****************************************************************
// �X�e�[�W���X�g�F������
// ================================================================
void C_elm_stage_list::init(S_element element, CTSTR& name, CTSTR& parent_name, int size, bool extend_enable, bool use_ini, C_tnm_element* p_parent)
{
	// �Œ�p�����[�^
	m_parent_element_name = parent_name;
	m_use_ini = use_ini;

	// ���N���X�̏�����
	C_elm_list_ex<C_elm_stage>::init(element, FM_STAGELIST, name, size, extend_enable, p_parent);
}

// ****************************************************************
// �X�e�[�W���X�g�F�I��
// ================================================================
void C_elm_stage_list::finish()
{
	int stage_cnt = (int)m_sub.size();
	for (int i = 0; i < stage_cnt; i++)
		m_sub[i].finish();
}

// ****************************************************************
// �X�e�[�W���X�g�F�T�u�̏�����
// ================================================================
void C_elm_stage_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)	{
		TSTR element_name;
		switch (i)	{
			case TNM_STAGE_BACK:	element_name = m_parent_element_name + _T("back");	break;
			case TNM_STAGE_FRONT:	element_name = m_parent_element_name + _T("front");	break;
			case TNM_STAGE_NEXT:	element_name = m_parent_element_name + _T("next");	break;
		}

		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), element_name, m_use_ini);
	}
}

// ****************************************************************
// �X�e�[�W���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_stage_list::_reinit(int begin, int end, bool restruct_flag)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit(restruct_flag);
}

// ****************************************************************
// �X�e�[�W���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_stage_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �X�e�[�W���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_stage_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}

