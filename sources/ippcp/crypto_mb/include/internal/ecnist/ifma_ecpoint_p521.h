/*************************************************************************
* Copyright (C) 2002 Intel Corporation
*
* Licensed under the Apache License,  Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* 	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law  or agreed  to  in  writing,  software
* distributed under  the License  is  distributed  on  an  "AS IS"  BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the  specific  language  governing  permissions  and
* limitations under the License.
*************************************************************************/

#ifndef IFMA_ECPOINT_P521_H
#define IFMA_ECPOINT_P521_H

#include "crypto_mb/status.h"
#include <internal/ecnist/ifma_arith_p521.h>

#ifndef BN_OPENSSL_DISABLE
#include <openssl/bn.h>
#include <openssl/ec.h>
#ifdef OPENSSL_IS_BORINGSSL
#include <openssl/ecdsa.h>
#endif
#endif /* BN_OPENSSL_DISABLE */

#if (_MBX>=_MBX_K1)

typedef struct {
   U64 X[P521_LEN52];
   U64 Y[P521_LEN52];
   U64 Z[P521_LEN52];
} P521_POINT;

typedef struct {
   U64 x[P521_LEN52];
   U64 y[P521_LEN52];
} P521_POINT_AFFINE;

typedef struct {
   int64u x[P521_LEN52];
   int64u y[P521_LEN52];
} SINGLE_P521_POINT_AFFINE;

/* check if coordinate is zero */
__MBX_INLINE __mb_mask MB_FUNC_NAME(is_zero_point_cordinate_)(const U64 T[])
{
   return MB_FUNC_NAME(is_zero_FE521_)(T);
}

/* set point to infinity */
__MBX_INLINE void MB_FUNC_NAME(set_point_to_infinity_)(P521_POINT* r)
{
   r->X[0] = r->X[1] = r->X[2] = r->X[3] = r->X[4] = r->X[5] = r->X[6] = r->X[7] = r->X[8] = r->X[9] = r->X[10] = get_zero64();
   r->Y[0] = r->Y[1] = r->Y[2] = r->Y[3] = r->Y[4] = r->Y[5] = r->Y[6] = r->Y[7] = r->Y[8] = r->Y[9] = r->Y[10] = get_zero64();
   r->Z[0] = r->Z[1] = r->Z[2] = r->Z[3] = r->Z[4] = r->Z[5] = r->Z[6] = r->Z[7] = r->Z[8] = r->Z[9] = r->Z[10] = get_zero64();
}

/* set point to infinity by mask */
__MBX_INLINE void MB_FUNC_NAME(mask_set_point_to_infinity_)(P521_POINT* r, __mb_mask mask)
{
   U64 zeros = get_zero64();

   r->X[0] = mask_mov64(r->X[0], mask, zeros);
   r->X[1] = mask_mov64(r->X[1], mask, zeros);
   r->X[2] = mask_mov64(r->X[2], mask, zeros);
   r->X[3] = mask_mov64(r->X[3], mask, zeros);
   r->X[4] = mask_mov64(r->X[4], mask, zeros);
   r->X[5] = mask_mov64(r->X[5], mask, zeros);
   r->X[6] = mask_mov64(r->X[6], mask, zeros);
   r->X[7] = mask_mov64(r->X[7], mask, zeros);
   r->X[8] = mask_mov64(r->X[8], mask, zeros);
   r->X[9] = mask_mov64(r->X[9], mask, zeros);
   r->X[10]= mask_mov64(r->X[10],mask, zeros);

   r->Y[0] = mask_mov64(r->Y[0], mask, zeros);
   r->Y[1] = mask_mov64(r->Y[1], mask, zeros);
   r->Y[2] = mask_mov64(r->Y[2], mask, zeros);
   r->Y[3] = mask_mov64(r->Y[3], mask, zeros);
   r->Y[4] = mask_mov64(r->Y[4], mask, zeros);
   r->Y[5] = mask_mov64(r->Y[5], mask, zeros);
   r->Y[6] = mask_mov64(r->Y[6], mask, zeros);
   r->Y[7] = mask_mov64(r->Y[7], mask, zeros);
   r->Y[8] = mask_mov64(r->Y[8], mask, zeros);
   r->Y[9] = mask_mov64(r->Y[9], mask, zeros);
   r->Y[10]= mask_mov64(r->Y[10],mask, zeros);

   r->Z[0] = mask_mov64(r->Z[0], mask, zeros);
   r->Z[1] = mask_mov64(r->Z[1], mask, zeros);
   r->Z[2] = mask_mov64(r->Z[2], mask, zeros);
   r->Z[3] = mask_mov64(r->Z[3], mask, zeros);
   r->Z[4] = mask_mov64(r->Z[4], mask, zeros);
   r->Z[5] = mask_mov64(r->Z[5], mask, zeros);
   r->Z[6] = mask_mov64(r->Z[6], mask, zeros);
   r->Z[7] = mask_mov64(r->Z[7], mask, zeros);
   r->Z[8] = mask_mov64(r->Z[8], mask, zeros);
   r->Z[9] = mask_mov64(r->Z[9], mask, zeros);
   r->Z[10]= mask_mov64(r->Z[10],mask, zeros);
}

/* set affine point to infinity */
__MBX_INLINE void MB_FUNC_NAME(set_point_affine_to_infinity_)(P521_POINT_AFFINE* r)
{
   r->x[0] = r->x[1] = r->x[2] = r->x[3] = r->x[4] = r->x[5] = r->x[6] = r->x[7] = r->x[8] = r->x[9] = r->x[10] = get_zero64();
   r->y[0] = r->y[1] = r->y[2] = r->y[3] = r->y[4] = r->y[5] = r->y[6] = r->y[7] = r->y[8] = r->y[9] = r->y[10] = get_zero64();
}

EXTERN_C void MB_FUNC_NAME(ifma_ec_nistp521_dbl_point_)(P521_POINT* r, const P521_POINT* p);
EXTERN_C void MB_FUNC_NAME(ifma_ec_nistp521_add_point_)(P521_POINT* r, const P521_POINT* p, const P521_POINT* q);
EXTERN_C void MB_FUNC_NAME(ifma_ec_nistp521_add_point_affine_)(P521_POINT* r, const P521_POINT* p, const P521_POINT_AFFINE* q);
EXTERN_C void MB_FUNC_NAME(ifma_ec_nistp521_mul_point_)(P521_POINT* r, const P521_POINT* p, const U64* scalar);
EXTERN_C void MB_FUNC_NAME(ifma_ec_nistp521_mul_pointbase_)(P521_POINT* r, const U64* scalar);
EXTERN_C void MB_FUNC_NAME(get_nistp521_ec_affine_coords_)(U64 x[], U64 y[], const P521_POINT* P);
EXTERN_C __mb_mask MB_FUNC_NAME(ifma_is_on_curve_p521_)(const P521_POINT* p, int use_jproj_coords);


#ifndef BN_OPENSSL_DISABLE

mbx_status internal_avx512_nistp521_ecdh_ssl_mb8(int8u* pa_shared_key[8],
                                           const BIGNUM* const pa_skey[8],
                                           const BIGNUM* const pa_pubx[8],
                                           const BIGNUM* const pa_puby[8],
                                           const BIGNUM* const pa_pubz[8],
                                           int8u* pBuffer, int use_jproj_coords);

mbx_status internal_avx512_nistp521_ecdsa_sign_setup_ssl_mb8(BIGNUM* pa_inv_skey[8],
                                                             BIGNUM* pa_sign_rp[8],
                                                       const BIGNUM* const pa_eph_skey[8],
                                                             int8u* pBuffer);

mbx_status internal_avx512_nistp521_ecdsa_sign_complete_ssl_mb8(int8u* pa_sign_r[8],
                                                                int8u* pa_sign_s[8],
                                                          const int8u* const pa_msg[8],
                                                          const BIGNUM* const pa_sign_rp[8],
                                                          const BIGNUM* const pa_inv_eph_skey[8],
                                                          const BIGNUM* const pa_reg_skey[8],
                                                                int8u* pBuffer);

mbx_status internal_avx512_nistp521_ecdsa_sign_ssl_mb8(int8u* pa_sign_r[8],
                                                       int8u* pa_sign_s[8],
                                                 const int8u* const pa_msg[8],
                                                 const BIGNUM* const pa_eph_skey[8],
                                                 const BIGNUM* const pa_reg_skey[8],
                                                       int8u* pBuffer);

mbx_status internal_avx512_nistp521_ecdsa_verify_ssl_mb8(const ECDSA_SIG* const pa_sig[8],
                                                         const int8u* const pa_msg[8],                                             
                                                         const BIGNUM* const pa_pubx[8],
                                                         const BIGNUM* const pa_puby[8],
                                                         const BIGNUM* const pa_pubz[8],                                       
                                                         int8u* pBuffer, int use_jproj_coords);

mbx_status internal_avx512_nistp521_ecpublic_key_ssl_mb8(BIGNUM* pa_pubx[8],
                                                         BIGNUM* pa_puby[8],
                                                         BIGNUM* pa_pubz[8],
                                                   const BIGNUM* const pa_skey[8],
                                                   int8u* pBuffer, int use_jproj_coords);

#endif /* BN_OPENSSL_DISABLE */

mbx_status internal_avx512_nistp521_ecdh_mb8(int8u* pa_shared_key[8],
                                       const int64u* const pa_skey[8], 
                                       const int64u* const pa_pubx[8],
                                       const int64u* const pa_puby[8],
                                       const int64u* const pa_pubz[8],
                                             int8u* pBuffer, int use_jproj_coords);

mbx_status internal_avx512_nistp521_ecdsa_sign_setup_mb8(int64u* pa_inv_eph_skey[8],
                                                         int64u* pa_sign_rp[8],
                                                   const int64u* const pa_eph_skey[8],
                                                         int8u* pBuffer);

mbx_status internal_avx512_nistp521_ecdsa_sign_complete_mb8(int8u* pa_sign_r[8],
                                                            int8u* pa_sign_s[8],
                                                      const int8u* const pa_msg[8],
                                                      const int64u* const pa_sign_rp[8],
                                                      const int64u* const pa_inv_eph_skey[8],
                                                      const int64u* const pa_reg_skey[8],
                                                            int8u* pBuffer);

mbx_status internal_avx512_nistp521_ecdsa_sign_mb8(int8u* pa_sign_r[8],
                                                   int8u* pa_sign_s[8],
                                             const int8u* const pa_msg[8],
                                             const int64u* const pa_eph_skey[8],
                                             const int64u* const pa_reg_skey[8],
                                                   int8u* pBuffer);

mbx_status internal_avx512_nistp521_ecdsa_verify_mb8(const int8u* const pa_sign_r[8],
                                                      const int8u* const pa_sign_s[8],
                                                      const int8u* const pa_msg[8],
                                                      const int64u* const pa_pubx[8],
                                                      const int64u* const pa_puby[8],
                                                      const int64u* const pa_pubz[8],                                       
                                                      int8u* pBuffer, int use_jproj_coords);

mbx_status internal_avx512_nistp521_ecpublic_key_mb8(int64u* pa_pubx[8],
                                                      int64u* pa_puby[8],
                                                      int64u* pa_pubz[8],
                                                const int64u* const pa_skey[8],
                                                int8u* pBuffer, int use_jproj_coords);

#endif /* #if (_MBX>=_MBX_K1) */

#endif  /* IFMA_ECPOINT_P521_H */
