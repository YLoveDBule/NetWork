#ifndef __TCP_PLATFORM_H__
#define __TCP_PLATFORM_H__

#define  TCP_PLATFORM_UNKOWN		       0
#define TCP_PLATFORM_IOS			       1
#define TCP_PLATFORM_ANDROID		      2
#define TCP_PLATFORM_WIN32		      3
#define TCP_PLATFORM_MARMALADE          4
#define TCP_PLATFORM_LINUX              5
#define TCP_PLATFORM_BADA               6
#define TCP_PLATFORM_BLACKBERRY  7
#define TCP_PLATFORM_MAC                8
#define TCP_PLATFORM_NACL               9
#define TCP_PLATFORM_EMSCRIPTEN         10
#define TCP_PLATFORM_TIZEN              11
#define TCP_PLATFORM_QT5                12
#define TCP_PLATFORM_WINRT              13

#define	TCP_TARGET_PLATFORM  TCP_PLATFORM_UNKOWN

//Apple 
#if defined(__APPLE__) && !defined(ANDROID) // execlude android for binding generator.
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE // TARGET_OS_IPHONE inlcudes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH. see TargetConditionals.h
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM         TCP_PLATFORM_IOS
#elif TARGET_OS_MAC
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM         TCP_PLATFORM_MAC
#endif
#endif

// android
#if defined(ANDROID)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM         TCP_PLATFORM_ANDROID
#endif

// win32
#if defined(_WIN32) && defined(_WINDOWS)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM         TCP_PLATFORM_WIN32  //TCP_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX) && !defined(__APPLE__)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM         TCP_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM         TCP_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM         TCP_PLATFORM_BADA
#endif

// qnx
#if defined(__QNX__)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM     TCP_PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM     TCP_PLATFORM_NACL
#endif

// Emscripten
#if defined(EMSCRIPTEN)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM     TCP_PLATFORM_EMSCRIPTEN
#endif

// tizen
#if defined(TIZEN)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM     TCP_PLATFORM_TIZEN
#endif

// qt5
#if defined(TCP_TARGET_QT5)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM     TCP_PLATFORM_QT5
#endif

// WinRT (Windows 8.1 Store/Phone App)
#if defined(WINRT)
#undef  TCP_TARGET_PLATFORM
#define TCP_TARGET_PLATFORM          TCP_PLATFORM_WINRT
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! TCP_TARGET_PLATFORM
#error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif

#if (TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32)
#ifndef __MINGW32__
#pragma warning (disable:4127)
#endif
#endif  // TCP_PLATFORM_WIN32


#endif
