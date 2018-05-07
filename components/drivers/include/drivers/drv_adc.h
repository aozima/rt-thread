#ifndef __DRV_ADC_H_INCLUDE__
#define __DRV_ADC_H_INCLUDE__

struct rt_adc_ops
{
    rt_err_t (*configure)(struct rt_device *device, int channel, rt_bool_t *enable);
    rt_uint32_t (*convert)(struct rt_device *device, int channel, int **res); //TODO
};

struct rt_device_adc
{
    struct rt_device parent;
    const struct rt_adc_ops *ops;
};

#endif /* __DRV_ADC_H_INCLUDE__ */
