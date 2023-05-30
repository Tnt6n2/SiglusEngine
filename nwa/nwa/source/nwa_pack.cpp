#include	"pch.h"
#include	"nwa_pack.h"

// ****************************************************************
// �m�v�`���k �g����
// ================================================================
/*
	Cnwa_pack nwa;

	// �����k�����i�\�[�X�f�[�^�ւ̃|�C���^�ƃf�[�^�T�C�Y�A�`�����l���A�r�b�g���A���g���A���k���[�h�A�y�d�q�n���[�h�A���w��B���[�v��������ׂɏ��������w��B 0 �̏ꍇ�A�o�b�t�@������Ȃ����A��������܂ŕԂ�܂���j
	nwa.ready((DWORD*)src_data, src_data_size, channels, bits_per_sample, samples_per_sec, pack_mod[0 �` 5], zero_mod[ 0 | 1 ], 1024*100);

	while (1)	{

		// �����k�J�n�i�o�͐�o�b�t�@�̃|�C���^�ƃT�C�Y���w��j
		if (nwa.proc(dst_buf, dst_buf_size))	{
			// ���o�͐�o�b�t�@�̃T�C�Y����
			nwa.pack_size

			break;	// �����k����
		}

		if (nwa.dst_buf_need_size > 0)	{
			// ���o�͐�o�b�t�@�̍Ċm��
		}

		// ���v���O���X�o�[�Ȃǂ̏���
		// nwa.src_complate_size * 100��) / src_data_size
	}

	// �����k������̏���

*/

// ****************************************************************
// �m�v�`���k �R���X�g���N�^
// ================================================================
Cnwa_pack::Cnwa_pack()
{
	// �֐��e�[�u��
	_pack_func_dat_set[0] = &Cnwa_pack::_pack_func_dat_set0;
	_pack_func_dat_set[1] = &Cnwa_pack::_pack_func_dat_set1;
	_pack_func_dat_set[2] = &Cnwa_pack::_pack_func_dat_set2;
	_pack_func_dat_set[3] = &Cnwa_pack::_pack_func_dat_set3;
	_pack_func_dat_set[4] = &Cnwa_pack::_pack_func_dat_set4;
	_pack_func_dat_set[5] = &Cnwa_pack::_pack_func_dat_set5;
}

// ****************************************************************
// �m�v�`���k ����
// ================================================================
bool Cnwa_pack::ready(BYTE* src_data_, DWORD src_data_size, WORD channels, WORD bits_per_sample, DWORD samples_per_sec, int pack_mod, int zero_mod, DWORD break_out_size_)
{
	_proc_step = __COMPLATE;	// �v���Z�X�X�e�b�v
	src_complate_size = 0;		// ���������T�C�Y
	dst_buf_need_size = 0;		// ����Ȃ��o�b�t�@��
	pack_size = 0;				// ���k�T�C�Y

	// ���k���[�h�`�F�b�N�i-1=�����k  0=��i��  1=���i��  2=���i��  3=��荂�i��  4=����荂�i��  5=������荂�i���j
	if (pack_mod < -1 || pack_mod > 5)	{	return false;	}
	// �P�U�r�b�g�̂ݑΉ�
	if (bits_per_sample != 16)			{	return false;	}
	// ���m�����^�X�e���I�̂ݑΉ�
	if (channels != 1 && channels != 2)	{	return false;	}

	// �P�T���v���̃o�C�g�T�C�Y
	_byte_per_sample = bits_per_sample / 8;

	// �\�[�X�f�[�^�T�C�Y��␳�i�X�e���I�Ŋ�T���v���������ꍇ�A�A�Ō�̂P�T���v���͗]���ł��j
	if (channels == 2)	{	src_data_size = ((src_data_size / 2) & 0xfffffe) * 2;	}

	// �\�[�X�f�[�^������
	if (src_data_size <= 0)				{	return false;	}

	// �w�b�_�ݒ�
	_header.channels        = channels;				// �`�����l���i1=���� / 2=��ڵ�j
	_header.bits_per_sample = bits_per_sample;		// �r�b�g�i8bit / 16bit�j
	_header.samples_per_sec = samples_per_sec;		// ���g���i11025 / 22050 / 44100�j

	_header.pack_mod = pack_mod;					// ���k���[�h
	_header.zero_mod = zero_mod;					// �������k���[�h

	_header.original_size   = src_data_size;		// �I���W�i���T�C�Y
	_header.pack_size       = 0;					// ���k��̃T�C�Y�i�����k������ɐݒ�j
	_header.sample_cnt      = src_data_size / _byte_per_sample;	// �T���v����

	_header.unit_sample_cnt = NWA_UNIT_SAMPLE_CNT;							// ���k�P�ʂ̃T���v����
	_header.unit_cnt        = _header.sample_cnt / _header.unit_sample_cnt;	// ���j�b�g��
	_header.last_sample_cnt = _header.sample_cnt % _header.unit_sample_cnt;	// �Ō�̈��k�̃T���v����
	_header.last_sample_pack_size = 0;										// �Ō�̈��k�T�C�Y�i�����k������ɐݒ�j

	// �Ō�̈��k�␳
	if (_header.last_sample_cnt)	{	_header.unit_cnt++;								}
	else							{	_header.last_sample_cnt = NWA_UNIT_SAMPLE_CNT;	}

	// �o�͐�̕K�v�T�C�Y
	if (_header.pack_mod == -1)		{	_dst_buf_additional = sizeof(NWA_HEADER_STRUCT);										}	// �����k���[�h
	else							{	_dst_buf_additional = sizeof(NWA_HEADER_STRUCT) + (_header.unit_cnt * sizeof(DWORD));	}	// ���k���[�h

	// ���j�b�g���
	_unit_index     = 0;											// ���j�b�g�̃C���f�b�N�X
	_unit_size      = NWA_UNIT_SAMPLE_CNT * _byte_per_sample;		// ���j�b�g�̃T�C�Y
	_last_unit_size = _header.last_sample_cnt * _byte_per_sample;	// �Ō�̃��j�b�g�̃T�C�Y

	_src_data       = src_data_;		// �\�[�X�f�[�^�̃|�C���^
	_pack_buf_size  = 0;				// ���k�o�b�t�@�T�C�Y
	_break_out_size = break_out_size_;	// ���[�v���������T�C�Y
	_break_out_last = 0;				// �O��̃��[�v�����l
	_proc_step      = __START;			// �v���Z�X�X�e�b�v

	return true;
}

// ****************************************************************
// �m�v�`���k �v���Z�X
// ================================================================
bool Cnwa_pack::proc(BYTE* dst_buf_, DWORD dst_buf_size_)
{
	_dst_buf = dst_buf_;
	_dst_buf_size = dst_buf_size_;
	dst_buf_need_size = 0;

	switch (_proc_step)	{
		case __START:
			if (!_proc_start())			{	return false;	}			// �X�^�[�g
			_proc_step = __PACK;
			break;
		case __COPY:
			if (!_proc_copy())			{	return false;	}			// �R�s�[
			_proc_step = __PACK;
			break;
		case __LAST_COPY:
			if (!_proc_copy())			{	return false;	}			// �R�s�[
			_proc_step = __END;
			return _proc_end();											// �I������
			break;
		case __END:
			return _proc_end();											// �I������
			break;
		case __COMPLATE:
			return true;
			break;
	}

	while (1)	{

		// ���X�g�t���O
		int last_flag = _unit_index == (_header.unit_cnt - 1) ? true : false;

		// ���j�b�g�T�C�Y
		DWORD unit_size = last_flag ? _last_unit_size : _unit_size;

		if (_header.pack_mod == -1)	{	// �����k���[�h
			_pack.dst_p = _src_data + src_complate_size;
			_dst_buf_additional = unit_size;
		}
		else						{	// ���k���[�h
			// �I�t�Z�b�g�ݒ�
			*((DWORD*)(_dst_buf + sizeof(NWA_HEADER_STRUCT)) + _unit_index) = _pack_buf_size;

			// ���k
			_dst_buf_additional = _pack_func_pack(_src_data + src_complate_size, unit_size);

			// �Ō�̈��k�T�C�Y
			if (last_flag)	{	_header.last_sample_pack_size = _dst_buf_additional;	}
		}

		// ����
		_unit_index++;
		src_complate_size += unit_size;

		// �R�s�[
		if (last_flag)	{
			_proc_step = __LAST_COPY;
			if (!_proc_copy())	{	return false;	}
			_proc_step = __END;
			return _proc_end();		// �I������
		}
		else	{
			_proc_step = __COPY;
			if (!_proc_copy())	{	return false;	}
			_proc_step = __PACK;
		}

		// ���������T���v�����ɉ����ă��[�v�A�E�g����
		if (_break_out_size > 0)	{
			DWORD tmp = src_complate_size / _break_out_size;
			if (_break_out_last != tmp)	{
				_break_out_last = tmp;
				return false;	// ��U������Ԃ�
			}
		}
	}
}

// ****************************************************************
// �m�v�`���k �v���Z�X �X�^�[�g
// ================================================================
bool Cnwa_pack::_proc_start()
{
	if (!_proc_check_dst_buf_size())	{	return false;	}		// �o�͐�T�C�Y�𔻒�

	_pack_buf_size += _dst_buf_additional;
	_dst_buf_additional = 0;
	pack_size = _pack_buf_size;
	return true;
}

// ****************************************************************
// �m�v�`���k �v���Z�X �R�s�[
// ================================================================
bool Cnwa_pack::_proc_copy()
{
	if (!_proc_check_dst_buf_size())	{	return false;	}		// �o�͐�T�C�Y�𔻒�

	CopyMemory(_dst_buf + _pack_buf_size, _pack.dst_p, _dst_buf_additional);

	_pack_buf_size += _dst_buf_additional;
	_dst_buf_additional = 0;
	pack_size = _pack_buf_size;
	return true;
}

// ****************************************************************
// �m�v�`���k �v���Z�X �I������
// ================================================================
bool Cnwa_pack::_proc_end()
{
	_header.pack_size = pack_size;		// ���k��̃T�C�Y

	// ���k���[�h�̕␳
	switch (_header.pack_mod)	{
		case -1:								// �����k���[�h�i�s�v�ȃ����o�̏������j
			_header.unit_sample_cnt = 0;		// ���k�P�ʂ̃T���v����
			_header.unit_cnt        = 0;		// ���j�b�g��
			_header.last_sample_cnt = 0;		// �Ō�̈��k�̃T���v����
			_header.last_sample_pack_size = 0;	// �Ō�̈��k�T�C�Y�i�����k������ɐݒ�j
			break;

		case 0:		_header.pack_mod = 2;	break;
		case 1:		_header.pack_mod = 1;	break;
		case 2:		_header.pack_mod = 0;	break;
	}

	CopyMemory(_dst_buf, &_header, sizeof(NWA_HEADER_STRUCT));
	_proc_step = __COMPLATE;
	return true;	// ���k����
}

// ****************************************************************
// �m�v�`���k �v���Z�X �o�͐�T�C�Y�𔻒�
// ================================================================
bool Cnwa_pack::_proc_check_dst_buf_size()
{
	DWORD need_size = _pack_buf_size + _dst_buf_additional + 1024;		// �]�T�����ĂP�j�m��
	if (_dst_buf_size < need_size)	{
		dst_buf_need_size = need_size - _dst_buf_size;
		return false;	// �o�b�t�@�T�C�Y������܂���
	}
	return true;
}

