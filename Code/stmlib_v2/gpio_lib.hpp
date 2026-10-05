#ifndef LED_CONTROL_HPP_
#define LED_CONTROL_HPP_

#include "main.h"

namespace stmlib_v2{

class gpio_output{
private:
    GPIO_TypeDef *gpio_port;
    const uint16_t gpio_pin;

public:
    gpio_output(GPIO_TypeDef *_gpio_port, const uint16_t _gpio_pin)
        : gpio_port(_gpio_port), gpio_pin(_gpio_pin){
    }

    void set_state(bool state){
        if (state){
            HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_SET);
        }
        else{
            HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_RESET);
        }
    }

    void change_state(){
        HAL_GPIO_TogglePin(gpio_port, gpio_pin);
    }

};



class gpio_input{
private:
    GPIO_TypeDef* gpio_port;
    const uint16_t gpio_pin;

public:
    gpio_input(GPIO_TypeDef* _gpio_port, const uint16_t _gpio_pin)
        : gpio_port(_gpio_port), gpio_pin(_gpio_pin){
        // nothing to do
    }
    
    bool get_state(){
        return HAL_GPIO_ReadPin(gpio_port, gpio_pin);
    }
    
};


}

#endif /* LED_CONTROL_HPP_ */