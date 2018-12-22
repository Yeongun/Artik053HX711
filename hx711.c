/*
 * hx711.c
 *
 *  Created on: 2018. 12. 12.
 *      Author: 김영운
 */

#include <stdio.h>
#include <apps/shell/tash.h>
#include <tinyara/gpio.h>
#include <tinyara/config.h>

//For XGPIO22 ,10
#define GPIO_FUNC_SHIFT 13
#define GPIO_INPUT (0x0 << GPIO_FUNC_SHIFT) //22
#define GPIO_OUTPUT (0x1 << GPIO_FUNC_SHIFT) //10

#define GPIO_PORT_SHIFT 3
#define GPIO_PORTG2 (0x6 << GPIO_PORT_SHIFT) //22
#define GPIO_PORTG1 (0x5 << GPIO_PORT_SHIFT) //10

#define GPIO_PIN_SHIFT 0
#define GPIO_PIN6 (0x6 << GPIO_PIN_SHIFT) //22
#define GPIO_PIN2 (0x2 << GPIO_PIN_SHIFT) //10

#define GPIO_PUPD_SHIFT 11
#define GPIO_PULLDOWN (0x1 << GPIO_PUPD_SHIFT) //22
#define GPIO_PULLUP (0x3 << GPIO_PUPD_SHIFT) //10

uint32_t dt; //dt is in
uint32_t sck; //sck is out

long readCount(){

	float gram;
	unsigned long count;
	unsigned char i;

	dt=GPIO_INPUT | GPIO_PORTG2 | GPIO_PIN6 | GPIO_PULLDOWN;
	sck=GPIO_OUTPUT | GPIO_PORTG1 | GPIO_PIN2 | GPIO_PULLUP;

	s5j_configgpio(sck);
	s5j_configgpio(dt);
	s5j_gpiowrite(dt,1);up_udelay(1);
	s5j_gpiowrite(sck,0);up_udelay(1);
	dt=GPIO_INPUT | GPIO_PORTG2 | GPIO_PIN6 | GPIO_PULLDOWN;
	s5j_configgpio(dt);

	count=0;
	while (s5j_gpioread(dt)==1);
	for(i=0;i<24;i++){
		s5j_gpiowrite(sck,1);
		up_udelay(1);
		count=count<<1;
		s5j_gpiowrite(sck,0);
		up_udelay(1);
		if(s5j_gpioread(dt)==0){
			count=count+1;
		}

	}
	count=count>>6;
	s5j_gpiowrite(sck,1);
	up_udelay(1);
	count=count^0x800000;
	s5j_gpiowrite(sck,0);
	up_udelay(1);
	return count;

}

