/**
  ******************************************************************************
  * File Name          : TIM.h
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************/
#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#define MPU6050_INT_Pin GPIO_PIN_5
#define MPU6050_INT_GPIO_Port GPIOB
#define MOTOR3_ECO_Pin GPIO_PIN_15
#define MOTOR3_ECO_GPIO_Port GPIOA

#define STIM_LIST_SIZE 10
// 定时器枚举
enum sTimEnEnum
{
	S_TIM_EN,
	S_TIM_DIS
};

enum TimMode
{
	TIM_DISABLE,
	TIM_HARDWARE,
	TIM_SOFTWARE
};
// 定时器结构体
struct sTime
{
	enum sTimEnEnum En;
	uint8_t Count; // 根据硬件定时器时基计算实际定时时间，如：定时10ms，Count = 10000/TimeBase_us，一般上层只需要提供所需的定时时间，由插入函数进行计算
	void (*Func)(void);
};
struct sTimeList
{
	uint32_t count;
	struct sTime sTimFuncList[STIM_LIST_SIZE]; // 功能列表，保存功能函数，可以通过序号索引
	struct sTime* sTimSeqList[STIM_LIST_SIZE]; // 排序列表，将sTimFuncList根据定时长度排序保存在此处
} ;

struct hTimInfo
{
	uint32_t TimeBase_us; // 保存时基参数，以微秒为单位
	uint32_t TimCnt; // 定时器计数，硬件模式时每次中断都调用第一个功能函数，软件模式时以定时器列表的最后一个计数值作为最大值
	uint32_t TimCntMax; // 定时器最大计数值，是所有软定时器的最小公倍数
	struct sTimeList sTimList;
	enum TimMode Mode; // 定时器模式，有失能（关闭定时器），
																		// 硬件定时器（只有第一个能用且定时时间以硬件定时器参数为准，可通过修改定时器驱动来实现需要的定时时间提高效率），
																		//软件模式（需要提供以毫秒为单位的时基数值用于计算延时时间
};
extern struct hTimInfo tim1;
extern struct hTimInfo tim4;

extern TIM_HandleTypeDef htim1; // 10ms 定时器
extern TIM_HandleTypeDef htim2; // 输入捕获兼捕获计数定时器
extern TIM_HandleTypeDef htim3; // 输入捕获
extern TIM_HandleTypeDef htim4; // 3.5ms 定时器

extern void Error_Handler(void);
extern void (*tim2cb)(void);
extern void (*tim2ic1cb)(void);
extern void (*tim3ic2cb)(void);

void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);

// 定时器控制块操作函数
struct hTimInfo* TimInit(struct hTimInfo *tim, uint32_t TimBas, enum TimMode Mod); // 定时器初始化，传入定时器和时基数及模式
enum TimMode TimModSet(struct hTimInfo *tim, enum TimMode Mod); // 设置定时器模式，返回设置的模式
#define TIM_MOD_GET(tim) (NULL==tim?TIM_DISABLE:tim->Mode)
enum TimMode TimModGet(struct hTimInfo *tim); // 获取定时器模式
uint32_t TimBaseSet(struct hTimInfo *tim, uint32_t TimBase); // 设置定时器时基
#define TIM_BASE_GET(tim) (NULL==tim?0:tim->TimBase_us)
uint32_t TimBaseGet(struct hTimInfo *tim); // 获取定时器时基
uint32_t CalMaxCnt(struct hTimInfo *tim); // 计算最大计数值
// 软定时器操作函数
uint32_t sTimIns(struct hTimInfo *tim, uint32_t us, void (*Func)(void), enum sTimEnEnum en); // 插入软定时器服务，返回定时器编号，0为错误
void sTimDel(struct hTimInfo *tim, uint32_t TimNum); // 删除软定时器
enum sTimEnEnum sTimModSet(struct hTimInfo *tim, uint32_t TimNum, enum sTimEnEnum EN); // 设置软定时器模式
enum sTimEnEnum sTimModGet(struct hTimInfo *tim, uint32_t TimNum); // 获取软定时器模式，如果定时器不存在返回未使能
uint32_t sTimCounSet(struct hTimInfo *tim, uint32_t TimNum, uint32_t count); // 设置软定时器定时时长
uint32_t sTimCounGet(struct hTimInfo *tim, uint32_t TimNum); // 获取软定时器定时时长
#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */
