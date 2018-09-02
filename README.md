# UEFI loader protocol proxy

[![Build Status](https://travis-ci.org/ipxe/ProxyLoaderPkg.svg?branch=master)](https://travis-ci.org/ipxe/ProxyLoaderPkg)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/16649/badge.svg)](https://scan.coverity.com/projects/ipxe-proxyloaderpkg)

This is a small UEFI application that allows trusted code to create a
valid `EFI_LOADED_IMAGE_PROTOCOL` instance that can be used to
transfer control to an arbitrary trusted entry point.

## Prerequisites

You need a working [EDK II](https://github.com/tianocore/edk2) build
environment.

On Linux, you can create a working EDK II build environment with the
following commands:

    git clone https://github.com/tianocore/edk2.git
    cd edk2
    . edksetup.sh
    make -C BaseTools/Source/C
    cd ..

## Building

Clone this repository within your existing EDK II build environment,
then build the `ProxyLoaderPkg` package.

On Linux, you can build with the following commands:

    cd edk2
    . edksetup.sh
    git clone https://github.com/ipxe/ProxyLoaderPkg.git
    build -a X64 -t GCC5 -p ProxyLoaderPkg/ProxyLoaderPkg.dsc
