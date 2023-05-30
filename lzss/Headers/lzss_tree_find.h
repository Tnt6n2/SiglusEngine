#pragma		once

#include	"lzss_tree.h"

// ****************************************************************
// �k�y�r�r�c���[����
// ================================================================
template <typename DATATYPE> class Clzss_tree_find
{

public:

	// ����
	void		ready(DATATYPE* src_data_, DWORD src_data_cnt_, DWORD window_size_, DWORD look_ahead_size_)
	{
		src_data_index = 0;
		matching_target = 0;
		matching_size = 0;
		window_top_index = 0;

		src_data = src_data_;
		src_data_cnt = src_data_cnt_;
		window_size = window_size_;
		look_ahead_size = look_ahead_size_;

		tree.ready(window_size);
	}

	// �v���Z�X
	void		proc(DWORD replace_cnt)
	{
		for(DWORD i = 0; i < replace_cnt; i++){
			src_data_index++;
			DWORD src_data_page = src_data_index / window_size;
			window_top_index++;
			window_top_index %= window_size;

			// �c���[�ڑ�
			tree.connect(window_top_index);

			// �c���[���[�g�̂a�h�f���擾
			DWORD target = tree.get_root_big();

			matching_size = 0;

			DWORD matching_counter;
			DWORD matching_result;
			DWORD matching_loop_cnt = look_ahead_size;
			DWORD src_nokori_size = src_data_cnt - src_data_index;
			if (src_nokori_size == 0)	{	return;	}
			if (matching_loop_cnt > src_nokori_size)	{	matching_loop_cnt = src_nokori_size;	}

			while(1){
				DATATYPE* p1 = src_data + src_data_index;
				DATATYPE* p2 = src_data + (src_data_page * window_size) + target;					// ���̌v�Z�̈Ӗ����킩��Ȃ�
				if (target > src_data_index % window_size)		{	p2 -= window_size;		}		// ���̌v�Z�̈Ӗ����킩��Ȃ�

				for (matching_counter = 0; matching_counter < matching_loop_cnt; matching_counter++)	{
					matching_result = (*p1++ - *p2++);	if (matching_result != 0)	{	break;	}
				}

				if (matching_counter > matching_size)	{
					matching_size = matching_counter;
					matching_target = target;
					if(matching_size == matching_loop_cnt){		// �Œ���v���܂����B
						// �c���[����ւ�
						tree.replace(target, window_top_index);
						break;
					}
				}

				// �c���[�ǉ��ڑ��i���R�[������ target �̒l���ω����܂��j
				if (tree.additional_connect(target, window_top_index, matching_result))	{	break;	}
			}
		}
	}

	DWORD		src_data_index;
	DWORD		matching_target;
	DWORD		matching_size;
	DWORD		window_top_index;

private:
	Clzss_tree	tree;

	DATATYPE*	src_data;
	DWORD		src_data_cnt;

	DWORD		window_size;

	DWORD		look_ahead_size;
};

