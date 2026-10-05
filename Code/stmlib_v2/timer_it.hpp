#ifndef TIMER_IT_HPP_
#define TIMER_IT_HPP_

#include "main.h"
#include <functional>


namespace stmlib_v2{

class HardTimer{

private:
    TIM_HandleTypeDef* tim;
    std::function<void(void)> callback;

public:
    HardTimer(TIM_HandleTypeDef* _tim) : tim(_tim) {};

    TIM_HandleTypeDef* get_handle(void){
        return tim;
    }

    void start(void){
        HAL_TIM_Base_Start_IT(tim);
    }

    void stop(void){
        HAL_TIM_Base_Stop_IT(tim);
    }

    void set_callback(std::function<void(void)> _callback){
        callback = _callback;
    }

    void handle_callback(void){
        if(callback){
            callback();
        }
    }
    
};



}

#endif /* TIMER_IT_HPP_ */