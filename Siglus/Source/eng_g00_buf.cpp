#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"element/elm_g00_buf.h"
#include	"engine/eng_g00_buf.h"

// ****************************************************************
// �f�O�O�o�b�t�@�F�o�b�t�@�����擾
// ================================================================
int tnm_g00_buf_get_g00_buf_cnt()
{
	return Gp_ini->g00_buf_cnt;
}

// ****************************************************************
// �f�O�O�o�b�t�@�F�t�@�C����ǂݍ���
// ================================================================
void tnm_g00_buf_load_file(C_elm_g00_buf* p_g00_buf, CTSTR& file_name)
{
	p_g00_buf->load_file(file_name);
}

// ****************************************************************
// �f�O�O�o�b�t�@�F�o�b�t�@�����
// ================================================================
void tnm_g00_buf_free_buf(C_elm_g00_buf* p_g00_buf)
{
	p_g00_buf->free();
}

// ****************************************************************
// �f�O�O�o�b�t�@�F�S�Ẵo�b�t�@�����
// ================================================================
void tnm_g00_buf_free_all_buf()
{
	C_elm_g00_buf* p_g00_buf = NULL;
	int g00_buf_cnt = tnm_g00_buf_get_g00_buf_cnt();
	for (int i = 0; i < g00_buf_cnt; i++)	{
		p_g00_buf = Gp_g00_buf_list->get_sub(i, false);
		tnm_g00_buf_free_buf(p_g00_buf);
	}
}


/*
// ****************************************************************
// �f�O�O�o�b�t�@�F�ǂݍ���ł��邩���擾
// ================================================================
bool tnm_g00_buf_is_load(int buf_no)
{
	C_elm_g00_buf* p_g00_buf = Gp_g00_buf_list->get_sub(buf_no, true);
	if (p_g00_buf == NULL)
		return false;

	return p_g00_buf->is_load();
}

// ****************************************************************
// �f�O�O�o�b�t�@�F�t�@�C�������擾
// ================================================================
TSTR tnm_g00_buf_get_file_name(int buf_no)
{
	C_elm_g00_buf* p_g00_buf = Gp_g00_buf_list->get_sub(buf_no, true);
	if (p_g00_buf == NULL)
		return _T("");

	return p_g00_buf->get_file_name();
}
*/


