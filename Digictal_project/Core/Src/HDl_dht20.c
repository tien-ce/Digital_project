/*
 * HDl_dht20.c
 *
 *  Created on: Oct 16, 2024
 *      Author: thuyh
 */
#include "HDL_dht20.h"
 I2C_HandleTypeDef hi2c1;
typedef enum{
	INIT_DHT20,
	CHECK_CONNECTION,
	CHECK_READY,
    REQUEST_DATA,
    READ_DATA,
    CONVERT_DATA,
	DONE
} DHT20_state;
DHT20_state state;
DHT20_t dht20;
status_active active ;
void HDL_DHT20_init(){
	state = INIT_DHT20;
	active = DHT20_OK;
}

void HDL_DHT20_run(){
	switch(state){
	case INIT_DHT20:
		DHT20_Init(&dht20, &hi2c1);
	 	 HAL_Delay(100);
		 state = CHECK_CONNECTION;
	 	 break;
	case CHECK_CONNECTION:
		if(DHT20_IsConnected(&dht20) ){
			state = CHECK_READY;
		}
		else{
			active = DHT20_ERROR_CONNECT;
		}
		break;
	case CHECK_READY:
		if((DHT20_ReadStatus(&dht20) & 0x18) != 0x18){
			DHT20_ResetSensor(&dht20);
		 	HAL_Delay(1000);
			state = CHECK_READY;
		}
		else {
			HAL_Delay(10);
			state = REQUEST_DATA;
		}
		break;
	case REQUEST_DATA:
		if(HAL_GetTick() - dht20.lastRead >= 1000){
			active = DHT20_RequestData(&dht20);
			if (active == DHT20_OK){
				HAL_Delay(80);
				state = READ_DATA;
			}
		}
		else{
			active = DHT20_ERROR_LASTREAD;
		}
		break;
	case READ_DATA:
		if(HAL_GetTick() - dht20.lastRequest < 1000){
			active = DHT20_ReadData(&dht20);
			if(active == DHT20_OK){
				state = CONVERT_DATA;
			}
			else{
				state = REQUEST_DATA;
			}
		}
		else{
			active = DHT20_ERROR_READ_TIMEOUT;
		}
		break;
	case CONVERT_DATA:
		active = DHT20_Convert(&dht20);
		if(active == DHT20_OK){
			state = DONE;
		}
		break;
	case DONE:
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOA, LED_GREEN_Pin);
		state = CHECK_READY;
		break;
	default :
		break;
	}

}
