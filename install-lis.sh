#!/bin/bash
set -ex
git clone https://github.com/anishida/lis.git
patch -p0 < lis.patch
cd lis && ./configure --prefix=$HOME/lis --enable-omp  --enable-shared  && make -j2 && make check && make install
