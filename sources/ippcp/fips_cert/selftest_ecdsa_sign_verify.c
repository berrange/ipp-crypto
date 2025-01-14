/*************************************************************************
* Copyright (C) 2023 Intel Corporation
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

#ifdef IPPCP_FIPS_MODE
#include "ippcp.h"
#include "owndefs.h"
#include "dispatcher.h"

// FIPS selftests are not processed by dispatcher.
// Prevent several copies of the same functions.
#ifdef _IPP_DATA

#include "ippcp/fips_cert.h"
#include "fips_cert_internal/common.h"
#include "fips_cert_internal/bn_common.h"

/*
 * KAT
 * vectors are taken from FIPS 186 testing vectors set
 * msg ==
 * "5905238877c77421f73e43ee3da6f2d9e2ccad5fc942dcec0cbd25482935faaf"
 * "416983fe165b1a045ee2bcd2e6dca3bdf46c4310a7461f9a37960ca672d3feb5"
 * "473e253605fb1ddfd28065b53cb5858a8ad28175bf9bd386a5e471ea7a65c17c"
 * "c934a9d791e91491eb3754d03799790fe2d308d16146d5c9b0d0debd97d79ce8"
 */
/* msgDigest == SHA-256(msg) */
static const Ipp8u msg_digest[] = { 0x56,0xec,0x33,0xa1,0xa6,0xe7,0xc4,0xdb,0x77,0x03,0x90,0x1a,0xfb,0x2e,0x1e,0x4e,
                                    0x50,0x09,0xfe,0x04,0x72,0x89,0xc5,0xc2,0x42,0x13,0x6c,0xe3,0xb7,0xf6,0xac,0x44 };
/* key pair */
static const Ipp8u d[]          = { 0x64,0xb4,0x72,0xda,0x6d,0xa5,0x54,0xca,0xac,0x3e,0x4e,0x0b,0x13,0xc8,0x44,0x5b,
                                    0x1a,0x77,0xf4,0x59,0xee,0xa8,0x4f,0x1f,0x58,0x8b,0x5f,0x71,0x3d,0x42,0x9b,0x51 };
static const Ipp8u k[]          = { 0xde,0x68,0x2a,0x64,0x87,0x07,0x67,0xb9,0x33,0x5d,0x4f,0x82,0x47,0x62,0x4a,0x3b,
                                    0x7f,0x3c,0xe9,0xf9,0x45,0xf2,0x80,0xa2,0x61,0x6a,0x90,0x4b,0xb1,0xbb,0xa1,0x94 };
/* signature */
static const Ipp8u r[]          = { 0xac,0xc2,0xc8,0x79,0x6f,0x5e,0xbb,0xca,0x7a,0x5a,0x55,0x6a,0x1f,0x6b,0xfd,0x2a,
                                    0xed,0x27,0x95,0x62,0xd6,0xe3,0x43,0x88,0x5b,0x79,0x14,0xb5,0x61,0x80,0xac,0xf3 };
static const Ipp8u s[]          = { 0x03,0x89,0x05,0xcc,0x2a,0xda,0xcd,0x3c,0x5a,0x17,0x6f,0xe9,0x18,0xb2,0x97,0xef,
                                    0x1c,0x37,0xf7,0x2b,0x26,0x76,0x6c,0x78,0xb2,0xa6,0x05,0xca,0x19,0x78,0xf7,0x8b };

/* pub key coordinates */
static const Ipp8u qx[]          = { 0x83,0xbf,0x71,0xc2,0x46,0xff,0x59,0x3c,0x2f,0xb1,0xbf,0x4b,0xe9,0x5d,0x56,0xd3,
                                     0xcc,0x8f,0xdb,0x48,0xa2,0xbf,0x33,0xf0,0xf4,0xc7,0x5f,0x07,0x1c,0xe9,0xcb,0x1c};
static const Ipp8u qy[]          = { 0xa9,0x4c,0x9a,0xa8,0x5c,0xcd,0x7c,0xdc,0x78,0x4e,0x40,0xb7,0x93,0xca,0xb7,0x6d,
                                     0xe0,0x13,0x61,0x0e,0x2c,0xdb,0x1f,0x1a,0xa2,0xf9,0x11,0x88,0xc6,0x14,0x40,0xce };

static const unsigned int primeBitSize = 256;

static const unsigned int ordWordSize   = 8;
static const unsigned int msgWordSize   = 8;
static const unsigned int primeWordSize = 8;

IPPFUN(fips_test_status, fips_selftest_ippsGFpECSignVerifyDSA_get_size_GFp_buff, (int *pGFpBuffSize)) {
    /* return bad status if input pointer is NULL */
    IPP_BADARG_RET((NULL == pGFpBuffSize), IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR);

    int ctx_size = 0;
    IppStatus sts = ippsGFpGetSize(primeBitSize, &ctx_size);
    if (sts != ippStsNoErr) { return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR; }
    *pGFpBuffSize = ctx_size + IPPCP_GFP_ALIGNMENT;

    return IPPCP_ALGO_SELFTEST_OK;
}

IPPFUN(fips_test_status, fips_selftest_ippsGFpECSignVerifyDSA_get_size_GFpEC_buff, (int *pGFpECBuffSize, Ipp8u *pGFpBuff)) {
    /* return bad status if input pointers are NULL */
    IPP_BADARG_RET((NULL == pGFpECBuffSize) || (NULL == pGFpBuff), IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR);

    IppStatus sts = ippStsNoErr;

    IppsGFpState* pGF = (IppsGFpState*)(IPP_ALIGNED_PTR(pGFpBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippsGFpInitFixed(primeBitSize, ippsGFpMethod_p256r1(), pGF);
    if(sts != ippStsNoErr) return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;

    /* GFpEC context size */
    int ctx_size = 0;
    sts = ippsGFpECGetSize(pGF, &ctx_size);
    if (sts != ippStsNoErr) { return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR; }

    *pGFpECBuffSize = ctx_size + IPPCP_GFP_ALIGNMENT;

    return IPPCP_ALGO_SELFTEST_OK;
}

IPPFUN(fips_test_status, fips_selftest_ippsGFpECSignVerifyDSA_get_size_data_buff, (int *pDataBuffSize, Ipp8u *pGFpBuff, Ipp8u *pGFpECBuff))
{
    /* return bad status if input pointers are NULL */
    IPP_BADARG_RET((NULL == pDataBuffSize) || (NULL == pGFpBuff) || (NULL == pGFpECBuff), IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR);

    IppStatus sts = ippStsNoErr;
    int total_size = 0;
    int tmp_size = 0;

    IppsGFpState* pGF = (IppsGFpState*)pGFpBuff;
    sts = ippsGFpInitFixed(primeBitSize, ippsGFpMethod_p256r1(), pGF);
    if(sts != ippStsNoErr) return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;

    /* GFpEC context size */
    IppsGFpECState* pEC = (IppsGFpECState*)pGFpECBuff;
    sts = ippsGFpECInitStd256r1(pGF, pEC);
    if(sts != ippStsNoErr) return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;

    /* signature */
    sts = ippsBigNumGetSize(ordWordSize, &tmp_size);
    if(sts != ippStsNoErr) return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    total_size += (2*(tmp_size + IPPCP_GFP_ALIGNMENT)); // bnR and bnS

    /* message */
    sts = ippsBigNumGetSize(msgWordSize, &tmp_size);
    if(sts != ippStsNoErr) return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    total_size += (tmp_size+IPPCP_GFP_ALIGNMENT); 

    /* Reg and eph keys */
    sts = ippsBigNumGetSize(primeWordSize, &tmp_size);
    if(sts != ippStsNoErr) return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    total_size += (2*(tmp_size + IPPCP_GFP_ALIGNMENT)); // bnRegPrivate and bnEphPrivate

    /* Public key x and y coordinates */
    ippsGFpElementGetSize(pGF, &tmp_size);
    if(sts != ippStsNoErr) return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    total_size += (2*(tmp_size + IPPCP_GFP_ALIGNMENT)); // pubGxCtxByteSize and pubGyCtxByteSize

    /* Public key */
    ippsGFpECPointGetSize(pEC, &tmp_size);
    if(sts != ippStsNoErr) return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    total_size += (tmp_size+IPPCP_GFP_ALIGNMENT);
    /* Scratch buffer */
    ippsGFpECScratchBufferSize(2, pEC, &tmp_size);
    total_size += (tmp_size+IPPCP_GFP_ALIGNMENT); 

    *pDataBuffSize = total_size;

    return IPPCP_ALGO_SELFTEST_OK;
}

IPPFUN(fips_test_status, fips_selftest_ippsGFpECSignDSA, (Ipp8u *pGFpBuff, Ipp8u *pGFpECBuff, Ipp8u *pDataBuff))
{
    IppStatus sts = ippStsNoErr;
    fips_test_status test_result = IPPCP_ALGO_SELFTEST_OK;
    
    /* Internal memory allocation feature */
    int internalMemMgm = 0;
#if IPPCP_SELFTEST_USE_MALLOC
    if(pGFpBuff == NULL || pGFpECBuff == NULL || pDataBuff == NULL) {
        internalMemMgm = 1;

        int gfpBuffSize = 0;
        fips_selftest_ippsGFpECSignVerifyDSA_get_size_GFp_buff(&gfpBuffSize);
        pGFpBuff = malloc((size_t)gfpBuffSize);

        int gfpECBuffSize = 0;
        fips_selftest_ippsGFpECSignVerifyDSA_get_size_GFpEC_buff(&gfpECBuffSize, pGFpBuff);
        pGFpECBuff = malloc((size_t)gfpECBuffSize);

        int dataBuffSize = 0;
        fips_selftest_ippsGFpECSignVerifyDSA_get_size_data_buff(&dataBuffSize, pGFpBuff, pGFpECBuff);
        pDataBuff = malloc((size_t)dataBuffSize);
    }
#else
    IPP_BADARG_RET((NULL == pGFpBuff) || (NULL == pGFpECBuff) || (NULL == pDataBuff), IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR);
#endif

    /* Init GFp context */
    IppsGFpState* pGF = (IppsGFpState*)(IPP_ALIGNED_PTR(pGFpBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippsGFpInitFixed(primeBitSize, ippsGFpMethod_p256r1(), pGF);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    /* Init GFpEC context */
    IppsGFpECState* pEC = (IppsGFpECState*)(IPP_ALIGNED_PTR(pGFpECBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippsGFpECInitStd256r1(pGF, pEC);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }

    Ipp8u* pLocDataBuff = pDataBuff;

    /* signature */
    int sByteSize;
    sts = ippsBigNumGetSize(ordWordSize, &sByteSize);
    IppsBigNumState* bnS = (IppsBigNumState*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippcp_init_set_bn(bnS, ordWordSize, ippBigNumPOS, (const Ipp32u *)msg_digest, ordWordSize);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff += sByteSize;

    int rByteSize;
    sts = ippsBigNumGetSize(ordWordSize, &rByteSize);
    IppsBigNumState* bnR = (IppsBigNumState*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippcp_init_set_bn(bnR, ordWordSize, ippBigNumPOS, (const Ipp32u *)msg_digest, ordWordSize);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff += rByteSize;

    /* message */
    int msgCtxByteSize;
    sts = ippsBigNumGetSize(msgWordSize, &msgCtxByteSize);
    IppsBigNumState* bnMsgDigest = (IppsBigNumState*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippcp_init_set_bn(bnMsgDigest, msgWordSize, ippBigNumPOS, (const Ipp32u *)msg_digest, msgWordSize);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff += msgCtxByteSize;    

    /* Reg and eph keys */
    int regKeyCtxByteSize;
    sts = ippsBigNumGetSize(primeWordSize, &regKeyCtxByteSize);
    IppsBigNumState* bnRegPrivate = (IppsBigNumState*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippcp_init_set_bn(bnRegPrivate, primeWordSize, ippBigNumPOS, (const Ipp32u *)d, primeWordSize);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff += regKeyCtxByteSize;    

    int ephKeyCtxByteSize;
    sts = ippsBigNumGetSize(primeWordSize, &ephKeyCtxByteSize);
    IppsBigNumState* bnEphPrivate = (IppsBigNumState*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippcp_init_set_bn(bnEphPrivate, primeWordSize, ippBigNumPOS, (const Ipp32u *)k, primeWordSize);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff += ephKeyCtxByteSize;    

    int srcatchSize;
    ippsGFpECScratchBufferSize(2, pEC, &srcatchSize);
    Ipp8u* pScratchBuffer = pLocDataBuff;

    /* RSA Signature Generation */
    sts = ippsGFpECSignDSA(bnMsgDigest, bnRegPrivate, bnEphPrivate,
                           bnR, bnS, pEC, pScratchBuffer);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }

    Ipp8u pOutR[32];
    Ipp8u pOutS[32];
    int lenR, lenS;

    IppsBigNumSGN sgn;
    ippsGet_BN(&sgn, &lenR, (Ipp32u*)pOutR, bnR);
    ippsGet_BN(&sgn, &lenS, (Ipp32u*)pOutS, bnS);

    int sigFlagErr;
    sigFlagErr = ippcp_is_mem_eq(r, sizeof(r), pOutR, sizeof(r));
    sigFlagErr &= ippcp_is_mem_eq(s, sizeof(s), pOutS, sizeof(s));

    if(1 != sigFlagErr) {
        test_result = IPPCP_ALGO_SELFTEST_KAT_ERR;
    }

    MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
    return test_result;
}

IPPFUN(fips_test_status, fips_selftest_ippsGFpECVerifyDSA, (Ipp8u *pGFpBuff, Ipp8u *pGFpECBuff, Ipp8u *pDataBuff))
{
    IppStatus sts = ippStsNoErr;
    fips_test_status test_result = IPPCP_ALGO_SELFTEST_OK;

    /* Internal memory allocation feature */
    int internalMemMgm = 0;
#if IPPCP_SELFTEST_USE_MALLOC
    if(pGFpBuff == NULL || pGFpECBuff == NULL || pDataBuff == NULL) {
        internalMemMgm = 1;

        int gfpBuffSize = 0;
        fips_selftest_ippsGFpECSignVerifyDSA_get_size_GFp_buff(&gfpBuffSize);
        pGFpBuff = malloc((size_t)gfpBuffSize);

        int gfpECBuffSize = 0;
        fips_selftest_ippsGFpECSignVerifyDSA_get_size_GFpEC_buff(&gfpECBuffSize, pGFpBuff);
        pGFpECBuff = malloc((size_t)gfpECBuffSize);

        int dataBuffSize = 0;
        fips_selftest_ippsGFpECSignVerifyDSA_get_size_data_buff(&dataBuffSize, pGFpBuff, pGFpECBuff);
        pDataBuff = malloc((size_t)dataBuffSize);
    }
#else
    IPP_BADARG_RET((NULL == pGFpBuff) || (NULL == pGFpECBuff) || (NULL == pDataBuff), IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR);
#endif

    /* Init GFp context */
    IppsGFpState* pGF = (IppsGFpState*)(IPP_ALIGNED_PTR(pGFpBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippsGFpInitFixed(primeBitSize, ippsGFpMethod_p256r1(), pGF);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }

    /* Init GFpEC context */
    IppsGFpECState* pEC = (IppsGFpECState*)(IPP_ALIGNED_PTR(pGFpECBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippsGFpECInitStd256r1(pGF, pEC);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }

    Ipp8u* pLocDataBuff = pDataBuff;

    /* signature */
    int sByteSize;
    sts = ippsBigNumGetSize(ordWordSize, &sByteSize);
    IppsBigNumState* bnS = (IppsBigNumState*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippcp_init_set_bn(bnS, ordWordSize, ippBigNumPOS, (const Ipp32u *)s, ordWordSize);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff += sByteSize;

    int rByteSize;
    sts = ippsBigNumGetSize(ordWordSize, &rByteSize);
    IppsBigNumState* bnR = (IppsBigNumState*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippcp_init_set_bn(bnR, ordWordSize, ippBigNumPOS, (const Ipp32u *)r, ordWordSize);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff += rByteSize;

    /* message */
    int msgCtxByteSize;
    sts = ippsBigNumGetSize(msgWordSize, &msgCtxByteSize);
    IppsBigNumState* bnMsgDigest = (IppsBigNumState*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippcp_init_set_bn(bnMsgDigest, msgWordSize, ippBigNumPOS, (const Ipp32u *)msg_digest, msgWordSize);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff += msgCtxByteSize;    

    /* Public key x and y coordinates */
    int pubGxCtxByteSize;
    ippsGFpElementGetSize(pGF, &pubGxCtxByteSize);
    IppsGFpElement* pubGx = (IppsGFpElement*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippsGFpElementInit((const Ipp32u *)qx, primeWordSize, pubGx, pGF);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff += pubGxCtxByteSize;

    int pubGyCtxByteSize;
    ippsGFpElementGetSize(pGF, &pubGyCtxByteSize);
    IppsGFpElement* pubGy = (IppsGFpElement*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippsGFpElementInit((const Ipp32u *)qy, primeWordSize, pubGy, pGF);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff += pubGyCtxByteSize;

    /* Public key */
    int pubKeyCtxSize;
    ippsGFpECPointGetSize(pEC, &pubKeyCtxSize);
    IppsGFpECPoint* regPublic = (IppsGFpECPoint*)(IPP_ALIGNED_PTR(pLocDataBuff, IPPCP_GFP_ALIGNMENT));
    sts = ippsGFpECPointInit(pubGx, pubGy, regPublic, pEC);
    if(sts != ippStsNoErr) {
        MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
        return IPPCP_ALGO_SELFTEST_BAD_ARGS_ERR;
    }
    pLocDataBuff+=pubKeyCtxSize;
    
    int srcatchSize; // single point multiplication
    ippsGFpECScratchBufferSize(2, pEC, &srcatchSize);
    Ipp8u* pScratchBuffer = pLocDataBuff;

    /* RSA Signature Generation */
    IppECResult verifRes;
    sts = ippsGFpECVerifyDSA(bnMsgDigest, regPublic, bnR,
                             bnS, &verifRes, pEC, pScratchBuffer);

    if( ippECValid != verifRes || ippStsNoErr != sts ) {
        test_result = IPPCP_ALGO_SELFTEST_KAT_ERR;
    }

    MEMORY_FREE_3(pGFpBuff, pGFpECBuff, pDataBuff, memMgmFlag)
    return test_result;
}

#endif // _IPP_DATA
#endif // IPPCP_FIPS_MODE
