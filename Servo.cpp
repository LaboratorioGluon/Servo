
#include "Servo.h"

#include <stm32f1xx_hal.h>

Servo::Servo(){
    mFreq = 0;
}

void Servo::Init(TIM_TypeDef* pTimer, uint16_t pTimerChannel, GPIO_TypeDef* pOutGpio, uint16_t pOutPin){

    InitTimer(pTimer, pTimerChannel);
    InitGpio(pOutGpio, pOutPin);

}


void Servo::setOutMs(float ms){
    __HAL_TIM_SET_COMPARE(&timServo, mConfigChannel, (uint32_t)(ms*1e-3*(mFreq/(32+1))));
}


void Servo::InitTimer(TIM_TypeDef* pTimer, uint16_t pTimerChannel){

    HAL_TIM_ActiveChannel active_channel;
    uint16_t config_channel;
    switch(pTimerChannel){
        case 1:
            active_channel = HAL_TIM_ACTIVE_CHANNEL_1;
            config_channel = TIM_CHANNEL_1;
            break;
        case 2:
            active_channel = HAL_TIM_ACTIVE_CHANNEL_2;
            config_channel = TIM_CHANNEL_2;
            break;
        case 3:
            active_channel = HAL_TIM_ACTIVE_CHANNEL_3;
            config_channel = TIM_CHANNEL_3;
            break;
        case 4:
            active_channel = HAL_TIM_ACTIVE_CHANNEL_4;
            config_channel = TIM_CHANNEL_4;
            break;
        default:
            return;
    }

    // Get clocks


    // Configure timer 1, ch1, remap 00
    timServo.Instance = TIM1;
    timServo.Channel = active_channel;
    timServo.Init.Prescaler = 32;

    uint32_t freq = getClockFreq(pTimer);
    timServo.Init.Period = (uint32_t) (20e-3*(freq/(32+1))); 
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
    sConfigOC.Pulse = 1000;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&timServo, &sConfigOC, config_channel);

    HAL_TIM_PWM_Start(&timServo, config_channel);

    //__HAL_TIM_SET_COMPARE(&timServo,config_channel, 100);


    mConfigChannel = config_channel;
}

void Servo::InitGpio(GPIO_TypeDef* pOutGpio, uint16_t pOutPin){

    GPIO_InitTypeDef gpioPwm;
    gpioPwm.Mode= GPIO_MODE_AF_PP;
    gpioPwm.Pin = pOutPin;
    gpioPwm.Pull = GPIO_NOPULL;
    gpioPwm.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init(pOutGpio, &gpioPwm);
}

uint32_t Servo::getClockFreq(TIM_TypeDef* pTimer){
    uint32_t freq;
    if(pTimer == TIM1){
        freq = HAL_RCC_GetPCLK2Freq();
        if ( (RCC->CFGR & RCC_CFGR_PPRE2) != 0){
            freq *= 2;  
        }
    }else{
        freq = HAL_RCC_GetPCLK1Freq();
        if ( (RCC->CFGR & RCC_CFGR_PPRE1) != 0){
            freq *= 2;  
        }
    }
    mFreq = freq;
    return freq;
}