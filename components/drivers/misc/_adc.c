#include <string.h>

#include <rtthread.h>
#include <rtdevice.h>

static rt_size_t _adc_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
	rt_err_t result = RT_EOK;
	rt_size_t i;
    struct rt_device_adc *adc = (struct rt_device_adc *)dev;
	int *value = (int *)buffer;
	
	if(!adc->ops->convert)
	{
		return 0;
	}

	for(i=0; i<size; i+=sizeof(int))
	{
		result = adc->ops->convert(dev, pos + i, value);
		if(result != RT_EOK)
		{
			return 0;
		}
		value++;
	}

    return i;
}

rt_err_t rt_device_adc_register(struct rt_device_adc *device, const char *name, const struct rt_adc_ops *ops, const void *user_data)
{
	rt_err_t result = RT_EOK;
	
	memset(device, 0, sizeof(struct rt_device_adc));

    device->parent.type         = RT_Device_Class_Miscellaneous;

    device->parent.init         = RT_NULL;
    device->parent.open         = RT_NULL;
    device->parent.close        = RT_NULL;
    device->parent.read         = _adc_read;
    device->parent.write        = RT_NULL;
    device->parent.control      = RT_NULL;

    device->ops                 = ops;
    device->parent.user_data    = (void *)user_data;

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

