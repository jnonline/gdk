/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;

using Gtk;

namespace Gdk.Framework.UI.GtkSharp
{
	[System.ComponentModel.ToolboxItem(true)]
	public partial class PropertyGrid : Gtk.Bin
	{		
		// Internal Properties
		int activeRowIndex = -1;
		Table propertiesTable;
		Dictionary<int, RowData> rowDataByRowIndex;
		GtmlTextBuffer propertyDescriptionBuffer;
		
		// Events
		public delegate void PropertyHandler(Property property);
		public event PropertyHandler PropertyValueChanged;
		
		// *******************************************************************
		public PropertyGrid ()
		{
			this.Build ();
			rootEventbox.ModifyBg(StateType.Normal, new Gdk.Color(250,250,250));
			
			// Setup a Gtml buffer on the property description
			propertyDescriptionBuffer = new GtmlTextBuffer();
			this.rootPropertyDescription.Buffer = propertyDescriptionBuffer;
		}
		
		// *******************************************************************
		public void Clear()
		{
			// Clear the root event box
			if(rootEventbox.Child != null)
				rootEventbox.Remove(rootEventbox.Child);
			
			// Clear the widget to property map
			activeRowIndex = -1;
			rowDataByRowIndex = new Dictionary<int, RowData>();
		}
		
		/// <summary>
		/// Binds to the grid to a list of properties.
		/// </summary>
		public void BindToPropertyList(List<Property> properties)
		{
			// Clear the grid first
			Clear();
			
			// Get a list of the unique category names (sorted)
			List<string> categories = properties
				.Select(n => n.Category)
				.Distinct()
				.OrderBy(n => n)
				.ToList();
			
			// Create a table to store the whole property grid
			uint numRows = (uint)categories.Count + (uint)properties.Count;
			uint numColumns = 3;
			propertiesTable = new Table(numRows, numColumns, false);
			rootEventbox.Add(propertiesTable);
			propertiesTable.RowSpacing = 1;
			
			// Loop through the categories
			uint rowIndex = 0;
			uint categoryIndex = 0;
			foreach(string category in categories)
			{
				// Create an event box to contain the category header
				EventBox categoryEventbox = new EventBox();
				
				// Add the category event box to a table row
				propertiesTable.Attach(categoryEventbox,
					0, numColumns + 1, 
					rowIndex, rowIndex + 1,
					AttachOptions.Fill | AttachOptions.Expand, AttachOptions.Fill,
					0, 0
					);
				
				categoryEventbox.Realize();
				categoryEventbox.ModifyBg(StateType.Normal, new Gdk.Color(196,196,196));
				categoryEventbox.ModifyBg(StateType.Prelight, new Gdk.Color(140,140,196));
				
				// Create a label for the Category header
				Label categoryLabel = new Label("<b>" + category + "</b>");
				categoryLabel.UseMarkup = true;
				categoryLabel.SetAlignment(0, 0.5f);
				categoryLabel.SetPadding(4, 4);
				categoryEventbox.Add(categoryLabel);
				
				// Next row
				rowIndex++;
							
				// Get the properties in this category
				List<Property> categoryProperties = properties
					.Where(n => n.Category == category)
					.OrderBy(n => n.Name)
					.ToList();
				
				// Add a seperator line
				propertiesTable.Attach(new VSeparator(), 
					1, 2, 
					rowIndex, (uint)categoryProperties.Count + rowIndex,
					AttachOptions.Fill, AttachOptions.Fill,
					0, 0
					);
				
				// Loop through the properties
				foreach(Property property in categoryProperties)
				{
					// Create an event box to contain the name label
					EventBox nameLabelEventBox = new EventBox();
					propertiesTable.Attach(nameLabelEventBox, 
							0, 1, 
							rowIndex, rowIndex+1,
							AttachOptions.Fill, AttachOptions.Fill,
							0, 0
							);
					nameLabelEventBox.Realize();
					nameLabelEventBox.ModifyBg(StateType.Normal, new Gdk.Color(250, 250, 250));
					
					// Add the property name to the table
					Label nameLabel = new Label("   " + property.Name);
					nameLabel.UseMarkup = true;
					nameLabel.SetAlignment(0, 0.5f);
					nameLabel.WidthRequest = 200;
					nameLabel.HeightRequest = 20;
					nameLabel.SetPadding(5,2);
					nameLabelEventBox.Add(nameLabel);
					
					// Bind to click events on the property name label
					nameLabelEventBox.ButtonReleaseEvent += HandleNameLabelButtonReleaseEvent;
					
					// Create an editor widget based on the property type
					// --------------------------------------------------
					
					Widget editorWidget = null;
					uint editorWidgetXPadding = 0;
					uint editorWidgetYPadding = 0;
					
					// ENUM
					if(property.Type.IsEnum)
					{
						// Build an array of the string values for this enum
						string[] items = Enum.GetNames(property.Type);
						
						// Create a ComboBox
						ComboBox combo = new ComboBox(items);
						combo.HeightRequest = 26;
						editorWidget = combo;
						combo.HasFrame = false;

						// Select the property value's entry
						combo.Active = 0;
						for(int i=0; i<items.Count(); i++)
						{
							if(string.CompareOrdinal(items[i], property.Value) == 0)
							{
								combo.Active = i;
								break;
							}
						}
						
						// Bind to events on the combo box
						combo.Changed += HandleComboBoxChanged;
					}
					
					// BOOL
					else if(property.Type == typeof(bool))
					{
						// Create a check box
						CheckButton check = new CheckButton();
						editorWidget = check;
						check.Active = Boolean.Parse(property.Value);
						check.DrawIndicator = true;
						check.Toggled += HandleCheckBoxToggled;
					}
					
					// COLOR
					else if(property.Type == typeof(Gdk.Framework.Color))
					{
						// Create a color selector
						ColorSelector colorSel = new ColorSelector();
						editorWidget = colorSel;
						Gdk.Framework.Color color;
						if(Gdk.Framework.Color.TryParse(property.Value, out color) == false)
							color = Gdk.Framework.Color.Purple;
						colorSel.Color = color;
						colorSel.ColorChanged += HandleColorSelectorColorChanged;
						
						editorWidgetXPadding = 2;
					}
					
					// STRING - default
					else
					{
						// Create an entry box
						Entry entry = new Entry(property.Value);
						editorWidget = entry;
						entry.Changed += HandleEntryChanged;
						entry.HasFrame = false;
					}
					
					// ----------------------
						
					// Add the editor widget to the end column of this row
					propertiesTable.Attach(editorWidget, 
						2, 3, rowIndex, rowIndex + 1,
						AttachOptions.Fill | AttachOptions.Expand, AttachOptions.Fill,
						editorWidgetXPadding, editorWidgetYPadding
						);
					
					// Bind to events on the editor widget
					editorWidget.FocusInEvent += HandleEditorWidgetFocusInEvent;
					if(editorWidget is Container)
					{
						(editorWidget as Container).FocusChildSet += HandleEditorWidgetChildFocusSet;
					}
					
					// Store information about this row
					RowData rowData = new RowData(property, nameLabel, nameLabelEventBox, editorWidget);
					rowDataByRowIndex.Add((int)rowIndex, rowData);
					
					rowIndex++;
				} // properties (in a category)
				
				categoryIndex++;
			} // categories
			
			// Tell the VBox to show all its new children
			rootEventbox.ShowAll();
		}
		
		// *******************************************************************
		void HandleEditorWidgetChildFocusSet (object o, FocusChildSetArgs args)
		{
			if(args.Widget != null)
			{
				// Get the index of the row this editor widget is in
				int row = GetTableWidgetRow(o as Widget);
				
				// Make this the active editting property
				SetActiveRow(row);
			}
		}
		
		// *******************************************************************
		void HandleEditorWidgetFocusInEvent (object o, FocusInEventArgs args)
		{
			// Get the index of the row this editor widget is in
			int row = GetTableWidgetRow(o as Widget);
			
			// Make this the active editting property
			SetActiveRow(row);
		}
		
		// *******************************************************************
		void SetActiveRow(int row)
		{
			// Is this row index already active?
			if(row == activeRowIndex)
				return;
			
			// De-activate the current row
			// ---------------------------
			
			if(activeRowIndex >= 0)
			{
				// Get the data / widgets for the current row
				RowData oldRow = rowDataByRowIndex[activeRowIndex];
				
				// De-activate the name label
				oldRow.NameLabelEventBox.ModifyBg(StateType.Normal, new Gdk.Color(250, 250, 250)); 
			}
			
			// Activate the new row
			// ---------------------------
			
			// This is the new active row
			activeRowIndex = row;
			
			if(row >= 0)
			{
				// Get the data / widgets for the current row
				RowData newRow = rowDataByRowIndex[row];
				
				// Activate the name label
				newRow.NameLabelEventBox.ModifyBg(StateType.Normal, new Gdk.Color(140, 140, 196)); 
				
				// Give focus to the edit control of this propery
				if(newRow.EditorWidget.HasFocus == false)
					newRow.EditorWidget.GrabFocus();
				
				// Set the property description text
				propertyDescriptionBuffer.Clear();
				propertyDescriptionBuffer.AppendGTML(
					"<size:2>" +
					"<b><u>Property:</u></b>\t\t" + newRow.Property.Name + "\n" + 
					"<b><u>Default Value:</u></b>\t" + newRow.Property.DefaultValue + "\n" +
					"<b><u>Description:</u></b>\t" + newRow.Property.Description +
					"</size>"
					);
			}
		}
		
		// *******************************************************************
		int GetTableWidgetRow(Widget widget)
		{
			// Get the row index this widget is attached to
			object rowIndexObj = propertiesTable.ChildGetProperty(widget, "top-attach").Val;
			uint rowIndex = (uint)rowIndexObj;
			return (int)rowIndex;
		}
		
		// *******************************************************************
		void HandleNameLabelButtonReleaseEvent (object o, ButtonReleaseEventArgs args)
		{
			// User clicked on a property name label
			
			// Get the index of the row this name label is in
			int row = GetTableWidgetRow(o as Widget);
			
			// Make this the active editting property
			SetActiveRow(row);
		}
		
		// *******************************************************************
		void HandleComboBoxChanged (object sender, EventArgs e)
		{
			// Get the row this editor widget belongs to
			ComboBox combo = sender as ComboBox;
			int row = GetTableWidgetRow(combo);
			RowData rowData = rowDataByRowIndex[row];
			
			// Update the property
			rowData.Property.Value = combo.ActiveText;
			
			// Fire the property changed event
			if(PropertyValueChanged != null)
				PropertyValueChanged(rowData.Property);
		}
		
		// *******************************************************************
		void HandleColorSelectorColorChanged (ColorSelector selector, Color color)
		{
			// Get the row this editor widget belongs to
			int row = GetTableWidgetRow(selector);
			RowData rowData = rowDataByRowIndex[row];
			
			// Update the property
			rowData.Property.Value = color.ToString();
			
			// Fire the property changed event
			if(PropertyValueChanged != null)
				PropertyValueChanged(rowData.Property);
		}
		
		// *******************************************************************
		void HandleCheckBoxToggled (object sender, EventArgs e)
		{
			// Get the row this editor widget belongs to
			CheckButton check = sender as CheckButton;
			int row = GetTableWidgetRow(check);
			RowData rowData = rowDataByRowIndex[row];
			
			// Update the property
			rowData.Property.Value = check.Active.ToString();
			
			// Fire the property changed event
			if(PropertyValueChanged != null)
				PropertyValueChanged(rowData.Property);
		}
		
		// *******************************************************************
		void HandleEntryChanged (object sender, EventArgs e)
		{
			// Get the row this editor widget belongs to
			Entry entry = sender as Entry;
			int row = GetTableWidgetRow(entry);
			RowData rowData = rowDataByRowIndex[row];
			
			// Do text validation based on the property type
			bool valueIsValid = false;
			if(rowData.Property.Type == typeof(float))
			{
				float floatValue;
				if(float.TryParse(entry.Text, out floatValue) == true)
				{
					valueIsValid = true;
				}
			}
			else if(rowData.Property.Type == typeof(int))
			{
				int intValue;
				if(int.TryParse(entry.Text, out intValue) == true)
				{
					valueIsValid = true;
				}
			}
			else if(rowData.Property.Type == typeof(short))
			{
				short shortValue;
				if(short.TryParse(entry.Text, out shortValue) == true)
				{
					valueIsValid = true;
				}
			}
			else if(rowData.Property.Type == typeof(byte))
			{
				byte byteValue;
				if(byte.TryParse(entry.Text, out byteValue) == true)
				{
					valueIsValid = true;
				}
			}
			if(rowData.Property.Type == typeof(Vector2))
			{
				Vector2 vec2Value;
				if(Vector2.TryParse(entry.Text, out vec2Value, false) == true)
				{
					valueIsValid = true;
				}
			}
			if(rowData.Property.Type == typeof(Vector3))
			{
				Vector3 vec3Value;
				if(Vector3.TryParse(entry.Text, out vec3Value, false) == true)
				{
					valueIsValid = true;
				}
			}
			
			// Is the current text a valid value?
			if(valueIsValid)
			{
				// Set the text color to black
				entry.ModifyText(StateType.Normal, new Gdk.Color(0,0,0));
				
				// Update the property
				rowData.Property.Value = entry.Text;
				
				// Fire the property changed event
				if(PropertyValueChanged != null)
					PropertyValueChanged(rowData.Property);
			}
			else
			{
				// Set the text color to Red (to signify it is invalid)
				entry.ModifyText(StateType.Normal, new Gdk.Color(196,0,0));
			}
		}
						
		// *******************************************************************
		internal class RowData
		{
			public Property Property;
			public Label NameLabel;
			public EventBox NameLabelEventBox;
			public Widget EditorWidget;
			
			// *******************************************************************
			public RowData(Property property, Label nameLabel, EventBox nameLabelEventBox, Widget editorWidget)
			{
				this.Property = property;
				this.NameLabel = nameLabel;
				this.NameLabelEventBox = nameLabelEventBox;
				this.EditorWidget = editorWidget;
			}
		}
		
	}
	
	/// <summary>
	/// Stores the information and value of a single property in a PropertyGrid
	/// </summary>
	public class Property
	{
        public string Name;
        public string Category;
        public string Description;
        public string Value;
        public string DefaultValue;
        public Type Type;
        public bool Checked;

        /// <summary>
        /// CTor
        /// </summary>
        public Property()
        {
        }

        /// <summary>
        /// CTor
        /// </summary>
        public Property(string name, string category, string description, bool isChecked, Type type, string value, string defaultValue)
        {
            this.Name = name;
            this.Category = category;
            this.Description = description;
			this.Checked = isChecked;
			this.Type = type;
			this.Value = value;
            this.DefaultValue = defaultValue;
        }
    }

}

