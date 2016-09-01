GCC=gcc-6.2.0
#GCC=gcc-5.3.0
#GCC=gcc-4.8.2
source /opt/intel/mkl/bin/mklvars.sh intel64
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lis/lib:/usr/local/$GCC/lib64
export OMP_NUM_THREADS=4

export PATH=/usr/local/$GCC/bin:$PATH
export CC=/usr/local/$GCC/bin/gcc
export CXX=/usr/local/$GCC/bin/g++


export CC=/usr/local/$GCC/bin/gcc
export CXX=/usr/local/$GCC/bin/g++
