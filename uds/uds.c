#include "uds.h"

#include "uart.h"

static uint8_t buffer_a[4096];
static uint32_t buffer_len_u32 = 0;

void UDS_Manage(void){
    if (UART_RX_State_Get() == TX_RX_Ready){
        uint8_t* mes_pa = UART_ReceiveBuffer_Get();
        uint32_t len_u32 = UART_ReceiveBufferLen_Get();

        UDS_Process(mes_pa, len_u32);
        while(UART_TX_State_Get() != TX_RX_Ready);
        UART_transferAsync(buffer_a, buffer_len_u32);
        UART_readAsync();
    }
}

UDS_State_e UDS_Process(uint8_t* mes_pa, uint32_t len){
    UDS_State_e state = UDS_State_OK;
    UDS_Response_Codes_e response;

    /* Check and execute SID */
    UDS_SID_e sid = (UDS_SID_e)mes_pa[UDS_SID_POS];
    state = UDS_SID_Execute(sid, mes_pa, len, &response);

    return state;
}

UDS_State_e UDS_SID_Execute(UDS_SID_e sid, uint8_t* mes_pa, uint32_t len_u32, UDS_Response_Codes_e* errorCode){
    UDS_State_e state = UDS_State_OK;
    
    switch(sid){
        case Write_Data_By_Identifier:{
            uint32_t count_u32 = len_u32 - UDS_2E_DATA_POS;
            UDS_0x2E_Identidiers identifier = (UDS_0x2E_Identidiers)((uint16_t)(mes_pa[UDS_2E_IDENT_POS] << 8) | mes_pa[UDS_2E_IDENT_POS + 1]);
            uint8_t* data_pa = &mes_pa[UDS_2E_DATA_POS];
            state = UDS_0x2E_Execute(identifier, data_pa, count_u32, errorCode);
            buffer_len_u32 = UDS_0x2E_Response(identifier, *errorCode, buffer_a);
            break;
        }
        default:{
            *errorCode = Service_Not_Supported;
            state = UDS_State_Error;
            buffer_a[UDS_SID_POS] = sid;
            buffer_a[UDS_ERROR_POS] = *errorCode;
            buffer_len_u32 = 2;
            break;
        }
    }

    return state;
}
