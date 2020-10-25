#ifndef __SERVO_H__
#define __SERVO_H__

#include <stm32f1xx_hal.h>

class Servo{
public:
    Servo();
    

    void Init(TIM_TypeDef* pTimer, uint16_t pTimerChannel, GPIO_TypeDef* pOutGpio, uint16_t pOutPin);

    void setOutMs(float ms);

private:

    void InitTimer(TIM_TypeDef* pTimer, uint16_t pTimerChannel);
    void InitGpio(GPIO_TypeDef* pOutGpio, uint16_t pOutPin);

    TIM_HandleTypeDef timServo;
};

#endif //__SERVO_H__