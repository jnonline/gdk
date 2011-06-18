using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Gdk;
using Gdk.Content;

namespace Gdk.ContentStudio
{
    public partial class AssetsTree : UserControl
    {
        // Properties
        public Content.Content Content;
        public ContentForm OwnerContentForm;

        private MultiSelectTreeNode rootNode;
        private bool showAllFiles;

        /// <summary>
        /// Called when a node is selected in the tree
        /// </summary>
        public event Action SelectedNodesChanged;
        public List<MultiSelectTreeNode> SelectedNodes { get { return treeView.SelectedNodes; } }

        /// <summary>
        /// Constructor
        /// </summary>
        public AssetsTree()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Returns true if an asset node is selected in the tree
        /// </summary>
        public bool IsAssetSelected
        {
            get
            {
                return
                    treeView.SelectedNodes.Count == 1 &&
                    treeView.SelectedNode != null &&
                    treeView.SelectedNode.Tag != null &&
                    treeView.SelectedNode.Tag is AssetTag
                    ;
            }
        }

        /// <summary>
        /// Returns true if the content node is selected in the tree
        /// </summary>
        public bool IsContentSelected
        {
            get
            {
                return
                    treeView.SelectedNodes.Count == 1 &&
                    treeView.SelectedNode != null &&
                    treeView.SelectedNode.Tag != null &&
                    treeView.SelectedNode.Tag is ContentTag
                    ;
            }
        }

        #region Tree Node Management Methods

        /// <summary>
        /// Loads a set of content into the tree
        /// </summary>
        public void LoadContent(Content.Content content)
        {
            // Set the assets & content folder
            this.Content = content;

            // Use the Icon manager images as the image list for the tree
            treeView.ImageList = IconManager.ImageList;

            // Add the root "Assets" node
            rootNode = new MultiSelectTreeNode("Content");
            rootNode.Tag = new ContentTag(rootNode);
            rootNode.ImageIndex = rootNode.SelectedImageIndex = (int)Icons.FolderIcon;
            treeView.Nodes.Add(rootNode);

            // Parse the asset list into the tree
            foreach (Asset asset in content.Assets)
            {
                // Ensure the folders for the asset path exists
                string folderPath = Path.GetDirectoryName(asset.Path);
                MultiSelectTreeNode folderNode = GetFolderNode(folderPath);

                // Get the details for this asset's processor
                ProcessorDetails processorDetails = ProcessorManager.GetProcessorDetails(asset.ProcessorType);

                // Create an asset node
                MultiSelectTreeNode assetNode = new MultiSelectTreeNode(asset.Name);
                assetNode.Tag = new AssetTag(assetNode, asset);
                asset.Tag = assetNode.Tag;
                assetNode.ImageIndex = assetNode.SelectedImageIndex = (int)processorDetails.Icon;
                AddChildNode(folderNode, assetNode);
            }

            // Update the ghosts in the tree
            UpdateGhosts();

            // Expand the entire tree
            rootNode.ExpandAll();
        }

        /// <summary>
        /// Determines if the given node is a folder
        /// </summary>
        /// <param name="node"></param>
        /// <returns></returns>
        private bool IsFolderNode(MultiSelectTreeNode node)
        {
            return (node.Tag is FolderTag) || (node.Tag is GhostFolderTag);
        }

        /// <summary>
        /// This utility method adds a child node to the given parent node, properly sorting the child node
        /// upon insertion.  Organized alphabetically, folders first
        /// </summary>
        /// <param name="parentNode"></param>
        /// <param name="childNode"></param>
        private void AddChildNode(MultiSelectTreeNode parentNode, MultiSelectTreeNode node)
        {
            int insertionIndex = 0;

            // Is the new node a folder?
            if (IsFolderNode(node))
            {
                // Walk through the children nodes until we find where we need to insert the child
                foreach (MultiSelectTreeNode childNode in parentNode.Nodes)
                {
                    // Have we hit a non folder?
                    if (IsFolderNode(childNode) == false)
                        break;

                    // Have we found a folder the new node comes before?
                    else if (string.Compare(node.Text, childNode.Text, true) < 0)
                        break;

                    //  move to the next node
                    insertionIndex++;
                }
            }
            else
            {
                // Walk through the children nodes until we find where we need to insert the child
                foreach (MultiSelectTreeNode childNode in parentNode.Nodes)
                {
                    // Is this a non-file node?  That the new node comes before?
                    if (IsFolderNode(childNode) == false &&
                        string.Compare(node.Text, childNode.Text, true) < 0)
                        break;

                    //  move to the next node
                    insertionIndex++;
                }
            }

            // Insert the node
            parentNode.Nodes.Insert(insertionIndex, node);

        }

        /// <summary>
        /// Enables or Disables the Show-All-Files functionality of the tree
        /// </summary>
        public bool ShowAllFiles
        {
            set
            {
                
                // Set the new value
                bool oldShowAllFiles = showAllFiles;
                showAllFiles = value;

                // Was the tree setting changed?
                if (showAllFiles != oldShowAllFiles)
                {
                    // Update the ghosts in the tree
                    UpdateGhosts();
                }
            }
            get
            {
                return showAllFiles;
            }
        }

        /// <summary>
        /// Gets the MultiSelectTreeNode for the given path.  Creating it if necessary
        /// </summary>
        public MultiSelectTreeNode GetFolderNode(string path)
        {
            // Empty path = root node
            if(string.IsNullOrEmpty(path))
                return rootNode;

            // Get the parent node to this one
            string nodeName = Path.GetFileName(path);
            string parentPath = Path.GetDirectoryName(path);
            MultiSelectTreeNode parentNode = GetFolderNode(parentPath);
            
            // Does this node exist in the parent node?
            foreach(MultiSelectTreeNode childNode in parentNode.Nodes)
            {
                if(string.Compare(childNode.Text, nodeName, true) == 0)
                    return childNode;
            }

            // The node doesnt exist, so let's create it
            MultiSelectTreeNode newNode = new MultiSelectTreeNode(nodeName);
            newNode.Tag = new FolderTag(newNode);
            rootNode.ImageIndex = rootNode.SelectedImageIndex = (int)Icons.FolderIcon;

            // Add the new node to the parent
            AddChildNode(parentNode, newNode);

            return newNode;
        }


        /// <summary>
        /// Updates the ghosts in the tree (adding or removing based on the ShowAllFiles flag)
        /// </summary>
        private void UpdateGhosts()
        {
            // Are we showing all files?
            if (showAllFiles == true)
            {
                // Add all the ghost nodes starting from the root
                AddGhostsToNode(Content.RootPath, rootNode);
            }
            else
            {
                // Remove all the ghost nodes
                CleanGhostsFromNode(rootNode);
            }
        }

        /// <summary>
        /// Adds the entire file structure at the given path to the given node)
        /// </summary>
        private void AddGhostsToNode(string path, MultiSelectTreeNode parentNode)
        {
            // Loop through all the folders in this path
            foreach (string childPath in Directory.GetDirectories(path))
            {
                // Check if we already have a node for this directory
                string directoryName = Path.GetFileName(childPath);
                MultiSelectTreeNode directoryNode = GetChildNodeByName(parentNode, directoryName);

                // Do we not already have a node?
                if (directoryNode == null)
                {
                    // Build a new node for this directory
                    directoryNode = new MultiSelectTreeNode(directoryName, (int)Icons.GhostFolderIcon, (int)Icons.GhostFolderIcon);
                    directoryNode.Tag = new GhostFolderTag(directoryNode);
                    directoryNode.OriginalForeColor = SystemColors.GrayText;
                    
                    // Add the new node to the parent
                    AddChildNode(parentNode, directoryNode);
                }

                // Recurse to this node
                AddGhostsToNode(childPath, directoryNode);
            }

            // Loop through all the files in this path
            foreach (string childPath in Directory.GetFiles(path))
            {
                // Check if we already have a node for this file
                string fileName = Path.GetFileName(childPath);
                MultiSelectTreeNode fileNode = GetChildNodeByName(parentNode, fileName);

                // Do we not already have a node?
                if (fileNode == null)
                {
                    // Build a new node for this file
                    fileNode = new MultiSelectTreeNode(fileName, (int)Icons.GhostFileIcon, (int)Icons.GhostFileIcon);
                    fileNode.Tag = new GhostFileTag(fileNode);
                    fileNode.OriginalForeColor = SystemColors.GrayText;

                    // Add the new node to the parent
                    AddChildNode(parentNode, fileNode);
                }
            }
        }

        /// <summary>
        /// Removes all nodes that are not included in the project
        /// </summary>
        private void CleanGhostsFromNode(MultiSelectTreeNode parentNode)
        {
            for (int i = 0; i < parentNode.Nodes.Count; i++)
            {
                MultiSelectTreeNode childNode = parentNode.Nodes[i] as MultiSelectTreeNode;

                // Is this a ghost node?
                if (childNode.Tag is GhostTag)
                {
                    // Remove this node
                    parentNode.Nodes.RemoveAt(i);
                    i--;
                }
                else
                {
                    // Recurse to the children
                    CleanGhostsFromNode(childNode);
                }
            }
        }

        /// <summary>
        /// Gets a child node with a given name from the given parent node
        /// </summary>
        private MultiSelectTreeNode GetChildNodeByName(MultiSelectTreeNode parentNode, string childName)
        {
            foreach (MultiSelectTreeNode childNode in parentNode.Nodes)
                if (string.Compare(childNode.Text, childName, true) == 0)
                    return childNode;
            return null;
        }

        /// <summary>
        /// Gets the full relative path from the given node
        /// </summary>
        private string GetPathForNode(MultiSelectTreeNode node)
        {
            string path = node.Text;
            while (node.Parent != rootNode)
            {
                node = node.Parent as MultiSelectTreeNode;
                path = node.Text + "/" + path;
            }

            return path;
        }

        /// <summary>
        /// Turns the given node into a Ghost form of itself
        /// </summary>
        private void Ghost(MultiSelectTreeNode node)
        {
            // Skip this node if it is already a ghost
            if (node.Tag is GhostTag)
                return;

            // If the node is an asset, remove it from the assets container
            if (node.Tag is AssetTag)
            {
                AssetTag assetTag = node.Tag as AssetTag;
                this.Content.Assets.Remove(assetTag.Asset);
            }

            // If "ShowAllFiles" is off, then just remove the node
            if (showAllFiles == false)
            {
                node.Remove();
                return;
            }

            // Mark this node as a ghost
            if (node.Tag is FolderTag)
            {
                node.Tag = new GhostFolderTag(node);
                node.ImageIndex = node.SelectedImageIndex = (int)Icons.GhostFolderIcon;
                node.OriginalForeColor = SystemColors.GrayText;
            }
            else
            {
                node.Tag = new GhostFileTag(node);
                node.ImageIndex = node.SelectedImageIndex = (int)Icons.GhostFileIcon;
                node.OriginalForeColor = SystemColors.GrayText;
            }

            // Mark all the children as ghosts
            foreach (MultiSelectTreeNode childNode in node.Nodes)
                Ghost(childNode);
        }

        /// <summary>
        /// Turns the node into a non-ghost version of itself
        /// </summary>
        /// <param name="node"></param>
        private void Unghost(MultiSelectTreeNode node)
        {
            // Skip this node if it isnt a ghost node
            if (!(node.Tag is GhostTag))
                return;

            // Is this a file ghost?
            if (node.Tag is GhostFileTag)
            {
                // Get the processors for the given file extension
                string fileExtension = Path.GetExtension(node.Text);
                fileExtension = fileExtension.TrimStart('.');
                List<ProcessorDetails> processorDetailList = ProcessorManager.GetProcessorsByFileExtension(fileExtension);

                // If there is no supporting processor, throw an error
                if (processorDetailList.Count == 0)
                {
                    MessageBox.Show(
                        "There are no supporting asset processors for the given file extension [" + fileExtension + "]",
                        "No Processors",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error
                        );
                    return;
                }

                // Default to the first processor that supports the given extension
                ProcessorDetails processorDetails = processorDetailList[0];

                // Create a new asset
                Asset asset = new Asset();
                asset.Path = GetPathForNode(node);
                asset.ProcessorType = processorDetails.Name;
                Content.Assets.Add(asset);

                // Turn the MultiSelectTreeNode into an asset node
                node.Tag = new AssetTag(node, asset);
                asset.Tag = node.Tag;
                node.ImageIndex = node.SelectedImageIndex = (int)processorDetails.Icon;
                node.OriginalForeColor = SystemColors.ControlText;
            }
            else
            {
                // Unghost this folder node
                node.Tag = new FolderTag(node);
                node.ImageIndex = node.SelectedImageIndex = (int)Icons.FolderIcon;
                node.OriginalForeColor = SystemColors.ControlText;
            }

            // Unghost the parent folder
            if (node.Parent != null)
                Unghost(node.Parent as MultiSelectTreeNode);
        }

        /// <summary>
        /// This method re-adds all the assets from the tree to the content bundle, so the assets are in Tree order
        /// </summary>
        /// <param name="assets"></param>
        public void ReAddAssetsToContent()
        {
            // Clear all the current assets from the content 
            this.Content.Assets.Clear();

            // Recursively add the assets back
            RecurseAddAssetsToContent(rootNode);
        }

        /// <summary>
        /// Adds all the assets below the given node to the passed in assets set
        /// </summary>
        /// <param name="assets"></param>
        public void RecurseAddAssetsToContent(MultiSelectTreeNode node)
        {
            if (node.Tag is AssetTag)
                this.Content.Assets.Add(((AssetTag)node.Tag).Asset);

            foreach (MultiSelectTreeNode childNode in node.Nodes)
                RecurseAddAssetsToContent(childNode);
        }

        /// <summary>
        /// Sets the color markings on the asset tree back to defaults
        /// </summary>
        public void ResetNodeColors()
        {
            RecurseResetNodeColors(rootNode);
        }

        /// <summary>
        /// Sets the color markings on the asset tree back to defaults
        /// </summary>
        public void RecurseResetNodeColors(MultiSelectTreeNode node)
        {
            // Ghost nodes are grey
            if(node.Tag is GhostTag)
                node.OriginalForeColor = SystemColors.GrayText;
            else
                node.OriginalForeColor = SystemColors.ControlText;
            node.OriginalBackColor = SystemColors.Window;

            // Clear assetTag node logs
            if (node.Tag is AssetTag)
                ((AssetTag)node.Tag).Log = string.Empty;

            // Recurse to children
            foreach (MultiSelectTreeNode childNode in node.Nodes)
                RecurseResetNodeColors(childNode);
        }

        #endregion

        #region Event Handlers

        /// <summary>
        /// Called when the user clicks "Include" from the context menu
        /// </summary>
        private void contextMenuInclude_Click(object sender, EventArgs e)
        {
            // Are there no nodes selected?
            if (treeView.SelectedNodes.Count == 0)
                return;

            // Loop through the selected nodes
            foreach (MultiSelectTreeNode selectedNode in treeView.SelectedNodes)
            {
                // Is this a GhostFile node?
                if (selectedNode.Tag is GhostFileTag)
                {
                    // Unghost this node
                    Unghost(selectedNode);
                }
            }

            // Call the SelectedNodesChanged event
            if (SelectedNodesChanged != null)
                this.SelectedNodesChanged();
        }

        /// <summary>
        /// Called when the user clicks "Exclude" from the context menu
        /// </summary>
        private void contextMenuExclude_Click(object sender, EventArgs e)
        {
            // Are there no nodes selected?
            if (treeView.SelectedNodes.Count == 0)
                return;

            // Are there more than 1 node or is that 1 node a group?
            if (treeView.SelectedNodes.Count > 0 || treeView.SelectedNodes[0].Nodes.Count > 0)
            {
                // Check with the user if they really want to do this
                DialogResult result = MessageBox.Show(
                    "Do you really want to exclude these nodes (including all child nodes)?", 
                    "Confirm Exclude?", 
                    MessageBoxButtons.YesNo, 
                    MessageBoxIcon.Question, 
                    MessageBoxDefaultButton.Button2
                    );

                // Bail early 
                if (result == DialogResult.No)
                    return;
            }

            // Make a copy of the selected nodes
            List<MultiSelectTreeNode> nodesToGhost = new List<MultiSelectTreeNode>();
            nodesToGhost.AddRange(treeView.SelectedNodes);

            // Loop through the selected nodes
            foreach (MultiSelectTreeNode selectedNode in nodesToGhost)
            {
                // Is this not a ghost node & not the root node?
                if (selectedNode != rootNode && !(selectedNode.Tag is GhostTag))
                {
                    // Ghost this node
                    Ghost(selectedNode);
                }
            }

            // Call the SelectedNodesChanged event
            if (SelectedNodesChanged != null)
                this.SelectedNodesChanged();
        }

        /// <summary>
        /// Called when the user clicks "Build" from the context menu
        /// </summary>
        private void contextMenuBuild_Click(object sender, EventArgs e)
        {
            List<Asset> assetsToBuild = new List<Asset>();

            // Loop through the selected nodes
            foreach (MultiSelectTreeNode node in SelectedNodes)
            {
                RecurseGetAssets(node, assetsToBuild);
            }

            // Do a build of this assets
            if (assetsToBuild.Count > 0)
            {
                this.OwnerContentForm.PerformBuild(assetsToBuild, true);
            }
        }

        /// <summary>
        /// Recursively gets all the assets within the given tree node
        /// </summary>
        private void RecurseGetAssets(MultiSelectTreeNode node, List<Asset> assets)
        {
            // Is this an asset node?
            if (node.Tag is AssetTag)
            {
                // Get the asset in this node
                AssetTag assetTag = node.Tag as AssetTag;
                if (assetTag != null)
                {
                    // Make sure we only include an asset once!
                    if (assets.Contains(assetTag.Asset) == false)
                        assets.Add(assetTag.Asset);
                }
            }
            // Is this a folder node?
            else if (node.Tag is FolderTag)
            {
                // Recurse to the child nodes
                foreach (MultiSelectTreeNode childNode in node.Nodes)
                    RecurseGetAssets(childNode, assets);
            }
        }

        /// <summary>
        /// Called just before the context menu opens
        /// </summary>
        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {
            // Get the current app mode
            bool inEditMode = this.OwnerContentForm.InEditMode;

            bool hasIncludableNode = false;
            bool hasExcludableNode = false;
            bool hasBuildableNode = false;

            // Loop through all the currently selected nodes, and find stats about them
            foreach (MultiSelectTreeNode node in treeView.SelectedNodes)
            {
                // Includable:  Node IS a ghost file
                if (node.Tag is GhostFileTag)
                    hasIncludableNode = true;
                    
                // Excludable:  Node IS NOT a ghost & Node IS NOT the root node
                if (!(node.Tag is GhostTag) && node != rootNode)
                    hasExcludableNode = true;

                // Buildable:   Node IS an asset node or a folder node
                if (node.Tag is AssetTag || node.Tag is FolderTag)
                    hasBuildableNode = true;
            }

            // Set the context menu option availabilities
            contextMenuInclude.Available =
                hasIncludableNode &&
                inEditMode
                ;
            contextMenuExclude.Available =
                hasExcludableNode &&
                inEditMode
                ;

            contextMenuBuild.Available =
                hasBuildableNode &&
                !inEditMode &&
                !this.OwnerContentForm.IsBuilding
                ;

            // If there are no visible menu items, just cancel the open event
            if (contextMenuInclude.Available == false &&
                contextMenuExclude.Available == false &&
                contextMenuBuild.Available == false)
            {
                e.Cancel = true;
                return;
            }

            e.Cancel = false;
        }

        /// <summary>
        /// Called when the user clicks anywhere in the treeview
        /// </summary>
        private void treeView_MouseClick(object sender, MouseEventArgs e)
        {
            /*
            // Get the node where the user clicked
            MultiSelectTreeNode clickedNode = treeView.GetNodeAt(e.Location);

            // If the node isnt selected, select it
            if (clickedNode != treeView.SelectedNode)
                treeView.SelectedNode = clickedNode;
            */
        }

        /// <summary>
        /// Called after the selected node has changed
        /// </summary>
        private void treeView_SelectedNodesChanged()
        {
            // Call the SelectedNodesChanged event
            if (SelectedNodesChanged != null)
                this.SelectedNodesChanged();
        }

        #endregion
    }

    #region Node Tag Classes
    // =============================================================================

    public class NodeTag
    {
        private MultiSelectTreeNode MultiSelectTreeNode;
        public MultiSelectTreeNode Node { get { return MultiSelectTreeNode; } }

        public NodeTag(MultiSelectTreeNode node)
        {
            this.MultiSelectTreeNode = node;
        }

        virtual public string NodeType { get { return "Node"; } }
        virtual public string NodeName { get { return MultiSelectTreeNode.Text; } }
    }

    public class ContentTag : NodeTag
    {
        public ContentTag(MultiSelectTreeNode node)
            : base(node)
        {
        }

        override public string NodeType { get { return "Content Settings"; } }
        override public string NodeName { get { return ""; } }
    }

    public class AssetTag : NodeTag
    {
        public Asset Asset;
        public string Log = string.Empty;

        public AssetTag(MultiSelectTreeNode node, Asset asset)
            : base(node)
        {
            this.Asset = asset;
        }

        override public string NodeType { get { return "Asset"; } }
        override public string NodeName { get { return Asset.Name; } }
    }

    public class FolderTag : NodeTag
    {
        public FolderTag(MultiSelectTreeNode node)
            : base(node)
        {
        }

        override public string NodeType { get { return "Folder"; } }
    }

    public abstract class GhostTag : NodeTag
    {
        public GhostTag(MultiSelectTreeNode node)
            : base(node)
        {
        }
    }

    public class GhostFolderTag : GhostTag
    {
        public GhostFolderTag(MultiSelectTreeNode node)
            : base(node)
        {
        }

        override public string NodeType { get { return "Unused Folder"; } }
    }

    public class GhostFileTag : GhostTag
    {
        public GhostFileTag(MultiSelectTreeNode node)
            : base(node)
        {
        }

        override public string NodeType { get { return "Unused File"; } }
    }

    #endregion
}
