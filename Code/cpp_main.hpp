#ifndef CPP_MAIN_HPP_
#define CPP_MAIN_HPP_

#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "fdcan.h"

#include "pid_control.hpp"
#include "gpio_lib.hpp"
#include "timer_it.hpp"
#include "canfd_it.hpp"
#include "canclassic_it.hpp"

#include <cmath>
#include <cstdint>
#include <stdio.h>



namespace Code
{
    void cpp_main();
    
    stmlib_v2::canfd_comm_it can1(&hfdcan1);

    stmlib_v2::HardTimer hard_timer_1kHz(&htim6);

    stmlib_v2::gpio_output led[2] = {
        stmlib_v2::gpio_output(LED0_GPIO_Port, LED0_Pin),
        stmlib_v2::gpio_output(LED1_GPIO_Port, LED1_Pin)
    };

    stmlib_v2::gpio_input relay_on(STOP_GPIO_Port, STOP_Pin);
    stmlib_v2::gpio_output discharge(DISCHARGE_GPIO_Port, DISCHARGE_Pin);
    stmlib_v2::gpio_output enable(ENABLE_GPIO_Port, ENABLE_Pin);

    uint32_t relay_off_time_ms = 0;

    void timer_interruption_1kHz();

    int can_rx_counter = 0;

}



#endif /* CPP_MAIN_HPP_ */