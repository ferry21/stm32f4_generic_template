/**
 * @file pushbutton.c
 * @author Andrew K. Walker
 * @date 07 JUN 2017
 * @brief Sets up external interrupt for the pushbutton on the discovery card.
 */
#include "pushbutton.h"

#include "debug.h"

uint8_t pushbutton_initialized = 0;

/**
 * @fn void EXTI0_Handler(void)
 * @brief Handles the external interrupt generated by the discovery pushbutton.
 *
 * @param None
 * @return None
 */
void EXTI0_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line0) != RESET)
   {

      debug_output_toggle(DEBUG_LED_ORANGE);

      /* Zero the tilt motor here for now.  Eventually, we need to tie that to
         the pin that will actually have the flag piped in. */
      /* quad_encoder_set_position(TILT_ZERO_POSITION_QC); */
      /* TB6612_set_duty(0.25); */

      /* Clear the EXTI line 0 pending bit */
      EXTI_ClearITPendingBit(EXTI_Line0);
   }
}

/* Public Function - Doxygen documentation is included in the header. */
void pushbutton_init(void)
{

   GPIO_InitTypeDef  GPIO_InitStructure;
   EXTI_InitTypeDef   EXTI_InitStructure;
   NVIC_InitTypeDef   NVIC_InitStructure;

   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* Connect EXTI Line0 to PA0 pin */
   SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

   /* Configure EXTI Line0 */
   EXTI_InitStructure.EXTI_Line = EXTI_Line0;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStructure);

   /* Enable and set EXTI Line0 Interrupt to the lowest priority */
   NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);



   pushbutton_initialized = 1;

}
