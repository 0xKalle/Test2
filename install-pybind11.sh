#!/bin/bash
set -ex
#git clone https://github.com/pybind/pybind11.git
#cd pybind11
wget https://github.com/pybind/pybind11/archive/v2.2.3.tar.gz
cd pybind11-2.2.3
cmake  -DPYBIND11_PYTHON_VERSION=$1 -DPYBIND11_CPP_STANDARD=$2 $3 -DPYBIND11_WERROR=OFF
make -j2 pytest
