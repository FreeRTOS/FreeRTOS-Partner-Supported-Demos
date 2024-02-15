/*******************************************************************************
// Copyright (c) 2003-2015 Cadence Design Systems, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--------------------------------------------------------------------------------
*/

#ifndef TESTCOMMON_H
#define TESTCOMMON_H

#include    <stdlib.h>

//#include    <xtensa/xtruntime.h>
//#include    <xtensa/tie/xt_timer.h>

#include    "FreeRTOS.h"

#include    "xtensa_context.h"
#include    "xtensa_rtos.h"


/*
*********************************************************************************************************
*                                             MISCELLANEOUS
*********************************************************************************************************
*/

#if defined(XT_SIMULATOR) || defined(XT_GDBIO)
#include <unistd.h>
/* Output a single char to the console device (defined in board library and xtbsp.h, except for sim). */
static inline void outbyte(char c)
{
    write(1, &c, 1);
}
#endif

#endif /* TESTCOMMON_H */

