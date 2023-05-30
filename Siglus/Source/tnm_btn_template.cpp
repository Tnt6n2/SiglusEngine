#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_btn_template.h"

// ****************************************************************
// �{�^���A�N�V�����e���v���[�g�F������
// ================================================================
void C_tnm_btn_action_template::init(int btn_no)
{
	// ini �ɂ���ď����������
	for (int i = 0; i < TNM_BTN_STATE_MAX; i++)	{
		action[i].rep_pat_no = Gp_ini->button.action[btn_no].state[i].rep_pat_no;
		action[i].rep_pos = Gp_ini->button.action[btn_no].state[i].rep_pos;
		action[i].rep_tr = Gp_ini->button.action[btn_no].state[i].rep_tr;
		action[i].rep_bright = Gp_ini->button.action[btn_no].state[i].rep_bright;
		action[i].rep_dark = Gp_ini->button.action[btn_no].state[i].rep_dark;
	}
}

// ****************************************************************
// �{�^���A�N�V�����e���v���[�g�F�A�N�V�������擾
// ================================================================
S_tnm_btn_action_template_pat* C_tnm_btn_action_template::get_action(int state)
{
	if (0 <= state && state < TNM_BTN_STATE_MAX)
		return &action[state];

	return NULL;
}

// ****************************************************************
// �{�^���A�N�V�����e���v���[�g���X�g�F������
// ================================================================
void C_tnm_btn_action_template_list::init()
{
	// �{�^���A�N�V�����e���v���[�g���쐬
	m_sub.resize(Gp_ini->button.action_cnt);

	// �{�^���A�N�V�����e���v���[�g��������
	for (int i = 0; i < (int)m_sub.size(); i++)
		m_sub[i].init(i);
}

// ****************************************************************
// �{�^�����ʉ��e���v���[�g�F������
// ================================================================
void C_tnm_btn_se_template::init(int btn_no)
{
	// ini �ɂ���ď����������
	hit_no = Gp_ini->button.se[btn_no].hit_no;
	push_no = Gp_ini->button.se[btn_no].push_no;
	decide_no = Gp_ini->button.se[btn_no].decide_no;
}

// ****************************************************************
// �{�^�����ʉ��e���v���[�g���X�g�F������
// ================================================================
void C_tnm_btn_se_template_list::init()
{
	// �{�^�����ʉ��e���v���[�g���쐬
	m_sub.resize(Gp_ini->button.se_cnt);

	// �{�^�����ʉ��e���v���[�g��������
	for (int i = 0; i < (int)m_sub.size(); i++)
		m_sub[i].init(i);
}
