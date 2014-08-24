using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Gurobi;

namespace diet_example
{
    class diet_example
    {
        static void Main(string[] args)
        {
            GRBEnv env = new GRBEnv();
            GRBModel m = new GRBModel(env);
            GRBVar x1 = m.AddVar(0, GRB.INFINITY, 20, GRB.CONTINUOUS, "food.1");
            GRBVar x2 = m.AddVar(0, GRB.INFINITY, 10, GRB.CONTINUOUS, "food.2");
            GRBVar x3 = m.AddVar(0, GRB.INFINITY, 31, GRB.CONTINUOUS, "food.3");
            GRBVar x4 = m.AddVar(0, GRB.INFINITY, 11, GRB.CONTINUOUS, "food.4");
            GRBVar x5 = m.AddVar(0, GRB.INFINITY, 12, GRB.CONTINUOUS, "food.5");
            m.Update();
            GRBConstr con1 = m.AddConstr(2 * x1 + 3 * x3 + 1 * x4 + 2 * x5 >= 21, "nutrient.iron");
            GRBConstr con2 = m.AddConstr(1 * x2 + 2 * x3 + 2 * x4 + 1 * x5 >= 12, "nutrient.calcium");
            m.Optimize();
            m.Write("diet.lp");
            foreach (GRBVar var in m.GetVars()) 
                Console.WriteLine("{0} = {1}, reduced cost = {2}", var.Get(GRB.StringAttr.VarName), var.Get(GRB.DoubleAttr.X), var.Get(GRB.DoubleAttr.RC));
            foreach (GRBConstr constr in m.GetConstrs())
                Console.WriteLine("{0}, slack = {1}, pi = {2}", constr.Get(GRB.StringAttr.ConstrName), constr.Get(GRB.DoubleAttr.Slack), constr.Get(GRB.DoubleAttr.Pi));
            m.Dispose();
            env.Dispose();
        }
    }
}
