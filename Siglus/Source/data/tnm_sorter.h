#pragma		once

// ****************************************************************
// �\�[�^�[
// ================================================================
//		�X�v���C�g�̕`�揇���𐧌䂵�܂��B
//		�}�E�X�̓����蔻��ɂ��g�p����̂ŏd�v�ł��B
// ================================================================
struct S_tnm_sorter
{
	int		order;
	int		layer;

	S_tnm_sorter() : order(0), layer(0)
	{
	}
	S_tnm_sorter(int order_, int layer_)
	{
		order = order_;
		layer = layer_;
	}

	bool operator == (const S_tnm_sorter& rhs) const
	{
		return order == rhs.order && layer == rhs.layer;
	}
	bool operator != (const S_tnm_sorter& rhs) const
	{
		return order != rhs.order || layer != rhs.layer;
	}
	bool operator < (const S_tnm_sorter& rhs) const
	{
		return order < rhs.order || (order == rhs.order && layer < rhs.layer);
	}
	bool operator <= (const S_tnm_sorter& rhs) const
	{
		return order < rhs.order || (order == rhs.order && layer <= rhs.layer);
	}
	bool operator > (const S_tnm_sorter& rhs) const
	{
		return order > rhs.order || (order == rhs.order && layer > rhs.layer);
	}
	bool operator >= (const S_tnm_sorter& rhs) const
	{
		return order > rhs.order || (order == rhs.order && layer >= rhs.layer);
	}
	S_tnm_sorter& operator += (const S_tnm_sorter& rhs)
	{
		order += rhs.order;
		layer += rhs.layer;
		return *this;
	}
	S_tnm_sorter operator + (const S_tnm_sorter& rhs)
	{
		return S_tnm_sorter(order + rhs.order, layer + rhs.layer);
	}

	S_tnm_sorter plus_1()
	{
		if (layer == INT_MAX)	{
			return S_tnm_sorter(order + 1, layer + 1);
		}
		return S_tnm_sorter(order, layer + 1);
	}
	S_tnm_sorter minus_1()
	{
		if (layer == INT_MIN)	{
			return S_tnm_sorter(order - 1, layer - 1);
		}
		return S_tnm_sorter(order, layer - 1);
	}
};

