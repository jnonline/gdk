
// This file has been generated by the GUI designer. Do not modify.
namespace Gdk.Framework.UI.GtkSharp {
    public partial class ColorSelector {
        private global::Gtk.HBox hbox1;
        
        private global::Gtk.EventBox eventboxColor;
        
        private global::Gtk.Label labelColor;
        
        private global::Gtk.Entry entryColor;
        
        protected virtual void Build() {
            global::Stetic.Gui.Initialize(this);
            // Widget Gdk.Framework.UI.GtkSharp.ColorSelector
            global::Stetic.BinContainer.Attach(this);
            this.Name = "Gdk.Framework.UI.GtkSharp.ColorSelector";
            // Container child Gdk.Framework.UI.GtkSharp.ColorSelector.Gtk.Container+ContainerChild
            this.hbox1 = new global::Gtk.HBox();
            this.hbox1.Name = "hbox1";
            this.hbox1.Spacing = 1;
            // Container child hbox1.Gtk.Box+BoxChild
            this.eventboxColor = new global::Gtk.EventBox();
            this.eventboxColor.WidthRequest = 50;
            this.eventboxColor.Name = "eventboxColor";
            // Container child eventboxColor.Gtk.Container+ContainerChild
            this.labelColor = new global::Gtk.Label();
            this.labelColor.Name = "labelColor";
            this.eventboxColor.Add(this.labelColor);
            this.hbox1.Add(this.eventboxColor);
            global::Gtk.Box.BoxChild w2 = ((global::Gtk.Box.BoxChild)(this.hbox1[this.eventboxColor]));
            w2.Position = 0;
            w2.Expand = false;
            // Container child hbox1.Gtk.Box+BoxChild
            this.entryColor = new global::Gtk.Entry();
            this.entryColor.CanFocus = true;
            this.entryColor.Name = "entryColor";
            this.entryColor.IsEditable = true;
            this.entryColor.HasFrame = false;
            this.entryColor.InvisibleChar = '●';
            this.hbox1.Add(this.entryColor);
            global::Gtk.Box.BoxChild w3 = ((global::Gtk.Box.BoxChild)(this.hbox1[this.entryColor]));
            w3.Position = 1;
            this.Add(this.hbox1);
            if ((this.Child != null)) {
                this.Child.ShowAll();
            }
            this.Hide();
            this.eventboxColor.ButtonPressEvent += new global::Gtk.ButtonPressEventHandler(this.EventboxColor_ButtonPressEvent);
            this.entryColor.Changed += new global::System.EventHandler(this.EntryColor_Changed);
        }
    }
}
