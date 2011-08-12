using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Gdk;
using Gdk.Content;

namespace Gdk.ContentStudio
{
    public partial class ParameterSetEditor : UserControl
    {
        private ParameterSet parameters;

        /// <summary>
        /// Constructor
        /// </summary>
        public ParameterSetEditor()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Loads the given parameters object into the control & edits it
        /// </summary>
        public void LoadParameters(ParameterSet parameters)
        {
            this.listView.Items.Clear();
            this.parameters = parameters;

            // Load all the parameters into the listview
            foreach (KeyValuePair<string, string> pair in parameters)
            {
                ListViewItem item = new ListViewItem(new string[] { pair.Value, pair.Key });
                listView.Items.Add(item);
            }
        }

        /// <summary>
        /// Called when the user clicks the "New" menu item over the parameters view
        /// </summary>
        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Display a textboxform to get the new parameter name
            TextBoxForm form = new TextBoxForm();
            form.Title = "New Parameter";
            form.Description = "Enter the unique name for the new parameter:";
            form.Value = "";
            form.DisallowedValues = new List<string>();
            foreach (KeyValuePair<string,string> pair in parameters)
                form.DisallowedValues.Add(pair.Key);

            // Show the dialog
            if (form.ShowDialog() == DialogResult.OK)
            {
                // Add a new item to the parameters & the list view
                parameters.Add(form.Value, "");
                listView.Items.Add(form.Value);
            }
        }

        /// <summary>
        /// Called when the user clicks the "New" menu item over the parameters view
        /// </summary>
        private void removeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Remove all the selected items
            while (listView.SelectedItems.Count > 0)
            {
                string paramName = listView.SelectedItems[0].Text;
                listView.SelectedItems[0].Remove();
                parameters.Remove(paramName);
            }
        }

        /// <summary>
        /// Called when the user tries to open the context menu
        /// </summary>
        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {
            
            // Show / Hide the menu items
            newToolStripMenuItem.Available = true;
            removeToolStripMenuItem.Available = listView.SelectedItems.Count > 0;
        }

        /// <summary>
        /// Called when the list view is resized (by window stretching
        /// </summary>
        private void listView_SizeChanged(object sender, EventArgs e)
        {
            chName.Width = listView.Width / 3;
            chValue.Width = listView.Width - chName.Width - 4;
        }

        /// <summary>
        /// Called after the user edits a label
        /// </summary>
        private void listView_AfterLabelEdit(object sender, LabelEditEventArgs e)
        {
            ListViewItem item = listView.Items[e.Item];
            parameters[item.SubItems[1].Text] = e.Label;
        }

        /// <summary>
        /// Called when the user clicks anywhere on the listview
        /// </summary>
        private void listView_MouseClick(object sender, MouseEventArgs e)
        {
            ListViewItem itemAtMouse = listView.GetItemAt(e.X, e.Y);
            if (itemAtMouse != null)
                itemAtMouse.Selected = true;
        }

    }
}
