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
// サウンド
// ================================================================
//		サウンドは５種類あります。
//			BGM		１つの専用バッファ
//			KOE		１つの専用バッファ
//			PCM		１６個の汎用バッファ
//			PCMCH	可変個の専用バッファ
//			SE		１６個の汎用バッファ
// ================================================================
//		PCMCH のバッファ数は Gameexe.ini で指定できます。
// ================================================================
//		SE の登録数は可変ですが、バッファ数は固定なので注意。
// ================================================================


// ****************************************************************
// サウンド：初期化
// ================================================================
void C_elm_sound::init()
{
	// BGM の初期化
	m_bgm.init(S_element(ELM_GLOBAL_BGM), FM_BGM, _T("bgm"));

	// KOE の初期化
	m_koe.init(S_element(ELM_GLOBAL_KOE_ST), FM_KOE, _T("koe"));

	// PCM の初期化
	m_pcm.init(S_element(ELM_GLOBAL_PCM), FM_PCM, _T("pcm"), TNM_PLAYER_TYPE_PCM);

	// PCMCH の初期化
	m_pcmch_list.init(S_element(ELM_GLOBAL_PCMCH), FM_PCMCHLIST, _T("pcmch"), Gp_ini->pcmch_cnt, false, NULL);

	// SE の初期化
	m_se.init(S_element(ELM_GLOBAL_SE), FM_SE, _T("se"), TNM_PLAYER_TYPE_SE);

	// MOV の初期化
	m_mov.init(S_element(ELM_GLOBAL_MOV), FM_MOV, _T("mov"));
}

// ****************************************************************
// サウンド：解放
// ================================================================
void C_elm_sound::free()
{
	// サポート用の無音サウンドを解放
	m_muon_sound.reset();
	m_muon_stream.reset();
	m_muon_player.reset();

	// MOV の解放
	m_mov.free();

	// SE の解放
	m_se.free();

	// PCMCH の解放
	int pcmch_cnt = m_pcmch_list.get_size();
	for (int i = 0; i < pcmch_cnt; i++)
		m_pcmch_list[i].free();

	// PCM の解放
	m_pcm.free();

	// KOE の解放
	m_koe.free();

	// BGM の解放
	m_bgm.free();
}

// ****************************************************************
// サウンド：再初期化
// ================================================================
void C_elm_sound::reinit()
{
	// BGM の再初期化
	m_bgm.reinit();

	// KOE の再初期化
	m_koe.reinit();

	// PCM の再初期化
	m_pcm.reinit();

	// PCMCH の再初期化
	m_pcmch_list.reinit();

	// SE の再初期化
	m_se.reinit();

	// MOV の再初期化
	m_mov.reinit();
}

// ****************************************************************
// サウンド：セーブ
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
// サウンド：ロード
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
// サウンド：フレーム処理
// ================================================================
void C_elm_sound::frame()
{
	m_mov.frame();
}
