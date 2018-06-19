\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
 
Subject To
 rsum2: v20 + v21 + v22 + v23 + v24 + v25 + v26 = 2
 rsum3: v30 + v31 + v32 + v33 + v34 + v35 + v36 = 4
 rsum4: v40 + v41 + v42 + v43 + v44 + v45 + v46 = 3
 rsum6: v60 + v61 + v62 + v63 + v64 + v65 + v66 = 4
 csum2: v02 + v12 + v22 + v32 + v42 + v52 + v62 = 5
 csum3: v03 + v13 + v23 + v33 + v43 + v53 + v63 = 3
 csum4: v04 + v14 + v24 + v34 + v44 + v54 + v64 = 3
 therm(4,0)(3,0): - v30 + v40 >= 0
 therm(3,0)(2,0): - v20 + v30 >= 0
 therm(2,0)(1,0): - v10 + v20 >= 0
 therm(1,0)(0,0): - v00 + v10 >= 0
 therm(1,1)(0,1): - v01 + v11 >= 0
 therm(0,1)(0,2): v01 - v02 >= 0
 therm(0,2)(0,3): v02 - v03 >= 0
 therm(0,3)(0,4): v03 - v04 >= 0
 therm(0,4)(0,5): v04 - v05 >= 0
 therm(0,5)(0,6): v05 - v06 >= 0
 therm(0,6)(1,6): v06 - v16 >= 0
 therm(2,2)(1,2): - v12 + v22 >= 0
 therm(1,2)(1,3): v12 - v13 >= 0
 therm(1,3)(1,4): v13 - v14 >= 0
 therm(1,4)(1,5): v14 - v15 >= 0
 therm(1,5)(2,5): v15 - v25 >= 0
 therm(2,5)(3,5): v25 - v35 >= 0
 therm(3,5)(4,5): v35 - v45 >= 0
 therm(2,3)(2,4): v23 - v24 >= 0
 therm(2,4)(3,4): v24 - v34 >= 0
 therm(3,3)(4,3): v33 - v43 >= 0
 therm(4,3)(4,2): - v42 + v43 >= 0
 therm(4,2)(3,2): - v32 + v42 >= 0
 therm(2,6)(3,6): v26 - v36 >= 0
 therm(3,6)(4,6): v36 - v46 >= 0
 therm(4,6)(5,6): v46 - v56 >= 0
 therm(5,6)(6,6): v56 - v66 >= 0
 therm(4,4)(5,4): v44 - v54 >= 0
 therm(5,4)(5,3): - v53 + v54 >= 0
 therm(5,3)(5,2): - v52 + v53 >= 0
 therm(5,2)(5,1): - v51 + v52 >= 0
 therm(5,1)(4,1): - v41 + v51 >= 0
 therm(4,1)(3,1): - v31 + v41 >= 0
 therm(3,1)(2,1): - v21 + v31 >= 0
 therm(5,5)(6,5): v55 - v65 >= 0
 therm(6,5)(6,4): - v64 + v65 >= 0
 therm(6,4)(6,3): - v63 + v64 >= 0
 therm(6,3)(6,2): - v62 + v63 >= 0
 therm(6,2)(6,1): - v61 + v62 >= 0
 therm(6,1)(6,0): - v60 + v61 >= 0
 therm(6,0)(5,0): - v50 + v60 >= 0
Bounds
Binaries
 v00 v01 v02 v03 v04 v05 v06 v10 v11 v12 v13 v14 v15 v16 v20 v21 v22 v23
 v24 v25 v26 v30 v31 v32 v33 v34 v35 v36 v40 v41 v42 v43 v44 v45 v46 v50
 v51 v52 v53 v54 v55 v56 v60 v61 v62 v63 v64 v65 v66
End
