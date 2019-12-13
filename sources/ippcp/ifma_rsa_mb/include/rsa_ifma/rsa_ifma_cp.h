/*******************************************************************************
* Copyright 2019 Intel Corporation
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
//  Purpose: MB RSA. Public definitions and declarations
//
*/

#ifdef IFMA_IPPCP_BUILD
#include "owndefs.h"
#endif /* IFMA_IPPCP_BUILD */

#if !defined(IFMA_IPPCP_BUILD) || (_IPP32E>=_IPP32E_K0)
#ifndef IFMA_RSA_CP_H
#define IFMA_RSA_CP_H

#include "rsa_ifma_defs.h"
#include "rsa_ifma_status.h"
#include "ifma_method.h"

EXTERN_C ifma_status ifma_cp_rsa52_public_mb8(const int8u* const from_pa[8],
                                                    int8u* const to_pa[8],
                                              const int64u* const n_pa[8],
                                                    int rsaBitlen,
                                              const ifma_RSA_Method* m,
                                                    int8u* pBuffer);

EXTERN_C ifma_status ifma_cp_rsa52_private_mb8(const int8u* const from_pa[8],
                                                     int8u* const to_pa[8],
                                               const int64u* const d_pa[8],
                                               const int64u* const n_pa[8],
                                                     int rsaBitlen,
                                               const ifma_RSA_Method* m,
                                                     int8u* pBuffer);

EXTERN_C ifma_status ifma_cp_rsa52_private_ctr_mb8(const int8u* const from_pa[8],
                                                         int8u* const to_pa[8],
                                                   const int64u* const p_pa[8],
                                                   const int64u* const q_pa[8],
                                                   const int64u* const dp_pa[8],
                                                   const int64u* const dq_pa[8],
                                                   const int64u* const iq_pa[8],
                                                         int rsaBitlen,
                                                   const ifma_RSA_Method* m,
                                                         int8u* pBuffer);

#endif /* IFMA_RSA_CP_H */
#endif
