# UEFI loader protocol proxy

[![Build](https://img.shields.io/github/workflow/status/ipxe/ProxyLoaderPkg/Build)](https://github.com/ipxe/ProxyLoaderPkg/actions?query=workflow%3ABuild+branch%3Amaster)
[![Coverity](https://img.shields.io/coverity/scan/16649)](https://scan.coverity.com/projects/ipxe-ProxyLoaderPkg)

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

## FAQ

### Why is it needed?

The UEFI Specification defines the `EFI_LOADED_IMAGE_PROTOCOL`, which
represents an image that has been verified, loaded into memory, and is
ready to be executed.

The basic design principle for UEFI protocols is that the published
interface should completely define the protocol API.  It should
therefore be possible for suitably verified application code to create
a new handle and install a custom `EFI_LOADED_IMAGE_PROTOCOL`
instance.  This protocol instance could represent an image stored in a
custom executable format, an image verified using a delegated security
policy, or any other image that cannot be loaded directly via the
firmware's built-in `LoadImage()` function.

Unfortunately there is a small design flaw in the UEFI specification
that makes it impossible for application code to install a custom
`EFI_LOADED_IMAGE_PROTOCOL` instance.  Specifically: the firmware's
built-in `Exit()` function must rely on the existence of unspecified
state hidden in the structure containing the
`EFI_LOADED_IMAGE_PROTOCOL` instance in order to locate its
`SetJump()` buffer.  Since the layout of this containing structure is
unspecified, it cannot be relied upon and so there is no safe way for
application code to install a custom `EFI_LOADED_IMAGE_PROTOCOL`
instance.

This problem can be solved by using the UEFI loader protocol proxy.

### How does it work?

The UEFI loader protocol proxy is a small UEFI application that does
only one task.  When started, it will look for a
`PROXY_LOADER_PROTOCOL` instance installed on its own image handle.
It will then jump to the entry point defined in the
`PROXY_LOADER_PROTOCOL` instance.

The `PROXY_LOADER_PROTOCOL` is defined in
[ProxyLoader.h](Application/ProxyLoader/ProxyLoader.h).

Trusted code that needs to install a custom
`EFI_LOADED_IMAGE_PROTOCOL` instance may call the firmware's built-in
`LoadImage()` function to load a copy of the UEFI loader protocol
proxy application, and then install a `PROXY_LOADER_PROTOCOL` instance
on the image handle to identify the real entry point for the image.

The UEFI loader protocol proxy application does not rely upon and will
not modify any fields within the `EFI_LOADED_IMAGE_PROTOCOL` instance.
These fields may be modified as needed (e.g. to configure
`LoadOptions` for the image).

### Is it secure?

Yes.  Any code that is already authorized to install a
`PROXY_LOADER_PROTOCOL` instance would already be able to simply jump
directly to the specified entry point.
