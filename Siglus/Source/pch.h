

// ****************************************************************
// boost
// ================================================================
#include	<boost/any.hpp>

// ****************************************************************
// tona3
// ================================================================
#include	<tona3.h>
#include	<tonawindow3.h>
#include	<tonagraphic3.h>
#include	<tonasound3.h>
#include	<tonad3d3.h>
#include	<tonanet3.h>

using namespace	NT3;

// ****************************************************************
// プリプロセッサ判別
// ================================================================

#ifdef __SIGLUS_STEAM

#define		__USE_STEAM		1

#else

#define		__USE_STEAM		0

#endif

// ****************************************************************
// Steam
// ================================================================

#if __USE_STEAM

#include	<steam/steam_api.h>
#pragma		comment(lib,"steam/steam_api.lib")
#pragma		comment(lib,"steam/sdkencryptedappticket64.lib")

#endif




