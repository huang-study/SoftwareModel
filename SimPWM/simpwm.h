#ifndef __SIMPWM_H
#define __SIMPWM_H

#define PWM_PASS_SIZE 6 // ����ͨ·ʱ�޸�
#define PWM_COUNT_SIZE 100

#ifndef uint32_t 
#define uint32_t unsigned int
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif
// �û���Ҫʵ��OPSet�ӿں�
enum PWMOPMove
{
	PWMOpHig = 0,
	PWMOpLow
};

void SimPWM_Init(void);
uint32_t SimPWM_InsPass(void *arg, void (*OPSet)(void *, enum PWMOPMove));
void SimPWM_DelPass(uint32_t PassNum);
uint32_t SimPWM_SetPassPWM(uint32_t PassNum, uint32_t PWM);
uint32_t SimPWM_GetPassPWMValue(uint32_t PassNum, uint32_t *PWM); // ���ݵ�ֵ
uint32_t SimPWM_GetPassPWM(uint32_t PassNum); // ֱ�ӷ��أ��޸�ͨ·����0

void SimPWM_AddCount(void); // ��ʱ������

#endif /* __SIMPWM_H */
