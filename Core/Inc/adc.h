#ifndef __ADC_H__
#define __ADC_H__

#include "main.h"
#include <stdint.h>

void ADC_Calibrate(void);
uint32_t ADC_ConvertToDistance(uint32_t adcValue);
uint32_t ADC_Filter(uint32_t newSample);



#endif 
