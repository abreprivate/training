/* This example formulates and solves a multi commodity network flow problem using the following data:

commodities = ['Pencils', 'Pens']
nodes = ['Detroit', 'Denver', 'Boston', 'New York', 'Seattle']
arcs, capacity = multidict({
  ('Detroit', 'Boston'):   100,
  ('Detroit', 'New York'):  80,
  ('Detroit', 'Seattle'):  120,
  ('Denver',  'Boston'):   120,
  ('Denver',  'New York'): 120,
  ('Denver',  'Seattle'):  120 })
cost = {
  ('Pencils', 'Detroit', 'Boston'):   10,
  ('Pencils', 'Detroit', 'New York'): 20,
  ('Pencils', 'Detroit', 'Seattle'):  60,
  ('Pencils', 'Denver',  'Boston'):   40,
  ('Pencils', 'Denver',  'New York'): 40,
  ('Pencils', 'Denver',  'Seattle'):  30,
  ('Pens',    'Detroit', 'Boston'):   20,
  ('Pens',    'Detroit', 'New York'): 20,
  ('Pens',    'Detroit', 'Seattle'):  80,
  ('Pens',    'Denver',  'Boston'):   60,
  ('Pens',    'Denver',  'New York'): 70,
  ('Pens',    'Denver',  'Seattle'):  30 }
demand = {
  ('Pencils', 'Detroit'):   -50,
  ('Pencils', 'Denver'):    -60,
  ('Pencils', 'Boston'):   50,
  ('Pencils', 'New York'): 50,
  ('Pencils', 'Seattle'):  10,
  ('Pens',    'Detroit'):   -60,
  ('Pens',    'Denver'):    -40,
  ('Pens',    'Boston'):   40,
  ('Pens',    'New York'): 30,
  ('Pens',    'Seattle'):  30 }

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "multi_flow.h"

/* mknam
 * inputs: r, s, t:  Non-NULL strings (but 0 length ok)
 * output: ptr to malloc'd concatenated string separated by two underscores: "r_s_t"
 *         ****** free the returned ptr when no longer needed ******
 */
char *mknam(char *r, char *s, char *t)
{
	int size = strlen(r)+strlen(s)+strlen(t)+3;
	char * ptr = malloc(size);
	strcpy(ptr, r);
	strcat(ptr,"_");
	strcat(ptr, s);
	strcat(ptr,"_");
	strcat(ptr, t);
	return ptr;
}

/* varind
 * inputs: i  index into Commodity array
 *         j  index into Source array
 *         k  index into Destination array
 * output  index for variable(Commodity,Source,Destination)
 *         If there are 3 Destinations and 2 Sources, then
 *         (0,0,0) -> 0
 *         (0,0,1) -> 1
 *         (0,0,2) -> 2
 *         (0,1,0) -> 3
 *         (0,1,1) -> 4
 *         etc.
 */
int varind(int i, int j, int k)
{
	return i*(NUMSOURCES*NUMDESTINATIONS)+j*(NUMDESTINATIONS)+k;
}

int
main(int   argc,
     char *argv[])
{
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;


  int 		i,j,k;
  int       error = 0;
  char	   *ptr;
  char     *name[NUMVARS];
  double    sol[NUMVARS];
  double    rc[NUMVARS];
  char     *conname[NUMCONSTRAINTS];
  double    slack[NUMCONSTRAINTS];
  double    pi[NUMCONSTRAINTS];
  int       ind[NUMVARS];
  double    val[NUMVARS];
  int       optimstatus;
  double    objval;

  /* Create environment */

  error = GRBloadenv(&env, "multi_flow.log");
  if (error) goto QUIT;

  /* Create an empty model */

  error = GRBnewmodel(env, &model, "multi_flow", 0, NULL, NULL, NULL, NULL, NULL);
  if (error) goto QUIT;


  /* Add variables: one flow variable for each commodity,source,dest combo */

  for ( i=0; i<NUMCOMMODITIES; i++)
  {
	  for ( j=0; j<NUMSOURCES; j++)
	  {
		  for ( k=0; k<NUMDESTINATIONS; k++)
		  {
			  error = GRBaddvar(model, 0, NULL, NULL, cost[i][j][k],
					       0, capacity[j][k], GRB_CONTINUOUS,
					       ptr=mknam(Commodity[i],Source[j],Destination[k]));
			  if (error) goto QUIT;
			  free(ptr);

		  }
	  }
  }

  /* Change objective sense to minimization */

  error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
  if (error) goto QUIT;

  /* Integrate new variables */

  error = GRBupdatemodel(model);
  if (error) goto QUIT;


  /* Supply constraints: one for each commodity,source pair */

  for(i=0; i<NUMCOMMODITIES; i++)
  {
	  for(j=0; j<NUMSOURCES; j++)
	  {
		  for ( k=0; k<NUMDESTINATIONS; k++)
		  {
			  ind[k] = varind(i,j,k);
			  val[k] = 1;
		  }
		  error = GRBaddconstr(model, NUMDESTINATIONS, ind, val, GRB_LESS_EQUAL,
				    supply[i][j], mknam("supply",Commodity[i],Source[j]));
		  if (error) goto QUIT;
	  }
  }


  /* Demand constraints: one for each commodity,dest pair */

  for(i=0; i<NUMCOMMODITIES; i++)
  {
	  for( k=0; k<NUMDESTINATIONS; k++)
	  {
		  for (j=0; j<NUMSOURCES; j++)
		  {
			  ind[j] = varind(i,j,k);
			  val[j] = 1;
		  }
		  error = GRBaddconstr(model, NUMSOURCES, ind, val, GRB_GREATER_EQUAL,
				    demand[i][k], mknam("demand",Commodity[i],Destination[k]));
		  if (error) goto QUIT;
	  }
  }

  /* Capacity constraints: one for each source,dest pair */

  for(j=0; j<NUMSOURCES; j++)
  {
	  for( k=0; k<NUMDESTINATIONS; k++)
	  {
		  for (i=0; i<NUMCOMMODITIES; i++)
		  {
			  ind[i] = varind(i,j,k);
			  val[i] = 1;
		  }
		  error = GRBaddconstr(model, NUMCOMMODITIES, ind, val, GRB_LESS_EQUAL,
				    capacity[j][k], mknam("capacity",Source[j],Destination[k]));
		  if (error) goto QUIT;
	  }
  }

  /* Integrate constraints */

  error = GRBupdatemodel(model);
  if (error) goto QUIT;

  /* Write model  */

  error = GRBwrite(model, "multi_flow.lp");
  if (error) goto QUIT;


  /* Optimize model */

  error = GRBoptimize(model);
  if (error) goto QUIT;


  /* Capture solution information */

  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
  if (error) goto QUIT;

  error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
  if (error) goto QUIT;

  error = GRBgetstrattrarray(model, GRB_STR_ATTR_VARNAME, 0, NUMVARS, name);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, NUMVARS, sol);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_RC, 0, NUMVARS, rc);
  if (error) goto QUIT;

  error = GRBgetstrattrarray(model, GRB_STR_ATTR_CONSTRNAME, 0, NUMCONSTRAINTS, conname);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_SLACK, 0, NUMCONSTRAINTS, slack);
  if (error) goto QUIT;

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_PI, 0, NUMCONSTRAINTS, pi);
  if (error) goto QUIT;

  printf("\nOptimization complete\n");
  if (optimstatus == GRB_OPTIMAL) {
	printf("\nVariables:\nV Name      Value    Red. Cost\n");
	for (i=0; i<NUMVARS; i++) {
        printf("%4s       %5.1f     %8.4f\n",name[i],sol[i],rc[i]);
	}

    printf("\nOptimal objective: %.4e\n", objval);

	printf("\nConstraints:\n C Name     Slack    Dual Value\n");
	for (i=0; i<NUMCONSTRAINTS; i++) {
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
