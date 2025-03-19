# STM32学习笔记（Markdown格式）
注：电脑上没有Markdown相关的编辑器，可在微信公众号关注软件管家，下载Typora，下载教程可在公众号查看。

使用注意事项：由于文档中的图片使用的本地图片，需将文件放入以下路径中：
  E:\笔记\STM32（即在E盘中创建一个名为-笔记-的文件下，在该文件夹下再创建一个名为-STM32-的文件夹）

# 学习笔记目录
一、软件安装及创建工程
-
二、模块教程
-
1.GPIO通用输入输出

2.GPIO输出输入实现

3.OLED显示屏

4.光敏传感器

5.外部中断

6.定时器定时中断

7.PWM输出

8.ADC

9.DMA

10.通信接口

11.USART串口通信

12.I2C通信

13.Unix时间戳

14.RTC时钟（Real Time Clock）

三、指针的详解与应用
-
# 智能家居学习项目（文档中连线指导）
- 动调光功能，单片机检测光敏电阻，ADC检测该数值变化，根据此数值调节PWM输出占空比，实现根据环境光照调节灯光亮度。

- 定时开关功能，读取年、月、日、时、分、秒，定时开关灯。

- 场景切换功能，预设场景模式，不同场景模式对应不同的灯光亮度和颜色温度。

- 远程控制功能，单片机串口通信，与手机交互实现远程控制灯光。

- 人体感应功能，用HC-SR501人体红外感应模块，有人灯亮，无人灯灭。

- 温湿度环境变化功能，温湿度传感器，例如空气质量不佳时，降低光线亮度以减少损耗。

