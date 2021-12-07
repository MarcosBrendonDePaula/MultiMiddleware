using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Middleware
{
    public class Config
    {
        public enum Langs
        {
            csharp,
            cpp
            
        };

        public static bool Debug = true;

        public static string Name              = "csnodo";
        public static Langs  Lang              = Langs.csharp;
        public static int MaxBuffer            = 1500;
        public static int CheckConnectionDelay = 100;
    }
}
