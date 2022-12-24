/* UDS module general description

 TODO:
 1. Add one empty byte to the request right after identifier. (About SID 0x2E )
 Main purpose of it to do start of the data aligned by word-size (4 bytes).
 In addiction it can contain usful information or commands.
 2. [0x2E] [0x0000U]. Add check about size of written data.
 3. Replace variables names of UDS_Response_Codes_e type from "response" to something else 
*/

#pragma once

#include "uart.h"
#include "uds_types.h"
#include "uds_2e.h"

#define UDS_BUF_IN_OUT_LEN      2048 

typedef struct{
    uint32_t in_b : 1;
    uint32_t out_b : 1;
    uint32_t busy : 1;
    uint32_t busy_image : 1;
}UDS_State;

extern UDS_State UDS_state;

uint8_t* uds_in_get(void);
uint8_t* uds_out_get(void);

uint16_t uds_inLen_get(void);
void uds_inLen_set(uint16_t value);

uint16_t uds_outLen_get(void);
void uds_outLen_set(uint16_t value);

void UDS_Manage(void);