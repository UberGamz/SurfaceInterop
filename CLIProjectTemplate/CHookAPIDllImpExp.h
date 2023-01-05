//
// Copyright (c) 2020 CNC Software, Inc. All rights reserved.
//

#ifdef DllImpExp
#undef DllImpExp
#endif
#ifdef MCHookAPIDefMod
#define DllImpExp	__declspec( dllexport )
#define DllExport	__declspec( dllexport )
#else
#define DllImpExp	__declspec( dllimport )
#endif
#define DLLImport __declspec (dllimport)
