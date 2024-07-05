/*************************************************************************
* Copyright (C) 2020 Intel Corporation
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

#include <crypto_mb/status.h>
#include <crypto_mb/sm3.h>

#include <internal/sm3/sm3_mb16.h>
#include <internal/common/ifma_defs.h>

DLL_PUBLIC
mbx_status16 OWNAPI(mbx_sm3_msg_digest_mb16)(const int8u* const msg_pa[16],
                                              int len[16],
                                           int8u* hash_pa[16])
{
    mbx_status16 status = 0;

    /* test input pointers */
    if(NULL==msg_pa || NULL==len || NULL==hash_pa) {
        status = MBX_SET_STS16_ALL(MBX_STATUS_NULL_PARAM_ERR);
        return status;
    }

#if (_MBX>=_MBX_K1)
    status |= internal_avx512_sm3_msg_digest_mb16(msg_pa, len, hash_pa);
#else
    status = MBX_SET_STS16_ALL(MBX_STATUS_UNSUPPORTED_ISA_ERR);
#endif /* #if (_MBX>=_MBX_K1) */
    return status;
}
