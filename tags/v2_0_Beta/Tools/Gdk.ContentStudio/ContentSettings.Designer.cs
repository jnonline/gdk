namespace Gdk.ContentStudio
{
    partial class ContentSettings
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ContentSettings));
            this.buttonEditBundle = new System.Windows.Forms.Button();
            this.buttonRemoveBundle = new System.Windows.Forms.Button();
            this.buttonAddBundle = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.listBoxBundles = new System.Windows.Forms.ListBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonEditBundle
            // 
            this.buttonEditBundle.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonEditBundle.Image = ((System.Drawing.Image)(resources.GetObject("buttonEditBundle.Image")));
            this.buttonEditBundle.Location = new System.Drawing.Point(400, 49);
            this.buttonEditBundle.Name = "buttonEditBundle";
            this.buttonEditBundle.Size = new System.Drawing.Size(24, 24);
            this.buttonEditBundle.TabIndex = 11;
            this.buttonEditBundle.UseVisualStyleBackColor = true;
            this.buttonEditBundle.Click += new System.EventHandler(this.buttonEditBundle_Click);
            // 
            // buttonRemoveBundle
            // 
            this.buttonRemoveBundle.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonRemoveBundle.Image = ((System.Drawing.Image)(resources.GetObject("buttonRemoveBundle.Image")));
            this.buttonRemoveBundle.Location = new System.Drawing.Point(400, 79);
            this.buttonRemoveBundle.Name = "buttonRemoveBundle";
            this.buttonRemoveBundle.Size = new System.Drawing.Size(24, 24);
            this.buttonRemoveBundle.TabIndex = 10;
            this.buttonRemoveBundle.UseVisualStyleBackColor = true;
            this.buttonRemoveBundle.Click += new System.EventHandler(this.buttonRemoveBundle_Click);
            // 
            // buttonAddBundle
            // 
            this.buttonAddBundle.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonAddBundle.Image = ((System.Drawing.Image)(resources.GetObject("buttonAddBundle.Image")));
            this.buttonAddBundle.Location = new System.Drawing.Point(400, 19);
            this.buttonAddBundle.Name = "buttonAddBundle";
            this.buttonAddBundle.Size = new System.Drawing.Size(24, 24);
            this.buttonAddBundle.TabIndex = 9;
            this.buttonAddBundle.UseVisualStyleBackColor = true;
            this.buttonAddBundle.Click += new System.EventHandler(this.buttonAddBundle_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.listBoxBundles);
            this.groupBox1.Controls.Add(this.buttonRemoveBundle);
            this.groupBox1.Controls.Add(this.buttonEditBundle);
            this.groupBox1.Controls.Add(this.buttonAddBundle);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(430, 341);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Bundles";
            // 
            // listBoxBundles
            // 
            this.listBoxBundles.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.listBoxBundles.FormattingEnabled = true;
            this.listBoxBundles.Location = new System.Drawing.Point(6, 19);
            this.listBoxBundles.Name = "listBoxBundles";
            this.listBoxBundles.Size = new System.Drawing.Size(388, 303);
            this.listBoxBundles.TabIndex = 0;
            // 
            // ContentSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "ContentSettings";
            this.Size = new System.Drawing.Size(430, 341);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonAddBundle;
        private System.Windows.Forms.Button buttonRemoveBundle;
        private System.Windows.Forms.Button buttonEditBundle;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListBox listBoxBundles;


    }
}
