using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Gdk.ContentStudio
{
    // ========================================
    // This TreeView derivation supports multi-select, 
    //  via Shift & Ctrl clicking on the nodes..
    //
    // The .SelectedNodes property exposes the nodes.
    //
    // Adapted From:
    //    http://www.arstdesign.com/articles/treeviewms.html
    // ========================================
    
    public class MultiSelectTreeView : TreeView
    {
        // Properties
        protected List<MultiSelectTreeNode> selectedNodes = new List<MultiSelectTreeNode>();
        protected MultiSelectTreeNode lastNode;
        protected MultiSelectTreeNode firstNode;

        public event Action SelectedNodesChanged;

        /// <summary>
        /// Gets and sets the selected nodes in the treeview
        /// </summary>
        public List<MultiSelectTreeNode> SelectedNodes
        {
            get
            {
                return selectedNodes;
            }
            set
            {
                // Remove highlighting from the currently selected nodes
                RemoveSelectedNodesHighlights();

                // Set the new list of highlighted nodes
                selectedNodes.Clear();
                selectedNodes.AddRange(value);

                // Highlight the newly selected nodes
                SetSelectedNodesHighlights();

                // Fire the changed event
                if (SelectedNodesChanged != null)
                    SelectedNodesChanged();
            }
        }

        /// <summary>
        /// Called just before a node is selected
        /// </summary>
        protected override void OnBeforeSelect(TreeViewCancelEventArgs e)
        {
            // e.Node is the current node exposed by the base TreeView control
            base.OnBeforeSelect(e);

            bool bControl = (ModifierKeys == Keys.Control);
            bool bShift = (ModifierKeys == Keys.Shift);

            MultiSelectTreeNode node = e.Node as MultiSelectTreeNode;

            // selecting twice the node while pressing CTRL ?
            if (bControl && selectedNodes.Contains(node))
            {
                // unselect it (let framework know we don't want selection this time)
                e.Cancel = true;

                // update nodes
                RemoveSelectedNodesHighlights();
                selectedNodes.Remove(node);
                SetSelectedNodesHighlights();

                // Fire the changed event
                if (SelectedNodesChanged != null)
                    SelectedNodesChanged();

                return;
            }

            lastNode = node;
            if (!bShift) 
                firstNode = node; // store begin of shift sequence
        }

        /// <summary>
        /// Called just after a node is selected
        /// </summary>
        protected override void OnAfterSelect(TreeViewEventArgs e)
        {
            // e.Node is the current node exposed by the base TreeView control

            base.OnAfterSelect(e);

            bool bControl = (ModifierKeys == Keys.Control);
            bool bShift = (ModifierKeys == Keys.Shift);

            MultiSelectTreeNode node = e.Node as MultiSelectTreeNode;

            if (bControl)
            {
                if (!selectedNodes.Contains(node)) // new node ?
                {
                    selectedNodes.Add(node);
                }
                else  // not new, remove it from the collection
                {
                    RemoveSelectedNodesHighlights();
                    selectedNodes.Remove(node);
                }
                SetSelectedNodesHighlights();
            }
            else
            {
                if (bShift)
                {
                    Queue<MultiSelectTreeNode> myQueue = new Queue<MultiSelectTreeNode>();

                    MultiSelectTreeNode uppernode = firstNode;
                    MultiSelectTreeNode bottomnode = node;

                    // case 1 : begin and end nodes are parent
                    bool bParent = IsParent(firstNode, node); // is m_firstNode parent (direct or not) of e.Node
                    if (!bParent)
                    {
                        bParent = IsParent(bottomnode, uppernode);
                        if (bParent) // swap nodes
                        {
                            MultiSelectTreeNode t = uppernode;
                            uppernode = bottomnode;
                            bottomnode = t;
                        }
                    }
                    if (bParent)
                    {
                        MultiSelectTreeNode n = bottomnode;
                        while (n != uppernode.Parent)
                        {
                            if (!selectedNodes.Contains(n)) // new node ?
                                myQueue.Enqueue(n);

                            n = n.Parent as MultiSelectTreeNode;
                        }
                    }
                    // case 2 : nor the begin nor the end node are descendant one another
                    else
                    {
                        if ((uppernode.Parent == null && bottomnode.Parent == null) || (uppernode.Parent != null && uppernode.Parent.Nodes.Contains(bottomnode))) // are they siblings ?
                        {
                            int nIndexUpper = uppernode.Index;
                            int nIndexBottom = bottomnode.Index;
                            if (nIndexBottom < nIndexUpper) // reversed?
                            {
                                MultiSelectTreeNode t = uppernode;
                                uppernode = bottomnode;
                                bottomnode = t;
                                nIndexUpper = uppernode.Index;
                                nIndexBottom = bottomnode.Index;
                            }

                            MultiSelectTreeNode n = uppernode;
                            while (nIndexUpper <= nIndexBottom)
                            {
                                if (!selectedNodes.Contains(n)) // new node ?
                                    myQueue.Enqueue(n);

                                n = n.NextNode as MultiSelectTreeNode;

                                nIndexUpper++;
                            } // end while

                        }
                        else
                        {
                            if (!selectedNodes.Contains(uppernode)) myQueue.Enqueue(uppernode);
                            if (!selectedNodes.Contains(bottomnode)) myQueue.Enqueue(bottomnode);
                        }

                    }

                    selectedNodes.AddRange(myQueue);

                    SetSelectedNodesHighlights();
                    firstNode = node; // let us chain several SHIFTs if we like it

                } // end if m_bShift
                else
                {
                    // in the case of a simple click, just add this item
                    if (selectedNodes != null && selectedNodes.Count > 0)
                    {
                        RemoveSelectedNodesHighlights();
                        selectedNodes.Clear();
                    }
                    selectedNodes.Add(node);
                }
            }

            // Fire the changed event
            if (SelectedNodesChanged != null)
                SelectedNodesChanged();
        }

        /// <summary>
        /// Determines if the given parent node is an ancestor of the given child
        /// </summary>
        protected bool IsParent(MultiSelectTreeNode parentNode, MultiSelectTreeNode childNode)
        {
            if (parentNode == childNode)
                return true;

            MultiSelectTreeNode n = childNode;
            bool bFound = false;
            while (!bFound && n != null)
            {
                n = n.Parent as MultiSelectTreeNode;
                bFound = (n == parentNode);
            }
            return bFound;
        }

        /// <summary>
        /// Applies highlighting to the currently selected nodes
        /// </summary>
        protected void SetSelectedNodesHighlights()
        {
            // Loop through the selected nodes
            foreach (MultiSelectTreeNode node in selectedNodes)
            {
                // Apply highlighting colors
                node.BackColor = SystemColors.Highlight;
                node.ForeColor = SystemColors.HighlightText;
                node.IsHighlighted = true;
            }
        }

        /// <summary>
        /// Removes highlighting from the currently selected nodes
        /// </summary>
        protected void RemoveSelectedNodesHighlights()
        {
            // Loop through the selected nodes
            foreach (MultiSelectTreeNode node in selectedNodes)
            {
                // Apply the original colors of the node
                node.BackColor = node.OriginalBackColor;
                node.ForeColor = node.OriginalForeColor;
                node.IsHighlighted = false;
            }
        }

    }

    /// <summary>
    /// These tree nodes are intended to be used with the multiselect tree
    /// </summary>
    public class MultiSelectTreeNode : TreeNode
    {
        public bool IsHighlighted = false;

        private System.Drawing.Color originalBackColor = System.Drawing.Color.White;
        public System.Drawing.Color OriginalBackColor
        {
            get { return this.originalBackColor; }
            set 
            {
                this.originalBackColor = value;
                if (this.IsSelected == false)
                    this.BackColor = this.originalBackColor;
            }
        }

        private System.Drawing.Color originalForeColor = System.Drawing.Color.Black;
        public System.Drawing.Color OriginalForeColor
        {
            get { return this.originalForeColor; }
            set
            {
                this.originalForeColor = value;
                if (this.IsSelected == false)
                    this.ForeColor = this.originalForeColor;
            }
        }

        public MultiSelectTreeNode(string text)
            : base(text)
        {
        }

        public MultiSelectTreeNode(string text, int imageIndex, int selectedImageIndex)
            : base(text, imageIndex, selectedImageIndex)
        {
        }
    }
}
