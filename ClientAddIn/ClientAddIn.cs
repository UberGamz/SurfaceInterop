// <copyright file="ClientAddIn.cs" company="AVA">
// Copyright (c) 12/23/2022 All rights reserved.
// </copyright>
// <summary>
//.Implements the ClientAddIn class
//
// If this project is helpful please take a short survey at ->
// https://survey.alchemer.com/s3/6799376/Mastercam-API-Developer-Satisfaction-Survey
// </summary>

using Mastercam.App;
using Mastercam.App.Types;

namespace ClientAddIn
{
    /// <summary> Main class for this NET-Hook </summary>
    public class ClientAddIn : NetHook3App
    {
        /// <summary> The REQUIRED main entry point 'Run' method for a NETHook. </summary>
        ///
        /// <param name="param">System parameter.</param>
        ///
        /// <returns>A MCamReturn return type representing the outcome of the NetHook add-in.</returns>
        public override MCamReturn Run(int param)
        {



            return MCamReturn.NoErrors;
        }
    }
}
