/*
 * led7.c
 *
 *  Created on: Sep 14, 2024
 *      Author: thuyh
 */
#include "led7.h"
int num ;
void led7_init(){
	num = 1;
	HAL_GPIO_WritePin(GPIOA,EN1_Pin, SET);
	HAL_GPIO_WritePin(GPIOA,EN2_Pin, RESET);
	setTimer1(0,1000);
}

void led7_run(){

	if(timer1_flag[0]==1){
		// num=3-num;
		setTimer1(0,1000);
	    HAL_GPIO_TogglePin(GPIOA, EN1_Pin) ;
	    HAL_GPIO_TogglePin(GPIOA, EN2_Pin) ;
		DISPLAY_LED_7_SEGMENT(GPIOB, SEG0_Pin, SEG1_Pin, SEG2_Pin, SEG3_Pin, SEG4_Pin, SEG5_Pin, SEG6_Pin, num);
	}
}
