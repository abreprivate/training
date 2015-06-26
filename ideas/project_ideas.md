# Project Ideas
## Visualization of Gurobi LOG
- TODO
-- Get example logs from MIPLIB
-- parse them into python data structures
-- create visualizations learned at SciPy
-- Basic JavaScript front end
--- Drag/drop files
--- d3.js (perhaps just from Bokeh)
-- deploy to public server
--- Google App Engine
--- Heroku

## hdf5 format for mip solvers
- Reference implementation(s)
-- ANSI C
-- pytables
- tools for reading / visualizing

## New Language API
### Features
- Works with multiple solvers
-- Gurobi
-- coin-or (or other free solver)
- Robust set of examples
- Access to C API
- Higher level idiomatic abstractions
- Cool and pragmatic visualizations
- Client-server out of the box

### Target Languages
- python (with numpy, ...)
- C++0x
- C99
- node.js (JavaScript)
-- Applicable for OPL
- golang (google)
- rust (mozilla)
- swift (apple)
- Crazier
-- Clojure
-- Fortran 90
-- Postgres
-- neo4j

## Papers
- Computational
-- Find target paper
--- Local Search algorithm
--- Data publicly available
-- Implement with IPython Notebook
-- Add Gurobi
-- Show improved results

## Other
- diff tool for MIPs
-- useful for regression / debugging
- tools for improving MIPs
-- callback to analyze nodes that are infeasible, but look promising
-- Gomory cut generator
--- Use scikitLearn to generalize cuts to suggest model changes
