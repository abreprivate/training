/* This example formulates and solves the diet model:

     minimize   min z=20x1 + 10x2 + 31x3 + 11x4 + 12x5
     subject to  2x1 + 0x2 + 3x3 + 1x4 + 2x5 ≥ 21
                 0x1 + 1x2 + 2x3 + 2x4 + 1x5 ≥ 12
*/

import gurobi.*;

public class diet {
  public static void main(String[] args) {
    try {
      GRBEnv    env   = new GRBEnv("diet.log");
      GRBModel  model = new GRBModel(env);

      // Create variables

    GRBVar x1 = model.addVar(0.0, GRB.INFINITY, 0.0, GRB.CONTINUOUS, "x1");
    GRBVar x2 = model.addVar(0.0, GRB.INFINITY, 0.0, GRB.CONTINUOUS, "x2");
    GRBVar x3 = model.addVar(0.0, GRB.INFINITY, 0.0, GRB.CONTINUOUS, "x3");
    GRBVar x4 = model.addVar(0.0, GRB.INFINITY, 0.0, GRB.CONTINUOUS, "x4");
    GRBVar x5 = model.addVar(0.0, GRB.INFINITY, 0.0, GRB.CONTINUOUS, "x5");

      // Integrate new variables

      model.update();

      // Set objective: minimize   min z=20x1 + 10x2 + 31x3 + 11x4 + 12x5

      GRBLinExpr expr = new GRBLinExpr();
      expr.addTerm(20.0, x1); 
      expr.addTerm(10.0, x2); 
      expr.addTerm(31.0, x3); 
      expr.addTerm(11.0, x4); 
      expr.addTerm(12.0, x5); 
      model.setObjective(expr, GRB.MINIMIZE);

      // Add constraint: 2x1 + 0x2 + 3x3 + 1x4 + 2x5 ≥ 21

      expr = new GRBLinExpr();
      expr.addTerm(2.0, x1); 
      expr.addTerm(0.0, x2); 
      expr.addTerm(3.0, x3); 
      expr.addTerm(1.0, x4); 
      expr.addTerm(2.0, x5); 
      GRBConstr iron = model.addConstr(expr, GRB.GREATER_EQUAL, 21.0, "iron");

      // Add constraint: 0x1 + 1x2 + 2x3 + 2x4 + 1x5 ≥ 12

      expr = new GRBLinExpr();
      expr.addTerm(0.0, x1); 
      expr.addTerm(1.0, x2); 
      expr.addTerm(2.0, x3); 
      expr.addTerm(2.0, x4); 
      expr.addTerm(1.0, x5); 
      GRBConstr calcium = model.addConstr(expr, GRB.GREATER_EQUAL, 12.0, "calcium");

      // Optimize model

      model.optimize();

	  System.out.println("\nVariables:\nV Name" + "\t" + "Value" + "\t" + "Red. Cost");
      System.out.println(x1.get(GRB.StringAttr.VarName)
                         + "\t" + x1.get(GRB.DoubleAttr.X)
                         + "\t" + x1.get(GRB.DoubleAttr.RC));
      System.out.println(x2.get(GRB.StringAttr.VarName)
                         + "\t" + x2.get(GRB.DoubleAttr.X)
                         + "\t" + x2.get(GRB.DoubleAttr.RC));
      System.out.println(x3.get(GRB.StringAttr.VarName)
                         + "\t" + x3.get(GRB.DoubleAttr.X)
                         + "\t" + x3.get(GRB.DoubleAttr.RC));
      System.out.println(x4.get(GRB.StringAttr.VarName)
                         + "\t" + x4.get(GRB.DoubleAttr.X)
                         + "\t" + x4.get(GRB.DoubleAttr.RC));
      System.out.println(x5.get(GRB.StringAttr.VarName)
                         + "\t" + x5.get(GRB.DoubleAttr.X)
                         + "\t" + x5.get(GRB.DoubleAttr.RC));

      System.out.println("\nObj: " + model.get(GRB.DoubleAttr.ObjVal));

	  System.out.println("\nConstraints:\nC Name" + "\t" + "Slack" + "\t" + "Dual Val");
      System.out.println(iron.get(GRB.StringAttr.ConstrName)
                         + "\t" + iron.get(GRB.DoubleAttr.Slack)
                         + "\t" + iron.get(GRB.DoubleAttr.Pi));
      System.out.println(calcium.get(GRB.StringAttr.ConstrName)
                         + "\t" + calcium.get(GRB.DoubleAttr.Slack)
                         + "\t" + calcium.get(GRB.DoubleAttr.Pi));    
                         
      // Dispose of model and environment

      model.dispose();
      env.dispose();

    } catch (GRBException e) {
      System.out.println("Error code: " + e.getErrorCode() + ". " +
                         e.getMessage());
    }
  }
}
