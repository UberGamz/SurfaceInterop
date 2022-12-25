// <copyright file="main.cpp" company="AVA">
// Copyright (c) 2022 - All rights reserved.
// </copyright>
// <summary>
//   Implements the (optional) main entry point functions.
//
//   If this project is helpful please take a short survey at ->
//   https://survey.alchemer.com/s3/6799376/Mastercam-API-Developer-Satisfaction-Survey
// </summary>

// A Mastercam C++/CLI "interop" DLL

// An "entry point" is NOT required for an "interop" type DLL.
// We can have them for development/testing purposes of the DLL without the need for a calling client add-in.
// We can use a C-Hook style using the required "m_verion" and "m_main" functions.
// -or-
// We can use the NET-Hook style entry point method "Run".

#include "stdafx.h"
#include "ResourceReaderService.h"
#include "SelectionManagerExample.h"

// -------------------------------------------
// ---- C-Hook style entry point functions ---
// -------------------------------------------

#pragma managed(push, off)
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma region Required Methods

/// <summary> The REQUIRED m_version function. </summary>
///
/// <remarks> Mastercam calls this function first, when your C-Hook is about to be run.
///           It passes in a Version# that represents the current version of Mastercam
///           that is about to attempt to run the C-Hook.
///           This allows you to check if your C-Hook can run on this version of Mastercam.
///           If so, just return (to Mastercam) the same version number that was supplied,
///           else some other value to abort the running of the C-Hook.
///           </remarks>
///
/// <param name="version"> The version supplied by Mastercam. </param>
///
/// <returns> The supplied version if OK to run, else any other value to abort. </returns>
extern "C" __declspec(dllexport) int m_version (int version)
	{
	int ret = C_H_VERSION;

	// Allow this C-Hook to run in any version of Mastercam
	// that has the same major version.
	if ((version / 100) == (C_H_VERSION / 100))
		ret = version;

	return ret;
	}

/// <summary> The REQUIRED m_main entry point function. </summary>
///
/// <remarks> This is the main entry point function of the C-Hook Add-In.
///           It is called by Mastercam to start the C-Hook, when...
///           The C-Hook is started via the Main Menu: Setting-Run User Application.
///           - or -
///           The C-Hook has an associated Function Table (FT) file that references
///           m_main in the FUNCTION CPP line.
///           </remarks>
/// <param name="not_used"> not used. </param>
///
/// <returns> The MC_RETURN state flag(s) back to Mastercam. </returns>
extern "C" __declspec(dllexport) int m_main (int not_used)
	{
	return MC_NOERROR | MC_UNLOADAPP;
	}

#pragma endregion
#pragma managed (pop)

