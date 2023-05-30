#pragma		once

#include	"data/tnm_tree.h"
#include	"data/tnm_btn.h"
#include	"data/tnm_gan.h"
#include	"data/tnm_sprite.h"

#include	"element/elm_int_event.h"
#include	"element/elm_frame_action.h"

#include	"engine/eng_graphics.h"

const int TNM_OBJECT_FLAGS_NO_EVENT = 0x0001;

const int TNM_OBJECT_EMOTE_TIMELINE_NUM = 8;

struct	S_tnm_btn_action_template_pat;
class	C_elm_object;
class	C_elm_object_list;
class	C_elm_group;
class	C_elm_group_list;
class	C_elm_world;
class	C_elm_world_list;

class	IEmotePlayer;

// ****************************************************************
// �I�u�W�F�N�g�̎��
// ================================================================
enum TNM_OBJECT_TYPE
{
	TNM_OBJECT_TYPE_NONE = 0,
	TNM_OBJECT_TYPE_RECT = 1,
	TNM_OBJECT_TYPE_PCT = 2,
	TNM_OBJECT_TYPE_STRING = 3,
	TNM_OBJECT_TYPE_WEATHER = 4,
	TNM_OBJECT_TYPE_NUMBER = 5,
	TNM_OBJECT_TYPE_MESH = 6,
	TNM_OBJECT_TYPE_BILLBOARD = 7,
	TNM_OBJECT_TYPE_SAVE_THUMB = 8,
	TNM_OBJECT_TYPE_MOVIE = 9,
	TNM_OBJECT_TYPE_CAPTURE = 10,
	TNM_OBJECT_TYPE_THUMB = 11,
	TNM_OBJECT_TYPE_EMOTE = 12,
	TNM_OBJECT_TYPE_MAX = 13,
};

// ****************************************************************
// �I�u�W�F�N�g�̎�� �� ������
// ================================================================
inline TSTR tnm_object_type_get_str(int type)
{
	switch (type)	{
		case TNM_OBJECT_TYPE_NONE:			return _T("none");
		case TNM_OBJECT_TYPE_RECT:			return _T("��`");
		case TNM_OBJECT_TYPE_PCT:			return _T("�߸��");
		case TNM_OBJECT_TYPE_STRING:		return _T("������");
		case TNM_OBJECT_TYPE_WEATHER:		return _T("��");
		case TNM_OBJECT_TYPE_NUMBER:		return _T("���l");
		case TNM_OBJECT_TYPE_MESH:			return _T("ү��");
		case TNM_OBJECT_TYPE_BILLBOARD:		return _T("����ް��");
		case TNM_OBJECT_TYPE_SAVE_THUMB:	return _T("���޻�Ȳ�");
		case TNM_OBJECT_TYPE_MOVIE:			return _T("Ѱ�ް");
		case TNM_OBJECT_TYPE_CAPTURE:		return _T("�������");
		case TNM_OBJECT_TYPE_THUMB:			return _T("��Ȳ�");
		case TNM_OBJECT_TYPE_EMOTE:			return _T("emote");
		default:							return _T("???");
	}
}

// ****************************************************************
// ���I�u�W�F�N�g�̎��
// ================================================================
const int	TNM_OBJECT_WEATHER_TYPE_NONE = 0;
const int	TNM_OBJECT_WEATHER_TYPE_A    = 1;
const int	TNM_OBJECT_WEATHER_TYPE_B    = 2;
const int	TNM_OBJECT_WEATHER_TYPE_MAX  = 3;

// ****************************************************************
// �q���̃\�[�g�^�C�v
// ================================================================
const int	TNM_OBJECT_SORT_TYPE_DEFAULT = 0;
const int	TNM_OBJECT_SORT_TYPE_NONE = 1;
const int	TNM_OBJECT_SORT_TYPE_TEXTURE = 2;
const int	TNM_OBJECT_SORT_TYPE_X = 3;
const int	TNM_OBJECT_SORT_TYPE_X_REV = 4;
const int	TNM_OBJECT_SORT_TYPE_Y = 5;
const int	TNM_OBJECT_SORT_TYPE_Y_REV = 6;
const int	TNM_OBJECT_SORT_TYPE_Z = 7;
const int	TNM_OBJECT_SORT_TYPE_Z_REV = 8;
const int	TNM_OBJECT_SORT_TYPE_MAX = 9;

// ****************************************************************
// �I�u�W�F�N�g�̌Œ�p�����[�^
// ================================================================
struct C_elm_object_def_param
{
	bool		use_flag;				// �I�u�W�F�N�g���g���t���O
	bool		use_group_flag;			// �O���[�v���g���t���O
	bool		set_cg_table;			// �b�f�e�[�u�����g���t���O
	bool		space_hide_flag;		// �X�y�[�X�L�[�ŏ�����t���O
	bool		excall_button_enable;	// �d�w�R�[�������{�^�����g����
	int			object_disp_no;			// �I�u�W�F�N�g��\������̐ݒ�ԍ�
	S_element	elm_group_list;			// �O���[�v���X�g�̃G�������g
	S_element	elm_world_list;			// ���[���h���X�g�̃G�������g
};

// ****************************************************************
// �t�B���^�I�u�W�F�N�g�̃p�����[�^
// ================================================================
struct C_elm_object_param_filter
{
	C_rect	rect;
	C_argb	color;
};

// ****************************************************************
// ������I�u�W�F�N�g�̕����p�����[�^
// ================================================================
struct C_elm_object_param_moji
{
	int		type;
	int		code;
	C_point	pos;
	int		size;
	C_argb	color;
};

// ****************************************************************
// ������I�u�W�F�N�g�̃p�����[�^
// ================================================================
struct C_elm_object_param_string
{
	int		moji_size;
	int		moji_space_x;
	int		moji_space_y;
	int		moji_cnt;
	int		moji_color;
	int		shadow_color;
	int		fuchi_color;
	int		shadow_mode;
};

// ****************************************************************
// ���l�I�u�W�F�N�g�̃p�����[�^
// ================================================================
struct C_elm_object_param_number
{
	int		number;			// ���l
	int		keta_max;		// ����
	int		disp_zero;		// �擪�̂O��\��
	int		disp_sign;		// �擪�̕�����\��
	int		tumeru_sign;	// �擪�̕������l�߂�
	int		space_mod;		// 0:�Ԋu=space�A1:�Ԋu=�J�b�g�T�C�Y+space
	int		space;			// �Ԋu
};

// ****************************************************************
// ���I�u�W�F�N�g�̃p�����[�^
// ================================================================
struct C_elm_object_param_weather
{
	int		type;
	int		cnt;
	int		pat_mode;
	int		pat_no_00;
	int		pat_no_01;
	int		pat_time;
	int		move_time_x;
	int		move_time_y;
	int		sin_time_x;
	int		sin_time_y;
	int		sin_power_x;
	int		sin_power_y;
	int		center_x;
	int		center_y;
	int		center_rotate;
	int		appear_range;
	int		zoom_min;
	int		zoom_max;
	int		scale_x;
	int		scale_y;
	int		active_time;
	bool	real_time_flag;		// �����ԃt���O
};

// ****************************************************************
// ���I�u�W�F�N�g�̈ꎞ�p�����[�^
// ================================================================
struct C_elm_object_work_param_weather_sub
{
	int		state;				// 0:sleep, 1:appear, 2:active, 3:disappear
	int		state_cur_time;
	int		state_time_len;
	int		move_start_pos_x;
	int		move_start_pos_y;
	int		move_start_distance;
	int		move_start_degree;
	int		move_time_x;		// 1000px �i�ނ̂ɂ����鎞��
	int		move_time_y;		// 1000px �i�ނ̂ɂ����鎞��
	int		move_cur_time;
	int		sin_time_x;
	int		sin_time_y;
	int		sin_power_x;
	int		sin_power_y;
	int		sin_cur_time;
	int		center_rotate;
	int		zoom_min;
	int		zoom_max;
	int		scale_x;
	int		scale_y;
	int		active_time_len;
	bool	real_time_flag;		// �����ԃt���O
	bool	restruct_flag;		// �č\�z�t���O
};

struct C_elm_object_work_param_weather
{
	int		cnt_max;			// ���܂ł̍ő��
	ARRAY<C_elm_object_work_param_weather_sub>	sub;
};

// ****************************************************************
// �T���l�C���I�u�W�F�N�g�̃p�����[�^
// ================================================================
struct C_elm_object_param_thumb
{
	int		save_no;
};

// ****************************************************************
// ���[�r�[�I�u�W�F�N�g�̃p�����[�^
// ================================================================
struct C_elm_object_param_movie
{
	bool	loop_flag;			// ���[�v�t���O
	bool	auto_free_flag;		// ��������t���O
	bool	real_time_flag;		// �����ԃt���O
	bool	pause_flag;			// �ꎞ��~�t���O
};

// ****************************************************************
// E-mote �I�u�W�F�N�g�̃p�����[�^
// ================================================================
struct C_elm_object_param_emote_timeline
{
	int		option;
};
struct C_elm_object_param_emote
{
	C_size		size;
	C_elm_object_param_emote_timeline	timeline[TNM_OBJECT_EMOTE_TIMELINE_NUM];
	int			koe_chara_no;
	int			koe_mouth_volume;
	int			rep_x;
	int			rep_y;
};

// ****************************************************************
// �I�u�W�F�N�g�̃{�^���p�����[�^
// ================================================================
struct C_elm_object_param_button
{
	int			sys_type;		// �V�X�e���^�C�v
	int			sys_type_opt;	// �V�X�e���^�C�v�I�v�V����
	int			action_no;		// �A�N�V�����ԍ�
	int			se_no;			// ���ʉ��ԍ�
	int			button_no;		// �{�^���ԍ�
	S_element	elm_group;		// �O���[�v�ԍ�
	int			push_keep;		// ��������Ԃ��ێ�����
	int			state;			// �ʏ�^�����Ă���^�g���Ȃ�
	int			mode;			// ���[�h
	int			cut_no;			// �J�b�g�ԍ�
	int			decided_action_scn_no;	// ����A�N�V�����̃V�[���ԍ�
	int			decided_action_cmd_no;	// ����A�N�V�����̃R�}���h�ԍ�
	int			decided_action_z_no;	// ����A�N�V�����̂y���x���ԍ�
	int			excall_enable;	// �d�w�R�[�������g����
	int			alpha_test;		// �����ȕ����͓�����Ȃ�
};

// ****************************************************************
// �I�u�W�F�N�g�̊�{�p�����[�^�i�o�n�c�j
// ================================================================
struct C_elm_object_param_base
{
	// ��{
	int				disp;				// �\���t���O
	C_elm_int_event	pat_no;				// �p�^�[���ԍ�
	S_tnm_sorter	sorter;				// �\�[�^�[
	int				world_no;			// ���[���h�ԍ�
	int				child_sort_type;	// �q���̃\�[�g�^�C�v

	// ���_�֘A
	C_elm_int_event	pos_x;			// �ʒu
	C_elm_int_event	pos_y;			// �ʒu
	C_elm_int_event	pos_z;			// �ʒu
	C_elm_int_event	center_x;		// ���S
	C_elm_int_event	center_y;		// ���S
	C_elm_int_event	center_z;		// ���S
	C_elm_int_event	center_rep_x;	// ���S�␳
	C_elm_int_event	center_rep_y;	// ���S�␳
	C_elm_int_event	center_rep_z;	// ���S�␳
	C_elm_int_event	scale_x;		// �g�k�i���{ = 1000�j
	C_elm_int_event	scale_y;		// �g�k�i���{ = 1000�j
	C_elm_int_event	scale_z;		// �g�k�i���{ = 1000�j
	C_elm_int_event	rotate_x;		// ��]�i�P�� = 3600�j
	C_elm_int_event	rotate_y;		// ��]�i�P�� = 3600�j
	C_elm_int_event	rotate_z;		// ��]�i�P�� = 3600�j
	int				dst_clip_use;	// �]����\���͈͎g�p
	C_elm_int_event	dst_clip_left;	// �]����\���͈�
	C_elm_int_event	dst_clip_top;	// �]����\���͈�
	C_elm_int_event	dst_clip_right;	// �]����\���͈�
	C_elm_int_event	dst_clip_bottom;// �]����\���͈�
	int				src_clip_use;	// �]�����\��
	C_elm_int_event	src_clip_left;	// �]�����\���͈�
	C_elm_int_event	src_clip_top;	// �]�����\���͈�
	C_elm_int_event	src_clip_right;	// �]�����\���͈�
	C_elm_int_event	src_clip_bottom;// �]�����\���͈�

	// �s�N�Z���֘A
	C_elm_int_event	tr;				// �s�����x
	C_elm_int_event	mono;			// ���m�N��
	C_elm_int_event	reverse;		// ���]�x
	C_elm_int_event	bright;			// ���x
	C_elm_int_event	dark;			// �Óx
	C_elm_int_event	color_r;		// �F�ϊ��i�q�j
	C_elm_int_event	color_g;		// �F�ϊ��i�f�j
	C_elm_int_event	color_b;		// �F�ϊ��i�a�j
	C_elm_int_event	color_rate;		// �F�ϊ��i���x�j
	C_elm_int_event	color_add_r;	// �F���Z�i�q�j
	C_elm_int_event	color_add_g;	// �F���Z�i�f�j
	C_elm_int_event	color_add_b;	// �F���Z�i�a�j
	int				mask_no;		// �}�X�N
	int				tone_curve_no;	// �g�[���J�[�u
	int				light_no;		// ���C�g
	int				use_fog;		// �t�H�O

	// �����֘A
	int				culling;		// �J�����O
	int				alpha_test;		// �A���t�@�e�X�g
	int				alpha_blend;	// �A���t�@�u�����h
	int				blend_type;		// �������@

	// ���̑��̃t���O
	int				flags;
};

// ****************************************************************
// �I�u�W�F�N�g�̋��ʃp�����[�^
// ================================================================
struct C_elm_object_param
{
	// �^�C�v�ʃp�����[�^�i�o�n�c�j
	TNM_OBJECT_TYPE					type;			// �^�C�v
	int								wipe_copy;		// ���C�v�R�s�[
	int								wipe_erase;		// ���C�v����
	int								click_disable;	// �N���b�N�֎~
	C_elm_object_param_filter		filter;			// �t�B���^�p�����[�^
	C_elm_object_param_string		string;			// ������p�����[�^
	C_elm_object_param_number		number;			// ���l�p�����[�^
	C_elm_object_param_weather		weather;		// ���p�����[�^
	C_elm_object_param_thumb		thumb;			// �T���l�C���p�����[�^
	C_elm_object_param_movie		movie;			// ���[�r�[�p�����[�^
	C_elm_object_param_emote		emote;			// E-mote �p�����[�^
	C_elm_object_param_button		button;			// �{�^���p�����[�^
	C_elm_object_param_base			obp;			// ��{�p�����[�^

	// �o�n�c�łȂ��p�����[�^
	C_elm_int_event_list		x_rep_list;				// �C�����W�w
	C_elm_int_event_list		y_rep_list;				// �C�����W�x
	C_elm_int_event_list		z_rep_list;				// �C�����W�y
	C_elm_int_event_list		tr_rep_list;			// �C���s�����x�w
	C_elm_int_list				f;						// �ėp�t���O
	TSTR						file_path;				// �摜�p�X
	TSTR						str;					// ������i������I�u�W�F�N�g�j
	TSTR						emote_timeline[TNM_OBJECT_EMOTE_TIMELINE_NUM];		// E-mote �^�C�����C��
	TSTR						button_decided_action_scn_name;	// �{�^������A�N�V�����̃V�[����
	TSTR						button_decided_action_cmd_name;	// �{�^������A�N�V�����̃R�}���h��
	C_elm_frame_action			frame_action;			// �t���[���A�N�V����
	C_elm_frame_action_list		frame_action_ch;		// �t���[���A�N�V�����`�����l��
	C_tnm_gan					gan;					// �f�`�m
};

// ****************************************************************
// �I�u�W�F�N�g���X�g
// ================================================================
class C_elm_object_list : public C_elm_list_ex<C_elm_object>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable, bool use_ini, bool use_group, bool set_cg_table, C_tnm_element* p_parent, S_element elm_group_list, S_element elm_world_list);	// ������
	void	finish();

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end, bool restruct_flag);		// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h

private:
	bool		m_use_ini;
	bool		m_use_group;
	bool		m_set_cg_table;
	S_element	m_elm_group_list;
	S_element	m_elm_world_list;
};

// ****************************************************************
// �I�u�W�F�N�g
// ================================================================
class C_elm_object : public C_tnm_element
{
public:

	C_elm_object()
#if __USE_EMOTE
		: m_emote_player(NULL)
#endif
	{}

	// �G�������g�̊�{����
	void	init(S_element element, CTSTR& name, bool use_flag, bool use_group_flag, bool set_cg_table, bool space_hide_flag, int object_disp_no, C_tnm_element* p_parent, S_element elm_group_list, S_element elm_world_list);	// ������
	void	reinit(bool finish_free_flag);					// �ď�����
	void	free();											// ����i�������łȂ��_�ɒ��Ӂj
	void	finish();										// �������I������
	void	save(C_tnm_save_stream& stream);				// �Z�[�u
	void	load(C_tnm_save_stream& stream);				// ���[�h
	void	copy(const C_elm_object* src, bool free_flag);	// �R�s�[

	bool	is_use()				{	return m_op_def.use_flag;		}
	int		get_object_disp_no()	{	return m_op_def.object_disp_no;	}

	// �������Ɖ������
	void	init_type(bool free_flag);					// �^�C�v�̏�����
	void	init_param(bool end_flag);					// �p�����[�^�̏�����
	void	init_button_param();						// �{�^���p�����[�^�̏�����
	void	free_type(bool child_flag);					// �^�C�v�̉��

	// �\�z����
	bool	create_rect(C_rect rect, C_argb color);		// ��`�I�u�W�F�N�g�̍쐬
	bool	create_pct(TSTR file_path);					// �s�N�`���I�u�W�F�N�g�̍쐬
	bool	create_string(TSTR str);					// ������I�u�W�F�N�g�̍쐬
	bool	create_number(TSTR file_path);				// ���l�I�u�W�F�N�g�̍쐬
	bool	create_weather(TSTR file_path);				// ���I�u�W�F�N�g�̍쐬
	bool	create_save_thumb(int save_no);				// �Z�[�u�T���l�C���I�u�W�F�N�g�̍쐬
	bool	create_thumb(int thumb_no);					// �T���l�C���I�u�W�F�N�g�̍쐬
	bool	create_movie(TSTR file_path, bool loop, bool auto_free, bool real_time, bool ready_only);	// ���[�r�[�I�u�W�F�N�g�̍쐬
	bool	create_mesh(TSTR file_path);				// ���b�V���I�u�W�F�N�g�̍쐬
	bool	create_billboard(TSTR file_path);			// �r���{�[�h�I�u�W�F�N�g�̍쐬
	bool	create_capture();							// �L���v�`���[�̍쐬
	bool	create_emote(int width, int height, TSTR file_path);	// E-mote �I�u�W�F�N�g�̍쐬

	// �t���[������
	void	frame_init();																// �t���[������������
	void	update_time(int past_game_time, int past_real_time, int parent_flags);		// ���Ԃ��X�V
	bool	frame(const S_tnm_render_param* parent_trp, int order, int parent_flags);	// �t���[������
	void	do_frame_action();															// �t���[���A�N�V��������

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// �X�v���C�g�c���[�̎擾

	// �����蔻�菈��
	void	regist_button(int min_order);		// �{�^����o�^
	void	button_event();						// �{�^���C�x���g����
	bool	is_hit();							// �������Ă���H
	bool	is_pushed();						// ������Ă���H
	bool	is_decided();						// ���肳�ꂽ�H
	bool	is_hit_this_frame();				// �������Ă���H
	bool	is_pushed_this_frame();				// ������Ă���H
	bool	is_decided_this_frame();			// ���肳�ꂽ�H

	// �\�[�^�[�̎擾�^�ݒ�
	void			set_sorter(S_tnm_sorter value)	{	m_op.obp.sorter = value;	}
	S_tnm_sorter	get_sorter()					{	return m_op.obp.sorter;		}

	// ��{�p�����[�^�̐ݒ�
	void	set_wipe_copy(int value)		{ m_op.wipe_copy = value ? 1 : 0; }
	void	set_wipe_erase(int value)		{ m_op.wipe_erase = value ? 1 : 0; }
	void	set_click_disable(int value)	{ m_op.click_disable = value ? 1 : 0; }
	void	set_disp(int value)				{ m_op.obp.disp = value ? 1 : 0; }
	void	set_pat_no(int value);
	void	set_order(int value)			{ m_op.obp.sorter.order = value; }
	void	set_layer(int value)			{ m_op.obp.sorter.layer = value; }
	void	set_world_no(int value)			{ m_op.obp.world_no = value; }
	void	set_pos_x(int value)			{ m_op.obp.pos_x.set_value(value); }
	void	set_pos_y(int value)			{ m_op.obp.pos_y.set_value(value); }
	void	set_pos_z(int value)			{ m_op.obp.pos_z.set_value(value); }
	void	set_center_x(int value)			{ m_op.obp.center_x.set_value(value); }
	void	set_center_y(int value)			{ m_op.obp.center_y.set_value(value); }
	void	set_center_z(int value)			{ m_op.obp.center_z.set_value(value); }
	void	set_center_rep_x(int value)		{ m_op.obp.center_rep_x.set_value(value); }
	void	set_center_rep_y(int value)		{ m_op.obp.center_rep_y.set_value(value); }
	void	set_center_rep_z(int value)		{ m_op.obp.center_rep_z.set_value(value); }
	void	set_scale_x(int value)			{ m_op.obp.scale_x.set_value(value); }
	void	set_scale_y(int value)			{ m_op.obp.scale_y.set_value(value); }
	void	set_scale_z(int value)			{ m_op.obp.scale_z.set_value(value); }
	void	set_rotate_x(int value)			{ m_op.obp.rotate_x.set_value(value); }
	void	set_rotate_y(int value)			{ m_op.obp.rotate_y.set_value(value); }
	void	set_rotate_z(int value)			{ m_op.obp.rotate_z.set_value(value); }
	void	set_clip_use(int value)			{ m_op.obp.dst_clip_use = value ? 1 : 0; }
	void	set_clip_left(int value)		{ m_op.obp.dst_clip_left.set_value(value); }
	void	set_clip_top(int value)			{ m_op.obp.dst_clip_top.set_value(value); }
	void	set_clip_right(int value)		{ m_op.obp.dst_clip_right.set_value(value); }
	void	set_clip_bottom(int value)		{ m_op.obp.dst_clip_bottom.set_value(value); }
	void	set_src_clip_use(int value)		{ m_op.obp.src_clip_use = value ? 1 : 0; }
	void	set_src_clip_left(int value)	{ m_op.obp.src_clip_left.set_value(value); }
	void	set_src_clip_top(int value)		{ m_op.obp.src_clip_top.set_value(value); }
	void	set_src_clip_right(int value)	{ m_op.obp.src_clip_right.set_value(value); }
	void	set_src_clip_bottom(int value)	{ m_op.obp.src_clip_bottom.set_value(value); }
	void	set_tr(int value)				{ m_op.obp.tr.set_value(limit(0, value, 255)); }
	void	set_mono(int value)				{ m_op.obp.mono.set_value(limit(0, value, 255)); }
	void	set_reverse(int value)			{ m_op.obp.reverse.set_value(limit(0, value, 255)); }
	void	set_bright(int value)			{ m_op.obp.bright.set_value(limit(0, value, 255)); }
	void	set_dark(int value)				{ m_op.obp.dark.set_value(limit(0, value, 255)); }
	void	set_color_r(int value)			{ m_op.obp.color_r.set_value(limit(0, value, 255)); } 
	void	set_color_g(int value)			{ m_op.obp.color_g.set_value(limit(0, value, 255)); }
	void	set_color_b(int value)			{ m_op.obp.color_b.set_value(limit(0, value, 255)); }
	void	set_color_rate(int value)		{ m_op.obp.color_rate.set_value(limit(0, value, 255)); }
	void	set_color_add_r(int value)		{ m_op.obp.color_add_r.set_value(limit(0, value, 255)); }
	void	set_color_add_g(int value)		{ m_op.obp.color_add_g.set_value(limit(0, value, 255)); }
	void	set_color_add_b(int value)		{ m_op.obp.color_add_b.set_value(limit(0, value, 255)); }
	void	set_mask_no(int value)			{ m_op.obp.mask_no = value; }
	void	set_tone_curve_no(int value)	{ m_op.obp.tone_curve_no = value; }
	void	set_culling(int value)			{ m_op.obp.culling = value ? 1 : 0; }
	void	set_alpha_test(int value)		{ m_op.obp.alpha_test = value ? 1 : 0; }
	void	set_alpha_blend(int value)		{ m_op.obp.alpha_blend = value ? 1 : 0; }
	void	set_blend_type(int value)		{ m_op.obp.blend_type = limit(0, value, TNM_BLEND_TYPE_MAX - 1); }
	void	set_light_no(int value)			{ m_op.obp.light_no = value; }
	void	set_fog_use(int value)			{ m_op.obp.use_fog = value ? 1 : 0; }
	void	set_child_sort_type(int value)	{ m_op.obp.child_sort_type = limit(0, value, TNM_OBJECT_SORT_TYPE_MAX - 1); }

	// ��{�p�����[�^�̎擾
	int		get_type()					{	return m_op.type;								}
	int		get_wipe_copy()				{	return m_op.wipe_copy;							}
	int		get_wipe_erase()			{	return m_op.wipe_erase;							}
	int		get_click_disable()			{	return m_op.click_disable;						}
	TSTR	get_file_path()				{	return m_op.file_path;							}
	int		get_disp()					{	return m_op.obp.disp;							}
	int		get_pat_no()				{	return m_op.obp.pat_no.get_value();				}
	int		get_order()					{	return m_op.obp.sorter.order;					}
	int		get_layer()					{	return m_op.obp.sorter.layer;					}
	int		get_world_no()				{	return m_op.obp.world_no;						}
	int		get_pos_x()					{	return m_op.obp.pos_x.get_value();				}
	int		get_pos_y()					{	return m_op.obp.pos_y.get_value();				}
	int		get_pos_z()					{	return m_op.obp.pos_z.get_value();				}
	int		get_center_x()				{	return m_op.obp.center_x.get_value();			}
	int		get_center_y()				{	return m_op.obp.center_y.get_value();			}
	int		get_center_z()				{	return m_op.obp.center_z.get_value();			}
	int		get_center_rep_x()			{	return m_op.obp.center_rep_x.get_value();		}
	int		get_center_rep_y()			{	return m_op.obp.center_rep_y.get_value();		}
	int		get_center_rep_z()			{	return m_op.obp.center_rep_z.get_value();		}
	int		get_scale_x()				{	return m_op.obp.scale_x.get_value();			}
	int		get_scale_y()				{	return m_op.obp.scale_y.get_value();			}
	int		get_scale_z()				{	return m_op.obp.scale_z.get_value();			}
	int		get_rotate_x()				{	return m_op.obp.rotate_x.get_value();			}
	int		get_rotate_y()				{	return m_op.obp.rotate_y.get_value();			}
	int		get_rotate_z()				{	return m_op.obp.rotate_z.get_value();			}
	int		get_clip_use()				{	return m_op.obp.dst_clip_use;					}
	int		get_clip_left()				{	return m_op.obp.dst_clip_left.get_value();		}
	int		get_clip_top()				{	return m_op.obp.dst_clip_top.get_value();		}
	int		get_clip_right()			{	return m_op.obp.dst_clip_right.get_value();		}
	int		get_clip_bottom()			{	return m_op.obp.dst_clip_bottom.get_value();	}
	int		get_src_clip_use()			{	return m_op.obp.src_clip_use;					}
	int		get_src_clip_left()			{	return m_op.obp.src_clip_left.get_value();		}
	int		get_src_clip_top()			{	return m_op.obp.src_clip_top.get_value();		}
	int		get_src_clip_right()		{	return m_op.obp.src_clip_right.get_value();		}
	int		get_src_clip_bottom()		{	return m_op.obp.src_clip_bottom.get_value();	}
	int		get_tr()					{	return m_op.obp.tr.get_value();					}
	int		get_mono()					{	return m_op.obp.mono.get_value();				}
	int		get_reverse()				{	return m_op.obp.reverse.get_value();			}
	int		get_bright()				{	return m_op.obp.bright.get_value();				}
	int		get_dark()					{	return m_op.obp.dark.get_value();				}
	int		get_color_r()				{	return m_op.obp.color_r.get_value();			}
	int		get_color_g()				{	return m_op.obp.color_g.get_value();			}
	int		get_color_b()				{	return m_op.obp.color_b.get_value();			}
	int		get_color_rate()			{	return m_op.obp.color_rate.get_value();			}
	int		get_color_add_r()			{	return m_op.obp.color_add_r.get_value();		}
	int		get_color_add_g()			{	return m_op.obp.color_add_g.get_value();		}
	int		get_color_add_b()			{	return m_op.obp.color_add_b.get_value();		}
	int		get_mask_no()				{	return m_op.obp.mask_no;						}
	int		get_tone_curve_no()			{	return m_op.obp.tone_curve_no;					}
	int		get_culling()				{	return m_op.obp.culling;						}
	int		get_alpha_test()			{	return m_op.obp.alpha_test;						}
	int		get_alpha_blend()			{	return m_op.obp.alpha_blend;					}
	int		get_blend_type()			{	return m_op.obp.blend_type;						}
	int		get_light_no()				{	return m_op.obp.light_no;						}
	int		get_fog_use()				{	return m_op.obp.use_fog;						}
	int		get_child_sort_type()		{	return m_op.obp.child_sort_type;				}

	// �摜��������擾
	int		get_pat_cnt();
	int		get_size_x(int cut_no);
	int		get_size_y(int cut_no);
	int		get_size_z(int cut_no);
	bool	get_pixel_color(int x, int y, int cut_no, C_argb* p_color);
	int		get_pixel_color_r(int x, int y, int cut_no);
	int		get_pixel_color_g(int x, int y, int cut_no);
	int		get_pixel_color_b(int x, int y, int cut_no);
	int		get_pixel_color_a(int x, int y, int cut_no);
	bool	check_click_disable(C_point mp, bool child_flag);

	// �q���g
	void	clear_hints()					{	m_op.obp.flags = 0;			}
	void	set_hints(int value)			{	m_op.obp.flags = value;		}
	void	add_hints(int value)			{	m_op.obp.flags |= value;	}
	void	sub_hints(int value)			{	m_op.obp.flags &= ~value;	}
	int		get_hints()						{	return m_op.obp.flags;		}

	// �C�x���g�p�����[�^
	C_elm_int_event&		get_pat_no_event()			{	return m_op.obp.pat_no;				}
	C_elm_int_event&		get_x_event()				{	return m_op.obp.pos_x;				}
	C_elm_int_event&		get_y_event()				{	return m_op.obp.pos_y;				}
	C_elm_int_event&		get_z_event()				{	return m_op.obp.pos_z;				}
	C_elm_int_event&		get_center_x_event()		{	return m_op.obp.center_x;			}
	C_elm_int_event&		get_center_y_event()		{	return m_op.obp.center_y;			}
	C_elm_int_event&		get_center_z_event()		{	return m_op.obp.center_z;			}
	C_elm_int_event&		get_center_rep_x_event()	{	return m_op.obp.center_rep_x;		}
	C_elm_int_event&		get_center_rep_y_event()	{	return m_op.obp.center_rep_y;		}
	C_elm_int_event&		get_center_rep_z_event()	{	return m_op.obp.center_rep_z;		}
	C_elm_int_event&		get_scale_x_event()			{	return m_op.obp.scale_x;			}
	C_elm_int_event&		get_scale_y_event()			{	return m_op.obp.scale_y;			}
	C_elm_int_event&		get_scale_z_event()			{	return m_op.obp.scale_z;			}
	C_elm_int_event&		get_rotate_x_event()		{	return m_op.obp.rotate_x;			}
	C_elm_int_event&		get_rotate_y_event()		{	return m_op.obp.rotate_y;			}
	C_elm_int_event&		get_rotate_z_event()		{	return m_op.obp.rotate_z;			}
	C_elm_int_event&		get_clip_left_event()		{	return m_op.obp.dst_clip_left;		}
	C_elm_int_event&		get_clip_top_event()		{	return m_op.obp.dst_clip_top;		}
	C_elm_int_event&		get_clip_right_event()		{	return m_op.obp.dst_clip_right;		}
	C_elm_int_event&		get_clip_bottom_event()		{	return m_op.obp.dst_clip_bottom;	}
	C_elm_int_event&		get_src_clip_left_event()	{	return m_op.obp.src_clip_left;		}
	C_elm_int_event&		get_src_clip_top_event()	{	return m_op.obp.src_clip_top;		}
	C_elm_int_event&		get_src_clip_right_event()	{	return m_op.obp.src_clip_right;		}
	C_elm_int_event&		get_src_clip_bottom_event()	{	return m_op.obp.src_clip_bottom;	}
	C_elm_int_event&		get_tr_event()				{	return m_op.obp.tr;					}
	C_elm_int_event&		get_mono_event()			{	return m_op.obp.mono;				}
	C_elm_int_event&		get_reverse_event()			{	return m_op.obp.reverse;			}
	C_elm_int_event&		get_bright_event()			{	return m_op.obp.bright;				}
	C_elm_int_event&		get_dark_event()			{	return m_op.obp.dark;				}
	C_elm_int_event&		get_color_r_event()			{	return m_op.obp.color_r;			}
	C_elm_int_event&		get_color_g_event()			{	return m_op.obp.color_g;			}
	C_elm_int_event&		get_color_b_event()			{	return m_op.obp.color_b;			}
	C_elm_int_event&		get_color_rate_event()		{	return m_op.obp.color_rate;			}
	C_elm_int_event&		get_color_add_r_event()		{	return m_op.obp.color_add_r;		}
	C_elm_int_event&		get_color_add_g_event()		{	return m_op.obp.color_add_g;		}
	C_elm_int_event&		get_color_add_b_event()		{	return m_op.obp.color_add_b;		}

	// �␳�C�x���g�p�����[�^
	C_elm_int_event_list&	x_rep_event_list()		{	return m_op.x_rep_list;		}
	C_elm_int_event_list&	y_rep_event_list()		{	return m_op.y_rep_list;		}
	C_elm_int_event_list&	z_rep_event_list()		{	return m_op.z_rep_list;		}
	C_elm_int_event_list&	tr_rep_event_list()		{	return m_op.tr_rep_list;	}

	// �t���O
	C_elm_int_list&			F()						{	return m_op.f;				}

	// �S�̃C�x���g
	void					end_all_event();	// �S�ẴC�x���g���~
	bool					check_all_event();	// �S�ẴC�x���g�𔻒�

	// ��`�p�����[�^
	C_rect	get_filter_rect()			{	return m_op.filter.rect;	}
	C_argb	get_filter_color()			{	return m_op.filter.color;	}

	// �t�@�C��
	bool	change_file(CTSTR& file_path);	// �t�@�C����ύX

	// ������p�����[�^
	bool	set_string(TSTR str);
	bool	set_string_param(int moji_size, int moji_space_x, int moji_space_y, int moji_cnt, int moji_color, int shadow_color, int fuchi_color, int shadow_mode);
	TSTR	get_string()				{	return m_op.str;					}

	// ���l�p�����[�^
	void	set_number(int value)		{	m_op.number.number = value;			}
	void	set_number_param(int keta, int disp_zero, int disp_sign, int tumeru_sign, int space_mod, int space);
	int		get_number()				{	return m_op.number.number;			}

	// ���p�����[�^
	bool	set_weather_param_type_a(int cnt, int pat_mode, int pat_no_00, int pat_no_01, int pat_time, int move_time_x, int move_time_y, int sin_time_x, int sin_power_x, int sin_time_y, int sin_power_y, int scale_x, int scale_y, int active_time, bool real_time);
	bool	set_weather_param_type_b(int cnt, int pat_mode, int pat_no_00, int pat_no_01, int pat_time, int center_x, int center_y, int appear_range, int move_time, int sin_time_x, int sin_power_x, int sin_time_y, int sin_power_y, int center_rotate, int zoom_min, int zoom_max, int scale_x, int scale_y, bool real_time);

	// ���[�r�[�p�����[�^
	int		get_omv_timer()				{	return m_omv_timer;			}
	void	pause_movie()				{	m_op.movie.pause_flag = 1;	}
	void	resume_movie()				{	m_op.movie.pause_flag = 0;	}
	void	seek_movie(int time);
	int		get_movie_seek_time();
	bool	check_movie();
	void	wait_movie(bool key_skip_flag, bool return_value_flag);
	void	end_movie_loop();
	void	set_movie_auto_free(bool value);

	// E-mote �p�����[�^
	void	emote_play_timeline(int buf, CTSTR& timeline);
	void	emote_play_timeline(int buf, CTSTR& timeline, int option);
	void	emote_stop_timeline(int buf);
	void	emote_stop_timeline_all();
	bool	emote_check_playing();
	void	emote_wait(bool key_skip_flag, bool return_value_flag);
	void	emote_skip();
	void	emote_pass();
	int		emote_find_timeline(CTSTR& timeline);
	void	emote_set_koe_chara_no(int no)	{ m_op.emote.koe_chara_no = no; }
	int		emote_get_koe_chara_no()		{ return m_op.emote.koe_chara_no; }
	void	emote_set_koe_mouth_volume(int v)	{ m_op.emote.koe_mouth_volume = v; }
	int		emote_get_koe_mouth_volume()		{ return m_op.emote.koe_mouth_volume; }
	void	emote_set_rep_pos(int rep_x, int rep_y)		{ m_op.emote.rep_x = rep_x; m_op.emote.rep_y = rep_y; }

	// �{�^���p�����[�^
	bool	clear_button_decide_action();
	void	set_button_param(int button_no, int group_no, int action_no, int se_no);
	void	set_button_no(int button_no)				{	m_op.button.button_no = button_no;	}
	void	set_button_group(const S_element& element);
	void	set_button_group_no(int group_no);
	void	set_button_action_no(int action_no)			{	m_op.button.action_no = action_no;	}
	void	set_button_se_no(int se_no)					{	m_op.button.se_no = se_no;			}
	void	set_button_state(int state)					{	m_op.button.state = state;			}
	int		get_button_state()							{	return m_op.button.state;			}
	int		get_button_hit_state();
	int		get_button_real_state();
	void	set_button_push_keep(int push_keep)			{	m_op.button.push_keep = push_keep;										}
	int		get_button_push_keep()						{	return m_op.button.push_keep;											}
	void	set_button_alpha_test(int alpha_test)		{ m_op.button.alpha_test = alpha_test; }
	int		get_button_alpha_test()						{ return m_op.button.alpha_test; }
	void	set_button_cut_no(int cut_no)				{ m_op.button.cut_no = cut_no; }
	void	set_button_sys_type(int sys_type, int opt)	{	m_op.button.sys_type = sys_type;	m_op.button.sys_type_opt = opt;		}
	void	set_button_mode(int mode)					{	m_op.button.mode = mode;												}
	int		get_button_no()								{	return m_op.button.button_no;			}
	int		get_button_group_no();
	int		get_button_action_no()						{	return m_op.button.action_no;			}
	int		get_button_se_no()							{	return m_op.button.se_no;				}
	bool	set_button_decided_action(CTSTR& scn_name, CTSTR& cmd_name);
	bool	set_button_decided_action(CTSTR& scn_name, int z_no);
	TSTR	get_button_decided_action_scn_name()		{	return m_op.button_decided_action_scn_name;	}
	TSTR	get_button_decided_action_cmd_name()		{	return m_op.button_decided_action_cmd_name;	}
	int		get_button_decided_action_z_no()			{	return m_op.button.decided_action_z_no;		}

	// �t���[���A�N�V����
	C_elm_frame_action*			get_frame_action()		{	return &m_op.frame_action;				}
	C_elm_frame_action_list*	get_frame_action_ch()	{	return &m_op.frame_action_ch;			}
	void						set_frame_action(int end_time, int real_flag, CTSTR& scn_name, CTSTR& cmd_name, C_tnm_prop_list& arg_list);

	// �f�`�m
	C_tnm_gan&			gan()							{	return m_op.gan;						}

	// �q��
	bool				alloc_child(int cnt);
	int					get_child_cnt()					{	return (int)m_child_list.get_size();	}
	C_elm_object_list&	child_list()					{	return m_child_list;					}

	// �O���[�v�ƃA�N�V����
	C_elm_group*						get_group();
	S_tnm_btn_action_template_pat*		get_action(int btn_state);

	// ���[���h
	C_elm_world*						get_world();

	// ���̑�
	BSP<C_d3d_album>	get_album() const				{	return m_album;							}
	BSP<C_tnm_sprite>	get_sprite() const				{	return m_sprite;						}
	S_tnm_sorter		get_total_sorter() const		{	return m_trp.sorter;					}

private:

	// �p�����[�^
	C_elm_object_def_param			m_op_def;
	C_elm_object_param				m_op;
	S_tnm_render_param				m_trp;

	// �q�G�������g
	C_elm_object_list				m_child_list;		// �q��

	// �ꎞ�p�����[�^
	ARRAY<C_elm_object_param_moji>	m_moji_list;		// ����
	C_elm_object_work_param_weather	m_weather;			// ��
	BSP<C_d3d_album>				m_album;			// �A���o��
	BSP<C_d3d_mesh>					m_mesh;				// ���b�V��
	BSP<C_d3d_frame_set>			m_frame_set;		// �t���[���Z�b�g
	BSP<C_tnm_sprite>				m_sprite;			// �X�v���C�g
	ARRAY<C_tnm_sprite>				m_sprite_list;		// �X�v���C�g���X�g
	C_point							m_total_pos;		// �ŏI�ʒu
	C_tnm_btn						m_button;			// �{�^��
	BSP<C_omv_player>				m_omv_player;		// omv �v���C���[
	int								m_omv_timer;		// omv �^�C�}�[
	BSP<C_d3d_texture>				m_emote_texture;	// E-mote
	BSP<C_d3d_surface>				m_emote_dpst;		// E-mote

	// ���l�I�u�W�F�N�g�̃X�v���C�g��
	static const int	m_sprite_cnt_number = 16;

	// E-mote
#if __USE_EMOTE
	IEmotePlayer*					m_emote_player;		// E-mote �v���C���[
#endif

	// �������Ɖ������
	void	init_filter_param();					// �t�B���^�p�����[�^�̏�����
	void	init_string_param();					// ������p�����[�^�̏�����
	void	init_number_param();					// ���l�p�����[�^�̏�����
	void	init_weather_param();					// ���p�����[�^�̏�����
	void	init_frame_action(bool end_flag);		// �t���[���A�N�V�����̏�����

	// �č\�z����
	bool	restruct_type();				// �^�C�v�̍č\�z
	bool	restruct_rect();				// ��`�I�u�W�F�N�g�̍č\�z
	bool	restruct_pct();					// �s�N�`���I�u�W�F�N�g�̍č\�z
	bool	restruct_string();				// ������I�u�W�F�N�g�̍č\�z
	bool	restruct_number();				// ���l�I�u�W�F�N�g�̍č\�z
	bool	restruct_weather();				// ���I�u�W�F�N�g�̍č\�z
	bool	restruct_mesh();				// ���b�V���I�u�W�F�N�g�̍č\�z
	bool	restruct_billboard();			// �r���{�[�h�I�u�W�F�N�g�̍č\�z
	bool	restruct_save_thumb();			// �Z�[�u�T���l�C���I�u�W�F�N�g�̍č\�z
	bool	restruct_thumb();				// �T���l�C���I�u�W�F�N�g�̍č\�z
	bool	restruct_capture();				// �L���v�`���[�I�u�W�F�N�g�̍č\�z
	bool	restruct_movie();				// ���[�r�[�I�u�W�F�N�g�̍č\�z
	bool	restruct_emote();				// E-mote �I�u�W�F�N�g�̍č\�z
	bool	restruct_button_action();		// �{�^���A�N�V�������č\�z
	void	setup_weather_param(int sub_no, int init_state);		// ���p�����[�^���Z�b�g�A�b�v

	// �t���[������
	void	update_weather_time(int past_game_time, int past_real_time);	// ���I�u�W�F�N�g�̎��Ԃ��X�V
	void	string_frame();					// ������I�u�W�F�N�g�̃t���[������
	void	number_frame();					// ���l�I�u�W�F�N�g�̃t���[������
	void	weather_frame();				// ���I�u�W�F�N�g�̃t���[������
	void	movie_frame();					// ���[�r�[�I�u�W�F�N�g�̃t���[������

	// �ϊ����� obp (object base param) �� trp (tnm render param) �� rp (render param) 
	void	create_trp();
	void	trp_to_rp(S_tnm_sprite_render_param* rpex, S_d3d_render_param* rp);

	// �t���[���A�N�V����
	void	do_frame_action_func();		// �t���[���A�N�V���������s����
};

