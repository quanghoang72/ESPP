/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>

/* Task Stack Size */
#define APP_TASK_START_STK_SIZE 128u
#define TEMPERATURE_TASK_STK_SIZE 128u
#define DIS_TASK_STK_SIZE 128u
#define SEND_TASK_STK_SIZE 128u

/* Task Priority */
#define APP_TASK_START_PRIO 1u
#define DIS_TASK_PRIO 2u
#define TEMPERATURE_TASK_PRIO 3u
#define SEND_TASK_PRIO 4u

void SystemClock_Config(void);

/* Task Control Block */
static OS_TCB AppTaskStartTCB;
static OS_TCB DisTaskTCB;
static OS_TCB TemperatureTaskTCB;
static OS_TCB SendTaskTCB;

/* Task Stack */
static CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
static CPU_STK TemperatureTaskStk[TEMPERATURE_TASK_STK_SIZE];
static CPU_STK DisTaskStk[DIS_TASK_STK_SIZE];
static CPU_STK SendTaskStk[DIS_TASK_STK_SIZE];

/* Semaphore */
OS_SEM sem;

/* Function Prototype */
static void AppTaskStart(void *p_arg);
static void TemperatureTask(void *p_arg);
static void DisTask(void *p_arg);
static void SendTask(void *p_arg);
/**
  * @brief  The application entry point.
  * @retval int
  */
/** GLOBAL VARIABLE **/
uint16_t counter_D = 0;
uint16_t counter_T = 0;
uint16_t count = 0; //count people

uint16_t result = 0;
uint8_t Temperature = 0;

int main(void)
{
  /* To store error code */
  OS_ERR os_err;

  /* Initialize uC/OS-III */
  OSInit(&os_err);

  if (os_err != OS_ERR_NONE)
  {
    while (DEF_TRUE)
      ;
  }

  OSSemCreate(
      (OS_SEM *)&sem,
      (CPU_CHAR *)"Semaphore to protect share resource",
      (OS_SEM_CTR)1,
      (OS_ERR *)&os_err);
  OSTaskCreate(
      /* pointer to task control block */
      (OS_TCB *)&AppTaskStartTCB,
      /* task name can be displayed by debuggers */
      (CPU_CHAR *)"App Task Start",
      /* pointer to the task */
      (OS_TASK_PTR)AppTaskStart,
      /* pointer to an OPTIONAL data area */
      (void *)0,
      /* task priority: the lower the number, the higher the priority */
      (OS_PRIO)APP_TASK_START_PRIO,
      /* pointer to task's stack base addr */
      (CPU_STK *)&AppTaskStartStk[0],
      /* task's stack limit to monitor and ensure that the stack 
       * doesn't overflow (10%) */
      (CPU_STK_SIZE)APP_TASK_START_STK_SIZE / 10,
      /* task's stack size */
      (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,
      /* max number of message that the task can receive through 
       * internal message queue (5) */
      (OS_MSG_QTY)5u,
      /* amount of clock ticks for the time quanta 
       * when round robin is enabled */
      (OS_TICK)0u,
      /* pointer to an OPTIONAL user-supplied memory location 
       * use as a TCB extension */
      (void *)0,
      /* contain task-specific option 
       * OS_OPT_TASK_STK_CHK: allow stack checking 
       * OS_OPT_TASK_STK_CLR: stack needs to be cleared */
      (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
      /* pointer to a variable that will receive an error code */
      (OS_ERR *)&os_err);

  if (os_err != OS_ERR_NONE)
  {
    while (DEF_TRUE)
      ;
  }

  /* Start Mulitasking */
  OSStart(&os_err);
  /* USER CODE BEGIN 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static void AppTaskStart(void *p_arg)
{
  OS_ERR os_err;

  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();

  HAL_TIM_Base_Start(&htim4);
  HAL_TIM_Base_Start(&htim6);

  OSTaskCreate(
      (OS_TCB *)&DisTaskTCB,
      (CPU_CHAR *)"Distance measurement Task",
      (OS_TASK_PTR)DisTask,
      (void *)0,
      (OS_PRIO)DIS_TASK_PRIO,
      (CPU_STK *)&DisTaskStk[0],
      (CPU_STK_SIZE)DIS_TASK_STK_SIZE / 10,
      (CPU_STK_SIZE)DIS_TASK_STK_SIZE,
      (OS_MSG_QTY)5u,
      (OS_TICK)0u,
      (void *)0,
      (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
      (OS_ERR *)&os_err);
  OSTaskCreate(
      (OS_TCB *)&TemperatureTaskTCB,
      (CPU_CHAR *)"Temperature measurement Task",
      (OS_TASK_PTR)TemperatureTask,
      (void *)0,
      (OS_PRIO)TEMPERATURE_TASK_PRIO,
      (CPU_STK *)&TemperatureTaskStk[0],
      (CPU_STK_SIZE)TEMPERATURE_TASK_STK_SIZE / 10,
      (CPU_STK_SIZE)TEMPERATURE_TASK_STK_SIZE,
      (OS_MSG_QTY)5u,
      (OS_TICK)0u,
      (void *)0,
      (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
      (OS_ERR *)&os_err);
  OSTaskCreate(
      (OS_TCB *)&SendTaskTCB,
      (CPU_CHAR *)"Sending data to usart2 and regular LED Task",
      (OS_TASK_PTR)SendTask,
      (void *)0,
      (OS_PRIO)SEND_TASK_PRIO,
      (CPU_STK *)&SendTaskStk[0],
      (CPU_STK_SIZE)SEND_TASK_STK_SIZE / 10,
      (CPU_STK_SIZE)SEND_TASK_STK_SIZE,
      (OS_MSG_QTY)5u,
      (OS_TICK)0u,
      (void *)0,
      (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
      (OS_ERR *)&os_err);
}
static void DisTask(void *p_arg)
{

  OS_ERR os_err;
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
  while (DEF_TRUE)
  {
    OSSemPend(
        (OS_SEM *)&sem,
        (OS_TICK)0,
        (OS_OPT)OS_OPT_PEND_BLOCKING,
        (CPU_TS *)NULL,
        (OS_ERR *)&os_err);

    HCSR04_Read();
    if (Distance <= 10 && Distance != 0) // !=0 to avoid initial value
    {
      HAL_GPIO_WritePin(GPIOA, LD2_Pin, 1);
      counter_D = 1;
    }

    if ((counter_T & counter_D) == 1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1) //ppl goout - check output status of HCSR04
    {
      if (count == 0) // prevent negative value
        {count = 0;}
      else{
        count--;
        HAL_UART_Transmit(&huart2, "Count PP out \n\r", 15, 100);}
      counter_T = 0;
      counter_D = 0;
    }
    else
    {
      counter_T = 0; //reset Temperature counter
    }
    OSSemPost(
        (OS_SEM *)&sem,
        (OS_OPT)OS_OPT_POST_1,
        (OS_ERR *)&os_err);
    OSTimeDlyHMSM(0, 0, 0, 700, OS_OPT_TIME_HMSM_STRICT, &os_err);
  }
}
static void TemperatureTask(void *p_arg)
{

  OS_ERR os_err;
  while (DEF_TRUE)
  {
    OSSemPend(
        (OS_SEM *)&sem,
        (OS_TICK)0,
        (OS_OPT)OS_OPT_PEND_BLOCKING,
        (CPU_TS *)NULL,
        (OS_ERR *)&os_err);

    result = DS18B20_Operation();
    Temperature = floor(result / 16.00 * 1.00);
    if (Temperature >= 28 && Temperature <= 32)
    {
      HAL_GPIO_WritePin(GPIOA, LD2_Pin, 0);
      counter_T = 1;
    }
    if ((counter_T & counter_D) == 1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 0) // comein - LD output of DS18b20
    {
      count++; // Dis<=10, 27 <= T <=32
      counter_T = 0;
      counter_D = 0;
      HAL_UART_Transmit(&huart2, "Count PP in \n\r", 15, 100);
    }
    //     else
    // {
    //   counter_D = 0; //reset Temperature counter
    // }
    OSSemPost(
        (OS_SEM *)&sem,
        (OS_OPT)OS_OPT_POST_1,
        (OS_ERR *)&os_err);
    OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &os_err);
  }
}

static void SendTask(void *p_arg)
{

  OS_ERR os_err;
  uint8_t msg[50];
    uint8_t buf[30];
    uint8_t test=5;

  while (DEF_TRUE)
  {
    OSSemPend(
        (OS_SEM *)&sem,
        (OS_TICK)0,
        (OS_OPT)OS_OPT_PEND_BLOCKING,
        (CPU_TS *)NULL,
        (OS_ERR *)&os_err);

    /* LED SETUP */
    if (count != 0)
    {
      Set_Pin_Input(GPIOA, GPIO_PIN_4);
    }
    else
    {
      Set_Pin_Output(GPIOA, GPIO_PIN_4);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
    }
  /* SEND DATA*/

   sprintf(buf,"%5d",count);
  HAL_UART_Transmit(&huart3, buf, 5, 100);

   sprintf(msg, "DIS: %d | TEM: %d | C: %d \n\r", Distance, Temperature, count);
    HAL_UART_Transmit(&huart2, msg, strlen(msg), 100);



    OSSemPost(
        (OS_SEM *)&sem,
        (OS_OPT)OS_OPT_POST_1,
        (OS_ERR *)&os_err);
    OSTimeDlyHMSM(0, 0, 1, 500, OS_OPT_TIME_HMSM_STRICT, &os_err);
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
