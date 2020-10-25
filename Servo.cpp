
#include "Servo.h"

#include <stm32f1xx_hal.h>

Servo::Servo(){

}

void Servo::Init(TIM_TypeDef* pTimer, uint16_t pTimerChannel, GPIO_TypeDef* pOutGpio, uint16_t pOutPin){

    InitTimer(pTimer, pTimerChannel);
    
}


void Servo::InitTimer(TIM_TypeDef* pTimer, uint16_t pTimerChannel){



    // Configure timer 2, ch1, remap 00
    timServo.Instance = TIM1;
    timServo.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
    timServo.Init.Prescaler = 1;
    timServo.Init.Period = 150000; 
    timServo.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    timServo.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    timServo.Init.RepetitionCounter = 0;
    timServo.Init.CounterMode = TIM_COUNTERMODE_UP;
    //HAL_TIM_Base_Init(&timServo);

    TIM_ClockConfigTypeDef clockCfgStepUp = {0};
    clockCfgStepUp.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&timServo, &clockCfgStepUp);

    HAL_TIM_PWM_Init(&timServo);

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1500;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&timServo, &sConfigOC, TIM_CHANNEL_1);
    //__HAL_TIM_SET_COMPARE(&timServo,TIM_CHANNEL_1, 100);

    HAL_TIM_PWM_Start(&timServo, TIM_CHANNEL_1);
}

void Servo::InitGpio(GPIO_TypeDef* pOutGpio, uint16_t pOutPin){

}