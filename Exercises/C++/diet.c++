
/* This example formulates and solves the diet model:

     minimize   min z=20x1 + 10x2 + 31x3 + 11x4 + 12x5
     subject to  2x1 + 0x2 + 3x3 + 1x4 + 2x5 ≥ 21
                 0x1 + 1x2 + 2x3 + 2x4 + 1x5 ≥ 12
*/

#include "gurobi_c++.h"
using namespace std;

int
main(int   argc,
     char *argv[])
{
  try {
    GRBEnv env = GRBEnv();

    GRBModel model = GRBModel(env);

    // Create variables

    GRBVar x1 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x1");
    GRBVar x2 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x2");
    GRBVar x3 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x3");
    GRBVar x4 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x4");
    GRBVar x5 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x5");

    // Integrate new variables

    model.update();

    // Set objective: minimize   min z=20x1 + 10x2 + 31x3 + 11x4 + 12x5

    model.setObjective(20 * x1 + 10 * x2 + 31 * x3 + 11 * x4 + 12 * x5, GRB_MINIMIZE);

    // Add constraint: 2x1 + 0x2 + 3x3 + 1x4 + 2x5 ≥ 21

    GRBConstr iron = model.addConstr(2 * x1 + 0 * x2 + 3 * x3 + 1 * x4 + 2 * x5 >= 21, "iron");

    // Add constraint: 0x1 + 1x2 + 2x3 + 2x4 + 1x5 ≥ 12

    GRBConstr calcium = model.addConstr(0 * x1 + 1 * x2 + 2 * x3 + 2 * x4 + 1 * x5 >= 12, "calcium");

    // Optimize model

    model.optimize();
    cout << "\nVariables:\nV Name" << "\t" << "Value" << "\t" << "Red. Cost" << endl;
    cout << x1.get(GRB_StringAttr_VarName) << "\t"
         << x1.get(GRB_DoubleAttr_X) << "\t"
         << x1.get(GRB_DoubleAttr_RC) << endl;
    cout << x2.get(GRB_StringAttr_VarName) << "\t"
         << x2.get(GRB_DoubleAttr_X) << "\t"
         << x2.get(GRB_DoubleAttr_RC) << endl;
    cout << x3.get(GRB_StringAttr_VarName) << "\t"
         << x3.get(GRB_DoubleAttr_X) << "\t"
         << x3.get(GRB_DoubleAttr_RC) << endl;
    cout << x4.get(GRB_StringAttr_VarName) << "\t"
         << x4.get(GRB_DoubleAttr_X) << "\t"
         << x4.get(GRB_DoubleAttr_RC) << endl;
    cout << x5.get(GRB_StringAttr_VarName) << "\t"
         << x5.get(GRB_DoubleAttr_X) << "\t"
         << x5.get(GRB_DoubleAttr_RC) << endl;

    cout << "\nObj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

    cout << "\nConstraints:\nC Name" << "\t" << "Slack" << "\t" << "Dual Val" << endl;
    cout << iron.get(GRB_StringAttr_ConstrName) << "\t"
         << iron.get(GRB_DoubleAttr_Slack) << "\t"
         << iron.get(GRB_DoubleAttr_Pi) << endl;
    cout << calcium.get(GRB_StringAttr_ConstrName) << "\t"
         << calcium.get(GRB_DoubleAttr_Slack) << "\t"
         << calcium.get(GRB_DoubleAttr_Pi) << endl;


  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }

  return 0;
}
