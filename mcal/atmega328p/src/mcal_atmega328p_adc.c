#include "mcal.h"

void mcal_adc_init(void)
{
    // do nothing
}

void mcal_adc_set(mcal_adc_blkEnum_t *x_adcBlock)
{
    ADC_SetClk(x_adcBlock, ADC_HOLD_FIX, ADC_FC_DIVIDE_LEVEL_16);
}

void mcal_adc_channel_set(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh)
{
    ADC_SetSWTrg(x_adcBlock, ADC_REG0, TRG_ENABLE((x_adcCh)));
    ADC_Enable(x_adcBlock);
}

void mcal_adc_conversion_start(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh)
{
    ADC_SetSWTrg(x_adcBlock, ADC_REG0, TRG_ENABLE((x_adcCh)));
    ADC_Enable(x_adcBlock);
    ADC_Start(x_adcBlock, ADC_TRG_SW);
}

uint32_t mcal_adc_conversionResult_get(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh)
{
    ADC_Result ret = ADC_GetConvertResult(x_adcBlock, ADC_REG0);
    return (uint32_t)ret.Bit.ADResult;
}

uint32_t mcal_adc_conversionStatus_get(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh)
{
    return (uint32_t)ADC_GetConvertState(x_adcBlock, ADC_TRG_SW);
}

void mcal_adc_flag_clear(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh)
{
    //do nothing
}
