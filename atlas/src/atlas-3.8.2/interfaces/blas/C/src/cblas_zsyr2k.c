/*
 *             Automatically Tuned Linear Algebra Software v3.8.2
 *                    (C) Copyright 1999 R. Clint Whaley
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
#ifdef ATL_USEPTHREADS
   #include "atlas_ptalias3.h"
#endif
#include "cblas.h"

void ATL_zsyr2k(const enum CBLAS_UPLO Uplo,
                const enum CBLAS_TRANSPOSE Trans, const int N, const int K,
                const double * alpha, const double *A, const int lda,
                const double *B, const int ldb, const double * beta,
                double *C, const int ldc);

void cblas_zsyr2k(const enum CBLAS_ORDER Order, const enum CBLAS_UPLO Uplo,
                  const enum CBLAS_TRANSPOSE Trans, const int N, const int K,
                  const void * alpha, const void *A, const int lda,
                  const void *B, const int ldb, const void * beta,
                  void *C, const int ldc)
{
   enum CBLAS_UPLO uplo;
   enum CBLAS_TRANSPOSE trans;

#ifndef NoCblasErrorChecks
   int info = 2000;
   if (Uplo != CblasUpper && Uplo != CblasLower)
      info = cblas_errprn(2, info, "UPLO must be %d or %d, but is set to %d",
                               CblasUpper, CblasLower, Uplo);
   if (N < 0) info = cblas_errprn(4, info,
                     "N cannot be less than zero; it is set to %d.", N);
   if (K < 0) info = cblas_errprn(5, info,
                     "K cannot be less than zero; it is set to %d.", K);
   if (Order == CblasColMajor)
   {
      if (Trans == AtlasNoTrans)
      {
         if ( (lda < N) || (lda < 1) )
            info = cblas_errprn(8, info, "lda must be >= MAX(N,1): lda=%d N=%d",
                                lda, N);
         if ( (ldb < N) || (ldb < 1) )
            info = cblas_errprn(10, info,"ldb must be >= MAX(N,1): ldb=%d N=%d",
                                ldb, N);
      }
      else
      {
         if (Trans != AtlasTrans)
            info = cblas_errprn(3, info,
                                "Trans must be %d or %d, but is set to %d",
                                CblasNoTrans, CblasTrans, Trans);
         if ( (lda < K) || (lda < 1) )
            info = cblas_errprn(8, info, "lda must be >= MAX(K,1): lda=%d K=%d",
                                lda, K);
         if ( (ldb < K) || (ldb < 1) )
            info = cblas_errprn(10, info,"ldb must be >= MAX(K,1): ldb=%d K=%d",
                                ldb, K);
      }
   }
   else if (Order == CblasRowMajor)
   {
      if (Trans == AtlasNoTrans)
      {
         if ( (lda < K) || (lda < 1) )
            info = cblas_errprn(8, info, "lda must be >= MAX(K,1): lda=%d K=%d",
                                lda, K);
         if ( (ldb < K) || (ldb < 1) )
            info = cblas_errprn(10, info,"ldb must be >= MAX(K,1): ldb=%d K=%d",
                                ldb, K);
      }
      else
      {
         if (Trans != AtlasTrans)
            info = cblas_errprn(3, info,
                                "Trans must be %d or %d, but is set to %d",
                                CblasNoTrans, CblasTrans, Trans);
         if ( (lda < N) || (lda < 1) )
            info = cblas_errprn(8, info, "lda must be >= MAX(N,1): lda=%d N=%d",
                                lda, N);
         if ( (ldb < N) || (ldb < 1) )
            info = cblas_errprn(10, info,"ldb must be >= MAX(N,1): ldb=%d N=%d",
                                ldb, N);
      }
   }
   else info = cblas_errprn(1, info, "Order must be %d or %d, but is set to %d",
                            CblasRowMajor, CblasColMajor, Order);

   if ( (ldc < N) || (ldc < 1) )
      info = cblas_errprn(13, info, "ldc must be >= MAX(N,1): ldc=%d N=%d",
                          ldc, N);
   if (info != 2000)
   {
      cblas_xerbla(info, "cblas_zsyr2k", "");
      return;
   }
#endif

   if (Order == CblasColMajor)
      ATL_zsyr2k(Uplo, Trans, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
   else
   {
      if (Uplo == CblasUpper) uplo = CblasLower;
      else uplo = CblasUpper;
      if (Trans == CblasNoTrans) trans = CblasTrans;
      else trans = CblasNoTrans;
      ATL_zsyr2k(uplo, trans, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
   }
}
