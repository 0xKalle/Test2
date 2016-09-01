import lis_wrapper
import numpy as np
from pysparse import itsolvers
from pysparse import precon
from pysparse.sparse import spmatrix
import scipy.sparse
b = np.ones(8)
x = np.zeros(8)
A = spmatrix.ll_mat_sym(8, 8)

def transpose(a):
    b = spmatrix.ll_mat(a.shape[1], a.shape[0], a.nnz)
    v, r, c = a.find()
    b.put(v, c, r)
    return b

#   double a[18] = 
#    { 7.0,      1.0,           2.0, 7.0,
#          -4.0, 8.0,      2.0,
#                1.0,                     5.0,
#                     7.0,           9.0,
#                          5.0, 1.0, 5.0,
#                              -1.0,      5.0,
#                                   11.0,
#                                         5.0

# ll_mat_sym nur mit unterer Dreiecksmatrix

A[0, 0] = 1.0  #7.0
#A[0,1]
A[2, 0] = 1.0
#A[0,3]
#A[0,4]
A[5, 0] = 2.0
A[6, 0] = 7.0
#A[0,7]

#A[1,0]
A[1, 1] = 1.0 #-4.0
A[2, 1] = 8.0
#A[1,3]
A[4, 2] = 2.0
#A[1,5]
#A[1,6]
#A[1,7]

#A[2,0] =
#A[2,1]
A[2, 2] = 1.0
#A[2,3]
#A[2,4]
#A[2,5]
#A[2,6]
A[7, 2] = 5.0

#A[3,0]
#A[3,1]
#A[3,2]
A[3, 3] = 1.0 #7.0
#A[3,4]
#A[3,5]
A[6, 3] = 9.0
#A[3,7]

#   double a[18] = 
#    { 7.0,      1.0,           2.0, 7.0,
#          -4.0, 8.0,      2.0,
#                1.0,                     5.0,
#                     7.0,           9.0,
#                          5.0, 1.0, 5.0,
#                              -1.0,      5.0,
#                                   11.0,
#                                         5.0

#A[4,0]
#A[4,1]
#A[4,2]
#A[4,3]
A[4, 4] = 1.0 #5.0
A[5, 4] = 1.0
A[6, 4] = 5.0
#A[4,7]

#A[5,0]
#A[5,1]
#A[5,2]
#A[5,3]
#A[5,4]
A[5, 5] = 1.0 # -1.0
#A[5,6]
A[7, 5] = 5.0

#A[6,0]
#A[6,1]
#A[6,2]
#A[6,3]
#A[6,4]
#A[6,5]
A[6, 6] = 1.0 #11.0
#A[6,7]

A[7, 7] = 1.0 #5.0

print A
Acsr = A.to_csr(0) #mklflag=0
print "A im CSR-Format:"
print "values  : ", Acsr.values()
print "columns : ", Acsr.columns()
print "pointers: ", Acsr.pointers()
print "Acsr.shape:  ", Acsr.shape
print "nnz:  ", Acsr.nnz
print "nval: ", len(Acsr.values())
print "ncol: ", len(Acsr.columns())
print "nind: ", len(Acsr.pointers())



x = np.zeros(8)
b = np.ones(8)
y = np.zeros(8)

# scalar params: verbose, abs_tol, rel_tol, div_tol, max_iter, single
# single = 1: solver uses float
# single = 0: solver users double

#lis(Acsr.values(), Acsr.columns(), Acsr.pointers(), x, b, 1, 1e-6, 1e-12, 1e5, 11111, 1) #, '-ssor_w 0.1')

lis_wrapper.lis(Acsr.values(), Acsr.columns(), Acsr.pointers(), x, b, 1, 1e-4, 10000)
print "Rhs:", b
print "Sol:", x
A.matvec(x, y)
print "Proof:"
print "A*x   :", y
y = np.zeros(8)
Acsr.matvec(x, y)
print "Acsr*x:", y

print "Test with scipy.sparse"

AA = np.zeros((8, 8), dtype=np.float64)
print AA
AA[0, 0] = 1.0  #7.0
#AA[0,1]
AA[2, 0] = 1.0
#AA[0,3]
#AA[0,4]
AA[5, 0] = 2.0
AA[6, 0] = 7.0
#AA[0,7]

#AA[1,0]
AA[1, 1] = 1.0 #-4.0
AA[2, 1] = 8.0
#AA[1,3]
AA[4, 2] = 2.0
#AA[1,5]
#AA[1,6]
#AA[1,7]

#AA[2,0] =
#AA[2,1]
AA[2, 2] = 1.0
#AA[2,3]
#AA[2,4]
#AA[2,5]
#AA[2,6]
AA[7, 2] = 5.0

#AA[3,0]
#AA[3,1]
#AA[3,2]
AA[3, 3] = 1.0 #7.0
#AA[3,4]
#AA[3,5]
AA[6, 3] = 9.0
#AA[3,7]

#   double a[18] = 
#    { 7.0,      1.0,           2.0, 7.0,
#          -4.0, 8.0,      2.0,
#                1.0,                     5.0,
#                     7.0,           9.0,
#                          5.0, 1.0, 5.0,
#                              -1.0,      5.0,
#                                   11.0,
#                                         5.0

#AA[4,0]
#AA[4,1]
#AA[4,2]
#AA[4,3]
AA[4, 4] = 1.0 #5.0
AA[5, 4] = 1.0
AA[6, 4] = 5.0
#AA[4,7]

#AA[5,0]
#AA[5,1]
#AA[5,2]
#AA[5,3]
#AA[5,4]
AA[5, 5] = 1.0 # -1.0
#AA[5,6]
AA[7, 5] = 5.0

#AA[6,0]
#AA[6,1]
#AA[6,2]
#AA[6,3]
#AA[6,4]
#AA[6,5]
AA[6, 6] = 1.0 #11.0
#AA[6,7]

AA[7, 7] = 1.0 #5.0

print AA
# dense matrix to sparse matrix (lower triangle)
AAcsr = scipy.sparse.csr_matrix(AA)

# LIS Manual: Appendix File Formats
# "Note that both the upper and lower triangular entries need to be stored
# irrespective of whether the matrix is symmetric or not."
# convert lower triangular csr matrix to 'full' csr matrix for LIS

AAcsr_full = AAcsr + AAcsr.T - scipy.sparse.diags(AAcsr.diagonal())
print
print "Test scipy.sparse"
x = np.zeros(8)
b = np.ones(8)
y = np.zeros(8)
lis_wrapper.lis(AAcsr_full.data, AAcsr_full.indices, AAcsr_full.indptr, x, b, 1, 1e-4, 10000)
# convert lower trianguar matrix AA to 'full' matrix
y = (AA + AA.T - np.eye(AA.shape[0]) * AA.diagonal()).dot(x)
print y

y = AAcsr_full.dot(x)
print y
