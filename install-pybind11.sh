#!/bin/bash
set -ex
git clone https://github.com/pybind/pybind11.git
#last working version
#wget https://github.com/pybind/pybind11/archive/29b5064e9c80be7034fc9d6ac15dbb7ebe304fc5.zip
#unzip 29b5064e9c80be7034fc9d6ac15dbb7ebe304fc5.zip
#ln -s pybind11-29b5064e9c80be7034fc9d6ac15dbb7ebe304fc5 pybind11
cd pybind11
cmake  -DPYBIND11_PYTHON_VERSION=2.7 -DPYBIND11_CPP_STANDARD=-std=c++14 -DPYBIND11_WERROR=OFF
make -j2 pytest
