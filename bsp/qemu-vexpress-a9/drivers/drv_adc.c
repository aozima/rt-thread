#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <rtthread.h>
#include <rtdevice.h>

rt_err_t configure(struct rt_device *device, int channel, rt_bool_t *enable)
{
	rt_err_t result = RT_EOK;
	
	return result;
}

rt_uint32_t convert(struct rt_device *device, int channel, int *value)
{
	rt_err_t result = RT_EOK;
	
	return result;
}

static const struct rt_adc_ops adc_ops =
{
	configure,
	convert,
};


int rt_hw_adc_init(void)
{
    int ret = RT_EOK;

    ret = rt_device_adc_create("adc", &adc_ops, RT_NULL);

    return ret;
}
INIT_DEVICE_EXPORT(rt_hw_adc_init);
