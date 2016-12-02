// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Specific_Resources.r
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================
// This file is compiled by the resource compiler and creates XOP-specific 
// resources that IGOR looks for in an XOP module (at startup).
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "XDK_Configuration.h"
#include <XOPResources.h>

// ============================================================================
// XOPStandardHeaders
// ============================================================================
#ifdef MACIGOR
# include "XOPStandardHeaders.r"
#endif

// ============================================================================
// #DEFINEs
// ============================================================================
#define UNUSED 0 

#ifdef _XOP_ADDS_MENUS_
  #define H_MENUS 1
#else 
  #define H_MENUS 0
#endif

// ============================================================================
// General XOP properties : XOPI 1100 
// ============================================================================
#ifdef WINIGOR
#  include "XDK_XOPI_1100_Win32.r"
#else
#  include "XDK_XOPI_1100_MacOS.r"
#endif

// ============================================================================
// External operations : XOPC 1100 
// ============================================================================
#ifdef _XOP_ADDS_OPERATIONS_
#ifdef WINIGOR
#   include "XDK_XOPC_1100_Win32.r"
# else
#   include "XDK_XOPC_1100_MacOS.r"
# endif  
#endif

// ============================================================================
// External functions : XOPF 1100 
// ============================================================================
#ifdef _XOP_ADDS_FUNCTIONS_
#ifdef WINIGOR
#    include "XDK_XOPF_1100_Win32.r"
# else
#    include "XDK_XOPF_1100_MacOS.r"
# endif  
#endif

// ============================================================================
// Custom Error Messages : STR# 1100 
// ============================================================================
#ifdef _XOP_HAS_ERROR_MESSAGES_
#ifdef WINIGOR
#    include "XDK_STR#_1100_Win32.r"
# else
#    include "XDK_STR#_1100_MacOS.r"
# endif  
#endif
