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
#include "atlas_kern3.h"

#ifdef Upper_
   #define syr2k_put Mjoin(PATL,syr2k_putU)
   int Mjoin(PATL,syr2kUT)
#else
   #define syr2k_put Mjoin(PATL,syr2k_putL)
   int Mjoin(PATL,syr2kLT)
#endif
   (const int N, const int K, const void *valpha, const void *A, const int lda,
    const void *B, const int ldb, const void *vbeta, void *C, const int ldc)
{
   int i;
   void *vc=NULL;
   TYPE *c;
   #ifdef TREAL
      const SCALAR alpha=*( (const SCALAR *)valpha );
      const SCALAR beta =*( (const SCALAR *)vbeta  );
      const SCALAR one=1.0, zero=0.0;
   #else
      #define alpha valpha
      const TYPE *beta=vbeta;
      const TYPE one[2]={1.0,0.0}, zero[2]={0.0,0.0};
   #endif

   i = ATL_MulBySize(N)*N;
   if (i <= ATL_MaxMalloc) vc = malloc(ATL_Cachelen+i);
   if (vc == NULL) return(1);
   c = ATL_AlignPtr(vc);
   CgemmTN(N, N, K, alpha, A, lda, B, ldb, zero, c, N);
   if ( SCALAR_IS_ONE(beta) ) Mjoin(syr2k_put,_b1)(N, c, beta, C, ldc);
   else if ( SCALAR_IS_ZERO(beta) ) Mjoin(syr2k_put,_b0)(N, c, beta, C, ldc);
   #ifdef TCPLX
      else if (SCALAR_IS_NONE(beta)) Mjoin(syr2k_put,_bn1)(N, c, beta, C, ldc);
      else if (beta[1] == *zero) Mjoin(syr2k_put,_bXi0)(N, c, beta, C, ldc);
   #endif
   else Mjoin(syr2k_put,_bX)(N, c, beta, C, ldc);
   free(vc);
   return(0);
}
