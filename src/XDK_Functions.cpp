// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Functions.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// ABOUT EXTERNAL FUNCTIONS PARAMETERS
//-----------------------------------------------------------------------------
// Parameters are passed to the external function in REVERSE order . The first 
// element in the <parameters structure> is the last parameter of the function. 
// The last element in the <parameters structure> is used to store the returned 
// value (i.e the xfunc result).
// In the XOPF 1100 resource you MUST define the parameters in the "natural" 
// order (the user's order). 
// To avoid numerical problems and for future compatibility, WaveMetrics 
// recommends that you use double-precision (double) for all numeric params 
// and results. In other words :
// The structure associated with a function like "res = myfunc(0.35,126,14.2)" 
// is something like : struct {
//                              double Param3;  <= 14.2
//                              double Param2;  <= 126
//                              double Param1;  <= 0.35
//  							              double Result;  => result "goes" in res
//                             }; 
//=============================================================================

//=============================================================================
// DIRECT vs MESSAGE CALL METHOD
//-----------------------------------------------------------------------------
// WaveMetrics says :
// The shorcuts that Igor takes in calling an external function by the direct 
// method require that you observe the following restrictions in a direct 
// method function. The XFunc must not do anything that causes the Macintosh 
// LoadSeg trap to run. The XFunc can not do anything that causes, directly 
// or indirectly, a resource to be load.
//=============================================================================

//=============================================================================
// DEPENDENCIEs
//=============================================================================
#include "XDK_StandardHeaders.h"

#if defined(_XOP_ADDS_FUNCTIONS_)

#include "XDK_Utils.h"
#include "XDK_Functions.h"
#include "DevRepository.h"
#include "MonitoredObjectManager.h"
#include "TangoBinding.h"

//=============================================================================
// XFUNCS PARAMETERS 
//=============================================================================
//-----------------------------------------------------------------------------
// STRUCTURES MUST BE 2-byte-aligned
//-----------------------------------------------------------------------------
#if defined(_XDK_WINDOWS_)
# pragma pack(2)
#elif defined(_XDK_MACOSX_)
# pragma options align=mac68k
#endif
//-----------------------------------------------------------------------------
// T_VoidStruct  
//-----------------------------------------------------------------------------
typedef struct {
  double result;
} T_VoidStruct;
//-----------------------------------------------------------------------------
// T_VoidStruct  
//-----------------------------------------------------------------------------
typedef struct {
  double argin;
  Handle dev;
  double result;
} T_HNDLDBLStruct;
//-----------------------------------------------------------------------------
// T_HNDLx1Struct;
//-----------------------------------------------------------------------------
typedef struct {
  Handle dev;
  double result;
} T_HNDLx1Struct;
//-----------------------------------------------------------------------------
// T_HNDLx2Struct;
//-----------------------------------------------------------------------------
typedef struct {
  Handle arg;
  Handle dev;
  double result;
} T_HNDLx2Struct;
//-----------------------------------------------------------------------------
// T_HNDLx3Struct; 
//-----------------------------------------------------------------------------
typedef struct {
  Handle arg;
  Handle attr;
  Handle dev;
  double result;
} T_HNDLx3Struct; 
//-----------------------------------------------------------------------------
// T_HNDLx2DBLx1Struct;
//-----------------------------------------------------------------------------
typedef struct {
  double value;
  Handle argout;
  Handle dev;
  double result;
} T_HNDLx2DBLx1Struct;
//-----------------------------------------------------------------------------
// T_InOutHNDLStruct;
//-----------------------------------------------------------------------------
typedef struct {
  Handle dev;
  Handle argout;
} T_InOutHNDLStruct;
//-----------------------------------------------------------------------------
// T_TangoCmdInOutStruct  
//-----------------------------------------------------------------------------
typedef struct {
  Handle argout;
  Handle argin;
  Handle cmd;
  Handle dev;
  double result;
} T_TangoCmdInOutStruct;
//-----------------------------------------------------------------------------
// T_AddMonitoredAttrStruct  
//-----------------------------------------------------------------------------
typedef struct {
  double pp;
  Handle dest;
  Handle attr;
  Handle dev;
  double result;
} T_AddMonitoredAttrStruct;
//-----------------------------------------------------------------------------
// T_RemoveMonitoredAttrStruct  
//-----------------------------------------------------------------------------
typedef struct {
  double client;
  Handle attr;
  Handle dev;
  double result;
} T_RemoveMonitoredAttrStruct;
//-----------------------------------------------------------------------------
// T_RemoveMonitoredAttrStruct  
//-----------------------------------------------------------------------------
typedef struct {
  double client;
  double pp;
  Handle attr;
  Handle dev;
  double result;
} T_SetMonitorPeriodStruct;
//-----------------------------------------------------------------------------
// T_ImgProjStruct
//-----------------------------------------------------------------------------
typedef struct {
  double   roi_y1;
  double   roi_x1;
  double   roi_y0;
  double   roi_x0;
  double   proj_mode;
  waveHndl dest_wav;
  waveHndl src_wav;
  double result;
} T_ImgProjStruct;
//-----------------------------------------------------------------------------
// RESET STRUCTURES ALIGNMENT
//-----------------------------------------------------------------------------
#if defined(_XDK_WINDOWS_)
# pragma pack()
#elif defined(_XDK_MACOSX_)
# pragma options align=reset
#endif

//=============================================================================
// XDK_Functions member : call_mode                                                  
//=============================================================================
XOPIORecResult XFUNC::call_mode (XOPIORecParam _xfunc_id)
{
	switch (_xfunc_id) 
  {
    case kTANGO_OPEN_DEV:               
      return (XOPIORecResult)XFUNC::tango_open_device;
      break;
    case kTANGO_CLOSE_DEV:               
      return (XOPIORecResult)XFUNC::tango_close_device;
      break;
    case kTANGO_CMD_INOUT:               
      return (XOPIORecResult)XFUNC::tango_command_inout;
      break;
    case kTANGO_READ_ATTR:               
      return (XOPIORecResult)XFUNC::tango_read_attribute;
      break;
    case kTANGO_READ_ATTRS:               
      return (XOPIORecResult)XFUNC::tango_read_attributes;
      break;
    case kTANGO_WRITE_ATTR:               
      return (XOPIORecResult)XFUNC::tango_write_attribute;
      break;
    case kTANGO_WRITE_ATTRS:               
      return (XOPIORecResult)XFUNC::tango_write_attributes;
      break;
    case kTANGO_GET_DEV_STATUS:               
      return (XOPIORecResult)XFUNC::tango_get_dev_status;
      break;
    case kTANGO_PING_DEV:               
      return (XOPIORecResult)XFUNC::tango_ping_dev;
      break;
    case kTANGO_SET_DEV_TMO:               
      return (XOPIORecResult)XFUNC::tango_set_dev_timeout;
      break;
    case kTANGO_GET_DEV_TMO:               
      return (XOPIORecResult)XFUNC::tango_get_dev_timeout;
      break;
    case kTANGO_CMD_LIST:               
      return (XOPIORecResult)XFUNC::tango_get_dev_cmd_list;
      break;
    case kTANGO_ATTR_LIST:               
      return (XOPIORecResult)XFUNC::tango_get_dev_attr_list;
      break;
    case kTANGO_BLACK_BOX:               
      return (XOPIORecResult)XFUNC::tango_get_dev_black_box;
      break;
    case kTANGO_ERROR_STACK:               
      return (XOPIORecResult)XFUNC::tango_error_stack;
      break;
    case kTANGO_ADD_MON_ATTR:               
      return (XOPIORecResult)XFUNC::tango_add_attr_monitor;
      break;
    case kTANGO_REM_MON_ATTR:               
      return (XOPIORecResult)XFUNC::tango_remove_attr_monitor;
      break;
    case kTANGO_SUSPEND_MON:               
      return (XOPIORecResult)XFUNC::tango_suspend_attr_monitor;
      break;
    case kTANGO_RESUME_MON:               
      return (XOPIORecResult)XFUNC::tango_resume_attr_monitor;
      break;
    case kTANGO_CHANGE_MON_PP:               
      return (XOPIORecResult)XFUNC::tango_set_attr_monitor_period;
      break;
    case kTANGO_RELOAD_DEV_INTERFACE:
      return (long)XFUNC::tango_reload_device_interface;
      break;
    case kTANGO_IMG_PROJ:               
      return (XOPIORecResult)XFUNC::compute_image_projection;
      break;
    default:
      break;
	}
	
  return 0;
}

//=============================================================================
// XDK_Functions member : exec                                               
//=============================================================================
int XFUNC::exec (XOPIORecParam _xfunc_id, void *_func_xparams)
{
  switch (_xfunc_id) 
  { 
    case 0:
      //dummy case to make MSVC happy.
    break;
    default:
      break;
	}

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_open_device
//=============================================================================
int XFUNC::tango_open_device (void *_p)
{
  if (_p == 0) 
    return kIGOR_INTERNAL_ERROR;

  XDK_UTILS->reset_error();

  T_HNDLx1Struct *xfp = static_cast<T_HNDLx1Struct *>(_p);

  std::string dev; 
  if (XDK_UTILS->handle_to_str(xfp->dev, dev)) {
    XDK_UTILS->set_error("XFUNC::tango_open_device failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_open_device");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->open_device(dev);

  if (xfp->result == kError) {
    std::string d = "failed to open device " + dev;
    XDK_UTILS->push_error("tango_open_device failed",
                          d.c_str(),
                          "XFUNC::tango_open_device");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_close_device
//=============================================================================
int XFUNC::tango_close_device (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx1Struct *xfp = static_cast<T_HNDLx1Struct *>(_p);

  std::string dev; 
  if (XDK_UTILS->handle_to_str(xfp->dev, dev)) {
    XDK_UTILS->set_error("XFUNC::tango_close_device failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_close_device");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->close_device(dev);

  if (xfp->result == kError) {
    std::string d = "failed to close device " + dev;
    XDK_UTILS->push_error("tango_close_device failed",
                          d.c_str(),
                          "XFUNC::tango_close_device");
  }
  
  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_command_inout
//=============================================================================
int XFUNC::tango_command_inout (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;
    
  T_TangoCmdInOutStruct *xfp = static_cast<T_TangoCmdInOutStruct *>(_p);

  std::string dev, cmd, argin, argout; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->cmd, cmd);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->argin, argin);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->argout, argout);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("XFUNC::command_in_out failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::command_in_out");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->command_in_out(dev, cmd, argin, argout);

  if (xfp->result == kError) {
    std::string d = "failed to execute " + cmd;
    d += "(" + argin + ", " + argout + ") on " + dev;
    XDK_UTILS->push_error("tango_command_in_out failed",
                          d.c_str(),
                          "XFUNC::tango_command_in_out");
  }

  return kNoError;
}


//=============================================================================
// XDK_Functions member : tango_read_attribute
//=============================================================================
int XFUNC::tango_read_attribute (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;
    
  T_HNDLx3Struct *xfp = static_cast<T_HNDLx3Struct *>(_p);

  std::string dev, attr, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->attr, attr);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("XFUNC::tango_read_attribute failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_read_attribute");
    xfp->result = kError;
    return kNoError;
  }

  if (arg.size() == 0) {
    XDK_UTILS->set_error("XFUNC::read_attribute failed",
                         "invalid argout specified (empty)",
                         "XFUNC::read_attribute");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->read_attribute(dev, attr, arg);
  
  if (xfp->result == kError) {
    std::string d = "failed to read attribute " + attr + " on " + dev;
    XDK_UTILS->push_error("tango_read_attribute failed",
                          d.c_str(),
                          "XFUNC::tango_read_attribute");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_read_attributes
//=============================================================================
int XFUNC::tango_read_attributes (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;
    
  T_HNDLx2Struct *xfp = static_cast<T_HNDLx2Struct *>(_p);

  std::string dev, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("XFUNC::tango_read_attributes failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_read_attributes");
    xfp->result = kError;
    return kNoError;
  }

  if (arg.size() == 0) {
    XDK_UTILS->set_error("XFUNC::read_attributes failed",
                         "invalid argout specified (empty)",
                         "XFUNC::read_attributes");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->read_attributes(dev, arg);
  
  if (xfp->result == kError) {
    std::string d = "failed to read " + dev + " attributes";
    XDK_UTILS->push_error("tango_read_attributes failed",
                          d.c_str(),
                          "XFUNC::tango_read_attributes");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_write_attribute
//=============================================================================
int XFUNC::tango_write_attribute (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;
    
  T_HNDLx3Struct *xfp = static_cast<T_HNDLx3Struct *>(_p);

  std::string dev, attr, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->attr, attr);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("XFUNC::write_attribute failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::write_attribute");
    xfp->result = kError;
    return kNoError;
  }

  
  if (arg.size() == 0) {
    XDK_UTILS->set_error("XFUNC::write_attribute failed",
                         "invalid argin specified (empty)",
                         "XFUNC::write_attribute");
    xfp->result = kError;
    return kNoError;
  }


  xfp->result = IDEV_PROXY->write_attribute(dev, attr, arg);
  
  if (xfp->result == kError) {
    std::string d = "failed to write attribute " + attr + " on " + dev;
    XDK_UTILS->push_error("tango_write_attribute failed",
                          d.c_str(),
                          "XFUNC::tango_write_attribute");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_write_attributes
//=============================================================================
int XFUNC::tango_write_attributes (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;
    
  T_HNDLx2Struct *xfp = static_cast<T_HNDLx2Struct *>(_p);

  std::string dev, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("XFUNC::write_attributes failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::write_attributes");
    xfp->result = kError;
    return kNoError;
  }

  if (arg.size() == 0) {
    XDK_UTILS->set_error("XFUNC::write_attributes failed",
                         "invalid argin specified (empty)",
                         "XFUNC::write_attributes");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->write_attributes(dev, arg);

  if (xfp->result == kError) {
    std::string d = "failed to write " + dev + " attributes";
    XDK_UTILS->push_error("tango_writes_attributes failed",
                          d.c_str(),
                          "XFUNC::tango_writes_attributes");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_get_dev_status
//=============================================================================
int XFUNC::tango_get_dev_status (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_InOutHNDLStruct *xfp = static_cast<T_InOutHNDLStruct *>(_p);

  std::string dev; 
  std::string status;

  int result = XDK_UTILS->handle_to_str(xfp->dev, dev);
  if (result) {
    xfp->argout = 0;
    goto bail;
  }

  result = IDEV_PROXY->status(dev, status);
  if (result) {
    xfp->argout = 0;
    goto bail;
  }

  result = XDK_UTILS->str_to_handle(status, xfp->argout);
  if (result) {
    xfp->argout = 0;
    goto bail;
  }

  return kNoError;

bail:
  std::string d = "failed to get " + dev + " status";
  XDK_UTILS->push_error("tango_get_dev_status failed",
                        d.c_str(),
                        "XFUNC::tango_get_dev_status");

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_ping_dev
//=============================================================================
int XFUNC::tango_ping_dev (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx1Struct *xfp = static_cast<T_HNDLx1Struct *>(_p);

  std::string dev; 
  int result = XDK_UTILS->handle_to_str(xfp->dev, dev);
  if(result) {
    XDK_UTILS->set_error("XFUNC::tango_ping_dev failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_ping_dev");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->ping(dev);

  if (xfp->result == kError) {
    std::string d = dev + " is unreachable";
    XDK_UTILS->push_error("tango_ping_dev failed",
                          d.c_str(),
                          "XFUNC::tango_ping_dev");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_set_dev_timeout
//=============================================================================
int XFUNC::tango_set_dev_timeout (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLDBLStruct *xfp = static_cast<T_HNDLDBLStruct *>(_p);

  int result;
  std::string dev; 
  result = XDK_UTILS->handle_to_str(xfp->dev, dev);
  if(result) {
    XDK_UTILS->set_error("XFUNC::set_timeout failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::set_timeout");
    xfp->result = kError;
    return kNoError;
  }

  int timeout = 0;
  int nt_type = (sizeof(int) == 4) ? NT_I32 : NT_I16;
  ::ConvertData2(&xfp->argin, &timeout, 1, NT_FP64, nt_type);

  xfp->result = IDEV_PROXY->set_timeout(dev, timeout);
  
  if (xfp->result == kError) {
    std::string d = "failed to set " + dev + " timeout";
    XDK_UTILS->push_error("tango_set_dev_timeout failed",
                          d.c_str(),
                          "XFUNC::tango_set_dev_timeout");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_get_dev_timeout
//=============================================================================
int XFUNC::tango_get_dev_timeout (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx1Struct *xfp = static_cast<T_HNDLx1Struct *>(_p);

  int result;
  std::string dev; 
  result = XDK_UTILS->handle_to_str(xfp->dev, dev);
  if(result) {
    XDK_UTILS->set_error("XFUNC::tango_get_dev_timeout failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_get_dev_timeout");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->get_timeout(dev);
  
  if (xfp->result == kError) {
    std::string d = "failed to get " + dev + " timeout";
    XDK_UTILS->push_error("tango_get_dev_timeout failed",
                          d.c_str(),
                          "XFUNC::tango_get_dev_timeout");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_get_dev_cmd_list
//=============================================================================
int XFUNC::tango_get_dev_cmd_list (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx2Struct *xfp = static_cast<T_HNDLx2Struct *>(_p);

  int result;
  std::string dev, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("tango_get_dev_cmd_list",
                         "internal error (handle_to_str failed)",
                         "XFUNC::tango_get_dev_cmd_list");
    xfp->result = kError;
    return kNoError;
  }

  DevDescriptor* ddesc = DEV_REP->device_desc(dev);
  if (ddesc == 0) 
  {
    std::string d = "failed to get " + dev + " commands list";
    XDK_UTILS->push_error("tango_get_dev_attr_list failed",
                          d.c_str(),
                          "XFUNC::tango_get_dev_cmd_list");
    return kError;
  }

  xfp->result = ddesc->update_commands_datafolder(arg);
  
  if (xfp->result == kError) {
    std::string d = "failed to get " + dev + " commands list";
    XDK_UTILS->push_error("tango_get_dev_cmd_list failed",
                          d.c_str(),
                          "XFUNC::tango_get_dev_cmd_list");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_get_dev_attr_list
//=============================================================================
int XFUNC::tango_get_dev_attr_list (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx2Struct *xfp = static_cast<T_HNDLx2Struct *>(_p);

  int result;
  std::string dev, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("tango_get_dev_attr_list failed",
                         "internal error (handle_to_str failed)",
                         "XFUNC::tango_get_dev_attr_list");
    xfp->result = kError;
    return kNoError;
  }

  DevDescriptor* ddesc = DEV_REP->device_desc(dev);
  if (ddesc == 0) 
  {
    std::string d = "failed to get " + dev + " attributes list";
    XDK_UTILS->push_error("tango_get_dev_attr_list failed",
                          d.c_str(),
                          "XFUNC::tango_get_dev_attr_list");
    return kError;
  }

  xfp->result = ddesc->update_attributes_datafolder(arg);

  if (xfp->result == kError) {
    std::string d = "failed to get " + dev + " attributes list";
    XDK_UTILS->push_error("tango_get_dev_attr_list failed",
                          d.c_str(),
                          "XFUNC::tango_get_dev_attr_list");
    return kError;
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_get_dev_black_box
//=============================================================================
int XFUNC::tango_get_dev_black_box (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx2DBLx1Struct *xfp = static_cast<T_HNDLx2DBLx1Struct *>(_p);

  yat::int32 n;
  int result;
  std::string dev, argout; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->argout, argout);
    if (result) break;
    n = (yat::int32)xfp->value;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("tango_get_dev_black_box failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_get_dev_black_box");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->black_box(dev, argout, n);
  
  if (xfp->result == kError) {
    std::string d = "failed to get " + dev + " black box";
    XDK_UTILS->push_error("tango_get_dev_black_box failed",
                          d.c_str(),
                          "XFUNC::tango_get_dev_black_box");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_error_stack
//=============================================================================
int XFUNC::tango_error_stack (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  T_VoidStruct *xfp = static_cast<T_VoidStruct *>(_p);

  xfp->result = XDK_UTILS->export_error();
   
  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_add_attr_monitor 
//=============================================================================
int XFUNC::tango_add_attr_monitor (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;

  T_AddMonitoredAttrStruct *xfp = static_cast<T_AddMonitoredAttrStruct *>(_p);

  yat::int32 pp = static_cast<yat::uint32>(xfp->pp);

  std::string dev, attr, dest;
  do 
  {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->attr, attr);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->dest, dest);
    if (result) break;
  } while (0);

  if (result) 
  {
    XDK_UTILS->set_error("XFUNC::tango_add_monitored_attr failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_add_monitored_attr");
    xfp->result = kError;
    return kNoError;
  }
  
  yat::int32 new_client_id;
  xfp->result = MON_OBJ_MANAGER->add_attribute(dev, attr, dest, pp, new_client_id);

  xfp->result = (xfp->result == kError) ? xfp->result : new_client_id;

  if (xfp->result == kError) 
  {
    std::string d = "failed to monitor " + dev + "/" + attr;
    XDK_UTILS->push_error("MonitoredObjectManager::add_attribute failed",
                          d.c_str(),
                          "XFUNC::tango_add_monitored_attr");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_remove_attr_monitor
//=============================================================================
int XFUNC::tango_remove_attr_monitor (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;

  T_RemoveMonitoredAttrStruct *xfp = static_cast<T_RemoveMonitoredAttrStruct *>(_p);

  yat::int32 client_id = (yat::int32)xfp->client;

  std::string dev, attr;
  do 
  {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->attr, attr);
    if (result) break;
  } while (0);

  if (result) 
  {
    XDK_UTILS->set_error("XFUNC::tango_add_monitored_attr failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_add_monitored_attr");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = MON_OBJ_MANAGER->remove_attribute(dev, attr, client_id);

  if (xfp->result == kError) 
  {
    std::string d = "failed to remove monitor " + dev + "/" + attr;
    XDK_UTILS->push_error("MonitoredObjectManager::remove_attribute",
                          d.c_str(),
                          "XFUNC::tango_remove_monitored_attr");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_suspend_attr_monitor
//=============================================================================
int XFUNC::tango_suspend_attr_monitor (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;

  T_RemoveMonitoredAttrStruct *xfp = static_cast<T_RemoveMonitoredAttrStruct *>(_p);

  yat::int32 client_id = (yat::int32)xfp->client;

  std::string dev, attr;
  do 
  {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->attr, attr);
    if (result) break;
  } while (0);

  if (result) 
  {
    XDK_UTILS->set_error("XFUNC::tango_suspend_monitor failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_suspend_monitor");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = MON_OBJ_MANAGER->suspend_monitor (dev, attr, client_id);

  if (xfp->result == kError) 
  {
    std::string d = "failed to suspend monitor " + dev + "/" + attr;
    XDK_UTILS->push_error("MonitoredObjectManager::suspend_monitor",
                          d.c_str(),
                          "XFUNC::tango_suspend_monitor");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_resume_attr_monitor
//=============================================================================
int XFUNC::tango_resume_attr_monitor (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;

  T_RemoveMonitoredAttrStruct *xfp = static_cast<T_RemoveMonitoredAttrStruct *>(_p);

  yat::int32 client_id = (yat::int32)xfp->client;

  std::string dev, attr;
  do 
  {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->attr, attr);
    if (result) break;
  } while (0);

  if (result) 
  {
    XDK_UTILS->set_error("XFUNC::tango_resume_monitor failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_resume_monitor");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = MON_OBJ_MANAGER->resume_monitor (dev, attr, client_id);

  if (xfp->result == kError) 
  {
    std::string d = "failed to resume monitor " + dev + "/" + attr;
    XDK_UTILS->push_error("MonitoredObjectManager::resume_monitor",
                          d.c_str(),
                          "XFUNC::tango_resume_monitor");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_set_attr_monitor_period
//=============================================================================
int XFUNC::tango_set_attr_monitor_period (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;

  T_SetMonitorPeriodStruct *xfp = static_cast<T_SetMonitorPeriodStruct *>(_p);

  yat::int32 client_id = (yat::int32)xfp->client;

  std::string dev, attr;
  do 
  {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->attr, attr);
    if (result) break;
    
  } while (0);

  if (result) 
  {
    XDK_UTILS->set_error("XFUNC::tango_change_monitor_period failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_change_monitor_period");
    xfp->result = kError;
    return kNoError;
  }

  yat::uint32 pp = static_cast<yat::uint32>(xfp->pp);

  xfp->result = MON_OBJ_MANAGER->change_monitor_period (dev, attr, pp, client_id);

  if (xfp->result == kError) 
  {
    std::string d = "failed to change monitor period for " + dev + "/" + attr;
    XDK_UTILS->push_error("MonitoredObjectManager::change_monitor_period",
                          d.c_str(),
                          "XFUNC::tango_change_monitor_period");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_reload_device_interface
//=============================================================================
int XFUNC::tango_reload_device_interface (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;

  T_HNDLx1Struct *xfp = static_cast<T_HNDLx1Struct *>(_p);

  std::string dev;
  do 
  {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
  } while (0);

  if (result) 
  {
    XDK_UTILS->set_error("XFUNC::tango_reload_device_interface failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_reload_device_interface");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = DEV_REP->reload_device_interface(dev);

  if (xfp->result == kError) 
  {
    std::string d = "failed to reload interface for device " + dev;
    XDK_UTILS->push_error("XFUNC::tango_reload_device_interface failed",
                          d.c_str(),
                          "XFUNC::tango_reload_device_interface");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : compute_image_projection
//=============================================================================            
template <typename T>
int compute_image_projection_impl (T_ImgProjStruct * xfp, T = 0)
{
  //- get src wave dimensions
  int src_ndims;
  MDWaveDims src_dims;
  waveHndl src_wave = xfp->src_wav;
  if (::MDGetWaveDimensions(src_wave, &src_ndims, src_dims) != kNoError) 
  {
    XDK_UTILS->set_error("XFUNC::compute_image_projection failed",
                         "internal error (MDGetWaveDimensions failed)",
                         "XFUNC::compute_image_projection");
    xfp->result = kError;
    return kNoError;
  }

  //- src wave must be an image
  if (src_ndims != 2) 
  {
    XDK_UTILS->set_error("XFUNC::compute_image_projection failed",
                         "source wave must be an image (i.e. a 2D wave)",
                         "XFUNC::compute_image_projection");
    xfp->result = kError;
    return kNoError;
  }

  //- get the proj type (or mode :-)
  size_t proj_mode = (size_t)xfp->proj_mode;

  //- dest wave
  waveHndl dest_wave = xfp->dest_wav;

  //- dest wave dims
  MDWaveDims dest_dims;
  ::memset(dest_dims, 0, sizeof(MDWaveDims));

  //- x & y range
  size_t x_min = static_cast<size_t>(xfp->roi_x0); 
  size_t x_max = static_cast<size_t>(xfp->roi_x1);  
  size_t y_min = static_cast<size_t>(xfp->roi_y0);  
  size_t y_max = static_cast<size_t>(xfp->roi_y1);  

  //- is null range
  bool null_range = false;

  //- x range checking
  if (x_min == x_max)
  {
    x_min = 0;
    x_max = src_dims[0] - 1;
    null_range = true;
  }
  else if (x_min > x_max)
  {
    size_t tmp = x_min;
    x_min = x_max;
    x_max = tmp;
  }

  //- y range checking
  if (y_min == y_max)
  {
    y_min = 0;
    y_max = src_dims[1] - 1;
    null_range = true;
  }
  else if (y_min > y_max)
  {
    size_t tmp = y_min;
    y_min = y_max;
    y_max = tmp;
  }

  //- set dim size of the dest wave
  switch (proj_mode)
  {
    //- x_min proj
    case 0:
      dest_dims[0] = x_max - x_min + 1;
      break;
    //- y_min proj
    case 1:
      dest_dims[0] = y_max - y_min + 1;
      break;
  }

  //- adapt dest wave to the situation
  if (::MDChangeWave(dest_wave, NT_FP64, dest_dims))
  {
    XDK_UTILS->set_error("XFUNC::compute_image_projection failed",
                         "internal error (MDChangeWave failed)",
                         "XFUNC::compute_image_projection");
    xfp->result = kError;
    return kNoError;
  }

  //- data offset: compute base addr of src data
  BCInt src_offset;
  if (::MDAccessNumericWaveData(src_wave, kMDWaveAccessMode0, &src_offset) != 0)
  {
    XDK_UTILS->set_error("XFUNC::compute_image_projection failed",
                         "internal error (MDAccessNumericWaveData failed)",
                         "XFUNC::compute_image_projection");
    xfp->result = kError;
    return kNoError;
  }

  //- data offset: compute base addr of dest data
  BCInt dest_offset;
  if (::MDAccessNumericWaveData(dest_wave, kMDWaveAccessMode0, &dest_offset) != 0)
  {
    XDK_UTILS->set_error("XFUNC::compute_image_projection failed",
                         "internal error (MDAccessNumericWaveData failed)",
                         "XFUNC::compute_image_projection");
    xfp->result = kError;
    return kNoError;
  }

  //- compute base addr of src data
  T * src = reinterpret_cast<T*>(static_cast<char*>(*src_wave) + src_offset);

  //- compute base addr of dest data
  double * dest = reinterpret_cast<double*>(static_cast<char*>(*dest_wave) + dest_offset);

  //- set each point of dest to 0
  ::memset(dest, 0, dest_dims[0] * sizeof(double));

  //- skip profile computation for null ranges
  if (! null_range)
  {
    size_t x = x_min;
    size_t y = y_min;
    switch(proj_mode) 
    {
      //- x proj
      case 0:
        {
          for (size_t i = 0; x <= x_max; x++, i++) 
            for (y = y_min; y <= y_max; y++)   
              dest[i] += src[x + src_dims[0] * y];        
        }
        break;
      //- y proj
      case 1:
        {
          for (size_t i = 0; y <= y_max; y++, i++) 
            for (x = x_min; x <= x_max; x++)   
              dest[i] += src[x + src_dims[0] * y];       
        }     
        break;
    }
  }

  ::WaveHandleModified(dest_wave);

  return kNoError;
}    

//=============================================================================
// XDK_Functions member : compute_image_projection
//=============================================================================            
int XFUNC::compute_image_projection (void *_p)
{
  if (_p == 0)
    return kIGOR_INTERNAL_ERROR;
    
  T_ImgProjStruct *xfp = reinterpret_cast<T_ImgProjStruct *>(_p);
  
  if (xfp->src_wav == 0 || xfp->dest_wav == 0) 
  {
    XDK_UTILS->set_error("XFUNC::compute_image_projection failed",
                         "internal error (null wavehandle)",
                         "XFUNC::compute_image_projection");
    xfp->result = kError;
    return kNoError;
  }

  //- get src wave data type
  switch (::WaveType(xfp->src_wav))
  {
    case NT_I8:
      return ::compute_image_projection_impl<char>(xfp); 
      break;
    case NT_I8 | NT_UNSIGNED:
      return ::compute_image_projection_impl<unsigned char>(xfp);  
      break;
    case NT_I16:
      return ::compute_image_projection_impl<short>(xfp);  
      break;
    case NT_I16 | NT_UNSIGNED:
      return ::compute_image_projection_impl<unsigned short>(xfp);  
      break;
    case NT_I32:
      return ::compute_image_projection_impl<yat::int32>(xfp);  
      break;
    case NT_I32 | NT_UNSIGNED:
      return ::compute_image_projection_impl<yat::uint32>(xfp);  
      break;
    case NT_FP32:
      return ::compute_image_projection_impl<float>(xfp);  
      break;
    case NT_FP64:
      return ::compute_image_projection_impl<double>(xfp);  
      break;
    default:
      XDK_UTILS->set_error("XFUNC::compute_image_projection failed",
                           "unexpected/unsupported data type for src wave",
                           "XFUNC::compute_image_projection");
      xfp->result = kError;
      return kError;
      break;
  }

  return kNoError;
}     


#endif // _XOP_ADDS_FUNCTIONS_