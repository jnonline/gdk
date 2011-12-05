namespace Gdk.Forms
{
    partial class ColorSelector
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
            this.pictureBoxColor = new System.Windows.Forms.PictureBox();
            this.textColor = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxColor)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBoxColor
            // 
            this.pictureBoxColor.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)));
            this.pictureBoxColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBoxColor.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxColor.Name = "pictureBoxColor";
            this.pictureBoxColor.Size = new System.Drawing.Size(33, 16);
            this.pictureBoxColor.TabIndex = 0;
            this.pictureBoxColor.TabStop = false;
            this.pictureBoxColor.Click += new System.EventHandler(this.pictureBoxColor_Click);
            // 
            // textColor
            // 
            this.textColor.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textColor.Location = new System.Drawing.Point(40, 1);
            this.textColor.Margin = new System.Windows.Forms.Padding(0);
            this.textColor.Name = "textColor";
            this.textColor.Size = new System.Drawing.Size(100, 13);
            this.textColor.TabIndex = 1;
            this.textColor.TextChanged += new System.EventHandler(this.textColor_TextChanged);
            // 
            // ColorSelector
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.textColor);
            this.Controls.Add(this.pictureBoxColor);
            this.Name = "ColorSelector";
            this.Size = new System.Drawing.Size(142, 16);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxColor)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxColor;
        private System.Windows.Forms.TextBox textColor;
    }
}
