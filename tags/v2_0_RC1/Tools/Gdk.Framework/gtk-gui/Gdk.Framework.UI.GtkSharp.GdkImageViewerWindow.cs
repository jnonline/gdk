
// This file has been generated by the GUI designer. Do not modify.
namespace Gdk.Framework.UI.GtkSharp
{
	public partial class GdkImageViewerWindow
	{
		private global::Gtk.UIManager UIManager;
		private global::Gtk.Action zoomInAction;
		private global::Gtk.Action zoomOutAction;
		private global::Gtk.Action zoom100Action;
		private global::Gtk.VBox vbox1;
		private global::Gtk.HBox hbox3;
		private global::Gtk.VBox vbox3;
		private global::Gtk.Label label2;
		private global::Gtk.HBox hbox1;
		private global::Gtk.ComboBox comboBackgroundMode;
		private global::Gtk.EventBox backgroundColorEventBox1;
		private global::Gtk.EventBox backgroundColorEventBox2;
		private global::Gtk.VSeparator vseparator1;
		private global::Gtk.VBox vbox2;
		private global::Gtk.Label label3;
		private global::Gtk.ComboBox comboZoom;
		private global::Gtk.VSeparator vseparator2;
		private global::Gtk.VBox vbox4;
		private global::Gtk.Label label1;
		private global::Gtk.ComboBox comboScaling;
		private global::Gtk.VSeparator vseparator3;
		private global::Gtk.VBox vbox5;
		private global::Gtk.Label label4;
		private global::Gtk.HBox hbox4;
		private global::Gtk.Button buttonShiftColor;
		private global::Gtk.Button buttonInvertColor;
		private global::Gtk.VSeparator vseparator4;
		private global::Gtk.ScrolledWindow GtkScrolledWindow;
		private global::Gtk.Image imageDisplay;
		private global::Gtk.HBox hbox2;
		private global::Gtk.Label labelBottom1;
        
		protected virtual void Build ()
		{
			global::Stetic.Gui.Initialize (this);
			// Widget Gdk.Framework.UI.GtkSharp.GdkImageViewerWindow
			this.UIManager = new global::Gtk.UIManager ();
			global::Gtk.ActionGroup w1 = new global::Gtk.ActionGroup ("Default");
			this.zoomInAction = new global::Gtk.Action ("zoomInAction", null, null, "gtk-zoom-in");
			w1.Add (this.zoomInAction, null);
			this.zoomOutAction = new global::Gtk.Action ("zoomOutAction", null, null, "gtk-zoom-out");
			w1.Add (this.zoomOutAction, null);
			this.zoom100Action = new global::Gtk.Action ("zoom100Action", null, null, "gtk-zoom-100");
			w1.Add (this.zoom100Action, null);
			this.UIManager.InsertActionGroup (w1, 0);
			this.AddAccelGroup (this.UIManager.AccelGroup);
			this.Name = "Gdk.Framework.UI.GtkSharp.GdkImageViewerWindow";
			this.Title = global::Mono.Unix.Catalog.GetString ("GdkImageViewerWindow");
			this.WindowPosition = ((global::Gtk.WindowPosition)(4));
			// Container child Gdk.Framework.UI.GtkSharp.GdkImageViewerWindow.Gtk.Container+ContainerChild
			this.vbox1 = new global::Gtk.VBox ();
			this.vbox1.Name = "vbox1";
			this.vbox1.Spacing = 2;
			// Container child vbox1.Gtk.Box+BoxChild
			this.hbox3 = new global::Gtk.HBox ();
			this.hbox3.Name = "hbox3";
			this.hbox3.Spacing = 4;
			// Container child hbox3.Gtk.Box+BoxChild
			this.vbox3 = new global::Gtk.VBox ();
			this.vbox3.Name = "vbox3";
			this.vbox3.Spacing = 6;
			// Container child vbox3.Gtk.Box+BoxChild
			this.label2 = new global::Gtk.Label ();
			this.label2.Name = "label2";
			this.label2.LabelProp = global::Mono.Unix.Catalog.GetString ("Background");
			this.vbox3.Add (this.label2);
			global::Gtk.Box.BoxChild w2 = ((global::Gtk.Box.BoxChild)(this.vbox3 [this.label2]));
			w2.Position = 0;
			w2.Expand = false;
			w2.Fill = false;
			// Container child vbox3.Gtk.Box+BoxChild
			this.hbox1 = new global::Gtk.HBox ();
			this.hbox1.Name = "hbox1";
			this.hbox1.Spacing = 6;
			// Container child hbox1.Gtk.Box+BoxChild
			this.comboBackgroundMode = global::Gtk.ComboBox.NewText ();
			this.comboBackgroundMode.AppendText (global::Mono.Unix.Catalog.GetString ("Checker"));
			this.comboBackgroundMode.AppendText (global::Mono.Unix.Catalog.GetString ("Solid"));
			this.comboBackgroundMode.AppendText (global::Mono.Unix.Catalog.GetString ("Circles"));
			this.comboBackgroundMode.Name = "comboBackgroundMode";
			this.comboBackgroundMode.Active = 0;
			this.hbox1.Add (this.comboBackgroundMode);
			global::Gtk.Box.BoxChild w3 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.comboBackgroundMode]));
			w3.Position = 0;
			w3.Expand = false;
			w3.Fill = false;
			// Container child hbox1.Gtk.Box+BoxChild
			this.backgroundColorEventBox1 = new global::Gtk.EventBox ();
			this.backgroundColorEventBox1.WidthRequest = 30;
			this.backgroundColorEventBox1.HeightRequest = 20;
			this.backgroundColorEventBox1.Name = "backgroundColorEventBox1";
			this.backgroundColorEventBox1.BorderWidth = ((uint)(1));
			this.hbox1.Add (this.backgroundColorEventBox1);
			global::Gtk.Box.BoxChild w4 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.backgroundColorEventBox1]));
			w4.Position = 1;
			w4.Expand = false;
			// Container child hbox1.Gtk.Box+BoxChild
			this.backgroundColorEventBox2 = new global::Gtk.EventBox ();
			this.backgroundColorEventBox2.WidthRequest = 30;
			this.backgroundColorEventBox2.HeightRequest = 20;
			this.backgroundColorEventBox2.Name = "backgroundColorEventBox2";
			this.backgroundColorEventBox2.BorderWidth = ((uint)(1));
			this.hbox1.Add (this.backgroundColorEventBox2);
			global::Gtk.Box.BoxChild w5 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.backgroundColorEventBox2]));
			w5.Position = 2;
			w5.Expand = false;
			this.vbox3.Add (this.hbox1);
			global::Gtk.Box.BoxChild w6 = ((global::Gtk.Box.BoxChild)(this.vbox3 [this.hbox1]));
			w6.Position = 1;
			w6.Expand = false;
			w6.Fill = false;
			this.hbox3.Add (this.vbox3);
			global::Gtk.Box.BoxChild w7 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vbox3]));
			w7.Position = 0;
			w7.Expand = false;
			w7.Fill = false;
			w7.Padding = ((uint)(4));
			// Container child hbox3.Gtk.Box+BoxChild
			this.vseparator1 = new global::Gtk.VSeparator ();
			this.vseparator1.Name = "vseparator1";
			this.hbox3.Add (this.vseparator1);
			global::Gtk.Box.BoxChild w8 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vseparator1]));
			w8.Position = 1;
			w8.Expand = false;
			w8.Fill = false;
			// Container child hbox3.Gtk.Box+BoxChild
			this.vbox2 = new global::Gtk.VBox ();
			this.vbox2.Name = "vbox2";
			this.vbox2.Spacing = 6;
			// Container child vbox2.Gtk.Box+BoxChild
			this.label3 = new global::Gtk.Label ();
			this.label3.Name = "label3";
			this.label3.LabelProp = global::Mono.Unix.Catalog.GetString ("Zoom");
			this.vbox2.Add (this.label3);
			global::Gtk.Box.BoxChild w9 = ((global::Gtk.Box.BoxChild)(this.vbox2 [this.label3]));
			w9.Position = 0;
			w9.Expand = false;
			w9.Fill = false;
			// Container child vbox2.Gtk.Box+BoxChild
			this.comboZoom = global::Gtk.ComboBox.NewText ();
			this.comboZoom.AppendText (global::Mono.Unix.Catalog.GetString ("12%"));
			this.comboZoom.AppendText (global::Mono.Unix.Catalog.GetString ("25%"));
			this.comboZoom.AppendText (global::Mono.Unix.Catalog.GetString ("50%"));
			this.comboZoom.AppendText (global::Mono.Unix.Catalog.GetString ("75%"));
			this.comboZoom.AppendText (global::Mono.Unix.Catalog.GetString ("100%"));
			this.comboZoom.AppendText (global::Mono.Unix.Catalog.GetString ("200%"));
			this.comboZoom.AppendText (global::Mono.Unix.Catalog.GetString ("400%"));
			this.comboZoom.AppendText (global::Mono.Unix.Catalog.GetString ("600%"));
			this.comboZoom.AppendText (global::Mono.Unix.Catalog.GetString ("800%"));
			this.comboZoom.Name = "comboZoom";
			this.comboZoom.Active = 4;
			this.vbox2.Add (this.comboZoom);
			global::Gtk.Box.BoxChild w10 = ((global::Gtk.Box.BoxChild)(this.vbox2 [this.comboZoom]));
			w10.Position = 1;
			w10.Expand = false;
			w10.Fill = false;
			this.hbox3.Add (this.vbox2);
			global::Gtk.Box.BoxChild w11 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vbox2]));
			w11.Position = 2;
			w11.Expand = false;
			w11.Fill = false;
			// Container child hbox3.Gtk.Box+BoxChild
			this.vseparator2 = new global::Gtk.VSeparator ();
			this.vseparator2.Name = "vseparator2";
			this.hbox3.Add (this.vseparator2);
			global::Gtk.Box.BoxChild w12 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vseparator2]));
			w12.Position = 3;
			w12.Expand = false;
			w12.Fill = false;
			// Container child hbox3.Gtk.Box+BoxChild
			this.vbox4 = new global::Gtk.VBox ();
			this.vbox4.Name = "vbox4";
			this.vbox4.Spacing = 6;
			// Container child vbox4.Gtk.Box+BoxChild
			this.label1 = new global::Gtk.Label ();
			this.label1.Name = "label1";
			this.label1.LabelProp = global::Mono.Unix.Catalog.GetString ("Scaling");
			this.vbox4.Add (this.label1);
			global::Gtk.Box.BoxChild w13 = ((global::Gtk.Box.BoxChild)(this.vbox4 [this.label1]));
			w13.Position = 0;
			w13.Expand = false;
			w13.Fill = false;
			// Container child vbox4.Gtk.Box+BoxChild
			this.comboScaling = global::Gtk.ComboBox.NewText ();
			this.comboScaling.AppendText (global::Mono.Unix.Catalog.GetString ("Bilinear"));
			this.comboScaling.AppendText (global::Mono.Unix.Catalog.GetString ("Nearest"));
			this.comboScaling.AppendText (global::Mono.Unix.Catalog.GetString ("Tiles"));
			this.comboScaling.AppendText (global::Mono.Unix.Catalog.GetString ("Hyper"));
			this.comboScaling.Name = "comboScaling";
			this.comboScaling.Active = 0;
			this.vbox4.Add (this.comboScaling);
			global::Gtk.Box.BoxChild w14 = ((global::Gtk.Box.BoxChild)(this.vbox4 [this.comboScaling]));
			w14.Position = 1;
			w14.Expand = false;
			w14.Fill = false;
			this.hbox3.Add (this.vbox4);
			global::Gtk.Box.BoxChild w15 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vbox4]));
			w15.Position = 4;
			w15.Expand = false;
			w15.Fill = false;
			// Container child hbox3.Gtk.Box+BoxChild
			this.vseparator3 = new global::Gtk.VSeparator ();
			this.vseparator3.Name = "vseparator3";
			this.hbox3.Add (this.vseparator3);
			global::Gtk.Box.BoxChild w16 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vseparator3]));
			w16.Position = 5;
			w16.Expand = false;
			w16.Fill = false;
			// Container child hbox3.Gtk.Box+BoxChild
			this.vbox5 = new global::Gtk.VBox ();
			this.vbox5.Name = "vbox5";
			this.vbox5.Spacing = 6;
			// Container child vbox5.Gtk.Box+BoxChild
			this.label4 = new global::Gtk.Label ();
			this.label4.Name = "label4";
			this.label4.LabelProp = global::Mono.Unix.Catalog.GetString ("Colors");
			this.vbox5.Add (this.label4);
			global::Gtk.Box.BoxChild w17 = ((global::Gtk.Box.BoxChild)(this.vbox5 [this.label4]));
			w17.Position = 0;
			w17.Expand = false;
			w17.Fill = false;
			// Container child vbox5.Gtk.Box+BoxChild
			this.hbox4 = new global::Gtk.HBox ();
			this.hbox4.Name = "hbox4";
			this.hbox4.Spacing = 6;
			// Container child hbox4.Gtk.Box+BoxChild
			this.buttonShiftColor = new global::Gtk.Button ();
			this.buttonShiftColor.CanFocus = true;
			this.buttonShiftColor.Name = "buttonShiftColor";
			this.buttonShiftColor.UseUnderline = true;
			// Container child buttonShiftColor.Gtk.Container+ContainerChild
			global::Gtk.Alignment w18 = new global::Gtk.Alignment (0.5F, 0.5F, 0F, 0F);
			// Container child GtkAlignment.Gtk.Container+ContainerChild
			global::Gtk.HBox w19 = new global::Gtk.HBox ();
			w19.Spacing = 2;
			// Container child GtkHBox.Gtk.Container+ContainerChild
			global::Gtk.Image w20 = new global::Gtk.Image ();
			w20.Pixbuf = global::Stetic.IconLoader.LoadIcon (this, "gtk-sort-ascending", global::Gtk.IconSize.Menu);
			w19.Add (w20);
			// Container child GtkHBox.Gtk.Container+ContainerChild
			global::Gtk.Label w22 = new global::Gtk.Label ();
			w22.LabelProp = global::Mono.Unix.Catalog.GetString ("Shift");
			w22.UseUnderline = true;
			w19.Add (w22);
			w18.Add (w19);
			this.buttonShiftColor.Add (w18);
			this.hbox4.Add (this.buttonShiftColor);
			global::Gtk.Box.BoxChild w26 = ((global::Gtk.Box.BoxChild)(this.hbox4 [this.buttonShiftColor]));
			w26.Position = 0;
			w26.Expand = false;
			w26.Fill = false;
			// Container child hbox4.Gtk.Box+BoxChild
			this.buttonInvertColor = new global::Gtk.Button ();
			this.buttonInvertColor.CanFocus = true;
			this.buttonInvertColor.Name = "buttonInvertColor";
			this.buttonInvertColor.UseUnderline = true;
			// Container child buttonInvertColor.Gtk.Container+ContainerChild
			global::Gtk.Alignment w27 = new global::Gtk.Alignment (0.5F, 0.5F, 0F, 0F);
			// Container child GtkAlignment.Gtk.Container+ContainerChild
			global::Gtk.HBox w28 = new global::Gtk.HBox ();
			w28.Spacing = 2;
			// Container child GtkHBox.Gtk.Container+ContainerChild
			global::Gtk.Image w29 = new global::Gtk.Image ();
			w29.Pixbuf = global::Stetic.IconLoader.LoadIcon (this, "gtk-refresh", global::Gtk.IconSize.Menu);
			w28.Add (w29);
			// Container child GtkHBox.Gtk.Container+ContainerChild
			global::Gtk.Label w31 = new global::Gtk.Label ();
			w31.LabelProp = global::Mono.Unix.Catalog.GetString ("Invert");
			w31.UseUnderline = true;
			w28.Add (w31);
			w27.Add (w28);
			this.buttonInvertColor.Add (w27);
			this.hbox4.Add (this.buttonInvertColor);
			global::Gtk.Box.BoxChild w35 = ((global::Gtk.Box.BoxChild)(this.hbox4 [this.buttonInvertColor]));
			w35.Position = 1;
			w35.Expand = false;
			w35.Fill = false;
			this.vbox5.Add (this.hbox4);
			global::Gtk.Box.BoxChild w36 = ((global::Gtk.Box.BoxChild)(this.vbox5 [this.hbox4]));
			w36.Position = 1;
			w36.Expand = false;
			w36.Fill = false;
			this.hbox3.Add (this.vbox5);
			global::Gtk.Box.BoxChild w37 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vbox5]));
			w37.Position = 6;
			w37.Expand = false;
			w37.Fill = false;
			// Container child hbox3.Gtk.Box+BoxChild
			this.vseparator4 = new global::Gtk.VSeparator ();
			this.vseparator4.Name = "vseparator4";
			this.hbox3.Add (this.vseparator4);
			global::Gtk.Box.BoxChild w38 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vseparator4]));
			w38.Position = 7;
			w38.Expand = false;
			w38.Fill = false;
			this.vbox1.Add (this.hbox3);
			global::Gtk.Box.BoxChild w39 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.hbox3]));
			w39.Position = 0;
			w39.Expand = false;
			w39.Fill = false;
			w39.Padding = ((uint)(2));
			// Container child vbox1.Gtk.Box+BoxChild
			this.GtkScrolledWindow = new global::Gtk.ScrolledWindow ();
			this.GtkScrolledWindow.Name = "GtkScrolledWindow";
			this.GtkScrolledWindow.ShadowType = ((global::Gtk.ShadowType)(1));
			// Container child GtkScrolledWindow.Gtk.Container+ContainerChild
			global::Gtk.Viewport w40 = new global::Gtk.Viewport ();
			w40.ShadowType = ((global::Gtk.ShadowType)(0));
			// Container child GtkViewport.Gtk.Container+ContainerChild
			this.imageDisplay = new global::Gtk.Image ();
			this.imageDisplay.Name = "imageDisplay";
			w40.Add (this.imageDisplay);
			this.GtkScrolledWindow.Add (w40);
			this.vbox1.Add (this.GtkScrolledWindow);
			global::Gtk.Box.BoxChild w43 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.GtkScrolledWindow]));
			w43.Position = 1;
			// Container child vbox1.Gtk.Box+BoxChild
			this.hbox2 = new global::Gtk.HBox ();
			this.hbox2.Name = "hbox2";
			this.hbox2.Spacing = 6;
			this.hbox2.BorderWidth = ((uint)(1));
			// Container child hbox2.Gtk.Box+BoxChild
			this.labelBottom1 = new global::Gtk.Label ();
			this.labelBottom1.Name = "labelBottom1";
			this.labelBottom1.LabelProp = global::Mono.Unix.Catalog.GetString ("label1");
			this.hbox2.Add (this.labelBottom1);
			global::Gtk.Box.BoxChild w44 = ((global::Gtk.Box.BoxChild)(this.hbox2 [this.labelBottom1]));
			w44.Position = 0;
			w44.Expand = false;
			w44.Fill = false;
			w44.Padding = ((uint)(4));
			this.vbox1.Add (this.hbox2);
			global::Gtk.Box.BoxChild w45 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.hbox2]));
			w45.Position = 2;
			w45.Expand = false;
			w45.Fill = false;
			w45.Padding = ((uint)(2));
			this.Add (this.vbox1);
			if ((this.Child != null)) {
				this.Child.ShowAll ();
			}
			this.DefaultWidth = 574;
			this.DefaultHeight = 482;
			this.Show ();
			this.comboBackgroundMode.Changed += new global::System.EventHandler (this.Background_ComboBackgroundModeChanged);
			this.backgroundColorEventBox1.ButtonPressEvent += new global::Gtk.ButtonPressEventHandler (this.Background_Color1ButtonPress);
			this.backgroundColorEventBox2.ButtonPressEvent += new global::Gtk.ButtonPressEventHandler (this.Background_Color2ButtonPress);
			this.comboZoom.Changed += new global::System.EventHandler (this.Zoom_ComboChanged);
			this.comboScaling.Changed += new global::System.EventHandler (this.Scaling_ComboChanged);
			this.buttonShiftColor.Clicked += new global::System.EventHandler (this.Colors_ShiftButtonClicked);
			this.buttonInvertColor.Clicked += new global::System.EventHandler (this.Colors_InvertButtonClicked);
		}
	}
}
