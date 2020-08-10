#include "simpwm.h"

struct PWMPass
{
	void *arg;
	uint32_t OpValue;
	void (*OPSet)(void *, enum PWMOPMove);
};
struct 
{
	uint32_t PWMCount; // PWM����ֵ
	uint32_t PWMGroupCount; // PWMͨ·��
	struct PWMPass PWMGroup[PWM_PASS_SIZE]; // PWMͨ·
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
{ // ����PWMͨ·������ͨ·���ƺ������ɹ�����ͨ·���
	uint32_t Count = 0;
	uint32_t NewPass = 0;
	
	if((OPSet == NULL) || (SimPWM_handle.PWMGroupCount >= PWM_PASS_SIZE))
	{ // �����ָ������������ʧ��
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
{ // ɾ��ͨ·
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
{ // ����ռ�ձȣ�ͨ·�ź� PWM ֵ���ɹ���0��ʧ�ܣ�1
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
{ // ���� PWM ������ͨ·�Ŵ��󷵻�1�� �ɹ�����0
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
{ // ���� PWM ������ͨ·�Ŵ��󷵻�1�� �ɹ�����0
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
{ // ����ֵ����
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
