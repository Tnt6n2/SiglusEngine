#pragma		once

#include	"data/tnm_def.h"

// ****************************************************************
// �{�^���A�N�V�����̃e���v���[�g�p�^�[��
// ================================================================
//		�{�^���̏�ԒP�́iHIT�APUSH�A�Ȃǁj�̃A�N�V�����̐ݒ�
// ================================================================
struct S_tnm_btn_action_template_pat
{
public:
	S_tnm_btn_action_template_pat()
	{
		rep_pat_no = 0;
		rep_pos    = C_point(0, 0);
		rep_tr     = 255;
		rep_bright = 0;
		rep_dark   = 0;
	}

	int			rep_pat_no;
	C_point		rep_pos;
	BYTE		rep_tr;
	BYTE		rep_bright;
	BYTE		rep_dark;
};

// ****************************************************************
// �{�^���A�N�V�����e���v���[�g
// ================================================================
//		�{�^���̊e��ԁiHIT�APUSH�A�Ȃǁj�̃A�N�V�����̐ݒ�
// ================================================================
class C_tnm_btn_action_template
{
public:
	void	init(int btn_no);

	S_tnm_btn_action_template_pat*	get_action(int state);

private:

	S_tnm_btn_action_template_pat	action[TNM_BTN_STATE_MAX];
};

// ****************************************************************
// �{�^���A�N�V�����e���v���[�g���X�g
// ================================================================
class C_tnm_btn_action_template_list
{
public:
	void	init();

	ARRAY<C_tnm_btn_action_template>		m_sub;
};

// ****************************************************************
// �{�^�����ʉ��̃e���v���[�g
// ================================================================
//		�{�^���̌��ʉ��̐ݒ�
// ================================================================
class C_tnm_btn_se_template
{
public:
	C_tnm_btn_se_template()
	{
		hit_no = -1;
		push_no = -1;
		decide_no = -1;
	}

	void	init(int btn_no);

	int		hit_no;
	int		push_no;
	int		decide_no;
};

// ****************************************************************
// �{�^�����ʉ��e���v���[�g���X�g
// ================================================================
class C_tnm_btn_se_template_list
{
public:
	void	init();

	ARRAY<C_tnm_btn_se_template>		m_sub;
};
