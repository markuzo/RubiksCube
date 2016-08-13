/*********************************************************************************/
/* Title:		stdafx.h																										         */
/* Author:	Markus Schlafli																							         */
/* Date:		27-Jul-2013																								           */
/*********************************************************************************/

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#ifdef RENDERER_EXPORT
#define RENDERER __declspec(dllexport)
#else
#define RENDERER __declspec(dllimport)
#endif

#ifdef ALGORITHMS_EXPORT
#define ALGORITHMS __declspec(dllexport)
#else
#define ALGORITHMS __declspec(dllimport)
#endif
