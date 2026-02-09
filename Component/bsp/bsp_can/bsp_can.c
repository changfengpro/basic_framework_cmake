#include "bsp_can.h"



void can_user_init(CAN_HandleTypeDef hcan)
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
    HAL_CAN_ConfigFilter(&hcan, &can_filter);      // init can filter
    HAL_CAN_Start(&hcan);                          // start can1
}