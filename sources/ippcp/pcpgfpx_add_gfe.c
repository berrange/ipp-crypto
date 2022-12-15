/*******************************************************************************
* Copyright (C) 2010 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the 'License');
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
* http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an 'AS IS' BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions
* and limitations under the License.
* 
*******************************************************************************/

/*
//     Intel(R) Integrated Performance Primitives. Cryptography Primitives.
//     Internal operations over GF(p) extension.
// 
//     Context:
//        cpGFpxAdd_GFE()
//
*/

#include "owncp.h"
#include "pcpbnumisc.h"
#include "pcpgfpxstuff.h"
#include "gsscramble.h"

IPP_OWN_DEFN (BNU_CHUNK_T*, cpGFpxAdd_GFE, (BNU_CHUNK_T* pR, const BNU_CHUNK_T* pA, const BNU_CHUNK_T* pGroundB, gsModEngine* pGFEx))
{
   gsModEngine* pGroundGFE = GFP_PARENT(pGFEx);
   mod_add addF = MOD_METHOD(pGroundGFE)->add;

   if(pR != pA) {
      int groundElemLen = GFP_FELEN(pGroundGFE);
      int deg = GFP_EXTDEGREE(pGFEx);
      cpGFpElementCopy(pR+groundElemLen, pA+groundElemLen, groundElemLen*(deg-1));
   }
   return addF(pR, pA, pGroundB, pGroundGFE);
}