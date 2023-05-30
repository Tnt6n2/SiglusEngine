#pragma		once

#include	"data/tnm_def.h"
#include	"data/tnm_sorter.h"
#include	"element/elm_world.h"

const int	TNM_SPRITE_SORT_TYPE_DEFAULT = 0;
const int	TNM_SPRITE_SORT_TYPE_NONE = 1;
const int	TNM_SPRITE_SORT_TYPE_PAT_NO = 2;

// ****************************************************************
// �ƂȂ܁F�����_�[�p�����[�^�d�w
// ================================================================
struct S_tnm_sprite_render_param
{
	S_tnm_sprite_render_param()
	{
		init();
	}
	void init()
	{
		sorter.order = 0;
		sorter.layer = 0;
		p_world = NULL;
		child_sort_type = TNM_SPRITE_SORT_TYPE_DEFAULT;
	}

	S_tnm_sorter	sorter;
	C_elm_world*	p_world;
	int				child_sort_type;
};

// ****************************************************************
// �ƂȂ܁F�X�v���C�g
// ================================================================
class C_tnm_sprite : public C_d3d_sprite
{
public:
	C_tnm_sprite();
	~C_tnm_sprite();

	// �X�v���C�g�������������
	void init();
	// �X�v���C�g���������i������Ȃ��j
	void init_no_release();
	// �X�v���C�g�����
	void release();

	// �����_�[�p�����[�^�d�w
	S_tnm_sprite_render_param		rpex;
};

// ****************************************************************
// �ƂȂ܁F�����_�[�p�����[�^
// ================================================================
struct S_tnm_render_param
{
	int				disp;			// �\���t���O
	int				pat_no;			// �p�^�[���ԍ�
	S_tnm_sorter	sorter;			// �\�[�^�[
	C_elm_world*	p_world;		// ���[���h

	// ���_�֘A
	float			pos_x;			// �ʒu
	float			pos_y;			// �ʒu
	float			pos_z;			// �ʒu
	float			center_x;		// ���S
	float			center_y;		// ���S
	float			center_z;		// ���S
	float			center_rep_x;	// ���S�␳
	float			center_rep_y;	// ���S�␳
	float			center_rep_z;	// ���S�␳
	float			scale_x;		// �g�k
	float			scale_y;		// �g�k
	float			scale_z;		// �g�k
	float			rotate_x;		// ��]
	float			rotate_y;		// ��]
	float			rotate_z;		// ��]
	int				dst_clip_use;	// �\���͈͎g�p
	int				dst_clip_left;	// �\���͈�
	int				dst_clip_top;	// �\���͈�
	int				dst_clip_right;	// �\���͈�
	int				dst_clip_bottom;// �\���͈�
	int				src_clip_use;	// �\����`�g�p
	int				src_clip_left;	// �\����`
	int				src_clip_top;	// �\����`
	int				src_clip_right;	// �\����`
	int				src_clip_bottom;// �\����`

	// �s�N�Z���֘A
	int				tr;				// �s�����x
	int				mono;			// ���m�N��
	int				reverse;		// ���]�x
	int				bright;			// ���x
	int				dark;			// �Óx
	int				color_r;		// �F�ϊ��i�q�j
	int				color_g;		// �F�ϊ��i�f�j
	int				color_b;		// �F�ϊ��i�a�j
	int				color_rate;		// �F�ϊ��i���x�j
	int				color_add_r;	// �F���Z�i�q�j
	int				color_add_g;	// �F���Z�i�f�j
	int				color_add_b;	// �F���Z�i�a�j
	int				mask_no;		// �}�X�N
	int				tone_curve_no;	// �g�[���J�[�u
	int				light_no;		// ���C�g

	// �����֘A
	bool			culling;		// �J�����O
	bool			alpha_test;		// �A���t�@�e�X�g
	bool			alpha_blend;	// �A���t�@�u�����h
	int				blend_type;		// �������@

	// ���̑�
	int				button_state;	// �{�^���X�e�[�g

	// �R���X�g���N�^
	S_tnm_render_param()
	{
		init();
	}

	// ������
	void init()
	{
		disp = false;
		pat_no = 0;
		sorter.order = 0;
		sorter.layer = 0;
		p_world = NULL;
		pos_x = 0.0f;
		pos_y = 0.0f;
		pos_z = 0.0f;
		center_x = 0.0f;
		center_y = 0.0f;
		center_z = 0.0f;
		center_rep_x = 0.0f;
		center_rep_y = 0.0f;
		center_rep_z = 0.0f;
		scale_x = 1.0f;
		scale_y = 1.0f;
		scale_z = 1.0f;
		rotate_x = 0.0f;
		rotate_y = 0.0f;
		rotate_z = 0.0f;
		dst_clip_use = 0;
		dst_clip_left = 0;
		dst_clip_top = 0;
		dst_clip_right = 0;
		dst_clip_bottom = 0;
		src_clip_use = 0;
		src_clip_left = 0;
		src_clip_top = 0;
		src_clip_right = 0;
		src_clip_bottom = 0;
		tr = 255;
		mono = 0;
		reverse = 0;
		bright = 0;
		dark = 0;
		color_r = 0;
		color_g = 0;
		color_b = 0;
		color_rate = 0;
		color_add_r = 0;
		color_add_g = 0;
		color_add_b = 0;
		mask_no = TNM_MASK_NONE;
		tone_curve_no = TNM_TONE_CURVE_NONE;
		light_no = TNM_LIGHT_NONE;
		culling = false;
		alpha_test = true;
		alpha_blend = true;
		blend_type = TNM_BLEND_TYPE_NORMAL;
		button_state = TNM_BTN_STATE_NORMAL;
	}
};

