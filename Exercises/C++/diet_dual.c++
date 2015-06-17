
/* This example formulates and solves the dual of the diet model:

     maximize   max v=21pi + 12pc
     subject to  2pi + 0pc <= 20
                 0pi + 1pc <= 10
                 3pi + 2pc <= 31
                 1pi + 2pc <= 11
                 2pi + 1pc <= 12
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

    GRBVar pi = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "pi");
    GRBVar pc = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "pc");

    // Integrate new variables

    model.update();

    // Set objective: maximize   max v=21pi + 12pc

    model.setObjective(21 * pi + 12 * pc, GRB_MAXIMIZE);

    // Add constraint: 2pi + 0pc <= 20

    GRBConstr c1 = model.addConstr(2 * pi + 0 * pc <= 20, "c1");

    // Add constraint: 0pi + 1pc <= 10

    GRBConstr c2 = model.addConstr(0 * pi + 1 * pc <= 10, "c2");

    // Add constraint: 3pi + 2pc <= 31

    GRBConstr c3 = model.addConstr(3 * pi + 2 * pc <= 31, "c3");

    // Add constraint: 1pi + 2pc <= 11

    GRBConstr c4 = model.addConstr(1 * pi + 2 * pc <= 11, "c4");

    // Add constraint: 2pi + 1pc <= 12

    GRBConstr c5 = model.addConstr(2 * pi + 1 * pc <= 12, "c5");

    // Optimize model

    model.optimize();
    cout << "\nVariables:\nV Name" << "\t" << "Value" << "\t" << "Red. Cost" << endl;
    cout << pi.get(GRB_StringAttr_VarName) << "\t"
         << pi.get(GRB_DoubleAttr_X) << "\t"
         << pi.get(GRB_DoubleAttr_RC) << endl;
    cout << pc.get(GRB_StringAttr_VarName) << "\t"
         << pc.get(GRB_DoubleAttr_X) << "\t"
         << pc.get(GRB_DoubleAttr_RC) << endl;

    cout << "\nObj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

    cout << "\nConstraints:\nC Name" << "\t" << "Slack" << "\t" << "Dual Val" << endl;
    cout << c1.get(GRB_StringAttr_ConstrName) << "\t"
         << c1.get(GRB_DoubleAttr_Slack) << "\t"
         << c1.get(GRB_DoubleAttr_Pi) << endl;
    cout << c2.get(GRB_StringAttr_ConstrName) << "\t"
         << c2.get(GRB_DoubleAttr_Slack) << "\t"
         << c2.get(GRB_DoubleAttr_Pi) << endl;
    cout << c3.get(GRB_StringAttr_ConstrName) << "\t"
         << c3.get(GRB_DoubleAttr_Slack) << "\t"
         << c3.get(GRB_DoubleAttr_Pi) << endl;
    cout << c4.get(GRB_StringAttr_ConstrName) << "\t"
         << c4.get(GRB_DoubleAttr_Slack) << "\t"
         << c4.get(GRB_DoubleAttr_Pi) << endl;
    cout << c5.get(GRB_StringAttr_ConstrName) << "\t"
         << c5.get(GRB_DoubleAttr_Slack) << "\t"
         << c5.get(GRB_DoubleAttr_Pi) << endl;


  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }

  return 0;
}
