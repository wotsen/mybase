/**
 * @file macros.h
 * @author wotsen (astralrovers@outlook.com)
 * @brief 
 * @version 0.0.0
 * @date 2020-09-12
 * 
 * @copyright 
 * MIT License
 * 
 * Copyright (c) 2020 wotsen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.

 * 
 */

#ifndef __mybase_MACROS_H__
#define __mybase_MACROS_H__

///< 色彩
#define COLORE_VTSEQ(ID) ("\x1b[" #ID "m")

#define COLORE_BLACK      COLORE_VTSEQ(30)
#define COLORE_RED        COLORE_VTSEQ(31)
#define COLORE_GREEN      COLORE_VTSEQ(32)
#define COLORE_YELLOW     COLORE_VTSEQ(33)
#define COLORE_BLUE       COLORE_VTSEQ(34)
#define COLORE_PURPLE     COLORE_VTSEQ(35)
#define COLORE_CYAN       COLORE_VTSEQ(36)
#define COLORE_LIGHT_GRAY COLORE_VTSEQ(37)
#define COLORE_WHITE      COLORE_VTSEQ(37)
#define COLORE_LIGHT_RED  COLORE_VTSEQ(91)
#define COLORE_DIM        COLORE_VTSEQ(2)
#define COLORE_BOLD       COLORE_VTSEQ(1)
#define COLORE_UNDERLINE  COLORE_VTSEQ(4)
#define COLORE_RESET      COLORE_VTSEQ(0)

///< 数组长度
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#endif // !__mybase_MACROS_H__
