// <copyright file="SelectionManagerExample.cpp" company="AVA">
// Copyright (c) 2022 - All rights reserved.
// </copyright>
// <summary>Declares the SelectionManager class</summary>

// This class demonstrates using Mastercam NET-Hook API and a C++ methods from a C++/CLI project.
#include "m_mill.h"
#pragma comment(lib,"MCMill.lib")
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
		static int SelectionManager::CreateGeometry();

		/// <summary> This method retrieves the entity of the geometry and do a translation. </summary>
		///
		/// <param name="geom"> The geometry selected by the user. </param>
		///
		/// <returns> True if it succeeds, false if it fails. </returns>
		//static bool SelectionManager::TranslateSelectedGeometry (int geometryId);

		/// <summary> This method allows to move the entity of the geometry selected. </summary>
		///
		/// <returns> True if it succeeds, false if it fails. </returns>
		static bool SelectionManager::MoveEntity(ent* entity);

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
		}
		static bool SelectionManager::AlterContourFinish(double maxStepdown, double minDepth, double maxDepth)
		{

			for (auto index = 0; index < TpMainOpMgr.GetMainOpList().GetSize(); ++index)
			{
				auto op = TpMainOpMgr.GetMainOpList()[index];
				if (op && (op->opcode == TP_SRF_FIN_CONTOUR
					) && op->db.select_flag == true)
				{
					bool success = false;
					DB_LIST_ENT_PTR operationPointer = nullptr;
					//set cut method to zig zag
					op->u.r_contour.common.max_stepdown = maxStepdown;
					op->u.f_contour.depth.max_depth = maxDepth;
					op->u.f_contour.depth.min_depth = minDepth;
					operation_manager(op, OPMGR_REWRITE, &operationPointer, &success);
					if (success == false)
					{
						//TO DO : Handle any errors rewriting the operation
					}
				}
			}
			return true;
		}
		static bool SelectionManager::AlterPencilFinish(bool status, double limitOne, double limitTwo)
		{
			for (auto index = 0; index < TpMainOpMgr.GetMainOpList().GetSize(); ++index)
			{
				auto op = TpMainOpMgr.GetMainOpList()[index];
				if (op && (op->opcode == TP_SRF_FIN_PENCIL
					) && op->db.select_flag == true)
				{
					bool success = false;
					DB_LIST_ENT_PTR operationPointer = nullptr;
					//set cut method to zig zag
					op->u.f_pencil.limit.on = status;
					op->u.f_pencil.limit.z1 = limitOne;
					op->u.f_pencil.limit.z2 = limitTwo;
					operation_manager(op, OPMGR_REWRITE, &operationPointer, &success);
					if (success == false)
					{
						//TO DO : Handle any errors rewriting the operation
					}
				}
			}
			return true;
		};
	}
	;
}


