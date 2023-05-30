#pragma		once

#include	"element/elm_object.h"
#include	"element/elm_group.h"
#include	"element/elm_mwnd.h"
#include	"element/elm_btn_sel.h"
#include	"element/elm_world.h"
#include	"element/elm_screen.h"

// ****************************************************************
// �X�e�[�W
// ================================================================
class C_elm_stage : public C_tnm_element
{
public:
	void	init(S_element element, CTSTR& name, bool use_ini);		// ������
	void	reinit(bool restruct_flag);								// �ď�����
	void	finish();												// �I��
	void	save(C_tnm_save_stream& stream);						// �Z�[�u
	void	load(C_tnm_save_stream& stream);						// ���[�h
	void	frame_init();											// �t���[��������
	void	update_time(int past_game_time, int past_real_time);	// ���Ԃ��X�V
	bool	frame(bool draw_futoku, bool draw_shadow, int order);	// �t���[������
	void	frame_action();											// �t���[���A�N�V��������
	void	button_frame_init();									// �{�^���t���[��������
	void	regist_button(int min_order);							// �{�^����o�^
	void	button_event(C_elm_mwnd* p_sel_mwnd);					// �{�^���C�x���g
	void	copy(C_elm_stage* src);									// �R�s�[

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter);		// �X�v���C�g�c���[�̎擾

	C_elm_object_list	m_object_list;		// �I�u�W�F�N�g
	C_elm_group_list	m_group_list;		// �O���[�v
	C_elm_mwnd_list		m_mwnd_list;		// ���b�Z�[�W�E�B���h�E
	C_elm_btn_select	m_btn_sel;			// �{�^���I����
	C_elm_world_list	m_world_list;		// ���[���h
	C_elm_effect_list	m_effect_list;		// �G�t�F�N�g
	C_elm_quake_list	m_quake_list;		// �N�G�C�N
};

// ****************************************************************
// �X�e�[�W���X�g
// ================================================================
class C_elm_stage_list : public C_elm_list_ex<C_elm_stage>
{
public:
	void	init(S_element element, CTSTR& name, CTSTR& parent_name, int size, bool extend_enable, bool use_ini, C_tnm_element* p_parent);	// ������
	void	wipe(S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter);		// ���C�v
	void	finish();

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end, bool restruct_flag);		// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h

	// �Œ�p�����[�^
	TSTR	m_parent_element_name;
	bool	m_use_ini;
};
