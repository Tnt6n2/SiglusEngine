#include	"g00_pch.h"
#include	"jpeg.h"

#define		_PIXEL_SIZE					sizeof(DWORD)

// ****************************************************************
// ‚i‚o‚d‚f‰ğ“€
// ----------------------------------------------------------------

const BYTE Gv_jpeg_angou_table[256] = {
	0x45, 0x0c, 0x85, 0xc0, 0x75, 0x14, 0xe5, 0x5d, 0x8b, 0x55, 0xec, 0xc0, 0x5b, 0x8b, 0xc3, 0x8b,
	0x81, 0xff, 0x00, 0x00, 0x04, 0x00, 0x85, 0xff, 0x6a, 0x00, 0x76, 0xb0, 0x43, 0x00, 0x76, 0x49,
	0x00, 0x8b, 0x7d, 0xe8, 0x8b, 0x75, 0xa1, 0xe0, 0x0c, 0x85, 0xc0, 0xc0, 0x75, 0x78, 0x30, 0x44,
	0x00, 0x85, 0xff, 0x76, 0x37, 0x81, 0x1d, 0xd0, 0xff, 0x00, 0x00, 0x75, 0x44, 0x8b, 0xb0, 0x43,
	0x45, 0xf8, 0x8d, 0x55, 0xfc, 0x52, 0x00, 0x76, 0x68, 0x00, 0x00, 0x04, 0x00, 0x6a, 0x43, 0x8b,
	0xb1, 0x43, 0x00, 0x6a, 0x05, 0xff, 0x50, 0xff, 0xd3, 0xa1, 0xe0, 0x04, 0x00, 0x56, 0x15, 0x2c,
	0x44, 0x00, 0x85, 0xc0, 0x74, 0x09, 0xc3, 0xa1, 0x5f, 0x5e, 0x33, 0x8b, 0xe5, 0x5d, 0xe0, 0x30,
	0x04, 0x00, 0x81, 0xc6, 0x00, 0x00, 0x81, 0xef, 0x04, 0x00, 0x85, 0x30, 0x44, 0x00, 0x00, 0x00,
	0x5d, 0xc3, 0x8b, 0x55, 0xf8, 0x8d, 0x5e, 0x5b, 0x4d, 0xfc, 0x51, 0xc4, 0x04, 0x5f, 0x8b, 0xe5,
	0x43, 0x00, 0xeb, 0xd8, 0x8b, 0x45, 0xff, 0x15, 0xe8, 0x83, 0xc0, 0x57, 0x56, 0x52, 0x2c, 0xb1,
	0x01, 0x00, 0x8b, 0x7d, 0xe8, 0x89, 0x00, 0xe8, 0x45, 0xf4, 0x8b, 0x20, 0x50, 0x6a, 0x47, 0x28,
	0x00, 0x50, 0x53, 0xff, 0x15, 0x34, 0xe4, 0x6a, 0xb1, 0x43, 0x00, 0x0c, 0x8b, 0x45, 0x00, 0x6a,
	0x8b, 0x4d, 0xec, 0x89, 0x08, 0x8a, 0x85, 0xc0, 0x45, 0xf0, 0x84, 0x8b, 0x45, 0x10, 0x74, 0x05,
	0xf5, 0x28, 0x01, 0x00, 0x83, 0xc4, 0x52, 0x6a, 0x08, 0x89, 0x45, 0x83, 0xc2, 0x20, 0x00, 0xe8,
	0xe8, 0xf4, 0xfb, 0xff, 0xff, 0x8b, 0x8b, 0x5d, 0x45, 0x0c, 0x83, 0xc0, 0x74, 0xc5, 0xf8, 0x53,
	0xc4, 0x08, 0x85, 0xc0, 0x75, 0x56, 0x30, 0x44, 0x8b, 0x1d, 0xd0, 0xf0, 0xa1, 0xe0, 0x00, 0x83,
};

// ****************************************************************
// ‚i‚o‚d‚f‰ğ“€ƒNƒ‰ƒX
// ----------------------------------------------------------------

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Cva_jpeg::Cva_jpeg()
{
	f_init();
}

// ƒfƒXƒgƒ‰ƒNƒ^
Cva_jpeg::~Cva_jpeg()
{
	f_free();
}

// ‰Šú‰»
void Cva_jpeg::f_init()
{
	v_ready_flag = false;
	v_destroy_flag = false;
	v_complate_flag = false;

	v_now_expand_line_cnt = 0;
	v_nokori_expand_line_cnt = 0;
	v_once_expand_line_cnt = 0;

	v_jpeg_data_p = NULL;
	v_jpeg_data_size = 0;

	v_bitmap_buf_p = NULL;
	v_bitmap_xl = 0;
	v_bitmap_yl = 0;
}

// ‰ğ•ú
void Cva_jpeg::f_free()
{
	if (v_ready_flag)	{
		if (v_destroy_flag)	{
			jpeg_destroy_decompress(&v_jpeg_cinfo);
		}
	}

	f_init();
}

// €”õ
bool Cva_jpeg::f_expand_ready(BYTE *jpeg_data_p, int jpeg_data_size, std::vector<BYTE> *ret_bitmap_buf_p, int *ret_bitmap_xl_p, int *ret_bitmap_yl_p, int once_expand_pixel_cnt)
{
	if (ret_bitmap_buf_p)	{
		ret_bitmap_buf_p->clear();
	}
	if (ret_bitmap_xl_p)	{
		*ret_bitmap_xl_p = 0;
	}
	if (ret_bitmap_yl_p)	{
		*ret_bitmap_yl_p = 0;
	}

	if (jpeg_data_p == NULL || jpeg_data_size <= 0 || ret_bitmap_buf_p == NULL)	{ return false; }

	v_jpeg_cinfo.err = jpeg_std_error(&v_jpeg_err.pub);
	v_jpeg_err.pub.error_exit = jpeg_my_error_exit;
	if (setjmp(v_jpeg_err.setjmp_buffer)) {
		jpeg_destroy_decompress(&v_jpeg_cinfo);
		return false;
	}

	// ŠÈˆÕˆÃ†‚Ì‰ğœi‚w‚n‚q‰‰Zj
	BYTE *sp = jpeg_data_p;
	for (int i = 0; i < jpeg_data_size; i++){
		*sp++ ^= Gv_jpeg_angou_table[i % 256];
	}

	jpeg_create_decompress(&v_jpeg_cinfo);
	jpeg_stdio_src(&v_jpeg_cinfo, jpeg_data_p, jpeg_data_size);
	(void)jpeg_read_header(&v_jpeg_cinfo, TRUE);
	(void)jpeg_start_decompress(&v_jpeg_cinfo);
	v_jpeg_row_stride = v_jpeg_cinfo.output_width * v_jpeg_cinfo.output_components;
	v_jpeg_out_row_buffer = (*v_jpeg_cinfo.mem->alloc_sarray)((j_common_ptr)&v_jpeg_cinfo, JPOOL_IMAGE, v_jpeg_row_stride, 1);

	v_now_expand_line_cnt = 0;
	v_nokori_expand_line_cnt = v_jpeg_cinfo.output_height;
	if (once_expand_pixel_cnt <= 0)	{		// ‚OˆÈ‰º‚Ì‚Í‚P‰ñ‚ÌŒÄ‚Ño‚µ‚Å‘S‰ğ“€‚µ‚Ü‚·
		v_once_expand_line_cnt = 0;			// ‘S‰ğ“€
	}
	else	{
		v_once_expand_line_cnt = once_expand_pixel_cnt / v_jpeg_cinfo.output_width;
		if (v_once_expand_line_cnt <= 0)	{
			v_once_expand_line_cnt = 1;		// Å’á‚Å‚à‚Pƒ‰ƒCƒ“
		}
	}

	v_jpeg_data_p = jpeg_data_p;
	v_jpeg_data_size = jpeg_data_size;

	v_bitmap_xl = v_jpeg_cinfo.output_width;
	v_bitmap_yl = v_jpeg_cinfo.output_height;
	if (ret_bitmap_xl_p)	{
		*ret_bitmap_xl_p = v_bitmap_xl;
	}
	if (ret_bitmap_yl_p)	{
		*ret_bitmap_yl_p = v_bitmap_yl;
	}

	v_bitmap_buf_p = ret_bitmap_buf_p;
	v_bitmap_buf_p->resize(v_bitmap_xl * v_bitmap_yl * _PIXEL_SIZE);
	v_dst_p = (BYTE *)&(*v_bitmap_buf_p)[0];

	v_ready_flag = true;
	v_destroy_flag = true;
	v_complate_flag = false;

	return true;
}

// ‰ğ“€
bool Cva_jpeg::f_expand_proc()
{
	// ‰ğ“€Š®—¹
	if (v_complate_flag)	{
		return true;
	}

	int expand_line_cnt;
	if (v_once_expand_line_cnt <= 0)	{
		expand_line_cnt = v_jpeg_cinfo.output_height;
	}
	else	{
		if (v_nokori_expand_line_cnt < v_once_expand_line_cnt)	{
			expand_line_cnt = v_nokori_expand_line_cnt;
		}
		else	{
			expand_line_cnt = v_once_expand_line_cnt;
		}
	}

	BYTE *sp;
	for (int i = 0; i < expand_line_cnt; i++)	{

		(void)jpeg_read_scanlines(&v_jpeg_cinfo, v_jpeg_out_row_buffer, 1);

		sp = (BYTE *)v_jpeg_out_row_buffer[0];
		for (int j = 0; j < v_bitmap_xl; j++)	{
			*(v_dst_p + 0) = *(sp + 2);
			*(v_dst_p + 1) = *(sp + 1);
			*(v_dst_p + 2) = *(sp + 0);
			*(v_dst_p + 3) = 255;
			v_dst_p += 4;
			sp += 3;
		}
	}

	v_now_expand_line_cnt += expand_line_cnt;
	v_nokori_expand_line_cnt -= expand_line_cnt;

	if (v_nokori_expand_line_cnt <= 0)	{

		v_destroy_flag = false;
		v_complate_flag = true;

		(void)jpeg_finish_decompress(&v_jpeg_cinfo);
		jpeg_destroy_decompress(&v_jpeg_cinfo);

		return true;
	}

	return false;
}
