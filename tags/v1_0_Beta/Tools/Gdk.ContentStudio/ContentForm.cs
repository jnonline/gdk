/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using Gdk;
using Gdk.Content;
using Gdk.Forms;

namespace Gdk.ContentStudio
{
    public partial class ContentForm : Form
    {
        // *********************************************
        // Properties

        private Content.Content content;
        private bool inEditMode;

        // Build mode specific
        private Thread buildThread;
        private List<Asset> buildAssets;
        private Builder builder;
        private bool forceBuild;
        private int skippedAssets;
        private int failedAssets;
        private int warningAssets;
        private int successfulAssets;

        /// <summary>
        /// Constructor
        /// </summary>
        public ContentForm(Content.Content content)
        {
            InitializeComponent();

            // Set the content
            this.content = content;

            this.comboBundle.SelectedIndexChanged += new EventHandler(comboBundle_SelectedIndexChanged);
            this.contentSettings.OwnerContentForm = this;
            this.assetEditor.OwnerContentForm = this;
            this.assetsTree.OwnerContentForm = this;

            // Load the content into the sub controls
            this.assetsTree.LoadContent(content);
            this.assetEditor.Content = content;
            this.contentSettings.LoadContent(content);

            // Setup the bundle combobox
            this.RefreshBundleCombo();

            // Set the window title to the asset file
            this.Text = content.GetContentFilePath();

            // Set the form to Edit mode
            PerformChangeMode(true);

            // Init the web browser control
            SetWebBrowserHtml("First Load");
            webBrowser1.Navigating += new WebBrowserNavigatingEventHandler(webBrowser1_Navigating);

            // Clear the status text's for run mode
            statusLabel.Text = "Ready";
            statusProgressBar.Visible = false;
            statusSuccessLabel.Text = "";
            statusWarningLabel.Text = "";
            statusErrorLabel.Text = "";
        }

        /// <summary>
        /// Gets if the form is currently in "Edit" mode
        /// </summary>
        /// <returns></returns>
        public bool InEditMode
        { 
            get { return this.inEditMode; }
        }

        /// <summary>
        /// Gets if the form is currently building asset(s)
        /// </summary>
        /// <returns></returns>
        public bool IsBuilding
        {
            get { return this.buildThread != null; }
        }

        /// <summary>
        /// This method is called when an HREF link is clicked inside the web browser control
        /// </summary>
        void webBrowser1_Navigating(object sender, WebBrowserNavigatingEventArgs e)
        {
            // Is this a url to a gdk asset?
            if (e.Url.Scheme == "gdkasset")
            {
                // Get the path & type of this asset
                string assetPath = e.Url.LocalPath;

                // Use the proper viewer depending on the file specified
                Form viewerForm = Gdk.Forms.AssetViewerHelper.GetViewerFormForAssetFile(assetPath);
                if (viewerForm != null)
                {
                    // Show the form
                    viewerForm.Show(this);
                }

                // Cancel the actual navigation request
                e.Cancel = true;
            }
        }

        /// <summary>
        /// Called when the bundle combo box is changed
        /// </summary>
        void comboBundle_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Tell the asset editor about the change
            this.assetEditor.BundleChanged(GetActiveBundle());
        }

        /// <summary>
        /// Gets the active bundle according to the "Bundle" combo box
        /// </summary>
        /// <returns></returns>
        public Bundle GetActiveBundle()
        {
            // Is no bundle or the base bundle selected?
            if (comboBundle.SelectedIndex <= 0)
                return null;
            return comboBundle.SelectedItem as Bundle;
        }

        /// <summary>
        /// Refreshes the Bundle combo box with the content data
        /// </summary>
        public void RefreshBundleCombo()
        {
            // Get the current bundle selected in the combo
            Bundle selectedBundle =  comboBundle.SelectedItem as Bundle;
            int selectedIndex = comboBundle.SelectedIndex;

            // Clear & Rebuild the combo
            comboBundle.Items.Clear();
            comboBundle.Items.Add(new Bundle("-- BASE --"));
            foreach (Bundle bundle in content.Bundles)
                comboBundle.Items.Add(bundle);

            // If no bundle was selected before, select the base
            if(selectedIndex <= 0)
            {
                comboBundle.SelectedIndex = 0;
            }
            else
            {
                // Reselect the bundle with the matching name 
                foreach (Bundle bundle in comboBundle.Items)
                    if (bundle == selectedBundle)
                    {
                        comboBundle.SelectedItem = bundle;
                        break;
                    }
            }
        }

        #region Event Handlers

            #region Form

        /// <summary>
        /// Called when the form is about to be closed
        /// </summary>
        private void ContentForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Save any modified assets
            if (PromptToSaveChanges() == false)
            {
                // User cancelled, so cancel the close
                e.Cancel = true;
            }
        }

            #endregion

            #region Toolbar Buttons

        /// <summary>
        /// Called when the user presses the "Save" button
        /// </summary>
        private void buttonSave_Click(object sender, EventArgs e)
        {
            PerformSave();
        }

        /// <summary>
        /// Called when the user presses the "Switch to Build Mode" button
        /// </summary>
        private void buttonSwitchToBuildMode_Click(object sender, EventArgs e)
        {
            PerformChangeMode(false);
        }

        /// <summary>
        /// Called when the user presses the "Switch to Edit Mode" button
        /// </summary>
        private void buttonSwitchToEditMode_Click(object sender, EventArgs e)
        {
            PerformChangeMode(true);
        }

        /// <summary>
        /// Called when the user presses the "Go" button
        /// </summary>
        private void buttonBuild_Click(object sender, EventArgs e)
        {
            PerformBuild();
        }

        /// <summary>
        /// Called when the user presses the "Go" button
        /// </summary>
        private void buttonRebuildAll_Click(object sender, EventArgs e)
        {
            // Reset the content build
            PerformResetBuild();

            // Do the build
            PerformBuild();
        }

        /// <summary>
        /// Called when the user presses the "Clean" button
        /// </summary>
        private void buttonClean_Click(object sender, EventArgs e)
        {
            PerformClean();
        }

        /// <summary>
        /// Called when the user presses the "Stop" button
        /// </summary>
        private void buttonStop_Click(object sender, EventArgs e)
        {
            PerformBuildStop();
        }

        /// <summary>
        /// Called when the user presses the "Show All Files" button
        /// </summary>
        private void buttonShowAllFiles_Click(object sender, EventArgs e)
        {
            assetsTree.ShowAllFiles = !assetsTree.ShowAllFiles;
        }

            #endregion

            #region AssetTree events

        /// <summary>
        /// Called when the the list of selected nodes within the asset tree is changed
        /// </summary>
        private void assetsTree_SelectedNodesChanged()
        {
            // Get the first selected node
            if (assetsTree.SelectedNodes.Count == 0)
                return;
            MultiSelectTreeNode node = assetsTree.SelectedNodes[0];
            if (node == null || node.Tag == null || !(node.Tag is NodeTag))
                return;

            NodeTag nodeTag = node.Tag as NodeTag;

            // Update the asset type & name labels
            labelNodeType.Text = nodeTag.NodeType + ":";
            labelNodeName.Text = nodeTag.NodeName;

            // Are we in edit mode?
            if (inEditMode)
            {
                // Edit Mode
                // ---------------------

                // Did the user select an asset node?
                if (nodeTag is AssetTag)
                {
                    AssetTag assetTag = nodeTag as AssetTag;

                    // Tell the asset editor to bind to this asset
                    assetEditor.BindToAssetTag(assetTag);
                }

                // Show/Hide editor controls based on the selected node type
                assetEditor.Visible = nodeTag is AssetTag;
                contentSettings.Visible = nodeTag is ContentTag;
            }
            else
            {
                // Build Mode
                // ---------------------

                // Did the user select an asset node?
                if (nodeTag is AssetTag)
                {
                    AssetTag assetTag = nodeTag as AssetTag;

                    // Load the log into the display
                    SetWebBrowserHtml(assetTag.Log);
                }
            }
        }

            #endregion

        #endregion

        #region Utility Methods

        /// <summary>
        /// Updates the window title to reflect the current file & dirty state
        /// </summary>
        /// <returns>The method returns false if the user cancelled the save prompt.  Otherwise the method returns true</returns>
        bool PromptToSaveChanges()
        {
            // Display a Yes/No/Cancel prompt
            DialogResult result = MessageBox.Show(
                "Would you like to save your changes to:" + System.Environment.NewLine + System.Environment.NewLine + content.GetContentFilePath(), 
                "Save Changes?", 
                MessageBoxButtons.YesNoCancel, 
                MessageBoxIcon.Warning, 
                MessageBoxDefaultButton.Button1
                );

            if (result == DialogResult.Yes)
            {
                // Save the changes
                PerformSave();
                return true;
            }
            else if (result == DialogResult.No)
            {
                // Dont save
                return true;
            }
            
            // User cancelled
            return false;
        }

        /// <summary>
        /// Saves the current content to the active file
        /// </summary>
        public void PerformSave()
        {
            // Re-add all the assets to the content (this forces them to use the UI Tree ordering)
            this.assetsTree.ReAddAssetsToContent();

            // Save the content
            this.content.Save();
        }

        /// <summary>
        /// Sets the current mode 
        /// </summary>
        public void PerformChangeMode(bool editMode)
        {
            // Bail early if this already the current mode?
            if(editMode == inEditMode)
                return;

            // Set the new mode
            this.inEditMode = editMode;

            // Show / Hide the "Edit Mode" controls
            buttonSwitchToBuildMode.Visible = inEditMode;
            buttonShowAllFiles.Visible = inEditMode;
            assetEditor.Visible = inEditMode && assetsTree.IsAssetSelected;
            contentSettings.Visible = inEditMode && assetsTree.IsContentSelected;
            assetsTree.ShowAllFiles = inEditMode && buttonShowAllFiles.Checked;
            labelBundle.Visible = inEditMode;
            comboBundle.Visible = inEditMode;

            // Show / Hide the "Build Mode" controls
            buttonSwitchToEditMode.Visible = !inEditMode;
            buttonBuildDropDown.Visible = !inEditMode;
            buttonStop.Visible = !inEditMode;
            buttonBuildDropDown.Enabled = true;
            buttonStop.Enabled = false;
            webBrowser1.Visible = !inEditMode;
            statusStrip1.Visible = !inEditMode;


            // Are we entering Edit Mode?
            if (this.inEditMode)
            {
                // Entering Edit Mode
                // -------------------------

                // Reset all the colors & error text in the asset tree
                assetsTree.ResetNodeColors();
            }
            else
            {
                // Entering Build Mode
                // -------------------------

                // Save the current content
                PerformSave();
            }
        }

        #endregion

        #region Build Thread Methods

        /// <summary>
        /// Resets the content build, so all the assets in the content are rebuilt
        /// </summary>
        public void PerformResetBuild()
        {
            // Get the path to the dependency tracker file for this content
            string trackerFilePath = this.content.GetDependencyTrackerFilePath();

            // Delete the tracker file
            if(System.IO.File.Exists(trackerFilePath) == true)
                System.IO.File.Delete(trackerFilePath);
        }

        /// <summary>
        /// Cleans the content build folder
        /// </summary>
        public void PerformClean()
        {
            string buildPath = content.GetBuildFolder();

            // Show a confirmation dialog
            DialogResult result = MessageBox.Show(
                "Cleaning the content build will delete the build folder and ALL files & content within it." + System.Environment.NewLine + System.Environment.NewLine + 
                "Build Folder: " + buildPath + System.Environment.NewLine + System.Environment.NewLine + 
                "Are you sure you wish to do this?",
                "Confirm Clean",
                MessageBoxButtons.YesNo,
                MessageBoxIcon.Question,
                MessageBoxDefaultButton.Button2
                );
            if (result == DialogResult.No)
                return;

            // Delete the build folder
            try
            {
                System.IO.Directory.Delete(buildPath, true);
            }
            catch (Exception ex)
            {
                MessageBox.Show(
                    "Failed to clean the content: " + buildPath + System.Environment.NewLine + System.Environment.NewLine + ExceptionUtilities.GetExceptionText(ex),
                    "Failed to clean the content",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error
                    );
            }
        }

        /// <summary>
        /// Does a build of all the assets in the content
        /// </summary>
        public void PerformBuild()
        {
            PerformBuild(this.content.Assets, false);
        }

        /// <summary>
        /// Does a build of a set of assets
        /// </summary>
        public void PerformBuild(List<Asset> assets, bool forceBuild)
        {
            // Only 1 build thread at a time..
            if (buildThread != null)
                return;

            // Enable/Disable toolbar buttons
            buttonSave.Enabled = false;
            buttonBuildDropDown.Enabled = false;
            buttonStop.Enabled = true;
            buttonSwitchToEditMode.Enabled = false;

            // Setup the progress bar
            statusLabel.Text = "Starting...";
            statusSuccessLabel.Text = "";
            statusWarningLabel.Text = "";
            statusErrorLabel.Text = "";
            statusProgressBar.Minimum = 0;
            statusProgressBar.Maximum = content.Assets.Count;
            statusProgressBar.Value = 0;
            statusProgressBar.Visible = true;

            // Set the counters
            skippedAssets = 0;
            failedAssets = 0;
            warningAssets = 0;
            successfulAssets = 0;

            // Reset the node colors for all the assets
            foreach (Asset asset in assets)
            {
                AssetTag assetTag = asset.Tag as AssetTag;
                assetsTree.RecurseResetNodeColors(assetTag.Node);
            }

            // Create a thread to run the builder
            buildThread = new Thread(BuildWorker);
            this.forceBuild = forceBuild;

            // Copy the list assets to be built
            buildAssets = new List<Asset>();
            buildAssets.AddRange(assets);

            // Start the thread
            buildThread.IsBackground = true;
            buildThread.Start();
        }

        /// <summary>
        /// Stops an active build
        /// </summary>
        public void PerformBuildStop()
        {
            // Send a stop request to the builder
            if (builder != null)
                builder.RequestStop = true;
        }

        /// <summary>
        /// Called when the BuildWorker thread method is done
        /// </summary>
        private void BuildComplete()
        {
            // Enable/Disable various toolbar buttons
            buttonSave.Enabled = true;
            buttonBuildDropDown.Enabled = true;
            buttonStop.Enabled = false;
            buttonSwitchToEditMode.Enabled = true;
            statusProgressBar.Visible = false;

            // Was the build stopped by the user?
            if (builder.RequestStop == true)
                statusLabel.Text = "Build Stopped";
            else
                statusLabel.Text = "Build Complete!";
        }

        /// <summary>
        /// This is the main worker method of the BuildThread
        /// </summary>
        public void BuildWorker()
        {
            // Create a logger to track build events
            Logger logger = new Logger();
            logger.NewLogMessage += new LogMessageHandler(logger_NewLogMessage);
            logger.AssetStatusUpdated += new StatusUpdateHandler(logger_AssetStatusUpdated);

            // Create a builder & do the build!
            try
            {
                builder = new Builder();
                builder.Build(this.content, this.buildAssets, logger, forceBuild);
            }
            catch (Exception ex)
            {
                // Tell the UI thread to display the error
                this.Invoke(new Action<Exception>(DisplayBuildException), new object[] { ex });
            }
            finally
            {
                // Detach the logger
                logger.NewLogMessage -= new LogMessageHandler(logger_NewLogMessage);
                logger.AssetStatusUpdated -= new StatusUpdateHandler(logger_AssetStatusUpdated);
            }

            // Signal the "Build Complete" to the UI thread
            this.Invoke(new Action(BuildComplete));

            // We're done, null out the build thread
            buildThread = null;
            builder = null;
        }

        /// <summary>
        /// Displays the exception in a dialog box
        /// </summary>
        /// <param name="ex"></param>
        void DisplayBuildException(Exception ex)
        {
            // Display the build fail
            MessageBox.Show(
                "Failed to perform the content build" + System.Environment.NewLine + System.Environment.NewLine + ExceptionUtilities.GetExceptionText(ex),
                "Failed to build",
                MessageBoxButtons.OK,
                MessageBoxIcon.Error
                );
        }

        /// <summary>
        /// Called when the status of a particular asset is updated
        /// </summary>
        void logger_AssetStatusUpdated(BuildStatus status, Asset asset)
        {
            this.Invoke(new Action<BuildStatus, Asset>(SetAssetBuildStatus), new object[] { status, asset });
        }

        /// <summary>
        /// Updates the status of the given asset
        /// </summary>
        void SetAssetBuildStatus(BuildStatus status, Asset asset)
        {
            // Get the treenode for this asset
            AssetTag assetTag = asset.Tag as AssetTag;
            MultiSelectTreeNode node = assetTag.Node;

            // Change the node to reflect the status
            switch (status)
            {
                case BuildStatus.Waiting:
                    node.OriginalForeColor = System.Drawing.Color.DarkGray;
                    node.OriginalBackColor = SystemColors.Window;
                    break;
                case BuildStatus.Building:
                    node.OriginalForeColor = System.Drawing.Color.DarkBlue;
                    node.OriginalBackColor = System.Drawing.Color.LightSeaGreen;
                    break;
                case BuildStatus.Skipped:
                    node.OriginalForeColor = System.Drawing.Color.DarkBlue;
                    node.OriginalBackColor = SystemColors.Window;
                    skippedAssets++;
                    break;
                case BuildStatus.Success:
                    node.OriginalForeColor = System.Drawing.Color.DarkGreen;
                    node.OriginalBackColor = SystemColors.Window;
                    successfulAssets++;
                    break;
                case BuildStatus.SuccessWithWarning:
                    node.OriginalForeColor = System.Drawing.Color.Black;
                    node.OriginalBackColor = System.Drawing.Color.FromArgb(250, 190, 0);
                    warningAssets++;
                    break;
                case BuildStatus.Failed:
                    node.OriginalForeColor = System.Drawing.Color.LightYellow;
                    node.OriginalBackColor = System.Drawing.Color.DarkRed;
                    failedAssets++;
                    break;
            }

            // Track to this node
            node.EnsureVisible();

            // Calculate the number of assets that have completed
            int assetsCompleted = skippedAssets + successfulAssets + warningAssets + failedAssets;

            // Update the progress bar
            statusProgressBar.Minimum = 0;
            statusProgressBar.Maximum = buildAssets.Count;
            statusProgressBar.Value = assetsCompleted;
            statusProgressBar.Visible = assetsCompleted < content.Assets.Count;

            // Update the status text
            statusLabel.Text = string.Format("Building [{0}/{1}]", assetsCompleted, buildAssets.Count);

            // Update the success status
            if (successfulAssets > 0)
                statusSuccessLabel.Text = string.Format("[{0} Successful]", successfulAssets);
            else
                statusSuccessLabel.Text = "";

            // Update the warning status
            if (warningAssets > 0)
                statusWarningLabel.Text = string.Format("[{0} Warnings]", warningAssets);
            else
                statusWarningLabel.Text = "";

            // Update the error status
            if (failedAssets > 0)
                statusErrorLabel.Text = string.Format("[{0} Errors]", failedAssets);
            else
                statusErrorLabel.Text = "";

            // If this is the selected node, update the log display
            if (assetsTree.SelectedNodes.Count > 0 &&
                assetsTree.SelectedNodes[0] == assetTag.Node)
            {
                SetWebBrowserHtml(assetTag.Log);
            }
        }

        /// <summary>
        /// Called when a new log message is generated for an asset
        /// </summary>
        void logger_NewLogMessage(LogLevel level, string message, Asset asset)
        {
            this.Invoke(new Action<LogLevel, string, Asset>(AddAssetLogMessage), new object[] { level, message, asset });
        }

        /// <summary>
        /// Adds a log message to an asset
        /// </summary>
        void AddAssetLogMessage(LogLevel level, string message, Asset asset)
        {
            // Get the assetTag for this asset
            AssetTag assetTag = asset.Tag as AssetTag;

            // Turn the log message into html
            // -----------------------------------

            // Switch NewLine markers to <br/> & append a <br/>
            string fullMessage = 
                message.Replace(System.Environment.NewLine, "<br/>") + 
                "<br/>"
                ;

            // Prepend coloring & labels for warnings & errors
            if (level == LogLevel.Warning)
                fullMessage = "<FONT COLOR=\"#DB8700\">WARNING: " + fullMessage + "</FONT>";
            else if(level == LogLevel.Error)
                fullMessage = "<FONT COLOR=\"#B00000\">ERROR: " + fullMessage + "</FONT>";

            // Append the full message to the log
            assetTag.Log += fullMessage;

            // If this is the selected node, update the log display
            if (assetsTree.SelectedNodes.Count > 0 &&
                assetsTree.SelectedNodes[0] == assetTag.Node)
            {
                // Stop tracking log changes after the log gets bit
                if(assetTag.Log.Length < 10000)
                    SetWebBrowserHtml(assetTag.Log);
            }
        }

        /// <summary>
        /// This utility method is used to put html into the web browser
        /// </summary>
        void SetWebBrowserHtml(string html)
        {
            // Put the HTML into the web browser
            if (html.StartsWith("<HTML>") == false)
                webBrowser1.DocumentText = "<HTML><BODY><CODE>" + html + "</CODE></BODY></HTML>";
            else
                webBrowser1.DocumentText = html;
        }

        #endregion

        

    }
}
