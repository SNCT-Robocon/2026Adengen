#include "cpp_main.hpp"
#include "canclassic_it.hpp"
#include "canfd_it.hpp"
#include "stm32g4xx_hal.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>



void default_fn(void){
    Code::cpp_main();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim == Code::hard_timer_1kHz.get_handle()){
        Code::hard_timer_1kHz.handle_callback();
    }
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs){
	if(hfdcan == Code::can1.get_handle()){
        Code::can1.rx_it_handler();
	}
}

void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes){
	if(hfdcan == Code::can1.get_handle()){
        Code::can1.tx_it_handler();
	}
}

void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs) {
    if(hfdcan == Code::can1.get_handle()){
        if ((ErrorStatusITs & FDCAN_IT_BUS_OFF) != RESET){
            Code::can1.clear_bus_off();
        }
    }
}








namespace Code{

    void cpp_main(){

        // HAL_Delay(1000);

        led[0].set_state(true);

        printf("init started\r\n");

        can1.start();

        enable.set_state(true);
        led[1].set_state(true);
        discharge.set_state(false);

        hard_timer_1kHz.set_callback(timer_interruption_1kHz);
        hard_timer_1kHz.start();

        printf("init finished\r\n");


        while(true){

            led[0].change_state();
            HAL_Delay(200);

        }        
    }

    void timer_interruption_1kHz(){

        if(relay_on.get_state()){
            relay_off_time_ms = 0;
        }
        else{
            relay_off_time_ms += 1;
        }

        if(100 < relay_off_time_ms && relay_off_time_ms < 2000){
            discharge.set_state(true);
        }
        else{
            discharge.set_state(false);
        }

        while(0 < can1.get_rx_busy_level()){
            
            stmlib_v2::canfd_packet rxpacket{};
            can1.rx(rxpacket);

            can_rx_counter += 1;

            if(rxpacket.id == 0x160){
                int32_t flag;
                memcpy(&flag, &rxpacket.data[0], 4);

                if(flag == 1){
                    enable.set_state(true);
                    led[1].set_state(true);
                }
                else{
                    enable.set_state(false);
                    led[1].set_state(false);
                }

            }

        }


    }


}


// #include "cpp_main.hpp"
// #include "canclassic_it.hpp"
// #include "canfd_it.hpp"
// #include "stm32g4xx_hal.h"
// #include <cstddef>
// #include <cstdint>
// #include <cstdio>
// #include <cstring>



// void default_fn(void){
//     Code::cpp_main();
// }

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//     if(htim == Code::hard_timer_1kHz.get_handle()){
//         Code::hard_timer_1kHz.handle_callback();
//     }
// }

// void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs){
// 	if(hfdcan == Code::can1.get_handle()){
//         Code::can1.rx_it_handler();
// 	}
// }

// void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes){
// 	if(hfdcan == Code::can1.get_handle()){
//         Code::can1.tx_it_handler();
// 	}
// }

// void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs) {
//     if(hfdcan == Code::can1.get_handle()){
//         if ((ErrorStatusITs & FDCAN_IT_BUS_OFF) != RESET){
//             Code::can1.clear_bus_off();
//         }
//     }
// }








// namespace Code{

//     void cpp_main(){

//         led[0].set_state(true);

//         printf("init started\r\n");

//         can1.start();

//         enable.set_state(true);
//         led[1].set_state(true);
//         discharge.set_state(false);

//         hard_timer_1kHz.set_callback(timer_interruption_1kHz);
//         hard_timer_1kHz.start();

//         printf("init finished\r\n");


//         while(true){

//             led[0].change_state();
//             HAL_Delay(200);

//         }        
//     }

//     void timer_interruption_1kHz(){

//         if(relay_on.get_state()){
//             relay_off_time_ms = 0;
//         }
//         else{
//             relay_off_time_ms += 1;
//         }

//         if(100 < relay_off_time_ms && relay_off_time_ms < 2000){
//             discharge.set_state(true);
//         }
//         else{
//             discharge.set_state(false);
//         }

//         while(0 < can1.get_rx_busy_level()){
            
//             stmlib_v2::canfd_packet rxpacket{};
//             can1.rx(rxpacket);

//             can_rx_counter += 1;

//             if(rxpacket.id == 0x160){
//                 int32_t flag;
//                 memcpy(&flag, &rxpacket.data[0], 4);

//                 if(flag == 1){
//                     enable.set_state(true);
//                     led[1].set_state(true);
//                 }
//                 else{
//                     enable.set_state(false);
//                     led[1].set_state(false);
//                 }

//             }

//         }


//     }


// }