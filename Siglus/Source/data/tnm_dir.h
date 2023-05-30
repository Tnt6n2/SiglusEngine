#pragma		once

// ****************************************************************
// �G���W���F�f�B���N�g��
// ================================================================
class C_tnm_dir
{
public:
	TSTR	get_local_save_file_path(int save_no)	{ return save_dir + str_format(_T("\\%04d"), save_no) + _T(".sav"); }
	TSTR	get_save_thumb_file_path(int save_no)	{ return save_dir + _T("\\") + get_save_thumb_file_name(save_no); }
	TSTR	get_save_thumb_file_name(int save_no);	// ini �ɂ���� bmp / png ���ς��
	TSTR	get_thumb_file_name(int thumb_no);		// ini �ɂ���� bmp / png ���ς��
	TSTR	get_config_save_file_path()				{ return save_dir + _T("\\config.sav"); }
	TSTR	get_global_save_file_path()				{ return save_dir + _T("\\global.sav"); }
	TSTR	get_read_save_file_path()				{ return save_dir + _T("\\read.sav"); }
	TSTR	get_window_ini_file_path()				{ return save_dir + _T("\\window.ini"); }
	TSTR	get_system_ini_file_path()				{ return save_dir + _T("\\system.ini"); }
	TSTR	get_movie_ini_file_path()				{ return save_dir + _T("\\movie.ini"); }
	TSTR	get_movie_playing_file_path()			{ return save_dir + _T("\\movie.dat"); }
	TSTR	get_log_file_path()						{ return save_dir + _T("\\log.txt"); }
	TSTR	get_tweet_image_file_path()				{ return save_dir + _T("\\tweet.png"); }
	TSTR	get_debug_log_file_path()				{ return debug_log_dir + _T("\\debug_log.txt"); }

	TSTR	exe_dir;		// C:\Program Files(x86)\mana\game			SiglusEngine.exe, icon, ...
	TSTR	base_dir;		// Q:\data\game								g00, dat, wav, ...
	TSTR	work_dir;		// C:\MyDocument\mana\game					savedata, SiglusDebuger.dll, ...
	TSTR	save_dir;		// C:\MyDocument\mana\game\savedata			save00.save, ...�i�Z�[�u�f�[�^�t�H���_�j
	TSTR	debug_log_dir;	// C:\MyDocument\mana\game\__DEBUG_LOG		debug_log.txt

	TSTR	append_dir;		// �A�y���h�f�B���N�g��
	TSTR	append_name;	// �A�y���h��
};
