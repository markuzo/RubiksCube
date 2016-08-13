/*********************************************************************************/
/* Title:		stdafx.h																										         */
/* Author:	Markus Schlafli																							         */
/* Date:		31-Jul-2013																								           */
/*********************************************************************************/

//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#ifdef ALGORITHMS_EXPORT
#define ALGORITHMS __declspec(dllexport)
#else
#define ALGORITHMS __declspec(dllimport)
#endif