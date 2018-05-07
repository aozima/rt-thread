#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <rtthread.h>
#include <rtdevice.h>


rt_err_t test_pwm(int argc, char** argv)
{
	rt_err_t result = RT_EOK;
	rt_device_t dev;
	int channel;
	struct rt_pwm_configuration configuration;
	
	dev = rt_device_find(argv[1]);
	if(!dev)
	{
		rt_kprintf("%s not found!\n", argv[1]);
		result = -RT_ERROR;
		goto _exit;
	}
	
	result = rt_device_open(dev, RT_DEVICE_FLAG_RDWR);
	if(result != RT_EOK)
	{
		rt_kprintf("open %s faild! \n", argv[1]);
		result = -RT_EIO;
		goto _exit;
	}
	
	rt_kprintf("test pwm: %s\n", argv[1]);
	for(channel=0; channel<9; channel++)
	{
		configuration.period = 800*(channel+1);
		configuration.pulse = configuration.period / 10 * (channel+1);
		rt_kprintf("\ntest pwm set channel: %d, period: %d, pulse: %d\n", channel, configuration.period, configuration.pulse); 
		
		if( rt_device_write(dev, channel, &configuration, sizeof(struct rt_pwm_configuration)) != sizeof(struct rt_pwm_configuration) )
		{
			rt_kprintf("test pwm set channel %d: faild! \n", channel);
			result = -RT_ERROR;
			goto _exit;
		}
	}

_exit:
	return result;
}

#include <finsh.h>

MSH_CMD_EXPORT(test_pwm, test pwm);
