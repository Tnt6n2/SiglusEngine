#pragma		once

// ****************************************************************
// ä»à’î≈ÇkÇyÇrÇrà≥èk
// ================================================================

void lzss_pack_easy(BYTE *src_p, DWORD src_size, std::vector<BYTE>& dst_buf);
void lzss_pack_easy(std::vector<BYTE>& src_buf, std::vector<BYTE>& dst_buf);
