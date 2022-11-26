#pragma once

#include "uds_types.h"

#define UDS_2E_IDENT_POS    1U
#define UDS_2E_DATA_POS     3U
#define UDS_2E_ERROR_POS    3U

typedef enum _UDS_0x2E_Identidiers{
    Write_To_Memory_RGB111 = 0x0000U,
}UDS_0x2E_Identidiers;

UDS_State_e UDS_0x2E_Execute(UDS_0x2E_Identidiers identifier, uint8_t* data_pa, uint32_t count_u32, UDS_Response_Codes_e* response);
uint32_t UDS_0x2E_Response(UDS_0x2E_Identidiers identifier, UDS_Response_Codes_e response, uint8_t* response_pa);