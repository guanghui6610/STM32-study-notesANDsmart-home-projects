## 目标功能

- [x] ##### 动调光功能，单片机检测光敏电阻，ADC检测该数值变化，根据此数值调节PWM输出占空比，实现根据环境光照调节灯光亮度。

- [x] ##### 定时开关功能，读取年、月、日、时、分、秒，定时开关灯。

- [x] ##### 场景切换功能，预设场景模式，不同场景模式对应不同的灯光亮度和颜色温度。

- [x] ##### 远程控制功能，单片机串口通信，与手机交互实现远程控制灯光。

- [x] ##### 人体感应功能，用HC-SR501人体红外感应模块，有人灯亮，无人灯灭。

- [x] ##### 温湿度环境变化功能，温湿度传感器，例如空气质量不佳时，降低光线亮度以减少损耗。

## 江科大PPT

 [STM32入门教程.pptx](STM32入门教程.pptx) 

## 官方参考手册

 [STM32F10xxx参考手册（中文）.pdf](STM32F10xxx参考手册（中文）.pdf) 

## 野火F103C8T6核心板引脚分配表

 [(野火小智)STM32F103C8核心板_双USB款_引脚分配表.xls]((野火小智)STM32F103C8核心板_双USB款_引脚分配表.xls) 

## 一、软件安装及创建工程

### 1.软件安装

1. 安装Keil5 MDK。
2. 安装器件支持包。只有Keil5才需要这一步，Keil4及之前的版本都不需要。因为现在的ARM芯片层出不穷，不可能每出一款新的芯片就升级整个软件，于是就将芯片的支持包单独拎出来，需要开发那个芯片就安装哪个器件支持包。若不安装器件支持包，那么在创建工程时不会出现对应的芯片型号。

```c
方式一：离线安装。去Keil公司官网的器件支持包下载页面选择对应的公司及器件型号，然后在Device Family Pack文本框中点击Download。下载完成后在本地直接点击pack文件进行安装即可。这个文件会自动识别Keil5文件的安装目录。
方式二：在线安装，速度取决于网速。打开Keil软件点Pack Installer，等待支持的公司及器件列表加载完成。STMicroelectronics公司列表下包含了所有的STM32芯片型号,然后在右侧的DFP文件后点击Install即可。
```

3. 软件注册。破解软件。
4. 安装对应的驱动，stlink,dap，usb转串口等

### 2. STM32的开发方式

1. **基于标准库（库函数）**。使用ST官方提供的封装好的库函数，通过调用这些函数来间接的配置寄存器。由于ST公司对寄存器封装的比较好，所以这种方式不仅可以满足寄存器配置，还可以提升开发效率。
2. **基于HAL库**。可以使用图形化界面快速配置STM32，适合快速上手STM32。但这种方式隐藏了底层逻辑，不利于对STM32的进一步学习。推荐学习完库函数的方式后，再去了解这种开发方式，可以极大的提升开发效率。

**注：**使用库函数进行开发，需要准备STM32库函数。这个库函数由[ST公司](https://www.st.com)提供（注意上面的器件支持包是[Keil公司](https://www.keil.com/)提供，不要搞混了），可以直接跳转到[最后的下载页面](https://www.st.com/en/embedded-software/stsw-stm32054.html)，选择对应的版本后便可以下载。

### 3.创建工程-基于库函数

**1.建立工程文件夹，Keil中新建工程，选择器件型号**

![屏幕截图 2024-10-21 153412](E:\笔记\STM32\STM32笔记\.assets\屏幕截图 2024-10-21 153412.png)



**2.在工程文件夹中创建Start,Library,System,Handware,User文件夹，并将固件库中的文件复制到Start和Library文件夹中**

- Start部分1：启动文件，位于固件库文件夹“.\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm”。STM32的程序从启动文件执行。
- Start部分2：位于固件库目录“.\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x”的三个文件。“stm32f10x.h”描述STM32**外设寄存器**及其对应的地址，作用相当于51单片机中的头文件“REGX52.h”。“system_stm32f10x.h”、“system_stm32f10x.c”主要**用于配置时钟**，比如配置STM32的主频为72MHz。
- Start部分3：位于固件库目录“.\Libraries\CMSIS\CM3\CoreSupport”的两个文件。“core_cm3.h”、“core_cm3.c”是**内核寄存器**的描述文件，并且还带了一些内核的配置函数。
- Library部分1：位于固件库目录“.\Libraries\STM32F10x_StdPeriph_Driver\src”。其中的```misc.c```是内核的库函数，其他的都是内核外的外设库函数。
- Library部分2：位于固件库目录“.\Libraries\STM32F10x_StdPeriph_Driver\inc”。是上述库函数的头文件。

![屏幕截图 2024-10-21 154620](E:\笔记\STM32\STM32笔记\.assets\屏幕截图 2024-10-21 154620.png)

![屏幕截图 2024-10-21 154637](E:\笔记\STM32\STM32笔记\.assets\屏幕截图 2024-10-21 154637.png)

**3.在Keil5软件项目管理中添加Start,Library,System,Handware,User到工程分组中**

![屏幕截图 2024-10-21 164729](E:\笔记\STM32\STM32笔记\.assets\屏幕截图 2024-10-21 164729.png)

**4.在Keil5软件工程选项(魔术棒):**

 - C/C++中，Define定义**USE_STDPERIPH_DRIVER**(这样才能包含标准外设库，即库函数）

 - C/C++中，Include Paths内声明所有包含头文件的文件夹(即将Start,Library,System,Handware,User文件路径添加上去)

   ![屏幕截图 2024-10-21 162149](E:\笔记\STM32\STM32笔记\.assets\屏幕截图 2024-10-21 162149.png)

 - Debug中，选择对应的调试器，Settings→Flash Download里勾选**Reset and Run**

   ![屏幕截图 2024-10-21 162225](E:\笔记\STM32\STM32笔记\.assets\屏幕截图 2024-10-21 162225.png)

   ![屏幕截图 2024-10-21 162219](E:\笔记\STM32\STM32笔记\.assets\屏幕截图 2024-10-21 162219.png)



**5.Use头文件下除main.c文件外还需要复制stm32f10x_conf.h、stm32f10x_it.c、stm32f10x_it.h文件**

 - stm32f10x_conf.h:调用固件外设库

 - stm32f10x_it.c、stm32f10x_it.c:存放STM32工程中所有的中断函数的模板文件

 - User部分1：存放用户编写的```main.c```等文件。

 - User部分2：位于固件库目录“.\Project\STM32F10x_StdPeriph_Template”中的三个文件。“stm32f10x_conf.h”用于配置库函数头文件的包含关系，并且也有**用于参数检查的函数定义，所有的库函数都需要**。“stm32f10x_it.h”、“stm32f10x_it.c”这两个文件用于存放中断函数。

   ![屏幕截图 2024-10-21 165230](E:\笔记\STM32\STM32笔记\.assets\屏幕截图 2024-10-21 165230.png)
   
   ### 1.4工程架构
   
   ![1741784537709](E:\笔记\STM32\STM32笔记\.assets\1741784537709.jpg)

## 二、模块教程

### 1.GPIO通用输入输出

- GPIO(General Purpose Input Output)通用输入输出口
- 可配置为8种输出输入模式
- 引脚电平：0V ~ 3.3V , 部分引脚可以容忍5V
- 输出模式下可以控制端口输出高低电平，用以驱动LED、控制蜂鸣器、模拟通信协议输出时序等
- 输入模式下可以读取端口的高低电平或电压，用于读取按键输入，外接模块电平信号输入、ADC电压采集、模拟通信协议接受数据等

#### **GPIO基本结构**

![屏幕截图 2024-10-21 170909](E:\笔记\STM32\STM32笔记\.assets\屏幕截图 2024-10-21 170909.png)

上图给出了GPIO的基本结构图。在STM32中，所有的GPIO都挂载在APB2外设总线上。命名方式采用GPIOA、GPIOB、GPIOC...的方式来命名。每个GPIO模块内，主要包括寄存器、驱动器等。

- 寄存器就是一段特殊的存储器，内核可以通过APB2总线对寄存器进行读写，从而完成输出电平和读取电平的功能。该寄存器的每一位都对应一个引脚，由于stm32是32位的单片机，所以所有的寄存器都是32位的，也就是说只有寄存器的低16位对应上了相应的GPIO口。
- 驱动器就是增加信号的驱动能力的。

#### **GPIO位结构**

![屏幕截图 2024-10-21 191306](E:\笔记\STM32\STM32笔记\.assets\屏幕截图 2024-10-21 191306.png)

上图就是将“GPIO的基本结构”进行放大，得到的实际的位结构。

**输入部分：**

1. 整体框架从左到右依次是寄存器、驱动器、IO引脚，从上到下分为“输入”、“输出”。
2. 最右侧的IO引脚上两个保护二极管，其作用是对IO引脚的输出电压进行限幅在0\~3.3V之间，进而可以避免过高的IO引脚输入电压对电路内部造成伤害。          V~DD~=3.3V，V~SS~=0V。
3. 输入驱动器的上、下拉电阻：相应的两个开关可以通过程序进行配置，分别有上拉输入模式（上开关导通&下开关断开）、下拉输入模式（下开关导通&上开关断开）、浮空输入模式（两个开关都断开）。上下拉电阻的作用就是给引脚输入提供一个默认的输入电平，进而避免引脚悬空导致的不确定。都属于弱上拉、弱下拉（弱是指电阻大，电流小，功耗小）。
4. 输入驱动器的触发器：这里是用肖特基管构成的施密特触发器。只有高于上限、低于下限电压才进行变化，作用是对输入电压进行整形，可以消除电压波纹、使电压的上升沿/下降沿更加陡峭。也就是说，**stm32的GPIO端口会自动对输入的数字电压进行整形。**
5. “模拟输入”、“复用功能输入”：都是连接到片上外设的一些端口，前者用于ADC等需要模拟输入的外设，后者用于串口输入引脚等需要数字量的外设。

**输出部分：**

驱动器中的MOS管：MOS管相当于一种开关，输出信号来控制这两个MOS管的开启状态，进而输出信号。可以选择推挽、开漏、关闭三种输出方式。

- 推挽输出模式：两个MOS管均有效，stm32对IO口有绝对的控制权，也称为强推输出模式。
- 开漏输出模式：P-MOS无效。只有低电平有驱动能力，高电平输出高阻。
- 关闭模式：两个MOS管均无效，端口电平由外部信号控制。

#### **GPIO模式**

|   模式名称   |   性质   |                        特征                        |
| :----------: | :------: | :------------------------------------------------: |
|   浮空输入   | 数字输入 |      可读取引脚电平，若引脚悬空，则电平不确定      |
|   上拉输入   | 数字输入 | 可读取引脚电平，内部连接上拉电阻，悬空时默认高电平 |
|   下拉输入   | 数字输入 | 可读取引脚电平，内部连接下拉电阻，悬空时默认低电平 |
|   模拟输入   | 模拟输入 |           GPIO无效，引脚直接接入内部ADC            |
|   开漏输出   | 数字输出 |    可输出引脚电平，高电平为高阻态，低电平接VSS     |
|   推挽输出   | 数字输出 |      可输出引脚电平，高电平接VDD，低电平接VSS      |
| 复用开漏输出 | 数字输出 |    由片上外设控制，高电平为高阻态，低电平接VSS     |
| 复用推挽输出 | 数字输出 |      由片上外设控制，高电平接VDD，低电平接VSS      |

#### **C语言语法**

**C语言数据类型**

|       关键字       | 位数 |         表示范围         | stdint关键字 |
| :----------------: | :--: | :----------------------: | :----------: |
|        char        |  8   |        -128 ~ 127        |    int8_t    |
|   unsigned char    |  8   |         0 ~ 255          |   uint8_t    |
|       short        |  16  |      -32768 ~ 32767      |   int16_t    |
|   unsigned short   |  16  |        0 ~ 65535         |   uint16_t   |
|        int         |  32  | -2147483648 ~ 2147483647 |   int32_t    |
|    unsigned int    |  32  |      0 ~ 4294967295      |   uint32_t   |
|     long long      |  64  |  -(2^64)/2 ~ (2^64)/2-1  |   int64_t    |
| unsigned long long |  64  |       0 ~ (2^64)-1       |   uint64_t   |

**宏定义#define**

关键字 ```#define```，主要用于：用一个字符串代替一个数字，便于理解，防止出错；或者提取程序中经常出现的参数，便于快速修改。

```c
//定义宏定义：
#define ABC 12345
//引用宏定义：
int a = ABC;  //等效于int a = 12345;
```

**关键字typedef**

关键字 ```typedef```，常用于将一个比较长的变量类型名换个名字，便于使用。

```c
//定义typedef：
typedef unsigned char uint8_t;
//引用typedef：
uint8_t num1; //等效于unsigned char num1;
```

**结构体struct**

关键字 ```struct```，用途：数据打包，将不同类型变量组成一个集合。

```c
//在main函数中定义结构体变量：
struct{
    char x;
    int y;
    float z;
} StructName;
//因为结构体变量类型较长，所以通常在main函数外用typedef更改变量类型名
typedef struct{
    char x;
    int y;
    float z;
} StructName;
//引用结构体成员
StructName struct1;
struct1.x = 'A';
struct1.y = 66;
struct1.z = 1.23;
```

**枚举类型**

关键字 ```enum```，用途：定义一个取值受限制的整型变量，用于限制变量取值范围；枚举也相当于一个宏定义的集合，可以直接把里面的枚举变量拿出来用。注意枚举变量用逗号隔开，且最后一个枚举变量不加逗号。

```c
//函数内定义枚举变量：
enum{
    FALSE = 0,
    TRUE  = 1
} EnumName;
//因为枚举变量类型较长，所以通常在函数外用typedef更改变量类型名
typedef enum{
    FALSE = 0,
    TRUE  = 1
} EnumName;

//引用枚举成员：
EnumName emu1;
emu1 = FALSE;
emu1 = TRUE;
```

### 2.GPIO输出输入实现

#### GPIO输出

1. **RCC开启GPIO时钟**
2. **使用GPIO_Init函数初始GPIO**
3. **使用各类函数控制GPIO**

**GPIO引脚初始化**

```c
/*开启时钟，使用各个外设前必须开启时钟，否则对外设的操作无效*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);//将赋值后的结构体变量传递给GPIO_Init函数
									//函数内部会自动根据结构体的参数配置相应寄存器实现GPIOA的初始化
```

#### GPIO输入

按键控制为例

**key.c**

```c
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
}
//此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
uint8_t Get_KeyNum(void)
{
	uint8_t Key_Num = 0;
	
	if(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_0) == 0)        	//读PB1输入寄存器的状态，如果																	为0，则代表按键按下
	{
		Delay_ms(50);											//延时消抖
		while(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_0) == 0);	//等待按键松手
		Delay_ms(50);											//延时消抖
		Key_Num = 1;											//置键码为1
	}
	
	if(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) == 0)
	{
		Delay_ms(50);
		while(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) == 0);
		Delay_ms(50);
		Key_Num = 2;
	}
	
	return Key_Num;
}
```

**main.c**

```c
#include "stm32f10x.h"    // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"

uint8_t Key_Num;
/*
1.使用RCC开启GPIO时钟
2.使用GPIO_Init函数初始化GPIO
3.使用各类函数控制GPIO
*/
int main(void)
{
	LED_Init();
	Key_Init();
	
	while(1)
	{
		Key_Num = Key_GetNum();
		if(Key_Num == 1)
		{
			LED1_Turn();
		}
		if(Key_Num == 2)
		{
			LED2_Turn();
		}
	}
}
```

### 3.OLED显示屏

OLED（Organic Light Emitting Diode）意为 有机发光二极管。由OLED构成的OLED显示屏是一款性能优异的新型显示屏，其每一个像素都是一个单独的发光二极管，每个像素都可以自发光，所以具有功耗低、响应速度快、宽视角、轻薄柔韧等特点。本实验采用 **0.96寸OLED模块**，其小巧玲珑、占用接口少、简单易用，是电子设计中非常常见的显示屏模块，下面是一些基本参数：

> - 供电：3~5.5V。
> - 通信协议：4针脚常用I2C，7针脚常用SPI。
> - 分辨率：128*64。

#### OLED显示常用函数声明

```c
void OLED_Init(void);//OLED初始化
void OLED_Clear(void);//OLED显示屏清0
void OLED_ShowChar(行，列，字节)；//显示字符
void OLED_ShowString(行，列，字符串)；//显示字符串
void OLED_ShowNum(行，列，数字，长度)；//显示十进制无符号数
void OLED_ShowSignedNum(行，列，数字，长度)；//显示十进制有符号数
void OLED_ShowHexNum(行，列，数字，长度)；//显示16位进制数
void OLED_ShowBinNum(行，列，数字，长度)；//显示2进制数
```

#### 显存数组

- 调用显存数组OLED_Update函数才会将显存数组的数据发送到OLED硬件，进行显示。（提高显示效率和对显示的掌控性）

#### 常用函数

```c
/*初始化函数*/
void OLED_Init(void);

/*更新函数*/
void OLED_Update(void);
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);

/*显存控制函数*/
void OLED_Clear(void);
void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);

/*显示函数*/
void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize);//显示字符
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize);//显示字符串
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);//显示十进制无符号数字
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);//显示十进制有符号数字
void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);//显示十六进制数字
void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);//显示二进制数字
void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);//显示浮点数
void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese);//显示汉字，需要提前取模
void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);//显示图像，同样需要取模
void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...);//与上述功能一致，不常用

/*绘图函数*/
void OLED_DrawPoint(uint8_t X, uint8_t Y);//点亮一个像素
uint8_t OLED_GetPoint(uint8_t X, uint8_t Y);//判断该点是否被点亮，点亮返回1
void OLED_DrawLine(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1);//画线
void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);//画矩形
void OLED_DrawTriangle(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t IsFilled);//画三角
void OLED_DrawCircle(uint8_t X, uint8_t Y, uint8_t Radius, uint8_t IsFilled);//画圆
void OLED_DrawEllipse(uint8_t X, uint8_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);//画椭圆
void OLED_DrawArc(uint8_t X, uint8_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);//画弧

```

#### 驱动芯片SSD1306(0.96寸OLED显示屏)

- SSD1306是一款OLED/PLED点阵显示屏的控制器，可以嵌入在屏幕中，用于执行接收数据、显示存储、扫描刷新等任务（相当于显卡）
- 驱动接口：128个SEG引脚和64个COM引脚，对应128*64像素点阵显示屏

#### OLED显示屏原理

1. 通过引出来的通信引脚，把想显示的内容发送给驱动芯片
2. 驱动芯片收到数据将其存储起来，也就是芯片内部有RAM显示存储器，存储的是屏幕显示的内容
3. 驱动芯片内部有时钟和扫描电路，根据显示存储器的数据，它会自动对应刷新到屏幕

#### OLED模块经验

- OLED_Clear函数放在中断程序中，会导致清屏不完整，可以采用进入中断后附值，在main函数中，判断该值变化后进行清屏操作，记得在main函数中将该值还原。
- 记得调用OLED_Updata函数将显存数组的内容更新到OLED硬件进行显示

### 4.光敏传感器

 [野火小智光敏模块规格手册_20240410.pdf](..\..\..\STM32野火\模块资料\野火小智光敏电阻_20240418\1-硬件资料_参考资料_模块手册\野火小智光敏模块规格手册_20240410.pdf) 

**LightSensor.c**

```c
void LightSenor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStrucure;
	GPIO_InitStrucure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStrucure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStrucure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStrucure);
}

uint8_t LightSensor_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_6);

}
```

**main.c**

```c
#include "stm32f10x.h"    // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "LightSensor.h"

uint8_t Key_Num;
/*
1.使用RCC开启GPIO时钟
2.使用GPIO_Init函数初始化GPIO
3.使用各类函数控制GPIO
*/
int main(void)
{
	LED_Init();
	LightSenor_Init();
	while(1)
	{
// 光照变亮，光敏电阻阻值减小，由于电流保持不变，电阻减小，同相输入端分压减小，此时反相输入端电压高于同相输入端，输出低电平。
		if (LightSensor_Get() == 0) // 光照变亮，执行下述IF语句。顺时针旋转电位器，是让反向输入端电压变低，所以想输出低电平点亮LED就需要正向输入端电压更低，即光敏电阻阻值更小，即光照更亮
		{
			LED1_ON();
			LED2_OFF();
		}
		else
		{
			LED1_OFF();
			LED2_ON();
		}
	}
}
```

### 5.外部中断

- 中断：主程序运行过程中，出现了特定的**中断触发条件（中断源）**，使得**CPU暂停当前正在运行的程序**，**转而去处理中断程序**，**处理完成后又返回原来被暂停的位置继续运行**
- 中断优先级：当有多个中断源同时申请中断时，CPU会根据中断源的轻重缓急进行裁决，**优先响应更加紧急的中断源**
- 中断嵌套：当一个中断程序正在运行时，又有新的更高优先级的中断源申请中断，CPU再次暂停当前中断程序，转而去处理新的中断程序，处理完成后依次进行返回
- 使用**NVIC统一管理中断**，每个中断通道都拥有16个可编程的优先等级，可对优先级进行分组，进一步设置抢占优先级和响应优先级

#### 中断执行流程

![1729572301529(1)](E:\笔记\STM32\STM32笔记\.assets\1729572301529(1).jpg)

#### NVIC基本结构

![1729572399982(1)](E:\笔记\STM32\STM32笔记\.assets\1729572399982(1).jpg)

上图给出了 **嵌套向量中断控制器NVIC** 的基本结构示意图。在stm32中，NVIC用于统一管理中断和分配中断优先级，属于内核外设，是CPU的小助手，可以让CPU专注于运算。从上图可以看出：

> - NVIC有很多输入口，每个都代表一个中断线路，如EXIT、TIM、ADC等。
> - 每个中断线路上的斜杠n表示n条线，因为一个外设可能会同时占用多个中断通道。
> - NVIC只有一个输出口，通过中断优先级确定中断执行的顺序。

#### NVIC优先级分组

- NVIC的中断优先级由优先级寄存器的4位（0~15）决定，这4位可以进行切分，分为高n位的抢占优先级和低4-n位的响应优先级
- 抢占优先级高的可以中断嵌套，响应优先级高的可以优先排队，抢占优先级和响应优先级均相同的按中断号排队

> 用医院的叫号系统来举例子。假设医生正在给某个病人看病，外面还有很多病人排队：
>
> 1. 新来的病人 抢占优先级高 就相当于直接进屋打断医生，给自己看病。
> 2. 新来的病人 响应优先级高 就相当于不打扰医生，但直接插队，排在队伍的第一个。

| 分组方式 |   抢占优先级    |   响应优先级    |
| :------: | :-------------: | :-------------: |
|  分组0   |  0位，取值为0   | 4位，取值为0~15 |
|  分组1   | 1位，取值为0~1  | 3位，取值为0~7  |
|  分组2   | 2位，取值为0~3  | 2位，取值为0~3  |
|  分组3   | 3位，取值为0~7  | 1位，取值为0~1  |
|  分组4   | 4位，取值为0~15 |  0位，取值为0   |

#### EXTI(Extern Interrupt)外部中断

- **EXTI可以监测指定GPIO口的电平信号，当其指定的GPIO口产生电平变化时，EXTI将立即向NVIC发出中断申请，经过NVIC裁决后即可中断CPU主程序，使CPU执行EXTI对应的中断程序**
- 支持的触发方式：**上升沿/下降沿/双边沿/软件触发**
- 支持的GPIO口：所有GPIO口，但**相同的Pin不能同时触发中断**
- 通道数：16个GPIO_Pin，外加PVD输出、RTC闹钟、USB唤醒、以太网唤醒

> 注：后面这四个功能是为了实现一些特殊的功能，比如想实现某个时间让stm32退出停止模式，由于外部中断可以在低功耗模式的停止模式下唤醒stm32，就可以在GPIO口上连接一个RTC时钟作为外部中断。

- 触发响应方式：**中断响应/事件响应**

> 注：中断响应就是正常的中断流程，申请中断让CPU执行中断函数；事件响应就是外部中断发生时，不把外部中断信号给CPU，而是选择触发一个事件，将这个信号通向其他外设，来触发其他外设的操作，可以实现外设之间的联合工作。

#### EXTI基本结构

![1729572654049(1)](E:\笔记\STM32\STM32笔记\.assets\1729572654049(1).jpg)

> - 最左侧：GPIO口的外设，每个外设都有16个引脚。
> - AFIO中断引脚选择：本质上就是数据选择器，从前面16*n个引脚中选择16根端口号不重复的引脚出来，连接到后面的EXTI通道中。在STM32中，AFIO主要完成两个任务：复用功能引脚重映射、中断引脚选择。
> - 注：**上面这个EXTI的基本结构也是编写代码时的主要参考图！**

#### EXTI框图

![1729573156593](E:\笔记\STM32\STM32笔记\.assets\1729573156593.jpg)

> - 边沿检测电路+软件中断事件寄存器：这几个进行或门输出，便可以实现“上升沿/下降沿/双边沿/软件触发”这四种触发方式。
> - 请求挂起寄存器：相当于一个中断标志位，通过读取该寄存器可以判断是哪个通道触发的中断。
> - 中断屏蔽寄存器/事件屏蔽寄存器：相当于开关，只有置1，中断信号才能继续向左走。
> - 脉冲发生器：产生一个电平脉冲，用于触发其他外设的动作。

#### 外部中断需求场合

对于stm32来说，若想获取一个由外部驱动的很快的突发信号，就需要外部中断。

- 如旋转编码器，平常不会有什么变化，但是一旦拧动时，会产生一段时间变化很快的突发信号，就需要stm32能在短时间内快速读取并处理掉这个数据。
- 再如红外遥控接收头，平常也不会有什么变化，但是一旦接收到信号时，这个信号也是转瞬即逝的。
- 但是**不推荐按键使用外部中断**。因为外部中断不能很好的处理按键抖动和松手检测的问题，所以要求不高时，还是建议在主函数内部循环读取。

#### NVIC优先级配置

```c
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//配置NVIC为分组2
												//即抢占优先级范围：0~3，响应优先级范围：0~3
												//此分组配置在整个工程中仅需调用一次
												//若有多个中断，可以把此代码放在main函数内,while													循环之前
												//若调用多次配置分组的代码，则后执行的配置会覆盖先													执行的配置
```

#### 光敏模块驱动代码

**LightSensor.c**

```c
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

static uint16_t LightSensor_Count;
void LightSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);		//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);		//外部中断必须开启AFIO的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA , GPIO_PinSource6);//将外部中断的6号线映射到GPIOA，即选择PA6为外部中断引脚
	
	EXTI_InitTypeDef EXTI_InitStructure;				//定义结构体变量
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;		//选择配置外部中断的6号线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//指定外部中断线使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//指定外部中断线为中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//指定外部中断线为下降沿触发
	EXTI_Init(&EXTI_InitStructure);		//将结构体变量交给EXTI_Init，配置EXTI外设
	
	NVIC_InitTypeDef NVIC_InitStructure;				//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//选择配置NVIC的EXTI9_5线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//指定NVIC线路的抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	  //指定NVIC线路的响应优先级为0
	NVIC_Init(&NVIC_InitStructure);	//将结构体变量交给NVIC_Init，配置NVIC外设
}


uint16_t LightSensor_GET_IO(void)
{
	return GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_6);
}

/*此函数为中断函数，无需调用，中断触发后自动执行*/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6) == SET)						//判断是否是外部中断6号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_6) == 0)
		{
			LightSensor_Count++;
		}
		EXTI_ClearITPendingBit(EXTI_Line6);						//清除外部中断6号线的中断标志位
																//中断标志位必须清除，否则中断将连续不断地触发，导致主程序卡死
		
	}
}

uint16_t LightSensor_Get(void)
{
	return LightSensor_Count;
}

```

**main.c**

```c
#include "stm32f10x.h"    // Device header
#include "LED.h"
#include "LightSensor.h"
#include "OLED.h"
#include "Key.h"

/*
1.使用RCC开启GPIO时钟
2.使用GPIO_Init函数初始化GPIO
3.使用各类函数控制GPIO

*/

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	LightSenor_Init();
	OLED_Init();
	LED_Init();
	KeyInterrupt_Init();
	
	OLED_ShowString(1 , 1 , "LCount:00");
	OLED_ShowString(2 , 1 , "KCount:00");
	while(1)
	{
		if(LightSensor1_Get() == 0)
		{
			OLED_ShowNum(1 , 8 , LightSensor_Get() , 2);
			LED1_ON();
			LED2_OFF();
		}
		else
		{
			LED1_OFF();
			LED2_ON();
		}
		if(Key_GetNum() == 2)
		{
			OLED_ShowNum(2 , 8 , KeyInterrupt_Get() , 2);
		}
	}
}

```

#### **注意事项**

1. 注意每个模块在使用的时候都要进行初始化。
2. 注意进入中断函数的时候要检查中断标志位，退出的时候清零中断标志位。
3. 注意主函数和中断函数不要操控同一个硬件，避免不必要的硬件冲突。中断函数一般执行简短快速的代码，如操作中断标志位等。

### 6.定时器定时中断

- **TIM（Timer）定时器**
- 定时器可以对**输入的时钟进行计数**，并在**计数值达到设定值时触发中断**
- 16位计数器、预分频器、自动重装寄存器的时基单元，在72MHz计数时钟下可以实现最大59.65s的定时

```c
如STM32中主频一般是72MHz，那么计数值设定为72，就是每1us触发一次中断；计数值设定为72000，就是每1ms触发一次中断。
```

- 不仅具备基本的定时中断功能，而且还包含内外时钟源选择、输入捕获、输出比较、编码器接口、主从触发模式等多种功能
- 根据复杂度和应用场景分为了高级定时器、通用定时器、基本定时器三种类型
- **STM32F103C8T6定时器资源：TIM1、TIM2、TIM3、TIM4**

|    类型    |          编号          | 总线 |                             功能                             |
| :--------: | :--------------------: | :--: | :----------------------------------------------------------: |
| 高级定时器 |       TIM1、TIM8       | APB2 | 拥有通用定时器全部功能，并额外具有重复计数器、死区生成、互补输出、刹车输入等功能（对出来的功能是为了三相无刷电机的驱动设计的） |
| 通用定时器 | TIM2、TIM3、TIM4、TIM5 | APB1 | 拥有基本定时器全部功能，并额外具有内外时钟源选择、输入捕获、输出比较、编码器接口、主从触发模式等功能 |
| 基本定时器 |       TIM6、TIM7       | APB1 |              拥有定时中断、主模式触发DAC的功能               |

#### 定时中断基本结构

![1729604486130](E:\笔记\STM32\STM32笔记\.assets\1729604486130.jpg)

> - 内部时钟：一般就是系统的主频72MHz，通向时基单元的输入。
>
> - 时基单元：16位预分频器 + 16位计数器 + 16位自动重装载寄存器。
>
> > 1. 预分频器：对输入的72MHz时钟进行预分频，寄存器内存储的值是实际的分频系数减一。写0就是不分频，写1就是2分频，写2就是3分频……
> > 2. 计数器：对预分频后的计数时钟进行计数，每遇到上升沿就加一。
> > 3. 自动重装载寄存器：存储计数的最大值，到达此值后触发中断并清零计数器。

**计数器计数频率：CK_CNT = CK_PSC / (PSC + 1)**

**计数器溢出频率：CK_CNT_OV = CK_PSC / (PSC + 1) / (ARR + 1)**

> - 注：CK_CNT定时器时钟；CK_PSC(Prescaler)预分频器的输入频率；CK_ARR(Auto-Reload Register)自动重装寄存器的输入频率

#### 预分频器时序图-2分频

![1742177560896](E:\笔记\STM32\STM32笔记\.assets\1742177560896.jpg)

> - CK_PSC：预分频器的输入时钟，选择内部时钟源就是72MHz。
> - CNT_EN：计数器使能。高电平计数器正常运行，低电平计数器停止。
> - CK_CNT：计数器时钟，既是预分频器的时钟输出，也是计数器的时钟输入。
> - 计数器寄存器：对CK_CNT进行自增计数，到达自动重装载值清零。
> - 更新事件：计数器寄存器到达自动重装载值时产生一个脉冲。
>
> 下面三行时序体现了预分频计数器的一种缓冲机制：
>
> - 预分频控制寄存器：供用户读写使用，实时响应用户控制，但并不直接决定分频系数。
> - 预分频缓冲器：也称为影子寄存器，真正起分频作用的寄存器。只有在更新事件到达时，才从“预分频控制寄存器”更新预分频参数。以确保更新事件的稳定性。
> - 预分频计数器：按照预分频参数进行计数，以产生对应的CK_CNT脉冲。

#### RCC时钟树

![1742179107014](E:\笔记\STM32\STM32笔记\.assets\1742179107014.jpg)

> > 时钟是所有外设的基础，所以是需要最先配置的。ST公司写好了```SystemInit```函数来配置时钟树，下面具体介绍：
> >
> > - 左侧是**时钟产生电路**，右侧**时钟分配电路**，中间的SYSCLK就是72MHz的系统时钟。
> >
> > - 时钟产生电路：
> >
> > > - 四个振荡源：
> > >
> > > > 1. 内部的8MHz高速RC振荡器。
> > > > 2. 外部的4-16MHz高速石英晶体振荡器：一般8MHz，相比于内部的RC高速振荡器更加稳定。
> > > > 3. 外部的32.768kHz低速晶振：一般给RTC提供时钟。
> > > > 4. 内部的40kHz低速RC振荡器：给看门狗提供时钟。
> > > >
> > > > 上面的两个高速晶振用于给系统提供时钟，如AGB、APB1、APB2的时钟。
> > >
> > > - SystemInit函数配置时钟的过程：首先启动内部8MHz时钟为系统时钟，然后配置外部8MHz时钟到PLLMUL模块进行9倍频到72MHz，等到这个72MHz时钟稳定后，再将其作为系统时钟。于是就实现了系统时钟从8MHz切换到72MHz。
> > > - CSS：监测外部时钟的运行状态，一旦外部时钟失效，就会自动把外部时钟切换成内部时钟，保障系统时钟的运行，防止程序卡死造成事故。如果外部晶振出问题，那么就会导致程序的时钟变为8MHz，也就是比预期的时钟慢了9倍。
> >
> > - 时钟分配电路
> >
> > > - AHB总线：有预分频器，SysytemInit配置分频系数为1，于是AHB时钟输出就是72MHz。
> > > - APB1总线：SysytemInit配置分频系数为2，于是APB1时钟输出就是36MHz。
> > > - APB2总线：SysytemInit配置分频系数为1，于是APB2时钟输出就是72MHz。
> > > - 外设时钟使能：就是库函数```RCC_APB2PeriphClockCmd```开启的地方，可以控制相应的外设时钟开启。
> > > - 定时器的时钟：从图中可以看出，按照SystemInit的默认配置，所有的定时器时钟都是72MHz。
>



#### 定时器计数中断驱动代码-内部时钟

```c
#include "stm32f10x.h"                  // Device header

extern uint16_t Num;

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);//开启TIM2的时钟
	
	TIM_InternalClockConfig(TIM2);//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_InitStructure;//定义结构体变量
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频，选择不分频，外部时钟源的输入滤波器采样频率，内部时钟无所谓
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式，选择向上计数
	TIM_InitStructure.TIM_Period = 10000 - 1;//计数周期，即ARR的值
	TIM_InitStructure.TIM_Prescaler = 7200 - 1;//预分频器，即PSC的值
	TIM_InitStructure.TIM_RepetitionCounter = 0;//重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2 , &TIM_InitStructure);//将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元
	
	TIM_ClearFlag(TIM2 , TIM_FLAG_Update);	//清除定时器更新标志位
											//TIM_TimeBaseInit函数末尾，手动产生了更新事件
											//若不清除此标志位，则开启中断后，会立刻进入一次中断
	TIM_ITConfig(TIM2 , TIM_IT_Update , ENABLE);//开启TIM2的更新中断
	
	NVIC_InitTypeDef NVIC_InitStructure;//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//选择配置NVIC的TIM2线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//指定NVIC线路的抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//指定NVIC线路的响应优先级为0
	NVIC_Init(&NVIC_InitStructure);//将结构体变量交给NVIC_Init，配置NVIC外设
	
	TIM_Cmd(TIM2 , ENABLE);//使能TIM2，定时器开始运行
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2 , TIM_IT_Update) == SET)
	{
		Num++;
		TIM_ClearITPendingBit(TIM2 , TIM_IT_Update);
	}
}

```

#### 注意事项：

> 复位后计数器的值从1开始而不是从0开始，说明上电初始化后TIM2就立刻中断了一次。这是因为时基单元初始化函数```TIM_TimeBaseInit```，在函数的最后生成了一个更新事件，来保证可以立刻重新装载预分频器和重复计数器的值。要消除这个影响，就在```TIM_TimeBaseInit```后面加一句```TIM_ClearFlag```来清除相应的中断标志位。

#### 定时器计数中断驱动代码-外部时钟

TIM.c

```c
#include "stm32f10x.h"                  // Device header

// 定时器初始化-TIM2
void Timer_Init(void)
{
    //1.初始化RCC内部时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	 //2.配置GPIO-PA0
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//虽然器件手册推荐浮空输入，但上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);	
	//3.选择时基单元的时钟-ETR外部时钟模式2
	TIM_ETRClockMode2Config(TIM2 , TIM_ExtTRGPSC_OFF , TIM_ExtTRGPolarity_NonInverted , 0x05);
	//4.配置时基单元
	TIM_TimeBaseInitTypeDef TimeBase_InitStructure;
	TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBase_InitStructure.TIM_Period = 10 - 1;//ARR自动重装器的值
	TimeBase_InitStructure.TIM_Prescaler =1 - 1;//PSC预分频的值
	TimeBase_InitStructure.TIM_RepetitionCounter = 0;//重复计数器的值（高级定时器才有）
	TIM_TimeBaseInit(TIM2 , &TimeBase_InitStructure);
	
	TIM_ClearFlag(TIM2 , TIM_FLAG_Update);//消除上一行TIM_TimeBaseInit立刻产生更新事件影响
	 //5.配置中断输出控制
	TIM_ITConfig(TIM2 , TIM_IT_Update , ENABLE);
	 //6.配置NVIC
	/*嵌套向量中断控制器NVIC(Nested Vectored Interrupt Controller)*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	//7.配置运行控制
	TIM_Cmd(TIM2 , ENABLE);
}

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}

uint16_t LightSensor_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0);
}

```

main.c

```c
#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "OLED.h"
#include "LED.h"

uint16_t Num;

/*
1.使用RCC开启GPIO时钟
2.使用GPIO_Init函数初始化GPIO
3.使用各类函数控制GPIO

*/
int main(void)
{
	
	/*决定了抢占优先级和响应优先级的位数分配*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//LED_Init();
	OLED_Init();
	Timer_Init();
	
	OLED_ShowString(1 , 1 ,"Num:");
	OLED_ShowString(2 , 1 ,"CNT:");
	
	while(1)
	{
		OLED_ShowNum(1 , 5 , Num , 3);
		OLED_ShowNum(2 , 5 , Timer_GetCounter() , 3);
		OLED_ShowNum(3 , 5 , GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0) , 3);
		//OLED_ShowNum(3 , 5 , GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0) , 3);
		
		/*显示重装值*/
		OLED_ShowNum(2 , 5 , TIM_GetCounter(TIM2) , 5);
		
	}
}
//TIM2定时中断后的中断函数
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2 , TIM_IT_Update) == SET)
	{
		Num++;
		TIM_ClearITPendingBit(TIM2 , TIM_IT_Update);
	}
}
```

**注意事项：**

> 1. PA0配置成上拉输入：虽然手册里写TIM2的ETR时钟配置成浮空输入，浮空输入会导致输入电平跳个没完。只有当外部输入信号的功率很小，内部的上拉的电阻可能会影响到输入信号，此时采用浮空输入防止影响外部输入的电平。其他情况一律上拉输入。
> 2. 关于时钟源选择函数 ```TIM_ETRClockMode2Config```：第三个参数设置极性，其实就是规定在外部时钟的 上升沿/下降沿 计数；第四个参数滤波器，就是设置对于外部时钟的采样情况，具体的含义可以参考器件手册“14.4.3 从模式控制寄存器(TIMx_SMCR)”中的位11:8。

### 7.PWM输出

#### TIM输出比较介绍

- **OC（Output Compare）输出比较**
- 输出比较可以**通过比较CNT计数器与CCR捕获/比较寄存器值**的关系**，来对输出电平进行置1、置0或翻转的操作，用于**输出一定频率和占空比的PWM波形

#### PWM介绍

- **PWM（Pulse Width Modulation）脉冲宽度调制**
- 在具有惯性的系统中，可以通过对一系列脉冲的宽度进行调制，来等效地获得所需要的模拟参量，常应用于电机控速等领域
- PWM参数：**频率 = 1 / TS**  ，**占空比 = TON / TS**   ， 分辨率 = 占空比变化步距
- 定时中断的频率就是PWM波的频率，只不过占空比的变化范围由自动重装载值决定

![1729824343476(1)](E:\笔记\STM32\STM32笔记\.assets\1729824343476(1).jpg)



#### 输出比较通道（高级）

![1729824428007](E:\笔记\STM32\STM32笔记\.assets\1729824428007.jpg)

> 相比于“通用定时器”，“高级定时器”的输出比较电路增加了“死区生成器”和“互补输出电路”。
>
> - 死区生成器：消除上下两路可能同时导通的短暂状态，以防止电路发热、功率损耗。
> - 互补输出OC1N：与OC1反相，用于驱动三相无刷电机。

#### 输出比较通道（通用）

![1729824467540](E:\笔记\STM32\STM32笔记\.assets\1729824467540.jpg)

> - 左侧输入：CNT和CCR比较的结果。
> - ETRF：定时器的小功能，一般不用，无需了解
> - 主模式控制器：可以将oc1ref映射到主模式的TRGO输出上去。
> - CC1P：极性选择。选择是否需要将oc1ref的高低电平翻转一下。
> - 输出使能电路：由CC1E选择要不要输出。
> - OC1：后续通过TIMx_CH1输出到GPIO引脚上。

#### 输出比较模式

|       模式       |                             描述                             |
| :--------------: | :----------------------------------------------------------: |
|       冻结       |                  CNT=CCR时，REF保持为原状态                  |
| 匹配时置有效电平 |                   CNT=CCR时，REF置有效电平                   |
| 匹配时置无效电平 |                   CNT=CCR时，REF置无效电平                   |
|  匹配时电平翻转  |                    CNT=CCR时，REF电平翻转                    |
|  强制为无效电平  |               CNT与CCR无效，REF强制为无效电平                |
|  强制为有效电平  |               CNT与CCR无效，REF强制为有效电平                |
|     PWM模式1     | 向上计数：CNT<CCR时，REF置有效电平，CNT≥CCR时，REF置无效电平向下计数：CNT>CCR时，REF置无效电平，CNT≤CCR时，REF置有效电平 |
|     PWM模式2     | 向上计数：CNT<CCR时，REF置无效电平，CNT≥CCR时，REF置有效电平向下计数：CNT>CCR时，REF置有效电平，CNT≤CCR时，REF置无效电平 |

#### PWM基本结构

![1729824549575](E:\笔记\STM32\STM32笔记\.assets\1729824549575.jpg)

> 上图与“定时器中断”的区别在于最后输出的时候不需要“更新事件”的中断申请，而是走输出比较电路。

#### 参数计算

![1729824593724(1)](E:\笔记\STM32\STM32笔记\.assets\1729824593724(1).jpg)

#### PWM驱动呼吸灯代码

PWM.c

```c
#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE); //开启TIM2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//开启GPIOA的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//将PA2引脚初始化为复用推挽输出
										//受外设控制的引脚，均需要配置为复用模式
	
	TIM_InternalClockConfig(TIM2);//选择TIM2为内部时钟，若不调用此函数，TIM默认也为										内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频，选									择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模																式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;	//计数周期，ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;//预分频器，PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;//重复计数器，高级定															时器才会用到
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseInitStructure);//将结构体变量交给											TIM_TimeBaseInit，配置TIM2的时基单元
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//结构体初始化，若结构体没有完整赋值，则									最好执行此函数，给结构体所有成员都赋一个默认值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//输出比较模式选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性，选择为												高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//CCR
	TIM_OC3Init(TIM2 , &TIM_OCInitStructure);//将结构体变量交给TIM_OC3Init，配置												TIM2的输出比较通道3
	
	TIM_Cmd(TIM2 , ENABLE);//使能TIM2，定时器开始运行
	
}

void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM2 , Compare);
}

```

main.c

```c
#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "OLED.h"
#include "Delay.h" 
#include "PWM.h"
#include "LED.h"

uint16_t Num;
uint16_t i;

/*
1.使用RCC开启GPIO时钟
2.使用GPIO_Init函数初始化GPIO
3.使用各类函数控制GPIO

*/
int main(void)
{
	OLED_Init();
	PWM_Init();
	
	while (1)
	{
		for (i = 0; i <= 100; i++)
		{
			PWM_SetCompare3(i);			//依次将定时器的CCR寄存器设置为0~100，PWM占空比逐渐增大，LED逐渐变亮
			Delay_ms(20);				//延时10ms
		}
		for (i = 0; i <= 100; i++)
		{
			PWM_SetCompare3(100 - i);	//依次将定时器的CCR寄存器设置为100~0，PWM占空比逐渐减小，LED逐渐变暗
			Delay_ms(20);				//延时10ms
		}
	}
}

```

### 8.ADC

#### ADC简介

- **ADC（Analog-Digital Converter）模拟-数字转换器**
- ADC可以将**引脚上连续变化的模拟电压**转换为**内存中存储的数字变量**，建立模拟电路到数字电路的桥梁
- 输入电压范围：**0 \~ 3.3**，转换结果范围：**0 ~ 4095**
- 18个输入通道，可测量16个外部和2个内部信号源
- 规则组和注入组两个转换单元模拟看门狗自动监测输入电压范围

> 注入通道【使用不多】：最多一次性选4路通道，配合4个16位寄存器，就可以一次性转换4路模拟数据。
> 规则通道【常用】：最多一次性选16路通道，但只有1个16位寄存器，存在新来的数据覆盖上一个数据的问题，此时要么尽快将数据取走，要是使用DMA帮助转运数据，进而可以实现一次性转换16路模拟数据。当然，一次就选一个通道，就是普通的ADC功能。

- STM32F103C8T6 ADC资源：ADC1、ADC2，10个外部输入通道
- 模拟看门狗自动监测输入电压范围，当AD值高于上阈值或低于下阈值时，就会申请中断，可减轻软件负担。

#### ADC电路结构

stm32中ADC采用了逐次逼近型ADC结构，下面以ADC0809芯片为例介绍这种结构：

![1742193330926](E:\笔记\STM32\STM32笔记\.assets\1742193330926.jpg)

> ADC0809是一个独立的8位逐次逼近型ADC，单片机内部没有集成ADC时需要外挂ADC芯片，ADC0809就是这么一款经典的ADC芯片。现在很多单片机内部已经集成了ADC外设，就不需要外挂芯片，可以直接测量电压。
>
> - IN7\~IN0：8路模拟输入。
> - ADDA、ADDB、ADDC、ALE：地址锁存，选择当前的模拟输入引脚。相当于38译码器。
> - CLOCK：时钟线。
> - START：开始AD转换。
> - EOC：转换结束标志位。
> - 内部DAC：加权电阻网络，用于产生和输入模拟信号进行比较的模拟信号。
> - OE：输出使能，控制三态门输出。
> - D7\~D0：输出的8位数字信号。
> - V~REF(+)~、V~REF(-)：参考电压。

#### ADC框图

![1730032959740](E:\笔记\STM32\STM32笔记\.assets\1730032959740.jpg)

> - ADCx_IN0\~ADCx_IN15、温度传感器、V~REFINT~：ADC的16个输入通道。
> - 注入通道【使用不多】：最多一次性选4路通道，配合4个16位寄存器，就可以一次性转换4路模拟数据。
> - 规则通道【常用】：最多一次性选16路通道，但只有1个16位寄存器，存在新来的数据覆盖上一个数据的问题，此时要么尽快将数据取走，要是使用DMA帮助转运数据，进而可以实现一次性转换16路模拟数据。当然，一次就选一个通道，就是普通的ADC功能。
> - 触发转换电路：stm32中的ADC触发方式：
>
> > - 软件触发：在程序中手动调一句代码。
> > - 硬件触发：上图所示的触发源。主要来自于定时器TIMx，也可以外部中断引脚EXTI。正常思路是：定时器每隔1ms产生一次中断 --> 中断函数中开启触发转换信号 --> ADC完成一次转换。缺点是需要频繁进入中断，消耗软件资源。但是得益于上图的硬件电路设计，stm32可以直接使用定时器主模式触发ADC转换，硬件全自动无需申请中断，可以极大地减轻CPU负担。
>
> - V~DDA~、V~SSA~：ADC的供电引脚。
> - V~REF+~、V~REF-~：ADC的参考电压，决定了ADC的输入电压的范围。stm32内部已经和V~DDA~、V~SSA~连接在一起了。
> - ADCCLK：来自ADC的预分频器，这个ADC的预分频器则来自于“RCC时钟树”。具体可以查看时钟树的电路，默认情况就是对72MHz进行ADC预分频，由于ADCCLK最大18MHz，所以只能选择6分频/8分频。
> - DMA请求：触发DMA进行数据转运。下一章讲。
> - 注入通道数据寄存器、规则通道数据寄存器：用于存放转换结果。
> - 模拟看门狗：一旦高于上阈值或低于下阈值，就会申请模拟看门狗的中断，最终进入NVIC。
> - 转换结束EOC：规则通道转换完成，会在状态寄存器置标志位。
> - 注入转换结束JEOC：注入通道转换完成，会在状态寄存器置标志位。
> - NVIC：嵌套向量中断控制器，控制是否响应上面这三个中断。

#### ADC基本结构

![1730033010296(1)](E:\笔记\STM32\STM32笔记\.assets\1730033010296(1).jpg)

#### 输入通道

|  通道  | ADC1 | ADC2 |
| :----: | :--: | :--: |
| 通道0  | PA0  | PA0  |
| 通道1  | PA1  | PA1  |
| 通道2  | PA2  | PA2  |
| 通道3  | PA3  | PA3  |
| 通道4  | PA4  | PA4  |
| 通道5  | PA5  | PA5  |
| 通道6  | PA6  | PA6  |
| 通道7  | PA7  | PA7  |
| 通道8  | PB0  | PB0  |
| 通道9  | PB1  | PB1  |
| 通道10 | PC0  | PC0  |
| 通道11 | PC1  | PC1  |
| 通道12 | PC2  | PC2  |
| 通道13 | PC3  | PC3  |
| 通道14 | PC4  | PC4  |
| 通道15 | PC5  | PC5  |

#### 规则组的转换模式

stm32的ADC最多同时支持16个通道，那么ADC每次扫描1个通道还是多个通道，便是选择 **非扫描模式/扫描模式**；而对于单个通道的ADC转换来说，触发一次ADC是只转换一次，还是自动的进行连续转换，便是选择 **单次转换/连续转换**。上面这两种选择进行组合，便产生了 **规则组的4种转换模式**：

**1. 单次转换、非扫描模式：**
触发一次仅转换一次；仅序列1有效，但可以任意指定需要转换的通道。此时ADC选择一组的方式退化成只能选择一个。读取数据时，需要等待EOC标志位置1，然后从数据寄存器读取结果。如要再进行转换，就需要再次触发转换。

![1742194072239](E:\笔记\STM32\STM32笔记\.assets\1742194072239.jpg)

**2. 连续转换、非扫描模式：**
相比于上一个模式，仅需要一次触发，ADC就会在一次转换完成后立刻进入下一次转换，实现不断地自动进行转换。此时就不需要读EOC看转换是否完成，直接想读数据的时候就读。

![1742194110944](E:\笔记\STM32\STM32笔记\.assets\1742194110944.jpg)

**3. 单次转换、扫描模式：**
相比于第一种模式，可以一次性转换多个通道，不过还是触发一次、所有通道只转换一次。

![1742194147543](E:\笔记\STM32\STM32笔记\.assets\1742194147543.jpg)

**4. 连续转换、扫描模式：**
不仅可以一次性转换多个通道，还可以实现触发一次、自动不间断转换。

![1742194179828](E:\笔记\STM32\STM32笔记\.assets\1742194179828.jpg)

#### 触发控制

![1730033167134](E:\笔记\STM32\STM32笔记\.assets\1730033167134.jpg)

> 要想ADC进行转换，还需要完成 **触发** 这个操作。触发信号可以是 软件触发、硬件触发。**软件触发**可以由ADC的库函数完成；**硬件触发**见上图。

#### 数据对齐

![1742194290697(1)](E:\笔记\STM32\STM32笔记\.assets\1742194290697(1).jpg)

因为ADC是12位的，而寄存器宽度为16位，所有便有了数据对齐方式的选择。

> 1. 右对齐【常用】：读出的值就是实际值。
> 2. 左对齐：有时候不需要太大的分辨率，便将12位ADC的转换数据左对齐，然后只取高8位。

#### 转换时间

- AD转换的步骤：**采样，保持，量化，编码**

- STM32中ADC的总转换时间为：**T~CONV~ = 采样时间 + 12.5个ADC周期**

  > - 采样时间：在配置ADC的多路选择开关时可选，是ADC采样周期的倍数，如1.5倍、7.5倍、13.5倍、……、239.5倍。
  >
  > - ADC周期：就是从RCC分频过来的RCC_CLK(最高14MHz)，总采样时间不会小于$1\mu s$。
  >
  > 例如：当ADCCLK=14MHz，采样时间为1.5个ADC周期，则T~CONV~ = 1.5 + 12.5 = 14个ADC周期 = 1μs

#### 校准

- **ADC有一个内置自校准模式**。**校准可大幅减小因内部电容器组的变化而造成的准精度误差**。校准期间，在每个电容器上都会计算出一个误差修正码(数字值)，这个码用于消除在随后的转换中每个电容器上产生的误差
- 建议在每次上电后执行一次校准
- 启动校准前， ADC必须处于关电状态超过至少两个ADC时钟周期

#### ADC外围电路设计

![1742194477567](E:\笔记\STM32\STM32笔记\.assets\1742194477567.jpg)

#### ADC单通道转换驱动代码

AD.c

```c
void AD_Init(void)
{
    //1.开启外部时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//选择时钟6分频，ADCCLK = 72MHz / 6 = 12MHz
	//2.配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);//将PA3引脚初始化为模拟输入
	//3.配置多路开关，选择通道进入规则组
	ADC_RegularChannelConfig(ADC1 , ADC_Channel_3 , 1 , ADC_SampleTime_55Cycles5);//规则组序列1的位置，配置为通道0
	//4.配置ADC转换器
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换，使能，每转换一次规则组序列后继续转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐，选择右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发，使用软件触发，不需要外部触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//选择独立模式，即单独使用ADC1
	ADC_InitStructure.ADC_NbrOfChannel = 1;//通道数，为1，仅在扫描模式下，才需要指定大于1的数，在非扫描模式下，只能是1
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//扫描模式，失能，只转换规则组的序列1这一个位置
	ADC_Init(ADC1 , &ADC_InitStructure);//将结构体变量交给ADC_Init，配置ADC1
	//5.配置开关控制
	ADC_Cmd(ADC1 , ENABLE);
	//6.进行ADC校准
	ADC_ResetCalibration(ADC1);//固定流程，内部有电路会自动执行校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	//7.对ADC进行软件触发
	ADC_SoftwareStartConvCmd(ADC1 , ENABLE);//软件触发AD转换一次
}

uint16_t AD_GetValue(void)
{
	return ADC_GetConversionValue(ADC1);//读数据寄存器，得到AD转换的结果
}
```

main.c

```c
#include "stm32f10x.h"    // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

float Voltage;
uint16_t ADValue;

/*
1.使用RCC开启GPIO时钟
2.使用GPIO_Init函数初始化GPIO
3.使用各类函数控制GPIO

*/

int main(void)
{
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1 , 1 , "ADValue:");
	OLED_ShowString(2 , 1 , "Voltage:0.00V");

	while(1)
	{
		ADValue = AD_GetValue();//获取AD转换的值
		Voltage = (float) ADValue / 4095 * 3.3;//将AD值线性变换到0~3.3的范围，表示电压
		
		OLED_ShowNum(1, 9, ADValue, 4);
		OLED_ShowNum(2, 9, Voltage, 1);//显示电压值的整数部分
		OLED_ShowNum(2, 11, (uint16_t)(Voltage *100) % 100 , 2);//显示电压值的小数部分
		
		Delay_ms(100);//延时100ms，手动增加一些转换的间隔时间
		
	}
}

```

#### ADC多通道转换驱动代码

AD.c

```c
void AD_Init(void)
{
    //1.开启外部时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//2.配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
    
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure1);
	//3.配置ADC转换器
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1 , &ADC_InitStructure);
	//4.配置ADC开关控制
	ADC_Cmd(ADC1 , ENABLE);
	//5.进行ADC校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	
}

uint16_t AD_GetValue(uint16_t ADC_Channel)
{
	ADC_SoftwareStartConvCmd(ADC1 , ENABLE);//ADC软件触发
	ADC_RegularChannelConfig(ADC1 , ADC_Channel , 1 , ADC_SampleTime_55Cycles5);//在每次转换前，根据函数形参灵活更改规则组的通道1
	
	while(ADC_GetFlagStatus(ADC1 , ADC_FLAG_EOC) == RESET);//等待转换完成并读取，硬件会自动清除EOC标志位
	return ADC_GetConversionValue(ADC1);
}
```

main.c

```c
#include "stm32f10x.h"    // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

float Voltage;
uint16_t ADValue;
uint16_t AD0 , AD1 , AD2;

/*
1.使用RCC开启GPIO时钟
2.使用GPIO_Init函数初始化GPIO
3.使用各类函数控制GPIO

*/

int main(void)
{
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1 , 1 , "AD0:");
	OLED_ShowString(2 , 1 , "AD1:");
	OLED_ShowString(3 , 1 , "AD2:");
	OLED_ShowString(4 , 1 , "AD3:");

	while(1)
	{
		AD0 = AD_GetValue(ADC_Channel_3);
		AD1 = AD_GetValue(ADC_Channel_6);
		AD2 = AD_GetValue(ADC_Channel_9);
		
		OLED_ShowNum(1 , 5 , AD0 , 4);
		OLED_ShowNum(2 , 5 , AD1 , 4);
		OLED_ShowNum(3 , 5 , AD2 , 4);
		
		Delay_ms(100);
		
	}
}
```

#### 注意事项

> 1. 关于函数命名。注意stm32库函数已经有了ADC打头的库函数了，如```ADC_Init()```，所以命名的时候不要再使用ADC，而可以使用AD.c。
> 2. GPIO配置成模拟输入AIN模式。AIN模式下，GPIO口无效，可以防止GPIO的输入输出对模拟电压造成干扰。AIN模式是ADC的专属模式。实际测试中，浮空输入、上拉输入、模拟输入的展示效果几乎没有区别（但是硬件原理完全不同）。
> 3. 函数提示设置：找到扳手图标—->Text Completion栏—->把Show Code Completion List For下面的框全部勾上。
> 4. 读取规则组数据后，无需软件清除EOC标志位。参考手册中说明，读取ADCC_DR就会自动清除EOC标志位。所以参考手册还是非常重要！！
> 5. 关于数据抖动。实测发现ADC转换后的结果会抖动，若想消除这种现象，可以有以下几种方法：
>
> > - 迟滞比较：设置两个阈值，低于下阈值执行操作，高于上阈值执行操作。
> > - 滤波：如均值滤波（LPF）。
> > - 裁剪分辨率：去除转换结果的最后抖动的几位。

### 9.DMA

- DMA（Direct Memory Access）直接存储器存取可以直接访问STM32内部的存储器，包括外设寄存器（一般指外设的数据寄存器DR，如ADC的数据寄存器、串口数据寄存器等）、运行内存SRAM（存储运行变量）、程序存储器FLASH（存储程序代码）等。
- DMA可以提供外设和存储器或者存储器和存储器之间的高速数据传输，无须CPU干预，节省了CPU的资源
- 12个独立可配置的通道： DMA1（7个通道）， DMA2（5个通道）。每个通道都支持软件触发和特定的硬件触发

> - **软件触发**应用场景：数据源中的数据已确定。如将FLASH中的数据转运到SRAM中（存储器-->存储器），一次触发后会将数据以最快的速度全部转运完毕。
> - **硬件触发**应用场景：数据源中的数据没有全部确定，需要在特定的时机转运数据。如将ADC数据转运到存储器（外设寄存器-->存储器），等对应ADC通道的数据转换完成后，才硬件触发一次DMA转运数据。

- STM32F103C8T6 DMA资源：DMA1（7个通道）

#### 存储器映像

| 类型 |  起始地址   |       存储器        |                 用途                 |
| :--: | :---------: | :-----------------: | :----------------------------------: |
| ROM  | 0x0800 0000 | **程序存储器Flash** |      存储C语言编译后的程序代码       |
| ROM  | 0x1FFF F000 |     系统存储器      | 存储BootLoader（刷机），用于串口下载 |
| ROM  | 0x1FFF F800 |      选项字节       |   存储一些独立于程序代码的配置参数   |
| RAM  | 0x2000 0000 |  **运行内存SRAM**   |       存储运行过程中的临时变量       |
| RAM  | 0x4000 0000 |     外设寄存器      |        存储各个外设的配置参数        |
| RAM  | 0xE000 0000 |   内核外设寄存器    |      存储内核各个外设的配置参数      |

> - 程序存储器FLASH：下载程序的位置，程序一般也是从主闪存里开始运行。若某变量地址为0x0800_xxxx，那么它就是属于主闪存的数据。
> - 系统存储器：存储BootLoader程序（俗称“刷机”），芯片出厂时自动写入，一般不允许修改。
> - 选项字节：存储的主要是FLASH的读保护、写保护、看门狗等配置。下载程序可以不刷新选项字节的内容，从而保持相应配置不变。
> - 运行内存SRAM：在程序中定义变量、数组、结构体的地方，类似于电脑的内存条。
> - 外设寄存器：初始化各种外设的过程中，最终所读写的寄存器就属于这个区域。
> - 内核外设寄存器：就是NVIC和SysTick。由于不是同一个厂家设计，所以专门留出来<u>内核外设</u>的地址空间，和<u>其他外设</u>的地址空间不一样。
>
> 注：由于stm32是32位的系统，所以寻址空间最大可达4GB（每个地址都代表1Byte），而stm32的存储器硬件最多也就是KB级别的，所以实际上4GB的寻址空间使用率远远低于1%。
> 注：上表中前三者存储介质都是FLASH，但是一般说“FLASH”就是代指“主闪存FLASH”，而不是另外两块区域。
> 注：上表中后三者存储介质是SRAM，但是一般将“SRAM”就是代指“运行内存”，“外设寄存器”就直接叫“寄存器”。

#### DMA框图

![1732336504349(1)](E:\笔记\STM32\STM32笔记\.assets\1732336504349(1).jpg)

> 将从以下加粗的四大部分介绍DMA的电路结构。
>
> - **总线矩阵：** 为了高效有条理的访问存储器，设置了总线矩阵。<u>左端是主动单元</u>，拥有存储器的访问权；<u>右端是被动单元</u>，它们的存储器只能被左端的主动单元读写。
>
> > - 总线矩阵内部的仲裁器：如果DMA和CPU都要访问同一个目标，那么DMA就会暂停CPU的访问，以防止冲突。但是总线仲裁器仍然会保证CPU得到一半的总线带宽，以确保CPU正常工作。
>
> - **主动单元：**
>
> 1. Cortex-M3核心（左上角）：包含了CPU和内核外设。<u>剩下的所有东西都可以看成是存储器</u>，比如Flash是主闪存、SRAM是运行内存、各种外设寄存器也都可以看成是一种SRAM存储器。
> 2. ICode总线：指令总线。加载程序指令。
> 3. DCode总线：数据总线，专门用来访问Flash。
> 4. 系统总线：是访问其他东西的。
> 5. DMA总线：用于访问各个存储器，包括DMA1总线（7个通道）、DMA2总线（5通道）、以太网外设的私有DMA总线。由于DMA要转运数据，所以**DMA也有访问的主动权**。
> 6. DMA1、DMA2：各个通道可以分别设置转运数据的源地址和目的地址，所以**各个通道可以独立的进行数据转运**。
>
> > > - 仲裁器：调度各个通道，防止产生冲突。虽然多个通道可以独立地转运数据，但是DMA总线只有一条，所以所有的通道都只能 **分时复用** 这一条DMA总线，若通道间产生冲突，就会由仲裁器根据通道的优先级决定使用顺序。
> > > - AHB从设备：用于配置DMA参数，也就是DMA自身的寄存器。DMA的外设配置寄存器直接连接在了被动单元侧的<u>AHB总线</u>上。所以DMA既是总线矩阵上的主动单元，可以读写各种寄存器；同时也是AHB总线上的被动单元。CPU配置DMA的线路：“系统”总线-->总线矩阵-->AHB总线-->DMA中的AHB从设备。
>
> - **被动单元：**
>
> 1. Flash：主闪存，只读存储器。若直接通过总线访问(无论是CPU还是DMA)，都只能读取数据而不能写入。若DMA的目的地址为FLASH区域，那么转运就会出错。要想对Flash写入，可以通过“Flash接口控制器”。
> 2. SRAM：运行内存，通过总线可以任意读写。
> 3. 各种外设寄存器（右侧两个方框）：需要对比参考手册中的描述，这些寄存器的类型可能为 只读/只写/读写。不过日常主要使用的数据寄存器，都是可以正常读写的。
>
> - **DMA请求：** 用于硬件触发DMA的数据转运。“请求”就是“触发”的意思，此线路右侧的触发源是各个外设，所以这个“DMA请求”就是 **DMA的硬件触发源**，如ADC转换完成、串口接收到数据等信号。

从上面DMA的电路介绍中，不难看出**寄存器是一种特殊的存储器**，寄存器的两大作用：

> 1. 存储数据。被CPU或DMA等读写，就像读写运行内存一样。
> 2. 控制电路。寄存器的每一位都接了一个导线，可以用于控制外设电路的状态，比如置引脚的高低电平、导通或断开开关、切换数据选择器，或者多位结合起来当做计数器、数据寄存器等。

所以寄存器是连接软件和硬件的桥梁，<u>软件读写寄存器，就相当于在控制硬件的执行</u>。既然外设相当于寄存器，寄存器又是存储器，那么<u>使用DMA转运数据，本质上就是从某个地址取数据，再放到另一个地址去。</u>

#### DMA基本结构

![1732336552478](E:\笔记\STM32\STM32笔记\.assets\1732336552478.jpg)

> 前面的“DMA电路框图”只是一个笼统的结构图，没有展现出对于DMA内部的执行细节。而上图“DMA基本结构”则可用于代码编写时的思路参考，以实现控制DMA工作。
>
> - 外设寄存器站点、存储器站点（Flash和SRAM）：数据转运的两大站点。“外设站点”的参数不一定是外设，“存储器站点”的参数也不一定是存储器。这两个站点的名字只是图一乐，真正表示的意思就是发送端和接收端的参数。可以看到图中由 **三类数据转运线路**。注意Falsh一般只读，所以不存在 “SRAM到Flash”或“Falsh到Flash”的线路。
>
> > 注：虽然名字“图一乐”，但在stm32手册中，“存储器”一般特指“Flash”和“SRAM”；“外设”一般特指“外设寄存器”。
>
> 下面的7个参数都属于DMA的初始化结构体：
>
> 1. 方向：指明“外设寄存器站点”是发送端还是接收端。
>
> - 站点参数：
>
> > 2. 起始地址：配合“方向”的设置，两个起始地址指明了发送端地址、接收端地址。
> > 3. 数据宽度：指定一次转运的数据宽度，可选字节Byte(8位)、半字HalfWord(16位)、字Word(32位)。比如ADC转换的数据位宽是16位，就需要选择数据宽度为“半字”。
> > 4. 地址是否自增：一次转运完成后，下一次转运是否地址自增。如ADC扫描模式使用DMA进行数据转运，数据源是ADC_DR寄存器，显然不需要地址自增；而数据目的地是存储器，就需要地址自增，以防止数据覆盖。
>
> 5. 传输计数器：是一种自减计数器，指定总共需要转运几次。转运结束后，之前自增的地址也会恢复成起始地址，以方便新一轮的转换。**参考手册规定，写“传输计数器”时必须使用“开关控制”关闭DMA。**
> 6. 自动重装器：传输计数器自减到0后，是否重装到最初的值继续转运，也就决定了两种转运模式：单次模式（不重装）、循环模式（重装）。例如想转运一个数组，就是单次模式；如果想转运ADC扫描模式+连续转换，此时DMA就需要循环模式。
> 7. M2M(Memory to Memory)：DMA的触发源选择器，设置为1选择软件触发、设置为0选择硬件触发。
>
> > - DMA的软件触发(连续触发)：和之前不同，DMA的软件触发无需手动触发。选择软件触发(M2M=1)后，若使能DMA，那么DMA会自动以最快的速度连续触发，尽快完成本轮转换。**软件触发不能和循环模式同时使用**，防止DMA停不下来。<u>软件触发常用于存储器到存储器的转运。</u>
> > - DMA的硬件触发(单次触发、使用更多)：硬件触发源可以选择ADC、串口、定时器等，由相应的外设库函数使能对应的硬件触发源。一般都是<u>与外设有关的转运使用硬件触发</u>，这些转运都需要特定的时机，如ADC转换完成、串口收到数据、定时时间到等。
>
> - 开关控制：即```DMA_Cmd```函数，用于使能DMA。
>
> 总结：DMA转运的必须条件：开关控制使能、传输计数器大于零、有触发源。

#### DMA请求

![1732336617533](E:\笔记\STM32\STM32笔记\.assets\1732336617533.jpg)

- EN位：其实就是DMA的开关控制。
- 软件触发需要M2M位置1，硬件触发则需要M2M位置0。

- 硬件触发源：**通道的选择由相应的外设库函数决定**，如ADC库函数```ADC_DMACmd```用于开启通道1、TIM库函数```TIM_DMACmd```可以开启通道2(TIM2_UP)。理论上可以同时开启同一通道的多个触发源，但一般只开启一个。
- 软件触发源：软件触发通道可以任意选择

#### 数据宽度与对齐

- 源端宽度 < 目的宽度：目的宽度的高位补0。（右对齐）
- 源端宽度 > 目的宽度：按照目的宽度，只保留源端的低位，多余的高位全部舍弃。（右对齐）

#### DMA数据转运

任务是将SRAM数组DataA转运到另一个SRAM数组DataB（存储器到存储器）。

![1742199207531](E:\笔记\STM32\STM32笔记\.assets\1742199207531.jpg)

> 下面给出各参数的配置说明：
>
> - 两个站点的参数：外设地址->DataA数组首地址、存储器地址->DataB数组首地址；数据宽度都是8位；为保证数据的一一对应，“外设”和“存储器”都设置为地址自增。
> - “方向”参数：默认是“外设”-->“存储器”，当然也可以将方向反过来。
> - 传输计数器：数组大小为7，所以计数器为7。
> - 自动重装：不需要。
> - 触发源：软件触发。由于是“存储器->存储器”的触发，所以不需要等待转运时机。
> - 开关控制：最后调用```DMA_Cmd```开启DMA转运。
>
> 注：上述为“**复制转运**”，转运完成后DataA数据不会消失。

#### DMA+ADC扫描模式

- 期望将外设ADC多通道（扫描模式）的数据，依次搬运到SRAM中的ADValue数组中（外设到存储器）。
- 注意下图左侧给出了ADC的扫描模式示意图，ADC每触发一次，7个通道依次进行ADC数据转换，每个通道转换完成时，都会将转换结果放到ADC_DR数据寄存器中，也就是**ADC的所有通道共用一个ADC数据寄存器**。所以每个通道转换完成后，都需要DMA立即转运一次，防止新来的通道数据将之前的通道数据覆盖掉。

![1732336777703](E:\笔记\STM32\STM32笔记\.assets\1732336777703.jpg)

- 一般来说，**DMA最常见的用途就是配合ADC的扫描模式**。因为ADC的扫描模式有数据覆盖的特征，或者说这个数据覆盖的问题是ADC固有的缺陷，这个缺陷使得ADC和DMA成为了最常见的伙伴。ADC对DMA的需求非常强烈，其他外设使用DMA可以提高效率，属于是锦上添花的操作，不使用DMA顶多只是损失一些性能；但是ADC的扫描模式如果不使用DMA，功能都会受到很大的限制。所以**ADC和DMA的结合最为常见**。

#### 实验1：DMA数据转运：存储器到存储器

- main.c

```c
uint8_t DataA[] = {0x11 , 0x22 , 0x33 , 0x44};
uint8_t DataB[] = {0x00 , 0x00 , 0x00 , 0x00};
uint8_t a = 0x11;//存储在运行内存SRAM中
/*stm32f103c8t6中有64KB的Flash,20KB的SRAM。所以有一些不需要更改但又很大的数组，就可以定义为const常量（如字模库等），存储在Flash中，减小SRAM空间占用*/
const uint8_t b = 0x22;//存储在Flash中

 
int main(void)
{
	OLED_Init();
	/*对一个变量取地址后，会存放到一个指针变量中，若不加强制类型转换，就是指针跨级赋值，编译会报错。*/
	DMA_USER_Init((uint32_t)&DataA , (uint32_t)&DataB , 4);//三个参数依次是转运起点的地址，转运终点的地址，和传输计数器的数量
	
	/*验证存储器映像
	OLED_ShowHexNum(1 , 1 , a , 2);
	OLED_ShowHexNum(1 , 4 , (uint32_t)&a , 8);//SRAM地址是0x2000开头
	OLED_ShowHexNum(2 , 1 , b , 2);
	OLED_ShowHexNum(2 , 4 , (uint32_t)&b , 8);//Flash地址是0x0800开头
	OLED_ShowHexNum(3 , 1 , (uint32_t)&ADC1->DR , 8);//ADC外设寄存器地址
	*/
	
	/*初始显示数组AB*/
	OLED_ShowHexNum(1 , 1 , DataA[0] , 2);
	OLED_ShowHexNum(1 , 4 , DataA[1] , 2);
	OLED_ShowHexNum(1 , 7 , DataA[2] , 2);
	OLED_ShowHexNum(1 , 10 , DataA[3] , 2);
	
	OLED_ShowHexNum(2 , 1 , DataB[0] , 2);
	OLED_ShowHexNum(2 , 4 , DataB[1] , 2);
	OLED_ShowHexNum(2 , 7 , DataB[2] , 2);
	OLED_ShowHexNum(2 , 10 , DataB[3] , 2);
	

	while(1)
	{
		/*数组A中的数加1 ， 随后DMA转运数组A的数到数组B*/
		DataA[0]++;
		DataA[1]++;
		DataA[2]++;
		DataA[3]++;
		OLED_ShowHexNum(1 , 1 , DataA[0] , 2);
		OLED_ShowHexNum(1 , 4 , DataA[1] , 2);
		OLED_ShowHexNum(1 , 7 , DataA[2] , 2);
		OLED_ShowHexNum(1 , 10 , DataA[3] , 2);
		Delay_ms(1000);
		
		DMA_Transfer();
		
		OLED_ShowHexNum(2 , 1 , DataB[0] , 2);
		OLED_ShowHexNum(2 , 4 , DataB[1] , 2);
		OLED_ShowHexNum(2 , 7 , DataB[2] , 2);
		OLED_ShowHexNum(2 , 10 , DataB[3] , 2);
		Delay_ms(1000);
	}
}

```

- DMA_USER.c

```c
uint8_t DMA_USER_BufferSize;

void DMA_USER_Init(uint32_t AddrA , uint32_t AddrB , uint32_t BuffSize)
{
	DMA_USER_BufferSize = BuffSize;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向，此为外设作为源端
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//软件触发
	DMA_InitStructure.DMA_BufferSize = BuffSize;//传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//是否使用自动重装
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMA多通道时候才会用到
	
	DMA_Init(DMA1_Channel1 , &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1 , ENABLE);
}

void DMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1 , DISABLE);//DMA重新写入传输计数器数量时候，需要先失能DMA
	DMA_SetCurrDataCounter(DMA1_Channel1 , DMA_USER_BufferSize);
	DMA_Cmd(DMA1_Channel1 , ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET) ;//等待转运完成，转运完成，该标志位置1，跳出While
	DMA_ClearFlag(DMA1_FLAG_TC1);//记得手动清除标志位
}

```

#### 实验2：DMA+AD多通道-外设到存储器

- main.c

```c
#include "stm32f10x.h"    // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

float L_Voltage;
float T_Voltage;
 
int main(void)
{
	uint16_t AD_Value[2] = {0 , 0};
	
	OLED_Init();
	OLED_ShowString(1 , 1 , "Light:+0.00V");
	OLED_ShowString(2 , 1 , "Temper:+0.00V");
	
	AD_Init((uint32_t)&AD_Value , 2);
	
	while(1)
	{
		L_Voltage = (float)AD_Value[0] / 4095 * 3.3;
		T_Voltage = (float)AD_Value[1] / 4095 * 3.3;
		OLED_ShowNum(1 , 8 , L_Voltage , 1);
		OLED_ShowNum(1 , 10 , (uint16_t)(L_Voltage * 100) % 100 , 2);
		OLED_ShowNum(2 , 9 , T_Voltage , 1);
		OLED_ShowNum(2 , 11 , (uint16_t)(T_Voltage * 100) % 100 , 2);
		
		Delay_ms(500);
	}
}
```

- AD.c

```c
#include "stm32f10x.h"                  // Device header


void AD_Init(uint32_t AddrB , uint16_t BufferSize)
{
	//1.开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//2.配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	//3.配置ADC多路开关，选择通道进入规则组
	ADC_RegularChannelConfig(ADC1 , ADC_Channel_0 , 1 , ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1 , ADC_Channel_2 , 2 , ADC_SampleTime_55Cycles5);
	//4.配置ADC转换器
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = BufferSize;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1 , &ADC_InitStructure);
	//5.配置ADC开关控制
	ADC_Cmd(ADC1 , ENABLE);
	//6.进行ADC校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	//7.开启ADC1硬件触发源
	ADC_DMACmd(ADC1 , ENABLE);
	//8.配置DMA
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向，此为外设作为源端
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//硬件触发
	DMA_InitStructure.DMA_BufferSize = BufferSize;//传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//是否使用自动重装
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMA多通道时候才会用到
	DMA_Init(DMA1_Channel1 , &DMA_InitStructure);
	//9.DMA开关控制使能
	DMA_Cmd(DMA1_Channel1 , ENABLE);
	//10.软件触发ADC
	ADC_SoftwareStartConvCmd(ADC1 , ENABLE);
	
}

```

- 不管ADC、DMA的初始化的顺序如何，最关键的一步 **“ADC软件触发”一定要等ADC、DMA初始化都完成后再进行**，也就是一定要放在最后！！
- 关于连续触发。编程时有两种思路，一种是直接ADC+DMA初始化完成，给一个ADC的软件触发信号，就可以直接啥都不用管直接在```while```中显示；另一种是每次进行ADC转换之前，都要进行一次软件触发。值得注意的是，这两种思路和DMA的配置没有任何关系！毕竟ADC的通道转换完成信号是DMA的硬件触发源，所以DMA要设置成自动重装、硬件触发模式就OK。ADC也必须是扫描模式（毕竟多通道），但至于是否 ADC连续转换就可以看个人的喜好了。

### 10.通信接口

- 通信的目的：将一个设备的数据传送到另一个设备，扩展硬件系统
- 通信协议：制定通信的规则，通信双方按照协议规则进行数据收发

| 名称  |         引脚         |  双工  | 时钟 | 电平 |  设备  |
| :---: | :------------------: | :----: | :--: | :--: | :----: |
| USART |        TX、RX        | 全双工 | 异步 | 单端 | 点对点 |
|  I2C  |       SCL、SDA       | 半双工 | 同步 | 单端 | 多设备 |
|  SPI  | SCLK、MOSI、MISO、CS | 全双工 | 同步 | 单端 | 多设备 |
|  CAN  |     CAN_H、CAN_L     | 半双工 | 异步 | 差分 | 多设备 |
|  USB  |        DP、DM        | 半双工 | 异步 | 差分 | 点对点 |

> 注：使用差分信号可以抑制共模噪声，可以极大的提高信号的抗干扰特性，所以一般差分信号的传输速度和传输距离都非常高。

#### 同步时序和异步时序的优缺点

- **同步时序**：优点是对时间要求不严格，可以软件手动翻转电平实现通信，所以可以大幅降低单片机对外围硬件电路的依赖。缺点是多一个时钟线。
- **异步时序：**优点是少一根时钟线，节省资源。缺点是对时间要求严格，对硬件电路的依赖严重。（CPU进中断了，异步时序不知道，还会继续按照约定的速度读取写入，会造成错误）
- **软件手动翻转电平**：所谓软件手动翻转电平，是指通过编写程序，直接对通信接口的控制寄存器或引脚输出电平进行操作，以实现信号的切换和数据传输。这种方法通常用于简单的通信协议，例如通过GPIO（通用输入输出引脚）模拟时钟信号和数据线信号的操作。

#### 四种通信的动态示意

[嵌入式模拟面试拷打【13】：SPI、UART、I2C通信的定义、应用与区别_牛客网](https://www.nowcoder.com/discuss/718485020645552128?sourceSSR=users)

### 11.USART串口通信

#### 串口通信(数据帧)

- 串口是一种应用十分广泛的通讯接口，串口成本低、容易使用、通信线路简单，可实现两个设备的互相通信
- 单片机的串口可以使单片机与单片机、单片机与电脑、单片机与各式各样的模块互相通信，极大地扩展了单片机的应用范围，增强了单片机系统的硬件实力
- 一般单片机中都会有串口的通信外设。在单片机领域中，相比于I^2^C、SPI等协议，串口是一种非常简单的通信接口，只支持**点对点的通信**。
- **单片机与电脑通信**，是串口的一大优势，可以外接电脑屏幕，非常适合调试程序、打印信息……I^2^C或SPI协议一般都是芯片之间的通信，有片选引脚所以支持总线通信，而不会直接外接在电脑上。

#### 电平标准

- 电平标准是数据1和数据0的表达方式，是传输线缆中人为规定的电压与数据的对应关系，串口常用的电平标准有如下三种：

```c
TTL电平：+3.3V或+5V表示1，0V表示0。一般低压小型设备，使用的都是TTL电平。传输范围几十米。
RS232电平：-3~-15V表示1，+3~+15V表示0。一般在大型机器上使用，由于环境比较恶劣，静电干扰比较大，所以通信电压都很大，并且允许波动的范围也很大。传输范围几十			  米。
RS485电平：两线压差+2~+6V表示1，-2~-6V表示0（差分信号）。抗干扰能力极强，通信距离可达上千米。
```

#### 串口参数及时序

- 波特率：串口通信的速率。发送器和接收器的波特率由波特率寄存器BRR里的DIV确定计算公式：波特率 = f~PCLK2/1~ / (16 * DIV)
- 起始位：标志一个数据帧的开始，固定为低电平
- 数据位：数据帧的有效载荷，1为高电平，0为低电平，低位先行
- 校验位：用于数据验证，根据数据位计算得来
- 停止位：用于数据帧间隔，固定为高电平

#### USART简介

- **USART（Universal Synchronous/Asynchronous Receiver/Transmitter）通用同步/异步收发器**
- USART是STM32内部集成的硬件外设，可根据数据寄存器的一个字节数据自动生成数据帧时序，从TX引脚发送出去，也可自动接收RX引脚的数据帧时序，拼接为一个字节数据，存放在数据寄存器里自带波特率发生器，最高达4.5Mbits/s
- 可配置数据位长度（8/9）、停止位长度（0.5/1/1.5/2）可选校验位（无校验/奇校验/偶校验）
- 支持同步模式、硬件流控制、DMA、智能卡、IrDA、LIN

![1742286222045](E:\笔记\STM32\STM32笔记\.assets\1742286222045.jpg)

> 下面介绍一些串口引脚的注意事项：
>
> - TX与RX：简单双向串口通信有两根通信线（发送端TX和接收端RX），要**交叉连接**。不过，若仅单向的数据传输，可以只接一根通信线。
> - GND：一定要共地。由于TX和RX的高低电平都是相对于GND来说的，所以GND严格来说也算是通信线。
> - VCC：相同的电平才能通信，如果两设备都有单独的供电，VCC就可以不接在一起。但如果某个模块没有供电，就需要连接VCC，注意供电电压要按照模块要求来，必要时需要添加电压转换电路。

#### USART框图

![1732523223805](E:\笔记\STM32\STM32笔记\.assets\1732523223805.jpg)

> - 发送数据的过程：某时刻给“TDR”写入数据0x55，此时硬件检测到写入数据，就会检查当前“发送移位寄存器”是否 有数据正在进行移位，若正在移位，就会等待移位完成；若没有移位，就会将TDR中的数据立刻移动到“发送移位寄存器”中，准备发送。然后，“发送移位寄存器”就会在下面的“发送器控制”的驱动下，向右一位一位的移位（低位先行），将数据输出到TX发送引脚。移位完成后，新的数据会再次自动的从TDR转移到“发送移位寄存器”中来。<u>有了TDR和“发送移位寄存器”的双重缓存，可以保证连续发送数据时，数据帧之间不会有空闲。</u>
> - 写入数据的时机：当数据从TDR移动到“发送移位寄存器”时，**标志位TXE**置位（TX Empty，发送寄存器空），此时检测到TXE置位就可以继续写入下一个数据，但注意此时上一个数据实际上还没发送出去。
> - 接收数据的过程：数据从RX引脚通向“接收移位寄存器”，在“接收器控制”的驱动下，一位一位的读取RX电平并放在最高位，整个过程不断右移（低位先行），最终就可以接收1个字节。当一个字节移位完成后，这一个字节的数据就会整体地一下子转移到“接收数据寄存器RDR”中。然后就准备继续读取下一帧数据。<u>同样，RDR和“接收移位寄存器”也组成了双重缓存结构，可以保证连续读取数据。</u>
> - 读取数据的时机：接收数据从“接收移位寄存器”转移到RDR的过程中，**标志位RXNE**置位（RX Not Empty，接收数据寄存器非空）。所以当检测到标志位RXNE置位时，就可以将数据读走。
>
> 下面来看看其他的硬件电路功能：
>
> - 发送器控制：控制“发送移位寄存器”工作。
> - 接收器控制：控制“接收移位寄存器”工作。
> - 硬件数据流控：也就是“硬件流控制”，简称“流控”。如果主设备连续发送，导致从设备内部无法及时处理接收数据，就会出现数据丢弃或覆盖的现象（注意不是UART没接收到，而是从设备没有及时将数据从RDR取走），此时“流控”就可以帮助从设备向主设备发信号，指明自己还没有准备好接收数据，主设备也就不会发送数据了。本教程不涉及。
>
> > - nRTS（Request To Send）：输出脚，请求发送。也就是告诉主设备，当前是否已经准备好接收。前方“n”表示低电平有效。
> > - nCTS（Clear To Send）：输入脚，清除发送。用于接收从设备的nRTS信号。前方“n”表示低电平有效。
>
> - SCLK：用于产生“同步功能”的时钟信号，配合“发送移位寄存器”输出，用于给从设备提供时钟。注意没有同步时钟输入，所以两个USART之间不能同步通信。
> - 唤醒单元：实现串口挂载多设备。前面提到串口一般是点对点通信，但是这个模块通过给串口分配地址“USART地址”，就可以决定是否唤醒USART工作，进而实现了总线通信。
> - **状态寄存器SR**：存储着串口通信的各种标志位，其中比较重要的有发送寄存器空TXE、接收数据寄存器非空RXNE。
> - USART中断控制：中断输出控制，配置中断是否可以通向NVIC。其中断申请位就是状态寄存器SR中的各种标志位。
> - 波特率发生器部分：其实就是分频器，APB时钟进行分频，得到发送和接收移位的时钟。
>
> > - f~PCLKx(x=1,2)~：时钟输入。由于UASRT1挂载在APB2上，所以 时钟输入f~PCLKx(x=1,2)~ 就是PCLK2的时钟，一般为72MHz。而UASRT2、USART3都挂载在APB1上，所以 时钟输入f~PCLKx(x=1,2)~ 就是PCLK1的时钟，一般为36MHz。
> > - /UASRTDIV：时钟分频系数。内部结构也就是虚线框中的“传统的波特率发生器”。
> > - /16：再进行16分频，得到最终的“发送器时钟”、“接收器时钟”。
>
> 注：发送时添加开始位、停止位；接收时去除开始位、停止位，这些工作由内部硬件电路自动完成。
> 注：更多关于控制寄存器CR、状态寄存器SR的描述可以查阅参考手册“25.6 USART寄存器描述”。

#### USB转串口模块

![1742286690803](E:\笔记\STM32\STM32笔记\.assets\1742286690803.jpg)

> 主要关注的是该模块的供电情况。
>
> - USB插座：直接插在电脑USB端口上，注意整个模块的供电来自于USB的VCC+5V。
> - CON6插针座：
>
> > - 引脚2、引脚3：用于连接到stm32上进行串口通信。
> > - 引脚5【CH340_VCC】：通过跳线帽可以选择 **接入+3.3V（stm32）** 或者+5V。CH340芯片的供电引脚，同时决定了TTL，所以也就是串口通信的TTL电平。神奇的是，即使不接跳线帽CH340也可以正常工作，TTL为3.3V，但是显然接上电路以后更加稳定。
> > - 通信和供电的选择：CON6插针座选择引脚4/6进行通信后，剩下的引脚可以用于给从设备供电，但是剩下的这个脚显然与TTL电平不匹配。此时需要注意 **优先保证供电电平的正确**，通信TTL电平不一致问题不大。当然，若从设备自己有电源，那么就不存在这个问题了。
>
> - TXD指示灯、RXD指示灯：若相应总线上有数据传输，那么指示灯就会闪烁。

#### USART基本结构

![1732523257783](E:\笔记\STM32\STM32笔记\.assets\1732523257783.jpg)

> 上图给出USART最主要、最基本的结构：
>
> - 波特率发生器：用于产生约定的通信速率。时钟来源是PCLK2/PCLK1，经过波特率发生器分频后，产生的时钟通向发送控制器、接收控制器。
> - 发送控制器、接收控制器：用于控制发送移位、接收移位。
> - GPIO：发送端配置成复用推挽输出、接收端配置成上拉输入。
> - 标志位：TXE置位时写入数据、RXNE置位时接收数据。
> - 开关控制：用于开启整个USART外设。

#### 数据模式

- HEX模式/十六进制模式/二进制模式：以原始数据的形式显示
- 文本模式/字符模式：以原始数据编码后的形式显示

#### 实验1：串口发送与接收

**main.c**

```c
#include "stm32f10x.h"    // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "SerialPort.h"


int main(void)
{
	OLED_Init();
	SerialPort_Init();
	
	uint16_t num = 0x30;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//发送1字节数据
	SerialPort_SendByte('A');
	SerialPort_SendByte('\r');
	SerialPort_SendByte('\n');
	//发送数组数据
	uint16_t Send_Array[4] = {0x30 , 0x31 , 0x32 , 0x33};
	SerialPort_SendArray(Send_Array , 4);
	SerialPort_SendByte('\r');
	SerialPort_SendByte('\n');
	//发送字符串数据
	SerialPort_SendString("Hello World!\r\n");
	//发送整个数字-先发高位
	SerialPort_SendNum(123456 , 6);
	
	//
	uint8_t RxByte = 0;
	OLED_ShowString(1 , 1 , "RxByte:");
	while(1)
	{
		//循环发送数字
		SerialPort_SendByte(num);
		OLED_ShowHexNum(1 , 1 , num , 2);
		num++;
		Delay_ms(1000);
		if(SerialPort_GetRxFlag() == 1)
		{
			RxByte = SerialPort_GetRxData();
			OLED_ShowHexNum(1 , 8 , RxByte , 2);
			SerialPort_SendByte(RxByte);
		}
		
	}
}
```

SerialPort.c

```c
#include "stm32f10x.h"                  // Device header

void SerialPort_Init(void)
{
	//1.开启RCC外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	//3.初始化USART结构体
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1 , &USART_InitStructure);
	//4.配置中断，开启NVIC（接收数据时候用）
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	//5.开启USART控制
	USART_Cmd(USART1 , ENABLE);
	//6.中断输出配置
	USART_ITConfig(USART1 , USART_IT_RXNE , ENABLE);
}

//发送字节
void SerialPort_SendByte(uint8_t SendByte)
{
	//向发送寄存器写入数据
	USART_SendData(USART1 , SendByte);
	//确认数据被转移到发送移位寄存器
	while(USART_GetFlagStatus(USART1 , USART_FLAG_TXE) == RESET);
}
//发送数组
void SerialPort_SendArray(uint16_t *SendArray , uint8_t SendSize)
{ 
	uint8_t i = 0;
	for(i = 0 ; i < SendSize ; i ++)
	{
		SerialPort_SendByte(SendArray[i]);
	}
}
//发送字符串
void SerialPort_SendString(char *SendString)
{
	uint8_t i;
	for(i = 0 ; SendString[i] != 0 ; i++)
	{
		SerialPort_SendByte(SendString[i]);
	}
}
//X的Y次方（10的幂次方），用来取数字的高位
uint32_t SerialPow(uint32_t X , uint32_t Y)
{
	uint32_t result = 1;
	while(Y--)
	{
		result *= X;
	}
	return result;
}
//发送整个数字
void SerialPort_SendNum(uint32_t SendNum , uint16_t SendSize)
{
	uint8_t i;
	for(i = 0 ; i < SendSize ; i++) 
	{
		SerialPort_SendByte((SendNum/SerialPow(10 , SendSize - i - 1)) % 10 + '0');
	}
}

/*接收数据*/

uint8_t SerialPort_RxData;
uint8_t SerialPort_RxFlag;

uint8_t SerialPort_GetRxFlag(void)
{
	if(SerialPort_RxFlag == 1)
	{
		SerialPort_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t SerialPort_GetRxData(void)
{
	return SerialPort_RxData;
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1 , USART_IT_RXNE) == SET)
	{
		SerialPort_RxFlag = 1;
		SerialPort_RxData = USART_ReceiveData(USART1);
		
		USART_ClearITPendingBit(USART1 , USART_IT_RXNE);
	}
}

```

### 12.I2C通信

#### I2C通信简介

- I2C（Inter IC Bus）是由Philips公司开发的一种**通用数据总线**

以下四点就是学习I2C协议的必要性（串口所不具备的）——[P31的3：00~14：53](https://www.bilibili.com/video/BV1th411z7sn/?p=31&vd_source=b193ce94c6c97ca3ca3bbabe904e8981)

- 两根通信线：SCL（Serial Clock）、SDA（Serial Data）
- 同步，半双工
- 带数据应答
- 支持总线挂载多设备（一主多从、多主多从）

#### 采用I2C协议的部分模块实物图

![1734666443647](E:\笔记\STM32\STM32笔记\.assets\1734666443647.jpg)

注：1.MPU6050（姿态测量） 2.OLED模块 3.存储器模块 4.实时时钟模块

#### 硬件电路

![1734668290183](E:\笔记\STM32\STM32笔记\.assets\1734668290183.jpg)

**NMOS（源极，栅极，漏极）**

- NMOS:在NMOS中，施加电压到栅极时，形成一个导电通道（电流从漏极流向源极）。

**开漏输出模式**

- 其特性是输出级的驱动部分只有一个N型MOSFET（NMOS）
- 当开漏输出的控制信号为低电平时，NMOS导通，输出引脚被拉到低电平。
- 当控制信号为高电平时，NMOS关断，输出引脚悬空（高阻状态），而不是主动输出高电平。

**硬件规定：**

- 硬件电路规定所有I2C设备的SCL连在一起，SDA连在一起。设备的SCL和SDA均要配置成**开漏输出模式**。SCL和SDA各添加一个 **上拉电阻**，阻值一般为4.7KΩ左右。
- CPU：主机，对总线控制权力大。任何时候，都是主机对SCL完全控制。空闲状态下，主机可以主动发起对SDA的控制。
- 被控IC~x~：从机，对总线控制权力小。任何时刻，从机都只能被动读取SCL线。只有在从机发送数据、或者从机发送应答位时，从机才短暂的具有对SDA的控制权。
- 从机设备地址：为了保证通信正常，每个从机设备都具有一个唯一的地址。在I2C协议标准中，从机设备地址分为7位地址、10位地址，其中7位较为简单且应用更广泛。不同的I2C模块出厂时，厂商都会为其分配唯一的7位地址，具体可以在芯片手册中查询，如MPU6050的地址是110_1000，AT24C02的地址是101_0000等，其中器件地址的最后几位是可以在电路中改变的。总线上都是不同模块一般不会有地址冲突，若总线上有相同的模块就需要外界电路来调整相应的器件地址。
- 通信引脚结构：输入线都很正常。输出线则采用开漏输出，引脚下拉是“强下拉（阻值比较小的电阻，提高大电流将引脚快速拉到低电平）”，引脚上拉则处于“浮空状态”。于是所有设备都只能输出低电平，为了避免高电平造成的引脚浮空，就需要在总线外面设置上拉电阻（弱上拉）。

**硬件规定的好处**：

1. 完全杜绝了电源短路的情况，保证了电路的安全
2. 避免了引脚模式的频繁切换。开漏+弱上拉的模式，同时兼具了输入和输出的功能。要想输出就直接操纵总线；要想输入就直接输出高电平，然后读取总线数据，无需专门将GPIO切换成输入模式
3. 此模式可以实现“线与”功能。只有总线上所有设备都输出高电平，总线才是高电平；否则只要有一个设备输出低电平，总线就是低电平。I2C可以利用这个特征，执行多主机模式下的时钟同步（所以SCL也采用此模式）和总线仲裁。

#### **软件规定——时序结构（一主多从）：**

下面的主语都是主机

- **起始条件：**SCL高电平期间，SDA从高电平切换到低电平。
- **终止条件**：SCL高电平期间，SDA从低电平切换到高电平。

![1734688517842](E:\笔记\STM32\STM32笔记\.assets\1734688517842.jpg)

- **发送一个字节**：SCL低电平期间，主机将数据位依次放到SDA线上（**高位先行**），然后释放SCL，从机将在SCL高电平期间读取数据位，所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可发送一个字节。

![1734688778630](E:\笔记\STM32\STM32笔记\.assets\1734688778630.jpg)

- **接收一个字节**：主机在接收之前释放SDA，只控制SCL变化。SCL低电平期间，从机将数据位依次放到SDA线上（高位先行，且一般贴着SCL下降沿变化）；SCL高电平期间，主机读取数据位，所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可接收一个字节。

![1734688840820](E:\笔记\STM32\STM32笔记\.assets\1734688840820.jpg)

- **发送应答**：主机在接收完一个字节之后，在下一个时钟发送一位数据，数据0表示应答，数据1表示非应答。若从机没有收到主机的应答，就会完全释放SDA的控制权，回到待机模式。
- **接收应答**：主机在发送完一个字节之后，在下一个时钟接收一位数据，判断从机是否应答，数据0表示应答，数据1表示非应答（主机在接收之前，需要释放SDA）。

![1734688931843](E:\笔记\STM32\STM32笔记\.assets\1734688931843.jpg)

#### 注意事项

> 要在SCL低电平时候调整SDA，因为SCL在每次操作之后都会拉低，所以确定SCL为低电平，于是先释放SDA，避免当SDA为低电平时先释放SCL造成停止
>
> 除了起始和终止条件，每个时序单元的SCL都是以低电平开始，低电平结束

### 13.Unix时间戳

#### Unix**时间戳**定义

- Unix 时间戳（Unix Timestamp）定义为从UTC（原子钟）/GMT（地球自转一周）的1970年1月1日0时0分0秒开始所经过的秒数，不考虑闰秒
- 时间戳存储在一个秒计数器中，秒计数器为32位/64位的整型变量
- 世界上所有时区的秒计数器相同，不同时区通过添加偏移来得到当地时间

![1735272765231(1)](E:\笔记\STM32\STM32笔记\.assets\1735272765231(1).jpg)

#### 时间戳转换

- C语言的**time.h模块提供了时间获取和时间戳转换的相关函数**，可以方便地进行秒计数器、日期时间和字符串之间的转换

| 函数                                                         | 作用                                   |
| ------------------------------------------------------------ | -------------------------------------- |
| time_t time(time_t*);                                        | 获取系统时钟                           |
| struct tm* gmtime(const time_t*);                            | 秒计数器转换为日期时间（格林尼治时间） |
| struct tm* localtime(const time_t*);                         | 秒计数器转换为日期时间（当地时间）     |
| time_t mktime(struct tm*);                                   | 日期时间转换为秒计数器（当地时间）     |
| char* ctime(const time_t*);                                  | 秒计数器转换为字符串（默认格式）       |
| char* asctime(const struct tm*);                             | 日期时间转换为字符串（默认格式）       |
| size_t strftime(char*, size_t, const char*, const struct tm*); | 日期时间转换为字符串（自定义格式）     |

![1735273006237(1)](E:\笔记\STM32\STM32笔记\.assets\1735273006237(1).jpg)

注：日期时间数据类型中的年是从1900开始的，即2024年在该数据类型中是124。

​	月份是0-11，所以需要月份加1，才能zheng'c

### 14.RTC时钟（Real Time Clock）

#### 备份寄存器（Backup Registers,BKP）

- 他们处在备份域里，当VDD电源被切断，他们仍然由VBAT维持供电。当系统在待机模式下被唤醒，或系统复位或电源复位时，他们也不会被复位。
- 此外，BKP控制寄存器用来管理侵入检测和RTC校准功能。**(PC13,TAMPER,RTC共用一个引脚)**
- 复位后，对备份寄存器和RTC的访问被禁止，并且备份域被保护以防止可能存在的意外的写操作。
- 用户数据存储容量：20字节（c8t6**中容量**和小容量）/ 84字节（大容量和互联型）

注：**如果要使用STM32内部的BKP和RTC，Vbat引脚必须接备用电池，在VDD主电源掉电后进行供电**

#### BKP基本结构

![1735010892051](E:\笔记\STM32\STM32笔记\.assets\1735010892051.jpg)

#### 晶振的补充

- 通常跟RTC有关的晶振都是统一的数值，即32.768kHz.原因如下：

1. 32kHz附近的晶振工艺比较完美
2. 是2^15^，通过自然溢出可以简化电路设计输出1Hz这样的

#### RTC（Real Time Clock）实时时钟简介

- RTC是一个独立的定时器，可为系统提供时钟和日历的功能

- RTC和时钟配置系统处于后备区域，系统复位时数据不清零，VDD（2.0\~3.6V）断电后可借助VBAT（1.8~3.6V）供电继续走时

- 32位的可编程计数器，可对应Unix时间戳的秒计数器

- 20位的可编程预分频器，可适配不同频率的输入时钟

- 可选择三种RTC时钟源：	

  ​	HSE时钟除以128（通常为8MHz/128）	

  ​	LSE振荡器时钟（通常为32.768KHz）(最常用)

  ​	LSI振荡器时钟（40KHz）

#### RTC框图

![1735013850147](E:\笔记\STM32\STM32笔记\.assets\1735013850147.jpg)

#### RTC基本结构

![1735013930060](E:\笔记\STM32\STM32笔记\.assets\1735013930060.jpg)

#### RTC操作事项

- 执行以下操作将使能对BKP和RTC的访问：

​		**设置RCC_APB1ENR的PWREN和BKPEN，使能PWR和BKP时钟**

​		**设置PWR_CR的DBP，使能对BKP和RTC的访问**

- 若在读取RTC寄存器时，RTC的APB1接口曾经处于禁止状态，则**软件首先必须等待RTC_CRL寄存器中的RSF位（寄存器同步标志）被硬件置1**（RTC等待同步，看RTC框图，PCLK1在主电源掉电时会停止，为保证RTC掉电后正常工程，RTC有专门的RTC时钟，由此出现了总线和RTC时钟不同步情况）
- **必须设置RTC_CRL寄存器中的CNF位，使RTC进入配置模式**后，才能写入RTC_PRL、RTC_CNT、RTC_ALR寄存器（正常库函数都帮你写过了，不用单独写）
- **对RTC任何寄存器的写操作，都必须在前一次写操作结束后进行。**可以通过查询RTC_CR寄存器中的RTOFF状态位，判断RTC寄存器是否处于更新中。仅当RTOFF状态位是1时，才可以写入RTC寄存器（等待上一步的完成）

#### 读写备份寄存器代码

main.c

```C
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Key.h"

uint8_t KeyNum;
uint16_t ArrayWrite[] = {0x1234 , 0x6789};
uint16_t ArrayRead[2];

int main(void)
{
	OLED_Init();
	Key_Init();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP , ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	OLED_ShowString(1 , 1 , "W:");
	OLED_ShowString(2 , 1 , "R:");
	
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if(KeyNum == 1)
		{
			ArrayWrite[0]++;
			ArrayWrite[1]++;
			
			OLED_ShowHexNum(1 , 3 , ArrayWrite[0] , 4);
			OLED_ShowHexNum(1 , 8 , ArrayWrite[1] , 4);

			BKP_WriteBackupRegister(BKP_DR1 , ArrayWrite[0]);
			BKP_WriteBackupRegister(BKP_DR2 , ArrayWrite[1]);
		}
		
		ArrayRead[0] = BKP_ReadBackupRegister(BKP_DR1);
		ArrayRead[1] = BKP_ReadBackupRegister(BKP_DR2);
			
		OLED_ShowHexNum(2 , 3 ,ArrayRead[0] , 4);
		OLED_ShowHexNum(2 , 8 ,ArrayRead[1] , 4);
	}
}

```

## 15.指针的详解与应用

#### 1.内存、首地址的理解

​	**内存**是C语言王国的居民们（包括**变量/函数/结构体**等）赖以生存的土地，比如居民char的房子占1字节，也可以理解成1块土地，居民int的房子占4字节，即4块土地。

​	每个居民的房子前面都会贴一个自己的地址，也叫做首地址。**如果你想拜访这个居民，只需要记住他们第一格的首地址就行。**

#### 2.*号的理解

​	当它在int之类的声明后面，它是邮局局长——比如int\* p翻译过来就是，邮局局长对p说“p你从现在开始就是快递员了”。

​	此时p从一个普普通通的变量居民，被\*局长赋予了一份快递员的工作，成为了一名快递员，也就是一个指针变量，**专门负责去记录别人的地址，也就是门牌号。**

​	在被赋予工作之后，\*号这个邮局局长再与p相遇时候，就会接待p一顿，吃个饭，**然后把p携带的门牌地址拿过来，然后按照这个地址，自己前往拜访这个居民。**

#### 3.int , char 等声明的理解

​	理解成*邮局局长手里的**大红印章**，给p授予快递员工作。

​	大红印章也要分种类，就能让快递员记录不一样的门牌地址。比如：int类型的大红印章让快递员找占地4格的房门号，char类型让找占地1格的房门号

#### 4.&号的理解

​	它就是**门牌**（取地址）

​	p = &a;表示变量a居民交给快递员p自己的门牌，欢迎它随时来玩（**通过这个门牌，下次邮局局长可以随时进入a的家里，修改里面的内容）**

​	但是，p = a;这种行为违反了C语言王国的法律。a是普通居民（变量），而p是快递员（指针变量）。C语言王国有规定：**信息传递讲究门当户对**，普通居民不能给快递员储存的内容，只能给自己的地址，否则是跨阶级的禁忌之恋。	

​	不过，如果有一天，*局长批准a居民也成为快递员（升级为指针），**那么p和a是同一阶级，门当户对，p能拿走a储存的东西了。此时a居民只能给p自己存储的内容（自己抄的门牌号）**，不能给自己的地址。除非p升级成二级指针，才能接受a快递员自己家的地址，

​	所以，**给地址其实是一种跨级的行为**。

#### 5.指针移动的理解

​	p被任命为快递员之后，主要工作就是找到门牌地址，然后报告给邮局局长（\*p）

​	找门牌号也是有规律的，比如依次往下找2格内存、4格内存等

​	之前邮局局长告诉p，“听着啊，我给你盖的是int类型的大红印章，所以你是int类快递员，找门牌号的时候每次只能移动4块土地”

#### 6.指针越界的理解

​	若快递员按照人类的错误指示，带回了一个门牌号，但是这个门牌号指的是境外的非法之地，邮局局长按照这个门牌号去访问时候，就进入了非法之地，程序会发送严重报错。

#### 7.野指针与空指针的理解

```c
int *p;
*p;//这种就是野指针，快递员都不知道自己每次需要走几格，为了交差，就干脆随便给个地址	
```

​	当p没活儿时候，为了防止p不安分，\*局长会安排它进入NULL里面暂时休息，这就是空指针

#### 8.数组c[]的理解

​	数组c其实和p这样是快递员小哥一样，都是**记地址的打工人**。

```c
int main()
{
	char c[] = {0x33 , 0x34 , 0x35};
    char *p;
    
    p = c;//朋友，我来看你了
    
    printf("*p = %x\n" , *p);
    printf("*(p+1) = %x\n" , *(p+1));
    printf("*(p+2) = %x\n" , *(p+2));
}
```

代码解析：

​	p这天接到一个任务，你去接替一下c的职务，**c记录了3间连续的门牌号**

​	将c赋值给p，实际上是**读取到第一间房子的门牌号（地址）**。注：p = c，这个等号相当于接过c手中的活

​	**p是char级别的快递员**，所以每走一步，只能读取1间房子。若要读取完c的房子，就得走3步，也就是3格。**p默认读取到的是c[0]**。所以他读取完第一间房时，如果想让他读下一间，就必须+1移动1格查第二间，然后又+1移动1格查第三间。

​	每次读取的时候，将地址汇报给刚过来的*局长，局长前往执行，将实际数据以16进制的方式传达给 printf 等高层领导。

#### 9.malloc的理解

​	malloc是土地监管局的，从<stdlib.h>大学毕业，和邮局有合作关系。它是职务是可以随时随地划分土地（申请内存），划分完土地之后，它就变成了快递员，记录该处的地址，等着\*局长带着印章过来，授予malloc快递员类型，让它盖这种类型的房子。



​	
