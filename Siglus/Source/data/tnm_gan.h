#pragma		once

#include	"data/tnm_gan_data.h"

class	C_tnm_save_stream;

// ****************************************************************
// ＧＡＮ
// ================================================================
class C_tnm_gan
{
public:

	C_tnm_gan();													// コンストラクタ
	~C_tnm_gan();													// デストラクタ

	void		init();												// 初期化
	void		free();												// 解放
	void		save(C_tnm_save_stream& stream);					// セーブ
	void		load(C_tnm_save_stream& stream);					// ロード
	void		copy(const C_tnm_gan* src);							// コピー
	void		update_time(int past_game_time, int past_real_time);						// 時間を進める

	bool		load_gan(CTSTR& file_name);							// 読み込み
	bool		load_gan_only(CTSTR& file_name);					// 読み込みのみ
	void		start_anm(int anm_set_no, bool anm_loop_flag, bool anm_real_time_flag);		// アニメ開始
	void		next_anm(int anm_set_no, bool anm_loop_flag, bool anm_real_time_flag);		// 次のアニメを設定（現在のアニメはワンショットになります）
	void		change_anm_set_no(int anm_set_no);					// アニメのセット番号を変更
	void		change_anm_loop(bool anm_loop_flag);				// アニメのワンショット／ループの変更
	void		pause_anm();										// アニメポーズ
	void		resume_anm();										// アニメポーズ解除


	int			get_pat_data_pat_no()	{	return (work.p_pat_data) ? work.p_pat_data->pat_no : 0;	}	// パターン情報取得：パターン番号
	int			get_pat_data_x()		{	return (work.p_pat_data) ? work.p_pat_data->x : 0;		}	// パターン情報取得：Ｘ座標
	int			get_pat_data_y()		{	return (work.p_pat_data) ? work.p_pat_data->y : 0;		}	// パターン情報取得：Ｙ座標
	int			get_pat_data_tr()		{	return (work.p_pat_data) ? work.p_pat_data->tr : 255;	}	// パターン情報取得：透明度

private:

	struct WORK
	{
		BSP<C_gan_data>	p_gan_data;
		S_gan_pat_data*	p_pat_data;

		TSTR			gan_name;
		int				now_time;
		int				anm_set_no;
		int				next_anm_set_no;

		bool			anm_start;
		bool			anm_pause;
		bool			anm_loop_flag;
		bool			anm_real_time_flag;
		bool			next_anm_flag;
		bool			next_anm_loop_flag;
		bool			next_anm_real_time_flag;
	}	work;

};



