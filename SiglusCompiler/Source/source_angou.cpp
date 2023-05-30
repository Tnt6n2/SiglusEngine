#include	"pch.h"

#include	"lzss.h"
#include	"source_angou.h"

BYTE easy_angou_code[EASY_ANGOU_CODE_SIZE] = EASY_ANGOU_CODE;
BYTE mask_angou_code[MASK_ANGOU_CODE_SIZE] = MASK_ANGOU_CODE;
BYTE gomi_angou_code[GOMI_ANGOU_CODE_SIZE] = GOMI_ANGOU_CODE;
BYTE last_angou_code[LAST_ANGOU_CODE_SIZE] = LAST_ANGOU_CODE;
BYTE name_angou_code[NAME_ANGOU_CODE_SIZE] = NAME_ANGOU_CODE;

// ****************************************************************
// �Í���
// ================================================================
void C_tnms_source_angou::encryption(BYTE *buf, DWORD buf_size, CTSTR& buf_name, BUFFER& dst_buf)
{
	dst_buf.clear();

	BYTE *wp;
	int ind, md5_ind, cnt;

	Stns_source_angou_header header = {0};
	header.version = 1;

	// ���k
	BUFFER lzss_buf;
	lzss_pack_easy(buf, buf_size, lzss_buf);
	header.lzss_data_size = (int)lzss_buf.size();

	// �ȈՈÍ��P
	wp = lzss_buf.get();
	ind = EASY_ANGOU_INDEX;
	for (int i = 0; i < header.lzss_data_size; i++)	{
		*wp++ ^= easy_angou_code[ind++];
		ind %= EASY_ANGOU_CODE_SIZE;
	}

	// MD5
	MD5_STRUCT md5st;
	MD5init(&md5st, header.lzss_data_size);
	MD5funk(&md5st, lzss_buf.get(), header.lzss_data_size);	// ret : 0=���� 1=�I�� 2=�d�q�q
	CopyMemory(header.md5_code, md5st.code, MD5_CODE_CNT);

	// ���O�Í�
	BUFFER name_buf;
	name_buf.resize((int)buf_name.size() * 2);
	CopyMemory(name_buf.get(), buf_name.c_str(), (int)name_buf.size());
	wp = name_buf.get();
	ind = NAME_ANGOU_INDEX;
	for (int i = 0; i < (int)name_buf.size(); i++)	{
		*wp++ ^= name_angou_code[ind++];
		ind %= NAME_ANGOU_CODE_SIZE;
	}

	// �}�X�N�T�C�Y�Z�o
	int mask_w_size = (header.md5_code[MASK_W_SIZE_MD5_CODE_INDEX] % MASK_W_SIZE_SURPLUS) + MASK_W_SIZE_ADD;
	int mask_h_size = (header.md5_code[MASK_H_SIZE_MD5_CODE_INDEX] % MASK_H_SIZE_SURPLUS) + MASK_H_SIZE_ADD;

	// �}�X�N�쐬
	BUFFER mask_buf;
	mask_buf.resize(mask_w_size * mask_h_size);
	wp = mask_buf.get();
	ind = MASK_ANGOU_INDEX;
	md5_ind = MASK_MD5_INDEX;
	for (int i = 0; i < (int)mask_buf.size(); i++)	{
		*wp++ = (mask_angou_code[ind++] ^ header.md5_code[md5_ind++]);
		ind %= MASK_ANGOU_CODE_SIZE;
		md5_ind %= MD5_CODE_CNT;
	}

	// �}�b�v�T�C�Y�Z�o
	int map_w_size = (header.md5_code[MAP_W_SIZE_MD5_CODE_INDEX] % MAP_W_SIZE_SURPLUS) + MAP_W_SIZE_ADD;
	int byte_half_data_size = (header.lzss_data_size + 1) / 2;
	int dword_half_data_size = (byte_half_data_size + 3) / 4;
	int map_h_size = (dword_half_data_size + (map_w_size - 1)) / map_w_size;
	int map_total_size = map_w_size * map_h_size * 4;

	// �k�y�r�r�o�b�t�@�̒���
	lzss_buf.resize(map_total_size * 2);		// * 2 = �}�b�v�͂Q��
	cnt = (int)lzss_buf.size() - header.lzss_data_size;
	if (cnt > 0)	{
		wp = lzss_buf.get() + header.lzss_data_size;
		ind = GOMI_ANGOU_INDEX;
		md5_ind = GOMI_MD5_INDEX;
		for (int i = 0; i < cnt; i++)	{
			*wp++ = (gomi_angou_code[ind++] ^ header.md5_code[md5_ind++]);
			ind %= GOMI_ANGOU_CODE_SIZE;
			md5_ind %= MD5_CODE_CNT;
		}
	}

	// �o�b�t�@�m��
	dst_buf.resize(sizeof(Stns_source_angou_header) + 4 + (int)name_buf.size() + (map_total_size * 2));		// 4=���O�̒��� *2=�}�b�v�͂Q��

	// �w�b�_�R�s�[
	wp = dst_buf.get();
	CopyMemory(wp, &header, sizeof(Stns_source_angou_header));

	// ���O�̒����R�s�[
	int *name_len_p = (int *)(wp + sizeof(Stns_source_angou_header));
	*name_len_p = (int)name_buf.size();

	// ���O�R�s�[
	BYTE *np = (BYTE *)(name_len_p + 1);
	CopyMemory(np, name_buf.get(), *name_len_p);

	// �^�C���R�s�[
	BYTE *dp1 = np + *name_len_p;
	BYTE *dp2 = dp1 + map_total_size;
	BYTE *sp1 = lzss_buf.get();
	BYTE *sp2 = sp1 + byte_half_data_size;
	tile_copy(dp1, sp1, map_w_size, map_h_size, mask_buf.get(), mask_w_size, mask_h_size, TILE_COPY_T_REP_X, TILE_COPY_T_REP_Y, 0, TILE_COPY_T_LIMIT);
	tile_copy(dp1, sp2, map_w_size, map_h_size, mask_buf.get(), mask_w_size, mask_h_size, TILE_COPY_T_REP_X, TILE_COPY_T_REP_Y, 1, TILE_COPY_T_LIMIT);
	tile_copy(dp2, sp1, map_w_size, map_h_size, mask_buf.get(), mask_w_size, mask_h_size, TILE_COPY_T_REP_X, TILE_COPY_T_REP_Y, 1, TILE_COPY_T_LIMIT);
	tile_copy(dp2, sp2, map_w_size, map_h_size, mask_buf.get(), mask_w_size, mask_h_size, TILE_COPY_T_REP_X, TILE_COPY_T_REP_Y, 0, TILE_COPY_T_LIMIT);

	// �ȈՈÍ��Q
	wp = dst_buf.get();
	ind = LAST_ANGOU_INDEX;
	for (int i = 0; i < (int)dst_buf.size(); i++)	{
		*wp++ ^= last_angou_code[ind++];
		ind %= LAST_ANGOU_CODE_SIZE;
	}
}