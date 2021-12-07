using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MiddlewareLoader;

namespace Middleware
{
    public class Node
    {
        public Config.Langs Lang;
        public bool Connected;
        public Socket Client;
        public string Name;
    }
}
