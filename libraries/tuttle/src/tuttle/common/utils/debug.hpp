#ifndef __TUTTLE_DEBUG__
#define __TUTTLE_DEBUG__

// pre-processeur directives :
//	# : convert to string
//	## : concatenate (or before __VA_ARGS__ to indicate that it may be empty)
//	__FILE__ : filename
//	__LINE__ : line number
//	__FUNCTION__ : function declaration
//	__PRETTY_FUNCTION__ : function name
//	__DATE__ : "Mmm dd yyyy"
//	__TIME__ : "hh:mm:ss"

//_____________________________________________________________________________
// Macros to output on terminal only in debug mode

/// @see COUT
#define COUT_DEBUG COUT

/// @see  COUT_INFOS
#define COUT_INFOS_DEBUG COUT_INFOS

/// @see  COUT_INFOS
#define COUT_WITHINFOS_DEBUG COUT_WITHINFOS

/// @see IF_DEBUG
#define IF_DEBUG(... )  __VA_ARGS__

#define COUT_VAR_DEBUG COUT_VAR
#define COUT_VAR2_DEBUG COUT_VAR2
#define COUT_VAR3_DEBUG COUT_VAR3
#define COUT_VAR4_DEBUG COUT_VAR4
#define COUT_INFOS_DEBUG COUT_INFOS
#define COUT_WITHINFOS_DEBUG COUT_WITHINFOS
#define COUT_EXCEPTION_DEBUG COUT_EXCEPTION

#ifdef WINDOWS
 #include "windows/MemoryLeaks.hpp"
#endif

#endif

