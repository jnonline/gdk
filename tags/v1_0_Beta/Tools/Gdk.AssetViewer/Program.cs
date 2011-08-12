/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Security;
using System.Security.Permissions;

using Gdk.Forms;

namespace Gdk.AssetViewer
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            // Setup winforms
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            // Are there any command line arguments?
            if (args.Length == 0)
            {
                // Register the file associations
                RegisterFileAssociations();
                return;
            }

            // Use the proper viewer depending on the file specified
            Form mainForm = Gdk.Forms.AssetViewerHelper.GetViewerFormForAssetFile(args[0]);
            if (mainForm == null)
            {
                
                MessageBox.Show(
                    "Unrecognized file type: " + System.IO.Path.GetExtension(args[0]), 
                    "Gdk.AssetViewer.exe", 
                    MessageBoxButtons.OK, 
                    MessageBoxIcon.Error
                    );
            }
            else
            {
                // Run the form
                Application.Run(mainForm);
            }
        }

        /// <summary>
        /// Registers the file association for this app
        /// </summary>
        static void RegisterFileAssociations()
        {
            RegistryPermission regPerms = new RegistryPermission(
                RegistryPermissionAccess.AllAccess,
               "HKEY_CURRENT_USER\\Software\\Classes"
               );
            regPerms.Assert();

            // Get the "HKEY_CURRENT_USER\Software\Classes" key
            RegistryKey hkcuSoftwareClasses = Registry.CurrentUser.OpenSubKey("Software\\Classes", true);

            // Register the file extensions
            // -------------------------------------

            // Create a child key for each file association
            RegistryKey gdkimageKey = hkcuSoftwareClasses.CreateSubKey(".gdkimage");
            gdkimageKey.SetValue(null, "gdkasset", RegistryValueKind.String);

            // Register the file types
            // -------------------------------------

            // Create the "gdkasset\shell\open\command" key
            RegistryKey openCommandKey = hkcuSoftwareClasses.CreateSubKey("gdkasset\\shell\\open\\command", RegistryKeyPermissionCheck.ReadWriteSubTree);
            gdkimageKey.SetValue(null, "gdkasset", RegistryValueKind.String);

            // Get the path to this application
            Assembly executingAssembly = System.Reflection.Assembly.GetExecutingAssembly();
            string fullExePath = executingAssembly.Location;

            // Setup this application for opening gdk assets
            openCommandKey.SetValue(null, "\"" + fullExePath + "\" \"%1\"", RegistryValueKind.String);

            // Display a confirmation dialog
            MessageBox.Show("Gdk Asset Viewer\n\nFile Associations Registered", "Gdk.AssetViewer.exe", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }
}
