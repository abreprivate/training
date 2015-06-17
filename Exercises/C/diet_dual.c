/* This example formulates and solves the dual of the diet model:

     maximize   max v=21pi + 12pc
     subject to  2pi + 0pc <= 20
                 0pi + 1pc <= 10
                 3pi + 2pc <= 31
                 1pi + 2pc <= 11
                 2pi + 1pc <= 12
*/

#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"

int
main(int   argc,
     char *argv[])
{
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  int 		i;
  int       error = 0;
  char     *varnames[2] = { "pi", "pc" };
  char     *name[2];
  double    sol[2];
  double    rc[2];
  char     *conname[5];
  double    slack[5];
  double    pi[5];
  int       ind[2];
  double    val[2];
  double    obj[2];
  char      vtype[2];
  int       optimstatus;
  double    objval;

  /* Create environment */

  error = GRBloadenv(&env, "diet.log");
  if (error) goto QUIT;

  /* Create an empty model */

  error = GRBnewmodel(env, &model, "diet", 0, NULL, NULL, NULL, NULL, NULL);
  if (error) goto QUIT;


  /* Add variables */

  obj[0] = 21; obj[1] = 12;
  for ( i = 0; i < 2; i++) {
	  vtype[i] = GRB_CONTINUOUS;
  }
  error = GRBaddvars(model, 2, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, varnames);
  if (error) goto QUIT;

  /* Change objective sense to minimization */

  error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
  if (error) goto QUIT;

  /* Integrate new variables */

  error = GRBupdatemodel(model);
  if (error) goto QUIT;


  /* First constraint: 2pi + 0pc <= 20 */

  ind[0] = 0; ind[1] = 1;
  val[0] = 2; val[1] = 0;

  error = GRBaddconstr(model, 2, ind, val, GRB_LESS_EQUAL, 20.0, "c1");
  if (error) goto QUIT;

  /* 2nd constraint: 0pi + 1pc <= 10 */

  ind[0] = 0; ind[1] = 1;
  val[0] = 0; val[1] = 1;

  error = GRBaddconstr(model, 2, ind, val, GRB_LESS_EQUAL, 10.0, "c2");
  if (error) goto QUIT;

  /* 3rd constraint: 3pi + 2pc <= 31 */

  ind[0] = 0; ind[1] = 1;
  val[0] = 3; val[1] = 2;

  error = GRBaddconstr(model, 2, ind, val, GRB_LESS_EQUAL, 31.0, "c3");
  if (error) goto QUIT;

  /* 4th constraint: 1pi + 2pc <= 11 */

  ind[0] = 0; ind[1] = 1;
  val[0] = 1; val[1] = 2;

  error = GRBaddconstr(model, 2, ind, val, GRB_LESS_EQUAL, 11.0, "c4");
  if (error) goto QUIT;

  /* 5th constraint: 2pi + 1pc <= 12 */

  ind[0] = 0; ind[1] = 1;
  val[0] = 2; val[1] = 1;

  error = GRBaddconstr(model, 2, ind, val, GRB_LESS_EQUAL, 12.0, "c5");
  if (error) goto QUIT;


  /* Optimize model */

  error = GRBoptimize(model);
  if (error) goto QUIT;

  /* Write model to 'mip1.lp' */

  error = GRBwrite(model, "diet.lp");
  if (error) goto QUIT;

  /* Capture solution information */

  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
  if (error) goto QUIT;

  error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
  if (error) goto QUIT;

  error = GRBgetstrattrarray(model, GRB_STR_ATTR_VARNAME, 0, 2, name);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, 2, sol);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_RC, 0, 2, rc);
  if (error) goto QUIT;

  error = GRBgetstrattrarray(model, GRB_STR_ATTR_CONSTRNAME, 0, 5, conname);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_SLACK, 0, 5, slack);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_PI, 0, 5, pi);
  if (error) goto QUIT;

  printf("\nOptimization complete\n");
  if (optimstatus == GRB_OPTIMAL) {
	printf("\nVariables:\nV Name      Value    Red. Cost\n");
	for (i=0; i<2; i++) {
        printf("%4s       %5.1f     %8.4f\n",name[i],sol[i],rc[i]);
	}

    printf("\nOptimal objective: %.4e\n", objval);

	printf("\nConstraints:\n C Name     Slack    Dual Value\n");
	for (i=0; i<5; i++) {
        printf("%7s    %5.1f     %8.4f\n",conname[i],slack[i],pi[i]);
	}
  } else if (optimstatus == GRB_INF_OR_UNBD) {
    printf("Model is infeasible or unbounded\n");
  } else {
    printf("Optimization was stopped early\n");
  }

QUIT:

  /* Error reporting */

  if (error) {
    printf("ERROR: %s\n", GRBgeterrormsg(env));
    exit(1);
  }

  /* Free model */

  GRBfreemodel(model);

  /* Free environment */

  GRBfreeenv(env);

  return 0;
}
