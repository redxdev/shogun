using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler
{
    public class NodeAttributes
    {
        public HashSet<string> Attributes
        {
            get;
            set;
        }

        public NodeAttributes()
        {
            this.Attributes = new HashSet<string>();
        }

        public NodeAttributes Has(string attr)
        {
            this.Attributes.Add(attr);
            return this;
        }

        public NodeAttributes Check(string attr)
        {
            if (!this.Attributes.Contains(attr))
                throw new NodeAttributeException("Node does not contain attribute " + attr);

            return this;
        }

        public NodeAttributes Fail(string attr)
        {
            if (this.Attributes.Contains(attr))
                throw new NodeAttributeException("Node does contains attribute " + attr);

            return this;
        }
    }
}
