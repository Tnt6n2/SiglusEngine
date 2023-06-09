#include	"pch.h"

#include	"data/tnm_save_00.h"

void C_tnm_save_stream::save(C_tnm_save_stream& stream)
{
	stream.save(this->get_buffer().size());
	stream.save(this->get(), (int)this->get_buffer().size());
}

void C_tnm_save_stream::load(C_tnm_save_stream& stream)
{
	int size = stream.load_ret<int>();
	this->resize(size);
	stream.load(this->get(), size);
}
