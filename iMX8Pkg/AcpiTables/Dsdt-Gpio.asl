/*
* Description: iMX8M GPIO Controller
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

Device (GPIO)
{
    Name (_HID, "NXP0103")
    Name (_UID, 0x0)
    Method (_STA)
    {
        Return(0xf)
    }
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate () {
                MEMORY32FIXED(ReadWrite, 0x30200000, 0x50000, )  // GPIO1-5
                MEMORY32FIXED(ReadWrite, 0x30330000, 0x10000, )  // IOMUXC

                Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 96, 97 }     // GPIO1 0-15, 16-31
                Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 98, 99 }     // GPIO2 0-15, 16-31
                Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 100, 101 }   // GPIO3 0-15, 16-31
                Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 102, 103 }   // GPIO4 0-15, 16-31
                Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 104, 105 }   // GPIO5 0-15, 16-31
        })
        Return(RBUF)
    }
}