#include "uds_2e.h"
#include "hub75.h"
#include <string.h>

UDS_State_e UDS_0x2E_Execute(UDS_0x2E_Identidiers identifier, uint8_t* data_pa, uint32_t count_u32, UDS_Response_Codes_e* errorCode){
    UDS_State_e state = UDS_State_OK;
    
    switch(identifier){
        /* 
        TODO: Add check about size of written data and corresponding error code. 
        */
        case Write_To_Memory_RGB111:{
            uint8_t* rgb111_pa = hub75_rgb111_get_buffer();
            memcpy(rgb111_pa, data_pa, count_u32);

            state = UDS_State_OK;
            break;
        }
        default:{
            *errorCode = Request_Out_Of_Range;
            state = UDS_State_Error;
            break;
        }
    }

    if (state == UDS_State_OK){
        *errorCode = Positive_Response;
    }
    return state;
}

uint32_t UDS_0x2E_Response(UDS_0x2E_Identidiers identifier, UDS_Response_Codes_e errorCode, uint8_t* response_pa){
    if (errorCode == Positive_Response){
        response_pa[UDS_SID_POS] = 0x6E;
        response_pa[UDS_2E_IDENT_POS] = (uint8_t)identifier;
        response_pa[UDS_2E_IDENT_POS + 1] = (uint8_t)(identifier >> 8);
        return 3;
    }
    else{
        response_pa[UDS_SID_POS] = 0x7F;
        response_pa[UDS_2E_IDENT_POS] = (uint8_t)identifier;
        response_pa[UDS_2E_IDENT_POS + 1] = (uint8_t)(identifier >> 8);
        response_pa[UDS_2E_ERROR_POS] = errorCode;
        return 4;
    }
}
