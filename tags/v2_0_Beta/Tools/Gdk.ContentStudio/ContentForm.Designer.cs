namespace Gdk.ContentStudio
{
    partial class ContentForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ContentForm));
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.assetsTree = new Gdk.ContentStudio.AssetsTree();
            this.webBrowser1 = new System.Windows.Forms.WebBrowser();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.labelNodeType = new System.Windows.Forms.Label();
            this.labelNodeName = new System.Windows.Forms.Label();
            this.assetEditor = new Gdk.ContentStudio.AssetEditor();
            this.contentSettings = new Gdk.ContentStudio.ContentSettings();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.buttonSave = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.buttonSwitchToEditMode = new System.Windows.Forms.ToolStripButton();
            this.buttonSwitchToBuildMode = new System.Windows.Forms.ToolStripButton();
            this.buttonBuildDropDown = new System.Windows.Forms.ToolStripDropDownButton();
            this.buttonBuild = new System.Windows.Forms.ToolStripMenuItem();
            this.buttonRebuildAll = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.buttonClean = new System.Windows.Forms.ToolStripMenuItem();
            this.buttonStop = new System.Windows.Forms.ToolStripButton();
            this.buttonShowAllFiles = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.labelBundle = new System.Windows.Forms.ToolStripLabel();
            this.comboBundle = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripContainer2 = new System.Windows.Forms.ToolStripContainer();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusProgressBar = new System.Windows.Forms.ToolStripProgressBar();
            this.statusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusSuccessLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusWarningLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusErrorLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.toolStripContainer2.BottomToolStripPanel.SuspendLayout();
            this.toolStripContainer2.ContentPanel.SuspendLayout();
            this.toolStripContainer2.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer2.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.assetsTree);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.webBrowser1);
            this.splitContainer2.Panel2.Controls.Add(this.flowLayoutPanel1);
            this.splitContainer2.Panel2.Controls.Add(this.assetEditor);
            this.splitContainer2.Panel2.Controls.Add(this.contentSettings);
            this.splitContainer2.Size = new System.Drawing.Size(806, 473);
            this.splitContainer2.SplitterDistance = 273;
            this.splitContainer2.TabIndex = 0;
            // 
            // assetsTree
            // 
            this.assetsTree.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.assetsTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.assetsTree.Location = new System.Drawing.Point(0, 0);
            this.assetsTree.Name = "assetsTree";
            this.assetsTree.ShowAllFiles = false;
            this.assetsTree.Size = new System.Drawing.Size(273, 473);
            this.assetsTree.TabIndex = 1;
            this.assetsTree.SelectedNodesChanged += new System.Action(this.assetsTree_SelectedNodesChanged);
            // 
            // webBrowser1
            // 
            this.webBrowser1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.webBrowser1.Location = new System.Drawing.Point(1, 30);
            this.webBrowser1.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser1.Name = "webBrowser1";
            this.webBrowser1.Size = new System.Drawing.Size(528, 443);
            this.webBrowser1.TabIndex = 4;
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.flowLayoutPanel1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.flowLayoutPanel1.Controls.Add(this.labelNodeType);
            this.flowLayoutPanel1.Controls.Add(this.labelNodeName);
            this.flowLayoutPanel1.ForeColor = System.Drawing.SystemColors.HighlightText;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(1, 1);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(528, 28);
            this.flowLayoutPanel1.TabIndex = 3;
            this.flowLayoutPanel1.WrapContents = false;
            // 
            // labelNodeType
            // 
            this.labelNodeType.AutoSize = true;
            this.labelNodeType.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelNodeType.Location = new System.Drawing.Point(3, 0);
            this.labelNodeType.Name = "labelNodeType";
            this.labelNodeType.Padding = new System.Windows.Forms.Padding(0, 8, 0, 0);
            this.labelNodeType.Size = new System.Drawing.Size(42, 21);
            this.labelNodeType.TabIndex = 0;
            this.labelNodeType.Text = "Asset:";
            // 
            // labelNodeName
            // 
            this.labelNodeName.AutoSize = true;
            this.labelNodeName.Location = new System.Drawing.Point(51, 0);
            this.labelNodeName.Name = "labelNodeName";
            this.labelNodeName.Padding = new System.Windows.Forms.Padding(0, 8, 0, 0);
            this.labelNodeName.Size = new System.Drawing.Size(85, 21);
            this.labelNodeName.TabIndex = 1;
            this.labelNodeName.Text = "asset name here";
            // 
            // assetEditor
            // 
            this.assetEditor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.assetEditor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.assetEditor.Location = new System.Drawing.Point(1, 30);
            this.assetEditor.Name = "assetEditor";
            this.assetEditor.Size = new System.Drawing.Size(528, 443);
            this.assetEditor.TabIndex = 0;
            // 
            // contentSettings
            // 
            this.contentSettings.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.contentSettings.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.contentSettings.Location = new System.Drawing.Point(1, 30);
            this.contentSettings.Name = "contentSettings";
            this.contentSettings.Size = new System.Drawing.Size(528, 443);
            this.contentSettings.TabIndex = 0;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.buttonSave,
            this.toolStripSeparator1,
            this.buttonSwitchToEditMode,
            this.buttonSwitchToBuildMode,
            this.buttonBuildDropDown,
            this.buttonStop,
            this.buttonShowAllFiles,
            this.toolStripSeparator2,
            this.labelBundle,
            this.comboBundle});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(806, 25);
            this.toolStrip1.Stretch = true;
            this.toolStrip1.TabIndex = 1;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // buttonSave
            // 
            this.buttonSave.Image = ((System.Drawing.Image)(resources.GetObject("buttonSave.Image")));
            this.buttonSave.ImageTransparentColor = System.Drawing.Color.Black;
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(51, 22);
            this.buttonSave.Text = "Save";
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // buttonSwitchToEditMode
            // 
            this.buttonSwitchToEditMode.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.buttonSwitchToEditMode.Font = new System.Drawing.Font("Segoe UI", 9F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonSwitchToEditMode.Image = ((System.Drawing.Image)(resources.GetObject("buttonSwitchToEditMode.Image")));
            this.buttonSwitchToEditMode.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonSwitchToEditMode.Name = "buttonSwitchToEditMode";
            this.buttonSwitchToEditMode.Size = new System.Drawing.Size(139, 22);
            this.buttonSwitchToEditMode.Text = "Switch to Edit Mode";
            this.buttonSwitchToEditMode.Click += new System.EventHandler(this.buttonSwitchToEditMode_Click);
            // 
            // buttonSwitchToBuildMode
            // 
            this.buttonSwitchToBuildMode.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.buttonSwitchToBuildMode.Font = new System.Drawing.Font("Segoe UI", 9F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonSwitchToBuildMode.Image = ((System.Drawing.Image)(resources.GetObject("buttonSwitchToBuildMode.Image")));
            this.buttonSwitchToBuildMode.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonSwitchToBuildMode.Name = "buttonSwitchToBuildMode";
            this.buttonSwitchToBuildMode.Size = new System.Drawing.Size(146, 22);
            this.buttonSwitchToBuildMode.Text = "Switch to Build Mode";
            this.buttonSwitchToBuildMode.Click += new System.EventHandler(this.buttonSwitchToBuildMode_Click);
            // 
            // buttonBuildDropDown
            // 
            this.buttonBuildDropDown.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.buttonBuild,
            this.buttonRebuildAll,
            this.toolStripMenuItem1,
            this.buttonClean});
            this.buttonBuildDropDown.Image = ((System.Drawing.Image)(resources.GetObject("buttonBuildDropDown.Image")));
            this.buttonBuildDropDown.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonBuildDropDown.Name = "buttonBuildDropDown";
            this.buttonBuildDropDown.Size = new System.Drawing.Size(63, 22);
            this.buttonBuildDropDown.Text = "Build";
            // 
            // buttonBuild
            // 
            this.buttonBuild.Image = ((System.Drawing.Image)(resources.GetObject("buttonBuild.Image")));
            this.buttonBuild.Name = "buttonBuild";
            this.buttonBuild.Size = new System.Drawing.Size(167, 22);
            this.buttonBuild.Text = "Build Assets";
            this.buttonBuild.Click += new System.EventHandler(this.buttonBuild_Click);
            // 
            // buttonRebuildAll
            // 
            this.buttonRebuildAll.Image = ((System.Drawing.Image)(resources.GetObject("buttonRebuildAll.Image")));
            this.buttonRebuildAll.Name = "buttonRebuildAll";
            this.buttonRebuildAll.Size = new System.Drawing.Size(167, 22);
            this.buttonRebuildAll.Text = "Rebuild All Assets";
            this.buttonRebuildAll.Click += new System.EventHandler(this.buttonRebuildAll_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(164, 6);
            // 
            // buttonClean
            // 
            this.buttonClean.Image = ((System.Drawing.Image)(resources.GetObject("buttonClean.Image")));
            this.buttonClean.Name = "buttonClean";
            this.buttonClean.Size = new System.Drawing.Size(167, 22);
            this.buttonClean.Text = "Clean";
            this.buttonClean.Click += new System.EventHandler(this.buttonClean_Click);
            // 
            // buttonStop
            // 
            this.buttonStop.Image = ((System.Drawing.Image)(resources.GetObject("buttonStop.Image")));
            this.buttonStop.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonStop.Name = "buttonStop";
            this.buttonStop.Size = new System.Drawing.Size(51, 22);
            this.buttonStop.Text = "Stop";
            this.buttonStop.Click += new System.EventHandler(this.buttonStop_Click);
            // 
            // buttonShowAllFiles
            // 
            this.buttonShowAllFiles.CheckOnClick = true;
            this.buttonShowAllFiles.Image = ((System.Drawing.Image)(resources.GetObject("buttonShowAllFiles.Image")));
            this.buttonShowAllFiles.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonShowAllFiles.Name = "buttonShowAllFiles";
            this.buttonShowAllFiles.Size = new System.Drawing.Size(99, 22);
            this.buttonShowAllFiles.Text = "Show All Files";
            this.buttonShowAllFiles.Click += new System.EventHandler(this.buttonShowAllFiles_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // labelBundle
            // 
            this.labelBundle.Name = "labelBundle";
            this.labelBundle.Size = new System.Drawing.Size(47, 22);
            this.labelBundle.Text = "Bundle:";
            // 
            // comboBundle
            // 
            this.comboBundle.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBundle.Name = "comboBundle";
            this.comboBundle.Size = new System.Drawing.Size(121, 25);
            // 
            // toolStripContainer2
            // 
            // 
            // toolStripContainer2.BottomToolStripPanel
            // 
            this.toolStripContainer2.BottomToolStripPanel.Controls.Add(this.statusStrip1);
            // 
            // toolStripContainer2.ContentPanel
            // 
            this.toolStripContainer2.ContentPanel.Controls.Add(this.splitContainer2);
            this.toolStripContainer2.ContentPanel.Size = new System.Drawing.Size(806, 473);
            this.toolStripContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolStripContainer2.LeftToolStripPanelVisible = false;
            this.toolStripContainer2.Location = new System.Drawing.Point(0, 0);
            this.toolStripContainer2.Name = "toolStripContainer2";
            this.toolStripContainer2.RightToolStripPanelVisible = false;
            this.toolStripContainer2.Size = new System.Drawing.Size(806, 520);
            this.toolStripContainer2.TabIndex = 3;
            this.toolStripContainer2.Text = "toolStripContainer2";
            // 
            // toolStripContainer2.TopToolStripPanel
            // 
            this.toolStripContainer2.TopToolStripPanel.Controls.Add(this.toolStrip1);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.statusProgressBar,
            this.statusLabel,
            this.statusSuccessLabel,
            this.statusWarningLabel,
            this.statusErrorLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 0);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(806, 22);
            this.statusStrip1.TabIndex = 0;
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(42, 17);
            this.toolStripStatusLabel1.Text = "Status:";
            // 
            // statusProgressBar
            // 
            this.statusProgressBar.Name = "statusProgressBar";
            this.statusProgressBar.Size = new System.Drawing.Size(100, 16);
            // 
            // statusLabel
            // 
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(86, 17);
            this.statusLabel.Text = "status text here";
            // 
            // statusSuccessLabel
            // 
            this.statusSuccessLabel.ForeColor = System.Drawing.Color.Green;
            this.statusSuccessLabel.Name = "statusSuccessLabel";
            this.statusSuccessLabel.Size = new System.Drawing.Size(76, 17);
            this.statusSuccessLabel.Text = "compete text";
            // 
            // statusWarningLabel
            // 
            this.statusWarningLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(164)))), ((int)(((byte)(0)))));
            this.statusWarningLabel.Name = "statusWarningLabel";
            this.statusWarningLabel.Size = new System.Drawing.Size(72, 17);
            this.statusWarningLabel.Text = "warning text";
            // 
            // statusErrorLabel
            // 
            this.statusErrorLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.statusErrorLabel.Name = "statusErrorLabel";
            this.statusErrorLabel.Size = new System.Drawing.Size(80, 17);
            this.statusErrorLabel.Text = "error text here";
            // 
            // ContentForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(806, 520);
            this.Controls.Add(this.toolStripContainer2);
            this.Name = "ContentForm";
            this.ShowInTaskbar = false;
            this.Text = "Content";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ContentForm_FormClosing);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.ResumeLayout(false);
            this.flowLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.toolStripContainer2.BottomToolStripPanel.ResumeLayout(false);
            this.toolStripContainer2.BottomToolStripPanel.PerformLayout();
            this.toolStripContainer2.ContentPanel.ResumeLayout(false);
            this.toolStripContainer2.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer2.TopToolStripPanel.PerformLayout();
            this.toolStripContainer2.ResumeLayout(false);
            this.toolStripContainer2.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton buttonSave;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton buttonSwitchToBuildMode;
        private System.Windows.Forms.ToolStripContainer toolStripContainer2;
        private AssetsTree assetsTree;
        private AssetEditor assetEditor;
        private ContentSettings contentSettings;
        private System.Windows.Forms.ToolStripButton buttonStop;
        private System.Windows.Forms.ToolStripButton buttonShowAllFiles;
        private System.Windows.Forms.ToolStripButton buttonSwitchToEditMode;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.Label labelNodeType;
        private System.Windows.Forms.Label labelNodeName;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripProgressBar statusProgressBar;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel;
        private System.Windows.Forms.ToolStripStatusLabel statusErrorLabel;
        private System.Windows.Forms.WebBrowser webBrowser1;
        private System.Windows.Forms.ToolStripStatusLabel statusSuccessLabel;
        private System.Windows.Forms.ToolStripDropDownButton buttonBuildDropDown;
        private System.Windows.Forms.ToolStripMenuItem buttonRebuildAll;
        private System.Windows.Forms.ToolStripMenuItem buttonBuild;
        private System.Windows.Forms.ToolStripStatusLabel statusWarningLabel;
        private System.Windows.Forms.ToolStripComboBox comboBundle;
        private System.Windows.Forms.ToolStripLabel labelBundle;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem buttonClean;
    }
}

