#pragma		once

// ****************************************************************
// ƒcƒŠ[
// ================================================================
template <typename TYPE>
class C_tree2
{
public:
	TYPE					data;
	ARRAY< C_tree2<TYPE> >	child_list;

	typename ARRAY< C_tree2<TYPE> >::iterator add_child(TYPE data_)
	{
		C_tree2<TYPE> node;
		node.data = data_;
		child_list.push_back(node);
		return child_list.end() - 1;
	}
};
