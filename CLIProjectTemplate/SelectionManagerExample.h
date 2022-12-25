// <copyright file="SelectionManagerExample.cpp" company="AVA">
// Copyright (c) 2022 - All rights reserved.
// </copyright>
// <summary>Declares the SelectionManager class</summary>

// This class demonstrates using Mastercam NET-Hook API and a C++ methods from a C++/CLI project.

#pragma once

namespace Mastercam::IO::Interop {

using namespace System;
using namespace System::Collections::Generic;

// Namespaces we reference in the NET-Hook API
using namespace Mastercam::App::Types;
using namespace Mastercam::Database;
using namespace Mastercam::Database::Types;
using namespace Mastercam::IO;
using namespace Mastercam::IO::Types;

/// <summary> The SelectionManager class. </summary>
public ref class SelectionManager
	{
public:
	/// <summary> This method create a geometry (circle). </summary>
	///
	/// <returns> ID of created geometry. </returns>
	static int SelectionManager::CreateGeometry ();

	/// <summary> This method retrieves the entity of the geometry and do a translation. </summary>
	///
	/// <param name="geom"> The geometry selected by the user. </param>
	///
	/// <returns> True if it succeeds, false if it fails. </returns>
	//static bool SelectionManager::TranslateSelectedGeometry (int geometryId);

	/// <summary> This method allows to move the entity of the geometry selected. </summary>
	///
	/// <returns> True if it succeeds, false if it fails. </returns>
	static bool SelectionManager::MoveEntity (ent *entity);

	static System::Collections::Generic::List<int>^ GetOperations(int opCode)
	{

		auto list = gcnew System::Collections::Generic::List<int>();
		for (auto index = 0; index < TpMainOpMgr.GetMainOpList().GetSize(); ++index)
		{
			auto op = TpMainOpMgr.GetMainOpList()[index];
			if (op && (opCode == TP_NULL || op->opcode == opCode))
			{
				list->Add(op->op_idn);
			}
		}

		return list;
	};
	static bool SelectionManager::AlterContourFinish(long opID, double maxStepdown, double minDepth, double maxDepth)
	{
		//Get
		prm_srf_common opCommon;
		prm_srf_depths opDepth;
		tp_entity thisToolPath;
		thisToolPath.op_idn = opID;

		//Set
		opCommon.prm_srf_common::max_stepdown = maxStepdown;
		opDepth.prm_srf_depths::min_depth = minDepth;
		opDepth.prm_srf_depths::max_depth = maxDepth;

		return true;
	};
	static bool SelectionManager::AlterPencilFinish(long opID, bool status, double limitOne, double limitTwo)
	{
		//Get
		prm_srf_limits opData;
		tp_entity thisToolPath;
		thisToolPath.op_idn = opID;

		//Set
		opData.prm_srf_limits::on = status;
		opData.prm_srf_limits::z1 = limitOne;
		opData.prm_srf_limits::z2 = limitTwo;

		return true;
	};
	};
}

