/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.IO;

using Gtk;

namespace Gdk.Framework.UI.GtkSharp
{
	/// <summary>
	/// Extends the Gtk.TextBuffer class and provides functionality for inserting GTML encoded text
	/// </summary>
    public class GtmlTextBuffer : Gtk.TextBuffer
	{
		// Common Tags
		TextTag tagBold;
		TextTag tagItalic;
		TextTag tagUnderline;
		TextTag tagStrikethrough;
		TextTag tagMonospace;
		TextTag[] tagFontSizes = new TextTag[7];
		Dictionary<Gdk.Color, TextTag> colorTagsByColor = new Dictionary<Gdk.Color, TextTag>();
		Dictionary<string, TextTag> linkTagsByUrl = new Dictionary<string, TextTag>();
			
		// States
		public bool BoldEnabled = false;
		public bool ItalicEnabled = false;
		public bool UnderlineEnabled = false;
		public bool StrikethroughEnabled = false;
		public bool MonospaceEnabled = false;
		public int FontSize = 3;
		public Gdk.Color TextColor = new Gdk.Color(0,0,0);
		public string LinkUrl = null;
		
		// ******************************************************************************
		public GtmlTextBuffer() : base(new TextTagTable())
		{
			// Create the common tags
			CreateCommonTags();
		}
		
		// ******************************************************************************
		private void CreateCommonTags()
		{
			tagBold  = new TextTag("bold");
			tagBold.Weight = Pango.Weight.Bold;
			this.TagTable.Add(tagBold);
			
			tagItalic  = new TextTag("italic");
			tagItalic.Style = Pango.Style.Italic;
			this.TagTable.Add(tagItalic);
			
			tagUnderline  = new TextTag("underline");
			tagUnderline.Underline = Pango.Underline.Single;
			this.TagTable.Add(tagUnderline);
			
			tagStrikethrough  = new TextTag("strikethrough");
			tagStrikethrough.Strikethrough = true;
			this.TagTable.Add(tagStrikethrough);
			
			tagMonospace = new TextTag("monospace");
			tagMonospace.Family = "monospace";
			this.TagTable.Add(tagMonospace);
			
			// Font Size Tags
			for(int i=0; i<7; i++)
			{
				tagFontSizes[i] = new TextTag("font_size_" + (i + 1));
				this.TagTable.Add(tagFontSizes[i]);
			}
			tagFontSizes[0].Scale = Pango.Scale.XXSmall;
			tagFontSizes[1].Scale = Pango.Scale.XSmall;
			tagFontSizes[2].Scale = Pango.Scale.Small;
			tagFontSizes[3].Scale = Pango.Scale.Medium;
			tagFontSizes[4].Scale = Pango.Scale.Large;
			tagFontSizes[5].Scale = Pango.Scale.XLarge;
			tagFontSizes[6].Scale = Pango.Scale.XXLarge;
		}
		
		/// <summary>
		/// Decodes a blob of GTML text and appends it to the end of the buffer
		/// </summary>
		public void AppendGTML(string gtml)
		{
			TextIter iter = this.EndIter;
			InsertGTML(ref iter, gtml);
		}
		
		/// <summary>
		/// Decodes a blob of GTML text and inserts it at the given iterator
		/// </summary>
		public void InsertGTML(ref TextIter iter, string gtml)
		{
			int lastPos = 0;
			int curPos = 0;
			bool inTagDefinition = false;
			
			// Walk through the string, a character at a time
			while(curPos < gtml.Length)
			{
				// Are we NOT currently in a tag definition?
				if(inTagDefinition == false)
				{
					// Is this a tag start marker?
					if(gtml[curPos] == '<')
					{
						// Do we have text we need to write out?
						if(curPos > lastPos)
							InsertTextWithCurrentState(ref iter, gtml.Substring(lastPos, curPos - lastPos));
						lastPos = curPos + 1;
						
						// We're now in a tag definition
						inTagDefinition = true;
					}
				}
				else
				{
					// Is this a tag start marker?
					if(gtml[curPos] == '<')
					{
						// This is an escape sequence of "<<", go back to non-tag mode
						inTagDefinition = false;
					}
					
					// Is this a tag end marker?
					else if(gtml[curPos] == '>')
					{
						// Get the tag string
						string tag = gtml.Substring(lastPos, curPos - lastPos);
						lastPos = curPos + 1;
						
						ProcessTag(tag);
						
						// We're now outside a tag definition
						inTagDefinition = false;
					}
				}
				
				// Move the cursor ahead 1
				curPos++;
			}
			
			// Write out the current text if we have some
			if(curPos > lastPos)
				InsertTextWithCurrentState(ref iter, gtml.Substring(lastPos, curPos - lastPos));
		}
		
		// ******************************************************************************
		private void ProcessTag(string tag)
		{
			// Get the tag name & its parameter
			int seperatorIndex = tag.IndexOf(":");
			string name = tag;
			string param = string.Empty;
			if(seperatorIndex > 0)
			{
				name = tag.Substring(0, seperatorIndex);
				param = tag.Substring(seperatorIndex + 1);
			}
			
			name = name.Trim().ToLower();
			param = param.Trim();
			
			
			// Process the tag by name
			switch(name)
			{
				case "b":
					this.BoldEnabled = true;
					break;
				case "/b":
					this.BoldEnabled = false;
					break;
				
				case "i":
					this.ItalicEnabled = true;
					break;
				case "/i":
					this.ItalicEnabled = false;
					break;
				
				case "u":
					this.UnderlineEnabled = true;
					break;
				case "/u":
					this.UnderlineEnabled = false;
					break;
					
				case "size":
					int size;
					if(int.TryParse(param, out size) && size >= 1 && size <= 7)
						this.FontSize = size;
					break;
				case "/size":
					this.FontSize = 3;
					break;
					
				case "color":
					Gdk.Color color = Gdk.Color.Zero;
					if(Gdk.Color.Parse(param, ref color))
						this.TextColor = color;
					break;
				case "/color":
					this.TextColor = new Gdk.Color(0,0,0);
					break;
				
				case "link":
					this.LinkUrl = param;
					break;
				case "/link":
					this.LinkUrl = null;
					break;
			}
		}
		
		// ******************************************************************************
		private void InsertTextWithCurrentState(ref TextIter iter, string text)
		{
			// Insert the text
			int offset = iter.Offset;
			this.Insert(ref iter, text);

			// Get the start & end iters of the inserted text
			TextIter startIter = this.GetIterAtOffset(offset);
			TextIter endIter = this.GetIterAtOffset(offset + text.Length);
			
			// Apply required tags
			this.ApplyTag(tagFontSizes[this.FontSize], startIter, endIter);
			
			// Apply formating tags
			if(this.BoldEnabled)
				this.ApplyTag(tagBold, startIter, endIter);
			if(this.ItalicEnabled)
				this.ApplyTag(tagItalic, startIter, endIter);
			if(this.UnderlineEnabled)
				this.ApplyTag(tagUnderline, startIter, endIter);
			if(this.StrikethroughEnabled)
				this.ApplyTag(tagStrikethrough, startIter, endIter);
			if(this.MonospaceEnabled)
				this.ApplyTag(tagMonospace, startIter, endIter);
			
			// Is the text color somthing other than black? (default)
			if(this.TextColor.Red != 0 || this.TextColor.Blue != 0 || this.TextColor.Green != 0)
			{
				// Get & Apply the color tag for the current text color
				TextTag colorTag;
				if(this.colorTagsByColor.TryGetValue(this.TextColor, out colorTag) == false)
				{
					// This is a new color, create a new tag for it
					colorTag = new TextTag("textcolor_" + this.TextColor.ToString());
					colorTag.ForegroundGdk = this.TextColor;
					//colorTag.ForegroundSet = true;
					this.colorTagsByColor.Add(this.TextColor, colorTag);
					this.TagTable.Add(colorTag);
				}
				this.ApplyTag(colorTag, startIter, endIter);
			}
			
			// Is there a active link to apply?
			if(string.IsNullOrEmpty(this.LinkUrl) == false)
			{
				// Get & Apply the link tag for the current url
				TextTag linkTag;
				if(this.linkTagsByUrl.TryGetValue(this.LinkUrl, out linkTag) == false)
				{
					// This is a new link url, create a new tag for it
					linkTag = new TextTag("link_" + this.LinkUrl);
					linkTag.ForegroundGdk = new Gdk.Color(0,0,255);
					linkTag.Underline = Pango.Underline.Single;
					linkTag.Data.Add("link_url", this.LinkUrl);

					//colorTag.ForegroundSet = true;
					this.linkTagsByUrl.Add(this.LinkUrl, linkTag);
					this.TagTable.Add(linkTag);
				}
				this.ApplyTag(linkTag, startIter, endIter);
			}
		}
		
		// ******************************************************************************
		public bool GetLinkAtIter(TextIter iter, out string linkUrl)
		{
			// Loop through the tags on this iter
			foreach(TextTag tag in iter.Tags)
			{
				// Is this a link tag?
				if(tag.Data.ContainsKey("link_url"))
				{
					// Get the link url & return it :)
					linkUrl = tag.Data["link_url"] as string;
					return true;
				}
			}
			
			// No link tags
			linkUrl = null;
			return false;
		}
		
	}
}