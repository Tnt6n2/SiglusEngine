#include	"pch.h"
#include	"lzss_tree.h"
#include	"lzss_tree_find.h"
#include	"lzss_pack.h"

// ****************************************************************
// �k�y�r�r���k �g����
// ================================================================
/*
	Clzss_pack lzss;

	// �����k�����i�\�[�X�f�[�^�ւ̃|�C���^�ƃf�[�^�����w��B���[�v��������ׂɏ��������w��B 0 �̏ꍇ�A�o�b�t�@������Ȃ����A��������܂ŕԂ�܂���j
	lzss.ready(src_data, src_data_cnt, 1024*100);

	while (1)	{

		// �����k�J�n�i�o�͐�o�b�t�@�̃|�C���^�ƃT�C�Y���w��j
		if (lzss.proc(dst_buf, dst_buf_size))	{
			// ���o�͐�o�b�t�@�̃T�C�Y����
			lzss.pack_size

			break;	// �����k����
		}

		if (lzss.dst_buf_need_size > 0)	{
			// ���o�͐�o�b�t�@�̍Ċm��
		}

		// ���v���O���X�o�[�Ȃǂ̏���
		// lzss.src_complate_cnt * 100��) / src_data_cnt
	}

	// �����k������̏���

*/

// ****************************************************************
// �k�y�r�r���k ����
// ================================================================

#define		LZSS_INDEX_BIT_COUNT		12
#define		LZSS_BREAK_EVEN				1

void Clzss_pack::ready(BYTE* src_data_, DWORD src_data_cnt_, DWORD break_out_cnt_)
{
	_src_data = src_data_;
	_src_data_cnt = src_data_cnt_;

	_sizeof_data_size = sizeof(BYTE);
	_window_size = 1 << LZSS_INDEX_BIT_COUNT;
	_break_even = LZSS_BREAK_EVEN;
	_length_bit_count = 16 - LZSS_INDEX_BIT_COUNT;
	_look_ahead_size = (1 << _length_bit_count) + _break_even;

	_pack_buf_size = 8;		// ���k�o�b�t�@�T�C�Y 8 = ORG_SIZE + ARC_SIZE
	_pack_bit_count = 0;	// ���k�r�b�g�J�E���g

	// ���k�f�[�^
	_pack_data_count = 1;	// �P�o�C�g�ڂ�BitFlag
	_pack_data[0] = 0;

	// �c���[��������
	_tree_find.ready(_src_data, _src_data_cnt, _window_size, _look_ahead_size);

	_replace_cnt = 0;

	_proc_step = __PACK;		// �v���Z�X�X�e�b�v

	_break_out_cnt = break_out_cnt_;		// ���[�v����������
	_break_out_last = 0;					// �O��̃��[�v�����l

	src_complate_cnt = 0;	// ����������
	dst_buf_need_size = 0;	// ����Ȃ��o�b�t�@��
	pack_size = 0;			// ���k�T�C�Y
}

// ****************************************************************
// �k�y�r�r���k �v���Z�X
// ================================================================
bool Clzss_pack::proc(BYTE* dst_buf_, DWORD dst_buf_size_)
{
	_dst_buf = dst_buf_;
	_dst_buf_size = dst_buf_size_;
	dst_buf_need_size = 0;

	switch (_proc_step)	{
		case __COPY:
			if (!_proc_copy())		{	return false;	}		// ���k�f�[�^���o�b�t�@�ɃR�s�[����
			break;
		case __LAST_COPY:
			if (!_proc_last_copy())	{	return false;	}		// �Ō�̈��k�f�[�^���o�b�t�@�ɃR�s�[����
			_proc_step = __END;
			return _proc_end();		// �I������
			break;
		case __END:
			return _proc_end();		// �I������
			break;
		case __COMPLATE:
			return true;
			break;
	}

	_proc_step = __PACK;
	while (1)	{
		if (_make_pack_data())	{	// ���k�f�[�^�𐶐�
			_proc_step = __COPY;
			if (!_proc_copy())		{	return false;	}		// ���k�f�[�^���o�b�t�@�ɃR�s�[����
			_proc_step = __PACK;

			// �����������ɉ����ă��[�v�A�E�g����
			if (_break_out_cnt > 0)	{
				DWORD tmp = src_complate_cnt / _break_out_cnt;
				if (_break_out_last != tmp)	{
					_break_out_last = tmp;
					return false;	// ��U������Ԃ�
				}
			}
		}
		else	{
			_proc_step = __LAST_COPY;
			if (!_proc_last_copy())	{	return false;	}		// �Ō�̈��k�f�[�^���o�b�t�@�ɃR�s�[����
			_proc_step = __END;
			return _proc_end();		// �I������
		}
	}
}

// ****************************************************************
// �k�y�r�r���k �v���Z�X ���k�f�[�^���o�b�t�@�ɃR�s�[����
// ================================================================
bool Clzss_pack::_proc_copy()
{
	if (!_copy_pack_buf(false))	{
		if(dst_buf_need_size > 0)	{
			return false;	// �o�b�t�@�T�C�Y������܂���
		}
	}
	return true;	// �R�s�[�����i�������̓R�s�[���Ă��Ȃ��j
}

// ****************************************************************
// �k�y�r�r���k �v���Z�X �Ō�̈��k�f�[�^���o�b�t�@�ɃR�s�[����
// ================================================================
bool Clzss_pack::_proc_last_copy()
{
	_copy_pack_buf(true);
	if (dst_buf_need_size > 0)	{
		return false;	// �o�b�t�@�T�C�Y������܂���
	}

	return true;	// �R�s�[����
}

// ****************************************************************
// �k�y�r�r���k �v���Z�X �I������
// ================================================================
bool Clzss_pack::_proc_end()
{
	if (_dst_buf_size < 8)	{
		dst_buf_need_size = 8 - _dst_buf_size;
		return false;	// �o�b�t�@�T�C�Y������܂���
	}

	DWORD* p = (DWORD *)_dst_buf;
	*p++ = _pack_buf_size;
	*p++ = _src_data_cnt * _sizeof_data_size;
	pack_size = _pack_buf_size;		// ���k�T�C�Y
	_proc_step = __COMPLATE;
	return true;	// ���k����
}

// ****************************************************************
// �k�y�r�r���k �f�[�^��
// ================================================================
bool Clzss_pack::_make_pack_data()
{
	static int bit[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

	// �I��
	if (_tree_find.src_data_index >= _src_data_cnt)	{	return false;	}

	// �c���[����
	if (_replace_cnt > 0)	{	_tree_find.proc(_replace_cnt);	}

	// ����������
	src_complate_cnt = _tree_find.src_data_index;

	// �I��
	if (_tree_find.src_data_index >= _src_data_cnt)	{	return false;	}

	// �f�[�^�ݒ�
	if (_tree_find.matching_size <= _break_even)	{
		_replace_cnt = 1;
		_pack_data[0] |= bit[_pack_bit_count];		// �P�����o�͂̂a�h�s�t���O�r�d�s
		_pack_data[_pack_data_count] = *(_src_data + _tree_find.src_data_index);
		_pack_data_count++;
	}
	else	{
		_replace_cnt = _tree_find.matching_size;
		DWORD tmp = ((_tree_find.window_top_index - _tree_find.matching_target) % _window_size) << _length_bit_count;
		*(WORD *)&_pack_data[_pack_data_count] = (WORD)(tmp | (_tree_find.matching_size - _break_even - 1));
		_pack_data_count += 2;
	}

	_pack_bit_count++;

	return true;
}

// ****************************************************************
// �k�y�r�r���k �f�[�^���o�b�t�@�ɃR�s�[����
// ================================================================
bool Clzss_pack::_copy_pack_buf(bool last_flag)
{
	dst_buf_need_size = 0;

	if (!last_flag && _pack_bit_count!=8)	{	return false;	}

	DWORD copy_size = _pack_buf_size + _pack_data_count;
	if (_dst_buf_size < copy_size)	{
		dst_buf_need_size = copy_size - _dst_buf_size;
		return false;
	}

	CopyMemory(_dst_buf + _pack_buf_size, _pack_data, _pack_data_count);
	_pack_buf_size += _pack_data_count;
	_pack_bit_count = 0;
	_pack_data_count = 1;		// �P�o�C�g�ڂ�BitFlag
	_pack_data[0]    = 0;

	return true;
}


