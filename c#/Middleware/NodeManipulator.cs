using System.Collections.Generic;
using MiddlewareLoader;

namespace Middleware
{
    public class NodeManipulator
    {
        public static Dictionary<string, Node> Nodes = new Dictionary<string, Node>();
        private static NodeManipulator OBJ = null;
        public static NodeManipulator Make()
        {
            if (NodeManipulator.OBJ == null)
                NodeManipulator.OBJ = new NodeManipulator();
            return NodeManipulator.OBJ;
        }

        public void NodeRegister(Config.Langs lang, string name, Socket client)
        {
            var novoNode = new Node();

            novoNode.Lang = lang;
            novoNode.Name = name;
            novoNode.Client = client;
            novoNode.Connected = true;
            if (!NodeManipulator.Nodes.ContainsKey(name))
                NodeManipulator.Nodes.Add(name, novoNode);
            else
                NodeManipulator.Nodes[name] = novoNode;
        }
    }
}
