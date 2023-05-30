#include	"pch.h"

#include	"tnm_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_syscom.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_read_flag.h"

#include	"element/elm_flag.h"

#include	"engine/eng_display.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_etc.h"
#include	"engine/eng_save.h"
#include	"engine/ifc_stack.h"
#include	"engine/eng_flag.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// �Z�[�u�i���[�J���j
// ================================================================
bool tnm_save_local()
{
	// �G���W���{�̂��Ăяo��
	return Gp_eng->save_local();
}

// ****************************************************************
// ���[�h�i���[�J���j
// ================================================================
bool tnm_load_local()
{
	// �G���W���{�̂��Ăяo��
	return Gp_eng->load_local();
}

// ****************************************************************
// �Z�[�u�i���[�J���̌��݂̕��́j
// ================================================================
bool tnm_save_local_msg(CTSTR& msg)
{
	// �G���W���{�̂��Ăяo��
	return Gp_eng->save_local_msg(msg);
}

// ****************************************************************
// �Z�[�u�i�R�[���j
// ================================================================
bool tnm_save_call()
{
	// �G���W���{�̂��Ăяo��
	return Gp_eng->save_call();
}

// ****************************************************************
// ���[�h�i�R�[���j
// ================================================================
bool tnm_load_call()
{
	// �G���W���{�̂��Ăяo��
	return Gp_eng->load_call();
}

// ****************************************************************
// �Z�[�u������������
// ================================================================
void tnm_init_save_history()
{
	if (Gp_global->debug_flag)
	{
		Gp_save_history->update_flag = false;
		Gp_save_history->update_time = ::timeGetTime();
		Gp_save_history->top = 0;
		Gp_save_history->cnt = 0;
		Gp_save_history->item.resize(Gp_ini->save_history_cnt);
	}
}

// ****************************************************************
// ���b�Z�[�W�o�b�N�Z�[�u��������
// ================================================================
void tnm_init_backlog_save()
{
	Gp_backlog_save->map.clear();
}

// ****************************************************************
// ���b�Z�[�W�o�b�N�Z�[�u�� ID �����݂��邩�𔻒�
// ================================================================
bool tnm_check_backlog_save(S_tid id)
{
	return Gp_backlog_save->map.find(id) != Gp_backlog_save->map.end();
}

// ****************************************************************
// ���b�Z�[�W�o�b�N�Z�[�u�𐮗�����
// ================================================================
void tnm_remove_backlog_save()
{
	Gp_msg_back->remove();
}
	
// ****************************************************************
// ���[�J���Z�[�u�f�[�^��������
// ================================================================
void tnm_init_local_save()
{
	Gp_eng->m_local_save_history_index = -1;
	Gp_eng->m_local_save.save_id = S_tid();
	Gp_eng->m_local_save.append_dir.clear();
	Gp_eng->m_local_save.append_name.clear();
	Gp_eng->m_local_save.save_scene_title.clear();
	Gp_eng->m_local_save.save_msg.clear();
	Gp_eng->m_local_save.save_full_msg.clear();
	Gp_eng->m_local_save.save_stream.clear();
	Gp_eng->m_local_save.save_stream_ex.clear();
}

// ****************************************************************
// ���[�J���Z�[�u�f�[�^��ݒ�
// ================================================================
void tnm_set_local_save(S_tnm_local_save* p_local_save)
{
	Gp_eng->m_local_save = *p_local_save;
}

// ****************************************************************
// ���[�J���Z�[�u�f�[�^�𔻒�
// ================================================================
bool tnm_is_exist_local_save()
{
	return !Gp_eng->m_local_save.save_stream.empty();
}

// ****************************************************************
// �I�����Z�[�u�f�[�^��������
// ================================================================
void tnm_init_sel_save()
{
	Gp_eng->m_sel_save.list.clear();
	Gp_eng->m_sel_save.map.clear();
}

void tnm_init_sel_save_stock()
{
	Gp_eng->m_sel_save_stock.save_id = S_tid();
	Gp_eng->m_sel_save_stock.append_dir.clear();
	Gp_eng->m_sel_save_stock.append_name.clear();
	Gp_eng->m_sel_save_stock.save_scene_title.clear();
	Gp_eng->m_sel_save_stock.save_msg.clear();
	Gp_eng->m_sel_save_stock.save_full_msg.clear();
	Gp_eng->m_sel_save_stock.save_stream.clear();
	Gp_eng->m_sel_save_stock.save_stream_ex.clear();
}

// ****************************************************************
// �I�����Z�[�u�f�[�^��ǉ�
// ================================================================
void tnm_set_sel_save(S_tnm_local_save* p_local_save)
{
	if (Gp_ini->sel_save_cnt > 0 && !p_local_save->save_stream.empty())
	{
		// ���t�Ȃ�擪���폜
		if (Gp_eng->m_sel_save.list.size() == Gp_ini->sel_save_cnt)
		{
			S_tid erase_id = Gp_eng->m_sel_save.list[0];
			Gp_eng->m_sel_save.map.erase(erase_id);
			Gp_eng->m_sel_save.list.erase(Gp_eng->m_sel_save.list.begin());
		}

		// �Z�[�u�f�[�^���R�s�[
		BSP<S_tnm_local_save> save(new S_tnm_local_save);
		save->save_id = p_local_save->save_id;
		save->append_dir = p_local_save->append_dir;
		save->append_name = p_local_save->append_name;
		save->save_scene_title = p_local_save->save_scene_title;
		save->save_msg = p_local_save->save_msg;
		save->save_full_msg = p_local_save->save_full_msg;
		save->save_stream = p_local_save->save_stream;
		save->save_stream_ex = p_local_save->save_stream_ex;

		// �Z�[�u�f�[�^��ǉ�
		Gp_eng->m_sel_save.list.push_back(p_local_save->save_id);
		Gp_eng->m_sel_save.map[p_local_save->save_id] = save;
	}
}

// ****************************************************************
// �I�����Z�[�u�f�[�^���擾
// ================================================================
void tnm_get_sel_save(S_tnm_local_save* p_local_save)
{
	if (!Gp_eng->m_sel_save.list.empty())
	{
		S_tid save_id = Gp_eng->m_sel_save.list.back();
		p_local_save->save_id = Gp_eng->m_sel_save.map[save_id]->save_id;
		p_local_save->append_dir = Gp_eng->m_sel_save.map[save_id]->append_dir;
		p_local_save->append_name = Gp_eng->m_sel_save.map[save_id]->append_name;
		p_local_save->save_scene_title = Gp_eng->m_sel_save.map[save_id]->save_scene_title;
		p_local_save->save_msg = Gp_eng->m_sel_save.map[save_id]->save_msg;
		p_local_save->save_full_msg = Gp_eng->m_sel_save.map[save_id]->save_full_msg;
		p_local_save->save_stream = Gp_eng->m_sel_save.map[save_id]->save_stream;
		p_local_save->save_stream_ex = Gp_eng->m_sel_save.map[save_id]->save_stream_ex;
	}
}

// ****************************************************************
// �I�����Z�[�u�f�[�^�𔻒�
// ================================================================
bool tnm_is_exist_sel_save()
{
	return !Gp_eng->m_sel_save.list.empty();
}

// ****************************************************************
// ���[�J���Z�[�u�f�[�^��I�����Z�[�u�f�[�^�ɃR�s�[
// ================================================================
void tnm_copy_local_save_to_sel_save()
{
	tnm_set_sel_save(&Gp_eng->m_local_save);
}
void tnm_copy_local_save_to_sel_save_stock()
{
	Gp_eng->m_sel_save_stock = Gp_eng->m_local_save;
}

// ****************************************************************
// �I�����Z�[�u�f�[�^���R�s�[
// ================================================================
void tnm_copy_sel_save_stock_to_sel_save()
{
	tnm_set_sel_save(&Gp_eng->m_sel_save_stock);
}

// ****************************************************************
// �C���i�[�Z�[�u
// ================================================================
void tnm_savedata_system_clear_inner_save(int inner_save_no)
{
	if (inner_save_no < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no)
		return;

	Gp_eng->m_inner_save_list[inner_save_no].save_id = S_tid();
	Gp_eng->m_inner_save_list[inner_save_no].append_dir.clear();
	Gp_eng->m_inner_save_list[inner_save_no].append_name.clear();
	Gp_eng->m_inner_save_list[inner_save_no].save_scene_title.clear();
	Gp_eng->m_inner_save_list[inner_save_no].save_msg.clear();
	Gp_eng->m_inner_save_list[inner_save_no].save_full_msg.clear();
	Gp_eng->m_inner_save_list[inner_save_no].save_stream.clear();
	Gp_eng->m_inner_save_list[inner_save_no].save_stream_ex.clear();
}

void tnm_savedata_system_copy_local_save_to_inner_save(int inner_save_no)
{
	if (inner_save_no < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no)
		return;

	Gp_eng->m_inner_save_list[inner_save_no] = Gp_eng->m_local_save;
}

void tnm_savedata_system_copy_inner_save(int inner_save_no_from, int inner_save_no_to)
{
	if (inner_save_no_from < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no_from)
		return;

	if (inner_save_no_to < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no_to)
		return;

	Gp_eng->m_inner_save_list[inner_save_no_to] = Gp_eng->m_inner_save_list[inner_save_no_from];
}

void tnm_savedata_system_get_inner_save(int inner_save_no, S_tnm_local_save* p_local_save)
{
	if (inner_save_no < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no)
		return;

	*p_local_save = Gp_eng->m_inner_save_list[inner_save_no];
}

bool tnm_savedata_system_check_inner_save(int inner_save_no)
{
	if (inner_save_no < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no)
		return false;

	if (Gp_eng->m_inner_save_list[inner_save_no].save_stream.empty())
		return false;

	return true;
}

int tnm_savedata_system_get_inner_save_size(int inner_save_no)
{
	if (inner_save_no < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no)
		return 0;

	return (int)Gp_eng->m_inner_save_list[inner_save_no].save_stream.size();
}

// ****************************************************************
// �t�@�C���Z�[�u�i�w�b�_�j
// ================================================================
bool tnm_save_save_header(int save_no, S_tnm_save_header* p_header)
{
	if (p_header == NULL)
		return false;

	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// �L���b�V���փZ�[�u
	if (!Gp_eng->m_save_cache.save_cache(save_no, p_header))
		return false;

	return true;
}

// ****************************************************************
// �t�@�C�����[�h�i�w�b�_�j
// ================================================================
bool tnm_load_save_header(int save_no, S_tnm_save_header* p_header)
{
	if (p_header == NULL)
		return false;

	ZeroMemory(p_header, sizeof(*p_header));

	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// �L���b�V�����烍�[�h
	if (!Gp_eng->m_save_cache.load_cache(save_no, p_header))
		return false;

	return true;
}

// ****************************************************************
// �Z�[�u�\���ǂ�������
// ================================================================
bool tnm_is_save_enable()
{
	// �X�g���[������Ȃ�Z�[�u�ł��Ȃ�
	if (Gp_eng->m_local_save.save_stream.empty())
		return false;

	return true;
}

// ****************************************************************
// �Z�[�u�\���ǂ�������
// ================================================================
bool tnm_is_save_enable(int save_no)
{
	// �X�g���[������Ȃ�Z�[�u�ł��Ȃ�
	if (Gp_eng->m_local_save.save_stream.empty())
		return false;

	return true;
}

// ****************************************************************
// ���[�h�\���ǂ�������
// ================================================================
bool tnm_is_load_enable()
{
	return true;	// �K���o����
}

// ****************************************************************
// ���[�h�\���ǂ�������
// ================================================================
bool tnm_is_load_enable(int save_no)
{
	// �t�@�C�����Ȃ��Ȃ烍�[�h�ł��Ȃ�
	if (!tnm_check_save_file(save_no))
		return false;

	return true;
}

// ****************************************************************
// �t�@�C���`�F�b�N
// ================================================================
bool tnm_check_save_file(int save_no)
{
	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// �L���b�V������`�F�b�N
	if (!Gp_eng->m_save_cache.check_file(save_no))
		return false;

	return true;
}

// ****************************************************************
// �ŐV�̃Z�[�u�f�[�^�ԍ����擾
// ================================================================
int tnm_get_new_save_no()
{
	return Gp_eng->m_save_cache.get_new_save_data_no();
}

int tnm_get_new_save_no(int start, int cnt)
{
	// �͈͎w��̏ꍇ�͌��ʂ��L���b�V���ł��Ȃ��̂ŒT���܂�
	// �e�f�[�^�̓��t���̂̓L���b�V�����g���܂�

	int new_no = -1;
	S_tnm_save_header new_header = { 0 };

	for (int i = 0; i < cnt; i++)
	{
		int save_no = start + i;

		S_tnm_save_header header = { 0 };

		if (0 <= save_no && save_no < Gp_ini->save_cnt)
		{
			if (Gp_eng->m_save_cache.load_cache(save_no, &header))
			{
				if (new_no == -1 || new_header < header)
				{
					new_no = save_no;
					new_header = header;
				}
			}
		}
	}

	return new_no;
}

// ****************************************************************
// �ŐV�̃N�C�b�N�Z�[�u�f�[�^�ԍ����擾
// ================================================================
int tnm_get_new_quick_save_no()
{
	return Gp_eng->m_save_cache.get_new_quick_save_data_no();
}

int tnm_get_new_quick_save_no(int start, int cnt)
{
	// �͈͎w��̏ꍇ�͌��ʂ��L���b�V���ł��Ȃ��̂ŒT���܂�
	// �e�f�[�^�̓��t���̂̓L���b�V�����g���܂�

	int new_no = -1;
	S_tnm_save_header new_header = { 0 };

	for (int i = 0; i < cnt; i++)
	{
		int quick_save_no = start + i;

		S_tnm_save_header header = { 0 };

		if (!Gp_eng->m_save_cache.load_cache(Gp_ini->save_cnt + quick_save_no, &header))
		{
			if (new_no == -1 || new_header < header)
			{
				new_no = quick_save_no;
				new_header = header;
			}
		}
	}

	return new_no;
}