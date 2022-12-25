// <copyright file="SingletonBehaviour.cpp" company="AVA">
// Copyright (c) 2022 - All rights reserved.
// </copyright>
// <summary>Implements the SingletonBehaviour class</summary>

#include "stdafx.h"
#include "SingletonBehaviour.h"

namespace Services {

	T SingletonBehaviour<T>::Instance ()
		{
		return instance;
		}
}
