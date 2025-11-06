#include "adc.h"
#include "main.h"

extern ADC_HandleTypeDef hadc;

void ADC_Init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc);
}

uint32_t ADC_Filter(uint32_t newSample)
{
    static uint32_t filteredValue = 0;
    filteredValue = (filteredValue * 3 + newSample) / 4;
    return filteredValue;
}


uint32_t ADC_ConvertToDistance(uint32_t adcValue)
{
    // calibration points
    const uint32_t adc_min = 640;     // 0%   -> 0.000 cm
    const uint32_t adc_q1  = 794;     // 25%  -> 0.500 cm
    const uint32_t adc_mid = 1110;    // 50%  -> 1.000 cm
    const uint32_t adc_q3  = 2479;    // 75%  -> 1.500 cm
    const uint32_t adc_max = 4092;    // 100% -> 2.000 cm

    // Output values (scaled)
    const uint32_t OUT_MIN = 0;       // 0.000 cm
    const uint32_t OUT_Q1  = 500;     // 0.500 cm
    const uint32_t OUT_MID = 1000;    // 1.000 cm
    const uint32_t OUT_Q3  = 1500;    // 1.500 cm
    const uint32_t OUT_MAX = 2000;    // 2.000 cm

    if (adcValue <= adc_min) return OUT_MIN;
    if (adcValue >= adc_max) return OUT_MAX;

    // 4-Segment Piecewise Logic ---
    if (adcValue < adc_q1)
    {
        // Segment 1: [645, 734] -> [0, 500]
        return ((adcValue - adc_min) * (OUT_Q1 - OUT_MIN)) / (adc_q1 - adc_min);
    }
    else if (adcValue < adc_mid)
    {
        // Segment 2: [734, 1110] -> [500, 1000]
        return OUT_Q1 + ((adcValue - adc_q1) * (OUT_MID - OUT_Q1)) / (adc_mid - adc_q1);
    }
    else if (adcValue < adc_q3)
    {
        // Segment 3: [1110, 2888] -> [1000, 1500]
        return OUT_MID + ((adcValue - adc_mid) * (OUT_Q3 - OUT_MID)) / (adc_q3 - adc_mid);
    }
    else
    {
        // Segment 4: [2888, 4092] -> [1500, 2000]
        return OUT_Q3 + ((adcValue - adc_q3) * (OUT_MAX - OUT_Q3)) / (adc_max - adc_q3);
    }
}