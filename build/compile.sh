#!/usr/bin/env bash
set -e

if [ ${TRAVIS_OS_NAME} = "linux" ]; then
    QT5DIR=/opt/qt55
    PACKAGE=TGZ
else
    QT5DIR=/usr/local/opt/qt5
    PACKAGE=Bundle
fi

cmake \
	-D EIGEN_INCLUDE:PATH=$PWD/eigen \
	-D Qt5_DIR:PATH=${QT5DIR} \
	-D FFTWF_LIBRARY:PATH=libfftw3f-3 \
	-D HDF5_INCLUDE:PATH=hdf5-1.8.16/hdf5/include \
	-D HDF5_LIBRARY:PATH=hdf5-1.8.16/hdf5/lib/libhdf5.a \
	-D HDF5_HL_LIBRARY:PATH=hdf5-1.8.16/hdf5/lib/libhdf5_hl.a \
	-D CPACK_GENERATOR=${PACKAGE} \
	-D OPENPSTD_VERSION_MAJOR=${OPENPSTD_MAJOR_VERSION} \
	-D OPENPSTD_VERSION_MINOR=${OPENPSTD_MINOR_VERSION} \
	-D OPENPSTD_VERSION_PATCH=${TRAVIS_BUILD_NUMBER} \
	-D OPENPSTD_SYSTEM_NAME=${TRAVIS_OS_NAME} \
	-G Unix\ Makefiles ./

make OpenPSTD-cli
make OpenPSTD-gui
make OpenPSTD-test
sudo cpack -V -G ${PACKAGE}
ls -alh
