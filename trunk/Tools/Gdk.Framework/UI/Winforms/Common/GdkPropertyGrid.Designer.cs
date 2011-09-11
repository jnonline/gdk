namespace Gdk.Forms
{
    partial class GdkPropertyGrid
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.lblDescription = new System.Windows.Forms.Label();
            this.lblDescriptionHeader = new System.Windows.Forms.Label();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.lblDescription);
            this.splitContainer1.Panel2.Controls.Add(this.lblDescriptionHeader);
            this.splitContainer1.Size = new System.Drawing.Size(308, 362);
            this.splitContainer1.SplitterDistance = 282;
            this.splitContainer1.TabIndex = 1;
            // 
            // lblDescription
            // 
            this.lblDescription.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lblDescription.Location = new System.Drawing.Point(3, 19);
            this.lblDescription.Name = "lblDescription";
            this.lblDescription.Size = new System.Drawing.Size(302, 56);
            this.lblDescription.TabIndex = 1;
            this.lblDescription.Text = "Description Text Here!";
            // 
            // lblDescriptionHeader
            // 
            this.lblDescriptionHeader.AutoSize = true;
            this.lblDescriptionHeader.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblDescriptionHeader.Location = new System.Drawing.Point(3, 4);
            this.lblDescriptionHeader.Name = "lblDescriptionHeader";
            this.lblDescriptionHeader.Size = new System.Drawing.Size(48, 13);
            this.lblDescriptionHeader.TabIndex = 0;
            this.lblDescriptionHeader.Text = "Header";
            // 
            // GdkPropertyGrid
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "GdkPropertyGrid";
            this.Size = new System.Drawing.Size(308, 362);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Label lblDescription;
        private System.Windows.Forms.Label lblDescriptionHeader;
    }
}
