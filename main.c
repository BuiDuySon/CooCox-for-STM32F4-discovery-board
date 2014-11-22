#include "stm32f4xx.h"

#include "stm32f4xx_gpio.h"

#include "stm32f4xx_rcc.h"

#include "CoOS.h"

#define STACK_SIZE_TASK_A 128
#define STACK_SIZE_TASK_B 128
#define STACK_SIZE_TASK_C 128

OS_STK task_A_stk[STACK_SIZE_TASK_A];
OS_STK task_B_stk[STACK_SIZE_TASK_B];
OS_STK task_C_stk[STACK_SIZE_TASK_C];

void taskA(void *pdata) {
	while (1) {
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_14);
	}
}
void taskB(void *pdata) {
	while (1) {
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14);
	}
}
void taskC(void *pdata) {
	while (1) {
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13);
	}
}

void Delay(__IO uint32_t nCount) {
	while (nCount--) {
	}
}

GPIO_InitTypeDef GPIO_InitStruct;

void init() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13
			| GPIO_Pin_12;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	Delay(0xFFFFF);
	GPIO_ResetBits(GPIOD,
			GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

}

int main(void) {
	init();
	CoInitOS();
	CoCreateTask(taskA, 0, 0, &task_A_stk[STACK_SIZE_TASK_A - 1],
			STACK_SIZE_TASK_A);
	CoCreateTask(taskB, 0, 1, &task_B_stk[STACK_SIZE_TASK_B - 1],
			STACK_SIZE_TASK_B);
	CoCreateTask(taskC, 0, 2, &task_C_stk[STACK_SIZE_TASK_C - 1],
			STACK_SIZE_TASK_C);

	CoStartOS();

	while (1){
		
	}

} //End of main loop
