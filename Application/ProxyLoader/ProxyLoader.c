/** @file
  UEFI loader protocol proxy
 
  Copyright (C) 2018 Michael Brown <mbrown@fensystems.co.uk>.
 
  This program and the accompanying materials are licensed and made
  available under the terms and conditions of the BSD License which
  accompanies this distribution.  The full text of the license may be
  found at http://opensource.org/licenses/bsd-license.php.
 
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS"
  BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER
  EXPRESS OR IMPLIED.
 
**/

#include <Library/UefiBootServicesTableLib.h>
#include "ProxyLoader.h"

/**
  Image entry point.

  @param  ImageHandle		The firmware allocated handle for the EFI image.
  @param  SystemTable		A pointer to the EFI System Table.

  @retval Status		Function execution status.
**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE		ImageHandle,
  IN EFI_SYSTEM_TABLE	*SystemTable
  )
{
  PROXY_LOADER_PROTOCOL *ProxyLoader;
  EFI_STATUS Status;

  //
  // Get ProxyLoader protocol installed on image handle
  //
  Status = gBS->OpenProtocol (
    ImageHandle,
    &gProxyLoaderProtocolGuid,
    (VOID **)&ProxyLoader,
    ImageHandle,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Transfer control to proxied entry point
  //
  return ProxyLoader->Entry ( ImageHandle, SystemTable );
}
