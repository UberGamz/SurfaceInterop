// <copyright file="SingletonBehaviour.h" company="AVA">
// Copyright (c) 2022 - All rights reserved.
// </copyright>
// <summary>Declares the SingletonBehaviour class</summary>

#pragma once

namespace Services {

	/// <summary> The singleton behavior class. </summary>
	///
	/// <typeparam name="T"> Type of T. </typeparam>
	generic <class T> where T : ref class
	public ref class SingletonBehaviour
		{
		public:

		/// <summary> Gets the instance of type T </summary>
		static T Instance ()
			{
			if (m_Instance != nullptr && !m_Instance->Equals (nullptr))
				{
				return m_Instance;
				}

			m_Instance = System::Activator::CreateInstance<T> ();
			return m_Instance;
			}

		private:

		/// <summary> Backing field for the T Instance property. </summary>
		static T m_Instance;
		};
}
