/*
 *             Automatically Tuned Linear Algebra Software v3.8.2
 *                    (C) Copyright 1997 R. Clint Whaley
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions, and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. The name of the ATLAS group or the names of its contributers may
 *      not be used to endorse or promote products derived from this
 *      software without specific written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE ATLAS GROUP OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "atlas_misc.h"
#include "atlas_lvl3.h"
void Mjoin(PATL,IBJBmm)(int IB, int JB, int K, const TYPE *A, const TYPE *B,
                        TYPE beta, TYPE *C, const int ldc)
{
   const int nKb = ATL_DivByNB(K);
   const int incB = ATL_MulByNB(JB)SHIFT, incA = ATL_MulByNB(IB)SHIFT;
   #ifdef TREAL
      #define zero ATL_rzero
   #else
      const TYPE zero[2] = {ATL_rzero, ATL_rzero};
   #endif
   int k;

   if (beta == ATL_rzero) Mjoin(PATL,gezero)(IB, JB, C, ldc);
   if (nKb)
   {
      Mjoin(PATL,pKBmm)(IB, JB, KB, ATL_rone, A, NB, B, NB, beta, C, ldc);
      B += incB;
      A += incA;
      for (k=nKb-1; k; k--)
      {
         Mjoin(PATL,pKBmm)(IB, JB, KB, ATL_rone, A, NB, B, NB, ATL_rone,
                           C, ldc);
         B += incB;
         A += incA;
      }
      if (k = K-ATL_MulByNB(nKb))
         Mjoin(PATL,pKBmm)(IB, JB, k, ATL_rone, A, k, B, k, ATL_rone, C, ldc);
   }
   else if (k = K-ATL_MulByNB(nKb))
      Mjoin(PATL,pKBmm)(IB, JB, k, ATL_rone, A, k, B, k, beta, C, ldc);
}
