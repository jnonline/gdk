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
using System.IO;

using Gdk;
using Gdk.Content;
using Gdk.Forms;

namespace Gdk.ContentStudio
{
    public partial class AssetEditor : UserControl
    {
        public AssetTag CurrentAssetTag;

        public Content.Content Content;
        public ContentForm OwnerContentForm;

        /// <summary>
        /// Constructor
        /// </summary>
        public AssetEditor()
        {
            InitializeComponent();

            // Bind to events on the property grid
            propertyGrid.PropertyValueChanged += new GdkPropertyGrid.GridPropertyEventHandler(propertyGrid_PropertyValueChanged);
            propertyGrid.PropertyCheckChanged += new GdkPropertyGrid.GridPropertyEventHandler(propertyGrid_PropertyCheckChanged);
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
        }

        /// <summary>
        /// Called by the outer form to tell this control to load details for a particular node
        /// </summary>
        /// <param name="nodeTag"></param>
        public void BindToAssetTag(AssetTag assetTag)
        {
            // Set the current asset tag
            this.CurrentAssetTag = assetTag;

            // Setup the Processors combo
            // ---------------------------------

            // Get the processors that support this extension
            string fileExtension = Path.GetExtension(assetTag.Asset.Name);
            fileExtension = fileExtension.TrimStart('.');
            List<ProcessorDetails> processorDetailList = ProcessorManager.GetProcessorsByFileExtension(fileExtension);

            // Load all the processors available to this asset into the combo box
            comboProcessor.Items.Clear();
            ProcessorDetails selectedProcessor = null;
            foreach (ProcessorDetails processorDetails in processorDetailList)
            {
                comboProcessor.Items.Add(processorDetails);
                if (processorDetails.Name == assetTag.Asset.ProcessorType)
                    selectedProcessor = processorDetails;
            }

            // Select the processor in use
            comboProcessor.SelectedItem = selectedProcessor;
        }

        /// <summary>
        /// Called when the user changes the processor type
        /// </summary>
        private void comboProcessor_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (CurrentAssetTag == null || comboProcessor.SelectedItem == null)
                return;

            // Get the selected processor
            ProcessorDetails selectedProcessor = comboProcessor.SelectedItem as ProcessorDetails;

            // Change the processor used for this asset
            CurrentAssetTag.Asset.ProcessorType = selectedProcessor.Name;

            // Change the image of the tree node for this asset
            CurrentAssetTag.Node.ImageIndex = CurrentAssetTag.Node.SelectedImageIndex = (int)selectedProcessor.Icon;

            // Setup the tooltip for the processor combo box
            toolTip.SetToolTip(comboProcessor, selectedProcessor.Description);

            // Rebind the property grid
            BindPropertyGrid();
        }

        /// <summary>
        /// Called to notify this control that the active bundle has changed
        /// </summary>
        /// 
        public void BundleChanged(Bundle bundle)
        {
            // Is this control not visible / enabled?
            if (this.Visible == false || this.Enabled == false)
                return;

            // If the base bundle is selected, allow the user to change the processor
            comboProcessor.Enabled = bundle == null;

            // Rebind the property grid
            BindPropertyGrid();
        }

        /// <summary>
        /// This method will load out the property grid
        /// </summary>
        private void BindPropertyGrid()
        {
            // Make sure we're in a valid state
            if (CurrentAssetTag == null || comboProcessor.SelectedItem == null)
                return;

            // Get the current processor
            ProcessorDetails selectedProcessor = comboProcessor.SelectedItem as ProcessorDetails;

            // Get the current bundle
            Bundle selectedBundle = OwnerContentForm.GetActiveBundle();

            // Are we editing a bundle or the base
            bool editingBase = selectedBundle == null;

            // Get the asset
            Asset asset = CurrentAssetTag.Asset;

            // Load all the properties for the given processor
            // ----------------------------------------------------

            // Create a list of properties for the property grid
            List<GridProperty> properties = new List<GridProperty>();
  
            // Loop through the parameters of the processor
            foreach (KeyValuePair<string, ProcessorParameter> pair in selectedProcessor.Parameters)
            {
                ProcessorParameter parameter = pair.Value;

                // Create a GridProperty that reflects this processor parameter
                GridProperty property = new GridProperty();
                property.Name = parameter.Name;
                property.Category = parameter.Category; 
                property.Description = parameter.Description;
                property.Type = parameter.Type;
                property.DefaultValue = parameter.DefaultValue.ToString();

                // Default the property value to the process default
                property.Value = property.DefaultValue;
                property.Checked = false;

                // Does the asset have a specific value?
                if (asset.ProcessorParameters.ContainsKey(parameter.Name))
                {
                    property.Value = asset.ProcessorParameters[parameter.Name];

                    // Are we editing the base?
                    if (editingBase == true)
                    {
                        // show the property as checked (has an override)
                        property.Checked = true;
                    }
                    else
                    {
                        // We're not editing the base, so show this as the 'base' / 'default' value
                        property.DefaultValue = property.Value;
                    }
                }
                
                // Are we editting a bundle?
                if (editingBase == false)
                {
                    // Get the AssetBundle we're editing
                    AssetBundle assetBundle = null;
                    if (asset.BundlesByName.ContainsKey(selectedBundle.Name))
                        assetBundle = asset.BundlesByName[selectedBundle.Name];

                    // Do we have a bundle and a corresponding value for this property? 
                    if (assetBundle != null && assetBundle.ProcessorParameters.ContainsKey(parameter.Name))
                    {
                        // Use the bundle override value
                        property.Value = assetBundle.ProcessorParameters[parameter.Name];
                        property.Checked = true;
                    }
                }

                // Add the property to the list
                properties.Add(property);
            }

            // Bind the properties to the property grid
            propertyGrid.DefaultValueColumnName = editingBase ? "Default Value" : "Base Value";
            propertyGrid.BindProperties(properties);
        }

        /// <summary>
        /// Called when the override checkbox in the property grid is checked
        /// </summary>
        void propertyGrid_PropertyCheckChanged(GridProperty property)
        {
            SetProperty(property);    
        }

        /// <summary>
        /// Called when the value in the property grid is changed
        /// </summary>
        void propertyGrid_PropertyValueChanged(GridProperty property)
        {
            SetProperty(property);
        }

        /// <summary>
        /// Apply the property to the asset
        /// </summary>
        void SetProperty(GridProperty property)
        {
            // Get the current bundle
            Bundle selectedBundle = this.OwnerContentForm.GetActiveBundle();

            // Get the asset
            Asset asset = CurrentAssetTag.Asset;

            // Are we editing the base?
            if (selectedBundle == null)
            {
                // BASE
                // ----------------------

                // Is the override checkbox clicked?
                if(property.Checked == true)
                {
                    // Set the value into the parameter set
                    asset.ProcessorParameters[property.Name] = property.Value;
                }
                else
                {
                    // Remove the property from the parameter set
                    if(asset.ProcessorParameters.ContainsKey(property.Name))
                        asset.ProcessorParameters.Remove(property.Name);
                }
            }
            else
            {
                // Bundle
                // ----------------------

                // Is the override checkbox clicked?
                if(property.Checked == true)
                {
                    // Does the asset have this bundle?
                    AssetBundle assetBundle = null;
                    if(asset.BundlesByName.ContainsKey(selectedBundle.Name))
                    {
                        // Get the bundle we're editing
                        assetBundle = asset.BundlesByName[selectedBundle.Name];
                    }
                    else
                    {
                        // Create the bundle
                        assetBundle = new AssetBundle();
                        assetBundle.Name = selectedBundle.Name;
                        asset.BundlesByName.Add(selectedBundle.Name, assetBundle);
                    }

                    // Set the value into the parameter set
                    assetBundle.ProcessorParameters[property.Name] = property.Value;
                }
                else
                {
                    // Does the asset have this bundle?
                    AssetBundle assetBundle = null;
                    if(asset.BundlesByName.ContainsKey(selectedBundle.Name))
                    {
                        // Get the bundle we're editing
                        assetBundle = asset.BundlesByName[selectedBundle.Name];

                        // Remove the property from the bundle's parameter set
                        if(assetBundle.ProcessorParameters.ContainsKey(property.Name))
                            assetBundle.ProcessorParameters.Remove(property.Name);

                        // Did we empty out the parameters for this bundle?
                        if(assetBundle.ProcessorParameters.Count == 0)
                        {
                            // Remove the bundle from this asset!
                            asset.BundlesByName.Remove(selectedBundle.Name);
                        }
                    }
                }
            }
        }

        
    }
}
