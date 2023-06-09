#pragma		once

//extern BSP<C_d3d_texture> G_emote_texture;
//extern BSP<C_d3d_surface> G_emote_dpst_buffer;

class IEmotePlayer;

void tnm_emote_init();
void tnm_emote_free();
void tnm_emote_on_device_lost();

void tnm_emote_load(ARRAY<TSTR> file_path, IEmotePlayer** player);
void tnm_emote_frame(IEmotePlayer* player, float ms);
void tnm_emote_draw(IEmotePlayer* player, BSP<C_d3d_texture> texture);
void tnm_emote_draw(IEmotePlayer* player, BSP<C_d3d_texture> texture, float rep_x, float rep_y);
