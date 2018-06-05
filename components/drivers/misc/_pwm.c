#include <string.h>

#include <rtthread.h>
#include <rtdevice.h>

/*
pos: channel
void *buffer: rt_uint32_t pulse[size]
size : number of pulse, only set to 1.
*/
static rt_size_t _pwm_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_err_t result = RT_EOK;
    struct rt_device_pwm *pwm = (struct rt_device_pwm *)dev;
    rt_uint32_t *pulse = (rt_uint32_t *)buffer;
    struct rt_pwm_configuration configuration;

    if (pwm->ops->control)
    {
        result = pwm->ops->control(pwm, PWM_CMD_GET, pos, &configuration);
        if (result != RT_EOK)
        {
            return 0;
        }

        *pulse = configuration.pulse;
    }

    return 0;
}

/*
pos: channel
void *buffer: rt_uint32_t pulse[size]
size : number of pulse, only set to 1.
*/
static rt_size_t _pwm_write(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_err_t result = RT_EOK;
    struct rt_device_pwm *pwm = (struct rt_device_pwm *)dev;
    rt_uint32_t *pulse = (rt_uint32_t *)buffer;
    struct rt_pwm_configuration configuration;

    if (pwm->ops->control)
    {
        result = pwm->ops->control(pwm, PWM_CMD_GET, pos, &configuration);
        if (result != RT_EOK)
        {
            return 0;
        }

        configuration.pulse = *pulse;
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
