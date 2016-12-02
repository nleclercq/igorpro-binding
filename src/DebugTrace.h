// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   DataCodec.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _DEBUG_TRACE_H_
#define _DEBUG_TRACE_H_

//=============================================================================
// DEPENDENCIEs
//=============================================================================
#include <yat/time/Timer.h>

#if defined(WINIGOR)
//=============================================================================
// WindowsOutputDebugString
//=============================================================================
struct WindowsOutputDebugString
{    
  //--------------------------------------------------------------------------
  //- this is a simplified version of a piece of code found on the following
  //- URL while searching for a windows 'OutputDebugStringA' example:
  //- http://coliru.stacked-crooked.com/a/481b444c09361f1d
  //--------------------------------------------------------------------------
  //- usage examples
  //--------------------------------------------------------------------------
  //- XOP_STDOUT("my name is %s", name.c_str());
  //--------------------------------------------------------------------------
  WindowsOutputDebugString ()
  {
    //- noop
  }
  ~WindowsOutputDebugString ()  
  {
    //- oss << '\n'; 
    OutputDebugStringA(oss.str().c_str());
  }
  std::ostringstream& operator() () 
  { 
    return oss;
  }
  void operator() (const char* msg) 
  { 
    oss << msg;
  }
  void operator() (const std::string& msg) 
  { 
    oss << msg;
  }
  template<class char_type> void operator() (const char_type* format, ...) 
  {
    char_type buf[4096];
    va_list ap;
    va_start(ap,format);
    vsnprintf_s(buf, 4096, format, ap);
    va_end(ap);
    operator()(buf);
  }
  void operator()(const Tango::DevFailed& e) 
  {
    static std::size_t exception_tag = 0;
    std::size_t num_errors = e.errors.length();
    for (std::size_t i = 0; i < num_errors; i++)
    {
      std::ostringstream msg;
      msg << "[Ex:" 
          << exception_tag 
          << "-Err:" 
          << i 
          << "] "
          << "Rsn: " 
          << e.errors[i].reason.in() 
          << " "
          << "Dsc: " 
          << e.errors[i].desc.in()   
          << " "
          << "Org: " 
          << e.errors[i].origin.in();
      operator()(msg.str().c_str());
    }
    exception_tag++;
  }
  static int vsnprintf_s (char* buffer, int bufsize, const char* format, va_list ap) 
  { 
    return ::vsnprintf_s (buffer, bufsize, _TRUNCATE, format, ap);
  }
  std::ostringstream oss;
};
//=============================================================================
// allow to push a Tango::DevFailed into a WindowsOutputDebugString
//=============================================================================
std::ostringstream& operator<< (std::ostringstream& oss, const Tango::DevFailed& e) 
{
  static std::size_t exception_tag = 0;
  std::size_t num_errors = e.errors.length();
  for ( std::size_t i = 0; i < num_errors; i++ ) 
  {
    oss << "[Ex:" 
        << exception_tag 
        << "-Err:" 
        << i 
        << "] "
        << "Rsn: " 
        << e.errors[i].reason.in() 
        << " "
        << "Dsc: " 
        << e.errors[i].desc.in()   
        << " "
        << "Org: " 
        << e.errors[i].origin.in();
  }
  exception_tag++;
  return oss;
}
#endif

//-----------------------------------------------------------------------------
// XOP_STDOUT
//-----------------------------------------------------------------------------
#if defined(_VERBOSE_)
# if defined(WINIGOR)
#   define XOP_STDOUT(A) WindowsOutputDebugString()() << A << std::endl
# else
#   define XOP_STDOUT(A) std::cout << A << std::endl
# endif
#else
# define XOP_STDOUT(A)
#endif
  
//-----------------------------------------------------------------------------
// LOGGING
//-----------------------------------------------------------------------------
struct LbflTrace
{
  LbflTrace(const std::string& _loc, const char* _cid)
    :  cid(_cid), loc(_loc)
  {
    XOP_STDOUT(loc << "[" << cid << "] <-");
  };
  LbflTrace(const std::string& _loc, const std::string& _cid)
    :  cid(_cid), loc(_loc)
  {
    XOP_STDOUT(loc << "[" << cid << "] <-");;
  };
  LbflTrace(const std::string& _loc, const void* _cid)
    :  cid(), loc(_loc)
  {
    std::ostringstream oss;
    oss << std::hex << _cid << std::dec;
    cid = oss.str();
    XOP_STDOUT(loc << "[" << cid << "] <-");
  };
  ~LbflTrace()
  {
    XOP_STDOUT(loc << "[" << cid << "][took: " << tmr.elapsed_msec() << "ms] ->");
  };
  yat::Timer  tmr;
  std::string cid;
  std::string loc;
};
//-----------------------------------------------------------------------------
# if defined(WINIGOR)
#   define XOP_FUNC  __FUNCTION__ 
# else
#   define XOP_FUNC  __func__
#endif
//-----------------------------------------------------------------------------
#if defined(_VERBOSE_)
# define XOP_TRACE(I) LbflTrace t(XOP_FUNC, I)
#else
# define XOP_TRACE(I)
#endif
//-----------------------------------------------------------------------------

#endif
