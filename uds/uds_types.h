#pragma once

#include "pico/stdlib.h"

#define UDS_SID_POS    0U
#define UDS_ERROR_POS  1U

typedef enum _UDS_State_e{
    UDS_State_OK,
    UDS_State_WrongSID,
    UDS_State_Error,
}UDS_State_e;

typedef enum _UDS_SID_e{
    Write_Data_By_Identifier = 0x2E,
}UDS_SID_e;

typedef enum _UDS_Response_Codes_e{
    Positive_Response = 0x00,
    Service_Not_Supported = 0x11,
    Request_Out_Of_Range = 0x31,
}UDS_Response_Codes_e;
