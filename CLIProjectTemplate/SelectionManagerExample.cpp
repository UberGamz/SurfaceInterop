// <copyright file="SelectionManagerExample.cpp" company="AVA">
// Copyright (c) 2022 - All rights reserved.
// </copyright>
// <summary> Declares the SelectionManager class which utilizes a NET-Hook API and some c++ code.
// If this project is helpful please take a short survey at ->
// https://survey.alchemer.com/s3/6799376/Mastercam-API-Developer-Satisfaction-Survey
// </summary>

#include "stdafx.h"
#include "SelectionManagerExample.h"
#include "NETHookApiReflection.h"

using namespace Mastercam::BasicGeometry;
using namespace Mastercam::Curves;

namespace Mastercam::IO::Interop {

#pragma region Nethook code example

int SelectionManager::CreateGeometry ()
	{
	PointGeometry ^pt1 = gcnew PointGeometry ();
	PointGeometry ^pt2 = gcnew PointGeometry (20, 20, 0);
	pt2->Color = 12;
	pt1->Commit ();
	pt2->Commit ();
	LineGeometry ^line = gcnew LineGeometry (pt1->Data, pt2->Data);
	line->LineWidth = 4;
	line->Commit ();
	return line->GetEntityID ();
	}

#pragma endregion

#pragma region C++ code example



bool SelectionManager::MoveEntity (ent *entity)
	{
	bool succf = false;
	p_3d fpt = entity->u.sld.min;
	p_3d tpt;
	tpt.Set (fpt[0], fpt[1], fpt[2] + 40);
	translate_silent (&entity->eptr, true, 0, fpt, tpt, 1, 1, &succf);
	return succf;
	}



#pragma endregion


};
