
namespace Gdk.ContentStudio
{
    partial class AssetEditor
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
            this.comboProcessor = new System.Windows.Forms.ComboBox();
            this.labelProcessor = new System.Windows.Forms.Label();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.propertyGrid = new Gdk.Forms.GdkPropertyGrid();
            this.SuspendLayout();
            // 
            // comboProcessor
            // 
            this.comboProcessor.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.comboProcessor.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboProcessor.FormattingEnabled = true;
            this.comboProcessor.Location = new System.Drawing.Point(66, 3);
            this.comboProcessor.Name = "comboProcessor";
            this.comboProcessor.Size = new System.Drawing.Size(259, 21);
            this.comboProcessor.TabIndex = 3;
            this.comboProcessor.SelectedIndexChanged += new System.EventHandler(this.comboProcessor_SelectedIndexChanged);
            // 
            // labelProcessor
            // 
            this.labelProcessor.AutoSize = true;
            this.labelProcessor.Location = new System.Drawing.Point(3, 6);
            this.labelProcessor.Name = "labelProcessor";
            this.labelProcessor.Size = new System.Drawing.Size(57, 13);
            this.labelProcessor.TabIndex = 4;
            this.labelProcessor.Text = "Processor:";
            // 
            // propertyGrid
            // 
            this.propertyGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.propertyGrid.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.propertyGrid.Location = new System.Drawing.Point(6, 30);
            this.propertyGrid.Name = "propertyGrid";
            this.propertyGrid.Size = new System.Drawing.Size(319, 345);
            this.propertyGrid.TabIndex = 6;
            // 
            // AssetEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.propertyGrid);
            this.Controls.Add(this.labelProcessor);
            this.Controls.Add(this.comboProcessor);
            this.Name = "AssetEditor";
            this.Size = new System.Drawing.Size(331, 378);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboProcessor;
        private System.Windows.Forms.Label labelProcessor;
        private System.Windows.Forms.ToolTip toolTip;
        private Gdk.Forms.GdkPropertyGrid propertyGrid;
    }
}
