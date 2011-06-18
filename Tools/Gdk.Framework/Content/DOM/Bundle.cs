using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

using Gdk;

namespace Gdk.Content
{
    public class Bundle
    {
        public string Name;
        
        /// <summary>
        /// Constructor
        /// </summary>
        public Bundle()
            : this("")
        {
        }

        public Bundle(string name)
        {
            this.Name = name;
        }

        /// <summary>
        /// Writes this bundle to the xml
        /// </summary>
        /// <param name="writer"></param>
        public void WriteXml(XmlWriter writer)
        {
            // Write the <Bundle> node
            writer.WriteStartElement("Bundle");
            writer.WriteAttributeString("Name", Name);
            writer.WriteEndElement();
        }

        /// <summary>
        /// Loads a Bundle from an xml <Bundle> element
        /// </summary>
        public static Bundle FromXml(XmlNode xmlBundle)
        {
            // Create the bundle
            Bundle bundle = new Bundle();
            
            // Load the attributes
            bundle.Name = xmlBundle.Attributes["Name"].Value;

            return bundle;
        }

        /// <summary>
        /// ToString() override
        /// </summary>
        public override string ToString()
        {
            return this.Name;
        }
    }
}
