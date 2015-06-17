/* This example formulates and solves the dual of the diet model:

     maximize   max v=21pi + 12pc
     subject to  2pi + 0pc <= 20
                 0pi + 1pc <= 10
                 3pi + 2pc <= 31
                 1pi + 2pc <= 11
                 2pi + 1pc <= 12                 
*/

import gurobi.*;

public class diet_dual {
  public static void main(String[] args) {
    try {
      GRBEnv    env   = new GRBEnv("diet_dual.log");
      GRBModel  model = new GRBModel(env);

      // Create variables

    GRBVar pi = model.addVar(0.0, GRB.INFINITY, 0.0, GRB.CONTINUOUS, "pi");
    GRBVar pc = model.addVar(0.0, GRB.INFINITY, 0.0, GRB.CONTINUOUS, "pc");

      // Integrate new variables

      model.update();

      // Set objective: maximize   max v=21pi + 12pc

      GRBLinExpr expr = new GRBLinExpr();
      expr.addTerm(21.0, pi); 
      expr.addTerm(12.0, pc); 
      model.setObjective(expr, GRB.MAXIMIZE);

      // Add constraint: 2pi + 0pc <= 20

      expr = new GRBLinExpr();
      expr.addTerm(2.0, pi); 
      expr.addTerm(0.0, pc); 
      GRBConstr c1 = model.addConstr(expr, GRB.LESS_EQUAL, 20.0, "c1");

      // Add constraint: 0pi + 1pc <= 10

      expr = new GRBLinExpr();
      expr.addTerm(0.0, pi); 
      expr.addTerm(1.0, pc); 
      GRBConstr c2 = model.addConstr(expr, GRB.LESS_EQUAL, 10.0, "c2");

      // Add constraint: 3pi + 2pc <= 31

      expr = new GRBLinExpr();
      expr.addTerm(3.0, pi); 
      expr.addTerm(2.0, pc); 
      GRBConstr c3 = model.addConstr(expr, GRB.LESS_EQUAL, 31.0, "c3");

      // Add constraint: 1pi + 2pc <= 11

      expr = new GRBLinExpr();
      expr.addTerm(1.0, pi); 
      expr.addTerm(2.0, pc); 
      GRBConstr c4 = model.addConstr(expr, GRB.LESS_EQUAL, 11.0, "c4");

      // Add constraint: 2pi + 1pc <= 12

      expr = new GRBLinExpr();
      expr.addTerm(2.0, pi); 
      expr.addTerm(1.0, pc); 
      GRBConstr c5 = model.addConstr(expr, GRB.LESS_EQUAL, 12.0, "c5");

      // Optimize model

      model.optimize();

	  System.out.println("\nVariables:\nV Name" + "\t" + "Value" + "\t" + "Red. Cost");
      System.out.println(pi.get(GRB.StringAttr.VarName)
                         + "\t" + pi.get(GRB.DoubleAttr.X)
                         + "\t" + pi.get(GRB.DoubleAttr.RC));
      System.out.println(pc.get(GRB.StringAttr.VarName)
                         + "\t" + pc.get(GRB.DoubleAttr.X)
                         + "\t" + pc.get(GRB.DoubleAttr.RC));

      System.out.println("\nObj: " + model.get(GRB.DoubleAttr.ObjVal));

	  System.out.println("\nConstraints:\nC Name" + "\t" + "Slack" + "\t" + "Dual Val");
      System.out.println(c1.get(GRB.StringAttr.ConstrName)
                         + "\t" + c1.get(GRB.DoubleAttr.Slack)
                         + "\t" + c1.get(GRB.DoubleAttr.Pi));
      System.out.println(c2.get(GRB.StringAttr.ConstrName)
              + "\t" + c2.get(GRB.DoubleAttr.Slack)
              + "\t" + c2.get(GRB.DoubleAttr.Pi));
      System.out.println(c3.get(GRB.StringAttr.ConstrName)
              + "\t" + c3.get(GRB.DoubleAttr.Slack)
              + "\t" + c3.get(GRB.DoubleAttr.Pi));
      System.out.println(c4.get(GRB.StringAttr.ConstrName)
              + "\t" + c4.get(GRB.DoubleAttr.Slack)
              + "\t" + c4.get(GRB.DoubleAttr.Pi));
      System.out.println(c5.get(GRB.StringAttr.ConstrName)
              + "\t" + c5.get(GRB.DoubleAttr.Slack)
              + "\t" + c5.get(GRB.DoubleAttr.Pi));
                         
      // Dispose of model and environment

      model.dispose();
      env.dispose();

    } catch (GRBException e) {
      System.out.println("Error code: " + e.getErrorCode() + ". " +
                         e.getMessage());
    }
  }
}
