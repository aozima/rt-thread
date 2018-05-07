#include <string.h>

#include <rtthread.h>
#include <rtdevice.h>

static rt_size_t _pwm_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    return 0;
}

static rt_size_t _pwm_write(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
	rt_err_t result = RT_EOK;
	rt_size_t i;
    struct rt_device_pwm *pwm = (struct rt_device_pwm *)dev;
	struct rt_pwm_configuration *configuration = (struct rt_pwm_configuration *)buffer;
	
	if(size != sizeof(struct rt_pwm_configuration))
	{
		return 0;
	}

	if(pwm->ops->set)
	{
		result = pwm->ops->set(dev, pos, configuration);
		if(result != RT_EOK)
		{
			return 0;
		}
	}

    return size;
}

rt_err_t rt_device_pwm_register(struct rt_device_pwm *device, const char *name, const struct rt_pwm_ops *ops, const void *user_data)
{
	rt_err_t result = RT_EOK;
	
	memset(device, 0, sizeof(struct rt_device_pwm));

    device->parent.type         = RT_Device_Class_Miscellaneous;

    device->parent.init         = RT_NULL;
    device->parent.open         = RT_NULL;
    device->parent.close        = RT_NULL;
    device->parent.read         = _pwm_read;
    device->parent.write        = _pwm_write;
    device->parent.control      = RT_NULL;

    device->ops                 = ops;
    device->parent.user_data    = (void *)user_data;

    result = rt_device_register(&device->parent, name, RT_DEVICE_FLAG_RDWR);

    return result;
}

rt_err_t rt_device_pwm_create(const char *name, const struct rt_pwm_ops *ops, const void *user_data)
{
	rt_err_t result = RT_EOK;
	struct rt_device_pwm *device;
	
	device = rt_malloc(sizeof(struct rt_device_pwm));
	if(!device)
	{
		result = -RT_ENOMEM;
		goto _exit;
	}
	
	result = rt_device_pwm_register(device, name, ops, user_data);
	
_exit:	
    return result;
}

