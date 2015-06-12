/* This example formulates and solves the diet model:

     minimize   min z=20x1 + 10x2 + 31x3 + 11x4 + 12x5
     subject to  2x1 + 0x2 + 3x3 + 1x4 + 2x5 ≥ 21
                 0x1 + 1x2 + 2x3 + 2x4 + 1x5 ≥ 12
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
  char     *varnames[5] = { "x1", "x2", "x3", "x4", "x5" };
  char     *name[5];
  double    sol[5];
  double    rc[5];
  char     *conname[2];
  double    slack[2];
  double    pi[2];
  int       ind[5];
  double    val[5];
  double    obj[5];
  char      vtype[5];
  int       optimstatus;
  double    objval;

  /* Create environment */

  error = GRBloadenv(&env, "diet.log");
  if (error) goto QUIT;

  /* Create an empty model */

  error = GRBnewmodel(env, &model, "diet", 0, NULL, NULL, NULL, NULL, NULL);
  if (error) goto QUIT;


  /* Add variables */

  obj[0] = 20; obj[1] = 10; obj[2] = 31; obj[3] = 11; obj[4] = 12;
  for ( i = 0; i < 5; i++) {
	  vtype[i] = GRB_CONTINUOUS;
  }
  error = GRBaddvars(model, 5, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, varnames);
  if (error) goto QUIT;

  /* Change objective sense to minimization */

  error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
  if (error) goto QUIT;

  /* Integrate new variables */

  error = GRBupdatemodel(model);
  if (error) goto QUIT;


  /* First constraint: 2x1 + 0x2 + 3x3 + 1x4 + 2x5 ≥ 21 */

  ind[0] = 0; ind[1] = 1; ind[2] = 2; ind[3] = 3; ind[4] = 4;
  val[0] = 2; val[1] = 0; val[2] = 3; val[3] = 1; val[4] = 2;

  error = GRBaddconstr(model, 5, ind, val, GRB_GREATER_EQUAL, 21.0, "iron");
  if (error) goto QUIT;

  /* Second constraint: 0x1 + 1x2 + 2x3 + 2x4 + 1x5 ≥ 12 */

  ind[0] = 0; ind[1] = 1; ind[2] = 2; ind[3] = 3; ind[4] = 4;
  val[0] = 0; val[1] = 1; val[2] = 2; val[3] = 2; val[4] = 1;

  error = GRBaddconstr(model, 5, ind, val, GRB_GREATER_EQUAL, 12.0, "calcium");
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

  error = GRBgetstrattrarray(model, GRB_STR_ATTR_VARNAME, 0, 5, name);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, 5, sol);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_RC, 0, 5, rc);
  if (error) goto QUIT;

  error = GRBgetstrattrarray(model, GRB_STR_ATTR_CONSTRNAME, 0, 2, conname);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_SLACK, 0, 2, slack);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_PI, 0, 2, pi);
  if (error) goto QUIT;

  printf("\nOptimization complete\n");
  if (optimstatus == GRB_OPTIMAL) {
	printf("\nVariables:\nV Name      Value    Red. Cost\n");
	for (i=0; i<5; i++) {
        printf("%4s       %5.1f     %8.4f\n",name[i],sol[i],rc[i]);
	}

    printf("\nOptimal objective: %.4e\n", objval);

	printf("\nConstraints:\n C Name     Slack    Dual Value\n");
	for (i=0; i<2; i++) {
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
