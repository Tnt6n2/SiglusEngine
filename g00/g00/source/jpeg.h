#pragma once

#include		<setjmp.h>
#include		<jinclude.h>
#include		<jpeglib.h>

//----------------------------------------------------------------------------------------
// 圧縮に必要な定義

struct jpeg_my_destination_mgr
{
	struct jpeg_destination_mgr pub;	/* public fields */

	unsigned char *outmem;				/* 改造（なまはむ）（JOCTET = unsigned char） */
	unsigned int outmem_write_size;		/* 改造（なまはむ）*/

	JOCTET * buffer;					/* start of buffer */
};

//----------------------------------------------------------------------------------------
// 解凍に必要な定義

struct jpeg_my_error_mgr {
	struct jpeg_error_mgr pub;		// "public" fields
	jmp_buf setjmp_buffer;			// for return to caller
};

typedef struct jpeg_my_error_mgr * jpeg_my_error_ptr;

METHODDEF(void) jpeg_my_error_exit(j_common_ptr jpeg_cinfo)
{
	// cinfo->err really points to a my_error_mgr struct, so coerce pointer
	jpeg_my_error_ptr myerr = (jpeg_my_error_ptr)jpeg_cinfo->err;

	// Always display the message.
	// We could postpone this until after returning, if we chose.
	(*jpeg_cinfo->err->output_message) (jpeg_cinfo);

	// Return control to the setjmp point
	longjmp(myerr->setjmp_buffer, 1);
}

bool Gf_jpeg_expand_buffer(BYTE *jpeg_data_p, int jpeg_data_size, std::vector<BYTE> *ret_bitmap_buf, int *ret_bitmap_xl, int *ret_bitmap_yl);

class Cva_jpeg
{

public:

private:

	bool		v_ready_flag;
	bool		v_destroy_flag;
	bool		v_complate_flag;

	int			v_now_expand_line_cnt;
	int			v_nokori_expand_line_cnt;
	int			v_once_expand_line_cnt;

	jpeg_decompress_struct		v_jpeg_cinfo;
	jpeg_my_error_mgr			v_jpeg_err;
	JSAMPARRAY					v_jpeg_out_row_buffer;
	int							v_jpeg_row_stride;

	BYTE		*v_jpeg_data_p;
	int			v_jpeg_data_size;

	std::vector<BYTE>		*v_bitmap_buf_p;
	int			v_bitmap_xl;
	int			v_bitmap_yl;

	BYTE		*v_dst_p;

public:

	Cva_jpeg();
	~Cva_jpeg();

	void f_init();
	void f_free();

	bool f_expand_ready(BYTE *jpeg_data_p, int jpeg_data_size, std::vector<BYTE> *ret_bitmap_buf_p, int *ret_bitmap_xl_p, int *ret_bitmap_yl_p, int once_expand_pixel_cnt);
	bool f_expand_proc();

private:

};

