#pragma once

#include "uds_types.h"

#define UDS_2E_IDENT_POS_H      1U
#define UDS_2E_IDENT_POS_L      2U
#define USD_2E_BLOCKLEN_POS_H   3U
#define USD_2E_BLOCKLEN_POS_L   4U
#define USD_2E_BLOCKNUM_POS_H   5U
#define USD_2E_BLOCKNUM_POS_L   6U
#define UDS_2E_DATA_POS         7U
#define UDS_2E_ERROR_POS        3U

typedef enum _UDS_0x2E_Identidiers{
    Write_To_Memory_RGB111 = 0x0000U,
}UDS_0x2E_Identidiers;

void UDS_0x2E_Execute(UDS_0x2E_Identidiers identifier, uint8_t* data_pa, uint16_t count_u16, UDS_Response_Codes_e* errorCode);