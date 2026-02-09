#include "bsp_can.h"

void can_user_init(CAN_HandleTypeDef *hcan)
{
    CAN_FilterTypeDef  can_filter;
    can_filter.FilterBank = 0;                       // filter 0
    can_filter.FilterMode =  CAN_FILTERMODE_IDMASK;  // mask mode
    can_filter.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter.FilterIdHigh = 0;
    can_filter.FilterIdLow  = 0;
    can_filter.FilterMaskIdHigh = 0;
    can_filter.FilterMaskIdLow  = 0;                // set mask 0 to receive all can id
    can_filter.FilterFIFOAssignment = CAN_RX_FIFO0; // assign to fifo0
    can_filter.FilterActivation = ENABLE;           // enable can filter
    can_filter.SlaveStartFilterBank  = 14;          // only meaningful in dual can mode
    HAL_CAN_ConfigFilter(hcan, &can_filter);      // init can filter
    HAL_CAN_Start(hcan);                          // start can1
}

void can_user_send_msg(CAN_HandleTypeDef *hcan, uint32_t id, uint8_t *data, uint8_t len)
{
    CAN_TxHeaderTypeDef tx_header;
    tx_header.StdId = id;                        // standard id
    tx_header.ExtId = 0;                         // not used in standard id mode
    tx_header.IDE = CAN_ID_STD;                 // use standard id
    tx_header.RTR = CAN_RTR_DATA;              // data frame
    tx_header.DLC = len;                       // data length
    uint32_t tx_mailbox;
    HAL_CAN_AddTxMessage(hcan, &tx_header, data, &tx_mailbox); // send can message
}