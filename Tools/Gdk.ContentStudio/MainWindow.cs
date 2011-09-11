using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection; 
using System.Text;
using System.Threading;
using Gtk;
using Gdk.Framework.Content;
using Gdk.Framework.UI.GtkSharp;

public partial class MainWindow : Gtk.Window
{	
	// Properties
	private Asset activeAsset;
	private TreeStore assetsTreeStore;
	private Bundle bundle;
	
	
	// Properties for logs
	private int buildNumAssetsSuccessful;
	private int buildNumAssetsSuccessWithWarnings;
	private int buildNumAssetsFailed;
	private int buildNumAssetsSkipped;
	private ListStore buildErrorsListStore;
	private ListStore buildOutputsListStore;
	private int buildErrorsCount;
	private int buildWarningsCount;
	private int buildOutputsCount;
	
	private Gdk.Cursor handCursor;
	private Gdk.Cursor textCursor;
	private bool hoveringOverLinkInAssetLog = false;
		
	// Properties used by the async build system
	private Thread buildThread;
	private ContentBuilder builder;
	private Bundle buildBundle;
	private bool buildForceRebuild;
		
	/// <summary>
	/// Constructor
	/// </summary>
	public MainWindow (): base (Gtk.WindowType.Toplevel)
	{
		Build ();
			
		// Set the window icon
		this.Icon = Gdk.Framework.IconsCache.GetIconPixbuf(Gdk.Framework.Icons.Package);
		
		// Setup the assets tree
		AssetsTree_Setup();
		
		// Setup the Asset details views
		this.handCursor = new Gdk.Cursor(Gdk.CursorType.Hand2);
		this.textCursor = new Gdk.Cursor(Gdk.CursorType.Xterm);
		
		this.assetNotebook.Page = 0;
		BuildAssetLog_Setup();
		
		// Setup the build logs & views
		BuildLog_Setup();
		BuildErrors_Setup();
		BuildOutputs_Setup();
		
		// Update the UI control states based on the app state
		UpdateAppUIState();
		
		assetProcessorCombo.AppendText("Item 1");
		assetProcessorCombo.AppendText("Item 2");
	}
	
	// **********************************************************************
	protected void Window_DeleteEvent (object sender, DeleteEventArgs a)
	{
		Application.Quit ();
		a.RetVal = true;
	}
	
	// **********************************************************************
	protected void Toolbar_New_Click (object sender, System.EventArgs e)
	{
		// Create a File Chooser Dialog
		FileChooserDialog dlg = new Gtk.FileChooserDialog(
			"Create a new file for this GDK content bundle",
            this,
            FileChooserAction.Save,
            "Cancel", ResponseType.Cancel,
            "Ok", ResponseType.Ok
			);
		
		// Setup the dialog filters
		dlg.Filter = new FileFilter();
		dlg.Filter.AddPattern("*.bundle");
		dlg.Filter.Name = "GDK Bundle Files (*.bundle)";

		// Show the dialog
		int response = dlg.Run();
		string filename = dlg.Filename;
		dlg.Destroy();
		
		// Did the user click OK on the dialog?
		if (response == (int)ResponseType.Ok) 
		{
			filename = filename.Replace('\\', '/');
			
			// Create a new asset bundle
			string bundleName = System.IO.Path.GetFileNameWithoutExtension(filename);
			string bundleRootPath = System.IO.Path.GetDirectoryName(filename).Replace('\\','/');
			this.bundle = new Bundle(bundleName, bundleRootPath);
			
			// Bind to the bundle
			BindToBundle();
		}
		
		// Update the UI control states based on the app state
		UpdateAppUIState();
	}
	
	// **********************************************************************
	protected void Toolbar_Open_Click (object sender, System.EventArgs e)
	{
		// Create a File Chooser Dialog
		FileChooserDialog dlg = new Gtk.FileChooserDialog(
			"Select a GDK content bundle file",
            this,
            FileChooserAction.Open,
            "Cancel", ResponseType.Cancel,
            "Open", ResponseType.Ok
			);
		
		// Setup the filters
		dlg.Filter = new FileFilter();
		dlg.Filter.AddPattern("*.bundle");
		dlg.Filter.Name = "GDK Bundle Files (*.bundle)";
		
		// Show the dialog
		int response = dlg.Run();
		string filename = dlg.Filename;
		dlg.Destroy();
		
		// Did the user click OK on the dialog?
		if (response == (int)ResponseType.Ok) 
		{
			filename = filename.Replace('\\', '/');
			
			// Load the bundle file
			try
			{
				this.bundle = Bundle.FromFile(filename);
				
				// Bind to the bundle
				BindToBundle();	
			}
			catch(Exception ex)
			{
				// An error occured loading the bundle, do nothing...
				MessageDialog md = new MessageDialog(
					this, DialogFlags.Modal, MessageType.Error, ButtonsType.Ok, 
					"Unable to load the selected bundle file:" + System.Environment.NewLine + System.Environment.NewLine + Gdk.Framework.ExceptionUtilities.GetExceptionText(ex)
					);
            	md.Run ();
            	md.Destroy();
			}
		}
		
		// Update the UI control states based on the app state
		UpdateAppUIState();
	}
	
	// **********************************************************************
	protected void Toolbar_Save_Click (object sender, System.EventArgs e)
	{
		// Save the bundle
		this.bundle.Save();
	}
	
	// **********************************************************************
	protected void Toolbar_Build_Click (object sender, System.EventArgs e)
	{
		// Start a build of the full bundle
		Build_Start(bundle, false);
	}
	
	// **********************************************************************
    protected void Toolbar_Rebuild_Click (object sender, System.EventArgs e)
	{
		// Start a re-build of the full bundle
		Build_Start(bundle, true);
	}
	
	// **********************************************************************
    protected void Toolbar_Clean_Click (object sender, System.EventArgs e)
	{
		Build_DoClean();
	}
	
	// **********************************************************************
	protected void Toolbar_Stop_Click (object sender, System.EventArgs e)
	{
		// Is there an actve build thread?
		if(this.buildThread != null)
		{
			// Kill the build thread
			this.buildThread.Abort();
			
			// Null out the build data
            this.buildThread = null;
            this.buildBundle = null;
			
			// Log that the build was stopped by the user
			BuildLog_WriteLine("\n<color:#C05000>Build Canceled by User</color>");

            // Update the UI State
            UpdateAppUIState();
		}
	}
	
	// **********************************************************************
	public event Action<Asset> ActiveAssetChanging;
	public event System.Action ActiveAssetChanged;
	
	// **********************************************************************
	public Asset ActiveAsset
	{
		get { return this.activeAsset; }
		set 
		{
			// Bail early if the asset isnt different
			if(this.activeAsset == value)
				return;
			
			// Raise the "ActiveAssetChanging" event, prior to changing
			ActiveAsset_OnChanging();
			if(ActiveAssetChanging != null)
				ActiveAssetChanging(value);
			
			// Change the asset
			this.activeAsset = value;
			
			// Raise the "ActiveAssetChanged" event, now that we've changed
			ActiveAsset_OnChanged();
			if(ActiveAssetChanged != null)
				ActiveAssetChanged();
		}
	}
	
	// **********************************************************************
	public void ActiveAsset_OnChanging()
	{
		
	}
	
	// **********************************************************************
	public void ActiveAsset_OnChanged()
	{
		// Was the active asset nulled?
		if(this.ActiveAsset == null)
		{
			// Update the name label
			assetNameLabel.Markup = "<i>Select an asset</i>";
			
			// Clear the asset log window
			this.assetBuildLogTextView.Buffer.Clear();
		}
		else
		{
			// Update the name label
			assetNameLabel.Text = this.ActiveAsset.Path.Replace("/", " / ");
			
			// Clear the log view
			this.assetBuildLogTextView.Buffer.Clear();
			
			// Load this asset's log into the log view
			BuildAssetLog_Write(this.ActiveAsset.BuildLog.ToString());	
		}
		
		// Load this asset into the property view
		AssetProperties_BindToActiveAsset();
		
		// Update the UI State
		UpdateAppUIState();
	}
	
	// **********************************************************************
	private void AssetProperties_BindToActiveAsset()
	{
		// Bind the processor combobox
		// ----------------------------
		
		// Clear the combo box
		if(assetProcessorCombo.Model is ListStore)
			(assetProcessorCombo.Model as ListStore).Clear();
		
		// Clear the property grid
		assetPropertyGrid.Clear();
		
		// Is there no active asset?
		if (this.ActiveAsset == null)
			return;
		
		// Get the processors that support this asset's extension
        string fileExtension = System.IO.Path.GetExtension(this.ActiveAsset.Path);
        fileExtension = fileExtension.TrimStart('.');
        List<ProcessorDetails> processorDetailList = ProcessorManager.GetProcessorsByFileExtension(fileExtension);

        // Load all the processors available to this asset into the combo box
		int itemIndex = 0;
        foreach (ProcessorDetails processorDetails in processorDetailList)
        {
            assetProcessorCombo.AppendText(processorDetails.Name);
			if(this.ActiveAsset.ProcessorType == processorDetails.Name)
				assetProcessorCombo.Active = itemIndex;
			itemIndex++;
        }
	}
	
	// **********************************************************************
	protected void AssetProperties_BindPropertyGrid()
	{
        // Is there no active asset?
		if (this.ActiveAsset == null)
			return;
		
        // Get the current processor
        ProcessorDetails selectedProcessor = ProcessorManager
			.GetProcessorDetails(assetProcessorCombo.ActiveText);

        // Load all the properties for the given processor
        // ----------------------------------------------------

        // Create a list of properties for the property grid
        List<Property> properties = new List<Property>();

        // Loop through the parameters of the processor
        foreach (KeyValuePair<string, ProcessorParameter> pair in selectedProcessor.Parameters)
        {
            ProcessorParameter parameter = pair.Value;

            // Create a Property that reflects this processor parameter
            Property property = new Property();
            property.Name = parameter.Name;
            property.Category = parameter.Category; 
            property.Description = parameter.Description;
            property.Type = parameter.Type;
            property.DefaultValue = parameter.DefaultValue.ToString();

            // Default the property value to the processor default
            property.Value = property.DefaultValue;
            property.Checked = false;

            // Does the asset have a custom value for this property?
            if (this.ActiveAsset.ProcessorParameters.ContainsKey(parameter.Name))
            {
                property.Value = this.ActiveAsset.ProcessorParameters[parameter.Name];
            }

            // Add the property to the list
            properties.Add(property);
        }

        // Bind the properties to the property grid
        this.assetPropertyGrid.BindToPropertyList(properties);
		
		
		// TODO: Remove  ----  TEST -------------------
		/*
		List<Property> props = new List<Property>();
		props.Add( new Property("Prop 1", "Cat 1", "Prop 1 Desc", false, typeof(string), "Prop 1 Value", "Prop 1 Value"));
		props.Add( new Property("Prop 2", "Cat 1", "Prop 2 Desc", false, typeof(string), "Prop 2 Value", "Prop 2 Value"));
		props.Add( new Property("Prop 3", "Cat 2", "Prop 3 Desc", false, typeof(bool), "true", "true"));
		props.Add( new Property("Prop 4", "Cat 2", "Prop 4 Desc", false, typeof(bool), "false", "false"));
		props.Add( new Property("Color A", "Cat 2", "1st color", false, typeof(Gdk.Framework.Color), "0,0,255,255", "0,0,0,255"));
		props.Add( new Property("Color B", "Cat 2", "2nd color", false, typeof(Gdk.Framework.Color), "0,255,0,255", "0,0,0,255"));
		props.Add( new Property("Prop 5", "Cat 2", "Prop 5 Desc", false, typeof(Gdk.Framework.DeviceType), "Web", "PC"));
		props.Add( new Property("Prop 6", "Cat 3", "Prop 6 Desc", false, typeof(Gdk.Framework.DeviceType), "PC", "PC"));
		props.Add( new Property("Int Prop", "Nums", "requires an int value", false, typeof(int), "123456", "2323"));
		props.Add( new Property("Float Prop", "Nums", "requires a float value", false, typeof(float), "3.14", "1.0"));
		props.Add( new Property("Vec2 Prop", "Nums", "requires a Vector2 value", false, typeof(Gdk.Framework.Vector2), "2.0, 2.0", "1,1"));
		*/
	}
	
	// **********************************************************************
	protected void AssetProperties_ProcessorComboChanged (object sender, System.EventArgs e)
	{
		// Verify we have an active asset
		if (this.ActiveAsset == null)
        	return;
		
		// Verify an item is selected
		if(this.assetProcessorCombo.Active < 0)
			return;

        // Get the selected processor
        ProcessorDetails selectedProcessor = ProcessorManager.GetProcessorDetails(assetProcessorCombo.ActiveText);
		if(selectedProcessor == null)
			return;
		
        // Change the processor used for this asset
        this.ActiveAsset.ProcessorType = selectedProcessor.Name;
		
		
        // Change the icon in the assets treeview for this asset
		TreeIter assetIter;
		if(this.AssetsTree_GetIterFromAssetPath(this.ActiveAsset.Path, out assetIter))
		{
			Gdk.Pixbuf icon = Gdk.Framework.IconsCache.GetIconPixbuf(selectedProcessor.Icon);
			this.assetsTreeStore.SetValue(assetIter, 0, icon);
		}
			
        // Setup the tooltip for the processor combo box
		assetProcessorCombo.TooltipText = selectedProcessor.Description;

        // Rebind the property grid
        AssetProperties_BindPropertyGrid();
	}
	
	// **********************************************************************
	protected void AssetProperties_PropertyGridValueChanged (Gdk.Framework.UI.GtkSharp.Property property)
	{
		AssetProperties_UpdateAssetFromProperty(property);
	}
	
	// **********************************************************************
    void AssetProperties_UpdateAssetFromProperty(Property property)
    {
		// Make sure we have an active asset
		if(this.ActiveAsset == null)
			return;
		
        // Set the value into the parameter set of the asset
		this.ActiveAsset.ProcessorParameters[property.Name] = property.Value;
    }
	
	// **********************************************************************
	[GLib.ConnectBefore]
	protected void AssetsTree_PopupMenu (object o, Gtk.PopupMenuArgs args)
	{
		// Open the Assets-Tree popup menu
		AssetsTree_OpenPopupMenu(0);
	}
	
	// **********************************************************************
	[GLib.ConnectBefore]
	protected void AssetsTree_ButtonPress (object o, Gtk.ButtonPressEventArgs args)
	{
		// Is this a single-click of the right mouse button?
		if (args.Event.Type == Gdk.EventType.ButtonPress && args.Event.Button == 3)
		{
			TreeViewDropPosition dropPosition;
			TreePath treePath;
			
			// Get the row under the mouse
			bool clickedOnRow = assetsTree.GetDestRowAtPos((int)args.Event.X, (int)args.Event.Y, out treePath, out dropPosition);
			
			// Did the user click on a NON-selected row?
			if (clickedOnRow &&
				assetsTree.Selection.PathIsSelected(treePath) == false)
			{
				// De-select all other rows
				assetsTree.Selection.UnselectAll();
				
				// Select the row under the mouse
				assetsTree.Selection.SelectPath(treePath);
			}
			
			// Open the Assets-Tree popup menu
			AssetsTree_OpenPopupMenu(args.Event.Button);
			
			args.RetVal = true;
		}
		
		// Is this a double-click of the left mouse button?
		if (args.Event.Type == Gdk.EventType.TwoButtonPress && args.Event.Button == 1)
		{
			TreeViewDropPosition dropPosition;
			TreePath treePath;
			
			// Get the row under the mouse
			if(assetsTree.GetDestRowAtPos((int)args.Event.X, (int)args.Event.Y, out treePath, out dropPosition))
			{
				// Is this an asset row?
				TreeIter iter;
				assetsTreeStore.GetIter(out iter, treePath);
				object item = assetsTreeStore.GetValue(iter, 2);
				if(item is Asset)
				{
					// Create a custom bundle with just this asset
					Bundle customBundle = new Bundle(bundle.Name, bundle.RootPath);
					customBundle.Assets.Add(item as Asset);
					
					// Start a build of the custom bundle
					Build_Start(customBundle, true);
				}
			}
		}		
	}
	
	// **********************************************************************
	protected void AssetsTree_PopupMenu_Build (object o, EventArgs args)
	{
		HashSet<Asset> assetsToBuild = new HashSet<Asset>();
		
		// Loop through the selected tree nodes
		foreach(TreePath treePath in assetsTree.Selection.GetSelectedRows())
		{
			// Collect all the assets at or below this node
			TreeIter treeIter;
			assetsTreeStore.GetIter(out treeIter, treePath);
			AssetsTree_RecurseCollectAssets(treeIter, assetsToBuild);
		}
			
		// Create a custom bundle using the collected assets
		Bundle customBundle = new Bundle(bundle.Name, bundle.RootPath);
		customBundle.Assets.AddRange(assetsToBuild);
		
		// Start a build of the custom bundle
		Build_Start(customBundle, true);
	}
	
	// **********************************************************************
	protected void AssetsTree_PopupMenu_AddFiles (object o, EventArgs args)
	{
		// Create a File Chooser Dialog
		FileChooserDialog dlg = new Gtk.FileChooserDialog(
			"Select asset file(s)",
            this,
            FileChooserAction.Open,
            "Cancel", ResponseType.Cancel,
            "Open", ResponseType.Ok
			);
		
		// Allow multi-select
		dlg.SelectMultiple = true;
		
		// Setup the filters
		dlg.Filter = new FileFilter();
		dlg.Filter.AddPattern("*.*");
		dlg.Filter.Name = "All Files (*.*)";
		
		// Are there any currently selected tree items?
		if(assetsTree.Selection.CountSelectedRows() > 0)
		{
			// Get the first selected item
			TreePath selectedPath = assetsTree.Selection.GetSelectedRows()[0];
			TreeIter selectedIter;
			assetsTreeStore.GetIter(out selectedIter, selectedPath);

			// Is this a folder?
			object item = assetsTreeStore.GetValue(selectedIter, 2);
			if(item is FolderItem)
			{
				// Use the path of the folder, as the starting path of the dialog
				FolderItem folderItem = item as FolderItem;
				dlg.SetCurrentFolder(System.IO.Path.Combine(bundle.RootPath, folderItem.Path));
			}
			// Is this an asset
			else if(item is Asset)
			{
				// Use the path of the asset's parent folder, as the starting path of the dialog
				Asset asset = item as Asset;
				dlg.SetCurrentFolder(System.IO.Path.Combine(bundle.RootPath, System.IO.Path.GetDirectoryName(asset.Path)));
			}
		}
		
		// Show the dialog
		int response = dlg.Run();
		string[] filenames = dlg.Filenames;
		dlg.Destroy();
		
		// Did the user click OK on the dialog?
		if (response == (int)ResponseType.Ok) 
		{
			// Loop through the files that were selected
			foreach(string tempFilename in filenames)
			{
				string filename = tempFilename.Replace('\\', '/');
				
				// Make sure the path exists within the bundle root
				if(filename.StartsWith(bundle.RootPath, StringComparison.InvariantCultureIgnoreCase) == false)
				{
					// An error occured loading the bundle, do nothing...
					MessageDialog md = new MessageDialog(
						this, DialogFlags.Modal, MessageType.Error, ButtonsType.Ok, 
						"The file: " + System.Environment.NewLine + filename + System.Environment.NewLine + System.Environment.NewLine +
					    "Does not exist in the bundle root: " + System.Environment.NewLine + bundle.RootPath
						);
	            	md.Run ();
	            	md.Destroy();
					return;
				}
				
				// Get the path of the asset, relative to the bundle
				string assetRelPath = filename.Substring(bundle.RootPath.Length + 1);
				
				// Make sure this asset isnt already in the bundle
				if(bundle.Assets.Exists(item => string.Compare(item.Path, assetRelPath, true) == 0))
					continue; // Asset file is already in the bundle
				
				// Get the processors for the given file extension
                string fileExtension = System.IO.Path.GetExtension(assetRelPath);
                fileExtension = fileExtension.TrimStart('.');
                List<ProcessorDetails> processorDetailList = ProcessorManager.GetProcessorsByFileExtension(fileExtension);

                // If there is no supporting processor, throw an error
                if (processorDetailList.Count == 0)
                {
					// An error occured loading the bundle, do nothing...
					MessageDialog md = new MessageDialog(
						this, DialogFlags.Modal, MessageType.Error, ButtonsType.Ok, 
						"There are no supporting asset processors for the given file extension [" + fileExtension + "]"
						);
	            	md.Run ();
	            	md.Destroy();
					return;
                }

                // Default to the first processor that supports the given extension
                ProcessorDetails processorDetails = processorDetailList[0];
				
				// Create a new Asset
				Asset asset = new Asset();
				asset.Path = assetRelPath;
				asset.ProcessorType = processorDetails.Name;
				
				// Add the asset to the bundle
				this.bundle.Assets.Add(asset);
				
				// Create the TreeIter for the new asset
				TreeIter assetIter = AssetsTree_EnsureAssetNodeExists(asset);
				// Expand to this new node
				this.assetsTree.ExpandToPath(this.assetsTreeStore.GetPath(assetIter));
			}
		}
	}
	
	// **********************************************************************
	protected void AssetsTree_PopupMenu_Remove (object o, EventArgs args)
	{
		// Confirm with the user that they want to remove these nodes..
		MessageDialog md = new MessageDialog(
			this, DialogFlags.Modal, MessageType.Question, ButtonsType.YesNo, 
		   	"Are you sure you wish to remove the selected node(s)?"
			);
    	int result = md.Run ();
    	md.Destroy();
		
		// Did the user hit "No"?
		if(result != (int)ResponseType.Yes)
			return; // Bail early
		
		// While there are selected items
		while(assetsTree.Selection.CountSelectedRows() > 0)
		{
			// Get the first item in the selection list
			TreePath selectedPath = assetsTree.Selection.GetSelectedRows()[0];
			TreeIter selectedIter;
			assetsTreeStore.GetIter(out selectedIter, selectedPath);
			
			// Remove any assets at or below this iter
			AssetsTree_RecurseRemoveAssetsFromBundle(selectedIter);
			
			// Remove the item from the tree store
			assetsTreeStore.Remove(ref selectedIter);
		}
	}
	
	// **********************************************************************
	private void AssetsTree_SelectionChanged(object o, EventArgs args) 
	{
		// Get the selected rows
		TreePath[] selectedRows = assetsTree.Selection.GetSelectedRows();
		
		// Is nothing selected?
		if(selectedRows == null || selectedRows.Count() == 0)
		{
			// Set the active asset to 'unset'
			this.ActiveAsset = null;
		}
		else
		{
			// Get the last selected item
			TreeIter selectedIter;
			assetsTreeStore.GetIter(out selectedIter, selectedRows.Last());
			object item = assetsTreeStore.GetValue(selectedIter, 2);

			// Is this an asset?
			if(item is Asset)
			{
				// Set this asset as the active item
				this.ActiveAsset = item as Asset;
			}
			else
			{
				this.ActiveAsset = null;
			}
		}
	}
	
	// **********************************************************************
	private void AssetsTree_RecurseRemoveAssetsFromBundle(TreeIter iter)
	{
		// Get the item for this iter
		object item = this.assetsTreeStore.GetValue(iter, 2);
		
		// If this is an asset, remove it from the bundle
		if(item is Asset)
			this.bundle.Assets.Remove(item as Asset);
			
		// Loop through the children of this iter
		TreeIter childIter;
		bool loop = assetsTreeStore.IterChildren(out childIter, iter);
		while(loop)
		{
			// Recurse
			AssetsTree_RecurseRemoveAssetsFromBundle(childIter);
			
			// Next child
			loop = assetsTreeStore.IterNext(ref childIter);
		}
	}
	
	// **********************************************************************
	private void AssetsTree_RecurseCollectAssets(TreeIter iter, HashSet<Asset> assetsSet)
	{
		// Get the item for this iter
		object item = this.assetsTreeStore.GetValue(iter, 2);
		
		// If this is an asset, add it to the set
		if(item is Asset)
			assetsSet.Add(item as Asset);
			
		// Loop through the children of this iter
		TreeIter childIter;
		bool loop = assetsTreeStore.IterChildren(out childIter, iter);
		while(loop)
		{
			// Recurse
			AssetsTree_RecurseCollectAssets(childIter, assetsSet);
			
			// Next child
			loop = assetsTreeStore.IterNext(ref childIter);
		}
	}
	
	// **********************************************************************
	private void AssetsTree_OpenPopupMenu(uint triggerButton)
	{
		// Create the popup menu
		// ------------------------------
		
		Menu menu = new Menu();
		MenuItem menuItem;
		
		int numSelectedRows = assetsTree.Selection.CountSelectedRows();
		
		// Add menu item: "Build" 
		if(numSelectedRows > 0)
		{
			menuItem = new MenuItem("Build");
			menuItem.Activated += new EventHandler(AssetsTree_PopupMenu_Build);
			menu.Append(menuItem);
			
			// Add a seperator
			menu.Append(new SeparatorMenuItem());
		}
		
		// Add menu item: "Add File(s)" 
		menuItem = new MenuItem("Add File(s)");
		menuItem.Activated += new EventHandler(AssetsTree_PopupMenu_AddFiles);
		menu.Append(menuItem);
		
		// Add menu item: "Remove" 
		if(numSelectedRows > 0)
		{
			menuItem = new MenuItem("Remove");
			menuItem.Activated += new EventHandler(AssetsTree_PopupMenu_Remove);
			menu.Append(menuItem);
		}
			
		// Are there no items in the menu?
		if(menu.Children.GetUpperBound(0) < 0)
			return; // Bail
		
		// Show the menu
		menu.ShowAll();
		menu.Popup(null, null, null, triggerButton, Global.CurrentEventTime);
	}
	
	// **********************************************************************
	private void AssetsTree_Setup()
	{
		// Setup the Active Asset		
		this.activeAsset = null;
		
		// Setup the TreeView properties
		// ------------------------------
		
		// Set the tre view for multi-select
		assetsTree.Selection.Mode = SelectionMode.Multiple;
		
		// Bind to some events on the tree
		assetsTree.Selection.Changed += new EventHandler(AssetsTree_SelectionChanged);
		
		// Setup the TreeView columns
		// ------------------------------

		// Column 1 - Icon & Name
		TreeViewColumn column = new TreeViewColumn();
		
		// Add a pixbuf cell renderer for the icon
		CellRendererPixbuf pixbufRenderer = new CellRendererPixbuf();
		pixbufRenderer.Height = 16;
		column.PackStart(pixbufRenderer, false);
		column.SetAttributes(pixbufRenderer, "pixbuf", 0);
		
		// Add a test cell renderer for the node name
		CellRendererText textRenderer = new CellRendererText();
		textRenderer.Height = 16;
		column.PackStart(textRenderer, true);
		column.SetAttributes(textRenderer, "text", 1);
		
		// Add the column to the treeview
		assetsTree.AppendColumn(column);
		
		// Setup the TreeStore for the tree view
		// ---------------------------------------
		
		// Create the tree store
		assetsTreeStore = new TreeStore(
			typeof(Gdk.Pixbuf),				// Icon
			typeof(string),					// Name
			typeof(object)					// Node data
			);
		assetsTree.Model = assetsTreeStore;
		
		// Create the root node
		Gdk.Pixbuf pixbufPackage = Gdk.Framework.IconsCache.GetIconPixbuf(Gdk.Framework.Icons.Package);
		assetsTreeStore.AppendValues(pixbufPackage, "Content", new FolderItem(""));
	}
	
	/// <summary>
	/// Gets the TreeIter for the given asset, creating it if necessary
	/// </summary>
	private TreeIter AssetsTree_EnsureAssetNodeExists(Asset asset)
	{
		// Ensure the folders for the asset path exists
        string folderPath = System.IO.Path.GetDirectoryName(asset.Path);
        TreeIter folderIter = AssetsTree_EnsureFolderNodeExists(folderPath);
		
		// Loop through the children of the parent folder
		TreeIter childIter;
		bool loop = assetsTreeStore.IterChildren(out childIter, folderIter);
		while(loop)
		{
			// Is this the the asset TreeIter we're looking for?
			if(string.Compare((string) assetsTreeStore.GetValue(childIter, 1), asset.Name, true) == 0)
				return childIter;
			
			// Next child
			loop = assetsTreeStore.IterNext(ref childIter);
		}
		
		// The asset TreeIter doesnt exist, let's create it
		
		// Get the details for this asset's processor
        ProcessorDetails processorDetails = ProcessorManager.GetProcessorDetails(asset.ProcessorType);
				
		// Create a tree item for this asset
		return AssetsTree_InsertChildSorted(folderIter, asset.Name, processorDetails.Icon, asset);
	}
	
	/// <summary>
    /// Gets the tree iter of a folder for the given path.  Creating it if necessary
    /// </summary>
    private TreeIter AssetsTree_EnsureFolderNodeExists(string path)
    {
        // Empty path = root iter
        if(string.IsNullOrEmpty(path))
		{
			TreeIter rootIter;
            assetsTreeStore.IterChildren(out rootIter);
			return rootIter;
		}

        // Get the iterator of the parent folder
        string itemName = System.IO.Path.GetFileName(path);
        string parentPath = System.IO.Path.GetDirectoryName(path);
        TreeIter parentIter = AssetsTree_EnsureFolderNodeExists(parentPath);
        
        // Loop through the children of the parent folder
		TreeIter childIter;
		bool loop = assetsTreeStore.IterChildren(out childIter, parentIter);
		while(loop)
		{
			// Is this the folder the caller is looking for?
			if(string.Compare((string) assetsTreeStore.GetValue(childIter, 1), itemName, true) == 0)
				return childIter;
			
			// Next child
			loop = assetsTreeStore.IterNext(ref childIter);
		}

        // The folder item doesnt exist, so let's create it
		return AssetsTree_InsertChildSorted(parentIter, itemName, Gdk.Framework.Icons.Folder, new FolderItem(path));
    }
	
	/// <summary>
	/// Adds a child item to the tree store, properly sorted in it's parent nodes child list
	/// </summary>
	private TreeIter AssetsTree_InsertChildSorted(TreeIter parentIter, string name, Gdk.Framework.Icons icon, object item)
	{
		int insertionIndex = 0;

        // Is the new node a folder?
        if (item is FolderItem)
        {
			// Walk through the children nodes until we find where we need to insert the child
			TreeIter childIter;
			bool loop = assetsTreeStore.IterChildren(out childIter, parentIter);
			while(loop)
			{
				// Have we hit a non folder?
                if ( (assetsTreeStore.GetValue(childIter, 2) is FolderItem) == false)
                    break;

                // Have we found a folder the new node comes before?
                else if (string.Compare(name, (string)assetsTreeStore.GetValue(childIter, 1), true) < 0)
                    break;

                //  move to the next node
                insertionIndex++;
				
				// Next child
				loop = assetsTreeStore.IterNext(ref childIter);
			}
        }
        else
        {
            // Walk through the children nodes until we find where we need to insert the child
			TreeIter childIter;
			bool loop = assetsTreeStore.IterChildren(out childIter, parentIter);
			while(loop)
			{
                // Is this a non-file node?  That the new node comes before?
                if ((assetsTreeStore.GetValue(childIter, 2) is FolderItem) == false &&
                    string.Compare(name, (string)assetsTreeStore.GetValue(childIter, 1), true) < 0)
                    break;

                //  move to the next node
                insertionIndex++;
					
            	// Next child
				loop = assetsTreeStore.IterNext(ref childIter);
			}
        }

        // Insert the node
		Gdk.Pixbuf pixbuf = Gdk.Framework.IconsCache.GetIconPixbuf(icon);
		TreeIter newIter = assetsTreeStore.InsertWithValues(parentIter, insertionIndex, pixbuf, name, item);
		return newIter;
	}
	
	// **********************************************************
    private bool AssetsTree_GetIterFromAssetPath(string path, out TreeIter iter)
    {
		iter = TreeIter.Zero;
		
        // Empty path = root iter
        if(string.IsNullOrEmpty(path))
		{
            assetsTreeStore.IterChildren(out iter);
			return true;
		}

        // Get the iterator of the parent
        string itemName = System.IO.Path.GetFileName(path);
        string parentPath = System.IO.Path.GetDirectoryName(path);
        TreeIter parentIter;
		if(AssetsTree_GetIterFromAssetPath(parentPath, out parentIter) == false)
			return false;
        
        // Loop through the children of the parent
		TreeIter childIter;
		bool loop = assetsTreeStore.IterChildren(out childIter, parentIter);
		while(loop)
		{
			// Is this the folder the caller is looking for?
			string childName = (string) assetsTreeStore.GetValue(childIter, 1);
			if(string.Compare(childName, itemName, true) == 0)
			{
				iter = childIter;
				return true;
			}
			
			// Next child
			loop = assetsTreeStore.IterNext(ref childIter);
		}

        // This part of the path is not-found
		return false;
    }
	
	// **********************************************************
	private void BuildLog_Setup()
	{
		// Setup the "Build Log" tab
		// ----------------------------------------
		
		// Create a GtmlTextBuffer for the TextView
		Gdk.Framework.UI.GtkSharp.GtmlTextBuffer buffer = new Gdk.Framework.UI.GtkSharp.GtmlTextBuffer();
		this.buildLogTextView.Buffer = buffer;
		
		// Start the log
		BuildLog_WriteLine("Ready...");
	}
	
	// **********************************************************
	private void BuildLog_Clear()
	{
		// Clear the build log buffer
		this.buildLogTextView.Buffer.Clear();
	}
	
	// **********************************************************
	private void BuildLog_Write(string gtml)
	{
		// Append the log message
		Gdk.Framework.UI.GtkSharp.GtmlTextBuffer buffer = this.buildLogTextView.Buffer as Gdk.Framework.UI.GtkSharp.GtmlTextBuffer;
		buffer.AppendGTML(gtml);
		
		// Scroll to the bottom
		TextMark mark = buffer.CreateMark(null, buffer.EndIter, false);
        buildLogTextView.ScrollToMark(mark, 0, false, 0, 1);
		buildLogTextView.ScrollToIter(buffer.EndIter, 0.2, true, 0.0, 1.0);
	}
			
	// **********************************************************
	private void BuildLog_WriteLine(string gtml)
	{
		BuildLog_Write(gtml + "\n");
	}
	
	// **********************************************************
	private void BuildErrors_Setup()
	{
		// Setup the TreeView properties
		// ------------------------------
		
		// Set the tre view for single-selection
		buildErrorsTreeView.Selection.Mode = SelectionMode.Single;
		
		// Bind to some events on the tree
		buildErrorsTreeView.Selection.Changed += BuildErrors_SelectionChanged;
		buildErrorsTreeView.ButtonPressEvent += BuildErrors_ButtonPressEvent;
		// Setup the TreeView columns
		// ------------------------------
		
		// Column 1 - Type Icon
		// Column 2 - Build error
		// Column 3 - Asset Name
		// Column 4 - Asset Path
		
		// Column 1 - Icon & Name
		TreeViewColumn column = new TreeViewColumn();
		column.Title = "!";
		CellRendererPixbuf pixbufRenderer = new CellRendererPixbuf();
		pixbufRenderer.Height = 14;
		//pixbufRenderer.StockSize = 14;
		column.PackStart(pixbufRenderer, false);
		column.SetAttributes(pixbufRenderer, "stock-id", 0);
		buildErrorsTreeView.AppendColumn(column);
				
		// Column 2 - Build Error
		column = new TreeViewColumn();
		column.Title = "Description";
		CellRendererText textRenderer = new CellRendererText();
		column.PackStart(textRenderer, false);
		column.SetAttributes(textRenderer, "text", 1);
		buildErrorsTreeView.AppendColumn(column);
		
		// Column 3 - Asset Name
		column = new TreeViewColumn();
		column.Title = "Asset";
		textRenderer = new CellRendererText();
		column.PackStart(textRenderer, false);
		column.SetAttributes(textRenderer, "text", 2);
		buildErrorsTreeView.AppendColumn(column);
		
		// Column 3 - Asset Name
		column = new TreeViewColumn();
		column.Title = "Path";
		textRenderer = new CellRendererText();
		column.PackStart(textRenderer, false);
		column.SetAttributes(textRenderer, "text", 3);
		buildErrorsTreeView.AppendColumn(column);
		
		// Setup the ListStore for the tree view
		// ---------------------------------------
		
		// Create the list store
		this.buildErrorsListStore = new ListStore(
			typeof(string),					// Icon stock-id
			typeof(string),					// Error text
			typeof(string),					// Asset.Name text
			typeof(string)					// Asset.Path text
			);
		this.buildErrorsTreeView.Model = this.buildErrorsListStore;
		
		// Clear out the errors (which resets all the view data)
		BuildErrors_Clear();
	}
	
	// **********************************************************
	private void BuildErrors_Clear()
	{
		// Reset the header tags
		this.buildErrorsCount = 0;
		this.buildWarningsCount = 0;
		this.buildErrorsTabLabel.Markup = "0";
		this.buildWarningsTabLabel.Markup = "0";
		this.buildErrorsTabImage.Sensitive = false;
		this.buildWarningsTabImage.Sensitive = false;
		
		// Clear out the list
		this.buildErrorsListStore.Clear();
	}
	
	// **********************************************************
	private void BuildErrors_AddLine(bool isError, string error, string assetName, string assetPath)
	{
		// Which icon do we use?
		this.buildErrorsListStore.AppendValues(
			isError ? "gtk-dialog-error" : "gtk-dialog-warning",
			error,
			assetName,
			assetPath
			);
	}
	
	// **************************************************************
	private void BuildErrors_ActivateAssetOfSelectedRow()
	{
		// Is there a selected item?  (and if so, get it)
		TreeIter selectedIter;
		if(buildErrorsTreeView.Selection.GetSelected(out selectedIter) == false)
			return; // Bail early, nothing is selected
		
		// Get the asset path 
		string assetPath = buildErrorsListStore.GetValue(selectedIter, 3) as string;
		
		// Find the TreeIter from the path of the asset
		TreeIter assetTreeIter;
		if(AssetsTree_GetIterFromAssetPath(assetPath, out assetTreeIter))
		{
			// Select this asset tree item
			this.assetsTree.Selection.UnselectAll();
			this.assetsTree.Selection.SelectIter(assetTreeIter);
			
			// Flip to the build log view
			this.assetNotebook.Page = 1;
		}
	}
	
	// **********************************************************
	private void BuildErrors_SelectionChanged(object o, EventArgs args) 
	{
		BuildErrors_ActivateAssetOfSelectedRow();
	}
	
	// **********************************************************
	[GLib.ConnectBefore]
	private void BuildErrors_ButtonPressEvent (object o, ButtonPressEventArgs args)
	{
		// Is this a double-click on the item?
		if (args.Event.Type == Gdk.EventType.TwoButtonPress)
		{
			BuildErrors_ActivateAssetOfSelectedRow();
		}
	}
	
	// **********************************************************
	private void BuildOutputs_Setup()
	{
		// Setup the TreeView properties
		// ------------------------------
		
		// Set the tre view for single-selection
		buildOutputsTreeView.Selection.Mode = SelectionMode.Single;
		
		// Bind to some events on the tree
		buildOutputsTreeView.ButtonPressEvent += BuildOutputs_ButtonPressEvent;
		
		// Setup the TreeView columns
		// ------------------------------
		
		// Column 1 - Icon
		TreeViewColumn column = new TreeViewColumn();
		column.Title = "Type";
		CellRendererPixbuf pixbufRenderer = new CellRendererPixbuf();
		pixbufRenderer.Height = 14;
		column.PackStart(pixbufRenderer, false);
		column.SetAttributes(pixbufRenderer, "pixbuf", 0);
		buildOutputsTreeView.AppendColumn(column);
				
		// Column 2 - Output path
		column = new TreeViewColumn();
		column.Title = "Output";
		CellRendererText textRenderer = new CellRendererText();
		column.PackStart(textRenderer, false);
		column.SetAttributes(textRenderer, "text", 1);
		buildOutputsTreeView.AppendColumn(column);
		
		// Column 3 - Source asset path
		column = new TreeViewColumn();
		column.Title = "Source Asset";
		textRenderer = new CellRendererText();
		column.PackStart(textRenderer, false);
		column.SetAttributes(textRenderer, "text", 3);
		buildOutputsTreeView.AppendColumn(column);
		
		// Setup the ListStore for the tree view
		// ---------------------------------------
		
		// Create the list store
		this.buildOutputsListStore = new ListStore(
			typeof(Gdk.Pixbuf),				// Icon
			typeof(string),					// Display Name
			typeof(string), 				// Full output path
			typeof(string)					// Source Asset
			);
		this.buildOutputsTreeView.Model = this.buildOutputsListStore;
		
		// Clear out the outputs (which resets all the view data)
		BuildOutputs_Clear();
	}
	
	// **********************************************************
	private void BuildOutputs_Clear()
	{
		// Reset the counters
		this.buildOutputsCount = 0;
		
		// Clear out the list
		this.buildOutputsListStore.Clear();
	}
	
	// **********************************************************
	private void BuildOutputs_AddOutput(string sourceAsset, string displayName, string fullOutputPath)
	{
		// Determine the icon pixbuf based on the output path extension
		string ext = System.IO.Path.GetExtension(fullOutputPath);
		Gdk.Pixbuf icon = null;
		switch(ext.ToLower())
		{
			case ".gdkimage": icon = Gdk.Framework.IconsCache.GetIconPixbuf(Gdk.Framework.Icons.Texture);  break;
			case ".gdkatlas": icon = Gdk.Framework.IconsCache.GetIconPixbuf(Gdk.Framework.Icons.Atlas);  break;
			case ".gdkmodel": icon = Gdk.Framework.IconsCache.GetIconPixbuf(Gdk.Framework.Icons.Model);  break;
			case ".gdkfont": icon = Gdk.Framework.IconsCache.GetIconPixbuf(Gdk.Framework.Icons.Font);  break;
			case ".gdkshader": icon = Gdk.Framework.IconsCache.GetIconPixbuf(Gdk.Framework.Icons.Shader);  break;
			default: icon = Gdk.Framework.IconsCache.GetIconPixbuf(Gdk.Framework.Icons.File);  break;
		}
		
		// Add a row to the list
		this.buildOutputsListStore.AppendValues(
			icon,
			displayName,
			fullOutputPath,
			sourceAsset
			);
	}
	
	// **********************************************************
	[GLib.ConnectBefore]
	private void BuildOutputs_ButtonPressEvent (object o, ButtonPressEventArgs args)
	{
		// Is this a double-click on the item?
		if (args.Event.Type == Gdk.EventType.TwoButtonPress)
		{
			// Is there a selected item?  (and if so, get it)
			TreeIter selectedIter;
			if(this.buildOutputsTreeView.Selection.GetSelected(out selectedIter) == false)
				return; // Bail early, nothing is selected
			
			// Get the output path 
			string outputPath = buildOutputsListStore.GetValue(selectedIter, 2) as string;
			
			// Process this output path as if it were a link
			Utility_LaunchAssetViewer(outputPath);
		}
	}
	
	// ****************************************************************************
	private void BuildAssetLog_Setup()
	{
		// Create a GtmlTextBuffer for the asset build log TextView
		Gdk.Framework.UI.GtkSharp.GtmlTextBuffer buffer = new Gdk.Framework.UI.GtkSharp.GtmlTextBuffer();
		this.assetBuildLogTextView.Buffer = buffer;
		
		buffer.MonospaceEnabled = true;
		
		// Attach to several events on the view
		this.assetBuildLogTextView.MotionNotifyEvent += BuildAssetLog_LogViewMotionNotifyEvent;
		this.assetBuildLogTextView.ButtonPressEvent += BuildAssetLog_LogViewButtonPressEvent;
	}
	
	// ****************************************************************************
	[GLib.ConnectBefore]
	private void BuildAssetLog_LogViewButtonPressEvent (object o, ButtonPressEventArgs args)
	{
		// Is this a single left-click event?
		if(args.Event.Button == 1 && args.Event.Type == Gdk.EventType.ButtonPress)
		{
			// Convert the mouse position to coordinates within the buffer
			int bufferX, bufferY;
			this.assetBuildLogTextView.WindowToBufferCoords(
				TextWindowType.Widget,
				(int)args.Event.X, (int)args.Event.Y,
				out bufferX, out bufferY
				);
			
			// Get the iter at the cursor position
			TextIter iter = this.assetBuildLogTextView.GetIterAtLocation(bufferX, bufferY);
			
			// Is there a link at this iter?
			Gdk.Framework.UI.GtkSharp.GtmlTextBuffer buffer = this.assetBuildLogTextView.Buffer as Gdk.Framework.UI.GtkSharp.GtmlTextBuffer;
			string linkUrl;
			if(buffer.GetLinkAtIter(iter, out linkUrl))
			{
				// Open this link
				Utility_ProcessLinkUrl(linkUrl);
			}
		}
	}
	
	// ****************************************************************************
	private void BuildAssetLog_LogViewMotionNotifyEvent (object o, MotionNotifyEventArgs args)
	{
		// Convert the mouse position to coordinates within the buffer
		int bufferX, bufferY;
		this.assetBuildLogTextView.WindowToBufferCoords(
			TextWindowType.Widget, 
			(int)args.Event.X, (int)args.Event.Y,
			out bufferX, out bufferY
			);
		
		// Get the iter at the cursor position
		TextIter iter = this.assetBuildLogTextView.GetIterAtLocation(bufferX, bufferY);
		
		// Is there a link at this iter?
		Gdk.Framework.UI.GtkSharp.GtmlTextBuffer buffer = this.assetBuildLogTextView.Buffer as Gdk.Framework.UI.GtkSharp.GtmlTextBuffer;
		string linkUrl;
		bool hovering;
		if(buffer.GetLinkAtIter(iter, out linkUrl))
		{
			hovering = true;
		}
		else
		{
			hovering = false;
		}
		
		// Has the hover state changed?
		if(hoveringOverLinkInAssetLog != hovering)
		{
			// Update the hover state
			hoveringOverLinkInAssetLog = hovering;
			
			// Set the mouse cursor
			Gdk.Window window = this.assetBuildLogTextView.GetWindow(TextWindowType.Text);
			if(hovering)
				window.Cursor = handCursor;
			else
				window.Cursor = textCursor;
			
		}
	}

	// ****************************************************************************
	private void BuildAssetLog_EraseAssetLog(Asset asset)
	{
		// Erase the log for this asset
		asset.BuildLog = new StringBuilder();
		
		// Is this the actively viewed asset?
		if(this.ActiveAsset == asset)
		{
			// Also clear the asset log buffer
			this.assetBuildLogTextView.Buffer.Clear();
		}
	}
	
	// ****************************************************************************
	private void BuildAssetLog_Write(string gtml)
	{
		// Append the log message
		Gdk.Framework.UI.GtkSharp.GtmlTextBuffer buffer = this.assetBuildLogTextView.Buffer as Gdk.Framework.UI.GtkSharp.GtmlTextBuffer;
		buffer.AppendGTML(gtml);
		
		// Scroll to the bottom
		TextMark mark = buffer.CreateMark(null, buffer.EndIter, false);
        assetBuildLogTextView.ScrollToMark(mark, 0, false, 0, 1);
		//assetBuildLogTextView.ScrollToIter(buffer.EndIter, 0.0, false, 0.0, 1.0);
	}
	
	// **********************************************************
	private void Utility_ProcessLinkUrl(string linkUrl)
	{
		// Get the link protocol
		int protocolSeparatorIndex = linkUrl.IndexOf(':');
		if(protocolSeparatorIndex <= 0)
			return;
		string protocol = linkUrl.Substring(0, protocolSeparatorIndex);
		string path = linkUrl.Substring(protocolSeparatorIndex + 1);
		
		// Process the protocol
		switch(protocol.ToLower())
		{
			case "gdkasset":
			{	
				// Link Format:
				// gdkasset:/Users/Justin/SVN/Gdk/trunk/Assets/Build/Base/Atlas/Test.gdkatlas
			
				// Launch an asset viewer for this built asset
				Utility_LaunchAssetViewer(path);
			}
			break;
		}
	}
	
	// **********************************************************
	private void Utility_LaunchAssetViewer(string path)
	{
		// Create a viewer window for displaying this asset 
		Gtk.Window window = Gdk.Framework.UI.GtkSharp.AssetViewerHelper.CreateAssetViewer(path);			
		if(window != null)
		{
			window.KeepAbove = true;
			window.TransientFor = this;
			window.TypeHint = Gdk.WindowTypeHint.Utility;
		}
		else
		{
			// No asset viewer for this asset extension :|
			BuildLog_WriteLine("No asset viewer exists for the asset type [" + System.IO.Path.GetExtension(path) + "]");
		}
	}
	
	// **********************************************************
	private void UpdateAppUIState()
	{
		// Build some state booleans
		bool buildInProgress = this.buildThread != null;
		bool bundleIsReady = this.bundle != null;
		
		// Toolbar buttons
		newAction.Sensitive = !buildInProgress;
		openAction.Sensitive = !buildInProgress;
		saveAction.Sensitive = bundleIsReady;
		
		buildAction.Sensitive = bundleIsReady && !buildInProgress;
		rebuildAction.Sensitive = bundleIsReady && !buildInProgress;
		cleanAction.Sensitive = bundleIsReady && !buildInProgress;
		stopAction.Sensitive = buildInProgress;
		
		// TreeView
		assetsTree.Sensitive = bundleIsReady;
		
		// Property view
		assetProcessorCombo.Sensitive = this.ActiveAsset != null;
	}
	
	// **********************************************************
	private void BindToBundle()
	{
		// Clear the tree view
		
		// Is there no bundle?
		if(this.bundle == null)
		{
			this.Title = "Gdk Content Studio";
		}
		else
		{
			// Update the window title to reflect the bundle name
			this.Title = "Gdk Content Studio - " + bundle.GetBundleFilePath();
			
			// Loop through the assets in the bundle
			foreach(Asset asset in this.bundle.Assets)
			{
				// Create the tree store item for this asset
				AssetsTree_EnsureAssetNodeExists(asset);
			}
			
			// Expand the whole tree
			this.assetsTree.ExpandAll();
		}
	}
	
	// **********************************************************
    private void Build_DoClean()
    {
		// Default the output path to the [BundleRoot]\[Build]\[BundleName] folder
        string bundleBuildFolder = Gdk.Framework.PathUtilities.Combine(this.bundle.RootPath, "Build");
		bundleBuildFolder = Gdk.Framework.PathUtilities.Combine(bundleBuildFolder, this.bundle.Name);
		
		// Prompt the user to MAKE SURE
		string message = 
			"This will delete <b>EVERY FILE</b> in the bundle build folder." + System.Environment.NewLine + 
			System.Environment.NewLine +
			"<u>Bundle Build Folder: </u>" + System.Environment.NewLine +
			bundleBuildFolder + System.Environment.NewLine +
			System.Environment.NewLine + 
			"<b>Are you sure you wish to do this?</b>";
				
        MessageDialog md = new MessageDialog(
            this, DialogFlags.Modal, MessageType.Question, ButtonsType.YesNo, message
            );
		md.UseMarkup = true;
        int result = md.Run();
        md.Destroy();
		
		// Did the user cancel?
		if(result != (int)ResponseType.Yes)
			return;
		
		// Clear the build log
		BuildLog_Clear();
		BuildLog_Write("Performing Clean...  ");
		
		// Delete!
		try
		{
			System.IO.Directory.Delete(bundleBuildFolder, true);
		}
		catch(Exception ex)
		{
			// The clean failed.. log the exception details
			BuildLog_WriteLine(
				"<color:#B00000>FAILED\n\n" +
				Gdk.Framework.ExceptionUtilities.GetExceptionText(ex) +
				"</color>"
				);
			return;
		}
		
		// Clean succeeded
		BuildLog_Write("<color:#008000>Successful</color>\n"); 
	}
	
    // **********************************************************
    private void Build_Start(Bundle bundleToBuild, bool forceRebuild)
    {
        // Only 1 build thread at a time..
        if (buildThread != null)
            return;
		
		// Reset the build counters
		this.buildNumAssetsSkipped = 0;
		this.buildNumAssetsSuccessful = 0;
		this.buildNumAssetsSuccessWithWarnings = 0;
		this.buildNumAssetsFailed = 0;
		
		// Clear the build logs
		BuildLog_Clear();
		BuildErrors_Clear();
		BuildOutputs_Clear();
		
		// Sort the assets in the bundle
		bundleToBuild.Assets.Sort((a,b) => { return a.Path.CompareTo(b.Path); });
		
		// Clear the asset build logs for all assets in this bundle
		foreach(Asset asset in bundleToBuild.Assets)
			BuildAssetLog_EraseAssetLog(asset);
		
		// Switch to the build-log tab on the bottom panel
		this.bottomNotebook.Page = 0;
		
        // Set the build parameters
        this.buildBundle = bundleToBuild;
        this.buildForceRebuild = forceRebuild;

        // Create the build thread
        buildThread = new Thread(Build_Worker);

        // Start the thread
        buildThread.IsBackground = true;
        buildThread.Start();

        // Update the UI State
        UpdateAppUIState();
    }

    // **********************************************************
    public void Build_Worker()
    {
        // Create a ContentBuilder
        this.builder = new ContentBuilder();
		
		// Bind to events on the builder
		this.builder.BuildCompleted += Build_Event_BuildCompleted;
		this.builder.BuildStatusChanged += Build_Event_BuildStatusChanged;
		this.builder.ErrorGenerated += Build_Event_ErrorGenerated;
		this.builder.WarningGenerated += Build_Event_WarningGenerated;
		this.builder.OutputGenerated += Build_Event_OutputGenerated;
		this.builder.NewLogMessage += Build_Event_NewLogMessage;

        // Default the output path to the [BundleRoot]\[Build] folder
        string buildDestFolder = System.IO.Path.Combine(this.buildBundle.RootPath, "Build").Replace('\\', '/');

        try
        {
            // Do the build!
            builder.Build(this.buildBundle, buildDestFolder, this.buildForceRebuild);
        }
		catch (ThreadAbortException)
		{
			// Aborted!
		}
        catch (Exception ex)
        {
            // The build failed (with an exception)
            Application.Invoke(delegate
            {
                // Display the exception
                MessageDialog md = new MessageDialog(
                    this, DialogFlags.Modal, MessageType.Error, ButtonsType.Ok,
                    "Failed to perform the content build" + System.Environment.NewLine + System.Environment.NewLine + Gdk.Framework.ExceptionUtilities.GetExceptionText(ex)
                    );
                md.Run();
                md.Destroy();
            });
        }
        finally
        {
            // Detach events
			this.builder.BuildCompleted -= Build_Event_BuildCompleted;
			this.builder.BuildStatusChanged -= Build_Event_BuildStatusChanged;
			this.builder.ErrorGenerated -= Build_Event_ErrorGenerated;
			this.builder.WarningGenerated -= Build_Event_WarningGenerated;
			this.builder.OutputGenerated -= Build_Event_OutputGenerated;
			this.builder.NewLogMessage -= Build_Event_NewLogMessage;
			
			this.builder = null;
        }

        // Signal the "Build Complete" to the UI thread
        Application.Invoke(delegate
        {
			// Log the final stats
			// ========= Build Finished: 3 succeeded, 2 failed, 2 skipped ==========
			string finalStats = "\n<b>===== Build Finished: ";
			finalStats += (this.buildNumAssetsSuccessful + this.buildNumAssetsSuccessWithWarnings).ToString() + " Succeeded, ";			
			finalStats += this.buildNumAssetsFailed > 0 ? "<color:#B00000>" : "<color:black>";
			finalStats += this.buildNumAssetsFailed + " Failed</color>, ";
			finalStats += this.buildNumAssetsSkipped + " Skipped =====</b>\n";
			this.BuildLog_Write(finalStats);
			
			// If there were errors, show the errors list
			if(this.buildNumAssetsFailed > 0)
				this.bottomNotebook.Page = 1;

			// We're done, null out the build data
            this.buildThread = null;
            this.buildBundle = null;

            // Update the UI State
            UpdateAppUIState();
        });
    }
	
	// *******************************************************************
	private void Build_Event_BuildCompleted(bool succeeded)
	{
	}
	
	// *******************************************************************
	private void Build_Event_BuildStatusChanged(Asset asset, BuildStatus status)
    {
		// Process this event on the UI thread
		Application.Invoke( delegate {
			
			// What status is this?
	    	switch(status)
			{
				case BuildStatus.Skipped:
					BuildLog_Write("<color:#404040>[" + asset.Path + "] - Skipping (asset is up-to-date)</color>\n"); 
					buildNumAssetsSkipped++;
					break;
				
				case BuildStatus.Building:
					BuildLog_Write("<color:#000000>[" + asset.Path + "] - Building... </color>"); 
					break;
				
				case BuildStatus.Success:
					BuildLog_Write("<color:#008000>Successful</color>\n"); 
					buildNumAssetsSuccessful++;
					break;
				
				case BuildStatus.SuccessWithWarnings:
					BuildLog_Write("<color:#DB8700>Warnings...</color>\n"); 
					buildNumAssetsSuccessWithWarnings++;
					break;
				
				case BuildStatus.Failed:
					BuildLog_Write("<color:#b00000>FAILED</color>\n"); 
					buildNumAssetsFailed++;
					break;
			}
			
		});
    }
	
	// *******************************************************************
	private void Build_Event_ErrorGenerated(Asset asset, string message)
    {
		// Process this event on the UI thread
		Application.Invoke( delegate {
				
			// Update the error counters
    		buildErrorsCount++;
			this.buildErrorsTabImage.Sensitive = true;
			this.buildErrorsTabLabel.Markup = "<b>" + buildErrorsCount.ToString() + "</b>";
			
			// Add a line to the error list
			BuildErrors_AddLine(true, message, asset.Name, asset.Path);
		});
	}
	
	// *******************************************************************
	private void Build_Event_WarningGenerated(Asset asset, string message)
    {
		// Process this event on the UI thread
		Application.Invoke( delegate {
				
			// Update the error counters
    		buildWarningsCount++;
			this.buildWarningsTabImage.Sensitive = true;
			this.buildWarningsTabLabel.Markup = "<b>" + buildWarningsCount.ToString() + "</b>";
			
			// Add a line to the warnings list
			BuildErrors_AddLine(false, message, asset.Name, asset.Path);
		});
	}
	
	// *******************************************************************
	private void Build_Event_OutputGenerated(Asset asset, string displayName, string fullOutputPath)
    {
		// Process this event on the UI thread
		Application.Invoke( delegate {
		
	    	buildOutputsCount++;
			
			// Add the output to our output list
			BuildOutputs_AddOutput(asset.Path, displayName, fullOutputPath);
		});
	}
	
	// *******************************************************************
    private void Build_Event_NewLogMessage(Asset asset, string gtml)
    {
		// Process this event on the UI thread
		Application.Invoke( delegate {
			
			string temp = gtml + "\n";
			
			// Append the log text to the asset's log
			asset.BuildLog.Append(temp);
			
			// Is this the active asset?
			if(this.ActiveAsset == asset)
			{
				// Write the line to the asset log view
				BuildAssetLog_Write(temp);
			}
			
		});
	}

}

/// <summary>
/// This intermediate object contains information for a folder node within the assets tree
/// </summary>
public class FolderItem
{
	public string Path;
	
	public FolderItem(string path)
	{
		this.Path = path;
	}
}