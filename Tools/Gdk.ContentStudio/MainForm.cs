using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

//using Gdk;
using Gdk.Content;

namespace Gdk.ContentStudio
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        #region Event Handlers

            #region Form

        /// <summary>
        /// Called when the application is about to be closed
        /// </summary>
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
        }

            #endregion

            #region File Menu

        /// <summary>
        /// Called when the user clicks File / New
        /// </summary>
        private void menuFileNew_Click(object sender, EventArgs e)
        {
            PerformNew();
        }

        /// <summary>
        /// Called when the user clicks File / Open
        /// </summary>
        private void menuFileOpen_Click(object sender, EventArgs e)
        {
            PerformOpen();
        }

        /// <summary>
        /// Called when the user clicks File / Save
        /// </summary>
        private void menuFileExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }

            #endregion

        #endregion

        #region Utility Methods

        /// <summary>
        /// Does the work of a File / New operation
        /// </summary>
        void PerformNew()
        {
            // Display a dialog to create a new file
            string ext = Content.Content.CONTENT_EXTENSION;
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Title = "Specify a new content file.";
            dialog.Filter = "content files (*." + ext + ")|*." + ext;
            dialog.FilterIndex = 1;
            dialog.RestoreDirectory = true;
            dialog.OverwritePrompt = true;
            dialog.DefaultExt = ext;
            if (dialog.ShowDialog() == DialogResult.Cancel)
            {
                // User cancelled
                return;
            }

            // Get the content root path & name
            string contentFilePath = dialog.FileName;
            string contentRootPath = System.IO.Path.GetDirectoryName(contentFilePath);
            string contentName = System.IO.Path.GetFileNameWithoutExtension(contentFilePath);

            // Create a new content
            Content.Content content = new Content.Content(contentName, contentRootPath);

            // Create a new ContentForm to manage the content
            ContentForm contentForm = new ContentForm(content);
            contentForm.MdiParent = this;
            contentForm.WindowState = FormWindowState.Maximized;
            contentForm.Show();
        }

        /// <summary>
        /// Does the work of a File / Open operation
        /// </summary>
        void PerformOpen()
        {
            // Display a dialog to open an existing file
            string ext = Content.Content.CONTENT_EXTENSION;
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "content files (*." + ext + ")|*." + ext;
            dialog.FilterIndex = 1;
            dialog.RestoreDirectory = true;
            dialog.Multiselect = false;
            dialog.CheckFileExists = true;
            dialog.DefaultExt = ext;
            if (dialog.ShowDialog() == DialogResult.Cancel)
            {
                // User cancelled
                return;
            }

            string contentFilePath = dialog.FileName;
            Content.Content content;

            // Load the content file
            try
            {
                content = Content.Content.FromFile(contentFilePath);
            }
            catch (Exception ex)
            {
                MessageBox.Show(
                    "Failed to load the file: " + System.Environment.NewLine + contentFilePath + System.Environment.NewLine + System.Environment.NewLine + ExceptionUtilities.GetExceptionText(ex),
                    "Failed to load the content file",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error
                    );
                return;
            }

            // Create afs new ContentForm to manage the content file
            ContentForm contentForm = new ContentForm(content);
            contentForm.MdiParent = this;
            contentForm.WindowState = FormWindowState.Maximized;
            contentForm.Show();
        }

        #endregion

        
    }
}
