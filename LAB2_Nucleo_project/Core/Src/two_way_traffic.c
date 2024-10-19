/*
 * two_way_traffic.c
 *
 *  Created on: Oct 4, 2024
 *      Author: thuyh
 */

#include "two_way_traffic.h"
#include "main.h"
typedef enum{
	REDGREEN,
	REDYELLOW,
	GREENRED,
	YELLOWRED
} state_traffic;
int state ;
void convert (GPIO_TypeDef * GPIOxA,GPIO_TypeDef * GPIOxB, uint16_t A, uint16_t B, State type){
	switch (type) {
		case YELLOW:
			HAL_GPIO_WritePin(GPIOxA, A, RESET);
			HAL_GPIO_WritePin(GPIOxB, B, SET);
			break;
		case GREEN:
			HAL_GPIO_WritePin(GPIOxA, A, SET);
			HAL_GPIO_WritePin(GPIOxB, B, RESET);
			break;
		case RED:
			HAL_GPIO_WritePin(GPIOxA, A, SET);
			HAL_GPIO_WritePin(GPIOxB, B, SET);
			break;
		default:
			break;
	}
}
void two_way_traffic_init(){
	setTimer1(1, 1000);
	state = GREENRED;
	convert(A1_GPIO_Port, B1_GPIO_Port, A1_Pin, B1_Pin, GREEN);
	convert(A3_GPIO_Port, B3_GPIO_Port, A3_Pin, B3_Pin, GREEN);
    convert(A2_GPIO_Port, B2_GPIO_Port, A2_Pin, B2_Pin, RED);
	convert(A4_GPIO_Port, B4_GPIO_Port, A4_Pin, B4_Pin, RED);}
GPIO_TypeDef* LED70[7] = {SEG0_GPIO_Port,SEG1_GPIO_Port,SEG2_GPIO_Port,SEG3_GPIO_Port,SEG4_GPIO_Port,SEG5_GPIO_Port,SEG6_GPIO_Port};
uint16_t LED70_Pin[7] = {SEG0_Pin,SEG1_Pin,SEG2_Pin,SEG3_Pin,SEG4_Pin,SEG5_Pin,SEG6_Pin};
GPIO_TypeDef* LED71[7] = {SEG1_0_GPIO_Port,SEG1_1_GPIO_Port,SEG1_2_GPIO_Port,SEG1_3_GPIO_Port,SEG1_4_GPIO_Port,SEG1_5_GPIO_Port,SEG1_6_GPIO_Port};
uint16_t LED71_Pin[7] = {SEG1_0_Pin,SEG1_1_Pin,SEG1_2_Pin,SEG1_3_Pin,SEG1_4_Pin,SEG1_5_Pin,SEG1_6_Pin};
int count = 3;

int count_1 = 5;
void two_way_traffic_run(){
    switch (state){
        case GREENRED:
            // GREEN 3s
            if(count >=4)
            {
                count =3;
            }
            if(count <=0)
            {
                state = YELLOWRED;
                count = 2;

                // Chuyển đổi trạng thái đèn giao thông
                convert(A1_GPIO_Port, B1_GPIO_Port, A1_Pin, B1_Pin, YELLOW);
                convert(A2_GPIO_Port, B2_GPIO_Port, A2_Pin, B2_Pin, RED);
                convert(A3_GPIO_Port, B3_GPIO_Port, A3_Pin, B3_Pin, YELLOW);
                convert(A4_GPIO_Port, B4_GPIO_Port, A4_Pin, B4_Pin, RED);
            }
            break;

        case YELLOWRED:
            // YELLOW 2s
            if(count >=3)
            {
                count =2;
            }
            if(count <=0)
            {
                state = REDGREEN;
                count = 5;

                // Chuyển đổi trạng thái đèn giao thông
                convert(A1_GPIO_Port, B1_GPIO_Port, A1_Pin, B1_Pin, RED);
                convert(A2_GPIO_Port, B2_GPIO_Port, A2_Pin, B2_Pin, GREEN);
                convert(A3_GPIO_Port, B3_GPIO_Port, A3_Pin, B3_Pin, RED);
                convert(A4_GPIO_Port, B4_GPIO_Port, A4_Pin, B4_Pin, GREEN);

                count_1 = 3;
            }
            break;

        case REDGREEN:
            // RED 5s
            if(count >=6)
            {
                count =5;
            }
            if(count_1 <=0){
                state = REDYELLOW;
                count_1 = 2;

                // Chuyển đổi trạng thái đèn giao thông
                convert(A1_GPIO_Port, B1_GPIO_Port, A1_Pin, B1_Pin, RED);
                convert(A2_GPIO_Port, B2_GPIO_Port, A2_Pin, B2_Pin, YELLOW);
                convert(A3_GPIO_Port, B3_GPIO_Port, A3_Pin, B3_Pin, RED);
                convert(A4_GPIO_Port, B4_GPIO_Port, A4_Pin, B4_Pin, YELLOW);
            }
            break;

        case REDYELLOW:
            if(count <=0)
            {
                state = GREENRED;
                count = 3;
                count_1 = 5;

                // Chuyển đổi trạng thái đèn giao thông
                convert(A1_GPIO_Port, B1_GPIO_Port, A1_Pin, B1_Pin, GREEN);
                convert(A2_GPIO_Port, B2_GPIO_Port, A2_Pin, B2_Pin, RED);
                convert(A3_GPIO_Port, B3_GPIO_Port, A3_Pin, B3_Pin, GREEN);
                convert(A4_GPIO_Port, B4_GPIO_Port, A4_Pin, B4_Pin, RED);
            }
            break;

        default:
            state = 0;
            break;
    }

    if(timer1_flag[1] == 1){
        setTimer1(1, 1000);
        count_1--;
        count--;
    }

    DISPLAY_LED_7_SEGMENT_PREMIUM(LED70, LED70_Pin, count-1);
    DISPLAY_LED_7_SEGMENT_PREMIUM(LED71, LED71_Pin, count_1-1);
}

