using System;
using System.Collections.Generic;
using Gurobi;

namespace facility_parametric
{
    class FacilityLocation
    {
        const double EPS = 1e-4;
        static void Main(string[] args)
        {
            double[] Demand = new double[] { 15, 18, 14, 20 };
            double[] Capacity = new double[] { 20, 22, 17, 19, 18 };
            double[,] ShipCosts =
                new double[,] { { 4000, 2500, 1200, 2200 }, 
                { 2000, 2600, 1800, 2600 }, 
                { 3000, 3400, 2600, 3100 }, 
                { 2500, 3000, 4100, 3700 },
                { 4500, 4000, 3000, 3200 } };
            int nWarehouses = Capacity.Length;
            int nCustomers = Demand.Length;

            GRBEnv env = new GRBEnv();
            GRBModel m = new GRBModel(env);

            GRBVar[,] Ship = new GRBVar[nWarehouses, nCustomers];
            for (int i = 0; i < nWarehouses; ++i)
                for (int j = 0; j < nCustomers; ++j)
                    Ship[i, j] = m.AddVar(0, GRB.INFINITY, 0, GRB.CONTINUOUS, "ship." + i + "." + j);

            GRBVar[] Shortage = new GRBVar[nCustomers];
            for (int j = 0; j < nCustomers; ++j)
                Shortage[j] = m.AddVar(0, GRB.INFINITY, 0, GRB.CONTINUOUS, "shortage." + j);

            GRBVar TotalShortage = m.AddVar(0, GRB.INFINITY, 0, GRB.CONTINUOUS, "TotalShortage");
            GRBVar TotalShippingCost = m.AddVar(0, GRB.INFINITY, 1, GRB.CONTINUOUS, "TotalShippingCost");
            m.Update();

            GRBConstr[] DemandCon = new GRBConstr[nCustomers];
            for (int j = 0; j < nCustomers; ++j)
            {
                GRBLinExpr lhs = 0.0;
                for (int i = 0; i < nWarehouses; ++i)
                    lhs += Ship[i, j];
                DemandCon[j] = m.AddConstr(lhs == Demand[j] - Shortage[j], "demand." + j);
            }

            GRBConstr[] CapacityCon = new GRBConstr[nWarehouses];
            for (int i = 0; i < nWarehouses; ++i)
            {
                GRBLinExpr lhs = 0.0;
                for (int j = 0; j < nCustomers; ++j)
                    lhs += Ship[i, j];
                CapacityCon[i] = m.AddConstr(lhs <= Capacity[i], "capacity." + i);
            }

            GRBLinExpr expr = 0.0;
            for (int j = 0; j < nCustomers; ++j)
                expr += Shortage[j];
            GRBConstr TotalShortageCon = m.AddConstr(expr == TotalShortage, "total_shortage");

            expr = 0.0;
            for (int i = 0; i < nWarehouses; ++i)
                for (int j = 0; j < nCustomers; ++j)
                    expr += ShipCosts[i, j] * Ship[i, j];
            GRBConstr TotalShippingCon = m.AddConstr(expr == TotalShippingCost, "total_shipping");

            while (true)
            {
                m.Optimize();
                double OptShortage = TotalShortage.Get(GRB.DoubleAttr.X);
                double OptShipping = TotalShippingCost.Get(GRB.DoubleAttr.X);
                Console.WriteLine("TotalShortage = {0}", OptShortage);
                Console.WriteLine("TotalShippingCost= {0}", OptShipping);
                if (OptShortage < EPS) break;
                double ObjectiveBound = TotalShortage.Get(GRB.DoubleAttr.SAObjUp);
                TotalShortage.Set(GRB.DoubleAttr.Obj, ((1 + EPS) * ObjectiveBound));
            }
        }

        static void PrintSolution(GRBModel m)
        {
            foreach (GRBVar var in m.GetVars())
                Console.WriteLine("{0} = {1}", var.Get(GRB.StringAttr.VarName), var.Get(GRB.DoubleAttr.X));
        }
    }
}