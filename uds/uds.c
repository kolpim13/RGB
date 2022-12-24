#include "uds.h"

#include "uart.h"
#include "tusb.h"

static uint8_t bufferIn_a[UDS_BUF_IN_OUT_LEN];
static uint8_t bufferOut_a[UDS_BUF_IN_OUT_LEN];
static volatile uint16_t bufferInLen_u16 = 0;
static volatile uint16_t bufferOutLen_u16 = 0;

static uint32_t image_startAddres = 0;
static uint16_t image_blocksLeft = 0;
static const uint16_t image_blockSize = 512u;

UDS_State UDS_state = { 0 };

static void UDS_SID_Execute(UDS_SID_e sid, uint8_t* data_pa, uint16_t len_u16, UDS_Response_Codes_e* errorCode);

__attribute__ ((always_inline)) inline uint8_t* uds_in_get(void){ return &bufferIn_a[0]; }
__attribute__ ((always_inline)) inline uint8_t* uds_out_get(void){ return &bufferOut_a[0]; }
__attribute__ ((always_inline)) inline uint16_t uds_inLen_get(void){ return bufferInLen_u16; }
__attribute__ ((always_inline)) inline void uds_inLen_set(uint16_t value){ bufferInLen_u16 = value; }
__attribute__ ((always_inline)) inline uint16_t uds_outLen_get(void){ return bufferOutLen_u16; }
__attribute__ ((always_inline)) inline void uds_outLen_set(uint16_t value){ bufferOutLen_u16 = value; }

void UDS_Manage(void){
    /* If there is nothing to process, then just leave. */
    if (UDS_state.in_b == false){
        return;
    }

    /* Needed variables */
    UDS_Response_Codes_e response;

    /* Process given mes */
    UDS_SID_e sid = (UDS_SID_e)bufferIn_a[UDS_SID_POS];
    UDS_SID_Execute(sid, bufferIn_a, bufferInLen_u16, &response);

    UDS_state.in_b = false;
    UDS_state.out_b = true;
}

static void UDS_SID_Execute(UDS_SID_e sid, uint8_t* data_pa, uint16_t len_u16, UDS_Response_Codes_e* errorCode){
    switch(sid){
        case Write_Data_By_Identifier:{
            UDS_0x2E_Identidiers identifier = (UDS_0x2E_Identidiers)((data_pa[UDS_2E_IDENT_POS_H] << 8) | data_pa[UDS_2E_IDENT_POS_L]);
            UDS_0x2E_Execute(identifier, data_pa, len_u16, errorCode);
            break;
        }
        default:{
            *errorCode = UDS_Service_Not_Supported;
            break;
        }
    }
}
