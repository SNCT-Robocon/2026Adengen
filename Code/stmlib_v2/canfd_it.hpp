#ifndef CANFD_IT_HPP_
#define CANFD_IT_HPP_

#include "main.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include "cstring"
#include "soft_fifo.hpp"
#include "stm32g4xx_hal_fdcan.h"

namespace stmlib_v2{

struct canfd_packet{
    uint32_t id;
    uint8_t data[32];
};

class canfd_comm_it{

private:
    FDCAN_HandleTypeDef* fdcan;
    SoftFifo<canfd_packet, 6> tx_soft_fifo;
    SoftFifo<canfd_packet, 6> rx_soft_fifo;

    bool tx(canfd_packet &packet){

        FDCAN_TxHeaderTypeDef tx_header = {0};

        tx_header.BitRateSwitch = FDCAN_BRS_ON;
        tx_header.DataLength = FDCAN_DLC_BYTES_32;
        tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
        tx_header.FDFormat = FDCAN_FD_CAN;
        tx_header.Identifier = packet.id;
        tx_header.IdType = FDCAN_STANDARD_ID;
        tx_header.MessageMarker = 0U;
        tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
        tx_header.TxFrameType = FDCAN_DATA_FRAME;

        return HAL_FDCAN_AddMessageToTxFifoQ(fdcan, &tx_header, packet.data);
    }


public:
    canfd_comm_it(FDCAN_HandleTypeDef* fdcan_ptr): fdcan(fdcan_ptr){
        // nothing to do
    }

    FDCAN_HandleTypeDef* get_handle(){
        return fdcan;
    }

    void clear_bus_off(){
        CLEAR_BIT(fdcan->Instance->CCCR, FDCAN_CCCR_INIT);
    }

    void start(){

        FDCAN_FilterTypeDef filter = {0};
        filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
        filter.FilterID1 = 0x000;
        filter.FilterID2 = 0x000;
        filter.FilterIndex = 0x01;
        filter.FilterType = FDCAN_FILTER_MASK;
        filter.IdType = FDCAN_STANDARD_ID;
        HAL_FDCAN_ConfigFilter(fdcan, &filter);

        filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
        filter.FilterID1 = 0x000;
        filter.FilterID2 = 0x000;
        filter.FilterIndex = 0x02;
        filter.FilterType = FDCAN_FILTER_MASK;
        filter.IdType = FDCAN_EXTENDED_ID;
        HAL_FDCAN_ConfigFilter(fdcan, &filter);

        HAL_FDCAN_Start(fdcan);
        HAL_FDCAN_ActivateNotification(fdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0U);
        HAL_FDCAN_ActivateNotification(fdcan, FDCAN_IT_TX_COMPLETE, FDCAN_TX_BUFFER0 | FDCAN_TX_BUFFER1 | FDCAN_TX_BUFFER2);
        HAL_FDCAN_ActivateNotification(fdcan, FDCAN_IT_BUS_OFF, 0U);

    }

    void write(canfd_packet &packet){

        if(tx(packet) == HAL_ERROR){
            tx_soft_fifo.input(packet);
        }
        
    }

    void tx_it_handler(){
        if(HAL_FDCAN_GetTxFifoFreeLevel(fdcan) > 0 && tx_soft_fifo.get_busy_level() > 0){
            canfd_packet packet;
            tx_soft_fifo.output(packet);
            tx(packet);
        }
    }

    void rx_it_handler(){
        FDCAN_RxHeaderTypeDef rx_header = {0};
        uint8_t buf[64] = {0};
        HAL_FDCAN_GetRxMessage(fdcan, FDCAN_RX_FIFO0, &rx_header, buf);

        canfd_packet packet;
        packet.id = rx_header.Identifier;
        memcpy(packet.data, buf, 32);
        rx_soft_fifo.input(packet);
    }

    size_t get_rx_busy_level(){
        return rx_soft_fifo.get_busy_level();
    }

    bool rx(canfd_packet &packet){
        return rx_soft_fifo.output(packet);
    }


};



}

#endif /* CANFD_IT_HPP_ */