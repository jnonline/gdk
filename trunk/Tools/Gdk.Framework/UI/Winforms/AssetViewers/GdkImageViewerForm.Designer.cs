namespace Gdk.Forms
{
    partial class GdkImageViewerForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(GdkImageViewerForm));
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.statusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.cmbBackground = new System.Windows.Forms.ToolStripComboBox();
            this.butBGColor1 = new System.Windows.Forms.ToolStripButton();
            this.butBGColor2 = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.butShiftColors = new System.Windows.Forms.ToolStripButton();
            this.toolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.cmbScaleMode = new System.Windows.Forms.ToolStripComboBox();
            this.pictureBox1 = new Gdk.Forms.CustomPictureBox();
            this.statusStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusLabel1,
            this.statusLabel2});
            this.statusStrip1.Location = new System.Drawing.Point(0, 400);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(543, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // statusLabel1
            // 
            this.statusLabel1.Name = "statusLabel1";
            this.statusLabel1.Size = new System.Drawing.Size(72, 17);
            this.statusLabel1.Text = "statusLabel1";
            // 
            // statusLabel2
            // 
            this.statusLabel2.Name = "statusLabel2";
            this.statusLabel2.Size = new System.Drawing.Size(72, 17);
            this.statusLabel2.Text = "statusLabel2";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.cmbBackground,
            this.butBGColor1,
            this.butBGColor2,
            this.toolStripSeparator1,
            this.butShiftColors,
            this.toolStripSeparator2,
            this.toolStripLabel2,
            this.cmbScaleMode});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(543, 25);
            this.toolStrip1.TabIndex = 2;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(74, 22);
            this.toolStripLabel1.Text = "Background:";
            // 
            // cmbBackground
            // 
            this.cmbBackground.Name = "cmbBackground";
            this.cmbBackground.Size = new System.Drawing.Size(80, 25);
            this.cmbBackground.SelectedIndexChanged += new System.EventHandler(this.cmbBackground_SelectedIndexChanged);
            // 
            // butBGColor1
            // 
            this.butBGColor1.AutoSize = false;
            this.butBGColor1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.None;
            this.butBGColor1.Image = ((System.Drawing.Image)(resources.GetObject("butBGColor1.Image")));
            this.butBGColor1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.butBGColor1.Name = "butBGColor1";
            this.butBGColor1.Size = new System.Drawing.Size(30, 22);
            this.butBGColor1.Text = "toolStripButton1";
            this.butBGColor1.Click += new System.EventHandler(this.butBGColor1_Click);
            // 
            // butBGColor2
            // 
            this.butBGColor2.AutoSize = false;
            this.butBGColor2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.None;
            this.butBGColor2.Image = ((System.Drawing.Image)(resources.GetObject("butBGColor2.Image")));
            this.butBGColor2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.butBGColor2.Name = "butBGColor2";
            this.butBGColor2.Size = new System.Drawing.Size(30, 22);
            this.butBGColor2.Text = "toolStripButton2";
            this.butBGColor2.Click += new System.EventHandler(this.butBGColor2_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // butShiftColors
            // 
            this.butShiftColors.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.butShiftColors.Image = ((System.Drawing.Image)(resources.GetObject("butShiftColors.Image")));
            this.butShiftColors.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.butShiftColors.Name = "butShiftColors";
            this.butShiftColors.Size = new System.Drawing.Size(23, 22);
            this.butShiftColors.Text = "Shift Color Channels";
            this.butShiftColors.ToolTipText = "Shift Color Channels";
            this.butShiftColors.Click += new System.EventHandler(this.butShiftColors_Click);
            // 
            // toolStripLabel2
            // 
            this.toolStripLabel2.Name = "toolStripLabel2";
            this.toolStripLabel2.Size = new System.Drawing.Size(71, 22);
            this.toolStripLabel2.Text = "Scale Mode:";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // cmbScaleMode
            // 
            this.cmbScaleMode.Name = "cmbScaleMode";
            this.cmbScaleMode.Size = new System.Drawing.Size(90, 25);
            this.cmbScaleMode.SelectedIndexChanged += new System.EventHandler(this.cmbScaleMode_SelectedIndexChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox1.ImageDrawMode = System.Drawing.Drawing2D.InterpolationMode.Default;
            this.pictureBox1.Location = new System.Drawing.Point(6, 28);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(531, 365);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // GdkImageViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(543, 422);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.pictureBox1);
            this.Name = "GdkImageViewerForm";
            this.Text = "Form1";
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private CustomPictureBox pictureBox1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel2;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripButton butBGColor1;
        private System.Windows.Forms.ToolStripButton butBGColor2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton butShiftColors;
        private System.Windows.Forms.ToolStripComboBox cmbBackground;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripLabel toolStripLabel2;
        private System.Windows.Forms.ToolStripComboBox cmbScaleMode;
    }
}

