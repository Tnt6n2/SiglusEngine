#include	"pch.h"
#include	"ifc_eng.h"

#include	"dialog/db_element_info.h"
#include	"dialog/db_wnd_info.h"
#include	"dialog/db_wnd_info_element_lv.h"

#include	"element/elm_list.h"
#include	"element/elm_flag.h"
#include	"element/elm_mask.h"
#include	"element/elm_world.h"
#include	"element/elm_stage.h"
#include	"element/elm_object.h"
#include	"element/elm_group.h"
#include	"element/elm_sound.h"
#include	"element/elm_pcm_event.h"
#include	"element/elm_frame_action.h"
#include	"element/elm_call.h"
#include	"element/elm_excall.h"

#include	"engine/eng_scene.h"
#include	"engine/flow_command.h"
#include	"engine/eng_flag.h"
#include	"engine/eng_mask.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// ���X�g�r���[������������i�G�������g�j�F�ėp�֐�
// ================================================================
void tnm_init_lv_element(int element_form, bool table_flag, C_tnm_dbg_listview* p_lv)
{
	p_lv->delete_all_column();

	// �e�[�u���\���̏ꍇ
	if (table_flag)	{
		p_lv->add_column(_T("no"), _T("no"), 30);

		switch (element_form)	{
			case FM_INTLIST:		// no break!
			case FM_INTEVENTLIST:	p_lv->add_column(_T("value"), _T("�l"), 50);	break;
			case FM_STRLIST:		p_lv->add_column(_T("value"), _T("�l"), 100);	break;
			case FM_COUNTERLIST:	for (int i = 0; i < sizeof_array(G_counter_edit_param); i++)	{	p_lv->add_column(G_counter_edit_param[i].param_name, G_counter_edit_param[i].column_str, G_counter_edit_param[i].column_width);	}	break;
			case FM_MASKLIST:		for (int i = 0; i < sizeof_array(G_mask_edit_param); i++)		{	p_lv->add_column(G_mask_edit_param[i].param_name, G_mask_edit_param[i].column_str, G_mask_edit_param[i].column_width);			}	break;
			case FM_OBJECTLIST:		for (int i = 0; i < sizeof_array(G_object_edit_param); i++)		{	p_lv->add_column(G_object_edit_param[i].param_name, G_object_edit_param[i].column_str, G_object_edit_param[i].column_width);	}	break;
			case FM_GROUPLIST:		for (int i = 0; i < sizeof_array(G_group_edit_param); i++)		{	p_lv->add_column(G_group_edit_param[i].param_name, G_group_edit_param[i].column_str, G_group_edit_param[i].column_width);		}	break;
			case FM_MWNDLIST:		for (int i = 0; i < sizeof_array(G_mwnd_edit_param); i++)		{	p_lv->add_column(G_mwnd_edit_param[i].param_name, G_mwnd_edit_param[i].column_str, G_mwnd_edit_param[i].column_width);			}	break;
			case FM_EFFECTLIST:		for (int i = 0; i < sizeof_array(G_effect_edit_param); i++)		{	p_lv->add_column(G_effect_edit_param[i].param_name, G_effect_edit_param[i].column_str, G_effect_edit_param[i].column_width);	}	break;
			case FM_QUAKELIST:
				p_lv->add_column(_T("type"), _T("����"), 50);
				break;
			case FM_WORLDLIST:		for (int i = 0; i < sizeof_array(G_world_edit_param); i++)		{	p_lv->add_column(G_world_edit_param[i].param_name, G_world_edit_param[i].column_str, G_world_edit_param[i].column_width);		}	break;
			case FM_PCMCHLIST:
				p_lv->add_column(_T("state"), _T("���"), 50);
				p_lv->add_column(_T("file"), _T("̧��"), 100);
				p_lv->add_column(_T("volume"), _T("��ح��"), 50);
				break;
			case FM_PCMEVENTLIST:
				p_lv->add_column(_T("pcm_buf_no"), _T("�ޯ̧"), 50);
				p_lv->add_column(_T("state"), _T("���"), 50);
				p_lv->add_column(_T("time"), _T("����"), 50);
				p_lv->add_column(_T("line"), _T("ײ�"), 50);
				break;
			case FM_FRAMEACTIONLIST:
				p_lv->add_column(_T("scn"), _T("���"), 100);
				p_lv->add_column(_T("cmd"), _T("�����"), 100);
				break;
			case FM_CALLLIST:
				break;
		}
	}

	// �G�������g�\���̏ꍇ
	else	{

		p_lv->add_column(_T("param"), _T("���Ұ�"), 100);
		p_lv->add_column(_T("value"), _T("�l"), 150);
		p_lv->add_item(0, _T("����Ė�"));
		if (tnm_form_is_list(element_form))	{
			p_lv->add_item(0, _T("����"));
		}

		switch (element_form)	{
			case FM_COUNTER:	for (int i = 0; i < sizeof_array(G_counter_edit_param); i++)	{	p_lv->add_item(0, G_counter_edit_param[i].item_str);	}	break;
			case FM_MASK:		for (int i = 0; i < sizeof_array(G_mask_edit_param); i++)		{	p_lv->add_item(0, G_mask_edit_param[i].item_str);		}	break;
			case FM_STAGE:		for (int i = 0; i < sizeof_array(G_stage_edit_param); i++)		{	p_lv->add_item(0, G_stage_edit_param[i].item_str);		}	break;
			case FM_OBJECT:		for (int i = 0; i < sizeof_array(G_object_edit_param); i++)		{	p_lv->add_item(0, G_object_edit_param[i].item_str);		}	break;
			case FM_GROUP:		for (int i = 0; i < sizeof_array(G_group_edit_param); i++)		{	p_lv->add_item(0, G_group_edit_param[i].item_str);		}	break;
			case FM_MWND:		for (int i = 0; i < sizeof_array(G_mwnd_edit_param); i++)		{	p_lv->add_item(0, G_mwnd_edit_param[i].item_str);		}	break;
			case FM_EFFECT:		for (int i = 0; i < sizeof_array(G_effect_edit_param); i++)		{	p_lv->add_item(0, G_effect_edit_param[i].item_str);		}	break;
			case FM_QUAKE:
				p_lv->add_item(0, _T("����"));
				break;
			case FM_WORLD:		for (int i = 0; i < sizeof_array(G_world_edit_param); i++)		{	p_lv->add_item(0, G_world_edit_param[i].item_str);		}	break;
			case FM_BGM:
				p_lv->add_item(0, _T("���"));
				p_lv->add_item(0, _T("�o�^��"));
				p_lv->add_item(0, _T("��ح��"));
				break;
			case FM_KOE:
				p_lv->add_item(0, _T("���"));
				p_lv->add_item(0, _T("���ԍ�"));
				p_lv->add_item(0, _T("��׸���ԍ�"));
				p_lv->add_item(0, _T("koe/exkoe"));
				p_lv->add_item(0, _T("��ح��"));
				break;
			case FM_PCM:
				p_lv->add_item(0, _T("���"));
				p_lv->add_item(0, _T("��ح��"));
				break;
			case FM_PCMCH:
				p_lv->add_item(0, _T("���"));
				p_lv->add_item(0, _T("̧�ٖ�"));
				p_lv->add_item(0, _T("��ح��"));
				break;
			case FM_PCMEVENT:
				p_lv->add_item(0, _T("�ޯ̧"));
				p_lv->add_item(0, _T("���"));
				p_lv->add_item(0, _T("����"));
				p_lv->add_item(0, _T("ײ�"));
				break;
			case FM_SE:
				p_lv->add_item(0, _T("���"));
				p_lv->add_item(0, _T("��ح��"));
				break;
			case FM_MOV:
				p_lv->add_item(0, _T("���"));
				p_lv->add_item(0, _T("̧��"));
				break;
			case FM_FRAMEACTION:
				p_lv->add_item(0, _T("���"));
				p_lv->add_item(0, _T("�����"));
				break;
			case FM_CALL:
				break;
			case FM_EXCALL:
				p_lv->add_item(0, _T("�m��"));
				break;
		}
	}
}

// ****************************************************************
// �G�������g�F���X�g�̗v�f�����擾
// ================================================================
int tnm_get_list_size(C_tnm_element* p_element, int form, int bit)
{
	int size = 0;

	switch (form)	{
		case FM_INTLIST:
			switch (bit)	{
				case 32:	size = ((C_elm_int_list *)p_element)->get_size() * 1;	break;
				case 16:	size = ((C_elm_int_list *)p_element)->get_size() * 2;	break;
				case 8:		size = ((C_elm_int_list *)p_element)->get_size() * 4;	break;
				case 4:		size = ((C_elm_int_list *)p_element)->get_size() * 8;	break;
				case 2:		size = ((C_elm_int_list *)p_element)->get_size() * 16;	break;
				case 1:		size = ((C_elm_int_list *)p_element)->get_size() * 32;	break;
			}
			break;

		case FM_INTEVENTLIST:	size = ((C_elm_int_event_list *)p_element)->get_size();		break;
		case FM_STRLIST:		size = ((C_elm_str_list *)p_element)->get_size();			break;
		case FM_COUNTERLIST:	size = ((C_elm_counter_list *)p_element)->get_size();		break;
		case FM_MASKLIST:		size = ((C_elm_mask_list *)p_element)->get_size();			break;
		case FM_STAGELIST:		size = ((C_elm_stage_list *)p_element)->get_size();			break;
		case FM_OBJECTLIST:		size = ((C_elm_object_list *)p_element)->get_size();		break;
		case FM_GROUPLIST:		size = ((C_elm_group_list *)p_element)->get_size();			break;
		case FM_MWNDLIST:		size = ((C_elm_mwnd_list *)p_element)->get_size();			break;
		case FM_EFFECTLIST:		size = ((C_elm_effect_list *)p_element)->get_size();		break;
		case FM_QUAKELIST:		size = ((C_elm_quake_list *)p_element)->get_size();			break;
		case FM_WORLDLIST:		size = ((C_elm_world_list *)p_element)->get_size();			break;
		case FM_PCMCHLIST:		size = ((C_elm_pcmch_list *)p_element)->get_size();			break;
		case FM_PCMEVENTLIST:	size = ((C_elm_pcm_event_list *)p_element)->get_size();		break;
		case FM_CALLLIST:		size = ((C_elm_call_list *)p_element)->get_size();			break;
	}

	return size;
}

// ****************************************************************
// ���X�g�r���[���X�V����F�P���G�������g
// ================================================================

// ����
void tnm_update_lv_int(const int* p_int, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	// ���̑��̃p�����[�^��\��
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_int ? tostr(*p_int) : _T(""));		(*p_inc_param) ++;
}

// �����C�x���g
void tnm_update_lv_int_event(const C_elm_int_event* p_int_event, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	// ���̑��̃p�����[�^��\��
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_int_event ? tostr(p_int_event->get_total_value()) : _T(""));		(*p_inc_param) ++;
}

// ������
void tnm_update_lv_str(const TSTR* p_str, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	// ���̑��̃p�����[�^��\��
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_str ? *p_str : _T(""));		(*p_inc_param) ++;
}

// �J�E���^
void tnm_update_lv_counter(const C_elm_counter* p_counter, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_counter ? tostr(p_counter->get_count()) : _T(""));										(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_counter ? (p_counter->param().is_running ? _T("��") : _T("�~")) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_counter ? (p_counter->param().real_flag ? _T("������") : _T("�ްю���")) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_counter ? (p_counter->param().frame_mode ? _T("�ڰ�") : _T("����")) : _T(""));			(*p_inc_param) ++;
}

// �}�X�N
void tnm_update_lv_mask(const C_elm_mask* p_mask, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_mask->get_mask_name());				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, tostr(p_mask->get_total_x()));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, tostr(p_mask->get_total_y()));		(*p_inc_param) ++;
}

// ���[���h
void tnm_update_lv_world(const C_elm_world* p_world, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_camera_eye_total_x()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_camera_eye_total_y()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_camera_eye_total_z()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_camera_pint_total_x()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_camera_pint_total_y()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_camera_pint_total_z()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_camera_up_total_x()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_camera_up_total_y()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_camera_up_total_z()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_camera_view_angle()) : _T(""));		(*p_inc_param) ++;

	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_order()) : _T(""));		(*p_inc_param)++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_layer()) : _T(""));		(*p_inc_param)++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_wipe_copy()) : _T(""));	(*p_inc_param)++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_world ? tostr(p_world->get_wipe_erase()) : _T(""));	(*p_inc_param)++;
}

// �X�e�[�W
void tnm_update_lv_stage(C_elm_stage* p_stage, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_stage ? tostr(p_stage->m_object_list.get_size()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_stage ? tostr(p_stage->m_group_list.get_size()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_stage ? tostr(p_stage->m_mwnd_list.get_size()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_stage ? tostr(p_stage->m_quake_list.get_size()) : _T(""));		(*p_inc_param) ++;
}

// �I�u�W�F�N�g
void tnm_update_lv_object(C_elm_object* p_object, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	// �^�C�v
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tnm_object_type_get_str(p_object->get_type()) : _T(""));	(*p_inc_param) ++;

	// �t�@�C�����Ȃ�
	TSTR file_name;
	if (p_object)	{
		switch (p_object->get_type())	{
			case TNM_OBJECT_TYPE_RECT:
				file_name = tostr(p_object->get_filter_rect()) + _T(", ") + tostr(p_object->get_filter_color());
				break;
			case TNM_OBJECT_TYPE_PCT:		file_name = p_object->get_file_path();	break;
			case TNM_OBJECT_TYPE_STRING:	file_name = p_object->get_string();		break;
			case TNM_OBJECT_TYPE_NUMBER:	file_name = p_object->get_file_path();	break;
			case TNM_OBJECT_TYPE_WEATHER:	file_name = p_object->get_file_path();	break;
			case TNM_OBJECT_TYPE_MOVIE:		file_name = p_object->get_file_path();	break;
			case TNM_OBJECT_TYPE_MESH:		file_name = p_object->get_file_path();	break;
			case TNM_OBJECT_TYPE_EMOTE:		file_name = p_object->get_file_path();	break;
		}
	}
	p_lv->set_cell_text(*p_item_no, *p_column_no, file_name);		(*p_inc_param) ++;

	// ��{�p�����[�^
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_child_cnt()) : _T(""));								(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_disp()) : _T(""));										(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_pat_no_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_order()) : _T(""));									(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_layer()) : _T(""));									(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_world_no()) : _T(""));									(*p_inc_param) ++;

	// ���_�p�����[�^
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_x_event().get_total_value()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_y_event().get_total_value()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_z_event().get_total_value()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_center_x_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_center_y_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_center_z_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_center_rep_x_event().get_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_center_rep_y_event().get_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_center_rep_z_event().get_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_scale_x_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_scale_y_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_scale_z_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_rotate_x_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_rotate_y_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_rotate_z_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_clip_use()) : _T(""));									(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_clip_left_event().get_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_clip_top_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_clip_right_event().get_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_clip_bottom_event().get_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_src_clip_use()) : _T(""));								(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_src_clip_left_event().get_total_value()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_src_clip_top_event().get_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_src_clip_right_event().get_total_value()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_src_clip_bottom_event().get_total_value()) : _T(""));	(*p_inc_param) ++;

	// �s�N�Z���p�����[�^
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_tr_event().get_total_value()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_mono_event().get_total_value()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_reverse_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_bright_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_dark_event().get_total_value()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_color_r_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_color_g_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_color_b_event().get_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_color_rate_event().get_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_color_add_r_event().get_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_color_add_g_event().get_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_color_add_b_event().get_total_value()) : _T(""));		(*p_inc_param) ++;

	// ���̑�
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_tone_curve_no()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_culling()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_alpha_test()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_alpha_blend()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_blend_type()) : _T(""));		(*p_inc_param) ++;

	// �{�^��
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_button_no()) : _T(""));							(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_button_group_no()) : _T(""));						(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_button_action_no()) : _T(""));						(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_button_se_no()) : _T(""));							(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_button_state()) : _T(""));							(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? p_object->get_button_decided_action_scn_name() : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? p_object->get_button_decided_action_cmd_name() : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_button_decided_action_z_no()) : _T(""));			(*p_inc_param) ++;

	// �t���[���A�N�V����
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? p_object->get_frame_action()->m_scn_name : _T(""));					(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? p_object->get_frame_action()->m_cmd_name : _T(""));					(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_frame_action()->m_counter.get_count()) : _T(""));	(*p_inc_param) ++;

	// ���[�r�[
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_omv_timer()) : _T(""));	(*p_inc_param) ++;

	// ���C�v�R�s�[
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_wipe_copy()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_wipe_erase()) : _T(""));	(*p_inc_param) ++;

	// ���̑��̃t���O
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_hints()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_object ? tostr(p_object->get_child_sort_type()) : _T(""));	(*p_inc_param) ++;
}

// �O���[�v
void tnm_update_lv_group(C_elm_group* p_group, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	// ���肵���{�^���ԍ�
	TSTR decided_no_str;
	if (p_group)	{
		int decided_no = p_group->get_decided_button_no();
		decided_no_str = tostr(decided_no);
		if (decided_no == TNM_GROUP_NOT_DECIDED)
			decided_no_str += _T(" (���I��)");
		else if (decided_no == TNM_GROUP_CANCELED)
			decided_no_str += _T(" (��ݾ�)");
	}

	p_lv->set_cell_text(*p_item_no, *p_column_no, p_group ? (p_group->get_doing_flag() ? _T("�I��") : _T("�~")) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_group ? (p_group->get_pause_flag() ? _T("�|�[�Y��") : _T("�~")) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_group ? (p_group->get_wait_flag() ? _T("�I��҂�") : _T("�~")) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_group ? (p_group->get_cancel_flag() ? _T("��") : _T("�s��")) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_group ? tostr(p_group->get_cancel_se_no()) : _T(""));						(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_group ? tostr(p_group->get_hit_button_no()) : _T(""));						(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_group ? tostr(p_group->get_pushed_button_no()) : _T(""));					(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, decided_no_str);																(*p_inc_param) ++;
}

// ���b�Z�[�W�E�B���h�E
void tnm_update_lv_mwnd(C_elm_mwnd* p_mwnd, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	// �f�o�b�O���b�Z�[�W
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_mwnd ? tostr(p_mwnd->get_debug_msg()) : _T(""));			(*p_inc_param) ++;

	// �g�ԍ�
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_mwnd ? tostr(p_mwnd->get_waku_no()) : _T(""));				(*p_inc_param) ++;

	// ���O�g�ԍ�
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_mwnd ? tostr(p_mwnd->get_name_waku_no()) : _T(""));			(*p_inc_param) ++;

	// �N���A�����t���O
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_mwnd ? tostr(p_mwnd->get_clear_ready_flag()) : _T(""));		(*p_inc_param) ++;
}

// �X�N���[��
void tnm_update_lv_screen(C_elm_screen* p_screen, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
}

// �G�t�F�N�g
void tnm_update_lv_effect(const C_elm_effect* p_effect, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_x_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_y_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_z_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_mono_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_reverse_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_bright_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_dark_total_value()) : _T(""));			(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_color_r_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_color_g_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_color_b_total_value()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_color_rate_total_value()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_color_add_r_total_value()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_color_add_g_total_value()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_color_add_b_total_value()) : _T(""));	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_begin_order()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_begin_layer()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_end_order()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_end_layer()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_wipe_copy()) : _T(""));				(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_effect ? tostr(p_effect->get_wipe_erase()) : _T(""));				(*p_inc_param) ++;
}

// �N�G�C�N
void tnm_update_lv_quake(const C_elm_quake* p_quake, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	TSTR type_str;
	if (p_quake)	{
		switch (p_quake->get_type())	{
			case TNM_QUAKE_TYPE_NONE:	type_str = _T("");			break;
			case TNM_QUAKE_TYPE_VEC:	type_str = _T("0(�Е���)");	break;
			case TNM_QUAKE_TYPE_DIR:	type_str = _T("1(������)");	break;
			case TNM_QUAKE_TYPE_ZOOM:	type_str = _T("2(�ް�)");	break;
			case TNM_QUAKE_TYPE_ROTATE:	type_str = _T("3(��])");	break;
		}
	}

	p_lv->set_cell_text(*p_item_no, *p_column_no, type_str);		(*p_inc_param) ++;
}

// BGM
void tnm_update_lv_bgm(C_elm_bgm* p_bgm, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	TSTR state_str;
	if (p_bgm)	{

		if (false);
		else if (p_bgm->is_fade_out_doing())				{	state_str = _T("̪��ޱ��");			}
		else if (p_bgm->is_playing() && p_bgm->is_loop())	{	state_str = _T("�Đ�(ٰ��)");		}
		else if (p_bgm->is_playing() && !p_bgm->is_loop())	{	state_str = _T("�Đ�(�ݼ���)");		}
		else												{	state_str = _T("");					}
	}

	p_lv->set_cell_text(*p_item_no, *p_column_no, state_str);												(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_bgm ? tostr(p_bgm->get_regist_name()) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_bgm ? tostr(p_bgm->get_volume()) : _T(""));				(*p_inc_param) ++;
}

// ��
void tnm_update_lv_koe(C_elm_koe* p_koe, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	TSTR state_str;
	if (p_koe)	{

		int state = p_koe->check();

		if (false);
		else if (state == TNM_PLAYER_STATE_PLAY)			{	state_str = _T("�Đ�");			}
		else if (state == TNM_PLAYER_STATE_FADE_OUT)		{	state_str = _T("̪��ޱ��");		}
		else												{	state_str = _T("");				}
	}

	p_lv->set_cell_text(*p_item_no, *p_column_no, state_str);																		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_koe ? tostr(p_koe->get_koe_no()) : _T(""));										(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_koe ? tostr(p_koe->get_chara_no()) : _T(""));									(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_koe ? tostr(p_koe->get_ex_koe_flag() ? _T("exkoe") : _T("koe")) : _T(""));		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_koe ? tostr(p_koe->get_volume()) : _T(""));										(*p_inc_param) ++;
}

// ���ʉ�
void tnm_update_lv_pcm(C_elm_pcm* p_pcm, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	TSTR state_str;
	if (p_pcm)	{

		int state = p_pcm->check();

		if (false);
		else if (state == TNM_PLAYER_STATE_PLAY)		{	state_str = _T("�Đ�");			}
		else if (state == TNM_PLAYER_STATE_FADE_OUT)	{	state_str = _T("̪��ޱ��");		}
		else											{	state_str = _T("��~");			}
	}

	p_lv->set_cell_text(*p_item_no, *p_column_no, state_str);																		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_pcm ? tostr(p_pcm->get_volume()) : _T(""));										(*p_inc_param) ++;
}

// ���ʉ��`�����l��
void tnm_update_lv_pcmch(C_elm_pcmch* p_pcmch, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	TSTR state_str;
	if (p_pcmch)	{

		int state = p_pcmch->check();

		if (false);
		else if (state == TNM_PLAYER_STATE_PLAY && p_pcmch->is_loop())		{	state_str = _T("�Đ�(ٰ��)");		}
		else if (state == TNM_PLAYER_STATE_PLAY && !p_pcmch->is_loop())		{	state_str = _T("�Đ�(�ݼ���)");		}
		else if (state == TNM_PLAYER_STATE_FADE_OUT)						{	state_str = _T("̪��ޱ��");			}
		else																{	state_str = _T("");					}
	}

	p_lv->set_cell_text(*p_item_no, *p_column_no, state_str);																		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_pcmch ? tostr(p_pcmch->get_pcm_name()) : _T(""));								(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_pcmch ? tostr(p_pcmch->get_volume()) : _T(""));									(*p_inc_param) ++;
}

// ���ʉ��C�x���g
void tnm_update_lv_pcm_event(C_elm_pcm_event* p_pcm_event, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	TSTR state_str;
	if (p_pcm_event)	{

		int state = p_pcm_event->check();

		if (false);
		else if (p_pcm_event->get_type() == TNM_PCM_EVENT_TYPE_ONESHOT)	{	state_str = _T("�Đ�(�ݼ���)");		}
		else if (p_pcm_event->get_type() == TNM_PCM_EVENT_TYPE_LOOP)	{	state_str = _T("�Đ�(ٰ��)");		}
		else if (p_pcm_event->get_type() == TNM_PCM_EVENT_TYPE_RANDOM)	{	state_str = _T("�Đ�(�����)");		}
		else															{	state_str = _T("��~");				}
	}

	p_lv->set_cell_text(*p_item_no, *p_column_no, p_pcm_event ? tostr(p_pcm_event->get_pcm_buf_no()) : _T(""));					(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, state_str);																	(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_pcm_event ? tostr(p_pcm_event->get_cur_time()) : _T(""));					(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_pcm_event ? tostr(p_pcm_event->get_cur_line_no()) : _T(""));				(*p_inc_param) ++;
}

// �V�X�e����
void tnm_update_lv_se(C_elm_se* p_se, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	TSTR state_str;
	if (p_se)	{

		int state = p_se->check();

		if (false);
		else if (state == TNM_PLAYER_STATE_PLAY)		{	state_str = _T("�Đ�");			}
		else if (state == TNM_PLAYER_STATE_FADE_OUT)	{	state_str = _T("̪��ޱ��");		}
		else											{	state_str = _T("");				}
	}

	p_lv->set_cell_text(*p_item_no, *p_column_no, state_str);																		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_se ? tostr(p_se->get_volume()) : _T(""));										(*p_inc_param) ++;
}

// ���[�r�[
void tnm_update_lv_mov(C_elm_mov* p_mov, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	TSTR state_str;
	if (p_mov)	{

		if (false);
		else if (p_mov->is_playing())	{	state_str = _T("�Đ�");		}
		else							{	state_str = _T("");			}
	}

	p_lv->set_cell_text(*p_item_no, *p_column_no, state_str);																		(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_mov ? tostr(p_mov->get_file_name()) : _T(""));									(*p_inc_param) ++;
}

// �t���[���A�N�V����
void tnm_update_lv_frame_action(C_elm_frame_action* p_frame_action, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_frame_action ? tostr(p_frame_action->m_scn_name) : _T(""));					(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_frame_action ? tostr(p_frame_action->m_cmd_name) : _T(""));					(*p_inc_param) ++;
}

// �R�[��
void tnm_update_lv_call(C_elm_call* p_call, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_call ? tostr(p_call->L.get_size()) : _T(""));									(*p_inc_param) ++;
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_call ? tostr(p_call->K.get_size()) : _T(""));									(*p_inc_param) ++;
}

// �d�w�R�[��
void tnm_update_lv_excall(C_elm_excall* p_excall, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	p_lv->set_cell_text(*p_item_no, *p_column_no, p_excall ? tostr(p_excall->is_ready() ? _T("��") : _T("�~")) : _T(""));			(*p_inc_param) ++;
}

// ****************************************************************
// �e�[�u���\���̃��X�g�r���[���X�V����
// ================================================================

// �ėp���X�g
template <typename TYPE>
void tnm_update_lv_list_table(C_elm_list_base<TYPE>* p_list, C_tnm_dbg_listview* p_lv, int* p_item_no, bool reduce)
{
	if (p_list == NULL)	{
		p_lv->clear();
		return;
	}

	int sub_cnt = p_list->get_size();
	for (int i = 0; i < sub_cnt; i++)	{
		if (*p_item_no >= p_lv->get_item_cnt())
			p_lv->add_item();	// ���X�g�r���[�ɃA�C�e��������Ȃ���Βǉ�����

		int column_no = 1;
		p_lv->set_cell_text(*p_item_no, 0, tostr(i));	// �R����0�ԁF�T�u�ԍ�

		// �T�u���
		tnm_update_lv_simple_element(&(*p_list)[i], (*p_list)[i].get_element_form(), p_lv, p_item_no, &column_no, &column_no);
		(*p_item_no) ++;
	}
	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (reduce)	{
		tnm_reduce_item_cnt(p_lv, *p_item_no);
	}
}

// �������X�g�i�ėp���X�g�̓��ꉻ�c�������Ⴄ�̂Ŏ��ۂ͓��ꉻ�ł͂Ȃ��j
void tnm_update_lv_list_table(C_elm_list_base<int>* p_list, int bit, int top_index, C_tnm_dbg_listview* p_lv, int* p_item_no, bool reduce)
{
	if (p_list == NULL)	{
		p_lv->clear();
		return;
	}

	// �r�b�g���Z�Ɏg���l
	int rev_bit = 32 / bit;
	int mask = 
		bit == 16 ? 0x0000FFFF : 
		bit == 8 ? 0x000000FF : 
		bit == 4 ? 0x0000000F : 
		bit == 2 ? 0x00000003 : 
		bit == 1 ? 0x00000001 : 
		0xFFFFFFFF;

	// �ő�P�O�O�O�܂ŕ\������ibit == 32 �̏ꍇ�͑S�ĕ\������j
	int bottom_index = top_index + (bit == 32 ? p_list->get_size() : 1000);
	bottom_index = std::min(bottom_index, p_list->get_size() * rev_bit);

	// �K�v�ȃA�C�e�������v�Z����
	int need_cnt = bottom_index - top_index;

	// �A�C�e����������Ȃ���Α��₷
	if (need_cnt > p_lv->get_item_cnt())	{
		p_lv->set_item_cnt(need_cnt);
	}

	for (int i = top_index; i < bottom_index; i++)	{
		int column_no = 1;
		int value, org_value = (*p_list)[i / rev_bit];
		p_lv->set_cell_text(*p_item_no, 0, tostr(i));	// �R����0�ԁF�T�u�ԍ�
		value = (org_value >> ((i % rev_bit) * bit)) & mask;
		tnm_update_lv_int(&value, p_lv, p_item_no, &column_no, &column_no);
		(*p_item_no) ++;
	}

	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (reduce)	{
		tnm_reduce_item_cnt(p_lv, need_cnt);
	}
}

// �����C�x���g���X�g�i�ėp���X�g�̓��ꉻ�j
void tnm_update_lv_list_table(C_elm_list_base<C_elm_int_event>* p_list, C_tnm_dbg_listview* p_lv, int* p_item_no, bool reduce)
{
	if (p_list == NULL)	{
		p_lv->clear();
		return;
	}

	// �T�u���
	int sub_cnt = p_list->get_size();
	for (int i = 0; i < sub_cnt; i++)	{
		if (*p_item_no >= p_lv->get_item_cnt())
			p_lv->add_item();	// ���X�g�r���[�ɃA�C�e��������Ȃ���Βǉ�����

		int column_no = 1;
		p_lv->set_cell_text(*p_item_no, 0, tostr(i));	// �R����0�ԁF�T�u�ԍ�

		// �T�u���
		tnm_update_lv_int_event(&(*p_list)[i], p_lv, p_item_no, &column_no, &column_no);
		(*p_item_no) ++;
	}
	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (reduce)	{
		tnm_reduce_item_cnt(p_lv, *p_item_no);
	}
}

// �����񃊃X�g�i�ėp���X�g�̓��ꉻ�j
void tnm_update_lv_list_table(C_elm_list_base<TSTR>* p_list, C_tnm_dbg_listview* p_lv, int* p_item_no, bool reduce)
{
	if (p_list == NULL)	{
		p_lv->clear();
		return;
	}

	// �T�u���
	int sub_cnt = p_list->get_size();
	for (int i = 0; i < sub_cnt; i++)	{
		if (*p_item_no >= p_lv->get_item_cnt())	{
			p_lv->add_item();	// ���X�g�r���[�ɃA�C�e��������Ȃ���Βǉ�����
		}

		int column_no = 1;
		p_lv->set_cell_text(*p_item_no, 0, tostr(i));	// �R����0�ԁF�T�u�ԍ�

		// �T�u���
		tnm_update_lv_str(&(*p_list)[i], p_lv, p_item_no, &column_no, &column_no);
		(*p_item_no) ++;
	}
	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (reduce)	{
		tnm_reduce_item_cnt(p_lv, *p_item_no);
	}
}

// �I�u�W�F�N�g���X�g�i�ėp���X�g�̓��ꉻ�j
void tnm_update_lv_list_table(C_elm_object_list* p_object_list, C_tnm_dbg_listview* p_lv, int* p_item_no, bool reduce)
{
	if (p_object_list == NULL)	{
		p_lv->clear();
		return;
	}

	// �T�u���
	int sub_cnt = p_object_list->get_size();
	for (int i = 0; i < sub_cnt; i++)	{

		// �L���ȃI�u�W�F�N�g�̂ݕ\������
		if ((*p_object_list)[i].get_type() != TNM_OBJECT_TYPE_NONE || (*p_object_list)[i].get_child_cnt() > 0)	{
			if (*p_item_no >= p_lv->get_item_cnt())	{
				p_lv->add_item();			// ���X�g�r���[�ɃA�C�e��������Ȃ���Βǉ�����
			}

			int column_no = 1;
			p_lv->set_cell_text(*p_item_no, 0, tostr(i));	// �R����0�ԁF���O

			// �T�u���
			tnm_update_lv_simple_element(&(*p_object_list)[i], (*p_object_list)[i].get_element_form(), p_lv, p_item_no, &column_no, &column_no);
			(*p_item_no) ++;
		}
	}
	// ���X�g�r���[�̃A�C�e�����𒲐�����
	if (reduce)	{
		tnm_reduce_item_cnt(p_lv, *p_item_no);
	}
}

// ****************************************************************
// ���X�g�r���[���X�V����F�P��G�������g
// ================================================================

// �P��G�������g
void tnm_update_lv_simple_element(C_tnm_element* p_element, int element_form, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	// �P��G�������g�̏���\��
	switch (element_form)	{
		case FM_COUNTER:		tnm_update_lv_counter((C_elm_counter *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);			break;
		case FM_MASK:			tnm_update_lv_mask((C_elm_mask *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);					break;
		case FM_STAGE:			tnm_update_lv_stage((C_elm_stage *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);				break;
		case FM_OBJECT:			tnm_update_lv_object((C_elm_object *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);				break;
		case FM_GROUP:			tnm_update_lv_group((C_elm_group *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);				break;
		case FM_MWND:			tnm_update_lv_mwnd((C_elm_mwnd *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);					break;
		case FM_EFFECT:			tnm_update_lv_effect((C_elm_effect *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);				break;
		case FM_QUAKE:			tnm_update_lv_quake((C_elm_quake *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);				break;
		case FM_WORLD:			tnm_update_lv_world((C_elm_world *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);				break;
		case FM_BGM:			tnm_update_lv_bgm((C_elm_bgm *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);					break;
		case FM_KOE:			tnm_update_lv_koe((C_elm_koe *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);					break;
		case FM_PCM:			tnm_update_lv_pcm((C_elm_pcm *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);					break;
		case FM_PCMCH:			tnm_update_lv_pcmch((C_elm_pcmch *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);				break;
		case FM_PCMEVENT:		tnm_update_lv_pcm_event((C_elm_pcm_event *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);		break;
		case FM_SE:				tnm_update_lv_se((C_elm_se *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);						break;
		case FM_MOV:			tnm_update_lv_mov((C_elm_mov *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);					break;
		case FM_FRAMEACTION:	tnm_update_lv_frame_action((C_elm_frame_action *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);	break;
		case FM_CALL:			tnm_update_lv_call((C_elm_call *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);					break;
		case FM_EXCALL:			tnm_update_lv_excall((C_elm_excall *)p_element, p_lv, p_item_no, p_column_no, p_inc_param);				break;
	}
}

// ****************************************************************
// ���X�g�r���[���X�V����F�G�������g����
// ================================================================

// �ʏ�\��
void tnm_update_lv_element(C_tnm_element* p_element, int element_form, int bit, int top_index, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param)
{
	// ���O��\��
	TSTR element_name;
	if (p_element)	{
		element_name = p_element->get_element_name();

		// �������X�g�̏ꍇ�̓r�b�g�\��
		if (element_form == FM_INTLIST)	{
			switch (bit)	{
				case 16:	element_name += _T(".bit16");	break;
				case 8:		element_name += _T(".bit8");	break;
				case 4:		element_name += _T(".bit4");	break;
				case 2:		element_name += _T(".bit2");	break;
				case 1:		element_name += _T(".bit1");	break;
			}
		}
	}
	p_lv->set_cell_text(*p_item_no, *p_column_no, element_name);	(*p_inc_param) ++;

	// ���X�g�̏ꍇ�̓T�C�Y��\��
	if (tnm_form_is_list(element_form))	{
		int list_size = tnm_get_list_size(p_element, element_form, bit);
		p_lv->set_cell_text(*p_item_no, *p_column_no, p_element ? tostr(list_size) : _T(""));	(*p_inc_param) ++;
	}
	else	{

		// �P��G�������g�̏���\��
		tnm_update_lv_simple_element(p_element, element_form, p_lv, p_item_no, p_column_no, p_inc_param);
	}
}

// �e�[�u���\��
void tnm_update_lv_element_table(C_tnm_element* p_element, int element_form, int bit, int top_index, C_tnm_dbg_listview* p_lv, int* p_item_no)
{
	// �K�؂ȃe���v���[�g�֐����Ă�
	switch (element_form)	{
		case FM_INTLIST:			tnm_update_lv_list_table((C_elm_int_list *)p_element, bit, top_index, p_lv, p_item_no, true);	break;
		case FM_INTEVENTLIST:		tnm_update_lv_list_table((C_elm_int_event_list *)p_element, p_lv, p_item_no, true);				break;
		case FM_STRLIST:			tnm_update_lv_list_table((C_elm_str_list *)p_element, p_lv, p_item_no, true);					break;
		case FM_COUNTERLIST:		tnm_update_lv_list_table((C_elm_counter_list *)p_element, p_lv, p_item_no, true);				break;
		case FM_MASKLIST:			tnm_update_lv_list_table((C_elm_mask_list *)p_element, p_lv, p_item_no, true);					break;
		case FM_OBJECTLIST:			tnm_update_lv_list_table((C_elm_object_list *)p_element, p_lv, p_item_no, true);				break;
		case FM_GROUPLIST:			tnm_update_lv_list_table((C_elm_group_list *)p_element, p_lv, p_item_no, true);					break;
		case FM_MWNDLIST:			tnm_update_lv_list_table((C_elm_mwnd_list *)p_element, p_lv, p_item_no, true);					break;
		case FM_EFFECTLIST:			tnm_update_lv_list_table((C_elm_effect_list *)p_element, p_lv, p_item_no, true);				break;
		case FM_QUAKELIST:			tnm_update_lv_list_table((C_elm_quake_list *)p_element, p_lv, p_item_no, true);					break;
		case FM_WORLDLIST:			tnm_update_lv_list_table((C_elm_world_list *)p_element, p_lv, p_item_no, true);					break;
		case FM_PCMCHLIST:			tnm_update_lv_list_table((C_elm_pcmch_list *)p_element, p_lv, p_item_no, true);					break;
		case FM_PCMEVENTLIST:		tnm_update_lv_list_table((C_elm_pcm_event_list *)p_element, p_lv, p_item_no, true);				break;
		case FM_FRAMEACTIONLIST:	tnm_update_lv_list_table((C_elm_frame_action_list*)p_element, p_lv, p_item_no, true);			break;
		case FM_CALLLIST:			tnm_update_lv_list_table((C_elm_call_list *)p_element, p_lv, p_item_no, true);					break;
	}
}


// ****************************************************************
// �G�������g�̒l���X�V����
// ================================================================

// �J�E���^
void tnm_update_value_counter(int param_no, CTSTR& value, C_elm_counter* p_counter)
{
	switch (param_no)	{
		case 0:		p_counter->set_count(str_to_int(value, 0));				break;
	}
}

// �}�X�N
void tnm_update_value_mask(int param_no, CTSTR& value, C_elm_mask* p_mask)
{
	switch (param_no)	{
	case 1:		p_mask->set_x(str_to_int(value, 0));				break;
	case 2:		p_mask->set_y(str_to_int(value, 0));				break;
	}
}

// �I�u�W�F�N�g
void tnm_update_value_object(int param_no, CTSTR& value, C_elm_object* p_object)
{
	switch (param_no)	{
		case 3:		p_object->set_disp(str_to_int(value, 0));				break;
		case 4:		p_object->set_pat_no(str_to_int(value, 0));				break;
		case 5:		p_object->set_order(str_to_int(value, 0));				break;
		case 6:		p_object->set_layer(str_to_int(value, 0));				break;
		case 7:		p_object->set_world_no(str_to_int(value, 0));			break;
		case 8:		p_object->set_pos_x(str_to_int(value, 0));				break;
		case 9:		p_object->set_pos_y(str_to_int(value, 0));				break;
		case 10:	p_object->set_pos_z(str_to_int(value, 0));				break;
		case 11:	p_object->set_center_x(str_to_int(value, 0));			break;
		case 12:	p_object->set_center_y(str_to_int(value, 0));			break;
		case 13:	p_object->set_center_z(str_to_int(value, 0));			break;
		case 14:	p_object->set_center_rep_x(str_to_int(value, 0));		break;
		case 15:	p_object->set_center_rep_y(str_to_int(value, 0));		break;
		case 16:	p_object->set_center_rep_z(str_to_int(value, 0));		break;
		case 17:	p_object->set_scale_x(str_to_int(value, 0));			break;
		case 18:	p_object->set_scale_y(str_to_int(value, 0));			break;
		case 19:	p_object->set_scale_z(str_to_int(value, 0));			break;
		case 20:	p_object->set_rotate_x(str_to_int(value, 0));			break;
		case 21:	p_object->set_rotate_y(str_to_int(value, 0));			break;
		case 22:	p_object->set_rotate_z(str_to_int(value, 0));			break;
		case 23:	p_object->set_clip_use(str_to_int(value, 0));			break;
		case 24:	p_object->set_clip_left(str_to_int(value, 0));			break;
		case 25:	p_object->set_clip_top(str_to_int(value, 0));			break;
		case 26:	p_object->set_clip_right(str_to_int(value, 0));			break;
		case 27:	p_object->set_clip_bottom(str_to_int(value, 0));		break;
		case 28:	p_object->set_src_clip_use(str_to_int(value, 0));		break;
		case 29:	p_object->set_src_clip_left(str_to_int(value, 0));		break;
		case 30:	p_object->set_src_clip_top(str_to_int(value, 0));		break;
		case 31:	p_object->set_src_clip_right(str_to_int(value, 0));		break;
		case 32:	p_object->set_src_clip_bottom(str_to_int(value, 0));	break;

		case 33:	p_object->set_tr(str_to_int(value, 0));					break;
		case 34:	p_object->set_mono(str_to_int(value, 0));				break;
		case 35:	p_object->set_reverse(str_to_int(value, 0));			break;
		case 36:	p_object->set_bright(str_to_int(value, 0));				break;
		case 37:	p_object->set_dark(str_to_int(value, 0));				break;
		case 38:	p_object->set_color_r(str_to_int(value, 0));			break;
		case 39:	p_object->set_color_g(str_to_int(value, 0));			break;
		case 40:	p_object->set_color_b(str_to_int(value, 0));			break;
		case 41:	p_object->set_color_rate(str_to_int(value, 0));			break;
		case 42:	p_object->set_color_add_r(str_to_int(value, 0));		break;
		case 43:	p_object->set_color_add_g(str_to_int(value, 0));		break;
		case 44:	p_object->set_color_add_b(str_to_int(value, 0));		break;
		case 45:	p_object->set_tone_curve_no(str_to_int(value, 0));		break;
		case 46:	p_object->set_culling(str_to_int(value, 0));			break;
		case 47:	p_object->set_alpha_test(str_to_int(value, 0));			break;
		case 48:	p_object->set_alpha_blend(str_to_int(value, 0));		break;

		case 49:	p_object->set_blend_type(str_to_int(value, 0));			break;
		case 50:	p_object->set_button_no(str_to_int(value, 0));			break;
		case 51:	p_object->set_button_group_no(str_to_int(value, 0));	break;
		case 52:	p_object->set_button_action_no(str_to_int(value, 0));	break;
		case 53:	p_object->set_button_se_no(str_to_int(value, 0));		break;
		case 54:	p_object->set_button_state(str_to_int(value, 0));		break;

		case 62:	p_object->set_wipe_copy(str_to_int(value, 0));			break;
		case 63:	p_object->set_wipe_erase(str_to_int(value, 0));			break;
		case 64:	p_object->set_hints(str_to_int(value, 0));				break;
		case 65:	p_object->set_child_sort_type(str_to_int(value, 0));	break;
	}
}

// �G�t�F�N�g
void tnm_update_value_effect(int param_no, CTSTR& value, C_elm_effect* p_effect)
{
	switch (param_no)	{
		case 0:		p_effect->set_pos_x(str_to_int(value, 0));				break;
		case 1:		p_effect->set_pos_y(str_to_int(value, 0));				break;
		case 2:		p_effect->set_pos_z(str_to_int(value, 0));				break;
		case 3:		p_effect->set_mono(str_to_int(value, 0));				break;
		case 4:		p_effect->set_reverse(str_to_int(value, 0));			break;
		case 5:		p_effect->set_bright(str_to_int(value, 0));				break;
		case 6:		p_effect->set_dark(str_to_int(value, 0));				break;
		case 7:		p_effect->set_color_r(str_to_int(value, 0));			break;
		case 8:		p_effect->set_color_g(str_to_int(value, 0));			break;
		case 9:		p_effect->set_color_b(str_to_int(value, 0));			break;
		case 10:	p_effect->set_color_rate(str_to_int(value, 0));			break;
		case 11:	p_effect->set_color_add_r(str_to_int(value, 0));		break;
		case 12:	p_effect->set_color_add_g(str_to_int(value, 0));		break;
		case 13:	p_effect->set_color_add_b(str_to_int(value, 0));		break;
		case 14:	p_effect->set_begin_order(str_to_int(value, 0));		break;
		case 15:	p_effect->set_begin_layer(str_to_int(value, 0));		break;
		case 16:	p_effect->set_end_order(str_to_int(value, 0));			break;
		case 17:	p_effect->set_end_layer(str_to_int(value, 0));			break;
		case 18:	p_effect->set_wipe_copy(str_to_int(value, 0));			break;
		case 19:	p_effect->set_wipe_erase(str_to_int(value, 0));			break;
	}
}

// ���[���h
void tnm_update_value_world(int param_no, CTSTR& value, C_elm_world* p_world)
{
	switch (param_no)	{
		case 0:		p_world->set_camera_eye_x(str_to_int(value, 0));		break;
		case 1:		p_world->set_camera_eye_y(str_to_int(value, 0));		break;
		case 2:		p_world->set_camera_eye_z(str_to_int(value, 0));		break;
		case 3:		p_world->set_camera_pint_x(str_to_int(value, 0));		break;
		case 4:		p_world->set_camera_pint_y(str_to_int(value, 0));		break;
		case 5:		p_world->set_camera_pint_z(str_to_int(value, 0));		break;
		case 6:		p_world->set_camera_up_x(str_to_int(value, 0));			break;
		case 7:		p_world->set_camera_up_y(str_to_int(value, 0));			break;
		case 8:		p_world->set_camera_up_z(str_to_int(value, 0));			break;
		case 9:		p_world->set_camera_view_angle(str_to_int(value, 0));	break;
	}
}

// ****************************************************************
// ���X�g�r���[���_�u���N���b�N�����F�ėp�֐�
// ================================================================

// �G�������g
void tnm_on_dblclk_lv_element(C_tnm_dbg_listview* p_lv, int item_no, int column_no, int element_form, int bit, bool table_flag)
{
	// �e�[�u���\��
	if (table_flag)	{
		switch (element_form)	{
			case FM_INTLIST:		tnm_on_dblclk_lv_int_list_table(p_lv, item_no, column_no, bit);		break;
			case FM_INTEVENTLIST:	tnm_on_dblclk_lv_int_event_list_table(p_lv, item_no, column_no);	break;
			case FM_STRLIST:		tnm_on_dblclk_lv_str_list_table(p_lv, item_no, column_no);			break;
			case FM_COUNTERLIST:	tnm_on_dblclk_lv_counter_list_table(p_lv, item_no, column_no);		break;
			case FM_MASKLIST:		tnm_on_dblclk_lv_mask_list_table(p_lv, item_no, column_no);			break;
			case FM_OBJECTLIST:		tnm_on_dblclk_lv_object_list_table(p_lv, item_no, column_no);		break;
			case FM_EFFECTLIST:		tnm_on_dblclk_lv_effect_list_table(p_lv, item_no, column_no);		break;
			case FM_WORLDLIST:		tnm_on_dblclk_lv_world_list_table(p_lv, item_no, column_no);		break;
		}
	}

	// �ʏ�\��
	else	{
		switch (element_form)	{
			case FM_COUNTER:		tnm_on_dblclk_lv_counter(p_lv, item_no, column_no);			break;
			case FM_MASK:			tnm_on_dblclk_lv_mask(p_lv, item_no, column_no);			break;
			case FM_OBJECT:			tnm_on_dblclk_lv_object(p_lv, item_no, column_no);			break;
			case FM_EFFECT:			tnm_on_dblclk_lv_effect(p_lv, item_no, column_no);			break;
			case FM_WORLD:			tnm_on_dblclk_lv_world(p_lv, item_no, column_no);			break;
		}
	}
}

// �������X�g�i�e�[�u���\���j
void tnm_on_dblclk_lv_int_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, int bit)
{
	if (column_no == 1)	{
		int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
		switch (bit)	{
			case 1:		p_lv->edit_item_text_edit_for_num(item_no, column_no, 0, 1, value);				break;
			case 2:		p_lv->edit_item_text_edit_for_num(item_no, column_no, 0, 3, value);				break;
			case 4:		p_lv->edit_item_text_edit_for_num(item_no, column_no, 0, 15, value);			break;
			case 8:		p_lv->edit_item_text_edit_for_num(item_no, column_no, 0, 255, value);			break;
			case 16:	p_lv->edit_item_text_edit_for_num(item_no, column_no, 0, 65535, value);			break;
			case 32:	p_lv->edit_item_text_edit_for_num(item_no, column_no, INT_MIN, INT_MAX, value);	break;
		}
	}
}

// �����C�x���g���X�g�i�e�[�u���\���j
void tnm_on_dblclk_lv_int_event_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	if (column_no == 1)	{
		int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
		p_lv->edit_item_text_edit_for_num(item_no, column_no, INT_MIN, INT_MAX, value);
	}
}

// �����񃊃X�g�i�e�[�u���\���j
void tnm_on_dblclk_lv_str_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	if (column_no == 1)	{
		TSTR value = p_lv->get_cell_text(item_no, column_no);
		p_lv->edit_item_text_edit_with_str(item_no, column_no, value);
	}
}

// �J�E���^
void tnm_on_dblclk_lv_counter(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	if (column_no == 1)	{
		int param_no = item_no - 1;
		if (0 <= param_no && param_no < sizeof_array(G_counter_edit_param))	{
			if (G_counter_edit_param[param_no].enable)	{
				int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
				p_lv->edit_item_text_edit_for_num(item_no, column_no, G_counter_edit_param[param_no].min, G_counter_edit_param[param_no].max, value);
			}
		}
	}
}

// �J�E���^���X�g�i�e�[�u���\���j
void tnm_on_dblclk_lv_counter_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	int param_no = column_no - 1;
	if (0 <= param_no && param_no < sizeof_array(G_counter_edit_param))	{
		if (G_counter_edit_param[param_no].enable)	{
			int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
			p_lv->edit_item_text_edit_for_num(item_no, column_no, G_counter_edit_param[param_no].min, G_counter_edit_param[param_no].max, value);
		}
	}
}

// �}�X�N
void tnm_on_dblclk_lv_mask(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	if (column_no == 1)	{
		int param_no = item_no - 1;
		if (0 <= param_no && param_no < sizeof_array(G_mask_edit_param))	{
			if (G_mask_edit_param[param_no].enable)	{
				int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
				p_lv->edit_item_text_edit_for_num(item_no, column_no, G_mask_edit_param[param_no].min, G_mask_edit_param[param_no].max, value);
			}
		}
	}
}

// �}�X�N���X�g�i�e�[�u���\���j
void tnm_on_dblclk_lv_mask_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	int param_no = column_no - 1;
	if (0 <= param_no && param_no < sizeof_array(G_mask_edit_param))	{
		if (G_mask_edit_param[param_no].enable)	{
			int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
			p_lv->edit_item_text_edit_for_num(item_no, column_no, G_mask_edit_param[param_no].min, G_mask_edit_param[param_no].max, value);
		}
	}
}

// �I�u�W�F�N�g
void tnm_on_dblclk_lv_object(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	if (column_no == 1)	{
		int param_no = item_no - 1;
		if (0 <= param_no && param_no < sizeof_array(G_object_edit_param))	{
			if (G_object_edit_param[param_no].enable)	{
				int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
				p_lv->edit_item_text_edit_for_num(item_no, column_no, G_object_edit_param[param_no].min, G_object_edit_param[param_no].max, value);
			}
		}
	}
}

// �I�u�W�F�N�g���X�g�i�e�[�u���\���j
void tnm_on_dblclk_lv_object_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	int param_no = column_no - 1;
	if (0 <= param_no && param_no < sizeof_array(G_object_edit_param))	{
		if (G_object_edit_param[param_no].enable)	{
			int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
			p_lv->edit_item_text_edit_for_num(item_no, column_no, G_object_edit_param[param_no].min, G_object_edit_param[param_no].max, value);
		}
	}
}

// �G�t�F�N�g
void tnm_on_dblclk_lv_effect(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	if (column_no == 1)	{
		int param_no = item_no - 1;
		if (0 <= param_no && param_no < sizeof_array(G_effect_edit_param))	{
			if (G_effect_edit_param[param_no].enable)	{
				int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
				p_lv->edit_item_text_edit_for_num(item_no, column_no, G_effect_edit_param[param_no].min, G_effect_edit_param[param_no].max, value);
			}
		}
	}
}

// �G�t�F�N�g���X�g�i�e�[�u���\���j
void tnm_on_dblclk_lv_effect_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	int param_no = column_no - 1;
	if (0 <= param_no && param_no < sizeof_array(G_effect_edit_param))	{
		if (G_effect_edit_param[param_no].enable)	{
			int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
			p_lv->edit_item_text_edit_for_num(item_no, column_no, G_effect_edit_param[param_no].min, G_effect_edit_param[param_no].max, value);
		}
	}
}

// ���[���h
void tnm_on_dblclk_lv_world(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	if (column_no == 1)	{
		int param_no = item_no - 1;
		if (0 <= param_no && param_no < sizeof_array(G_world_edit_param))	{
			if (G_world_edit_param[param_no].enable)	{
				int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
				p_lv->edit_item_text_edit_for_num(item_no, column_no, G_world_edit_param[param_no].min, G_world_edit_param[param_no].max, value);
			}
		}
	}
}

// ���[���h���X�g�i�e�[�u���\���j
void tnm_on_dblclk_lv_world_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no)
{
	int param_no = column_no - 1;
	if (0 <= param_no && param_no < sizeof_array(G_world_edit_param))	{
		if (G_world_edit_param[param_no].enable)	{
			int value = str_to_int(p_lv->get_cell_text(item_no, column_no), 0);
			p_lv->edit_item_text_edit_for_num(item_no, column_no, G_world_edit_param[param_no].min, G_world_edit_param[param_no].max, value);
		}
	}
}

// ****************************************************************
// ���X�g�r���[�̒l��ύX�����F�ėp�֐�
// ================================================================

// �G�������g
void tnm_on_change_lv_value_element(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_tnm_element* p_element, int element_form, int bit, int top_index, bool table_flag)
{
	// �e�[�u���\��
	if (table_flag)	{
		switch (element_form)	{
			case FM_INTLIST:		tnm_on_change_lv_value_int_list_table(p_lv, item_no, column_no, value, (C_elm_int_list *)p_element, bit, top_index);	break;
			case FM_INTEVENTLIST:	tnm_on_change_lv_value_int_event_list_table(p_lv, item_no, column_no, value, (C_elm_int_event_list *)p_element);		break;
			case FM_STRLIST:		tnm_on_change_lv_value_str_list_table(p_lv, item_no, column_no, value, (C_elm_str_list *)p_element);					break;
			case FM_COUNTERLIST:	tnm_on_change_lv_value_counter_list_table(p_lv, item_no, column_no, value, (C_elm_counter_list *)p_element);			break;
			case FM_MASKLIST:		tnm_on_change_lv_value_mask_list_table(p_lv, item_no, column_no, value, (C_elm_mask_list *)p_element);					break;
			case FM_OBJECTLIST:		tnm_on_change_lv_value_object_list_table(p_lv, item_no, column_no, value, (C_elm_object_list *)p_element);				break;
			case FM_EFFECTLIST:		tnm_on_change_lv_value_effect_list_table(p_lv, item_no, column_no, value, (C_elm_effect_list *)p_element);				break;
			case FM_WORLDLIST:		tnm_on_change_lv_value_world_list_table(p_lv, item_no, column_no, value, (C_elm_world_list *)p_element);				break;
		}
	}

	// �ʏ�\��
	else	{
		switch (element_form)	{
			case FM_COUNTER:		tnm_on_change_lv_value_counter(p_lv, item_no, column_no, value, (C_elm_counter *)p_element);							break;
			case FM_MASK:			tnm_on_change_lv_value_mask(p_lv, item_no, column_no, value, (C_elm_mask*)p_element);									break;
			case FM_OBJECT:			tnm_on_change_lv_value_object(p_lv, item_no, column_no, value, (C_elm_object *)p_element);								break;
			case FM_EFFECT:			tnm_on_change_lv_value_effect(p_lv, item_no, column_no, value, (C_elm_effect *)p_element);								break;
			case FM_WORLD:			tnm_on_change_lv_value_world(p_lv, item_no, column_no, value, (C_elm_world *)p_element);								break;
		}
	}
}

// �������X�g
void tnm_on_change_lv_value_int_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_int_list* p_int_list, int bit, int top_index)
{
	if (column_no == 1)	{
		switch (bit)	{
			case 1:		p_int_list->set_bit_1(top_index + item_no, str_to_int(value, 0));	break;
			case 2:		p_int_list->set_bit_2(top_index + item_no, str_to_int(value, 0));	break;
			case 4:		p_int_list->set_bit_4(top_index + item_no, str_to_int(value, 0));	break;
			case 8:		p_int_list->set_bit_8(top_index + item_no, str_to_int(value, 0));	break;
			case 16:	p_int_list->set_bit_16(top_index + item_no, str_to_int(value, 0));	break;
			case 32:	p_int_list->set_value(top_index + item_no, str_to_int(value, 0));	break;
		}
	}
}

// �����C�x���g���X�g
void tnm_on_change_lv_value_int_event_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_int_event_list* p_int_event_list)
{
	if (column_no == 1)	{
		(*p_int_event_list)[item_no].set_value(str_to_int(value, 0));
	}
}

// �����񃊃X�g
void tnm_on_change_lv_value_str_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_str_list* p_str_list)
{
	if (column_no == 1)	{
		p_str_list->set_value(item_no, value);
	}
}

// �J�E���^
void tnm_on_change_lv_value_counter(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_counter* p_counter)
{
	if (column_no == 1)	{
		tnm_update_value_counter(item_no - 1, value, p_counter);
	}
}

// �J�E���^���X�g
void tnm_on_change_lv_value_counter_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_counter_list* p_counter_list)
{
	int counter_no = str_to_int(p_lv->get_cell_text(item_no, 0), -1);
	if (counter_no >= 0)	{
		C_elm_counter* p_counter = &(*p_counter_list)[counter_no];
		tnm_update_value_counter(column_no - 1, value, p_counter);
	}
}

// �}�X�N
void tnm_on_change_lv_value_mask(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_mask* p_mask)
{
	if (column_no == 1)	{
		tnm_update_value_mask(item_no - 1, value, p_mask);
	}
}

// �}�X�N���X�g
void tnm_on_change_lv_value_mask_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_mask_list* p_mask_list)
{
	int mask_no = str_to_int(p_lv->get_cell_text(item_no, 0), -1);
	if (mask_no >= 0)	{
		C_elm_mask* p_mask = &(*p_mask_list)[mask_no];
		tnm_update_value_mask(column_no - 1, value, p_mask);
	}
}

// �I�u�W�F�N�g
void tnm_on_change_lv_value_object(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_object* p_object)
{
	if (column_no == 1)	{
		tnm_update_value_object(item_no - 1, value, p_object);
	}
}

// �I�u�W�F�N�g���X�g
void tnm_on_change_lv_value_object_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_object_list* p_object_list)
{
	int object_no = str_to_int(p_lv->get_cell_text(item_no, 0), -1);
	if (object_no >= 0)	{
		C_elm_object* p_object = &(*p_object_list)[object_no];
		tnm_update_value_object(column_no - 1, value, p_object);
	}
}

// �G�t�F�N�g
void tnm_on_change_lv_value_effect(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_effect* p_effect)
{
	if (column_no == 1)	{
		tnm_update_value_effect(item_no - 1, value, p_effect);
	}
}

// �G�t�F�N�g���X�g
void tnm_on_change_lv_value_effect_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_effect_list* p_effect_list)
{
	int object_no = str_to_int(p_lv->get_cell_text(item_no, 0), -1);
	if (object_no >= 0)	{
		C_elm_effect* p_effect = &(*p_effect_list)[object_no];
		tnm_update_value_effect(column_no - 1, value, p_effect);
	}
}

// ���[���h
void tnm_on_change_lv_value_world(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_world* p_world)
{
	if (column_no == 1)	{
		tnm_update_value_world(item_no - 1, value, p_world);
	}
}

// ���[���h���X�g
void tnm_on_change_lv_value_world_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_world_list* p_world_list)
{
	int world_no = str_to_int(p_lv->get_cell_text(item_no, 0), -1);
	if (world_no >= 0)	{
		C_elm_world* p_world = &(*p_world_list)[world_no];
		tnm_update_value_world(column_no - 1, value, p_world);
	}
}

// ****************************************************************
// ���X�g�r���[�̃A�C�e�������팸����
// ================================================================
void tnm_reduce_item_cnt(C_tnm_dbg_listview* p_lv, int item_no)
{
	if (p_lv->get_item_cnt() > item_no)
		p_lv->set_item_cnt(item_no);
}
