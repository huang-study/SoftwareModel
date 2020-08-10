#ifndef __SIMPWM_H
#define __SIMPWM_H

#define PWM_PASS_SIZE 6 // 增加通路时修改
#define PWM_COUNT_SIZE 100

#ifndef uint32_t 
#define uint32_t unsigned int
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif
// 用户需要实现OPSet接口函
enum PWMOPMove
{
	PWMOpHig = 0,
	PWMOpLow
};

void SimPWM_Init(void);
uint32_t SimPWM_InsPass(void *arg, void (*OPSet)(void *, enum PWMOPMove));
void SimPWM_DelPass(uint32_t PassNum);
uint32_t SimPWM_SetPassPWM(uint32_t PassNum, uint32_t PWM);
uint32_t SimPWM_GetPassPWMValue(uint32_t PassNum, uint32_t *PWM); // 传递到值
uint32_t SimPWM_GetPassPWM(uint32_t PassNum); // 直接返回，无该通路返回0

void SimPWM_AddCount(void); // 定时器调用

#endif /* __SIMPWM_H */
