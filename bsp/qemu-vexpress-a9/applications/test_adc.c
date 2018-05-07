#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <rtthread.h>
#include <rtdevice.h>


rt_err_t test_adc(int argc, char** argv)
{
	rt_err_t result = RT_EOK;
	int value;
	rt_device_t dev;
	int channel;
	
	dev = rt_device_find(argv[1]);
	if(!dev)
	{
		rt_kprintf("%s not found!\n", argv[1]);
		result = -RT_ERROR;
		goto _exit;
	}
	
	rt_kprintf("test_adc\n");
	for(channel=0; channel<9; channel++)
	{
		result = rt_device_read(dev, channel, &value, sizeof(value));
		if(result != RT_EOK)
		{
			rt_kprintf("channel %d: 0x%08X \n", channel, value);
			result = -RT_ERROR;
			goto _exit;
		}
		rt_kprintf("channel %d: 0x08X \n", channel, value);
	}

_exit:
	return result;
}

#include <finsh.h>

MSH_CMD_EXPORT(test_adc, test adc);
