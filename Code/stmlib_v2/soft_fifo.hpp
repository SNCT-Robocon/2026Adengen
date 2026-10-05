#ifndef SOFT_FIFO_HPP_
#define SOFT_FIFO_HPP_

#include "main.h"


namespace stmlib_v2{

template<typename T, size_t buf_size_bit>
class SoftFifo{
private:
    const size_t BUF_SIZE = 1 << buf_size_bit;
    const size_t BUF_MASK = BUF_SIZE - 1;
    T buffer[1 << buf_size_bit];
    volatile size_t data_head = 0;
    volatile size_t data_tail = 0;

public:
    bool input(const T &input_data){

        size_t next_head = (data_head + 1) & BUF_MASK;
        if(next_head == data_tail){
            return false;
        }

        buffer[data_head] = input_data;
        data_head = next_head;

        return true;
    }

    bool output(T &output_data){

        if(data_head == data_tail){
            return false;
        }

        output_data = buffer[data_tail];
        data_tail = (data_tail + 1) & BUF_MASK;

        return true;
    }

    size_t get_busy_level(){
        return (data_head - data_tail) & BUF_MASK;
    }

    size_t get_free_level(){
        return BUF_SIZE - get_busy_level() - 1;
    }

    void reset(){
        data_head = 0;
        data_tail = 0;
    }

};

}


#endif /* SOFT_FIFO_HPP_ */