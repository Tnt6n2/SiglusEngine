#include	"pch.h"

#if __USE_EMOTE
#include	"iemote.h"
#endif

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_tonecurve.h"
#include	"data/tnm_btn_template.h"
#include	"element/elm_object.h"
#include	"element/elm_group.h"
#include	"element/elm_sound.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_mask.h"
#include	"engine/eng_fog.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_etc.h"
#include	"engine/flow_command.h"
#include	"engine/eng_emote.h"

// ****************************************************************
// �I�u�W�F�N�g�F������
// ================================================================
//		����ς݂��O��ł��B�t�H�[�}�b�g���s���܂��B
// ================================================================
void C_elm_object::init(S_element element, CTSTR& name, bool use_flag, bool use_group_flag, bool set_cg_table, bool space_hide_flag, int object_disp_no, C_tnm_element* p_parent, S_element elm_group_list, S_element elm_world_list)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_OBJECT, name, p_parent);

	// �Œ�p�����[�^
	m_op_def.use_flag = use_flag;
	m_op_def.use_group_flag = use_group_flag;
	m_op_def.set_cg_table = set_cg_table;
	m_op_def.object_disp_no = object_disp_no;
	m_op_def.space_hide_flag = space_hide_flag;
	m_op_def.elm_group_list = elm_group_list;
	m_op_def.elm_world_list = elm_world_list;

	// �C�����W
	m_op.x_rep_list.init(element + S_element(ELM_OBJECT_X_REP), name + _T(".x_rep"), 0, true, 0);
	m_op.y_rep_list.init(element + S_element(ELM_OBJECT_Y_REP), name + _T(".y_rep"), 0, true, 0);
	m_op.z_rep_list.init(element + S_element(ELM_OBJECT_Z_REP), name + _T(".z_rep"), 0, true, 0);
	m_op.tr_rep_list.init(element + S_element(ELM_OBJECT_TR_REP), name + _T(".tr_rep"), 0, true, 255);

	// �ėp�t���O
	m_op.f.init(element + S_element(ELM_OBJECT_F), name + _T(".f"), 0, true);

	// �t���[���A�N�V����
	m_op.frame_action.init(element + S_element(ELM_OBJECT_FRAME_ACTION), name + _T(".frame_action"), element);

	// �t���[���A�N�V�����`�����l��
	m_op.frame_action_ch.init(element + S_element(ELM_OBJECT_FRAME_ACTION_CH), name + _T(".frame_action_ch"), 0, true, element);

	// �q��
	m_child_list.init(element + S_element(ELM_OBJECT_CHILD), name + _T(".child"), 0, true, false, use_group_flag, set_cg_table, this, tnm_set_element_down(elm_group_list), tnm_set_element_down(elm_world_list));
	
	// �X�v���C�g�̎��̂����
	m_sprite = BSP<C_tnm_sprite>(new C_tnm_sprite);

	// �ď�����
	reinit(false);	// �I������������s��Ȃ�
}

// ****************************************************************
// �I�u�W�F�N�g�F�ď�����
// ================================================================
void C_elm_object::reinit(bool finish_free_flag)
{
	// ����I������
	if (finish_free_flag)
	{
		// �I�������i�q�����܂ށj
		finish();

		// �^�C�v���������i�q�����܂ށj
		free_type(true);

		// �q�������
		m_child_list.clear();
	}

	// ���C�v�R�s�[�̏�����
	m_op.wipe_copy = 0;
	m_op.wipe_erase = 0;

	// �^�C�v�̏�����
	init_type(false);		// ����������s��Ȃ�

	// �p�����[�^�̏�����
	init_param(false);		// �I���������s��Ȃ�
}

// ****************************************************************
// �I�u�W�F�N�g�F���
// ================================================================
void C_elm_object::free()
{
	init_type(true);
}

// ****************************************************************
// �I�u�W�F�N�g�F�I��
// ================================================================
void C_elm_object::finish()
{
	// �q��
	int child_cnt = m_child_list.get_size();
	for (int i = 0; i < child_cnt; i++)
		m_child_list[i].finish();

	// �t���[���A�N�V�����`�����l��
	int frame_action_ch_cnt = m_op.frame_action_ch.get_size();
	for (int i = 0; i < frame_action_ch_cnt; i++)
		m_op.frame_action_ch[i].finish();

	// �t���[���A�N�V����
	m_op.frame_action.finish();
}

// ****************************************************************
// �I�u�W�F�N�g�F�^�C�v�̏�����
// ================================================================
//		�^�C�v�ŗL�̃p�����[�^�����������܂��B
//		�u�`��p�����[�^�v�ugan�v�u�{�^���v�u�t���[���A�N�V�����v�Ȃǂ͏���������܂���B
//		�u���C�v�R�s�[�v�u���C�v�폜�v�u�q���v�͏���������܂���B
// ================================================================
void C_elm_object::init_type(bool free_flag)
{
	// �����������
	if (free_flag)
	{
		// �^�C�v������i�����̂݁j
		free_type(false);
	}

	// ��{�p�����[�^
	m_op.type = TNM_OBJECT_TYPE_NONE;

	// �t�B���^�p�����[�^
	init_filter_param();

	// ������p�����[�^
	init_string_param();

	// ���l�p�����[�^
	init_number_param();

	// ���p�����[�^
	init_weather_param();

	// �T���l�C���p�����[�^
	m_op.thumb.save_no = -1;

	// ���[�r�[�p�����[�^
	m_op.movie.loop_flag = false;
	m_op.movie.auto_free_flag = true;	// �f�t�H���g�Ŏ����������
	m_op.movie.real_time_flag = true;	// �f�t�H���g�Ŏ�����
	m_op.movie.pause_flag = false;
	m_omv_timer = 0;

	// E-mote �p�����[�^
	m_op.emote.size = C_size(0, 0);
	for (int i = 0; i < TNM_OBJECT_EMOTE_TIMELINE_NUM; i++)
	{
		m_op.emote_timeline[i].clear();
		m_op.emote.timeline[i].option = 0;
	}
	m_op.emote.koe_chara_no = -1;
	m_op.emote.koe_mouth_volume = 0;
	m_op.emote.rep_x = 0;
	m_op.emote.rep_y = 0;

	// ���̑��̃p�����[�^
	m_op.file_path.clear();
	m_op.str.clear();
}

// ****************************************************************
// �I�u�W�F�N�g�F�^�C�v�̉��
// ================================================================
void C_elm_object::free_type(bool child_flag)
{
	// �q��
	if (child_flag)
	{
		int child_cnt = m_child_list.get_size();
		for (int i = 0; i < child_cnt; i++)
			m_child_list[i].free_type(child_flag);
	}

	// E-mote
#if __USE_EMOTE
	if (m_emote_player)
	{
		m_emote_player->Release();
		m_emote_player = NULL;
	}
#endif

	// ����
	m_moji_list.clear();
	m_weather.cnt_max = 0;
	m_weather.sub.clear();
	m_album.reset();
	m_emote_texture.reset();
	m_emote_dpst.reset();
	m_mesh.reset();
	m_frame_set.reset();
	m_sprite->init();
	m_sprite_list.clear();
	m_omv_player.reset();
}

// ****************************************************************
// �I�u�W�F�N�g�F�t�B���^�p�����[�^�̏�����
// ================================================================
void C_elm_object::init_filter_param()
{
	m_op.filter.rect = C_rect(0, 0, 0, 0);	// ��`
	m_op.filter.color = C_argb(0, 0, 0, 0);	// �F
}

// ****************************************************************
// �I�u�W�F�N�g�F������p�����[�^�̏�����
// ================================================================
void C_elm_object::init_string_param()
{
	m_op.string.moji_size = 12;
	m_op.string.moji_space_x = 0;
	m_op.string.moji_space_y = 0;
	m_op.string.moji_cnt = 0;
	m_op.string.moji_color = Gp_ini->mwnd.moji_color;
	m_op.string.shadow_color = Gp_ini->mwnd.shadow_color;
	m_op.string.fuchi_color = Gp_ini->mwnd.fuchi_color;
	m_op.string.shadow_mode = -1;	// �V�X�e���̐ݒ�ɏ]��
}

// ****************************************************************
// �I�u�W�F�N�g�F�p�����[�^�̏�����
// ================================================================
//		�^�C�v�ŗL�łȂ��p�����[�^�����������܂��B
//		�u�`��p�����[�^�v�ugan�v�u�{�^���v�u�t���[���A�N�V�����v�Ȃǂ�����������܂��B
//		�u���C�v�R�s�[�v�u���C�v�폜�v�u�q���v�͏���������܂���B
// ================================================================
void C_elm_object::init_param(bool finish_flag)
{
	// �I��
	if (finish_flag)
		finish();

	// ��{�p�����[�^
	m_op.click_disable = 0;					// �N���b�N�֎~

	// �`��p�����[�^
	m_op.obp.disp = 0;						// �\���t���O
	m_op.obp.pat_no.init(0);				// �p�^�[���ԍ�
	m_op.obp.sorter = S_tnm_sorter(0, 0);	// �\�[�^�[
	m_op.obp.world_no = TNM_WORLD_NONE;		// ���[���h
	m_op.obp.child_sort_type = TNM_OBJECT_SORT_TYPE_DEFAULT;
	m_op.obp.pos_x.init(0);					// �ʒu�i�w�j
	m_op.obp.pos_y.init(0);					// �ʒu�i�x�j
	m_op.obp.pos_z.init(0);					// �ʒu�i�y�j
	m_op.obp.center_x.init(0);				// ���S�i�w�j
	m_op.obp.center_y.init(0);				// ���S�i�x�j
	m_op.obp.center_z.init(0);				// ���S�i�y�j
	m_op.obp.center_rep_x.init(0);			// ���S�␳�i�w�j
	m_op.obp.center_rep_y.init(0);			// ���S�␳�i�x�j
	m_op.obp.center_rep_z.init(0);			// ���S�␳�i�y�j
	m_op.obp.scale_x.init(1000);			// �g�k�i�w�j
	m_op.obp.scale_y.init(1000);			// �g�k�i�x�j
	m_op.obp.scale_z.init(1000);			// �g�k�i�y�j
	m_op.obp.rotate_x.init(0);				// ��]�i�w�j
	m_op.obp.rotate_y.init(0);				// ��]�i�x�j
	m_op.obp.rotate_z.init(0);				// ��]�i�y�j
	m_op.obp.dst_clip_use = 0;				// �]����\���͈͎g�p
	m_op.obp.dst_clip_left.init(0);			// �]����\���͈�
	m_op.obp.dst_clip_top.init(0);			// �]����\���͈�
	m_op.obp.dst_clip_right.init(0);		// �]����\���͈�
	m_op.obp.dst_clip_bottom.init(0);		// �]����\���͈�
	m_op.obp.src_clip_use = 0;				// �]�����\���͈͎g�p
	m_op.obp.src_clip_left.init(0);			// �]�����\���͈�
	m_op.obp.src_clip_top.init(0);			// �]�����\���͈�
	m_op.obp.src_clip_right.init(0);		// �]�����\���͈�
	m_op.obp.src_clip_bottom.init(0);		// �]�����\���͈�
	m_op.obp.tr.init(255);					// �s�����x
	m_op.obp.mono.init(0);					// ���m�N��
	m_op.obp.reverse.init(0);				// ���]�x
	m_op.obp.bright.init(0);				// ���x
	m_op.obp.dark.init(0);					// �Óx
	m_op.obp.color_r.init(0);				// �F�ϊ��i�q�j
	m_op.obp.color_g.init(0);				// �F�ϊ��i�f�j
	m_op.obp.color_b.init(0);				// �F�ϊ��i�a�j
	m_op.obp.color_rate.init(0);			// �F�ϊ��i���x�j
	m_op.obp.color_add_r.init(0);			// �F���Z�i�q�j
	m_op.obp.color_add_g.init(0);			// �F���Z�i�f�j
	m_op.obp.color_add_b.init(0);			// �F���Z�i�a�j

	m_op.obp.mask_no = TNM_MASK_NONE;					// �}�X�N
	m_op.obp.tone_curve_no = TNM_TONE_CURVE_NONE;		// �g�[���J�[�u
	m_op.obp.light_no = TNM_LIGHT_NONE;					// ���C�g
	m_op.obp.use_fog = 0;								// �t�H�O
	m_op.obp.culling = 0;								// �J�����O
	m_op.obp.alpha_test = 1;							// �A���t�@�e�X�g
	m_op.obp.alpha_blend = 1;							// �A���t�@�u�����h
	m_op.obp.blend_type = TNM_BLEND_TYPE_NORMAL;		// �������@
	m_op.obp.flags = 0;									// ���̑��̃t���O

	// �C�����W
	m_op.x_rep_list.reinit();
	m_op.y_rep_list.reinit();
	m_op.z_rep_list.reinit();
	m_op.tr_rep_list.reinit();

	// �ėp�t���O
	m_op.f.reinit();

	// �t���[���A�N�V����
	init_frame_action(false);	// �I���������s��Ȃ�

	// �{�^���p�����[�^
	init_button_param();

	// �f�`�m�p�����[�^
	m_op.gan.free();
}

// ****************************************************************
// �I�u�W�F�N�g�F�Z�[�u
// ================================================================

#define		___OBJ_SAVE_SMALL		1

#define		SAVE_EVENT(param)											\
			stream.save(param.m_loop_type);								\
			if (param.m_loop_type != E_tnm_int_event_loop_type_none)	\
			{															\
				stream.save(param);										\
			}															\
			else														\
			{															\
				stream.save(param.m_value);								\
			}

#define		LOAD_EVENT(param)											\
			stream.load(param.m_loop_type);								\
			if (param.m_loop_type != E_tnm_int_event_loop_type_none)	\
			{															\
				stream.load(param);										\
			}															\
			else														\
			{															\
				stream.load(param.m_value);								\
			}

void C_elm_object::save(C_tnm_save_stream& stream)
{
	if (m_op.type == TNM_OBJECT_TYPE_CAPTURE)
	{
		tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("�L���v�`���[�I�u�W�F�N�g���Z�[�u���悤�Ƃ��܂����B\n���̃I�u�W�F�N�g�̓Z�[�u�ł��܂���BGameexe.ini �Łu�Z�[�u���Ȃ��v�ݒ�ɂ��ĉ������B"));

		// ���x���͏o�����A�Z�[�u�͍s���܂��B���ꂪ�Ȃ��ƃ��[�h���ɃT�C�Y�s�����ŗ������Ⴄ�B
	}

	stream.save(m_op.type);				// �^�C�v
	stream.save(m_op.wipe_copy);		// ���C�v�R�s�[
	stream.save(m_op.wipe_erase);		// ���C�v����
	stream.save(m_op.click_disable);	// �N���b�N�֎~
	stream.save(m_op.filter);			// �t�B���^�[�p�����[�^
	stream.save(m_op.string);			// ������p�����[�^
	stream.save(m_op.number);			// ���l�p�����[�^

	if (m_op.type == TNM_OBJECT_TYPE_WEATHER)
	{
		stream.save(m_op.weather);			// ���p�����[�^
	}

	stream.save(m_op.thumb);			// �T���l�C���p�����[�^
	stream.save(m_op.movie);			// ���[�r�[�p�����[�^

	if (m_op.type == TNM_OBJECT_TYPE_EMOTE)
	{
		stream.save(m_op.emote);			// E-mote �p�����[�^
	}

	if (m_op.button.elm_group.is_empty())
	{
		stream.save(0);
	}
	else
	{
		stream.save(1);
		stream.save(m_op.button);			// �{�^���p�����[�^
	}

#if ___OBJ_SAVE_SMALL

	stream.save(m_op.obp.disp);
	stream.save(m_op.obp.pat_no);
	stream.save(m_op.obp.sorter);
	stream.save(m_op.obp.world_no);
	stream.save(m_op.obp.child_sort_type);
	SAVE_EVENT(m_op.obp.pos_x);
	SAVE_EVENT(m_op.obp.pos_y);
	SAVE_EVENT(m_op.obp.pos_z);
	SAVE_EVENT(m_op.obp.center_x);
	SAVE_EVENT(m_op.obp.center_y);
	SAVE_EVENT(m_op.obp.center_z);
	SAVE_EVENT(m_op.obp.center_rep_x);
	SAVE_EVENT(m_op.obp.center_rep_y);
	SAVE_EVENT(m_op.obp.center_rep_z);
	SAVE_EVENT(m_op.obp.scale_x);
	SAVE_EVENT(m_op.obp.scale_y);
	SAVE_EVENT(m_op.obp.scale_z);
	SAVE_EVENT(m_op.obp.rotate_x);
	SAVE_EVENT(m_op.obp.rotate_y);
	SAVE_EVENT(m_op.obp.rotate_z);
	stream.save(m_op.obp.dst_clip_use);
	SAVE_EVENT(m_op.obp.dst_clip_left);
	SAVE_EVENT(m_op.obp.dst_clip_top);
	SAVE_EVENT(m_op.obp.dst_clip_right);
	SAVE_EVENT(m_op.obp.dst_clip_bottom);
	stream.save(m_op.obp.src_clip_use);
	SAVE_EVENT(m_op.obp.src_clip_left);
	SAVE_EVENT(m_op.obp.src_clip_top);
	SAVE_EVENT(m_op.obp.src_clip_right);
	SAVE_EVENT(m_op.obp.src_clip_bottom);

	SAVE_EVENT(m_op.obp.tr);
	SAVE_EVENT(m_op.obp.mono);
	SAVE_EVENT(m_op.obp.reverse);
	SAVE_EVENT(m_op.obp.bright);
	SAVE_EVENT(m_op.obp.dark);
	SAVE_EVENT(m_op.obp.color_r);
	SAVE_EVENT(m_op.obp.color_g);
	SAVE_EVENT(m_op.obp.color_b);
	SAVE_EVENT(m_op.obp.color_rate);
	SAVE_EVENT(m_op.obp.color_add_r);
	SAVE_EVENT(m_op.obp.color_add_g);
	SAVE_EVENT(m_op.obp.color_add_b);
	stream.save(m_op.obp.mask_no);
	stream.save(m_op.obp.tone_curve_no);
	stream.save(m_op.obp.light_no);
	stream.save(m_op.obp.use_fog);
	stream.save(m_op.obp.culling);
	stream.save(m_op.obp.alpha_test);
	stream.save(m_op.obp.alpha_blend);
	stream.save(m_op.obp.blend_type);
	stream.save(m_op.obp.flags);

#else

	stream.save(m_op.obp);				// �����_�[�p�����[�^

#endif

	// �o�n�c����Ȃ��f�[�^���Z�[�u
	m_op.x_rep_list.save(stream);		// �C�����W�w
	m_op.y_rep_list.save(stream);		// �C�����W�x
	m_op.z_rep_list.save(stream);		// �C�����W�y
	m_op.tr_rep_list.save(stream);		// �C���s�����x�w
	m_op.f.save(stream);				// �ėp�t���O
	stream.save(m_op.file_path);		// �t�@�C���p�X
	stream.save(m_op.str);				// ������
	stream.save(m_op.button_decided_action_scn_name);		// �{�^������A�N�V�����̃V�[����
	stream.save(m_op.button_decided_action_cmd_name);		// �{�^������A�N�V�����̃R�}���h��

	// E-mote �^�C�����C��
	if (m_op.type == TNM_OBJECT_TYPE_EMOTE)
	{
		for (int i = 0; i < TNM_OBJECT_EMOTE_TIMELINE_NUM; i++)
		{
			stream.save(m_op.emote_timeline[i]);
		}
	}

	// �q�G�������g
	m_op.frame_action.save(stream);		// �t���[���A�N�V����
	m_op.frame_action_ch.save(stream);	// �t���[���A�N�V�����`�����l��
	m_op.gan.save(stream);				// �f�`�m
	m_child_list.save(stream);			// �q��
}

// ****************************************************************
// �I�u�W�F�N�g�F���[�h
// ================================================================
void C_elm_object::load(C_tnm_save_stream& stream)
{
	// ���������ς݂�O��Ƃ��Ă��܂��B

	// �p�����[�^�����[�h
	stream.load(m_op.type);				// �^�C�v
	stream.load(m_op.wipe_copy);		// ���C�v�R�s�[
	stream.load(m_op.wipe_erase);		// ���C�v����
	stream.load(m_op.click_disable);	// �N���b�N�֎~
	stream.load(m_op.filter);			// �t�B���^�[�p�����[�^
	stream.load(m_op.string);			// ������p�����[�^
	stream.load(m_op.number);			// ���l�p�����[�^

	if (m_op.type == TNM_OBJECT_TYPE_WEATHER)
	{
		stream.load(m_op.weather);			// ���p�����[�^
	}

	stream.load(m_op.thumb);			// �T���l�C���p�����[�^
	stream.load(m_op.movie);			// ���[�r�[�p�����[�^

	if (m_op.type == TNM_OBJECT_TYPE_EMOTE)
	{
		stream.load(m_op.emote);			// E-mote �p�����[�^
	}

	int button_exist = stream.load_ret<int>();
	if (button_exist)
	{
		stream.load(m_op.button);			// �{�^���p�����[�^
	}

#if ___OBJ_SAVE_SMALL

	stream.load(m_op.obp.disp);
	stream.load(m_op.obp.pat_no);
	stream.load(m_op.obp.sorter);
	stream.load(m_op.obp.world_no);
	stream.load(m_op.obp.child_sort_type);
	LOAD_EVENT(m_op.obp.pos_x);
	LOAD_EVENT(m_op.obp.pos_y);
	LOAD_EVENT(m_op.obp.pos_z);
	LOAD_EVENT(m_op.obp.center_x);
	LOAD_EVENT(m_op.obp.center_y);
	LOAD_EVENT(m_op.obp.center_z);
	LOAD_EVENT(m_op.obp.center_rep_x);
	LOAD_EVENT(m_op.obp.center_rep_y);
	LOAD_EVENT(m_op.obp.center_rep_z);
	LOAD_EVENT(m_op.obp.scale_x);
	LOAD_EVENT(m_op.obp.scale_y);
	LOAD_EVENT(m_op.obp.scale_z);
	LOAD_EVENT(m_op.obp.rotate_x);
	LOAD_EVENT(m_op.obp.rotate_y);
	LOAD_EVENT(m_op.obp.rotate_z);
	stream.load(m_op.obp.dst_clip_use);
	LOAD_EVENT(m_op.obp.dst_clip_left);
	LOAD_EVENT(m_op.obp.dst_clip_top);
	LOAD_EVENT(m_op.obp.dst_clip_right);
	LOAD_EVENT(m_op.obp.dst_clip_bottom);
	stream.load(m_op.obp.src_clip_use);
	LOAD_EVENT(m_op.obp.src_clip_left);
	LOAD_EVENT(m_op.obp.src_clip_top);
	LOAD_EVENT(m_op.obp.src_clip_right);
	LOAD_EVENT(m_op.obp.src_clip_bottom);

	LOAD_EVENT(m_op.obp.tr);
	LOAD_EVENT(m_op.obp.mono);
	LOAD_EVENT(m_op.obp.reverse);
	LOAD_EVENT(m_op.obp.bright);
	LOAD_EVENT(m_op.obp.dark);
	LOAD_EVENT(m_op.obp.color_r);
	LOAD_EVENT(m_op.obp.color_g);
	LOAD_EVENT(m_op.obp.color_b);
	LOAD_EVENT(m_op.obp.color_rate);
	LOAD_EVENT(m_op.obp.color_add_r);
	LOAD_EVENT(m_op.obp.color_add_g);
	LOAD_EVENT(m_op.obp.color_add_b);
	stream.load(m_op.obp.mask_no);
	stream.load(m_op.obp.tone_curve_no);
	stream.load(m_op.obp.light_no);
	stream.load(m_op.obp.use_fog);
	stream.load(m_op.obp.culling);
	stream.load(m_op.obp.alpha_test);
	stream.load(m_op.obp.alpha_blend);
	stream.load(m_op.obp.blend_type);
	stream.load(m_op.obp.flags);

#else

	stream.load(m_op.obp);				// �����_�[�p�����[�^

#endif

	// �o�n�c�łȂ��f�[�^�����[�h
	m_op.x_rep_list.load(stream);		// �C�����W�w
	m_op.y_rep_list.load(stream);		// �C�����W�x
	m_op.z_rep_list.load(stream);		// �C�����W�y
	m_op.tr_rep_list.load(stream);		// �C���s�����x�w
	m_op.f.load(stream);				// �ėp�t���O
	stream.load(m_op.file_path);		// �t�@�C���p�X
	stream.load(m_op.str);				// ������
	stream.load(m_op.button_decided_action_scn_name);		// �{�^������A�N�V�����̃V�[����
	stream.load(m_op.button_decided_action_cmd_name);		// �{�^������A�N�V�����̃R�}���h��

	// E-mote �^�C�����C��
	if (m_op.type == TNM_OBJECT_TYPE_EMOTE)
	{
		for (int i = 0; i < TNM_OBJECT_EMOTE_TIMELINE_NUM; i++)
		{
			stream.load(m_op.emote_timeline[i]);
		}
	}

	// ���[�r�[�̏ꍇ�A�����V���b�g�i���[�v�����Ɏ����������j�ꍇ�͏�����
	if (m_op.type == TNM_OBJECT_TYPE_MOVIE)
	{
		if (m_op.movie.loop_flag == 0 && m_op.movie.auto_free_flag == 1 && m_op.movie.pause_flag == 0)
		{
			init_type(true);
		}
	}

	// �^�C�v���č\�z
	restruct_type();

	// �{�^���A�N�V�������č\�z
	restruct_button_action();

	// E-mote �^�C�����C��
#if __USE_EMOTE
	if (m_op.type == TNM_OBJECT_TYPE_EMOTE)
	{
		if (m_emote_player)
		{
			for (int i = 0; i < TNM_OBJECT_EMOTE_TIMELINE_NUM; i++)
			{
				if (!m_op.emote_timeline[i].empty())
				{
					m_emote_player->PlayTimeline(TSTR_to_UTF8(m_op.emote_timeline[i]).c_str(), m_op.emote.timeline[i].option);
				}
			}

			m_emote_player->Skip();
		}
	}
#endif

	// �q�G�������g
	m_op.frame_action.load(stream);		// �t���[���A�N�V����
	m_op.frame_action_ch.load(stream);	// �t���[���A�N�V�����`�����l��
	m_op.gan.load(stream);				// �f�`�m
	m_child_list.load(stream);			// �q��
}

// ****************************************************************
// �I�u�W�F�N�g�F�R�s�[���쐬
// ================================================================
void C_elm_object::copy(const C_elm_object* src, bool free_flag)
{
	if (free_flag)
	{
		// �I�������i�q�����܂ށj
		finish();
		// �^�C�v������i�q�����܂ށj
		free_type(true);
	}

	// �^�C�v�p�����[�^���R�s�[
	m_op.type = src->m_op.type;
	m_op.wipe_copy = src->m_op.wipe_copy;
	m_op.wipe_erase = src->m_op.wipe_erase;
	m_op.click_disable = src->m_op.click_disable;
	m_op.filter = src->m_op.filter;
	m_op.string = src->m_op.string;
	m_op.number = src->m_op.number;
	m_op.weather = src->m_op.weather;
	m_op.emote = src->m_op.emote;
	m_op.thumb = src->m_op.thumb;
	m_op.movie = src->m_op.movie;
	m_op.button = src->m_op.button;
	m_op.file_path = src->m_op.file_path;
	m_op.str = src->m_op.str;

	// E-mote
	for (int i = 0; i < TNM_OBJECT_EMOTE_TIMELINE_NUM; i++)
	{
		m_op.emote_timeline[i] = src->m_op.emote_timeline[i];
	}

	// ���[�r�[�̏ꍇ�͈ړ��ɂȂ�
	if (m_op.type == TNM_OBJECT_TYPE_MOVIE)
	{
		m_album = src->m_album;
		m_omv_player = src->m_omv_player;
		m_omv_timer = src->m_omv_timer;
	}
#if __USE_EMOTE
	else if (m_op.type == TNM_OBJECT_TYPE_EMOTE)
	{
		m_emote_player = src->m_emote_player->Clone();

		// �e�N�X�`�����쐬
		m_emote_texture = G_rsm.create_texture(_T("emote"), m_op.emote.size.cx, m_op.emote.size.cy, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);
		m_emote_dpst = G_rsm.create_dpst_buffer(_T("emote"), m_op.emote.size.cx, m_op.emote.size.cy, true);
	}
#endif
	else
	{
		restruct_type();		// �^�C�v���č\�z
	}

	// ��
	m_weather = src->m_weather;

	// �����č\�z
	restruct_weather();

	// �����_�[�p�����[�^���R�s�[
	m_op.obp = src->m_op.obp;

	// �����C�x���g�͂o�n�c�I
	m_op.x_rep_list.resize(src->m_op.x_rep_list.get_size());
	memcpy(m_op.x_rep_list.get_ptr(), src->m_op.x_rep_list.get_ptr(), src->m_op.x_rep_list.get_size() * sizeof(C_elm_int_event));
	m_op.y_rep_list.resize(src->m_op.y_rep_list.get_size());
	memcpy(m_op.y_rep_list.get_ptr(), src->m_op.y_rep_list.get_ptr(), src->m_op.y_rep_list.get_size() * sizeof(C_elm_int_event));
	m_op.z_rep_list.resize(src->m_op.z_rep_list.get_size());
	memcpy(m_op.z_rep_list.get_ptr(), src->m_op.z_rep_list.get_ptr(), src->m_op.z_rep_list.get_size() * sizeof(C_elm_int_event));
	m_op.tr_rep_list.resize(src->m_op.tr_rep_list.get_size());
	memcpy(m_op.tr_rep_list.get_ptr(), src->m_op.tr_rep_list.get_ptr(), src->m_op.tr_rep_list.get_size() * sizeof(C_elm_int_event));

	// �������o�n�c
	m_op.f.resize(src->m_op.f.get_size());
	memcpy(m_op.f.get_ptr(), src->m_op.f.get_ptr(), src->m_op.f.get_size() * sizeof(m_op.f[0]));

	// �o�n�c�łȂ��f�[�^���R�s�[
	m_op.button_decided_action_scn_name = src->m_op.button_decided_action_scn_name;
	m_op.button_decided_action_cmd_name = src->m_op.button_decided_action_cmd_name;

	// �{�^���A�N�V�������č\�z
	restruct_button_action();

	// �t���[���A�N�V����
	m_op.frame_action.copy(&src->m_op.frame_action, false);		// �I���������s��Ȃ�

	// �t���[���A�N�V�����`�����l��
	int frame_action_ch_cnt = src->m_op.frame_action_ch.get_size();
	m_op.frame_action_ch.resize(frame_action_ch_cnt);
	for (int i = 0; i < frame_action_ch_cnt; i++)
		m_op.frame_action_ch[i].copy(&src->m_op.frame_action_ch[i], false);		// �I���������s��Ȃ�

	// GAN
	m_op.gan.copy(&src->m_op.gan);

	// �q��
	int child_cnt = src->m_child_list.get_size();
	m_child_list.resize(child_cnt);
	for (int i = 0; i < child_cnt; i++)
		m_child_list[i].copy(&src->m_child_list[i], false);
}

// ****************************************************************
// �I�u�W�F�N�g�F�^�C�v���č\�z
// ================================================================
bool C_elm_object::restruct_type()
{
	switch (m_op.type)
	{
		case TNM_OBJECT_TYPE_NONE:			init_type(false);		break;
		case TNM_OBJECT_TYPE_RECT:			restruct_rect();		break;
		case TNM_OBJECT_TYPE_PCT:			restruct_pct();			break;
		case TNM_OBJECT_TYPE_STRING:		restruct_string();		break;
		case TNM_OBJECT_TYPE_WEATHER:		restruct_weather();		break;
		case TNM_OBJECT_TYPE_NUMBER:		restruct_number();		break;
		case TNM_OBJECT_TYPE_MESH:			restruct_mesh();		break;
		case TNM_OBJECT_TYPE_BILLBOARD:		restruct_billboard();	break;
		case TNM_OBJECT_TYPE_SAVE_THUMB:	restruct_save_thumb();	break;
		case TNM_OBJECT_TYPE_THUMB:			restruct_thumb();		break;
		case TNM_OBJECT_TYPE_MOVIE:			restruct_movie();		break;
		case TNM_OBJECT_TYPE_EMOTE:			restruct_emote();		break;
	}

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F��`�I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_rect(C_rect rect, C_argb color)
{
	// ���ɍ���Ă���ꍇ�͉������Ȃ�
	if (m_op.type == TNM_OBJECT_TYPE_RECT && m_op.filter.rect == rect && m_op.filter.color == color)
		return true;

	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ肷��
	m_op.type = TNM_OBJECT_TYPE_RECT;
	m_op.filter.rect = rect;
	m_op.filter.color = color;

	// ��`�I�u�W�F�N�g���č\�z����
	if (!restruct_rect())
		return false;

	return true;
}

bool C_elm_object::restruct_rect()
{
	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F�s�N�`���I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_pct(TSTR file_path)
{
	// ������Fİݶ��ސ�ł��i�L�l�e�B�b�N�ڐA�p�F�V�삳����j
	int pos = file_path.find(_T('?'));
	if (pos != TSTR::npos)
	{
		TSTR tone_curve_str = file_path.substr(pos + 1);
		int tone_curve_no = str_to_int(tone_curve_str, -1);
		if (tone_curve_no > 0)	{
			m_op.obp.tone_curve_no = tone_curve_no;
		}
		file_path = file_path.substr(0, pos);
	}

	// ���ɍ���Ă���ꍇ�͉������Ȃ�
	if (m_op.type == TNM_OBJECT_TYPE_PCT && m_op.file_path == file_path)
		return true;

	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_PCT;
	m_op.file_path = file_path;

	// �s�N�`���I�u�W�F�N�g���č\�z����
	if (!restruct_pct())
		return false;

	return true;
}

bool C_elm_object::restruct_pct()
{
	if (m_op.file_path.empty())
		return true;

	// �e�N�X�`����ǂݍ���
	m_album = tnm_load_pct_d3d(m_op.file_path, false, m_op_def.set_cg_table);
	if (!m_album)
	{
		m_op.file_path.clear();		// ���s�����̂Ńt�@�C�������N���A
		return false;
	}

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F�Z�[�u�T���l�C���I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_save_thumb(int save_no)
{
	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_SAVE_THUMB;
	m_op.thumb.save_no = save_no;

	// �T���l�C���I�u�W�F�N�g���č\�z����
	if (!restruct_save_thumb())
		return false;

	return true;
}

bool C_elm_object::restruct_save_thumb()
{
	// �e�N�X�`����ǂݍ���
	m_album = tnm_load_pct_d3d_from_save_thumb(m_op.thumb.save_no);
	if (!m_album)
		return false;

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F�T���l�C���I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_thumb(int thumb_no)
{
	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_THUMB;
	m_op.thumb.save_no = thumb_no;

	// �T���l�C���I�u�W�F�N�g���č\�z����
	if (!restruct_thumb())
		return false;

	return true;
}

bool C_elm_object::restruct_thumb()
{
	// �e�N�X�`����ǂݍ���
	m_album = tnm_load_pct_d3d_from_thumb(m_op.thumb.save_no);
	if (!m_album)
		return false;

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F�L���v�`���[�I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_capture()
{
	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_CAPTURE;

	// �L���v�`���[�I�u�W�F�N�g���č\�z����
	if (!restruct_capture())
		return false;

	return true;
}

bool C_elm_object::restruct_capture()
{
	if (!Gp_wnd->m_capture_texture_for_object)
	{
		tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("�L���v�`���[������܂���B"));
		return false;
	}

	m_album = BSP<C_d3d_album>(new C_d3d_album);
	m_album->add_texture(Gp_wnd->m_capture_texture_for_object);

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�FE-mote �I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_emote(int width, int height, TSTR file_path)
{
	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_EMOTE;
	m_op.file_path = file_path;
	m_op.emote.size = C_size(width, height);

	// E-mote �I�u�W�F�N�g���č\�z����
	if (!restruct_emote())
		return false;

	return true;
}

#include <boost/algorithm/string/classification.hpp> // is_any_of
#include <boost/algorithm/string/split.hpp>


bool C_elm_object::restruct_emote()
{
#if __USE_EMOTE

	TSTR file_name_full = m_op.file_path;
	ARRAY<TSTR> file_name_list;
	ARRAY<TSTR> file_path_list;

	// �t�@�C�����𕪊�
	std::vector<std::string> file_name_list_mbstr;
	boost::algorithm::split(file_name_list_mbstr, TSTR_to_MBSTR(file_name_full), boost::is_any_of("|"));
	for (int i = 0; i < (int)file_name_list_mbstr.size(); i++)
	{
		file_name_list.push_back(MBSTR_to_TSTR(file_name_list_mbstr[i]));
	}

	// psb ��T��
	for (int i = 0; i < (int)file_name_list.size(); i++)
	{
		TSTR file_path = tnm_find_psb(file_name_list[i]);
		file_path_list.push_back(file_path);
	}

	tnm_emote_load(file_path_list, &m_emote_player);

	// �e�N�X�`�����쐬
	m_emote_texture = G_rsm.create_texture(_T("emote"), m_op.emote.size.cx, m_op.emote.size.cy, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);
	m_emote_dpst = G_rsm.create_dpst_buffer(_T("emote"), m_op.emote.size.cx, m_op.emote.size.cy, true);

#endif

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F���b�V���I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_mesh(TSTR file_path)
{
	// ���ɍ���Ă���ꍇ�͉������Ȃ�
	if (m_op.type == TNM_OBJECT_TYPE_MESH && m_op.file_path == file_path)
		return true;

	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_MESH;
	m_op.file_path = file_path;

	// ���b�V���I�u�W�F�N�g���č\�z����
	if (!restruct_mesh())
		return false;

	return true;
}

bool C_elm_object::restruct_mesh()
{
#if 1
	// ���b�V����ǂݍ���
	TSTR file_path = tnm_find_x(m_op.file_path);
	if (file_path.empty())
	{
		tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("3D�t�@�C�� ") + m_op.file_path + _T(" ��������܂���ł����B") + get_last_error_msg());
		m_op.file_path.clear();		// ���s�����̂Ńt�@�C�������N���A
		return false;
	}

	m_mesh = G_rsm.create_mesh_from_x(file_path, S_vtx_d3_pct::FVF, false);
	if (!m_mesh)
	{
		tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("3D�t�@�C�� ") + m_op.file_path + _T(" ���ǂݍ��߂܂���ł����B") + get_last_error_msg());
		m_op.file_path.clear();		// ���s�����̂Ńt�@�C�������N���A
		return false;
	}
#else
	// �t���[���Z�b�g��ǂݍ���
	m_frame_set = G_rsm.create_frame_set_from_x(Gp_dir->x_dir + _T("\\") + m_op.file_path + _T(".x"), false);
	if (!m_frame_set)
	{
		tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("3D�t�@�C�� ") + m_op.file_path + _T(" ���ǂݍ��߂܂���ł����B") + get_last_error_msg());
		m_op.file_path.clear();		// ���s�����̂Ńt�@�C�������N���A
		return false;
	}
#endif

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F�r���{�[�h�I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_billboard(TSTR file_path)
{
	// ���ɍ���Ă���ꍇ�͉������Ȃ�
	if (m_op.type == TNM_OBJECT_TYPE_BILLBOARD && m_op.file_path == file_path)
		return true;

	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_BILLBOARD;
	m_op.file_path = file_path;

	// �r���{�[�h�I�u�W�F�N�g���č\�z����
	if (!restruct_billboard())
		return false;

	return true;
}

bool C_elm_object::restruct_billboard()
{
	// �e�N�X�`����ǂݍ���
	m_album = tnm_load_pct_d3d(m_op.file_path, false, m_op_def.set_cg_table);
	if (!m_album)
		return false;

	// �X�v���C�g���쐬
	//if (!m_sprite->create_rect_indirect(S_vtx_d3_billboard::FVF, sizeof(S_vtx_d3_billboard)))
	//	return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�r���{�[�h�I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B"));

	return true;
}


// ****************************************************************
// �I�u�W�F�N�g�F�q�����쐬
// ================================================================
bool C_elm_object::alloc_child(int child_cnt)
{
	// �q�������
	m_child_list.clear();

	// ��O����
	if (child_cnt <= 0)
		return false;

	// �q�����쐬
	m_child_list.resize(child_cnt);

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F�t�@�C����ύX
// ================================================================
bool C_elm_object::change_file(CTSTR& file_path)
{
	// �^�C�v������i�����̂݁j
	free_type(false);

	// �p�����[�^��ݒ�
	m_op.file_path = file_path;
	
	// �^�C�v���č\�z
	if (!restruct_type())
		return false;

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F�t���[������������
// ================================================================
void C_elm_object::frame_init()
{
	// �����_�[�p�����[�^��������
	m_trp.init();

	// �X�v���C�g��������
	m_sprite->init();
	int sptite_cnt = (int)m_sprite_list.size();
	for (int i = 0; i < sptite_cnt; i++)
		m_sprite_list[i].init();

	// �{�^���f�[�^��������
	m_button.frame_init();

	// �q��
	int child_cnt = m_child_list.get_size();
	for (int i = 0; i < child_cnt; i++)
		m_child_list[i].frame_init();
}

// ****************************************************************
// �I�u�W�F�N�g�F���Ԃ��X�V
// ================================================================
void C_elm_object::update_time(int past_game_time, int past_real_time, int parent_flags)
{
	int flags = m_op.obp.flags | parent_flags;

	// �C�x���g�̃t���[�����Ԃ��X�V
	if (!(flags && TNM_OBJECT_FLAGS_NO_EVENT))
	{
		m_op.obp.pat_no.update_time(past_game_time, past_real_time);
		m_op.obp.pos_x.update_time(past_game_time, past_real_time);
		m_op.obp.pos_y.update_time(past_game_time, past_real_time);
		m_op.obp.pos_z.update_time(past_game_time, past_real_time);
		m_op.obp.center_x.update_time(past_game_time, past_real_time);
		m_op.obp.center_y.update_time(past_game_time, past_real_time);
		m_op.obp.center_z.update_time(past_game_time, past_real_time);
		m_op.obp.center_rep_x.update_time(past_game_time, past_real_time);
		m_op.obp.center_rep_y.update_time(past_game_time, past_real_time);
		m_op.obp.center_rep_z.update_time(past_game_time, past_real_time);
		m_op.obp.scale_x.update_time(past_game_time, past_real_time);
		m_op.obp.scale_y.update_time(past_game_time, past_real_time);
		m_op.obp.scale_z.update_time(past_game_time, past_real_time);
		m_op.obp.rotate_x.update_time(past_game_time, past_real_time);
		m_op.obp.rotate_y.update_time(past_game_time, past_real_time);
		m_op.obp.rotate_z.update_time(past_game_time, past_real_time);
		m_op.obp.dst_clip_left.update_time(past_game_time, past_real_time);
		m_op.obp.dst_clip_top.update_time(past_game_time, past_real_time);
		m_op.obp.dst_clip_right.update_time(past_game_time, past_real_time);
		m_op.obp.dst_clip_bottom.update_time(past_game_time, past_real_time);
		m_op.obp.src_clip_left.update_time(past_game_time, past_real_time);
		m_op.obp.src_clip_top.update_time(past_game_time, past_real_time);
		m_op.obp.src_clip_right.update_time(past_game_time, past_real_time);
		m_op.obp.src_clip_bottom.update_time(past_game_time, past_real_time);
		m_op.obp.tr.update_time(past_game_time, past_real_time);
		m_op.obp.mono.update_time(past_game_time, past_real_time);
		m_op.obp.reverse.update_time(past_game_time, past_real_time);
		m_op.obp.bright.update_time(past_game_time, past_real_time);
		m_op.obp.dark.update_time(past_game_time, past_real_time);
		m_op.obp.color_r.update_time(past_game_time, past_real_time);
		m_op.obp.color_g.update_time(past_game_time, past_real_time);
		m_op.obp.color_b.update_time(past_game_time, past_real_time);
		m_op.obp.color_rate.update_time(past_game_time, past_real_time);
		m_op.obp.color_add_r.update_time(past_game_time, past_real_time);
		m_op.obp.color_add_g.update_time(past_game_time, past_real_time);
		m_op.obp.color_add_b.update_time(past_game_time, past_real_time);

		int x_rep_cnt = m_op.x_rep_list.get_size();
		int y_rep_cnt = m_op.y_rep_list.get_size();
		int z_rep_cnt = m_op.z_rep_list.get_size();
		int tr_rep_cnt = m_op.tr_rep_list.get_size();
		for (int i = 0; i < x_rep_cnt; i++)
			m_op.x_rep_list[i].update_time(past_game_time, past_real_time);
		for (int i = 0; i < y_rep_cnt; i++)
			m_op.y_rep_list[i].update_time(past_game_time, past_real_time);
		for (int i = 0; i < z_rep_cnt; i++)
			m_op.z_rep_list[i].update_time(past_game_time, past_real_time);
		for (int i = 0; i < tr_rep_cnt; i++)
			m_op.tr_rep_list[i].update_time(past_game_time, past_real_time);
	}

	// �t���[���A�N�V�����̎��Ԃ��X�V
	m_op.frame_action.update_time(past_game_time, past_real_time);

	// �t���[���A�N�V�����`�����l���̎��Ԃ��X�V
	int frame_action_ch_cnt = m_op.frame_action_ch.get_size(); 
	for (int i = 0; i < frame_action_ch_cnt; i++)
		m_op.frame_action_ch[i].update_time(past_game_time, past_real_time);

	// �f�`�m�̎��Ԃ��X�V
	m_op.gan.update_time(past_game_time, past_real_time);

	// ���̎��Ԃ��X�V
	update_weather_time(past_game_time, past_real_time);

	// ���[�r�[�̎��Ԃ��X�V
	if (m_op.type == TNM_OBJECT_TYPE_MOVIE)
	{
		// ���[�r�[�͎����ԂŌv��
		if (m_op.movie.pause_flag == false)
		{
			if (m_op.movie.real_time_flag == true)
			{
				m_omv_timer += past_real_time;
			}
			else
			{
				m_omv_timer += past_game_time;
			}
		}

		// �I�����Ă���Ή��
		if (!m_omv_player->is_playing())
		{
			if (m_op.movie.auto_free_flag)
			{
				init_type(true);				// ��������t���O�������Ă���̂ŉ��
			}
			else
			{
				m_op.movie.pause_flag = 1;		// ����ȊO�͈ꎞ��~
			}
		}
	}

	// ���b�V���̎��Ԃ��X�V
	if (m_op.type == TNM_OBJECT_TYPE_MESH)
	{
		// �A�j���[�V������i�߂�i�ؕb �� �b�ɕϊ��j
		//m_sprite->advance_animation((float)past_game_time / 1000);
	}

	// E-mote �̎��Ԃ��X�V
#if __USE_EMOTE
	if (m_op.type == TNM_OBJECT_TYPE_EMOTE)
	{
		tnm_emote_frame(m_emote_player, (float)past_game_time);
	}
#endif

	// �q���̎��Ԃ��X�V
	int child_cnt = m_child_list.get_size();
	for (int i = 0; i < child_cnt; i++)
		m_child_list[i].update_time(past_game_time, past_real_time, flags);
}

// ****************************************************************
// �I�u�W�F�N�g�F�t���[������
// ================================================================
#define MMM_EVENT(param)	param.frame();
#define MMM_NO_EVENT(param)	param.frame_no_event();

bool C_elm_object::frame(const S_tnm_render_param* parent_trp, int parent_order, int parent_flags)
{
	int i;
	int flags = m_op.obp.flags | parent_flags;

	// �ŏI�ʒu�̏�����
	m_total_pos = C_point(0, 0);

	// �C�x���g�̃t���[������
	// �œK�����c�B���΂炭�̓}�N���̂܂܎c���Ă����ĉ������B2012/03/13
	if (flags && TNM_OBJECT_FLAGS_NO_EVENT)
	{
		// ������̓C�x���g���Ȃ����Ƃ��������Ă���ꍇ�̏����B�����ɏ������܂��B
		MMM_NO_EVENT(m_op.obp.pat_no)
		MMM_NO_EVENT(m_op.obp.pos_x)
		MMM_NO_EVENT(m_op.obp.pos_y)
		MMM_NO_EVENT(m_op.obp.pos_z)
		MMM_NO_EVENT(m_op.obp.center_x)
		MMM_NO_EVENT(m_op.obp.center_y)
		MMM_NO_EVENT(m_op.obp.center_z)
		MMM_NO_EVENT(m_op.obp.center_rep_x)
		MMM_NO_EVENT(m_op.obp.center_rep_y)
		MMM_NO_EVENT(m_op.obp.center_rep_z)
		MMM_NO_EVENT(m_op.obp.scale_x)
		MMM_NO_EVENT(m_op.obp.scale_y)
		MMM_NO_EVENT(m_op.obp.scale_z)
		MMM_NO_EVENT(m_op.obp.rotate_x)
		MMM_NO_EVENT(m_op.obp.rotate_y)
		MMM_NO_EVENT(m_op.obp.rotate_z)
		MMM_NO_EVENT(m_op.obp.dst_clip_left)
		MMM_NO_EVENT(m_op.obp.dst_clip_top)
		MMM_NO_EVENT(m_op.obp.dst_clip_right)
		MMM_NO_EVENT(m_op.obp.dst_clip_bottom)
		MMM_NO_EVENT(m_op.obp.src_clip_left)
		MMM_NO_EVENT(m_op.obp.src_clip_top)
		MMM_NO_EVENT(m_op.obp.src_clip_right)
		MMM_NO_EVENT(m_op.obp.src_clip_bottom)
		MMM_NO_EVENT(m_op.obp.tr)
		MMM_NO_EVENT(m_op.obp.mono)
		MMM_NO_EVENT(m_op.obp.reverse)
		MMM_NO_EVENT(m_op.obp.bright)
		MMM_NO_EVENT(m_op.obp.dark)
		MMM_NO_EVENT(m_op.obp.color_r)
		MMM_NO_EVENT(m_op.obp.color_g)
		MMM_NO_EVENT(m_op.obp.color_b)
		MMM_NO_EVENT(m_op.obp.color_rate)
		MMM_NO_EVENT(m_op.obp.color_add_r)
		MMM_NO_EVENT(m_op.obp.color_add_g)
		MMM_NO_EVENT(m_op.obp.color_add_b)

		// �C�����W�C�x���g�̃t���[������
		int x_rep_cnt = m_op.x_rep_list.get_size();
		int y_rep_cnt = m_op.y_rep_list.get_size();
		int z_rep_cnt = m_op.z_rep_list.get_size();
		int tr_rep_cnt = m_op.tr_rep_list.get_size();
		for (int i = 0; i < x_rep_cnt; i++)
			MMM_NO_EVENT(m_op.x_rep_list[i])
		for (int i = 0; i < y_rep_cnt; i++)
			MMM_NO_EVENT(m_op.y_rep_list[i])
		for (int i = 0; i < z_rep_cnt; i++)
			MMM_NO_EVENT(m_op.z_rep_list[i])
		for (int i = 0; i < tr_rep_cnt; i++)
			MMM_NO_EVENT(m_op.tr_rep_list[i])
	}
	else
	{
		// ������͒ʏ�̏����B�C�x���g���肠��B
		MMM_EVENT(m_op.obp.pat_no)
		MMM_EVENT(m_op.obp.pos_x)
		MMM_EVENT(m_op.obp.pos_y)
		MMM_EVENT(m_op.obp.pos_z)
		MMM_EVENT(m_op.obp.center_x)
		MMM_EVENT(m_op.obp.center_y)
		MMM_EVENT(m_op.obp.center_z)
		MMM_EVENT(m_op.obp.center_rep_x)
		MMM_EVENT(m_op.obp.center_rep_y)
		MMM_EVENT(m_op.obp.center_rep_z)
		MMM_EVENT(m_op.obp.scale_x)
		MMM_EVENT(m_op.obp.scale_y)
		MMM_EVENT(m_op.obp.scale_z)
		MMM_EVENT(m_op.obp.rotate_x)
		MMM_EVENT(m_op.obp.rotate_y)
		MMM_EVENT(m_op.obp.rotate_z)
		MMM_EVENT(m_op.obp.dst_clip_left)
		MMM_EVENT(m_op.obp.dst_clip_top)
		MMM_EVENT(m_op.obp.dst_clip_right)
		MMM_EVENT(m_op.obp.dst_clip_bottom)
		MMM_EVENT(m_op.obp.src_clip_left)
		MMM_EVENT(m_op.obp.src_clip_top)
		MMM_EVENT(m_op.obp.src_clip_right)
		MMM_EVENT(m_op.obp.src_clip_bottom)
		MMM_EVENT(m_op.obp.tr)
		MMM_EVENT(m_op.obp.mono)
		MMM_EVENT(m_op.obp.reverse)
		MMM_EVENT(m_op.obp.bright)
		MMM_EVENT(m_op.obp.dark)
		MMM_EVENT(m_op.obp.color_r)
		MMM_EVENT(m_op.obp.color_g)
		MMM_EVENT(m_op.obp.color_b)
		MMM_EVENT(m_op.obp.color_rate)
		MMM_EVENT(m_op.obp.color_add_r)
		MMM_EVENT(m_op.obp.color_add_g)
		MMM_EVENT(m_op.obp.color_add_b)

		// �C�����W�C�x���g�̃t���[������
		int x_rep_cnt = m_op.x_rep_list.get_size();
		int y_rep_cnt = m_op.y_rep_list.get_size();
		int z_rep_cnt = m_op.z_rep_list.get_size();
		int tr_rep_cnt = m_op.tr_rep_list.get_size();
		for (int i = 0; i < x_rep_cnt; i++)
			MMM_EVENT(m_op.x_rep_list[i])
		for (int i = 0; i < y_rep_cnt; i++)
			MMM_EVENT(m_op.y_rep_list[i])
		for (int i = 0; i < z_rep_cnt; i++)
			MMM_EVENT(m_op.z_rep_list[i])
		for (int i = 0; i < tr_rep_cnt; i++)
			MMM_EVENT(m_op.tr_rep_list[i])
	}

	// �t���[���A�N�V�����̃t���[������
	m_op.frame_action.frame();

	// �t���[���A�N�V�����`�����l��
	int frame_action_cnt = m_op.frame_action_ch.get_size();
	for (i = 0; i < frame_action_cnt; i++)
		m_op.frame_action_ch[i].frame();

	//
	// ���`�悷��H m_trp.disp ���Ɍv�Z����i�������̂��߁j
	// �\�[�^�[�Ȃǂ��������Ȃ��Ƃ��߂��ۂ��B
	//
	m_trp.disp = m_op.obp.disp ? true : false;
	m_trp.pat_no = m_op.obp.pat_no.get_total_value() + m_op.button.cut_no;
	m_trp.sorter = m_op.obp.sorter + S_tnm_sorter(parent_order, 0);
	m_trp.p_world = get_world();
	if (parent_trp)
	{
		m_trp.disp = parent_trp->disp && m_trp.disp;
		m_trp.sorter = m_trp.sorter + S_tnm_sorter(parent_trp->sorter.order, parent_trp->sorter.layer);
		if (m_trp.p_world == NULL)		// �q���f�t�H���g�ꍇ�̂ݐe�������p��
		{
			m_trp.p_world = parent_trp->p_world;
		}
	}

	//
	// ���`�悵�Ȃ��A���q�������Ȃ��ꍇ�͂����ŏI���B
	//
	if (!m_trp.disp && m_child_list.is_empty())
	{
		return true;
	}

	//
	// �V�X�e���{�^���̏ꍇ�A���[�h�ɂ���Ă͕\�����Ȃ�
	//
	if (m_op.button.sys_type != TNM_SYSCOM_TYPE_NONE)
	{
		int mode = 0;
		switch (m_op.button.sys_type)
		{
			case TNM_SYSCOM_TYPE_READ_SKIP:
				mode = tnm_syscom_read_skip_get_onoff_flag() ? 1 : 0;
				break;
			case TNM_SYSCOM_TYPE_AUTO_MODE:
				mode = tnm_syscom_auto_mode_get_onoff_flag() ? 1 : 0;
				break;
			case TNM_SYSCOM_TYPE_LOCAL_EXTRA_SWITCH:
				mode = tnm_syscom_local_extra_switch_get_onoff_flag(m_op.button.sys_type_opt) ? 1 : 0;
				break;
			case TNM_SYSCOM_TYPE_LOCAL_EXTRA_MODE:
				mode = tnm_syscom_local_extra_mode_get_value(m_op.button.sys_type_opt);
				break;
			case TNM_SYSCOM_TYPE_GLOBAL_EXTRA_SWITCH:
				if (0 <= m_op.button.sys_type_opt && m_op.button.sys_type_opt < (int)Gp_config->global_extra_switch_flag.size())	{
					mode = Gp_config->global_extra_switch_flag[m_op.button.sys_type_opt].value ? 1 : 0;
				}
				break;
			case TNM_SYSCOM_TYPE_GLOBAL_EXTRA_MODE:
				if (0 <= m_op.button.sys_type_opt && m_op.button.sys_type_opt < (int)Gp_config->global_extra_mode_flag.size())	{
					mode = Gp_config->global_extra_mode_flag[m_op.button.sys_type_opt];
				}
				break;
		}
		if (mode != m_op.button.mode)
		{
			m_trp.disp = false;	// ���[�h���Ⴄ�I�u�W�F�N�g�͕\�����Ȃ�
		}
	}

	// 
	// �u�I�u�W�F�N�g��\���v�̐ݒ������
	//
	if (0 <= m_op_def.object_disp_no && m_op_def.object_disp_no < Gp_ini->object_cnt)
	{
		if (0 <= m_op_def.object_disp_no && m_op_def.object_disp_no < (int)Gp_config->object_disp_flag.size())
		{
			if (!Gp_config->object_disp_flag[m_op_def.object_disp_no].value)
			{
				m_trp.disp = false;
			}
		}
	}

	//
	// �u�X�y�[�X�L�[�ŉB���v�̐ݒ������
	//
	if (m_op_def.space_hide_flag)
	{
		if (tnm_is_hide_mwnd())
		{
			m_trp.disp = false;
		}
	}

	//
	// ���`�悵�Ȃ��A���q�������Ȃ��ꍇ�͂����ŏI���B
	//
	if (!m_trp.disp && m_child_list.is_empty())
	{
		return true;
	}

	//
	// �����_�[�p�����[�^���쐬����
	//
	create_trp();

	// 
	// �e�̃����_�[�p�����[�^��������
	// 
	m_trp.sorter += S_tnm_sorter(parent_order, 0);
	if (parent_trp)
	{
		tnm_add_parent_trp(&m_trp, parent_trp);
	}

	//
	// �q���̃t���[������
	//
	int child_cnt = m_child_list.get_size();
	for (int i = 0; i < child_cnt; i++)
	{
		m_child_list[i].frame(&m_trp, 0, flags);
	}

	// �`�悵�Ȃ��ꍇ�͂����ŏI���B
	if (!m_trp.disp || m_trp.tr == 0)
	{
		return true;
	}

	// �q���ɒ��S�␳��n�����̂ł������S�␳�͕K�v�Ȃ�
	// ���W�ƒ��S���W�Ɉڍs����
	m_trp.pos_x += m_trp.center_rep_x;
	m_trp.pos_y += m_trp.center_rep_y;
	m_trp.pos_z += m_trp.center_rep_z;
	m_trp.center_x += m_trp.center_rep_x;
	m_trp.center_y += m_trp.center_rep_y;
	m_trp.center_z += m_trp.center_rep_z;
	m_trp.center_rep_x = 0.0f;
	m_trp.center_rep_y = 0.0f;
	m_trp.center_rep_z = 0.0f;

	//
	// �{�^���A�N�V��������
	//

	// �e�̃{�^���X�e�[�g�ɂ���ă{�^���X�e�[�g���v�Z
	int btn_state = TNM_BTN_STATE_NORMAL;
	if (parent_trp)
	{
		if (parent_trp->button_state == TNM_BTN_STATE_SELECT)
		{
			btn_state = TNM_BTN_STATE_SELECT;
		}
		else if (parent_trp->button_state == TNM_BTN_STATE_DISABLE)
		{
			btn_state = TNM_BTN_STATE_DISABLE;
		}
		else
		{
			btn_state = get_button_real_state();
		}
	}
	else
	{
		btn_state = get_button_real_state();
	}

	// �O���[�v�̃A�N�V������K�p
	C_float3 btn_rep_pos(0.0f, 0.0f, 0.0f);
	S_tnm_btn_action_template_pat* btn_action = get_action(btn_state);
	if (btn_action)
	{
		// �{�^���␳�����L�����Ă����i��œ������`��������j
		btn_rep_pos.x = (float)btn_action->rep_pos.x;
		btn_rep_pos.y = (float)btn_action->rep_pos.y;

		// �A�N�V���������Z
		m_trp.pat_no += btn_action->rep_pat_no;
		m_trp.pos_x += btn_action->rep_pos.x;
		m_trp.pos_y += btn_action->rep_pos.y;
		m_trp.tr = limit(0, m_trp.tr * btn_action->rep_tr / 255, 255);
		m_trp.bright = limit(0, m_trp.bright + btn_action->rep_bright, 255);
		m_trp.dark = limit(0, m_trp.dark + btn_action->rep_dark, 255);
	}

	//
	// �Q�c�J��������
	//
	if (m_trp.p_world && m_trp.p_world->get_mode() == TNM_CAMERA_MODE_2D)
	{
		C_point3 camera(m_trp.p_world->get_camera_eye_total_x(), m_trp.p_world->get_camera_eye_total_y(), m_trp.p_world->get_camera_eye_total_z());

		// �J������K�p
		if (m_trp.pos_z - camera.z > 0)
		{
			double camera_scale = (double)1000 / (m_trp.pos_z - camera.z);
			m_trp.pos_x = (float)((m_trp.pos_x - camera.x) * camera_scale + Gp_global->game_screen_size.cx / 2);
			m_trp.pos_y = (float)((m_trp.pos_y - camera.y) * camera_scale + Gp_global->game_screen_size.cy / 2);
			m_trp.scale_x = (float)(m_trp.scale_x * camera_scale);
			m_trp.scale_y = (float)(m_trp.scale_y * camera_scale);
			btn_rep_pos.x = (float)(btn_rep_pos.x * camera_scale);
			btn_rep_pos.y = (float)(btn_rep_pos.y * camera_scale);
		}
		else
		{
			// �J���������͕`�悵�Ȃ�
			m_trp.disp = false;
		}
	}

	// �`�悵�Ȃ��ꍇ�͂����ŏI���B
	if (!m_trp.disp || m_trp.tr == 0)
	{
		return true;
	}

	//
	// �X�v���C�g���쐬����
	//

	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_NONE)	{

		// NOP
	}
	else if (m_op.type == TNM_OBJECT_TYPE_RECT)	{

		// ��`�X�v���C�g���쐬
		if (!m_sprite->create_polygon_no_init())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("��`�I�u�W�F�N�g�̃X�v���C�g�̍쐬�Ɏ��s���܂����B"));
			return false;
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE || m_op.type == TNM_OBJECT_TYPE_EMOTE)	{

		// ��`�X�v���C�g���쐬
		if (!m_sprite->create_polygon_no_init())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("E-mote �I�u�W�F�N�g�̃X�v���C�g�̍쐬�Ɏ��s���܂����B"));
			return false;
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_STRING || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_WEATHER)	{

		// ��`�X�v���C�g���쐬
		int sprite_cnt = (int)m_sprite_list.size();
		for (int i = 0; i < sprite_cnt; i++)	{
			if (!m_sprite_list[i].create_polygon_no_init())	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�}���`�I�u�W�F�N�g�̃X�v���C�g�̍쐬�Ɏ��s���܂����B"));
				return false;
			}
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_MESH)	{

		// ���b�V���X�v���C�g�̍쐬
		if (!m_sprite->create_mesh_no_init())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���b�V���I�u�W�F�N�g�̃X�v���C�g�̍쐬�Ɏ��s���܂����B"));
			return false;
		}

		// �X�v���C�g�Ƀ��b�V����ݒ�
		m_sprite->set_mesh(m_mesh);
	}

	//
	// �^�C�v�ʃX�v���C�g�̐ݒ�
	//
	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_NONE)	{

		// ��{�̃����_�[�p�����[�^��ݒ�
		// �q��������ꍇ�����i�������̂��߁j
		if (!m_child_list.is_empty())	{
			trp_to_rp(&m_sprite->rpex, &m_sprite->rp);
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_RECT)	{

		// ��{�̃����_�[�p�����[�^��ݒ�
		trp_to_rp(&m_sprite->rpex, &m_sprite->rp);

		// �t�B���^�ݒ�
		m_sprite->rp.center.x -= m_op.filter.rect.left;				// �ʒu�𐮂���
		m_sprite->rp.center.y -= m_op.filter.rect.top;				// �ʒu�𐮂���
		m_sprite->rp.size.x = (float)m_op.filter.rect.width();		// �傫���𐮂���
		m_sprite->rp.size.y = (float)m_op.filter.rect.height();		// �傫���𐮂���
		m_sprite->rp.filter.color = m_op.filter.color;				// �F�����肷��
	}
	else if (m_op.type == TNM_OBJECT_TYPE_PCT)	{

		// ��{�̃����_�[�p�����[�^��ݒ�
		trp_to_rp(&m_sprite->rpex, &m_sprite->rp);

		// �p�^�[���ԍ�����e�N�X�`����ݒ�
		if (m_album && m_album->get_texture_cnt() > 0)	{
			if (0 <= m_trp.pat_no && m_trp.pat_no < m_album->get_texture_cnt())	{		// �p�^�[���ԍ����s���Ȃ�\�����Ȃ�
				m_sprite->set_texture(m_album->get_texture(m_trp.pat_no));
				m_sprite->rp.size_fit_to_texture = 0;
			}
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_MESH)	{

		// ��{�̃����_�[�p�����[�^��ݒ�
		trp_to_rp(&m_sprite->rpex, &m_sprite->rp);
	}
	else if (m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE)	{

		// ��{�̃����_�[�p�����[�^��ݒ�
		trp_to_rp(&m_sprite->rpex, &m_sprite->rp);

		// �e�N�X�`����ݒ�
		if (m_album && m_album->get_texture_cnt() > 0)	{
			m_sprite->set_texture(m_album->get_texture(0));
			m_sprite->rp.size_fit_to_texture = 0;
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_EMOTE)	{

		// ��{�̃����_�[�p�����[�^��ݒ�
		trp_to_rp(&m_sprite->rpex, &m_sprite->rp);

		// �e�N�X�`����ݒ�
		m_sprite->set_texture(m_emote_texture);
		m_sprite->rp.size_fit_to_texture = 0;
	}
	else if (m_op.type == TNM_OBJECT_TYPE_MOVIE)	{

		movie_frame();
	}
	else if (m_op.type == TNM_OBJECT_TYPE_STRING)	{

		string_frame();
	}
	else if (m_op.type == TNM_OBJECT_TYPE_NUMBER)	{

		number_frame();
	}
	else if (m_op.type == TNM_OBJECT_TYPE_WEATHER)	{

		weather_frame();
	}

	//
	// �}�X�N��ݒ�
	//
	if (m_trp.mask_no != TNM_MASK_NONE)	{

#if 0
		// �ĂP�D�I�u�W�F�N�g���}�X�N�Ƃ��Ďg��
		bool mask_ok = false;
		if (m_op_def.p_object_list)	{
			int object_cnt = m_op_def.p_object_list->get_size();
			if (0 <= m_trp.mask_no && m_trp.mask_no < object_cnt)	{
				C_elm_object* p_object = &(*m_op_def.p_object_list)[m_trp.mask_no];
				BSP<C_d3d_album> p_album = p_object->get_album();
				if (p_album)	{
					BSP<C_d3d_texture> p_texture = p_album->get_texture(0);
					if (p_texture)	{

						if (m_op.type == TNM_OBJECT_TYPE_RECT || m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_MOVIE)	{
							m_sprite->rp.mask = true;
							m_sprite->set_texture(p_texture, TNM_TEXTURE_STAGE_MASK);
						}
						else if (m_op.type == TNM_OBJECT_TYPE_STRING || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_WEATHER)	{
							int sprite_cnt = (int)m_sprite_list.size();
							for (int i = 0; i < sprite_cnt; i++)	{
								m_sprite_list[i].rp.mask = true;
								m_sprite_list[i].set_texture(p_texture, TNM_TEXTURE_STAGE_MASK);
							}
						}
						mask_ok = true;	// �}�X�N�̐ݒ�ɐ���
					}
				}
			}
		}
		// �}�X�N�̐ݒ�Ɏ��s�����ꍇ�̓}�X�N��������
		if (!mask_ok)	{
			m_trp.mask_no = TNM_MASK_NONE;
		}
#else
		// �ĂQ�D�}�X�N�G�������g���g��
		BSP<C_d3d_texture> p_mask_texture = tnm_get_mask_texture(m_trp.mask_no);
		if (p_mask_texture)	{
			int mask_x = 0, mask_y = 0;
			tnm_get_mask_total_param(m_trp.mask_no, &mask_x, &mask_y);

			if (m_op.type == TNM_OBJECT_TYPE_NONE)	{

				// NOP
			}
			else if (m_op.type == TNM_OBJECT_TYPE_RECT || m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_MOVIE || m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
				m_sprite->rp.use_mask = true;
				m_sprite->rp.mask_x = mask_x;
				m_sprite->rp.mask_y = mask_y;
				m_sprite->set_texture(p_mask_texture, TNM_TEXTURE_STAGE_MASK);
			}
			else if (m_op.type == TNM_OBJECT_TYPE_STRING || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_WEATHER)	{
				int sprite_cnt = (int)m_sprite_list.size();
				for (int i = 0; i < sprite_cnt; i++)	{
					m_sprite_list[i].rp.use_mask = true;
					m_sprite_list[i].rp.mask_x = mask_x;
					m_sprite_list[i].rp.mask_y = mask_y;
					m_sprite_list[i].set_texture(p_mask_texture, TNM_TEXTURE_STAGE_MASK);
				}
			}
		}
		else	{
			m_trp.mask_no = TNM_MASK_NONE;	// �}�X�N��������
		}
#endif
	}

	//
	// �g�[���J�[�u��ݒ�
	//
	if (m_trp.tone_curve_no != TNM_TONE_CURVE_NONE)	{
		if (Gp_tonecurve->check_tonecurve_no(m_trp.tone_curve_no))	{
			int sat = Gp_tonecurve->get_sat(m_trp.tone_curve_no);		// �ʓx���擾
			BYTE mono = (BYTE)linear_limit(sat, -100, 255, 0, 0);	// ���m�N���x�ɒ���

			if (m_op.type == TNM_OBJECT_TYPE_NONE)	{

				// NOP
			}
			else if (m_op.type == TNM_OBJECT_TYPE_RECT || m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE || m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
				m_sprite->rp.tone_curve_no = m_trp.tone_curve_no;
				m_sprite->rp.tone_curve_sat = mono;
				m_sprite->set_texture(Gp_tonecurve->get_texture(), TNM_TEXTURE_STAGE_TONE_CURVE);
			}
			else if (m_op.type == TNM_OBJECT_TYPE_STRING || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_WEATHER)	{
				int sprite_cnt = (int)m_sprite_list.size();
				for (int i = 0; i < sprite_cnt; i++)	{
					m_sprite_list[i].rp.tone_curve_no = m_trp.tone_curve_no;
					m_sprite_list[i].rp.tone_curve_sat = mono;
					m_sprite_list[i].set_texture(Gp_tonecurve->get_texture(), TNM_TEXTURE_STAGE_TONE_CURVE);
				}
			}
		}
		else	{
			m_trp.tone_curve_no = TNM_TONE_CURVE_NONE;	// �g�[���J�[�u��������
		}
	}

	//
	// ���[���h�ɂ���ċ�`�^�C�v�ƒ��_�o�b�t�@��ݒ�
	//

	// ���[���h�Ȃ��܂��͂Q�c�J�����̏ꍇ
	if (m_trp.p_world == NULL || m_trp.p_world->get_mode() == TNM_CAMERA_MODE_2D)
	{
		if (m_op.type == TNM_OBJECT_TYPE_NONE)
		{
			// NOP
		}
		else if (m_op.type == TNM_OBJECT_TYPE_RECT)
		{
			m_sprite->rp.d2_rect = true;
			m_sprite->set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
			m_sprite->set_index_buffer(G_rect_index_buffer);
		}
		else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE || m_op.type == TNM_OBJECT_TYPE_EMOTE)
		{
			// �}�X�N�̂���Ȃ��Œ��_�o�b�t�@���ς��
			if (m_trp.mask_no == TNM_MASK_NONE)
			{
				m_sprite->rp.d2_rect = true;
				m_sprite->set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
				m_sprite->set_index_buffer(G_rect_index_buffer);
			}
			else
			{
				m_sprite->rp.d2_rect = true;
				m_sprite->set_vertex_buffer(G_rect_vertex_buffer_d2_pct_mask);
				m_sprite->set_index_buffer(G_rect_index_buffer);
			}
		}
		else if (m_op.type == TNM_OBJECT_TYPE_STRING || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_WEATHER)
		{
			// �}�X�N�̂���Ȃ��Œ��_�o�b�t�@���ς��
			if (m_trp.mask_no == TNM_MASK_NONE)
			{
				int sprite_cnt = (int)m_sprite_list.size();
				for (int i = 0; i < sprite_cnt; i++)
				{
					m_sprite_list[i].rp.d2_rect = true;
					m_sprite_list[i].set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
					m_sprite_list[i].set_index_buffer(G_rect_index_buffer);
				}
			}
			else
			{
				int sprite_cnt = (int)m_sprite_list.size();
				for (int i = 0; i < sprite_cnt; i++)
				{
					m_sprite_list[i].rp.d2_rect = true;
					m_sprite_list[i].set_vertex_buffer(G_rect_vertex_buffer_d2_pct_mask);
					m_sprite_list[i].set_index_buffer(G_rect_index_buffer);
				}
			}
		}
	}

	// �R�c�J�����̏ꍇ
	else
	{
		if (m_op.type == TNM_OBJECT_TYPE_NONE)
		{
			// NOP
		}
		else if (m_op.type == TNM_OBJECT_TYPE_RECT)
		{
			m_sprite->rp.d3_rect = true;
			m_sprite->set_vertex_buffer(G_rect_vertex_buffer_d3_filter);
			m_sprite->set_index_buffer(G_rect_index_buffer);
		}
		else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE || m_op.type == TNM_OBJECT_TYPE_EMOTE)
		{
			m_sprite->rp.d3_rect = true;
			m_sprite->set_vertex_buffer(G_rect_vertex_buffer_d3_pct);
			m_sprite->set_index_buffer(G_rect_index_buffer);

			// ���t�H�O�ƃ��C�g��ݒ肵�Ă݂�
			m_sprite->rp.light_no = m_op.obp.light_no;
			if (m_op.obp.use_fog)
			{
				m_sprite->rp.use_fog = true;
				BSP<C_d3d_album> fog_album = tnm_get_fog_album();
				if (fog_album)
				{
					m_sprite->set_texture(fog_album->get_texture(0), TNM_TEXTURE_STAGE_FOG);
				}
			}
		}
		else if (m_op.type == TNM_OBJECT_TYPE_STRING || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_WEATHER)
		{
			int sprite_cnt = (int)m_sprite_list.size();
			for (int i = 0; i < sprite_cnt; i++)
			{
				m_sprite_list[i].rp.d3_rect = true;			// �R�c��`
				m_sprite_list[i].set_vertex_buffer(G_rect_vertex_buffer_d3_pct);
				m_sprite_list[i].set_index_buffer(G_rect_index_buffer);
			}
		}
		else if (m_op.type == TNM_OBJECT_TYPE_MESH)
		{
			m_sprite->rp.alpha_blend = false;	// �A���t�@�u�����h�Ȃ�

			// ���t�H�O�ƃ��C�g��ݒ肵�Ă݂�
			m_sprite->rp.light_no = m_op.obp.light_no;
			if (m_op.obp.use_fog)	{
				m_sprite->rp.use_fog = true;
			}
		}
	}

	//
	// �ŏI�ʒu�Ɠ������`���v�Z����
	//
	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_NONE)
	{
		// NOP
	}
	else if (m_op.type == TNM_OBJECT_TYPE_RECT)
	{
		// �\������ꍇ�͓������`���v�Z
		if (m_sprite->rp.disp)
		{
			float total_pos_x = m_sprite->rp.pos.x - btn_rep_pos.x;	// �{�^���A�N�V�����������ɖ߂�
			float total_pos_y = m_sprite->rp.pos.y - btn_rep_pos.y;	// �{�^���A�N�V�����������ɖ߂�
			float total_rect_l = -m_sprite->rp.center.x;
			float total_rect_t = -m_sprite->rp.center.y;
			float total_rect_r = -m_sprite->rp.center.x + m_sprite->rp.size.x;
			float total_rect_b = -m_sprite->rp.center.y + m_sprite->rp.size.y;

			// �g�p��`�ƍ��킹��
			if (m_trp.src_clip_use)
			{
				total_rect_l = std::max(total_rect_l, (float)m_trp.src_clip_left);
				total_rect_t = std::max(total_rect_t, (float)m_trp.src_clip_top);
				total_rect_r = std::min(total_rect_r, (float)m_trp.src_clip_right);
				total_rect_b = std::min(total_rect_b, (float)m_trp.src_clip_bottom);
			}

			// �g�k
			total_rect_l = total_rect_l * m_sprite->rp.scale.x;
			total_rect_t = total_rect_t * m_sprite->rp.scale.y;
			total_rect_r = total_rect_r * m_sprite->rp.scale.x;
			total_rect_b = total_rect_b * m_sprite->rp.scale.y;

			// �����ŋ�`���΍��W�ɒ���
			total_rect_l += total_pos_x;
			total_rect_t += total_pos_y;
			total_rect_r += total_pos_x;
			total_rect_b += total_pos_y;

			// �{�^����ݒ�
			m_button.set_param(C_rect((LONG)total_rect_l, (LONG)total_rect_t, (LONG)total_rect_r, (LONG)total_rect_b), m_trp.sorter);
			m_button.set_center(C_point((int)total_pos_x, (int)total_pos_y));
			m_button.set_rotate((int)(m_trp.rotate_z / D3DX_PI * 1800));
			m_button.set_push_keep(m_op.button.push_keep ? true : false);

			// �\����`�͑�Q��`�Ƃ��ė^����
			if (m_sprite->rp.dst_clip_use)
			{
				m_button.set_rect_2(m_sprite->rp.dst_clip_use, m_sprite->rp.dst_clip);
			}

			// �ŏI���W��ۑ�
			m_total_pos = C_point((int)total_pos_x, (int)total_pos_y);
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE || m_op.type == TNM_OBJECT_TYPE_EMOTE)
	{
		// �e�N�X�`��������ꍇ�͓������`���v�Z
		if (m_sprite->rp.disp && m_sprite->get_texture())
		{
			float total_pos_x = m_sprite->rp.pos.x - btn_rep_pos.x;	// �{�^���A�N�V�����������ɖ߂�
			float total_pos_y = m_sprite->rp.pos.y - btn_rep_pos.y;	// �{�^���A�N�V�����������ɖ߂�
			float total_rect_l = -m_sprite->rp.center.x - m_sprite->get_texture()->get_center().x;
			float total_rect_t = -m_sprite->rp.center.y - m_sprite->get_texture()->get_center().y;
			float total_rect_r = -m_sprite->rp.center.x - m_sprite->get_texture()->get_center().x + m_sprite->get_texture()->get_size().cx;
			float total_rect_b = -m_sprite->rp.center.y - m_sprite->get_texture()->get_center().y + m_sprite->get_texture()->get_size().cy;

			// �g�p��`�ƍ��킹��
			if (m_trp.src_clip_use)
			{
				total_rect_l = std::max(total_rect_l, (float)m_trp.src_clip_left);
				total_rect_t = std::max(total_rect_t, (float)m_trp.src_clip_top);
				total_rect_r = std::min(total_rect_r, (float)m_trp.src_clip_right);
				total_rect_b = std::min(total_rect_b, (float)m_trp.src_clip_bottom);
			}

			// �g�k
			total_rect_l = total_rect_l * m_sprite->rp.scale.x;
			total_rect_t = total_rect_t * m_sprite->rp.scale.y;
			total_rect_r = total_rect_r * m_sprite->rp.scale.x;
			total_rect_b = total_rect_b * m_sprite->rp.scale.y;

			// �����ŋ�`���΍��W�ɒ���
			total_rect_l += total_pos_x;
			total_rect_t += total_pos_y;
			total_rect_r += total_pos_x;
			total_rect_b += total_pos_y;

			// �{�^����ݒ�
			m_button.set_param(C_rect((LONG)total_rect_l, (LONG)total_rect_t, (LONG)total_rect_r, (LONG)total_rect_b), m_trp.sorter);
			m_button.set_center(C_point((int)total_pos_x, (int)total_pos_y));
			m_button.set_rotate((int)(m_trp.rotate_z / D3DX_PI * 1800));
			m_button.set_push_keep(m_op.button.push_keep ? true : false);

			// �����x���l������ꍇ
			if (m_op.button.alpha_test)
			{
				m_button.set_texture(m_sprite->get_texture());
			}

			// �\����`�͑�Q��`�Ƃ��ė^����
			if (m_sprite->rp.dst_clip_use)
			{
				m_button.set_rect_2(m_sprite->rp.dst_clip_use, m_sprite->rp.dst_clip);
			}

			// �ŏI���W��ۑ�
			m_total_pos = C_point((int)total_pos_x, (int)total_pos_y);
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_STRING || m_op.type == TNM_OBJECT_TYPE_NUMBER)
	{
		// �������`�Ɗg���p�t���O
		float total_pos_x = m_trp.pos_x - btn_rep_pos.x;	// �{�^���A�N�V�����������ɖ߂�
		float total_pos_y = m_trp.pos_y - btn_rep_pos.y;	// �{�^���A�N�V�����������ɖ߂�
		float total_rect_l = 0.0f;
		float total_rect_t = 0.0f;
		float total_rect_r = 0.0f;
		float total_rect_b = 0.0f;

		bool first = true;

		for (int i = 0; i < (int)m_sprite_list.size(); i++)
		{
			if (m_sprite_list[i].get_texture())
			{
				// ���̕����̓������`
				float this_hit_rect_l = m_sprite_list[i].rp.pos.x - m_sprite_list[i].rp.center.x - total_pos_x - btn_rep_pos.x;	// �{�^���A�N�V�����������ɖ߂�
				float this_hit_rect_t = m_sprite_list[i].rp.pos.y - m_sprite_list[i].rp.center.y - total_pos_y - btn_rep_pos.y;	// �{�^���A�N�V�����������ɖ߂�
				float this_hit_rect_r = m_sprite_list[i].rp.pos.x - m_sprite_list[i].rp.center.x + m_sprite_list[i].get_texture()->get_original_width() - total_pos_x - btn_rep_pos.x;	// �{�^���A�N�V�����������ɖ߂�
				float this_hit_rect_b = m_sprite_list[i].rp.pos.y - m_sprite_list[i].rp.center.y + m_sprite_list[i].get_texture()->get_original_height() - total_pos_y;	// �{�^���A�N�V�����������ɖ߂�

				// �������`���g������
				if (first || this_hit_rect_l <= total_rect_l)	total_rect_l = this_hit_rect_l;
				if (first || this_hit_rect_t <= total_rect_t)	total_rect_t = this_hit_rect_t;
				if (first || this_hit_rect_r >= total_rect_r)	total_rect_r = this_hit_rect_r;
				if (first || this_hit_rect_b >= total_rect_b)	total_rect_b = this_hit_rect_b;
				first = false;
			}
		}

		// �g�p��`�ƍ��킹��
		if (m_trp.src_clip_use)
		{
			total_rect_l = std::max(total_rect_l, (float)m_trp.src_clip_left);
			total_rect_t = std::max(total_rect_t, (float)m_trp.src_clip_top);
			total_rect_r = std::min(total_rect_r, (float)m_trp.src_clip_right);
			total_rect_b = std::min(total_rect_b, (float)m_trp.src_clip_bottom);
		}

		// �g�k
		total_rect_l = total_rect_l * m_sprite->rp.scale.x;
		total_rect_t = total_rect_t * m_sprite->rp.scale.y;
		total_rect_r = total_rect_r * m_sprite->rp.scale.x;
		total_rect_b = total_rect_b * m_sprite->rp.scale.y;

		// �����ŋ�`���΍��W�ɒ���
		total_rect_l += total_pos_x;
		total_rect_t += total_pos_y;
		total_rect_r += total_pos_x;
		total_rect_b += total_pos_y;

		// �������`��ݒ�
		m_button.set_param(C_rect((LONG)total_rect_l, (LONG)total_rect_t, (LONG)total_rect_r, (LONG)total_rect_b), m_trp.sorter);
		m_button.set_center(C_point((int)total_pos_x, (int)total_pos_y));
		m_button.set_rotate((int)(m_trp.rotate_z / D3DX_PI * 1800));
		m_button.set_push_keep(m_op.button.push_keep ? true : false);

		// �\����`�͑�Q��`�Ƃ��ė^����
		if (m_sprite->rp.dst_clip_use)
		{
			m_button.set_rect_2(m_sprite->rp.dst_clip_use, m_sprite->rp.dst_clip);
		}

		// �ŏI���W��ۑ�
		m_total_pos = C_point((int)total_pos_x, (int)total_pos_y);
	}

	// �ŏI�I�ɕs�����ȏꍇ�̓A���t�@�u�����h�Ȃ��ɂ��č�����
	switch (m_op.type)
	{
		case TNM_OBJECT_TYPE_MOVIE:
		{
			if (m_sprite->rp.tr == 255)
			{
				if (m_sprite->rp.alpha_test || m_sprite->rp.alpha_blend)
				{
					if (m_omv_player)
					{
						S_omv_theora_info theora_info;
						m_omv_player->get_theora_info(&theora_info);
						if (theora_info.type == OMV_THEORA_TYPE_RGB)
						{
							m_sprite->rp.alpha_test = false;
							m_sprite->rp.alpha_blend = false;

							// �����F�A���t�@�u�����h���I�t�ɂ���ƁA�g�k���ɉ��������Ȃ�܂��i���Ԃ�j
							// �Ȃ̂ŕ��ʂ͂��Ă͂����܂���B�����̏d�����[�r�[��������ɂ���Ă݂܂��B
						}
					}
				}
			}
		}
	}

#if __USE_EMOTE
	if (m_op.type == TNM_OBJECT_TYPE_EMOTE)
	{
		// ���p�N�ݒ�
		if (!Gp_local->pod.emote_mouth_stop_flag && m_op.emote.koe_chara_no >= 0)
		{
			if (m_op.emote.koe_chara_no == Gp_sound->m_koe.get_chara_no() && !Gp_sound->m_koe.get_ex_koe_flag())
			{
				if (Gp_sound->m_koe.is_playing())
				{
					float mouth_volume = Gp_sound->m_koe.get_current_mouth_volume();
					m_emote_player->SetVariable("face_talk", mouth_volume);
				}
				else
				{
					m_emote_player->SetVariable("face_talk", (float)m_op.emote.koe_mouth_volume / 1000);
				}
			}
			else
			{
				m_emote_player->SetVariable("face_talk", (float)m_op.emote.koe_mouth_volume / 1000);
			}
		}
		else
		{
			m_emote_player->SetVariable("face_talk", (float)m_op.emote.koe_mouth_volume / 1000);
		}

		// �X�e�[�g���e���v���[�g�ŏ���������
		G_d3d.state.set_template();
		// �r���[�|�[�g���e�N�X�`���S�̂ɂ���
		G_d3d.device.set_view_port(C_point(0, 0), m_emote_texture->get_size_ex());
		// �����_�[�^�[�Q�b�g���e�N�X�`���ɐݒ肷��
		Gp_wnd->m_view.set_render_target(m_emote_texture->get_surface(0).body());
		Gp_wnd->m_view.set_dpst_buffer(m_emote_dpst->body());
		// �����_�[�^�[�Q�b�g���N���A
		Gp_wnd->m_view.clear_render_target(0);
		// E-mote �`��
		tnm_emote_draw(m_emote_player, m_emote_texture, (float)m_op.emote.rep_x, (float)m_op.emote.rep_y);

		// E-mote �̃X�v���C�g��ݒ�
		m_sprite->rp.alpha_test = true;
		m_sprite->rp.alpha_blend = true;
	}
#endif

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�Fm_trp ���쐬����
// ================================================================
void C_elm_object::create_trp()
{
	// ��{�ݒ�
	//m_trp.disp = m_op.obp.disp ? true : false;		// ��s���Đݒ�ς݂Ȃ̂ł����ł͉������܂���B
	m_trp.pat_no = m_op.obp.pat_no.get_total_value() + m_op.button.cut_no;
	m_trp.sorter = m_op.obp.sorter;
	m_trp.p_world = get_world();

	// ���_�֘A
	m_trp.pos_x = (float)(m_op.obp.pos_x.get_total_value());
	m_trp.pos_y = (float)(m_op.obp.pos_y.get_total_value());
	m_trp.pos_z = (float)(m_op.obp.pos_z.get_total_value());
	m_trp.center_x = (float)(m_op.obp.center_x.get_total_value());
	m_trp.center_y = (float)(m_op.obp.center_y.get_total_value());
	m_trp.center_z = (float)(m_op.obp.center_z.get_total_value());
	m_trp.center_rep_x = (float)(m_op.obp.center_rep_x.get_total_value());
	m_trp.center_rep_y = (float)(m_op.obp.center_rep_y.get_total_value());
	m_trp.center_rep_z = (float)(m_op.obp.center_rep_z.get_total_value());
	m_trp.scale_x = (float)m_op.obp.scale_x.get_total_value() / TNM_SCALE_UNIT;
	m_trp.scale_y = (float)m_op.obp.scale_y.get_total_value() / TNM_SCALE_UNIT;
	m_trp.scale_z = (float)m_op.obp.scale_z.get_total_value() / TNM_SCALE_UNIT;
	m_trp.rotate_x = D3DXToRadian((float)m_op.obp.rotate_x.get_total_value() / TNM_ANGLE_UNIT);
	m_trp.rotate_y = D3DXToRadian((float)m_op.obp.rotate_y.get_total_value() / TNM_ANGLE_UNIT);
	m_trp.rotate_z = D3DXToRadian((float)m_op.obp.rotate_z.get_total_value() / TNM_ANGLE_UNIT);
	m_trp.dst_clip_use = m_op.obp.dst_clip_use;
	m_trp.dst_clip_left = m_op.obp.dst_clip_left.get_total_value();
	m_trp.dst_clip_top = m_op.obp.dst_clip_top.get_total_value();
	m_trp.dst_clip_right = m_op.obp.dst_clip_right.get_total_value();
	m_trp.dst_clip_bottom = m_op.obp.dst_clip_bottom.get_total_value();
	m_trp.src_clip_use = m_op.obp.src_clip_use;
	m_trp.src_clip_left = m_op.obp.src_clip_left.get_total_value();
	m_trp.src_clip_top = m_op.obp.src_clip_top.get_total_value();
	m_trp.src_clip_right = m_op.obp.src_clip_right.get_total_value();
	m_trp.src_clip_bottom = m_op.obp.src_clip_bottom.get_total_value();

	// �F�֘A
	m_trp.tr = m_op.obp.tr.get_total_value();
	m_trp.mono = m_op.obp.mono.get_total_value();
	m_trp.reverse = m_op.obp.reverse.get_total_value();
	m_trp.bright = m_op.obp.bright.get_total_value();
	m_trp.dark = m_op.obp.dark.get_total_value();
	m_trp.color_r = m_op.obp.color_r.get_total_value();
	m_trp.color_g = m_op.obp.color_g.get_total_value();
	m_trp.color_b = m_op.obp.color_b.get_total_value();
	m_trp.color_rate = m_op.obp.color_rate.get_total_value();
	m_trp.color_add_r = m_op.obp.color_add_r.get_total_value();
	m_trp.color_add_g = m_op.obp.color_add_g.get_total_value();
	m_trp.color_add_b = m_op.obp.color_add_b.get_total_value();
	m_trp.mask_no = m_op.obp.mask_no;
	m_trp.tone_curve_no = m_op.obp.tone_curve_no;

	// �����֘A
	m_trp.culling = m_op.obp.culling ? true : false;
	m_trp.alpha_test = m_op.obp.alpha_test ? true : false;
	m_trp.alpha_blend = m_op.obp.alpha_blend ? true : false;
	m_trp.blend_type = m_op.obp.blend_type;

	// �C�����W
	int x_rep_cnt = m_op.x_rep_list.get_size();
	int y_rep_cnt = m_op.y_rep_list.get_size();
	int z_rep_cnt = m_op.z_rep_list.get_size();
	int tr_rep_cnt = m_op.tr_rep_list.get_size();
	for (int i = 0; i < x_rep_cnt; i++)
		m_trp.pos_x += m_op.x_rep_list[i].get_total_value();
	for (int i = 0; i < y_rep_cnt; i++)
		m_trp.pos_y += m_op.y_rep_list[i].get_total_value();
	for (int i = 0; i < z_rep_cnt; i++)
		m_trp.pos_z += m_op.z_rep_list[i].get_total_value();
	for (int i = 0; i < tr_rep_cnt; i++)
		m_trp.tr = m_trp.tr * m_op.tr_rep_list[i].get_total_value() / 255;

	// �f�`�m
	m_trp.pat_no += m_op.gan.get_pat_data_pat_no();
	m_trp.pos_x += m_op.gan.get_pat_data_x();
	m_trp.pos_y += m_op.gan.get_pat_data_y();
	m_trp.tr = (m_trp.tr * m_op.gan.get_pat_data_tr()) / 255;
}

// ****************************************************************
// �I�u�W�F�N�g�Fm_trp �� rp
// ================================================================
void C_elm_object::trp_to_rp(S_tnm_sprite_render_param* rpex, S_d3d_render_param* rp)
{
	tnm_trp_to_rp(&m_trp, rpex, rp);
}

// ****************************************************************
// �I�u�W�F�N�g�F�X�v���C�g�c���[���\�z
// ================================================================
bool xxx_texture_sort(C_elm_object* lhs, C_elm_object* rhs)
{
	return lhs->get_sprite()->get_texture() < rhs->get_sprite()->get_texture();
}
bool xxx_x_sort(C_elm_object* lhs, C_elm_object* rhs)
{
	return lhs->get_pos_x() < rhs->get_pos_x();
}
bool xxx_x_rev_sort(C_elm_object* lhs, C_elm_object* rhs)
{
	return lhs->get_pos_x() > rhs->get_pos_x();
}
bool xxx_y_sort(C_elm_object* lhs, C_elm_object* rhs)
{
	return lhs->get_pos_y() < rhs->get_pos_y();
}
bool xxx_y_rev_sort(C_elm_object* lhs, C_elm_object* rhs)
{
	return lhs->get_pos_y() > rhs->get_pos_y();
}
bool xxx_z_sort(C_elm_object* lhs, C_elm_object* rhs)
{
	return lhs->get_pos_z() < rhs->get_pos_z();
}
bool xxx_z_rev_sort(C_elm_object* lhs, C_elm_object* rhs)
{
	return lhs->get_pos_z() > rhs->get_pos_z();
}

void C_elm_object::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_NONE)	{
		if (m_trp.disp && m_trp.tr > 0)	{

			// �q��������ꍇ�̓_�~�[�̃X�v���C�g�̎q���Ƃ��ēo�^
			if (m_child_list.get_size() > 0)	{
				ARRAY< C_tree2<C_tnm_sprite *> >::iterator dummy = root.add_child(m_sprite.get());	// �P��X�v���C�g���_�~�[�Ƃ��ēo�^
				int child_cnt = m_child_list.get_size();

				if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_DEFAULT)	{

					// �X�v���C�g��ǉ�
					for (int i = 0; i < child_cnt; i++)	{
						m_child_list[i].get_sprite_tree(*dummy);
					}
				}
				else if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_NONE)	{

					// �X�v���C�g��ǉ�
					for (int i = 0; i < child_cnt; i++)	{
						m_child_list[i].get_sprite_tree(*dummy);
					}
					// �f�t�H���g�̃\�[�g���I�t�ɂ���
					m_sprite->rpex.child_sort_type = TNM_SPRITE_SORT_TYPE_NONE;
				}
				else	{

					// �e�N�X�`�����Ƀ\�[�g
					ARRAY<C_elm_object *> cp(child_cnt);
					for (int i = 0; i < child_cnt; i++)	{
						cp[i] = &m_child_list[i];
					}
					if (false);
					else if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_TEXTURE)	{
						std::stable_sort(cp.begin(), cp.end(), xxx_texture_sort);
					}
					else if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_X)	{
						std::stable_sort(cp.begin(), cp.end(), xxx_x_sort);
					}
					else if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_X_REV)	{
						std::stable_sort(cp.begin(), cp.end(), xxx_x_rev_sort);
					}
					else if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_Y)	{
						std::stable_sort(cp.begin(), cp.end(), xxx_y_sort);
					}
					else if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_Y_REV)	{
						std::stable_sort(cp.begin(), cp.end(), xxx_y_rev_sort);
					}
					else if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_Z)	{
						std::stable_sort(cp.begin(), cp.end(), xxx_z_sort);
					}
					else if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_Z_REV)	{
						std::stable_sort(cp.begin(), cp.end(), xxx_z_rev_sort);
					}

					// �X�v���C�g��ǉ�
					for (int i = 0; i < child_cnt; i++)	{
						cp[i]->get_sprite_tree(*dummy);
					}
					// �f�t�H���g�̃\�[�g���I�t�ɂ���
					m_sprite->rpex.child_sort_type = TNM_SPRITE_SORT_TYPE_NONE;
				}
			}
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_RECT || m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_MESH || m_op.type == TNM_OBJECT_TYPE_BILLBOARD || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE || m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
		if (m_trp.disp && m_trp.tr > 0)	{
			ARRAY< C_tree2<C_tnm_sprite *> >::iterator parent = root.add_child(m_sprite.get());	// �P��X�v���C�g��o�^

			// �q��������ꍇ�́��̃X�v���C�g�̎q���Ƃ��ēo�^
			if (m_child_list.get_size() > 0)	{
				int child_cnt = m_child_list.get_size();

				if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_DEFAULT)	{

					// �X�v���C�g��ǉ�
					for (int i = 0; i < child_cnt; i++)	{
						m_child_list[i].get_sprite_tree(*parent);
					}
				}
				else if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_NONE)	{

					// �X�v���C�g��ǉ�
					for (int i = 0; i < child_cnt; i++)	{
						m_child_list[i].get_sprite_tree(*parent);
					}
					// �f�t�H���g�̃\�[�g���I�t�ɂ���
					m_sprite->rpex.child_sort_type = TNM_SPRITE_SORT_TYPE_NONE;
				}
				else if (m_op.obp.child_sort_type == TNM_OBJECT_SORT_TYPE_TEXTURE)	{

					// �e�N�X�`�����Ƀ\�[�g
					ARRAY<C_elm_object *> cp(child_cnt);
					for (int i = 0; i < child_cnt; i++)	{
						cp[i] = &m_child_list[i];
					}
					std::stable_sort(cp.begin(), cp.end(), xxx_texture_sort);
					// �X�v���C�g��ǉ�
					for (int i = 0; i < child_cnt; i++)	{
						cp[i]->get_sprite_tree(*parent);
					}
					// �f�t�H���g�̃\�[�g���I�t�ɂ���
					m_sprite->rpex.child_sort_type = TNM_SPRITE_SORT_TYPE_NONE;
				}
			}
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_STRING || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_WEATHER)	{

		// ���e�������炦�ē����Ń\�[�g������ׂ��H���̏ꍇ�q���̈����́H

		ARRAY<C_tnm_sprite>::iterator end = m_sprite_list.end();
		for (ARRAY<C_tnm_sprite>::iterator itr = m_sprite_list.begin(); itr != end; ++itr)	{
			if (itr->rp.disp && itr->rp.tr > 0)	{
				root.add_child(&*itr);		// �X�v���C�g���X�g��o�^
			}
		}

		// �q��������ꍇ�̓_�~�[�̃X�v���C�g�̎q���Ƃ��ēo�^
		if (m_child_list.get_size() > 0)	{
			ARRAY< C_tree2<C_tnm_sprite *> >::iterator dummy = root.add_child(m_sprite.get());	// �P��X�v���C�g���_�~�[�Ƃ��ēo�^
			int child_cnt = m_child_list.get_size();
			for (int i = 0; i < child_cnt; i++)
				m_child_list[i].get_sprite_tree(*dummy);
		}
	}
}

// ****************************************************************
// �p�^�[���ԍ���ݒ�
// ================================================================
void C_elm_object::set_pat_no(int pat_no)
{
	// �p�^�[���ԍ����`�F�b�N
	if ((m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_NUMBER) && pat_no > 0)	{
		if (m_album && pat_no >= m_album->get_texture_cnt())	{

			// �p�^�[���ԍ��G���[
			tnm_set_error(TNM_ERROR_TYPE_PAT_NO, str_format(
				_T("�p�^�[���ԍ��͈̔͊O�G���[�ł��B�i%s�j\npatno = %d, �摜�̃J�b�g�� = %d")
				, get_element_name().c_str(), pat_no, m_album->get_texture_cnt()));
		}
	}

	m_op.obp.pat_no.set_value(pat_no);
}

// ****************************************************************
// �p�^�[�������擾
// ================================================================
int C_elm_object::get_pat_cnt()
{
	if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_NUMBER)	{
		if (m_album)	{
			return m_album->get_texture_cnt();
		}
	}

	return 0;	// ��{�� 0
}

// ****************************************************************
// �T�C�Y���擾
// ================================================================
int C_elm_object::get_size_x(int cut_no)
{
	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE)	{
		if (m_album && m_album->get_texture(cut_no))	{
			return m_album->get_texture(cut_no)->get_original_width();
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
		return m_op.emote.size.cx;
	}
	else if (m_op.type == TNM_OBJECT_TYPE_MESH)	{
		if (m_sprite->get_mesh())	{
			return (int)m_sprite->get_mesh()->get_size().x;
		}
	}

	return 0;
}

// ****************************************************************
// �T�C�Y���擾
// ================================================================
int C_elm_object::get_size_y(int cut_no)
{
	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE)	{
		if (m_album && m_album->get_texture(cut_no))	{
			return m_album->get_texture(cut_no)->get_original_height();
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
		return m_op.emote.size.cy;
	}
	else if (m_op.type == TNM_OBJECT_TYPE_MESH)	{
		if (m_sprite->get_mesh())	{
			return (int)m_sprite->get_mesh()->get_size().y;
		}
	}

	return 0;
}

// ****************************************************************
// �T�C�Y���擾
// ================================================================
int C_elm_object::get_size_z(int cut_no)
{
	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE)	{
		return 0;
	}
	else if (m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
		return 0;
	}
	else if (m_op.type == TNM_OBJECT_TYPE_MESH)	{
		if (m_sprite->get_mesh())	{
			return (int)m_sprite->get_mesh()->get_size().z;
		}
	}

	return 0;
}

// ****************************************************************
// �s�N�Z���̐F���擾
// ================================================================
bool C_elm_object::get_pixel_color(int x, int y, int cut_no, C_argb* p_color)
{
	if (!p_color)
		return false;

	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE)	{
		if (m_album && m_album->get_texture(cut_no))	{
			*p_color = m_album->get_texture(cut_no)->get_color(x, y);
			return true;
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
		if (m_emote_texture) {
			*p_color = m_emote_texture->get_color(x, y);
			return true;
		}
	}

	return false;
}

int C_elm_object::get_pixel_color_r(int x, int y, int cut_no)
{
	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE)	{
		if (m_album && m_album->get_texture(cut_no))	{
			return m_album->get_texture(cut_no)->get_color(x, y).r;
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
		if (m_emote_texture) {
			m_emote_texture->get_color(x, y).r;
		}
	}

	return 0;
}
int C_elm_object::get_pixel_color_g(int x, int y, int cut_no)
{
	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE)	{
		if (m_album && m_album->get_texture(cut_no))	{
			return m_album->get_texture(cut_no)->get_color(x, y).g;
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
		if (m_emote_texture) {
			m_emote_texture->get_color(x, y).g;
		}
	}

	return 0;
}
int C_elm_object::get_pixel_color_b(int x, int y, int cut_no)
{
	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE)	{
		if (m_album && m_album->get_texture(cut_no))	{
			return m_album->get_texture(cut_no)->get_color(x, y).b;
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
		if (m_emote_texture) {
			m_emote_texture->get_color(x, y).b;
		}
	}

	return 0;
}
int C_elm_object::get_pixel_color_a(int x, int y, int cut_no)
{
	if (false);
	else if (m_op.type == TNM_OBJECT_TYPE_PCT || m_op.type == TNM_OBJECT_TYPE_NUMBER || m_op.type == TNM_OBJECT_TYPE_SAVE_THUMB || m_op.type == TNM_OBJECT_TYPE_THUMB || m_op.type == TNM_OBJECT_TYPE_CAPTURE || m_op.type == TNM_OBJECT_TYPE_MOVIE)	{
		if (m_album && m_album->get_texture(cut_no))	{
			return m_album->get_texture(cut_no)->get_color(x, y).a;
		}
	}
	else if (m_op.type == TNM_OBJECT_TYPE_EMOTE)	{
		if (m_emote_texture) {
			m_emote_texture->get_color(x, y).a;
		}
	}

	return 0;
}

// ****************************************************************
// �I�u�W�F�N�g�F�N���b�N�֎~�̔���
// ================================================================
bool C_elm_object::check_click_disable(C_point mp, bool child_flag)
{
	if (m_op.click_disable)	{

		// �摜�̒��S�ʒu���擾
		int pct_x = m_total_pos.x;
		int pct_y = m_total_pos.y;

		if (get_pixel_color_a(mp.x - pct_x, mp.y - pct_y, 0) > 0)
			return true;	// �F������̂ŃN���b�N�֎~
	}

	// �q��
	if (child_flag)	{
		int child_cnt = m_child_list.get_size();
		for (int i = 0; i < child_cnt; i++)	{
			if (m_child_list[i].check_click_disable(mp, child_flag))
				return true;
		}
	}

	return false;
}

// ****************************************************************
// �I�u�W�F�N�g�F�C�x���g�̒�~
// ================================================================
void C_elm_object::end_all_event()
{
	m_op.obp.pat_no.end_event();
	m_op.obp.pos_x.end_event();
	m_op.obp.pos_y.end_event();
	m_op.obp.pos_z.end_event();
	m_op.obp.center_x.end_event();
	m_op.obp.center_y.end_event();
	m_op.obp.center_z.end_event();
	m_op.obp.center_rep_x.end_event();
	m_op.obp.center_rep_y.end_event();
	m_op.obp.center_rep_z.end_event();
	m_op.obp.scale_x.end_event();
	m_op.obp.scale_y.end_event();
	m_op.obp.scale_z.end_event();
	m_op.obp.rotate_x.end_event();
	m_op.obp.rotate_y.end_event();
	m_op.obp.rotate_z.end_event();
	m_op.obp.dst_clip_left.end_event();
	m_op.obp.dst_clip_top.end_event();
	m_op.obp.dst_clip_right.end_event();
	m_op.obp.dst_clip_bottom.end_event();
	m_op.obp.src_clip_left.end_event();
	m_op.obp.src_clip_top.end_event();
	m_op.obp.src_clip_right.end_event();
	m_op.obp.src_clip_bottom.end_event();
	m_op.obp.tr.end_event();
	m_op.obp.mono.end_event();
	m_op.obp.reverse.end_event();
	m_op.obp.bright.end_event();
	m_op.obp.dark.end_event();
	m_op.obp.color_r.end_event();
	m_op.obp.color_g.end_event();
	m_op.obp.color_b.end_event();
	m_op.obp.color_rate.end_event();
	m_op.obp.color_add_r.end_event();
	m_op.obp.color_add_g.end_event();
	m_op.obp.color_add_b.end_event();

	// �C�����W
	int x_rep_cnt = m_op.x_rep_list.get_size();
	int y_rep_cnt = m_op.y_rep_list.get_size();
	int z_rep_cnt = m_op.z_rep_list.get_size();
	int tr_rep_cnt = m_op.tr_rep_list.get_size();
	for (int i = 0; i < x_rep_cnt; i++)
		m_op.x_rep_list[i].end_event();
	for (int i = 0; i < y_rep_cnt; i++)
		m_op.y_rep_list[i].end_event();
	for (int i = 0; i < z_rep_cnt; i++)
		m_op.z_rep_list[i].end_event();
	for (int i = 0; i < tr_rep_cnt; i++)
		m_op.tr_rep_list[i].end_event();
}

// ****************************************************************
// �I�u�W�F�N�g�F�C�x���g�̔���
// ================================================================
bool C_elm_object::check_all_event()
{
	if (m_op.obp.pat_no.check_event())			return true;
	if (m_op.obp.pos_x.check_event())			return true;
	if (m_op.obp.pos_y.check_event())			return true;
	if (m_op.obp.pos_z.check_event())			return true;
	if (m_op.obp.center_x.check_event())		return true;
	if (m_op.obp.center_y.check_event())		return true;
	if (m_op.obp.center_z.check_event())		return true;
	if (m_op.obp.center_rep_x.check_event())	return true;
	if (m_op.obp.center_rep_y.check_event())	return true;
	if (m_op.obp.center_rep_z.check_event())	return true;
	if (m_op.obp.scale_x.check_event())			return true;
	if (m_op.obp.scale_y.check_event())			return true;
	if (m_op.obp.scale_z.check_event())			return true;
	if (m_op.obp.rotate_x.check_event())		return true;
	if (m_op.obp.rotate_y.check_event())		return true;
	if (m_op.obp.rotate_z.check_event())		return true;
	if (m_op.obp.dst_clip_left.check_event())	return true;
	if (m_op.obp.dst_clip_top.check_event())	return true;
	if (m_op.obp.dst_clip_right.check_event())	return true;
	if (m_op.obp.dst_clip_bottom.check_event())	return true;
	if (m_op.obp.src_clip_left.check_event())	return true;
	if (m_op.obp.src_clip_top.check_event())	return true;
	if (m_op.obp.src_clip_right.check_event())	return true;
	if (m_op.obp.src_clip_bottom.check_event())	return true;
	if (m_op.obp.tr.check_event())				return true;
	if (m_op.obp.mono.check_event())			return true;
	if (m_op.obp.reverse.check_event())			return true;
	if (m_op.obp.bright.check_event())			return true;
	if (m_op.obp.dark.check_event())			return true;
	if (m_op.obp.color_r.check_event())			return true;
	if (m_op.obp.color_g.check_event())			return true;
	if (m_op.obp.color_b.check_event())			return true;
	if (m_op.obp.color_rate.check_event())		return true;
	if (m_op.obp.color_add_r.check_event())		return true;
	if (m_op.obp.color_add_g.check_event())		return true;
	if (m_op.obp.color_add_b.check_event())		return true;

	// �C�����W
	int x_rep_cnt = m_op.x_rep_list.get_size();
	int y_rep_cnt = m_op.y_rep_list.get_size();
	int z_rep_cnt = m_op.z_rep_list.get_size();
	int tr_rep_cnt = m_op.tr_rep_list.get_size();
	for (int i = 0; i < x_rep_cnt; i++)	{
		if (m_op.x_rep_list[i].check_event())
			return true;
	}
	for (int i = 0; i < y_rep_cnt; i++)	{
		if (m_op.y_rep_list[i].check_event())
			return true;
	}
	for (int i = 0; i < z_rep_cnt; i++)	{
		if (m_op.z_rep_list[i].check_event())
			return true;
	}
	for (int i = 0; i < tr_rep_cnt; i++)	{
		if (m_op.tr_rep_list[i].check_event())
			return true;
	}

	return false;
}

// ****************************************************************
// �I�u�W�F�N�g�F�t���[���A�N�V��������
// ================================================================
void C_elm_object::do_frame_action()
{
	// �t���[���A�N�V���������s����
	do_frame_action_func();

	// �q���̃t���[���A�N�V����
	int child_cnt = m_child_list.get_size();
	for (int i = 0; i < child_cnt; i++)
		m_child_list[i].do_frame_action();
}

// ****************************************************************
// �I�u�W�F�N�g�F�t���[���A�N�V���������s����
// ================================================================
void C_elm_object::do_frame_action_func()
{
	// �t���[���A�N�V����
	m_op.frame_action.do_action();

	// �t���[���A�N�V�����`�����l��
	for (int i = 0; i < (int)m_op.frame_action_ch.get_size(); i++)
		m_op.frame_action_ch[i].do_action();
}

// ****************************************************************
// �I�u�W�F�N�g�F�t���[���A�N�V������������
// ================================================================
void C_elm_object::init_frame_action(bool end_flag)
{
	// �t���[���A�N�V����
	m_op.frame_action.reinit(end_flag);

	// �t���[���A�N�V�����`�����l��
	m_op.frame_action_ch.reinit(end_flag, true);
}

// ****************************************************************
// �I�u�W�F�N�g�F�t���[���A�N�V������ݒ�
// ================================================================
void C_elm_object::set_frame_action(int end_time, int real_flag, CTSTR& scn_name, CTSTR& cmd_name, C_tnm_prop_list& arg_list)
{
	m_op.frame_action.set_param(end_time, real_flag, scn_name, cmd_name, arg_list);
}

// ****************************************************************
// �I�u�W�F�N�g�F�������郏�[���h���擾
// ================================================================
C_elm_world* C_elm_object::get_world()
{
	// ���[���h�ɏ������Ă���ꍇ
	if (m_op.obp.world_no >= 0)
	{
		// �O���[�v���擾����
		if (!m_op_def.elm_world_list.is_empty())
		{
			if (m_op_def.elm_world_list.code[0] == ELM_UP)
			{
				// ���΃p�X
				S_tnm_command_proc_arg_struct arg;
				arg.disp_out_of_range_error = false;
				if (tnm_command_proc_object(this, m_op_def.elm_world_list.begin(), m_op_def.elm_world_list.begin(), m_op_def.elm_world_list.end(), &arg))
				{
					C_elm_world_list* p_world_list = (C_elm_world_list *)arg.p_ret_element;
					if (p_world_list)
					{
						if (m_op.obp.world_no < p_world_list->get_size())
						{
							return &(*p_world_list)[m_op.obp.world_no];
						}
					}
				}
			}
			else
			{
				// ��΃p�X
				C_elm_world_list* p_world_list = (C_elm_world_list *)tnm_get_element_ptr(m_op_def.elm_world_list.begin(), m_op_def.elm_world_list.end(), false);
				if (p_world_list)
				{
					if (m_op.obp.world_no < p_world_list->get_size())
					{
						return &(*p_world_list)[m_op.obp.world_no];
					}
				}
			}
		}
	}

	return NULL;
}


// ****************************************************************
// �I�u�W�F�N�g�FE-mote
// ================================================================

int C_elm_object::emote_find_timeline(CTSTR& timeline)
{
	for (int i = 0; i < TNM_OBJECT_EMOTE_TIMELINE_NUM; i++)
	{
		if (m_op.emote_timeline[i] == timeline)
		{
			return i;
		}
	}

	return -1;
}

void C_elm_object::emote_play_timeline(int buf, CTSTR& timeline)
{
	emote_play_timeline(buf, timeline, 0);
}

void C_elm_object::emote_play_timeline(int buf, CTSTR& timeline, int option)
{
	if (buf < 0 || TNM_OBJECT_EMOTE_TIMELINE_NUM <= buf) { return; }

	TSTR stop_timeline = m_op.emote_timeline[buf];

	m_op.emote_timeline[buf].clear();
	m_op.emote.timeline[buf].option = 0;

#if __USE_EMOTE
	if (m_emote_player)
	{
		// �w��̃^�C�����C�������݂��Ȃ���Β�~
		if (emote_find_timeline(stop_timeline) == -1)
		{
			m_emote_player->StopTimeline(TSTR_to_UTF8(stop_timeline).c_str());
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	}
#endif

#if __USE_EMOTE
	if (m_emote_player)
	{
		// �w��̃^�C�����C�������݂��Ȃ���΍Đ�
		if (emote_find_timeline(timeline) == -1)
		{
			m_emote_player->PlayTimeline(TSTR_to_UTF8(timeline).c_str(), option);
		}
	}
#endif

	m_op.emote_timeline[buf] = timeline;
	m_op.emote.timeline[buf].option = option;
}

void C_elm_object::emote_stop_timeline(int buf)
{
	if (buf < 0 || TNM_OBJECT_EMOTE_TIMELINE_NUM <= buf) { return; }

	TSTR stop_timeline = m_op.emote_timeline[buf];

	m_op.emote_timeline[buf].clear();
	m_op.emote.timeline[buf].option = 0;

#if __USE_EMOTE
	if (m_emote_player)
	{
		// �w��̃^�C�����C�������݂��Ȃ���Β�~
		if (emote_find_timeline(stop_timeline) == -1)
		{
			m_emote_player->StopTimeline();
		}
	}
#endif
}

void C_elm_object::emote_stop_timeline_all()
{
	for (int buf = 0; buf < TNM_OBJECT_EMOTE_TIMELINE_NUM; buf++)
	{
		m_op.emote_timeline[buf].clear();
		m_op.emote.timeline[buf].option = 0;
	}

#if __USE_EMOTE
	if (m_emote_player)
	{
		m_emote_player->StopTimeline();
	}
#endif
}

bool C_elm_object::emote_check_playing()
{
#if __USE_EMOTE
	if (m_emote_player)
	{
		return m_emote_player->IsAnimating() ? true : false;
	}
#endif

	return false;
}

void C_elm_object::emote_wait(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_OBJ_EMOTE_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

void C_elm_object::emote_skip()
{
#if __USE_EMOTE
	if (m_emote_player)
	{
		m_emote_player->Skip();
	}
#endif
}

void C_elm_object::emote_pass()
{
#if __USE_EMOTE
	if (m_emote_player)
	{
		m_emote_player->Pass();
	}
#endif
}

// ****************************************************************
// �I�u�W�F�N�g���X�g�F������
// ================================================================
void C_elm_object_list::init(S_element element, CTSTR& name, int size, bool extend_enable, bool use_ini, bool use_group, bool set_cg_table, C_tnm_element* p_parent, S_element elm_group_list, S_element elm_world_list)
{
	// �Œ�p�����[�^
	m_use_ini = use_ini;
	m_use_group = use_group;
	m_set_cg_table = set_cg_table;
	m_elm_group_list = elm_group_list;
	m_elm_world_list = elm_world_list;

	// ���N���X�̏�����
	C_elm_list_ex<C_elm_object>::init(element, FM_OBJECTLIST, name, size, extend_enable, p_parent);
}

// ****************************************************************
// �I�u�W�F�N�g���X�g�F�I��
// ================================================================
void C_elm_object_list::finish()
{
	int object_cnt = (int)m_sub.size();
	for (int i = 0; i < object_cnt; i++)
		m_sub[i].finish();
}

// ****************************************************************
// �I�u�W�F�N�g���X�g�F�T�u�̏�����
// ================================================================
void C_elm_object_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)	{
		bool use_flag = true;
		bool use_group_flag = m_use_group;
		bool space_hide_flag = false;
		int object_disp_no = -1;
		if (m_use_ini)	{
			use_flag = Gp_ini->object[i].use;
			space_hide_flag = Gp_ini->object[i].space_hide;
			object_disp_no = Gp_ini->object[i].object_disp_no;
		}

		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]")
			, use_flag, use_group_flag, m_set_cg_table, space_hide_flag, object_disp_no
			, this, tnm_set_element_down(m_elm_group_list), tnm_set_element_down(m_elm_world_list));
	}
}

// ****************************************************************
// �I�u�W�F�N�g���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_object_list::_reinit(int begin, int end, bool restruct_flag)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit(true);				// ����͏I���t���O
}

// ****************************************************************
// �I�u�W�F�N�g���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_object_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)	{
		if (!m_use_ini || (Gp_ini->object[i].use && Gp_ini->object[i].save))	{
			m_sub[i].save(stream);
		}
	}
}

// ****************************************************************
// �I�u�W�F�N�g���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_object_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)	{
		if (!m_use_ini || (Gp_ini->object[i].use && Gp_ini->object[i].save))	{
			m_sub[i].load(stream);
		}
	}
}



