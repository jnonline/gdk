namespace Gdk.ContentStudio
{
    partial class AssetsTree
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AssetsTree));
            this.treeView = new Gdk.ContentStudio.MultiSelectTreeView();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.contextMenuInclude = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuExclude = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuBuild = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // treeView
            // 
            this.treeView.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.treeView.ContextMenuStrip = this.contextMenuStrip1;
            this.treeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeView.FullRowSelect = true;
            this.treeView.HideSelection = false;
            this.treeView.Location = new System.Drawing.Point(0, 0);
            this.treeView.Name = "treeView"; 
            this.treeView.Size = new System.Drawing.Size(188, 223);
            this.treeView.TabIndex = 0;
            this.treeView.SelectedNodesChanged += new System.Action(treeView_SelectedNodesChanged);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextMenuInclude,
            this.contextMenuExclude,
            this.contextMenuBuild});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(153, 92);
            this.contextMenuStrip1.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStrip1_Opening);
            // 
            // contextMenuInclude
            // 
            this.contextMenuInclude.Name = "contextMenuInclude";
            this.contextMenuInclude.Size = new System.Drawing.Size(152, 22);
            this.contextMenuInclude.Text = "Include";
            this.contextMenuInclude.Click += new System.EventHandler(this.contextMenuInclude_Click);
            // 
            // contextMenuExclude
            // 
            this.contextMenuExclude.Name = "contextMenuExclude";
            this.contextMenuExclude.Size = new System.Drawing.Size(152, 22);
            this.contextMenuExclude.Text = "Exclude";
            this.contextMenuExclude.Click += new System.EventHandler(this.contextMenuExclude_Click);
            // 
            // contextMenuBuild
            // 
            this.contextMenuBuild.Image = ((System.Drawing.Image)(resources.GetObject("contextMenuBuild.Image")));
            this.contextMenuBuild.Name = "contextMenuBuild";
            this.contextMenuBuild.Size = new System.Drawing.Size(152, 22);
            this.contextMenuBuild.Text = "Build";
            this.contextMenuBuild.Click += new System.EventHandler(this.contextMenuBuild_Click);
            // 
            // AssetsTree
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.treeView);
            this.Name = "AssetsTree";
            this.Size = new System.Drawing.Size(188, 223);
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Gdk.ContentStudio.MultiSelectTreeView treeView;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem contextMenuInclude;
        private System.Windows.Forms.ToolStripMenuItem contextMenuExclude;
        private System.Windows.Forms.ToolStripMenuItem contextMenuBuild;
    }
}
