LIS-Solver Python extension
==============================

Pybind11 https://github.com/pybind/pybind11 is a lightweight header-only library
that exposes C++ types in Python and vice versa, mainly to create Python bindings
of existing C++ Code or as in this case of C Code.

On great advantage of pybind11 is that Python numpy arrays (ndarrays) can easily
be passed to the C++/C world without making deep copies.

LIS (Library of Iterative Solvers for linear systems) http://www.ssisc.org/lis/
is a parallel software library for solving linear equations and eigenvalue
problems which uses OpenMP or MPI for parallel computing environments.