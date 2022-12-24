#include "uds_2e.h"
#include "hub75.h"
#include <string.h>

static void UDS_0x2E_WriteToRGB111Buffer0(uint8_t* data_pa, uint16_t count_u16, UDS_Response_Codes_e* errorCode);

void UDS_0x2E_Execute(UDS_0x2E_Identidiers identifier, uint8_t* data_pa, uint16_t count_u16, UDS_Response_Codes_e* errorCode){
    switch(identifier){
        case Write_To_Memory_RGB111:{
            UDS_0x2E_WriteToRGB111Buffer0(data_pa, count_u16, errorCode);
            break;
        }
        default:{
            *errorCode = UDS_Request_Out_Of_Range;
            break;
        }
    }
}

/* 
 * Copy data from UDS directly to the matrix buffer0
 ToDo: Check length of the data transfered, to check if data will fit the buffer
 ToDo: Transfer offset and other calculations to here
*/
static void UDS_0x2E_WriteToRGB111Buffer0(uint8_t* data_pa, uint16_t count_u16, UDS_Response_Codes_e* errorCode){
    uint16_t block_len = (uint16_t)((uint16_t)(data_pa[USD_2E_BLOCKLEN_POS_H] << 8) | (uint16_t)data_pa[USD_2E_BLOCKLEN_POS_L]);
    uint16_t block_num = (uint16_t)((uint16_t)(data_pa[USD_2E_BLOCKNUM_POS_H] << 8) | (uint16_t)data_pa[USD_2E_BLOCKNUM_POS_L]);

    if (UDS_2E_DATA_POS > count_u16){
        *errorCode = UDS_General_Error;
        return;
    }

    uint8_t* rgb111_pa = hub75_rgb111_get_buffer();
    count_u16 = count_u16 - UDS_2E_DATA_POS;
    data_pa = &data_pa[UDS_2E_DATA_POS];
    
    memcpy(rgb111_pa, data_pa, count_u16);
    errorCode = UDS_Positive_Response;
}