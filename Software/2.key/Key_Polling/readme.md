﻿![在这里插入图片描述](C:\Users\Administrator\Downloads\[nucleo-H7A3ZI-Q教程] 2、按键输入 -- 支持短按，长按和组合按键检测的代码移植.assets\34ee4465fd224b0da9630364753dfe61.jpeg)
# 前言
   **cubeMX**  是一个很好的工具，提高了在配置外设的效率，故本文不对外设寄存器进行讲解，百度和官方参考手册一直都是最好的答案。因此本文会简单介绍板子原理图，cubeMX的配置流程，以及移植代码的关键步骤。
   **[本文对应的代码在此，可以提前克隆下来](https://github.com/ALanStewart47/Project-H7A3ZI-Q/tree/main/Software/2.key)**。

# 原理图
![nucleo-h7a3zi-q key sheet](https://img-blog.csdnimg.cn/direct/af5ba7c24cf945a4a2f7efd9398c5bb6.png)
由图可知，当按下按键时候，PC13会得到一个高电平；反之，**没有按键按下时候则是低电平**。
这里稍微说一下，当按键按下时候，电流经过按键流到ESD管，由于器件有齐纳二极管一样的特性，当反向偏置的时候可以流过微小电流，并二极管两端电压保持稳定，由下图可以得知该器件的稳压值（反向截止电压）为3V，选择3V应该也是考虑到齐纳二极管的特性即稳压值会有误差；不过该器件主要功能应该是进行ESD保护。
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/326ae069c9874ccfa593348b05ef2cf3.png)
# CubeMX配置 
在cubemx新建工程，选择官方开发板进行配置（Board Selector），搜索NUCLEO-H7A3ZI-Q，并进入配置界面，弹出是否按照默认配置的时候，选择YES即可。
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/90b848fa93384e57b3b6d286fb6cb965.png)
进入配置界面后，会看到软件已经帮我们配置好开发板的按键、串口等外设了，我们只需配置想要的系统时钟即可，当然我们也可以不进行修改，按照默认的96MHz系统时钟生成代码工程。

# 代码移植
本文的按键功能是通过移植安富莱的按键板级支持包来实现的，而板级支持包是如何实现的，请移步到 [STM32H7的GPIO应用之按键FIFO](https://www.cnblogs.com/armfly/p/10879956.html) 此网站学习。此BSP支持短按，长按和组合按键检测功能，功能还是十分强大的。

## 简单介绍下移植的代码过程
1、拷贝bsp_key.c和bsp_key.h到你的工程里，如下图
![拷贝bsp_key](https://img-blog.csdnimg.cn/direct/dd9e2937f6234e8492b198587a222e0f.png)
需要注意的是，我把bsp_key.h的文件放在User\Bsp\inc文件夹里面。
2、通过MDK加入bsp_key.c和把对应头文件地址，如下图
![MDK添加bsp_key.c文件](https://img-blog.csdnimg.cn/direct/26bd45eb89af4caeba44bb45970d2b42.png)

![添加bsp_key.h](https://img-blog.csdnimg.cn/direct/edb245424f4c4566805f89c17411b0ad.png)
到这里我们已经把bsp_key的代码部分已经拷到自己的工程里面了。
接下来就到了真正的移植代码的部分啦

### 代码修改部分

 1. 头文件。源代码是依赖于bsp.h，我们这里并没有这个.h文件，故改为依赖HAL库stm32h7xx_hal.h，但是这里我把hal.h的依赖改到bsp_key.h里面，这样显得更整齐。

```c
#include "bsp_key.h"

#define HARD_KEY_NUM	    1	   							/* 实体按键个数（根据自己的项目实际来） */	
#define KEY_COUNT   	 	(HARD_KEY_NUM + 0)	/* 1个独立建 + 0个组合按键 */

/* 使能GPIO时钟 */
#define ALL_KEY_GPIO_CLK_ENABLE() {	\
		__HAL_RCC_GPIOB_CLK_ENABLE();	\
		__HAL_RCC_GPIOC_CLK_ENABLE();	\
		__HAL_RCC_GPIOG_CLK_ENABLE();	\
		__HAL_RCC_GPIOH_CLK_ENABLE();	\
		__HAL_RCC_GPIOI_CLK_ENABLE();	\
	};

```
这里还有一个HARD_KEY_NUM 需要修改的，改成你硬件上的按键数量，这里板上仅有一个功能按键。因为只有一个按键也无法带来组合按键的玩法，暂时就不展开这个功能了。
 2. 确定激活电平。激活电平意思是当按键按下的时候，对应IO的电平是低电平还是高电平，H7A3ZI板子按键是低电平（上面有介绍），故激活电平为0，这里bsp_key.c是用结构体数组的方式储存按键端口，pin和激活电平。

```c
/* 依次定义GPIO */
typedef struct
{
	GPIO_TypeDef* gpio;
	uint16_t pin;
	uint8_t ActiveLevel;	/* 激活电平 */
}X_GPIO_T;

/* GPIO和PIN定义 */
static const X_GPIO_T s_gpio_list[HARD_KEY_NUM] = {
	{GPIOC, GPIO_PIN_13, 1},	/* K1 -- PC13 , 激活电平为1（高电平）*/
};	

/* 定义一个宏函数简化后续代码 
	判断GPIO引脚是否有效按下
*/
static KEY_T s_tBtn[KEY_COUNT] = {0};
static KEY_FIFO_T s_tKey;		/* 按键FIFO变量,结构体 */
```

 3. 屏蔽或者删除组合按键的代码

```c
/*
*********************************************************************************************************
*	函 数 名: IsKeyDownFunc
*	功能说明: 判断按键是否按下。单键和组合键区分。单键事件不允许有其他键按下。
*	形    参: 无
*	返 回 值: 返回值1 表示按下(导通），0表示未按下（释放）
*********************************************************************************************************
*/
static uint8_t IsKeyDownFunc(uint8_t _id)
{
	/* 实体单键 */
	if (_id < HARD_KEY_NUM)
	{
		uint8_t i;
		uint8_t count = 0;
		uint8_t save = 255;
		
		/* 判断有几个键按下 */
		for (i = 0; i < HARD_KEY_NUM; i++)
		{
			if (KeyPinActive(i)) 
			{
				count++;
				save = i;
			}
		}
		
		if (count == 1 && save == _id)
		{
			return 1;	/* 只有1个键按下时才有效 */
		}		

		return 0;
	}
	
	/* 组合键 部分 */
	if (_id == HARD_KEY_NUM + 0)
	{
		/*
		if (KeyPinActive(KID_K1) && KeyPinActive(KID_K2))
		{
			return 1;
		}
		else
		{
			return 0;
		}
		*/
	}

	/* 组合键 部分 */
	if (_id == HARD_KEY_NUM + 1)
	{
		/*
		if (KeyPinActive(KID_K2) && KeyPinActive(KID_K3))
		{
			return 1;
		}
		else
		{
			return 0;
		}
		*/
	}

	return 0;
}
```
可以看到此处已经屏蔽了组合按键的代码了

4、(可选)删除一些没用到的宏定义或者结构体内容。在bsp_key.h里面，还有很多其他按键的宏定义，可以根据实际情况屏蔽或者删掉，本文暂不作拓展。

5、编写main函数。定义一个unsigned char的变量s_keyCode

```c
	uint8_t s_KeyCode	;
```
在while循环里面，加入bsp_KeyScan10ms()，该函数为周期性检测按键，然后通过bsp_GetKey()获取按键数值。

```c
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		HAL_Delay(10);
		bsp_KeyScan10ms();
		s_KeyCode=bsp_GetKey();
		if(s_KeyCode	!=	KEY_NONE)
		{
			
			if(s_KeyCode	==	KEY_DOWN_K1)
			{
				printf("Key 1 down\r\n");
			}

			if(s_KeyCode	==	KEY_1_UP)
			{
				printf("Key 1 up\r\n");
			}
			if(s_KeyCode	==	KEY_1_LONG)
			{
				printf("Key 1 long\r\n");
			}
		}
		
    /* USER CODE BEGIN 3 */
  }
```
## 下载验证
通过[本链接即可获得本文的代码](https://github.com/ALanStewart47/Project-H7A3ZI-Q/tree/main/Software/2.key/Key_Polling)，编译下载到H7A3ZI-Q开发板后，打开串口工具，会发现串口会发送用户按键动作行为。
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/9b5709ea49724a5498b4eca3c59a101f.png)

