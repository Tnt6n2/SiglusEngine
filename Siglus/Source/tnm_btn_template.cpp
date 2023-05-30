#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_btn_template.h"

// ****************************************************************
// ボタンアクションテンプレート：初期化
// ================================================================
void C_tnm_btn_action_template::init(int btn_no)
{
	// ini によって初期化される
	for (int i = 0; i < TNM_BTN_STATE_MAX; i++)	{
		action[i].rep_pat_no = Gp_ini->button.action[btn_no].state[i].rep_pat_no;
		action[i].rep_pos = Gp_ini->button.action[btn_no].state[i].rep_pos;
		action[i].rep_tr = Gp_ini->button.action[btn_no].state[i].rep_tr;
		action[i].rep_bright = Gp_ini->button.action[btn_no].state[i].rep_bright;
		action[i].rep_dark = Gp_ini->button.action[btn_no].state[i].rep_dark;
	}
}

// ****************************************************************
// ボタンアクションテンプレート：アクションを取得
// ================================================================
S_tnm_btn_action_template_pat* C_tnm_btn_action_template::get_action(int state)
{
	if (0 <= state && state < TNM_BTN_STATE_MAX)
		return &action[state];

	return NULL;
}

// ****************************************************************
// ボタンアクションテンプレートリスト：初期化
// ================================================================
void C_tnm_btn_action_template_list::init()
{
	// ボタンアクションテンプレートを作成
	m_sub.resize(Gp_ini->button.action_cnt);

	// ボタンアクションテンプレートを初期化
	for (int i = 0; i < (int)m_sub.size(); i++)
		m_sub[i].init(i);
}

// ****************************************************************
// ボタン効果音テンプレート：初期化
// ================================================================
void C_tnm_btn_se_template::init(int btn_no)
{
	// ini によって初期化される
	hit_no = Gp_ini->button.se[btn_no].hit_no;
	push_no = Gp_ini->button.se[btn_no].push_no;
	decide_no = Gp_ini->button.se[btn_no].decide_no;
}

// ****************************************************************
// ボタン効果音テンプレートリスト：初期化
// ================================================================
void C_tnm_btn_se_template_list::init()
{
	// ボタン効果音テンプレートを作成
	m_sub.resize(Gp_ini->button.se_cnt);

	// ボタン効果音テンプレートを初期化
	for (int i = 0; i < (int)m_sub.size(); i++)
		m_sub[i].init(i);
}
