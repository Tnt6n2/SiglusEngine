#pragma once

class C_tnm_save_stream;

extern TSTR G_oauth_request_token;
extern TSTR G_oauth_request_token_secret;
extern TSTR G_oauth_access_token;
extern TSTR G_oauth_access_token_secret;
extern TSTR G_twitter_user_id;
extern TSTR G_twitter_user_name;
extern TSTR G_twitter_screen_name;

bool tnm_twitter_start();
bool twitter_is_authorized();
bool twitter_save_state(C_tnm_save_stream& stream);
bool twitter_load_state(C_tnm_save_stream& stream);
bool twitter_authorize();
bool twitter_tweet(CTSTR& text, CTSTR& image_path);
