// <copyright file="ResourceReaderService.cpp" company="AVA">
// Copyright (c) 2022 - All rights reserved.
// </copyright>
// <summary>Implements the ResourceReaderService class</summary>

#include "stdafx.h"
#include "ResourceReaderService.h"

namespace Services {

using namespace System;
using namespace System::Reflection;
using namespace System::Resources;
using namespace System::Text;
using namespace System::Windows::Forms;

String^ ResourceReaderService::GetStringFromResource (String^ name)
	{
	// We only create the ResourceManager once!
	// Here we look at the .resx resources in the assembly,
	// and create a ResourceManager for the one we find.
	if (rm == nullptr)
		{
		auto match = -1;

		auto assembly = Assembly::GetExecutingAssembly ();
		auto resNames = assembly->GetManifestResourceNames ();
		if (resNames->Length < 1)
			{
			return String::Empty;
			}

		auto resName = gcnew String ("");
		auto matchName = gcnew String ("CLIProjectTemplate");

		for (auto i = 0; i < resNames->Length; ++i)
			{
			if (String::CompareOrdinal (matchName, resNames[i]->Substring (0, resNames[i]->IndexOf ('.'))) == 0)
				{
				resName = resNames[i]->Substring (0, resNames[i]->LastIndexOf ('.'));
				break;
				}
			}

		if (String::IsNullOrWhiteSpace(resName))
			{
			return String::Format ("ERROR: Could not find the {0} resource!", matchName);
			}

		rm = gcnew ResourceManager (resName, assembly);
		if (rm == nullptr)
			{
			return "ERROR: Could not create the ResourceManager!";
			}
		}

	return rm->GetString (name);
	}

System::String^ ResourceReaderService::GetString (String^ name)
	{
	return ResourceReaderService::Instance ()->GetStringFromResource (name);
	}
}