extern "C" {
    void c_lis(double* val, int nnz, int* col, int* ind, double* x_arr, int len_xarr,
            double* b, int info, const char* lis_cmd, const char* logfname);
}
