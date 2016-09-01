#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "wrapper.h"

#include <iostream>
#include <stdio.h>
#include "lis_config.h"
#include "lis.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"

namespace py = pybind11;
using namespace std;

void wrapper(py::array_t<double> values, py::array_t<int> columns,
             py::array_t<int> index, py::array_t<double> x,
             py::array_t<double> b, int info, double rel_tol, int max_iter){

    py::buffer_info info_values = values.request();
    auto ptr_values = static_cast<double *>(info_values.ptr);
    py::buffer_info info_columns = columns.request();
    auto ptr_columns = static_cast<int *>(info_columns.ptr);
    py::buffer_info info_index = index.request();
    auto ptr_index = static_cast<int *>(info_index.ptr);
    py::buffer_info info_x = x.request();
    auto ptr_x = static_cast<double *>(info_x.ptr);
    py::buffer_info info_b = b.request();
    auto ptr_b = static_cast<double *>(info_b.ptr);

    cout << "wrapper: info_values.shape[0] " << info_values.shape[0] << endl;
    cout << "wrapper: info_x.shape[0] " << info_x.shape[0] << endl;

    c_lis(ptr_values, info_values.shape[0],
          ptr_columns, ptr_index, ptr_x, info_x.shape[0],
          ptr_b, info, rel_tol, max_iter);
}

void c_lis(double* val, int nnz, int* col, int* ind, double* x_arr, int len_xarr,
        double* b, int info, double rel_tol, int max_iter) {

    LIS_MATRIX A;
    LIS_VECTOR X, B;
    LIS_SOLVER solver;
    char *dargvs[] = {"dummy"};
    char **dargv;
    LIS_INT err, i, dargc = 1;
    LIS_INT iter, iter_double, iter_quad, nsol, nprecon;
    double times, itimes, ptimes, p_c_times, p_i_times;
    LIS_REAL resid;
    char solvername[128], preconname[128];
    char tolstr[80];
    char iterstr[80];

    printf("LIS start...\n");
    //printf("lis_command: %s\n", lis_command);
    LIS_DEBUG_FUNC_IN;
    //create and associate the coefficient matrix in CSR format
    dargv = &dargvs[0];
    err = lis_initialize(&dargc, &dargv);
    CHKERR(err);
    err = lis_matrix_create(0, &A);
    CHKERR(err);
    err = lis_matrix_set_size(A, 0, len_xarr);
    CHKERR(err);
    err = lis_matrix_set_csr(nnz, ind, col, val, A);
    CHKERR(err);
    err = lis_matrix_assemble(A);
    CHKERR(err);
    // create rhs and solution vectors
    err = lis_vector_create(0, &B);
    CHKERR(err);
    err = lis_vector_create(0, &X);
    CHKERR(err);
    err = lis_vector_set_size(B, 0, len_xarr);
    CHKERR(err);
    err = lis_vector_set_size(X, 0, len_xarr);
    CHKERR(err);
    printf("\nLIS: OpenMP Infos... \n");
#ifdef _OPENMP
#ifdef _LONG__LONG
    printf("LIS: max number of threads = %lld\n", omp_get_num_procs());
    printf("LIS: number of threads = %lld\n", omp_get_max_threads());
#else
    printf("LIS: max number of threads = %d\n", omp_get_num_procs());
    printf("LIS: number of threads = %d\n", omp_get_max_threads());
#endif
#endif    
    // set solution vector X to 0
    //err = lis_vector_set_all(0.0, X);
    //CHKERR(err);
    // setup X
    for (i = 0; i < len_xarr; i++) {
        lis_vector_set_value(LIS_INS_VALUE, i, *(x_arr + i), X);
    }

    // setup rhs
    for (i = 0; i < len_xarr; i++) {
        lis_vector_set_value(LIS_INS_VALUE, i, *(b + i), B);
    }
    // solver
    err = lis_solver_create(&solver);
    CHKERR(err);
    lis_solver_set_option("-i cg", solver);
    snprintf(tolstr, 80, "-tol %e", rel_tol);
    lis_solver_set_option(tolstr, solver);
    snprintf(iterstr, 80, "-maxiter %d", max_iter);
    lis_solver_set_option(iterstr, solver);
    //lis_solver_set_option("-p jacobi", solver);
    //lis_solver_set_option("-p ilu -ilu_fill 2", solver);

    lis_solver_set_option("-p ssor", solver);
    //lis_solver_set_option("-adds true", solver);
    lis_solver_set_option("-ssor_w 1.0", solver);
    // dont't set initial vector x to 0
    lis_solver_set_option("-initx_zeros 0", solver);

    lis_solver_set_option("-print mem", solver);
    err = lis_solve(A, B, X, solver);
    CHKERR(err);
    lis_solver_get_iterex(solver, &iter, &iter_double, &iter_quad);
    lis_solver_get_residualnorm(solver, &resid);
    lis_solver_get_solver(solver, &nsol);
    lis_solver_get_solvername(nsol, solvername);
    lis_solver_get_precon(solver, &nprecon);
    lis_solver_get_preconname(nprecon, preconname);
    printf("\nLIS: statistical infos...\n");
#ifdef _LONGLONG
    printf
            ("%s: number of iterations     = %lld (double = %lld, quad = %lld)\n",
            solvername, iter, iter_double, iter_quad);
#else
    printf("%s: number of iterations     = %d (double = %d, quad = %d)\n",
            solvername, iter, iter_double, iter_quad);
#endif
    if (info) {
        lis_solver_get_timeex(solver, &times, &itimes, &ptimes, &p_c_times, &p_i_times);
        printf("%s: elapsed time             = %e sec.\n", solvername, times);
        printf("%s:   preconditioner         = %e sec.\n", preconname, ptimes);
        printf("%s:   matrix creation        = %e sec.\n", solvername, p_c_times);
        printf("%s:   linear solver          = %e sec.\n", solvername, itimes);
    }
#ifdef _LONG_DOUBLE
    printf("%s: relative residual 2-norm = %Le\n\n", solvername, resid);
#else
    printf("%s: relative residual 2-norm = %e\n\n", solvername, resid);
#endif
    //copy solution vector back to python
    for (i = 0; i < len_xarr; i++) {
        lis_vector_get_value(X, i, (x_arr + i));
    }
    // write residual infos
    lis_solver_output_rhistory(solver, "residual.log");
    // clean up
    lis_vector_destroy(X);
    lis_vector_destroy(B);
    lis_matrix_unset(A);
    lis_matrix_destroy(A);
    lis_solver_destroy(solver);
    lis_finalize();
    LIS_DEBUG_FUNC_OUT;
    printf("LIS terminated...\n");
    return;
}

PYBIND11_PLUGIN(lis_wrapper) {
    pybind11::module m("lis_wrapper", "Test LIS interface");
    m.def("lis", &wrapper, "Call LIS wrapper");
    return m.ptr();
}

