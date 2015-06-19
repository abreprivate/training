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

#define NUMCOMMODITIES 2
  char * Commodity[NUMCOMMODITIES] = {
			"Pencils",
			"Pens"
  };

#define NUMSOURCES 2
  char * Source[NUMSOURCES] = {
			"Detroit",
			"Denver"
  };

#define NUMDESTINATIONS 3
  char * Destination[NUMDESTINATIONS] = {
			"Boston",
			"New York",
			"Seattle"
  };

  double cost[NUMCOMMODITIES][NUMSOURCES][NUMDESTINATIONS] = {
    { {10, 20, 60}, {40, 40, 30} },
    { {20, 20, 80}, {60, 70, 30} }
  };
#define NUMVARS NUMSOURCES*NUMSOURCES*NUMDESTINATIONS


  double capacity[NUMSOURCES][NUMDESTINATIONS] = {
    {100, 80, 120},
    {120, 120, 120}
  };
#define NUMCAPACITYCONSTRS NUMSOURCES*NUMDESTINATIONS

  double supply[NUMCOMMODITIES][NUMSOURCES] = {
    {50, 60},
    {60, 40}
  };
#define NUMSUPPLYCONSTRS NUMCOMMODITIES*NUMSOURCES

  double demand[NUMCOMMODITIES][NUMDESTINATIONS] = {
    {50, 50, 10},
    {40, 30, 30}
  };
#define NUMDEMANDCONSTRS NUMCOMMODITIES*NUMDESTINATIONS

#define NUMCONSTRAINTS NUMCAPACITYCONSTRS+NUMSUPPLYCONSTRS+NUMDEMANDCONSTRS

