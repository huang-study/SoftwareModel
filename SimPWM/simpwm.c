#include "simpwm.h"

struct PWMPass
{
	void *arg;
	uint32_t OpValue;
	void (*OPSet)(void *, enum PWMOPMove);
};
struct 
{
	uint32_t PWMCount; // PWM计数值
	uint32_t PWMGroupCount; // PWM通路组
	struct PWMPass PWMGroup[PWM_PASS_SIZE]; // PWM通路
} SimPWM_handle;

void SimPWM_Init(void)
{
	int Count = 0;
	SimPWM_handle.PWMCount = 0;
	SimPWM_handle.PWMGroupCount = 0;
	
	for(Count = 0; Count < PWM_PASS_SIZE; Count++)
	{
		SimPWM_handle.PWMGroup[Count].arg = NULL;
		SimPWM_handle.PWMGroup[Count].OpValue = 0;
		SimPWM_handle.PWMGroup[Count].OPSet = NULL;
	}
}
uint32_t SimPWM_InsPass(void *arg, void (*OPSet)(void *, enum PWMOPMove))
{ // 插入PWM通路，传入通路控制函数，成功返回通路编号
	uint32_t Count = 0;
	uint32_t NewPass = 0;
	
	if((OPSet == NULL) || (SimPWM_handle.PWMGroupCount >= PWM_PASS_SIZE))
	{ // 传入空指针或已满则分配失败
		return NewPass;
	}
	
	for(Count = 0; Count < PWM_PASS_SIZE; Count++)
	{
		if(SimPWM_handle.PWMGroup[Count].OPSet == NULL)
		{
			NewPass = Count;
			SimPWM_handle.PWMGroup[Count].arg = arg;
			SimPWM_handle.PWMGroup[Count].OPSet = OPSet;
			SimPWM_handle.PWMGroup[Count].OpValue = 0;
			SimPWM_handle.PWMGroupCount++;
			break;
		}
	}
	
	return NewPass+1;
}
void SimPWM_DelPass(uint32_t PassNum)
{ // 删除通路
	if((PassNum == 0)  \
		|| (PassNum > PWM_PASS_SIZE)  \
		|| (0 == SimPWM_handle.PWMGroupCount) \
		|| (NULL == SimPWM_handle.PWMGroup[PassNum-1].OPSet))
	{
		return;
	}
	SimPWM_handle.PWMGroup[PassNum-1].arg = NULL;
	SimPWM_handle.PWMGroup[PassNum-1].OPSet = NULL;
	SimPWM_handle.PWMGroup[PassNum-1].OpValue = 0;
	SimPWM_handle.PWMGroupCount--;
}
uint32_t SimPWM_SetPassPWM(uint32_t PassNum, uint32_t PWM)
{ // 设置占空比，通路号和 PWM 值，成功：0，失败：1
		if((PassNum == 0)  \
		|| (PassNum > PWM_PASS_SIZE)  \
		|| (0 == SimPWM_handle.PWMGroupCount) \
		|| (NULL == SimPWM_handle.PWMGroup[PassNum-1].OPSet))
	{
		return 1;
	}
	SimPWM_handle.PWMGroup[PassNum-1].OpValue = PWM;
	
	return 0;
}
uint32_t SimPWM_GetPassPWMValue(uint32_t PassNum, uint32_t *PWM)
{ // 返回 PWM 参数，通路号错误返回1， 成功返回0
		if((PassNum == 0)  \
		|| (PassNum > PWM_PASS_SIZE)  \
		|| (0 == SimPWM_handle.PWMGroupCount) \
		|| (NULL == SimPWM_handle.PWMGroup[PassNum-1].OPSet))
	{
		return 1;
	}
	*PWM = SimPWM_handle.PWMGroup[PassNum-1].OpValue;
	
	return 0;
}
uint32_t SimPWM_GetPassPWM(uint32_t PassNum)
{ // 返回 PWM 参数，通路号错误返回1， 成功返回0
		if((PassNum == 0)  \
		|| (PassNum > PWM_PASS_SIZE)  \
		|| (0 == SimPWM_handle.PWMGroupCount) \
		|| (NULL == SimPWM_handle.PWMGroup[PassNum-1].OPSet))
	{
		return 0;
	}
	return SimPWM_handle.PWMGroup[PassNum-1].OpValue;
}
void SimPWM_AddCount(void)
{ // 计数值增加
	uint32_t Count = 0;
	if(++SimPWM_handle.PWMCount >= PWM_COUNT_SIZE)
	{
		SimPWM_handle.PWMCount = 0;
	}
	
	for(Count = 0; Count < PWM_PASS_SIZE; Count++)
	{
		if(SimPWM_handle.PWMGroup[Count].OPSet != NULL)
		{
			SimPWM_handle.PWMGroup[Count].OPSet( \
				SimPWM_handle.PWMGroup[Count].arg, 
				SimPWM_handle.PWMCount<SimPWM_handle.PWMGroup[Count].OpValue? \
				PWMOpHig:PWMOpLow);
		}
	}
}
