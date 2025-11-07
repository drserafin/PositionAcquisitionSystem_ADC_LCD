## Goal of the Lab
The main goal of this lab is to design and implement a real-time position measurement system that uses an Analog-to-Digital Converter (ADC) to read the position of a slide potentiometer and display the position on an LCD screen in centimeters.

## Demo Video
[![Demo Video](https://img.youtube.com/vi/5cuOldIjPko/0.jpg)](https://www.youtube.com/watch?v=5cuOldIjPko)

## Walkthrough
First, we set up and initialize the ADC and a general-purpose timer.  
In this repository, we included a `.ioc` file that reflects the configuration settings used to initialize these two peripherals and enable interrupts.  
The system is designed to sample at a **10 Hz** rate.

## main.c
Before we start the ADC, we need to calibrate it by calling:
```c
HAL_ADCEx_Calibration_Start(&hadc);
```
From there, we can begin sampling.
Normally, with STM32 HAL functions, sampling the ADC looks like this:
```c
HAL_ADC_Start(&hadc);
HAL_ADC_PollForConversion(&hadc, 100);
adc_value = HAL_ADC_GetValue(&hadc);
HAL_ADC_Stop(&hadc);
```
However, during debugging, I decided to use a timer interrupt instead of polling to start the ADC conversions.
This allows sampling to occur automatically at a fixed rate (10 Hz) inside the timer’s callback function:
```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        HAL_ADC_Start_IT(&hadc); 
    }
}

```
After the ADC finishes conversion, an ADC interrupt is triggered automatically.
This interrupt calls the following callback function, where we read the converted value and set a flag to indicate new data is ready:
```c
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc_ptr)
{
    ADC_Mailbox = HAL_ADC_GetValue(&hadc);
    ADC_Flag = 1; 
}

```

## 🧩 adc.c — Function Explanations

### `void ADC_Calibrate(void)`
**Meaning:**  
This function prepares the ADC to take accurate measurements.  

**What it does:**  
It runs a **calibration** routine to make sure the ADC’s readings are as precise as possible.  

**In simple terms:**  
Think of it like *“zeroing out” a scale before weighing something*, so your measurements start accurate.

---

### `uint32_t ADC_Filter(uint32_t newSample)`
**Meaning:**  
This function smooths out the raw ADC readings to reduce small fluctuations or noise.  

**What it does:**  
It averages the current reading with previous ones, using a simple weighted filter.  

**In simple terms:**  
It makes the signal more stable — instead of jumping around with every tiny movement, the output changes smoothly.

---

### `uint32_t ADC_ConvertToDistance(uint32_t adcValue)`
**Meaning:**  
This function converts the raw ADC value into a meaningful physical distance (in centimeters).  

**What it does:**  
It uses a set of **calibration points** to translate voltage readings into distance based on how far the slider is moved.  

**In simple terms:**  
It turns the *electrical signal* from the potentiometer into a *real-world measurement* that represents the slider’s position.

---

### 🧠 Example Walkthrough
When I moved the slider all the way **down**, I noticed the raw ADC reading was around **640**,  
not 0. This means the potentiometer doesn’t physically reach the true zero voltage.  

So, I used **640** as my minimum calibration point (`adc_min`).  
From there, I took several measurements along the slider’s range to build realistic calibration points:  

| Slider Position | Approx. ADC Value | Mapped Distance (cm) |
|------------------|------------------|----------------------|
| 0% (bottom)      | 640              | 0.000 cm             |
| 25%              | 794              | 0.500 cm             |
| 50% (middle)     | 1110             | 1.000 cm             |
| 75%              | 2479             | 1.500 cm             |
| 100% (top)       | 4092             | 2.000 cm             |

Using these calibration points, the function breaks the range into **segments** and performs **linear interpolation** between each one, so even if the slider stops somewhere between two points, the code estimates its position smoothly.

