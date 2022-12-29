// <copyright file="NETHookApiReflection.h" company="CNC Software, LLC.">
// Copyright (c) 2022 CNC Software, LLC. All rights reserved.
// </copyright>
// <summary>Declares the NETHookApiReflection class</summary>

#pragma once

namespace Mastercam::IO::Interop {

using namespace System;
using namespace System::Reflection;
using namespace Mastercam::Database;

public ref class NETHookApiReflection sealed
	{
public:

	/// <summary> Gets the name of the referenced assembly we're reflecting into </summary>
	static property String ^ReferencedAssemblyName
		{
		String ^get ();
		}

	/// <summary> Gets the NETHook assembly we are reflecting on. </summary>
	static property Assembly ^NetHookAssembly
		{
		Assembly ^get ();
		}

	/// <summary> Gets the field value of the specified type. </summary>
	///
	/// <param name="obj">		 The object. </param>
	/// <param name="fieldName"> The name of the field to retrieve. </param>
	///
	/// <returns> Nullptr if it fails, else the field value. </returns>
	static Pointer ^GetFieldValue (Object ^obj, String ^fieldName);

	/// <summary> Construct a NET-Hook Chain object from a native CHAIN pointer. </summary>
	///
	/// <param name="ptr"> The pointer to the native (linked-list) of chain(s). </param>
	///
	/// <returns> Nullptr if it fails, else a Mastercam::Database::Chain^. </returns>
	static Chain^ ConstructChain (IntPtr ptr);

private:

	/// <summary> Initializes static members of this sealed class. </summary>
	static NETHookApiReflection ()
		{
		}

	/// <summary> Gets an assembly. </summary>
	///
	/// <param name="referencedAssemblyName"> The name of the referenced assembly. </param>
	///
	/// <returns> nullptr if it fails, else the assembly. </returns>
	static Assembly ^GetAssembly (String ^referencedAssemblyName);

	/// <summary> The name of the assembly we're reflecting on. </summary>
	static String ^m_ReferencedAssemblyName;

	/// <summary> The assembly we're reflecting on. </summary>
	static Assembly ^m_NetHookApiAssembly;

	/// <summary> Find the NET-Hook API Chain constructor information via Reflection. </summary>
	///
	/// <returns> Nullptr if it fails, else a ConstructorInfo^. </returns>
	static ConstructorInfo^ ReflectChainConstructorInfo ();

	};
}

