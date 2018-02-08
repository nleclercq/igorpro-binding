#pragma TextEncoding = "UTF-8"
#pragma rtGlobals = 3
#pragma version = 1.0
#pragma IgorVersion = 6.0
#pragma hide = 1 

//==============================================================================
// tango_loader.ipf
//------------------------------------------------------------------------------
// N.Leclercq - SOLEIL
//==============================================================================

//==============================================================================
// DEPENDENCIES
//==============================================================================
#include "tango_panel"
#include "tango_monitor"
#include "tango_code_generator"

//==============================================================================
// Menu: Tango
//==============================================================================
Menu "Tango"
	"TANGO Browser...", open_tango_panel()
	"-"
	"Kill all Monitors", tmon_kill_all_monitors()
end

//==============================================================================
// fonction : IgorStartOrNewHook
//==============================================================================
static function IgorStartOrNewHook(igorAppNameStr)
	String igorAppNameStr
 	tools_df_make("root:tango:common", 0);
 	print "Tango-Binding::welcome!"
 	tango_load_prefs()
 	print "Tango-Binding::initialization done!"
end

//==============================================================================
// function: IgorBeforeNewHook
//==============================================================================
static function IgorBeforeNewHook (igorAppNameStr)
 String igorAppNameStr
 tango_binding_cleanup()
end

//==============================================================================
// function: IgorBeforeQuitHook
//==============================================================================
static function IgorBeforeQuitHook (unsavedExp, unsavedNotebooks, unsavedProcedures)
 String unsavedExp, unsavedNotebooks, unsavedProcedures
 tango_binding_cleanup()
end

//==============================================================================
// function: tango_binding_cleanup
//==============================================================================
static function tango_binding_cleanup ()
 tmon_kill_all_monitors()
 tango_monitor_stop("*","*")
 print "Tango-Binding::monitors killed"
 tango_save_prefs()
 print "Tango-Binding::preferences saved"
 print "Tango-Binding::bye!"
end