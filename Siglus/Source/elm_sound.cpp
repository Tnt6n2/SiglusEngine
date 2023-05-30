#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_def.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// �T�E���h
// ================================================================
//		�T�E���h�͂T��ނ���܂��B
//			BGM		�P�̐�p�o�b�t�@
//			KOE		�P�̐�p�o�b�t�@
//			PCM		�P�U�̔ėp�o�b�t�@
//			PCMCH	�ό̐�p�o�b�t�@
//			SE		�P�U�̔ėp�o�b�t�@
// ================================================================
//		PCMCH �̃o�b�t�@���� Gameexe.ini �Ŏw��ł��܂��B
// ================================================================
//		SE �̓o�^���͉ςł����A�o�b�t�@���͌Œ�Ȃ̂Œ��ӁB
// ================================================================


// ****************************************************************
// �T�E���h�F������
// ================================================================
void C_elm_sound::init()
{
	// BGM �̏�����
	m_bgm.init(S_element(ELM_GLOBAL_BGM), FM_BGM, _T("bgm"));

	// KOE �̏�����
	m_koe.init(S_element(ELM_GLOBAL_KOE_ST), FM_KOE, _T("koe"));

	// PCM �̏�����
	m_pcm.init(S_element(ELM_GLOBAL_PCM), FM_PCM, _T("pcm"), TNM_PLAYER_TYPE_PCM);

	// PCMCH �̏�����
	m_pcmch_list.init(S_element(ELM_GLOBAL_PCMCH), FM_PCMCHLIST, _T("pcmch"), Gp_ini->pcmch_cnt, false, NULL);

	// SE �̏�����
	m_se.init(S_element(ELM_GLOBAL_SE), FM_SE, _T("se"), TNM_PLAYER_TYPE_SE);

	// MOV �̏�����
	m_mov.init(S_element(ELM_GLOBAL_MOV), FM_MOV, _T("mov"));
}

// ****************************************************************
// �T�E���h�F���
// ================================================================
void C_elm_sound::free()
{
	// �T�|�[�g�p�̖����T�E���h�����
	m_muon_sound.reset();
	m_muon_stream.reset();
	m_muon_player.reset();

	// MOV �̉��
	m_mov.free();

	// SE �̉��
	m_se.free();

	// PCMCH �̉��
	int pcmch_cnt = m_pcmch_list.get_size();
	for (int i = 0; i < pcmch_cnt; i++)
		m_pcmch_list[i].free();

	// PCM �̉��
	m_pcm.free();

	// KOE �̉��
	m_koe.free();

	// BGM �̉��
	m_bgm.free();
}

// ****************************************************************
// �T�E���h�F�ď�����
// ================================================================
void C_elm_sound::reinit()
{
	// BGM �̍ď�����
	m_bgm.reinit();

	// KOE �̍ď�����
	m_koe.reinit();

	// PCM �̍ď�����
	m_pcm.reinit();

	// PCMCH �̍ď�����
	m_pcmch_list.reinit();

	// SE �̍ď�����
	m_se.reinit();

	// MOV �̍ď�����
	m_mov.reinit();
}

// ****************************************************************
// �T�E���h�F�Z�[�u
// ================================================================
void C_elm_sound::save(C_tnm_save_stream& stream)
{
	// BGM
	m_bgm.save(stream);

	// KOE
	m_koe.save(stream);

	// PCM
	m_pcm.save(stream);

	// PCMCH
	m_pcmch_list.save(stream);

	// SE
	m_se.save(stream);

	// MOV
	m_mov.save(stream);
}

// ****************************************************************
// �T�E���h�F���[�h
// ================================================================
void C_elm_sound::load(C_tnm_save_stream& stream)
{
	// BGM
	m_bgm.load(stream);

	// KOE
	m_koe.load(stream);

	// PCM
	m_pcm.load(stream);

	// PCMCH
	m_pcmch_list.load(stream);

	// SE
	m_se.load(stream);

	// MOV
	m_mov.load(stream);
}

// ****************************************************************
// �T�E���h�F�t���[������
// ================================================================
void C_elm_sound::frame()
{
	m_mov.frame();
}
