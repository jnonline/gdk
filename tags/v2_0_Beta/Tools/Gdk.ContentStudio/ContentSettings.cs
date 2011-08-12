/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Gdk;
using Gdk.Content;

namespace Gdk.ContentStudio
{
    public partial class ContentSettings : UserControl
    {
        private Content.Content content;
        public ContentForm OwnerContentForm;

        /// <summary>
        /// Constructor
        /// </summary>
        public ContentSettings()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Loads all the control values with the settings for the given content
        /// </summary>
        public void LoadContent(Content.Content content)
        {
            this.content = content;

            // Load the Bundles into the listbox
            foreach (Bundle bundle in this.content.Bundles)
                this.listBoxBundles.Items.Add(bundle);
        }

        /// <summary>
        /// Called when a user presses the "Add" button next to the bundles listbox
        /// </summary>
        private void buttonAddBundle_Click(object sender, EventArgs e)
        {
            // Display a "New Platform" textboxform
            Gdk.Forms.TextBoxForm form = new Gdk.Forms.TextBoxForm();
            form.Title = "New Bundle";
            form.Description = "Enter the name for the new bundle:";
            form.Value = "";
            form.DisallowedValues = new List<string>();
            foreach(Bundle bundle in this.content.Bundles)
                form.DisallowedValues.Add(bundle.Name);

            // Show the dialog
            if (form.ShowDialog() == DialogResult.OK)
            {
                Bundle bundle = new Bundle(form.Value);
                this.content.Bundles.Add(bundle);
                this.listBoxBundles.Items.Add(bundle);

                // Select the new bundle
                this.listBoxBundles.SelectedItem = bundle;
            }

            // Refresh the main Bundle combo box
            OwnerContentForm.RefreshBundleCombo();
        }

        /// <summary>
        /// Called when a user presses the "Edit" button next to the bundles listbox
        /// </summary>
        private void buttonEditBundle_Click(object sender, EventArgs e)
        {
            // Get the selected platform
            if (this.listBoxBundles.SelectedItem == null)
                return;
            Bundle selectedBundle = this.listBoxBundles.SelectedItem as Bundle;
            int selectedIndex = this.listBoxBundles.SelectedIndex;
            string oldBundleName = selectedBundle.Name;

            // Display a "Edit Bundle" TextBoxForm
            Gdk.Forms.TextBoxForm form = new Gdk.Forms.TextBoxForm();
            form.Title = "Rename Bundle";
            form.Description = "New name of the bundle:";
            form.Value = selectedBundle.Name;
            form.DisallowedValues = new List<string>();
            foreach (Bundle bundle in this.content.Bundles)
                if(bundle != selectedBundle)
                    form.DisallowedValues.Add(bundle.Name);

            // Show the dialog
            if (form.ShowDialog() == DialogResult.OK)
            {
                // Update the bundle with the new name
                string newBundleName = form.Value;
                selectedBundle.Name = newBundleName;
                
                // Re-add the item to the combo box, to trigger a rebind to the name
                this.listBoxBundles.Items.RemoveAt(selectedIndex);
                this.listBoxBundles.Items.Insert(selectedIndex, selectedBundle);
                this.listBoxBundles.SelectedIndex = selectedIndex;

                // Refresh the main Bundle combo box
                OwnerContentForm.RefreshBundleCombo();

                // Update any assets that have settings for this bundle, to use the new bundle name
                foreach (Asset asset in content.Assets)
                {
                    if(asset.BundlesByName.ContainsKey(oldBundleName))
                    {
                        AssetBundle assetBundle = asset.BundlesByName[oldBundleName];
                        asset.BundlesByName.Remove(oldBundleName);
                        assetBundle.Name = newBundleName;
                        asset.BundlesByName.Add(assetBundle.Name, assetBundle);
                    }
                }
            }
        }

        /// <summary>
        /// Called when a user presses the "Remove" button next to the bundles listbox
        /// </summary>
        private void buttonRemoveBundle_Click(object sender, EventArgs e)
        {
            // Get the selected bundle
            if (this.listBoxBundles.SelectedItem == null)
                return;
            Bundle selectedBundle = this.listBoxBundles.SelectedItem as Bundle;

            // Show a Confirm dialog
            DialogResult confirmResult = MessageBox.Show(
                "Removing this bundle [" + selectedBundle.Name + "] will remove any customized settings for assets that build in this bundle.  Are you sure you want to do this?",
                "Confirm Bundle Removal",
                MessageBoxButtons.YesNo,
                MessageBoxIcon.Exclamation,
                MessageBoxDefaultButton.Button2
                );
            if (confirmResult == DialogResult.No)
                return;

            // Remove the selected bundle
            this.listBoxBundles.Items.Remove(selectedBundle);
            this.content.Bundles.Remove(selectedBundle);

            // Select the first bundle in the list
            if (this.listBoxBundles.Items.Count > 0)
                this.listBoxBundles.SelectedIndex = 0;

            // Refresh the main Bundle combo box
            OwnerContentForm.RefreshBundleCombo();

            // Delete any settings for this bundle across all the assets
            foreach (Asset asset in content.Assets)
            {
                if(asset.BundlesByName.ContainsKey(selectedBundle.Name))
                    asset.BundlesByName.Remove(selectedBundle.Name);
            }
            
        }
    }
}
