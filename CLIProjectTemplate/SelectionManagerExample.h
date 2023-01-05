// <copyright file="SelectionManagerExample.cpp" company="AVA">
// Copyright (c) 2022 - All rights reserved.
// </copyright>
// <summary>Declares the SelectionManager class</summary>

// This class demonstrates using Mastercam NET-Hook API and a C++ methods from a C++/CLI project.
#include "NETHookApiReflection.h"
#include "SolidsExtrude_CH.h"
#include "math2_ch.h"
#include "math1_ch.h"
#include "DbGet_CH.h"
#include "m_mill.h"
#include "cmaPoint2D_CH.h"
#include "BaseTypes_CH.h"

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
		static bool SelectionManager::DoSolidExtrudeCreate(Mastercam::Database::Chain^ chain, double distance)
		{
			auto result = false;

			if (chain)
			{
				// "convert" the managed Chain to a native CHAIN (pointer)
				auto pChain = GetNativeChain(chain);

				// "convert" the managed String to CString
				// If the supplied name is null or empty, initialize to a "blank" string
				System::String^ solidName;
				CString name = (!System::String::IsNullOrEmpty(solidName)) ? CString(solidName) : L"";

				// See SLD_EXTRUDE_PARAMS in sldTypes_CH.h
				// Starting in Mastercam 2022 the constructor for SLD_EXTRUDE_PARAMS initializes defaults.

				SLD_DRAFT_PARAMS draftParams;
				draftParams.add = false;
				SLD_THINWALL_PARAMS thinwallParams;
				thinwallParams.doThinWall = false;
				SLD_EXTRUDE_PARAMS extrudeParams;
				extrudeParams.creationType = 0;
				//extrudeParams.body = ?;
				extrudeParams.distMethod = 0;
				extrudeParams.distance = distance;
				//extrudeParams.local = ?;
				extrudeParams.direction = p_3di(1.0, 0.0, 0.0);
				extrudeParams.reverseDir = true;
				extrudeParams.bothDir = false;
				extrudeParams.splitDraft = false;
				extrudeParams.draftEnds = false;
				extrudeParams.flipOutward = false;
				extrudeParams.draft = draftParams;
				extrudeParams.thinwall = thinwallParams;

				//auto solidResult = sld_new_extrude(pChain,extrudeParams);
				result = sld_extrude_from_chain(pChain, extrudeParams, 0, nullptr, name.GetBuffer());
				name.ReleaseBuffer();
			}

			return result;
		}
		static bool SelectionManager::DoSolidExtrudeCut(Mastercam::Database::Chain^ chain, double distance, int geoID)
		{
			auto result = false;
			auto succf = true;


			if (chain)
			{


				// "convert" the managed Chain to a native CHAIN (pointer)
				auto pChain = GetNativeChain(chain);

				// "convert" the managed String to CString
				// If the supplied name is null or empty, initialize to a "blank" string
				System::String^ solidName;
				CString name = (!System::String::IsNullOrEmpty(solidName)) ? CString(solidName) : L"";

				// See SLD_EXTRUDE_PARAMS in sldTypes_CH.h
				// Starting in Mastercam 2022 the constructor for SLD_EXTRUDE_PARAMS initializes defaults.

				;

				SLD_DRAFT_PARAMS draftParams;
				draftParams.add = false;
				SLD_THINWALL_PARAMS thinwallParams;
				thinwallParams.doThinWall = false;
				SLD_EXTRUDE_PARAMS extrudeParams;

				DB_LIST_ENT_PTR body = nullptr;
				auto spEntity = std::make_unique<ent>();
				GetEntityByID(geoID, *spEntity, &succf);
				extrudeParams.body = spEntity->eptr;



				extrudeParams.creationType = 1;
				extrudeParams.distMethod = 0;
				extrudeParams.distance = distance;
				//extrudeParams.local = ?;
				extrudeParams.direction = p_3di(1.0, 0.0, 0.0);
				extrudeParams.reverseDir = true;
				extrudeParams.bothDir = false;
				extrudeParams.splitDraft = false;
				extrudeParams.draftEnds = false;
				extrudeParams.flipOutward = false;
				extrudeParams.draft = draftParams;
				extrudeParams.thinwall = thinwallParams;

				result = sld_extrude_from_chain(pChain, extrudeParams, 0, nullptr, name.GetBuffer());
				name.ReleaseBuffer();
			}

			return result;
		}
		static CHAIN* SelectionManager::GetNativeChain(Mastercam::Database::Chain^ chain)
		{
			System::Reflection::Pointer^ cPtr = NETHookApiReflection::GetFieldValue(chain, "Data");
			if (cPtr)
			{
				return static_cast<CHAIN*> (System::Reflection::Pointer::Unbox(cPtr));
			}

			return nullptr;
		}
		static int SelectionManager::Intersect(Mastercam::Database::Geometry^ GEO1, Mastercam::Database::Geometry^ GEO2) {

			EptrArray geomEptrs;
			EptrArray geomEptrs2;
			DB_LIST_ENT_PTR firstGeo;
			DB_LIST_ENT_PTR secondGeo;
			NoStackEnt(firstGeoID);
			NoStackEnt(secondGeoID);
			firstGeoID.ent_idn = GEO1->GetEntityID();
			secondGeoID.ent_idn = GEO1->GetEntityID();
			geomEptrs.Add(firstGeoID.eptr, UT_START);
			geomEptrs2.Add(secondGeoID.eptr, UT_START);
			ent freshNewGuy;
			get_ent_from_eptr(firstGeoID.eptr, &freshNewGuy);

			System::Windows::Forms::MessageBox::Show(freshNewGuy.id.ToString());

			std::vector<gt> firstEntity;
			ent firstNewGuy;
			if (get_ent_from_eptr(geomEptrs[UT_START], &firstNewGuy) == 0) {
				System::Windows::Forms::MessageBox::Show("ent found");
				if (firstNewGuy.id == L_ID) {
					gt tempEnt;
					tempEnt.id = 'L';
					tempEnt.u.li.e1 = firstNewGuy.u.li.e1.ConvertTo2d();
					tempEnt.u.li.e2 = firstNewGuy.u.li.e2.ConvertTo2d();
					firstEntity.push_back(tempEnt);
				}
				if (firstNewGuy.id == A_ID) {
					gt tempEnt;
					tempEnt.id = 'A';
					tempEnt.u.ar.c = firstNewGuy.u.ar.c.ConvertTo2d();
					tempEnt.u.ar.r = firstNewGuy.u.ar.r;
					tempEnt.u.ar.sa = firstNewGuy.u.ar.sa;
					tempEnt.u.ar.sw = firstNewGuy.u.ar.sw;
					firstEntity.push_back(tempEnt);
				}
			}

			std::vector<gt> secondEntity;
			ent secondNewGuy;
			if (get_ent_from_eptr(geomEptrs2[UT_START], &secondNewGuy) == 0) {
				if (secondNewGuy.id == L_ID) {
					gt tempEnt;
					tempEnt.id = 'L';
					tempEnt.u.li.e1 = secondNewGuy.u.li.e1.ConvertTo2d();
					tempEnt.u.li.e2 = secondNewGuy.u.li.e2.ConvertTo2d();
					secondEntity.push_back(tempEnt);
				}
				if (secondNewGuy.id == A_ID) {
					gt tempEnt;
					tempEnt.id = 'A';
					tempEnt.u.ar.c = secondNewGuy.u.ar.c.ConvertTo2d();
					tempEnt.u.ar.r = secondNewGuy.u.ar.r;
					tempEnt.u.ar.sa = secondNewGuy.u.ar.sa;
					tempEnt.u.ar.sw = secondNewGuy.u.ar.sw;
					secondEntity.push_back(tempEnt);
				}
			}

			// Find the insections b/w the lineEnt and the boundyEnts
			p_2di biasPt;
			for (const auto& firstEnt : firstEntity) {
				for (const auto& secondEnt : secondEntity) {
					p_2d intersectPts;
					short nIntersections = 0;
					bool success = false;

					ints_on_gt(&firstEnt, &secondEnt, biasPt, &intersectPts, &nIntersections, MTOL, &success);
					if (success) {
						auto newPointGeo = intersectPts;
						auto newPoint = gcnew Mastercam::BasicGeometry::PointGeometry();
						newPoint->Data.x = newPointGeo[0];
						newPoint->Data.y = newPointGeo[1];
						newPoint->Commit();
						return newPoint->GetEntityID();
					}
					else { return 0; };
				}
			}
		};
	};
}


