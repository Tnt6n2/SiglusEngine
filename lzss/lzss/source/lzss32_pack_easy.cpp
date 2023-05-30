#include	"pch.h"
#include	"lzss_tree.h"
#include	"lzss_tree_find.h"
#include	"lzss32_pack.h"
#include	<vector>

// ****************************************************************
// �ȈՔłk�y�r�r�R�Q���k
// ================================================================

void lzss32_pack_easy(DWORD *src_p, DWORD src_size, std::vector<BYTE>& dst_buf)
{
	dst_buf.clear();
	if (src_size <= 0)	{	return;	}
	dst_buf.resize(src_size * 5);		// �T�{�̃T�C�Y���m�ۂ���
	Clzss32_pack lzss;
	lzss.ready(src_p, src_size , 0);
	while (1)	{
		// ���k�J�n�i�o�͐�o�b�t�@�̃|�C���^�ƃT�C�Y���w��j
		if (lzss.proc(&dst_buf[0], (DWORD)dst_buf.size()))	{
			dst_buf.resize(lzss.pack_size);
			break;	// ���k����
		}
		// �o�͐�o�b�t�@�̍Ċm��
		if (lzss.dst_buf_need_size > 0)	{
			dst_buf.resize((DWORD)dst_buf.size() + src_size * 5);		// ����ɂT�{�̃T�C�Y��ǉ��m�ۂ���
		}
	}
}

void lzss32_pack_easy(std::vector<DWORD>& src_buf, std::vector<BYTE>& dst_buf)
{
	dst_buf.clear();
	if (src_buf.empty())	{	return;	}
	lzss32_pack_easy(&src_buf[0], (DWORD)src_buf.size(), dst_buf);
}


