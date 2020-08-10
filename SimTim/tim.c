/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************/
#include "tim.h"
#include "my_math.h"

// Ӳ����ʱ�������ж�ʱ����ʱ����Ϣ�Լ������б���ʱ����ʼ�������������򣨸��ݶ�ʱ���ڣ�����һ����ʱ����������б��еĹ��ܺ���
// ���������б�������б������б��¼����������Ϣ�������б�����ݶ�ʱ���ڳ����������ʱ��ָ�루���ʹ�÷ֶβ��ң�
// ��ʱ��ͨ��˳���ʵ�֣�ͨ����ʼ��ʱ���붨ʱ���ں������б���ϵͳ	��Դ���ٵ������ʹ�ù̶����ȵ����飬������ʹ���ڴ�ѣ�
// δ�����̰߳�ȫ����
// ��ʱ����ʱʱ����Ӳ����ʱ����ʱʱ��Ϊ����������ʱ�����½�

#define JUDGE_STIM_NUM(num) ((num > 0) && (num < (STIM_LIST_SIZE)))


struct hTimInfo tim1;
struct hTimInfo tim4;

TIM_HandleTypeDef htim1; // 10ms ��ʱ��
TIM_HandleTypeDef htim2; // ���벶��沶�������ʱ��
TIM_HandleTypeDef htim3; // ���벶��
TIM_HandleTypeDef htim4; // 3.5ms ��ʱ��

void (*tim2cb)(void) = NULL;
void (*tim2ic1cb)(void) = NULL;
void (*tim3ic2cb)(void) = NULL;

// ��ʱ�����ƿ��������
struct hTimInfo* TimInit(struct hTimInfo *tim, uint32_t TimBas, enum TimMode Mod)
{ // ��ʱ����ʼ�������붨ʱ����ʱ������ģʽ
	uint32_t count = 0;
	
	if(tim == NULL)
		return NULL;
	
	tim->TimeBase_us =  TimBas;// ���ݶ�ʱ��ʱ����
	tim->sTimList.count = 0;
	tim->TimCnt = 0;
	tim->TimCntMax = 1;
	for(count = 0; count < STIM_LIST_SIZE; count++)
	{
		tim->sTimList.sTimFuncList[count].Count = 0;
		tim->sTimList.sTimFuncList[count].En = S_TIM_DIS;
		tim->sTimList.sTimFuncList[count].Func = NULL;
		tim->sTimList.sTimSeqList[count] = &tim->sTimList.sTimFuncList[count];
	}
	tim->Mode = Mod;
	
	return tim;
}

enum TimMode TimModSet(struct hTimInfo *tim, enum TimMode Mod)
{ // ���ö�ʱ��ģʽ���������õ�ģʽ
	if(NULL == tim)
		return TIM_DISABLE;
	tim->Mode = Mod;
		return tim->Mode;
}

enum TimMode TimModGet(struct hTimInfo *tim)
{ // ��ȡ��ʱ��ģʽ
	if(NULL == tim)
		return TIM_DISABLE;
	else
		return tim->Mode;
}
                      
uint32_t TimBaseSet(struct hTimInfo *tim, uint32_t TimBase)
{ // ���ö�ʱ��ʱ��
	if(NULL == tim)
		return 0;
	
	tim->TimeBase_us = TimBase;
	return tim->TimeBase_us;
}

uint32_t TimBaseGet(struct hTimInfo *tim)
{ // ��ȡ��ʱ��ʱ��
	if(NULL == tim)
		return 0;
	else
		return tim->TimeBase_us;
}
uint32_t CalMaxCnt(struct hTimInfo *tim)
{ // ����������ֵ
	uint32_t count = 0;
	uint32_t ComM = 0;
	
	if(NULL  == tim)
	{
		return 0;
	}
	ComM = tim->sTimList.sTimFuncList[count].Count;
	for(count = 0; count < STIM_LIST_SIZE; count++)
	{
		ComM = ComMul(ComM, tim->sTimList.sTimFuncList[count].Count);
	}
	tim->TimCntMax = ComM;
	return ComM;
}

// ��ʱ����������
uint32_t sTimIns(struct hTimInfo *tim, uint32_t us, void (*Func)(void), enum sTimEnEnum en)
{ // ������ʱ�����񣬷��ض�ʱ����ţ�0Ϊ����
	uint32_t count;
	uint32_t Cnt = 0, InCnt = 0;
	uint32_t DoneFlag = 0; // ��ɱ�־������ѭ���Ƚϵ�����ֵ�л���ʱ��˵���������
	struct sTime *Max = NULL;
	uint32_t sTimNum = 0;
	
	if((NULL == tim) || (NULL == Func))
		return 0;
	
	if(STIM_LIST_SIZE <= tim->sTimList.count)
		return 0;
	
	for(count = 0; count < STIM_LIST_SIZE; count++)
	{
		if(NULL == tim->sTimList.sTimFuncList[count].Func)
		{
			tim->TimeBase_us=(tim->TimeBase_us==0?1:tim->TimeBase_us); // ��ֵֹΪ0
			tim->sTimList.sTimFuncList[count].Count = (us/tim->TimeBase_us)==0?1:us/tim->TimeBase_us; // ��������ֵС��ʱ����ʱ����ʱ���Ķ�ʱ������
			tim->sTimList.sTimFuncList[count].Func = Func;
			tim->sTimList.sTimFuncList[count].En = en;
			tim->sTimList.count++;
			sTimNum = count+1;
			// ð�����򣬽�����б�������򷽱����
			for(Cnt = 0; Cnt < STIM_LIST_SIZE; Cnt++)
			{
				DoneFlag = 0;
				for(InCnt = 0; InCnt < STIM_LIST_SIZE-1; InCnt++)
				{
					if(tim->sTimList.sTimSeqList[InCnt]->Count > tim->sTimList.sTimSeqList[InCnt+1]->Count)
					{
						Max = tim->sTimList.sTimSeqList[InCnt];
						tim->sTimList.sTimSeqList[InCnt] = tim->sTimList.sTimSeqList[InCnt+1];
						tim->sTimList.sTimSeqList[InCnt+1] = Max;
						DoneFlag++;
					}
				}
				if(0 == DoneFlag)
				{
					break;
				}
			}
			CalMaxCnt(tim);
			break;
		}
	}
	
	return sTimNum;
}
void sTimDel(struct hTimInfo *tim, uint32_t TimNum)
{ // ɾ����ʱ��
	uint32_t Cnt = 0, InCnt = 0;
	uint32_t DoneFlag = 0; // ��ɱ�־������ѭ���Ƚϵ�����ֵ�л���ʱ��˵���������
	struct sTime *Max = NULL;
	
	if((NULL == tim) || (!JUDGE_STIM_NUM(TimNum)) || (0 >= tim->sTimList.count))
		return;
	
	if(NULL == tim->sTimList.sTimFuncList[TimNum-1].Func)
		return;
	
	tim->sTimList.sTimFuncList[TimNum-1].Func = NULL;
	tim->sTimList.sTimFuncList[TimNum-1].Count = 0;
	tim->sTimList.sTimFuncList[TimNum-1].En = S_TIM_DIS;
	tim->sTimList.count--;
	// ð�����򣬽�����б�������򷽱����
	for(Cnt = 0; Cnt < STIM_LIST_SIZE; Cnt++)
	{
		DoneFlag = 0;
		for(InCnt = 0; InCnt < STIM_LIST_SIZE-1; InCnt++)
		{
			if(tim->sTimList.sTimSeqList[InCnt]->Count > tim->sTimList.sTimSeqList[InCnt+1]->Count)
			{
				Max = tim->sTimList.sTimSeqList[InCnt];
				tim->sTimList.sTimSeqList[InCnt] = tim->sTimList.sTimSeqList[InCnt+1];
				tim->sTimList.sTimSeqList[InCnt+1] = Max;
				DoneFlag++;
			}
		}
		if(0 == DoneFlag)
		{
			break;
		}
	}
	CalMaxCnt(tim);
}

enum sTimEnEnum sTimModSet(struct hTimInfo *tim, uint32_t TimNum, enum sTimEnEnum EN)
{ // ������ʱ��ģʽ
	if((NULL == tim) || (!JUDGE_STIM_NUM(TimNum)) || (0 >= tim->sTimList.count))
		return S_TIM_DIS;
	
	if(NULL != tim->sTimList.sTimFuncList[TimNum-1].Func)
	{
		tim->sTimList.sTimFuncList[TimNum-1].En = EN;
		return EN;
	}else
	{
		return S_TIM_DIS;
	}
}
enum sTimEnEnum sTimModGet(struct hTimInfo *tim, uint32_t TimNum)
{ // ��ȡ��ʱ��ģʽ�������ʱ�������ڷ���δʹ��
	if((NULL == tim) || (!JUDGE_STIM_NUM(TimNum)))
		return S_TIM_DIS;
	
	if(NULL != tim->sTimList.sTimFuncList[TimNum-1].Func)
	{
		return tim->sTimList.sTimFuncList[TimNum-1].En;
	}else
	{
		return S_TIM_DIS;
	}
}

uint32_t sTimCounSet(struct hTimInfo *tim, uint32_t TimNum, uint32_t count)
{ // ������ʱ����ʱʱ��
	
	uint32_t Cnt = 0, InCnt = 0;
	uint32_t DoneFlag = 0; // ��ɱ�־������ѭ���Ƚϵ�����ֵ�л���ʱ��˵���������
	struct sTime *Max = NULL;
	
	if((NULL == tim) || (!JUDGE_STIM_NUM(TimNum)))
		return 0;
	
	if(NULL != tim->sTimList.sTimFuncList[TimNum-1].Func)
	{
		tim->sTimList.sTimFuncList[TimNum-1].Count = count;
		
		// ð�����򣬽�����б�������򷽱����
		for(Cnt = 0; Cnt < STIM_LIST_SIZE; Cnt++)
		{
			DoneFlag = 0;
			for(InCnt = 0; InCnt < STIM_LIST_SIZE-1; InCnt++)
			{
				if(tim->sTimList.sTimSeqList[InCnt]->Count > tim->sTimList.sTimSeqList[InCnt+1]->Count)
				{
					Max = tim->sTimList.sTimSeqList[InCnt];
					tim->sTimList.sTimSeqList[InCnt] = tim->sTimList.sTimSeqList[InCnt+1];
					tim->sTimList.sTimSeqList[InCnt+1] = Max;
					DoneFlag++;
				}
			}
			if(0 == DoneFlag)
			{
				break;
			}
		}
		CalMaxCnt(tim);
		return tim->sTimList.sTimFuncList[TimNum-1].Count;
	}else
	{
		return 0;
	}
}
uint32_t sTimCounGet(struct hTimInfo *tim, uint32_t TimNum)
{ // ��ȡ��ʱ����ʱʱ��
	if((NULL == tim) || (!JUDGE_STIM_NUM(TimNum)))
		return 0;
	
	if(NULL != tim->sTimList.sTimFuncList[TimNum-1].Func)
	{
		return tim->sTimList.sTimFuncList[TimNum-1].Count;
	}else
	{    
		return 0;
	}
}

void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}
void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

}
void MX_TIM3_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 100-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

}
/* TIM4 init function */
void MX_TIM4_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 72-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 3500-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* TIM1 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration    
    PA15     ------> TIM2_CH1 
    */
    GPIO_InitStruct.Pin = MOTOR3_ECO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(MOTOR3_ECO_GPIO_Port, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_TIM2_PARTIAL_1();

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspInit 0 */

  /* USER CODE END TIM4_MspInit 0 */
    /* TIM4 clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();

    /* TIM4 interrupt Init */
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspInit 1 */

  /* USER CODE END TIM4_MspInit 1 */
  }
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* tim_icHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_icHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM3 GPIO Configuration    
    PB5     ------> TIM3_CH2 
    */
    GPIO_InitStruct.Pin = MPU6050_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(MPU6050_INT_GPIO_Port, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_TIM3_PARTIAL();

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /* TIM1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  
    /**TIM2 GPIO Configuration    
    PA15     ------> TIM2_CH1 
    */
    HAL_GPIO_DeInit(MOTOR3_ECO_GPIO_Port, MOTOR3_ECO_Pin);

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();

    /* TIM4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
} 

void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* tim_icHandle)
{

  if(tim_icHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  
    /**TIM3 GPIO Configuration    
    PB5     ------> TIM3_CH2 
    */
    HAL_GPIO_DeInit(MPU6050_INT_GPIO_Port, MPU6050_INT_Pin);

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
} 

// ��ʱ������ʱ��ص�����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint32_t cnt = 0;
	if (htim == &htim1)
	{
		if(tim1.Mode != TIM_DISABLE)
		{ // �ر�
			if(tim1.Mode == TIM_HARDWARE)
			{ // Ӳ����ִ�е�һ��������
				if(NULL != tim1.sTimList.sTimFuncList[0].Func)
				{
					tim1.sTimList.sTimFuncList[0].Func();
				}
			}else if(tim1.Mode == TIM_SOFTWARE)
			{ // ���
				if(tim1.TimCnt++ > tim1.TimCntMax) // ������������б������ʱֵʱ��λ
				{
					tim1.TimCnt = 0;
					return;
				}
				if(tim1.sTimList.count == 0)
					return;
				for(cnt = 0; cnt < STIM_LIST_SIZE; cnt++)
				{
					if((S_TIM_EN == tim1.sTimList.sTimSeqList[cnt]->En)  // ��ʱ��û��ʹ��
						&& (0 != tim1.sTimList.sTimSeqList[cnt]->Count)  // ��ʱ����ʱֵ��Ч
						&& (NULL != tim1.sTimList.sTimSeqList[cnt]->Func)) // ��ʱ��û�й��ܺ���
					{
						if(0 == tim1.TimCnt%tim1.sTimList.sTimSeqList[cnt]->Count)
						{ // �������Ϊ 0 ��˵��������ʱ����ʱʱ�䣬ִ�з�����
							tim1.sTimList.sTimSeqList[cnt]->Func();
						}
					}
				}
			}else
			{ // �����������ʱ���ر�
				tim1.Mode = TIM_DISABLE;
			}
		}else
		{			
			// �رն�ʱ��
		}
	}else if(htim == &htim2)
	{
		if(tim2cb != NULL)
			tim2cb(); 
	}else if (htim == &htim4)
	{
		if(tim4.Mode != TIM_DISABLE)
		{ // �ر�
			if(tim4.Mode == TIM_HARDWARE)
			{ // Ӳ����ִ�е�һ��������
				if(NULL != tim4.sTimList.sTimFuncList[0].Func)
				{
					tim4.sTimList.sTimFuncList[0].Func();
				}
			}else if(tim4.Mode == TIM_SOFTWARE)
			{ // ���
				if(tim4.TimCnt++ > tim4.TimCntMax) // ������������б������ʱֵʱ��λ
				{
					tim4.TimCnt = 0;
					return;
				}
				if(tim4.sTimList.count == 0)
					return;
				for(cnt = 0; cnt < STIM_LIST_SIZE; cnt++)
				{
					if((S_TIM_EN == tim4.sTimList.sTimSeqList[cnt]->En)  // ��ʱ��û��ʹ��
						&& (0 != tim4.sTimList.sTimSeqList[cnt]->Count)  // ��ʱ����ʱֵ��Ч
						&& (NULL != tim4.sTimList.sTimSeqList[cnt]->Func)) // ��ʱ��û�й��ܺ���
					{
						if(0 == tim4.TimCnt%tim4.sTimList.sTimSeqList[cnt]->Count)
						{ // �������Ϊ 0 ��˵��������ʱ����ʱʱ�䣬ִ�з�����
							tim4.sTimList.sTimSeqList[cnt]->Func();
						}
					}
				}
			}else
			{ // �����������ʱ���ر�
				tim4.Mode = TIM_DISABLE;
			}
		}else
		{			
			// �رն�ʱ��
		}
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim2)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			if(tim2ic1cb != NULL)
				tim2ic1cb();
		}
	}else if(htim == &htim3)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if(tim3ic2cb != NULL)
				tim3ic2cb();
		}
	}
}

