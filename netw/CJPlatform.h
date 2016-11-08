#ifndef __CJ_PLATFORM_H__
#define __CJ_PLATFORM_H__

//
#define CJ_TARGET_PLATFORM CJ_TARGET_PLATFORM

#define CJ_PLATFORM_UNKNOWN            0
#define CJ_PLATFORM_IOS                1
#define CJ_PLATFORM_ANDROID            2
#define CJ_PLATFORM_WIN32              3
#define CJ_PLATFORM_MARMALADE          4
#define CJ_PLATFORM_LINUX              5
#define CJ_PLATFORM_BADA               6
#define CJ_PLATFORM_BLACKBERRY         7
#define CJ_PLATFORM_MAC                8
#define CJ_PLATFORM_NACL               9
#define CJ_PLATFORM_EMSCRIPTEN        10
#define CJ_PLATFORM_TIZEN             11
#define CJ_PLATFORM_QT5               12
#define CJ_PLATFORM_WINRT             13

// Determine target platform by compile environment macro.
#ifndef CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM             CJ_PLATFORM_UNKNOWN
#endif

// Apple: Mac and iOS
#if defined(__APPLE__) && !defined(ANDROID) // execlude android for binding generator.
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE // TARGET_OS_IPHONE inlcudes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH. see TargetConditionals.h
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM         CJ_PLATFORM_IOS
#elif TARGET_OS_MAC
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM         CJ_PLATFORM_MAC
#endif
#endif

// android
#if defined(ANDROID)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM         CJ_PLATFORM_ANDROID
#endif

// win32
#if defined(_WIN32) && defined(_WINDOWS)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM         CJ_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX) && !defined(__APPLE__)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM         CJ_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM         CJ_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM         CJ_PLATFORM_BADA
#endif

// qnx
#if defined(__QNX__)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM     CJ_PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM     CJ_PLATFORM_NACL
#endif

// Emscripten
#if defined(EMSCRIPTEN)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM     CJ_PLATFORM_EMSCRIPTEN
#endif

// tizen
#if defined(TIZEN)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM     CJ_PLATFORM_TIZEN
#endif

// qt5
#if defined(CJ_TARGET_QT5)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM     CJ_PLATFORM_QT5
#endif

// WinRT (Windows 8.1 Store/Phone App)
#if defined(WINRT)
#undef  CJ_TARGET_PLATFORM
#define CJ_TARGET_PLATFORM          CJ_PLATFORM_WINRT
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! CJ_TARGET_PLATFORM
#error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif

#if (CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32)
#ifndef __MINGW32__
#pragma warning (disable:4127)
#endif
#endif  // CJ_PLATFORM_WIN32

#endif