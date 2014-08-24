using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Gurobi;

namespace ShortestPath
{
    using Star = Dictionary<string, List<Arc>>;
    struct Arc
    {
        public Arc(string source, string dest, int length)
        {
            this.source = source;
            this.dest = dest;
            this.length = length;
        }
        public string source;
        public string dest;
        public int length;
    }

    class ShortestPath
    {
        static void ReadNetwork(StreamReader stream, HashSet<string> nodes, List<Arc> arcs)
        {
            string line, source, dest;
            int length;
            while ((line = stream.ReadLine()) != null)
            {
                source = line.Substring(0, 15).Trim();
                dest = line.Substring(15, 15).Trim();
                length = int.Parse(line.Substring(30));
                nodes.Add(source);
                nodes.Add(dest);
                arcs.Add(new Arc(source, dest, length));
            }
        }

        static void SolvePrimal(GRBEnv env, HashSet<string> nodes_set, List<Arc> arcs)
        {
            GRBModel m = new GRBModel(env);

            Dictionary<string, GRBVar> distances = new Dictionary<string, GRBVar>(); // pi
            foreach (string node in nodes_set)
                distances[node] = m.AddVar(0, GRB.INFINITY, 0, GRB.CONTINUOUS, "distance." + node);
            m.Update();
            distances[ORIGIN].Set(GRB.DoubleAttr.Obj, -1);
            distances[DESTINATION].Set(GRB.DoubleAttr.Obj, 1);
            m.Set(GRB.IntAttr.ModelSense, -1);

            Dictionary<Arc, GRBConstr> constrs = new Dictionary<Arc, GRBConstr>();
            foreach (Arc a in arcs)
                constrs[a] = m.AddConstr(distances[a.dest] <= distances[a.source] + a.length, "distance_con." + a.source + "." + a.dest);

            m.Update();
            m.Write("shortest_path.lp");
            m.Optimize();

            foreach (var pair in distances)
            {
                Console.WriteLine("distance to {0} is {1}", pair.Key, pair.Value.Get(GRB.DoubleAttr.X));
            }

            foreach (var pair in constrs)
            {
                GRBConstr con = pair.Value;
                if (con.Get(GRB.DoubleAttr.Pi) > 0.5)
                {
                    Console.WriteLine("Arc {0}, {1} is in shortest path", pair.Key.source, pair.Key.dest);
                }
            }
            Console.WriteLine("Length of shortest path is {0}", m.Get(GRB.DoubleAttr.ObjVal));
            m.Dispose();
        }

        static void GetStars(HashSet<string> nodes_set, List<Arc> arcs, Star forward_stars, Star reverse_stars)
        {
            foreach (string node in nodes_set)
            {
                forward_stars[node] = new List<Arc>();
                reverse_stars[node] = new List<Arc>();
            }
            foreach (Arc a in arcs)
            {
                forward_stars[a.source].Add(a);
                reverse_stars[a.dest].Add(a);
            }
        }

        static void SolveDual(GRBEnv env, HashSet<string> nodes_set, List<Arc> arcs)
        {
            GRBModel dual = new GRBModel(env);
            Star forward_stars = new Star();
            Star reverse_stars = new Star();
            GetStars(nodes_set, arcs, forward_stars, reverse_stars);
            Dictionary<Arc, GRBVar> arc_traversed = new Dictionary<Arc, GRBVar>();
            foreach (Arc a in arcs) arc_traversed[a] = dual.AddVar(0, 1, a.length, GRB.CONTINUOUS, "arc_traversed." + a.source + "." + a.dest);
            dual.Update();

            Dictionary<string, GRBConstr> flow_balance = new Dictionary<string, GRBConstr>();
            foreach (string node in nodes_set)
            {
                GRBLinExpr lhs = new GRBLinExpr();
                List<Arc> forward_star = forward_stars[node];
                List<Arc> reverse_star = reverse_stars[node];
                Console.WriteLine("node " + node);
                Console.Write("Forward star: ");
                foreach (Arc a in forward_star)
                {
                    Console.Write(a.dest + ' ');
                    //                    lhs -= arc_traversed[a];
                    lhs.AddTerm(-1, arc_traversed[a]);
                }
                Console.Write("\nReverse star: ");
                foreach (Arc a in reverse_star)
                {
                    Console.Write(a.source + ' ');
                    lhs.AddTerm(1, arc_traversed[a]);
                }
                Console.WriteLine("");
                flow_balance[node] = dual.AddConstr(lhs, 'E', 0, "flow_balance." + node);
            }
            dual.Update();
            flow_balance[ORIGIN].Set(GRB.DoubleAttr.RHS, -1);
            flow_balance[DESTINATION].Set(GRB.DoubleAttr.RHS, 1);
            dual.Optimize();
            foreach (var pair in arc_traversed)
            {
                Console.WriteLine("Arc {0}:{1} traversed = {2}", pair.Key.source, pair.Key.dest, pair.Value.Get(GRB.DoubleAttr.X));
            }
            Console.WriteLine("length of shortest path = " + dual.Get(GRB.DoubleAttr.ObjVal));
            dual.Dispose();
        }

        static void SolveDual2(GRBEnv env, HashSet<string> nodes_set, List<Arc> arcs)
        {
            GRBModel dual = new GRBModel(env);
            Dictionary<string, GRBLinExpr> lhs = new Dictionary<string, GRBLinExpr>();
            Dictionary<string, GRBConstr> flow_balance = new Dictionary<string, GRBConstr>();
            Dictionary<Arc, GRBVar> arc_traversed = new Dictionary<Arc, GRBVar>();

            foreach (string node in nodes_set) lhs[node] = new GRBLinExpr();

            foreach (Arc a in arcs)
            {
                GRBVar var = dual.AddVar(0, 1, a.length, GRB.CONTINUOUS, "arc_traversed." + a.source + "_" + a.dest);
                lhs[a.source].AddTerm(-1, var);
                lhs[a.dest].AddTerm(1, var);
            }
            dual.Update();
            foreach (string node in nodes_set) flow_balance[node] = dual.AddConstr(lhs[node], 'E', 0, "flow_balance." + node);
            dual.Update();
            flow_balance[ORIGIN].Set(GRB.DoubleAttr.RHS, -1);
            flow_balance[DESTINATION].Set(GRB.DoubleAttr.RHS, 1);
            dual.Optimize();
            dual.Dispose();
        }

        static void SolveDual3(GRBEnv env, HashSet<string> nodes_set, List<Arc> arcs)
        {
            GRBModel dual = new GRBModel(env);
            Dictionary<string, GRBConstr> flow_balance = new Dictionary<string, GRBConstr>();
            Dictionary<Arc, GRBVar> arc_traversed = new Dictionary<Arc, GRBVar>();

            GRBConstr[] constrs = dual.AddConstrs(nodes_set.Count);
            dual.Update();

            int i = 0;
            foreach (string s in nodes_set)
            {
                GRBConstr con = constrs[i];
                con.Set(GRB.StringAttr.ConstrName, "flow_balance." + s);
                con.Set(GRB.CharAttr.Sense, GRB.EQUAL);
                flow_balance[s] = con;
                ++i;
            }
            flow_balance[ORIGIN].Set(GRB.DoubleAttr.RHS, -1);
            flow_balance[DESTINATION].Set(GRB.DoubleAttr.RHS, 1);

            foreach (Arc a in arcs)
            {
                GRBColumn col = new GRBColumn();
                col.AddTerm(1, flow_balance[a.dest]);
                col.AddTerm(-1, flow_balance[a.source]);
                arc_traversed[a] = dual.AddVar(0, 1, a.length, GRB.CONTINUOUS, col, "arc_traversed." + a.source + "." + a.dest);
            }
            dual.Optimize();
            dual.Write("dual3.lp");
            dual.Write("dual3.sol");
            dual.Dispose();
        }

        const string ORIGIN = "Honolulu";
        const string DESTINATION = "Heathrow London";

        static void Main(string[] args)
        {
            HashSet<string> nodes_set = new HashSet<string>();
            List<Arc> arcs = new List<Arc>();

            StreamReader stream = new StreamReader("shortest_path.txt");
            ReadNetwork(stream, nodes_set, arcs);
            GRBEnv env = new GRBEnv();
            SolvePrimal(env, nodes_set, arcs);
            Console.WriteLine("primal solved");
            try
            {
                SolveDual3(env, nodes_set, arcs);
                env.Dispose();
            }
            catch (GRBException e)
            {
                Console.WriteLine(e.ErrorCode);
                throw e;
            }
        }
    }
}
