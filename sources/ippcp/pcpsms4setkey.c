/*******************************************************************************
* Copyright 2013-2019 Intel Corporation
* All Rights Reserved.
*
* If this  software was obtained  under the  Intel Simplified  Software License,
* the following terms apply:
*
* The source code,  information  and material  ("Material") contained  herein is
* owned by Intel Corporation or its  suppliers or licensors,  and  title to such
* Material remains with Intel  Corporation or its  suppliers or  licensors.  The
* Material  contains  proprietary  information  of  Intel or  its suppliers  and
* licensors.  The Material is protected by  worldwide copyright  laws and treaty
* provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
* modified, published,  uploaded, posted, transmitted,  distributed or disclosed
* in any way without Intel's prior express written permission.  No license under
* any patent,  copyright or other  intellectual property rights  in the Material
* is granted to  or  conferred  upon  you,  either   expressly,  by implication,
* inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
* property rights must be express and approved by Intel in writing.
*
* Unless otherwise agreed by Intel in writing,  you may not remove or alter this
* notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
* suppliers or licensors in any way.
*
*
* If this  software  was obtained  under the  Apache License,  Version  2.0 (the
* "License"), the following terms apply:
*
* You may  not use this  file except  in compliance  with  the License.  You may
* obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
*
* Unless  required  by   applicable  law  or  agreed  to  in  writing,  software
* distributed under the License  is distributed  on an  "AS IS"  BASIS,  WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
* See the   License  for the   specific  language   governing   permissions  and
* limitations under the License.
*******************************************************************************/

/* 
// 
//  Purpose:
//     Cryptography Primitive.
//     Initialization of SM4S
// 
//  Contents:
//        ippsSMS4SetKey()
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcpsms4.h"
#include "pcptool.h"

/*F*
//    Name: cpSMS4_SetRoundKeys
//
// Purpose: Computes SMS4 round keys for future usage
//          depending on  user's secret key.
//
*F*/
static void cpSMS4_SetRoundKeys(Ipp32u* pRounKey, const Ipp8u* pKey)
{
   __ALIGN16 Ipp32u K[4 + SMS4_ROUND_KEYS_NUM];
   K[0] = SMS4_FK[0]^HSTRING_TO_U32(pKey);
   K[1] = SMS4_FK[1]^HSTRING_TO_U32(pKey+sizeof(Ipp32u));
   K[2] = SMS4_FK[2]^HSTRING_TO_U32(pKey+sizeof(Ipp32u)*2);
   K[3] = SMS4_FK[3]^HSTRING_TO_U32(pKey+sizeof(Ipp32u)*3);

   {
      int nr;
      for(nr=0; nr < SMS4_ROUND_KEYS_NUM; nr++) {
         pRounKey[nr] = K[4+nr] = K[nr] ^ cpExpKeyMix_SMS4(K[nr+1]^K[nr+2]^K[nr+3]^SMS4_CK[nr]);
      }
   }

   /* clear secret data */
   PurgeBlock(K, sizeof(K));
}

/*F*
//    Name: ippsSMS4SetKey
//
// Purpose: Set/reset new secret key for future usage.
//
// Returns:                Reason:
//    ippStsNullPtrErr        pCtx == NULL
//    ippStsLengthErr         keyLen<16
//    ippStsContextMatchErr   !VALID_SMS4_ID(pCtx)
//    ippStsNoErr             no errors
//
// Parameters:
//    pKey        security key
//    keyLen      length of the secret key (in bytes)
//    pCtx        pointer to SMS4 initialized context
//
// Note:
//    if pKey==NULL, then zero value key being setup
//
*F*/

IPPFUN(IppStatus, ippsSMS4SetKey,(const Ipp8u* pKey, int keyLen, IppsSMS4Spec* pCtx))
{
   /* test pointers */
   IPP_BAD_PTR1_RET(pCtx);

   /* use aligned SMS4 context */
   pCtx = (IppsSMS4Spec*)( IPP_ALIGNED_PTR(pCtx, SMS4_ALIGNMENT) );
   /* test the context ID */
   IPP_BADARG_RET(!VALID_SMS4_ID(pCtx), ippStsContextMatchErr);

   /* make sure in legal keyLen */
   IPP_BADARG_RET(keyLen<16, ippStsLengthErr);

   {
      __ALIGN16 Ipp8u defaultKey[MBS_SMS4] = {0};
      const Ipp8u* pSecretKey = pKey? pKey : defaultKey;

      /* set encryption round keys */
      #if (_IPP>=_IPP_P8) || (_IPP32E>=_IPP32E_Y8)
      if(IsFeatureEnabled(ippCPUID_AES))
         cpSMS4_SetRoundKeys_aesni(SMS4_RK(pCtx), pSecretKey);
      else
      #endif
         cpSMS4_SetRoundKeys(SMS4_RK(pCtx), pSecretKey);

      /* set deccryption round keys */
      {
         int n;
         for(n=0; n < SMS4_ROUND_KEYS_NUM; n++) {
            SMS4_DRK(pCtx)[n] = SMS4_RK(pCtx)[SMS4_ROUND_KEYS_NUM - n-1];
         }
      }

      return ippStsNoErr;
   }
}
