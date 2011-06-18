using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Gdk.Forms
{
    public partial class TextBoxForm : Form
    {
        public TextBoxForm()
        {
            InitializeComponent();

            // Default to "Cancel" result
            DialogResult = DialogResult.Cancel;
        }

        public string Title
        {
            get { return this.Text; }
            set { this.Text = value; }
        }

        public string Description
        {
            get { return this.labelContent.Text; }
            set { this.labelContent.Text = value; }
        }

        public string Value
        {
            get { return this.textBox1.Text; }
            set { this.textBox1.Text = value; }
        }

        /// <summary>
        /// If specifed, the entered text MUST fit one of these values
        /// </summary>
        public List<string> AllowedValues;

        /// <summary>
        /// If specifed, the entered text must NOT fit one of these values
        /// </summary>
        public List<string> DisallowedValues;

        /// <summary>
        /// If specified, the input value must be greater than or equal to this number of characters
        /// </summary>
        public int MinAllowedLength = 0;

        /// <summary>
        /// Called when the user pressed the "OK" button
        /// </summary>
        private void buttonOK_Click(object sender, EventArgs e)
        {
            if (this.Value.Length < MinAllowedLength)
            {
                MessageBox.Show(
                    "You must enter a value at least " + MinAllowedLength + " character(s) in length.",
                    "Value length too small",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning
                    );
                return;
            }

            // Validate Allowed Values
            if (AllowedValues != null)
            {
                foreach(string allowedValue in AllowedValues)
                    if (string.Equals(this.Value, allowedValue, StringComparison.OrdinalIgnoreCase))
                    {
                        MessageBox.Show(
                            "The entered value does not fit the list of allowed values.",
                            "Value not allowed",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Warning
                            );
                        return;
                    }
            }

            // Validate DisAllowed Values
            if (DisallowedValues != null)
            {
                foreach (string allowedValue in DisallowedValues)
                    if (string.Equals(this.Value, allowedValue, StringComparison.OrdinalIgnoreCase))
                    {
                        MessageBox.Show(
                            "The entered value is not allowed or is already in use.",
                            "Value not allowed",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Warning
                            );
                        return;
                    }
            }

            // Success!
            DialogResult = DialogResult.OK;
            Close();
        }

        /// <summary>
        /// Called when the user pressed the "OK" button
        /// </summary>
        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
