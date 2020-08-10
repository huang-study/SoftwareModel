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
// ��ʱ��ö��
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
// ��ʱ���ṹ��
struct sTime
{
	enum sTimEnEnum En;
	uint8_t Count; // ����Ӳ����ʱ��ʱ������ʵ�ʶ�ʱʱ�䣬�磺��ʱ10ms��Count = 10000/TimeBase_us��һ���ϲ�ֻ��Ҫ�ṩ����Ķ�ʱʱ�䣬�ɲ��뺯�����м���
	void (*Func)(void);
};
struct sTimeList
{
	uint32_t count;
	struct sTime sTimFuncList[STIM_LIST_SIZE]; // �����б����湦�ܺ���������ͨ���������
	struct sTime* sTimSeqList[STIM_LIST_SIZE]; // �����б���sTimFuncList���ݶ�ʱ�������򱣴��ڴ˴�
} ;

struct hTimInfo
{
	uint32_t TimeBase_us; // ����ʱ����������΢��Ϊ��λ
	uint32_t TimCnt; // ��ʱ��������Ӳ��ģʽʱÿ���ж϶����õ�һ�����ܺ��������ģʽʱ�Զ�ʱ���б�����һ������ֵ��Ϊ���ֵ
	uint32_t TimCntMax; // ��ʱ��������ֵ����������ʱ������С������
	struct sTimeList sTimList;
	enum TimMode Mode; // ��ʱ��ģʽ����ʧ�ܣ��رն�ʱ������
																		// Ӳ����ʱ����ֻ�е�һ�������Ҷ�ʱʱ����Ӳ����ʱ������Ϊ׼����ͨ���޸Ķ�ʱ��������ʵ����Ҫ�Ķ�ʱʱ�����Ч�ʣ���
																		//���ģʽ����Ҫ�ṩ�Ժ���Ϊ��λ��ʱ����ֵ���ڼ�����ʱʱ��
};
extern struct hTimInfo tim1;
extern struct hTimInfo tim4;

extern TIM_HandleTypeDef htim1; // 10ms ��ʱ��
extern TIM_HandleTypeDef htim2; // ���벶��沶�������ʱ��
extern TIM_HandleTypeDef htim3; // ���벶��
extern TIM_HandleTypeDef htim4; // 3.5ms ��ʱ��

extern void Error_Handler(void);
extern void (*tim2cb)(void);
extern void (*tim2ic1cb)(void);
extern void (*tim3ic2cb)(void);

void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);

// ��ʱ�����ƿ��������
struct hTimInfo* TimInit(struct hTimInfo *tim, uint32_t TimBas, enum TimMode Mod); // ��ʱ����ʼ�������붨ʱ����ʱ������ģʽ
enum TimMode TimModSet(struct hTimInfo *tim, enum TimMode Mod); // ���ö�ʱ��ģʽ���������õ�ģʽ
#define TIM_MOD_GET(tim) (NULL==tim?TIM_DISABLE:tim->Mode)
enum TimMode TimModGet(struct hTimInfo *tim); // ��ȡ��ʱ��ģʽ
uint32_t TimBaseSet(struct hTimInfo *tim, uint32_t TimBase); // ���ö�ʱ��ʱ��
#define TIM_BASE_GET(tim) (NULL==tim?0:tim->TimBase_us)
uint32_t TimBaseGet(struct hTimInfo *tim); // ��ȡ��ʱ��ʱ��
uint32_t CalMaxCnt(struct hTimInfo *tim); // ����������ֵ
// ��ʱ����������
uint32_t sTimIns(struct hTimInfo *tim, uint32_t us, void (*Func)(void), enum sTimEnEnum en); // ������ʱ�����񣬷��ض�ʱ����ţ�0Ϊ����
void sTimDel(struct hTimInfo *tim, uint32_t TimNum); // ɾ����ʱ��
enum sTimEnEnum sTimModSet(struct hTimInfo *tim, uint32_t TimNum, enum sTimEnEnum EN); // ������ʱ��ģʽ
enum sTimEnEnum sTimModGet(struct hTimInfo *tim, uint32_t TimNum); // ��ȡ��ʱ��ģʽ�������ʱ�������ڷ���δʹ��
uint32_t sTimCounSet(struct hTimInfo *tim, uint32_t TimNum, uint32_t count); // ������ʱ����ʱʱ��
uint32_t sTimCounGet(struct hTimInfo *tim, uint32_t TimNum); // ��ȡ��ʱ����ʱʱ��
#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */
