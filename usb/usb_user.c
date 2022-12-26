#include "usb_user.h"

#include "tusb.h"
#include "uds.h"

static void cdc_task(void);

void usb_task(void){
    cdc_task();
}

static void cdc_task(void){
    /* Get data from cdc 0. [to UDS module] */
    if (UDS_state.in_b == false
        && tud_cdc_n_available(TUD_UDS))
    {
        uint8_t* buf_ap = uds_in_get();

        uint16_t count = tud_cdc_n_read(TUD_UDS, buf_ap, UDS_BUF_IN_OUT_LEN);
        uds_inLen_set(count);

        UDS_state.in_b = true;
    }

    /* Send data to cdc 0 [from UDS module] */
    if (UDS_state.out_b == true){
        uint8_t* buf_ap = uds_out_get();
        uint16_t len_u16 = uds_outLen_get();

        tud_cdc_n_write(TUD_UDS, buf_ap, len_u16);

        UDS_state.out_b = false;
    }
}