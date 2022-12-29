// <copyright file="NETHookApiReflection.cpp" company="CNC Software, LLC.">
// Copyright (c) 2022 CNC Software, LLC. All rights reserved.
// </copyright>
// <summary>Implements the NETHookApiReflection class</summary>

#include "stdafx.h"
#include "NETHookApiReflection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Mastercam::IO::Interop {

#pragma region Properties (public)

	Assembly ^NETHookApiReflection::NetHookAssembly::get ()
		{
		if (m_NetHookApiAssembly == nullptr)
			{
			m_NetHookApiAssembly = GetAssembly (nullptr);
			}

		return m_NetHookApiAssembly;
		}

	System::String ^NETHookApiReflection::ReferencedAssemblyName::get ()
		{
		return m_ReferencedAssemblyName;
		}

#pragma endregion

#pragma region Methods (public)

	Chain^ NETHookApiReflection::ConstructChain (IntPtr ptr)
		{
		Chain^ chain = nullptr;

		if (ptr != IntPtr::Zero)
			{
			ConstructorInfo^ ChainConstructor = ReflectChainConstructorInfo();
			Object^ o = ChainConstructor->Invoke (gcnew array<Object^> { ptr });
			if (o != nullptr) // Should never be, but just in case...
				{
				chain = static_cast<Chain^> (o);
				}
			}

		return chain;
		}

#pragma endregion

#pragma region Methods (private)

	Pointer^ NETHookApiReflection::GetFieldValue (Object ^obj, String ^fieldName)
		{
		if (obj == nullptr)
			{
			throw gcnew ArgumentNullException ("obj");
			}

		if (String::IsNullOrWhiteSpace (fieldName))
			{
			throw gcnew ArgumentNullException ("fieldName");
			}

		BindingFlags bindingFlags =
			BindingFlags::Instance |
			BindingFlags::NonPublic;

		auto field = obj->GetType ()->GetField (fieldName, bindingFlags);

		if (field == nullptr)
			{
			throw gcnew ArgumentException ("fieldName", "No such field was found.");
			}

		auto v = field->GetValue (obj);
		return (Pointer^)v;
		}

	//=================================================================================================

	Assembly ^NETHookApiReflection::GetAssembly (String ^referencedAssemblyName)
		{
		try
			{
			// Use the default if we're not given anything.
			if (String::IsNullOrWhiteSpace (referencedAssemblyName))
				{
				referencedAssemblyName = gcnew String ("NETHook3_0");
				}

			m_ReferencedAssemblyName = referencedAssemblyName;
			Assembly ^assembly = Assembly::GetExecutingAssembly ();
			for each (auto assemblyName in assembly->GetReferencedAssemblies ())
				{
				if (String::Compare (assemblyName->Name, m_ReferencedAssemblyName, true) == 0)
					{
					// Load this Assembly so that we can reflect on it.
					return Assembly::Load (assemblyName->Name);
					}
				}
			}
		catch (Exception ^e)
			{
			::MessageBox (get_MainFrame ()->GetSafeHwnd (), CString (e->Message), _T ("NETHookApiReflection.GetAssembly"), MB_ICONERROR);
			}

		return nullptr; // Failure!
		}

	ConstructorInfo^ NETHookApiReflection::ReflectChainConstructorInfo ()
		{
		try
			{
			if (NetHookAssembly != nullptr)
				{
				auto classType = NetHookAssembly->GetType ("Mastercam.Database.Chain");
				if (classType != nullptr)
					{
					BindingFlags bindingFlags =
						BindingFlags::Instance |
						BindingFlags::NonPublic;

					auto constructors =	classType->GetConstructors (bindingFlags);
					return constructors[0];
					}

				}
			}
		catch (Exception ^e)
			{
			::MessageBox (get_MainFrame ()->GetSafeHwnd (), CString (e->Message), _T ("NETHookApiReflection.GetChainConstructorInfo"), MB_ICONERROR);
			}

		return nullptr; // Failure!
		}

#pragma endregion
	}

