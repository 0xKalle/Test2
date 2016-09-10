#!/bin/bash
set -ex
wget http://www.ssisc.org/lis/dl/lis-1.6.6.tar.gz
tar -xzvf lis-1.6.6.tar.gz
patch -p0 < lis.patch
cd lis-1.6.6 && ./configure --prefix=/usr/local/lis --enable-omp  --enable-shared  && make -j2 && sudo make install

