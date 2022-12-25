// <copyright file="ResourceReaderService.h" company="AVA">
// Copyright (c) 2022 CNC Software, LLC. All rights reserved.
// </copyright>
// <summary>Declares the ResourceReaderService class</summary>

#pragma once

#include "SingletonBehaviour.h"

namespace Services {

using namespace System;

public ref class ResourceReaderService : SingletonBehaviour<ResourceReaderService^>
    {
public:

    /// <summary> Gets a string from the assembly's .resx resources. </summary>
    ///
    /// <param name="name"> The name of the string resource to retrieve. </param>
    ///
    /// <returns> The string from the resource. </returns>
    static String^ GetString (String^ name);

private:

    /// <summary> Gets a string from the assembly's .resx resources. </summary>
    ///
    /// <param name="name"> The name of the string resource to retrieve. </param>
    ///
    /// <returns> The string from the resource. </returns>
    static String^ GetStringFromResource (String^ name);

    // <summary> The Resource Manager. </summary>
    static Resources::ResourceManager^ rm = nullptr;
    };

}