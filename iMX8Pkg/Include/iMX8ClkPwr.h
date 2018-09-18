/** @file
*
*  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef _IMX8_CLK_PWR_H_
#define _IMX8_CLK_PWR_H_

//
// Public functions
//
VOID
ImxUngateActiveClock (
  VOID
  );

EFI_STATUS
ImxSetSAI2ClockRate (
  UINT32 ClockRate
  );

#endif

