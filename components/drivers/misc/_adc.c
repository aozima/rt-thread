#include <rtthread.h>
#include <rtdevice.h>

rt_err_t rt_device_adc_register(struct rt_device_adc *device, const char *name, const struct rt_adc_ops *ops, const void *user_data)
{
	rt_err_t result = RT_EOK;

    device->ops                 = ops;
    device->parent.user_data    = (void *)user_data;

    /* register a character device */
    result = rt_device_register(&device->parent, name, RT_DEVICE_FLAG_RDWR);

    return result;
}

rt_err_t rt_device_adc_create(const char *name, const struct rt_adc_ops *ops, const void *user_data)
{
	rt_err_t result = RT_EOK;
	struct rt_device_adc *device;
	
	device = rt_malloc(sizeof(struct rt_device_adc));
	if(!device)
	{
		result = -RT_ENOMEM;
		goto _exit;
	}
	
	result = rt_device_adc_register(device, name, ops, user_data);
	
_exit:	
    return result;
}

