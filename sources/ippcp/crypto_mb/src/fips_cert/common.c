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
#ifdef MBX_FIPS_MODE

#include <internal/fips_cert/common.h>

#ifdef OPENSSL_IS_BORINGSSL

BIGNUM *BN_lebin2bn(const unsigned char *s, int len, BIGNUM *ret)
{
    return BN_le2bn(s, len, ret);
}

#endif

int mbx_is_mem_eq(const int8u *p1, int32u p1_byte_len, const int8u *p2, int32u p2_byte_len)
{
  if ((p1_byte_len != p2_byte_len) || (p1 == NULL) || (p2 == NULL)) {
    return 0;
  }

  while (p1_byte_len) {
    if (*p1 != *p2) {
      return 0;
    }
    ++p1;
    ++p2;

    --p1_byte_len;
  }

  return 1;
}

#endif // MBX_FIPS_MODE
