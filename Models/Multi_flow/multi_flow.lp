Minimize
  10 Pencils_Detroit_Boston + 20 Pencils_Detroit_New_York
   + 60 Pencils_Detroit_Seattle + 40 Pencils_Denver_Boston
   + 40 Pencils_Denver_New_York + 30 Pencils_Denver_Seattle
   + 20 Pens_Detroit_Boston + 20 Pens_Detroit_New_York
   + 80 Pens_Detroit_Seattle + 60 Pens_Denver_Boston
   + 70 Pens_Denver_New_York + 30 Pens_Denver_Seattle
Subject To
 supply_Pencils_Detroit: Pencils_Detroit_Boston + Pencils_Detroit_New_York
   + Pencils_Detroit_Seattle <= 50
 supply_Pencils_Denver: Pencils_Denver_Boston + Pencils_Denver_New_York
   + Pencils_Denver_Seattle <= 60
 supply_Pens_Detroit: Pens_Detroit_Boston + Pens_Detroit_New_York
   + Pens_Detroit_Seattle <= 60
 supply_Pens_Denver: Pens_Denver_Boston + Pens_Denver_New_York
   + Pens_Denver_Seattle <= 40
 demand_Pencils_Boston: Pencils_Detroit_Boston + Pencils_Denver_Boston
   >= 50
 demand_Pencils_New_York: Pencils_Detroit_New_York
   + Pencils_Denver_New_York >= 50
 demand_Pencils_Seattle: Pencils_Detroit_Seattle + Pencils_Denver_Seattle
   >= 10
 demand_Pens_Boston: Pens_Detroit_Boston + Pens_Denver_Boston >= 40
 demand_Pens_New_York: Pens_Detroit_New_York + Pens_Denver_New_York >= 30
 demand_Pens_Seattle: Pens_Detroit_Seattle + Pens_Denver_Seattle >= 30
 capacity_Detroit_Boston: Pencils_Detroit_Boston + Pens_Detroit_Boston
   <= 100
 capacity_Detroit_New_York: Pencils_Detroit_New_York
   + Pens_Detroit_New_York <= 80
 capacity_Detroit_Seattle: Pencils_Detroit_Seattle + Pens_Detroit_Seattle
   <= 120
 capacity_Denver_Boston: Pencils_Denver_Boston + Pens_Denver_Boston <= 120
 capacity_Denver_New_York: Pencils_Denver_New_York + Pens_Denver_New_York
   <= 120
 capacity_Denver_Seattle: Pencils_Denver_Seattle + Pens_Denver_Seattle
   <= 120
Bounds
 Pencils_Detroit_Boston <= 100
 Pencils_Detroit_New_York <= 80
 Pencils_Detroit_Seattle <= 120
 Pencils_Denver_Boston <= 120
 Pencils_Denver_New_York <= 120
 Pencils_Denver_Seattle <= 120
 Pens_Detroit_Boston <= 100
 Pens_Detroit_New_York <= 80
 Pens_Detroit_Seattle <= 120
 Pens_Denver_Boston <= 120
 Pens_Denver_New_York <= 120
 Pens_Denver_Seattle <= 120
End
