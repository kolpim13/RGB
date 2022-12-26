#pragma once

#include "pico/stdlib.h"

#define UDS_SID_POS                     0U
#define UDS_NEGATIVE_RESPONSE_SID_POS   0U
#define UDS_REJECTED_SID_POS            1U
#define UDS_NEGATIVE_RESPONSE_CODE_POS  2U

#define UDS_NEGATIVE_RESPONSE_SID       0x7FU

typedef enum _UDS_SID_e{
    Read_Data_By_Identifier = 0x22,
    Write_Data_By_Identifier = 0x2E,
}UDS_SID_e;

typedef enum _UDS_Response_Codes_e{
    UDS_Positive_Response = 0x00,
    UDS_Service_Not_Supported = 0x11,
    UDS_Request_Out_Of_Range = 0x31,
    UDS_General_Error = 0x7F,
}UDS_Response_Codes_e;
