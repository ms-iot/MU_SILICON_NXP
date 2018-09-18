/*
* Description: iMX8MQuad UART Controllers
*
*  Copyright (c) Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
*/

Device (UAR1)
{
    Name (_HID, "NXP0107")
    Name (_UID, 0x1)
    Name (_DDN, "UART1")
    Method (_STA)
    {
        Return(0x0)
    }
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate () {
            MEMORY32FIXED(ReadWrite, 0x30860000, 0x4000, )
            Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) { 58 }
       })
       Return(RBUF)
    }
}

Device (UAR2)
{
    Name (_HID, "NXP0107")
    Name (_UID, 0x2)
    Name (_DDN, "UART2")
    Method (_STA)
    {
        Return(0x0)
    }
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate () {
            MEMORY32FIXED(ReadWrite, 0x30890000, 0x4000, )
            Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 59 }
        })
        Return(RBUF)
    }
}

Device (UAR3)
{
    Name (_HID, "NXP0107")
    Name (_UID, 0x3)
    Name (_DDN, "UART3")
    Method (_STA)
    {
       Return(0x0)
    }
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate () {
            MEMORY32FIXED(ReadWrite, 0x30880000, 0x4000, )
            Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 60 }
        })
        Return(RBUF)
    }
}

Device (UAR4)
{
    Name (_HID, "NXP0107")
    Name (_UID, 0x4)
    Name (_DDN, "UART4")
    Method (_STA)
    {
        Return(0x0)
    }
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate () {
            MEMORY32FIXED(ReadWrite, 0x30A60000, 0x4000, )
            Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 61 }
        })
        Return(RBUF)
    }
}
