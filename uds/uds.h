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

void UDS_Manage(void);
UDS_State_e UDS_Process(uint8_t* mes_pa, uint32_t len);
UDS_State_e UDS_SID_Execute(UDS_SID_e sid, uint8_t* mes_pa, uint32_t len_u32, UDS_Response_Codes_e* response);