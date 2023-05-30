#include	"pch.h"
#include	"nwa_unpack.h"

// ****************************************************************
// �m�v�`�� �R���X�g���N�^
// ================================================================
Cnwa_unpack::Cnwa_unpack()
{
	_init();
}

// ****************************************************************
// �m�v�`�� �f�X�g���N�^
// ================================================================
Cnwa_unpack::~Cnwa_unpack()
{
	close();
}

// ****************************************************************
// �m�v�`�� ������
// ================================================================
void Cnwa_unpack::_init()
{
	_pm.fp = NULL;
	_pm.read_sample_pos = 0;
	_unpack.unit_no = -1;
}

// ****************************************************************
// �m�v�`�� �t�@�C���I�[�v��
// ================================================================
bool Cnwa_unpack::open(const TCHAR* file_path, DWORD offset)
{
	// �N���[�Y
	close();

	// �t�@�C�����J��
	_tfopen_s(&_pm.fp, file_path, _T("rb"));
	if (!_pm.fp)	{	return false;	}

	// �t�@�C���I�t�Z�b�g
	_pm.offset = offset;
	if (_pm.offset)	{	_fseeki64(_pm.fp, _pm.offset, SEEK_SET);	}

	// �m�v�`�w�b�_�ǂݍ���
	DWORD size = (DWORD)fread(&_info.header, 1, sizeof(NWA_HEADER_STRUCT), _pm.fp);
	if (size != sizeof(NWA_HEADER_STRUCT))	{	close();	return false;	}

	// ���j�b�g�I�t�Z�b�g�e�[�u���ǂݍ���
	if (_info.header.pack_mod == -1)	{	// �����k���[�h
		_info.unit_ofs_tbl.clear();
	}
	else								{	// ���k���[�h
		_info.unit_ofs_tbl.resize(_info.header.unit_cnt);
		size = (DWORD)fread(&_info.unit_ofs_tbl[0], 1, _info.header.unit_cnt * 4, _pm.fp);
		if (size != _info.header.unit_cnt * 4)	{	close();	return false;	}
	}

	// �P�T���v���̃o�C�g��
	_info.one_sample_byte_size = _info.header.bits_per_sample / 8;

	return true;
}

// ****************************************************************
// �m�v�`�� �t�@�C���N���[�Y
// ================================================================
void Cnwa_unpack::close()
{
	if (_pm.fp)	{	fclose(_pm.fp);	}
	_info.unit_ofs_tbl.clear();
	_unpack.buf.clear();
	_init();
}

// ****************************************************************
// �m�v�`�� �ǂݍ��݈ʒu��ݒ�i�T���v���l�F�X�e���I�͍��E�łP�T���v���ł��j
// ================================================================
void Cnwa_unpack::set_read_sample_pos(DWORD sample_pos)
{
	// �⑫�F�m�v�`�̓`�����l�����ƂɃT���v���𐔂��Ă���̂ŁA�`�����l�������|����
	_pm.read_sample_pos = sample_pos * _info.header.channels;
}

// ****************************************************************
// �m�v�`�� �ǂݍ��݈ʒu���擾�i�T���v���l�F�X�e���I�͍��E�łP�T���v���ł��j
// ================================================================
DWORD Cnwa_unpack::get_read_sample_pos()
{
	// �⑫�F�m�v�`�̓`�����l�����ƂɃT���v���𐔂��Ă���̂ŁA�`�����l�������|����
	if (_info.header.channels <= 0)	{	return 0;	}
	return _pm.read_sample_pos / _info.header.channels;
}

// ****************************************************************
// �m�v�`�� ���j�b�g��ǂݍ���ŉ𓀂���
// ================================================================
bool Cnwa_unpack::_read_unit(DWORD unit_no)
{
	// ���łɉ𓀂���Ă���
	if (_unpack.unit_no == unit_no)	{	return true;	}

	// �t�@�C���̓ǂݍ��݈ʒu
	_fseeki64(_pm.fp, _pm.offset + _info.unit_ofs_tbl[unit_no], SEEK_SET);

	// �t�@�C���̓ǂݍ��݃T�C�Y
	DWORD read_size;
	if (unit_no == (_info.header.unit_cnt - 1))	{
		read_size = _info.header.last_sample_pack_size;
		_unpack.unit_sample_cnt = _info.header.last_sample_cnt;
	}
	else	{
		read_size = _info.unit_ofs_tbl[unit_no + 1] - _info.unit_ofs_tbl[unit_no];
		_unpack.unit_sample_cnt = _info.header.unit_sample_cnt;
	}

	// �ǂݍ��݃o�b�t�@�m��
	std::vector<BYTE> read_buf;
	read_buf.resize(read_size);

	// �ǂݍ���
	DWORD size = (DWORD)fread(&read_buf[0], 1, read_size, _pm.fp);
	if (size != read_size)	{	return false;	}

	// ��
	_unpack.buf.resize(_unpack.unit_sample_cnt * (_info.header.bits_per_sample / 8));
	nwa_unpack_unit(&read_buf[0], _unpack.unit_sample_cnt, 0, _unpack.unit_sample_cnt, &_unpack.buf[0], &_info.header);

	_unpack.unit_no = unit_no;
	return true;
}

// ****************************************************************
// �m�v�`�� �����k�f�[�^�̓ǂݍ���
// ================================================================
bool Cnwa_unpack::_read_no_pack_data(DWORD need_byte_size, BYTE* dp, DWORD* copy_byte_size)
{
	*copy_byte_size = 0;

//	DWORD alpha = _info.header.channels * _info.one_sample_byte_size;
	DWORD alpha = _info.one_sample_byte_size;

	// �t�@�C���̓ǂݍ��݈ʒu
	DWORD data_ofs = _pm.read_sample_pos * alpha;
	DWORD seek_ofs = sizeof(NWA_HEADER_STRUCT) + data_ofs;
	_fseeki64(_pm.fp, _pm.offset + seek_ofs, SEEK_SET);

	// �f�[�^�̃T���v���l�𒴂��ēǂ񂶂�ʖ�
	int nokori_byte_size = (_info.header.sample_cnt * _info.one_sample_byte_size) - data_ofs;
	if (nokori_byte_size <= 0)	{	return false;	}
	if (need_byte_size > (DWORD)nokori_byte_size)	{	need_byte_size = nokori_byte_size;	}

	// �ǂݍ���
	DWORD size = (DWORD)fread(dp, 1, need_byte_size, _pm.fp);
	if (size == 0)	{	return false;	}

	// �ǂݎ�ꂽ�o�C�g�����T���v���␳����
	*copy_byte_size = (size / alpha) * alpha;

	return true;
}

// ****************************************************************
// �m�v�`�� �ǂݍ��݁i�T���v�����F�X�e���I�͍��E�łP�T���v���ł��j
// ================================================================
DWORD Cnwa_unpack::read_sample(BYTE* buf, DWORD sample_cnt)
{
	// �w�b�_��񂪂��������ꍇ�́A�����Ŗ߂�
	if (_info.header.channels != 1 && _info.header.channels != 2)					{	return 0;	}
	if (_info.header.bits_per_sample != 8 && _info.header.bits_per_sample != 16)	{	return 0;	}

	// �⑫�F�m�v�`�̓`�����l�����ƂɃT���v���𐔂��Ă���̂ŁA�`�����l�������|����
	DWORD	need_byte_size = sample_cnt * _info.header.channels * _info.one_sample_byte_size;

	BYTE*	dp = buf;
	while (1)	{
		if (need_byte_size <= 0)							{	break;	}	// �K�v�Ȃ����ǂݍ��߂܂����B
		if (_pm.read_sample_pos >= _info.header.sample_cnt)	{	break;	}	// ����ȏ�̃T���v���͂���܂���B

		DWORD	copy_byte_size = 0;

		if (_info.header.pack_mod == -1)	{	// �����k���[�h
			if(!Cnwa_unpack::_read_no_pack_data(need_byte_size, dp, &copy_byte_size))	{	break;	}		// �G���[
		}
		else								{	// ���k���[�h
			// ���j�b�g��ǂݍ���ŉ𓀂���
			if (!_read_unit(_pm.read_sample_pos / _info.header.unit_sample_cnt))		{	break;	}		// �G���[

			// �R�s�[
			DWORD	ofs  = (_pm.read_sample_pos % _info.header.unit_sample_cnt) * _info.one_sample_byte_size;
			copy_byte_size = (_unpack.unit_sample_cnt * _info.one_sample_byte_size) - ofs;
			if (copy_byte_size > need_byte_size)	{	copy_byte_size = need_byte_size;	}
			CopyMemory(dp, &_unpack.buf[0] + ofs, copy_byte_size);
		}

		if (copy_byte_size == 0)	{	break;	}		// �f�[�^���𓀂ł��Ă��Ȃ��A�������͓ǂݎ��Ă��Ȃ�

		dp += copy_byte_size;
		need_byte_size -= copy_byte_size;

		// �ǂݍ��݃T���v���ʒu��i�߂�
		_pm.read_sample_pos += (copy_byte_size / _info.one_sample_byte_size);
	}

	return (sample_cnt - (need_byte_size / _info.header.channels / _info.one_sample_byte_size));
}

// ****************************************************************
// �m�v�`�� ���擾 �`�����l�����擾
// ================================================================
WORD Cnwa_unpack::get_info_channels()
{
	return _info.header.channels;
}

// ****************************************************************
// �m�v�`�� ���擾 �r�b�g�l�擾
// ================================================================
WORD Cnwa_unpack::get_info_bits_per_sample()
{
	return _info.header.bits_per_sample;
}

// ****************************************************************
// �m�v�`�� ���擾 ���g���擾
// ================================================================
DWORD Cnwa_unpack::get_info_samples_per_sec()
{
	return _info.header.samples_per_sec;
}

// ****************************************************************
// �m�v�`�� ���擾 ���T���v�����擾�i�T���v�����F�X�e���I�͍��E�łP�T���v���ł��j
// ================================================================
DWORD Cnwa_unpack::get_info_sample_cnt()
{
	if (_info.header.channels <= 0)	{	return 0;	}
	return _info.header.sample_cnt / _info.header.channels;
}

// ****************************************************************
// �m�v�`�� ���擾 �I���W�i���T�C�Y�擾
// ================================================================
DWORD Cnwa_unpack::get_info_original_size()
{
	return _info.header.original_size;
}

// ****************************************************************
// �m�v�`�� ���擾 ���k�T�C�Y�擾
// ================================================================
DWORD Cnwa_unpack::get_info_pack_size()
{
	return _info.header.pack_size;
}

// ****************************************************************
// �m�v�`�� ���擾 ���k���[�h�擾�i-1=�����k  0=��i��  1=���i��  2=���i��  3=��荂�i��  4=����荂�i��  5=������荂�i���j
// ================================================================
int Cnwa_unpack::get_info_pack_mod()
{
	return _info.header.pack_mod;
}

// ****************************************************************
// �m�v�`�� ���擾 �y�d�q�n���k���[�h�擾�i0=�������Ȃ�  1=���������������k����j
// ================================================================
int Cnwa_unpack::get_info_zero_mod()
{
	return _info.header.zero_mod;
}



