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
//#include "cmaPoint2D_CH.h"
//#include "BaseTypes_CH.h"
#include "Assoc_CH.h"
#include "m_chookapi.h"
#include "array"
#include "Chain_CH.h"
#include "OffsetChains_CH.H"
//#include "DbLoIo_ch.h"
//#include "3dvars_ch.h"


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

			p_2di biasPt;
			bool successful;
			auto firstEnt = std::make_unique<ent>();
			auto secondEnt = std::make_unique<ent>();
			GetEntityByID(GEO1->GetEntityID(), *firstEnt, &successful);//#include "Assoc_CH.h"
			GetEntityByID(GEO2->GetEntityID(), *secondEnt, &successful);//#include "Assoc_CH.h"

			std::vector<gt> firstEntity;//GT List of first entity
			ent firstNewGuy;
				if (firstEnt->id == L_ID) { // If first entity is a line
					gt tempEnt;
					//converts line data to GT data
					tempEnt.id = 'L';
					tempEnt.u.li.e1 = firstEnt->u.li.e1.ConvertTo2d();
					tempEnt.u.li.e2 = firstEnt->u.li.e2.ConvertTo2d();
					firstEntity.push_back(tempEnt);
				}
				if (firstEnt->id == A_ID) { // if first entity is an arc
					gt tempEnt;
					//converts arc data to GT data
					tempEnt.id = 'A';
					tempEnt.u.ar.c = firstEnt->u.ar.c.ConvertTo2d();
					tempEnt.u.ar.r = firstEnt->u.ar.r;
					tempEnt.u.ar.sa = firstEnt->u.ar.sa;
					tempEnt.u.ar.sw = firstEnt->u.ar.sw;
					firstEntity.push_back(tempEnt);
				}

			std::vector<gt> secondEntity;//GT List of second entity
			ent secondNewGuy;
				if (secondEnt->id == L_ID) { // If second entity is a line
					gt tempEnt;
					//converts line data to GT data
					tempEnt.id = 'L';
					tempEnt.u.li.e1 = secondEnt->u.li.e1.ConvertTo2d();
					tempEnt.u.li.e2 = secondEnt->u.li.e2.ConvertTo2d();
					secondEntity.push_back(tempEnt);
				}
				if (secondEnt->id == A_ID) { // If second entity is an arc
					gt tempEnt;
					//converts arc data to GT data
					tempEnt.id = 'A';
					tempEnt.u.ar.c = secondEnt->u.ar.c.ConvertTo2d();
					tempEnt.u.ar.r = secondEnt->u.ar.r;
					tempEnt.u.ar.sa = secondEnt->u.ar.sa;
					tempEnt.u.ar.sw = secondEnt->u.ar.sw;
					secondEntity.push_back(tempEnt);
				}

			for (const auto& firstEnt : firstEntity) {
				for (const auto& secondEnt : secondEntity) {
					p_2d intersectPts;
					short nIntersections = 0;
					bool success = false;

					ints_on_gt(&firstEnt, &secondEnt, biasPt, &intersectPts, &nIntersections, MTOL, &success);
					if (success) {
						auto newPointGeo = intersectPts;
						auto newPoint = gcnew Mastercam::BasicGeometry::PointGeometry(); // creates new point
						newPoint->Data.x = newPointGeo[0]; // pulls x cord from p_2d
						newPoint->Data.y = newPointGeo[1]; // pulls y cord from p_2d
						newPoint->Commit(); // saves new point to database
						return newPoint->GetEntityID(); // sends new point GeoID back to NETHook side
					}
					else { return NULL; };
				}
			}
			return successful;
		};

		static Mastercam::Database::Chain^ SelectionManager::ChainLinker (System::Collections::Generic::List<int>^ GeoList){


			bool successful;
			Mastercam::Database::Chain^ returnChain; // NETHook side chain
			CHAIN* tempChain; // CLI side chain
			ent entity; // blank entity
			std::vector<ent> entities; // Blank vector list needed for CreateChain method
			for (auto i=0;i<GeoList->Count;i++){
			GetEntityByID(GeoList[i], entity, &successful); // Gets ent value and assigns to entity
			
			entities.push_back(entity); // adds entity to vector list
			}
			tempChain = SelectionManager::CreateChain(entities); // Sends Entity List to chain creator (in order)
			returnChain = SelectionManager::GetNetChain(tempChain);
			
			return returnChain;
		}
		/// <summary> Construct a Chain from the supplied list of entities. </summary>
		/// <param name="entities"> The (in order!) line and/or arc entities. </param>
		/// <returns> The new chain if successful, else null. </returns>
		/// static CHAIN* SelectionManager::CreateChain(std::vector<ent>& entities){
		static CHAIN* SelectionManager::CreateChain(std::vector<ent>& entities)
		{
			CHAIN* chain;
			short err = alloc_chain(&chain, FALSE /* partial*/);
			bool result = (err == 0 && chain != nullptr);

			if (result)
			{
				CHAIN_ENT* last_chain_ent = nullptr;
				for (auto& entity : entities)
				{
					short errCode = add_curve_to_chain(&entity, 0, TRUE, 0, chain, &last_chain_ent);
					if (errCode != CHAIN_OK) // bail out !
					{
						result = false;
						break;
					}
				}
			}

			return (result ? chain : nullptr);
		}

		static Mastercam::Database::Chain^ SelectionManager::GetNetChain(CHAIN* chain)
		{
			auto newChains = gcnew List<Chain^>();

			//** you created a chain and now use it below
			if (chain)
			{
				for (auto currentChain = chain; currentChain != nullptr; currentChain = currentChain->next)
				{
					IntPtr chainPointer(currentChain);
					auto o = NETHookApiReflection::ConstructChain ( chainPointer );
					if (o != nullptr) // Should never be, but just in case...
					{
						newChains->Add(dynamic_cast<Chain^> (o));
					}
				}
			}

			return newChains[0];
		}
		static int SelectionManager::IntersectPoint(Mastercam::Database::Geometry^ GEO1, Mastercam::Database::Geometry^ GEO2) {
			p_2di biasPt;
			bool successful;
			auto firstEnt = std::make_unique<ent>();
			auto secondEnt = std::make_unique<ent>();
			GetEntityByID(GEO1->GetEntityID(), *firstEnt, &successful);//#include "Assoc_CH.h"
			GetEntityByID(GEO2->GetEntityID(), *secondEnt, &successful);//#include "Assoc_CH.h"

			std::vector<gt> firstEntity;//GT List of first entity
			ent firstNewGuy;
			if (firstEnt->id == L_ID) { // If first entity is a line
				gt tempEnt;
				//converts line data to GT data
				tempEnt.id = 'L';
				tempEnt.u.li.e1 = firstEnt->u.li.e1.ConvertTo2d();
				tempEnt.u.li.e2 = firstEnt->u.li.e2.ConvertTo2d();
				firstEntity.push_back(tempEnt);
			}

			std::vector<gt> secondEntity;//GT List of second entity
			ent secondNewGuy;
			if (secondEnt->id == L_ID) { // If second entity is a line
				gt tempEnt;
				//converts line data to GT data
				tempEnt.id = 'L';
				tempEnt.u.li.e1 = secondEnt->u.li.e1.ConvertTo2d();
				tempEnt.u.li.e2 = secondEnt->u.li.e2.ConvertTo2d();
				secondEntity.push_back(tempEnt);
			}

			for (auto& firstEnt : firstEntity) {
				for (auto& secondEnt : secondEntity) {
					p_2d intersectPts;
					long nIntersections;
					bool success = false;
					double tolerance = 2.0;

					ints_on_gt_new(&firstEnt, &secondEnt, biasPt, &intersectPts, &nIntersections, tolerance, &success);
					if (success) {
						auto newPointGeo = intersectPts;
						auto newPoint = gcnew Mastercam::BasicGeometry::PointGeometry(); // creates new point
						newPoint->Data.x = newPointGeo[0]; // pulls x cord from p_2d
						newPoint->Data.y = newPointGeo[1]; // pulls y cord from p_2d
						newPoint->Commit(); // saves new point to database
						return newPoint->GetEntityID(); // sends new point GeoID back to NETHook side
					}
					else { return NULL; };
				}
			}
			return NULL;
		}
		static Mastercam::Database::Chain^ SelectionManager::ChainOffsetWithResult(Mastercam::Database::Chain^ chain, double distance, int color, int level)
		{
			Mastercam::Database::Chain^ resultChain;
			if (chain)
			{
				auto pChain = GetNativeChain(chain);
				Cnc::XformOffsetChains::OffsetChainsParams offsetParams;
				Cnc::XformOffsetChains::ResultMethod resultMethod;
				//Cnc::XformOffsetChains::DepthMethod depthMethod;
				Cnc::XformOffsetChains::TranslateDirection translateDirection;
				//Cnc::XformOffsetChains::FilletCornerStyle filletCornerStyle;
				resultMethod = Cnc::XformOffsetChains::ResultMethod::Copy ;
				//depthMethod = Cnc::XformOffsetChains::DepthMethod::Incremental ;
				translateDirection = Cnc::XformOffsetChains::TranslateDirection::DefinedSide ;
				//filletCornerStyle = Cnc::XformOffsetChains::FilletCornerStyle::Sharp ;

				offsetParams.m_ResultMethod = resultMethod;            //!< ResultMethod
				offsetParams.m_Number = 1;                           //!< Number
				offsetParams.m_Distance = distance;                      //!< Distance
				//offsetParams.m_Depth = 0;                         //!< Depth
				//offsetParams.m_ContourDepth = 0;                  //!< Contour Depth
				//offsetParams.m_AbsoluteDepth = 0;                 //!< Absolute Depth
				//offsetParams.m_IncrementalDepth = 0;              //!< Incremental Depth
				//offsetParams.m_DepthMethod = depthMethod;              //!< Depth Method
				//offsetParams.m_Angle = 0;                         //!< Angle
				offsetParams.m_Direction = translateDirection;         //!< Translate Direction
				//offsetParams.m_FilletCorners = false;                   //!< Fillet Corners
				//offsetParams.m_FilletCornerStyle = filletCornerStyle;  //!< Fillet Corner Style
				//offsetParams.m_InfiniteLookAhead = false;               //!< Infinite Look Ahead
				//offsetParams.m_Tolerance = 0.001;                     //!< Tolerance
				//offsetParams.m_MaximumDepth = 0;                  //!< Maximum Depth
				//offsetParams.m_AutoPreview = false;                     //!< Auto Preview
				offsetParams.m_UseNewAttributeColor = true;            //!< true to use new color attribute
				offsetParams.m_UseNewAttributeLevel = true;            //!< true to use new level attribute
				offsetParams.m_ResultColor = color;                      //!< Result Color
				offsetParams.m_Level = level;                            //!< Level
				//offsetParams.m_SeparateByLevel = false;                 //!< Separate By Level

				EptrArray entityArray;
				List<int>^ GeoList;
				bool success = Cnc::XformOffsetChains::OffsetChains(pChain, offsetParams, entityArray);
				if (success == true) {
					if (entityArray.GetSize() > 1) {
						GeoList->Add(entityArray[1]->eptr->ent_idn);
						System::Windows::Forms::MessageBox::Show(GeoList[0].ToString());
					}
					//resultChain = ChainLinker(GeoList);
				}

			}
			return resultChain;
		}

		static bool SelectionManager::BreakAtPoints(Mastercam::Curves::ArcGeometry^ arc, Mastercam::BasicGeometry::PointGeometry^ point) {

			bool successful = false;
			p_2d pointy;
			auto firstEnt = std::make_unique<ent>();
			auto secondEnt = std::make_unique<ent>();
			GetEntityByID(arc->GetEntityID(), *firstEnt, &successful);//#include "Assoc_CH.h"
			GetEntityByID(point->GetEntityID(), *secondEnt, &successful);//#include "Assoc_CH.h"
			if (secondEnt->id == P_ID) {
				pointy = secondEnt->u.pt.ConvertTo2d();
			}
			std::vector<gt> firstEntity;
			if (firstEnt->id == A_ID) {
				gt tempEnt;
				tempEnt.id = 'A';
				tempEnt.u.ar.c = firstEnt->u.ar.c.ConvertTo2d();
				tempEnt.u.ar.r = firstEnt->u.ar.r;
				tempEnt.u.ar.sa = firstEnt->u.ar.sa;
				tempEnt.u.ar.sw = firstEnt->u.ar.sw;
				firstEntity.push_back(tempEnt);
			}
			for (auto& firstEnt : firstEntity) {
				successful = pt_on_gt(pointy, &firstEnt);
				if (successful == true) {
					return successful;
							
				}
			}
			return successful;
		}
	};
}


