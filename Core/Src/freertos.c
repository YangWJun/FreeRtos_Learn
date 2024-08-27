/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "queue.h"
#include "OLED.h"
#include "MPU6050.h"
#include "stream_buffer.h"
#include "list.h"
#include "croutine.h"
#include "semphr.h"
#include "usart.h"
#include "event_groups.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
const osThreadAttr_t myTask03_attributes = {
  .name = "myTask03",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask04 */
osThreadId_t myTask04Handle;
const osThreadAttr_t myTask04_attributes = {
  .name = "myTask04",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask05 */
osThreadId_t myTask05Handle;
const osThreadAttr_t myTask05_attributes = {
  .name = "myTask05",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};
/* Definitions for USAR_RX */
osMessageQueueId_t USAR_RXHandle;
const osMessageQueueAttr_t USAR_RX_attributes = {
  .name = "USAR_RX"
};
/* Definitions for myMutex01 */
osMutexId_t myMutex01Handle;
const osMutexAttr_t myMutex01_attributes = {
  .name = "myMutex01"
};
/* Definitions for UARST_RX */
osSemaphoreId_t UARST_RXHandle;
const osSemaphoreAttr_t UARST_RX_attributes = {
  .name = "UARST_RX"
};
/* Definitions for myEvent01 */
osEventFlagsId_t myEvent01Handle;
const osEventFlagsAttr_t myEvent01_attributes = {
  .name = "myEvent01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void OLED_SHOW_1(void *argument);
void OLED_SHOW_2(void *argument);
void USART_TX(void *argument);
void LED_Flash_2(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of myMutex01 */
  myMutex01Handle = osMutexNew(&myMutex01_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of UARST_RX */
  UARST_RXHandle = osSemaphoreNew(1, 0, &UARST_RX_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueue01 */
  myQueue01Handle = osMessageQueueNew (3, sizeof(uint32_t), &myQueue01_attributes);

  /* creation of USAR_RX */
  USAR_RXHandle = osMessageQueueNew (2, sizeof(uint8_t), &USAR_RX_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(OLED_SHOW_1, NULL, &myTask02_attributes);

  /* creation of myTask03 */
  myTask03Handle = osThreadNew(OLED_SHOW_2, NULL, &myTask03_attributes);

  /* creation of myTask04 */
  myTask04Handle = osThreadNew(USART_TX, NULL, &myTask04_attributes);

  /* creation of myTask05 */
  myTask05Handle = osThreadNew(LED_Flash_2, NULL, &myTask05_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of myEvent01 */
  myEvent01Handle = osEventFlagsNew(&myEvent01_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
 // vTaskStartScheduler();
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {   static  uint16_t count;
      HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_3);
      count++;
      if(count==5) {
         // xTaskNotifyGive(myTask02Handle);  //释放任务通知，�?�知Task2
          xEventGroupSetBits(myEvent01Handle , 0x01);
count=0;
      }

    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_OLED_SHOW_1 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OLED_SHOW_1 */
void OLED_SHOW_1(void *argument)
{
  /* USER CODE BEGIN OLED_SHOW_1 */
  /* Infinite loop */
  for(;;)
  { static int32_t count;
      uint16_t COUNT;
      count++;
     xQueueSend(myQueue01Handle,&count,10);
    
      xSemaphoreTake(myMutex01Handle,portMAX_DELAY);
      OLED_ShowNum(1,1,count,4);
      xSemaphoreGive(myMutex01Handle);

    // uint32_t  x=ulTaskNotifyTake(pdTRUE,10);//获取任务通知

    osDelay(1);
  }
  /* USER CODE END OLED_SHOW_1 */
}

/* USER CODE BEGIN Header_OLED_SHOW_2 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OLED_SHOW_2 */
void OLED_SHOW_2(void *argument)
{
  /* USER CODE BEGIN OLED_SHOW_2 */
  /* Infinite loop */
  for(;;)
  {   int32_t  count;
      xQueueReceive(myQueue01Handle,&count,10);
      xSemaphoreTake(myMutex01Handle,portMAX_DELAY);
      OLED_ShowNum(2,1,count,4);
      xSemaphoreGive(myMutex01Handle);
      osDelay(1);
  }
  /* USER CODE END OLED_SHOW_2 */
}

/* USER CODE BEGIN Header_USART_TX */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USART_TX */
void USART_TX(void *argument)
{
  /* USER CODE BEGIN USART_TX */
  /* Infinite loop */
    static  uint8_t  Rx;
    static uint32_t count;
    BaseType_t err=pdFALSE;
  for(;;)
  {

      count++;
      err= xSemaphoreTake(UARST_RXHandle,10);
    if(err==pdTRUE)
    { xQueueReceive(USAR_RXHandle,&Rx,10);
        xSemaphoreTake(myMutex01Handle,10);
        OLED_ShowHexNum(3,1,Rx,4);
      OLED_ShowNum(4,1,count,4);
      xSemaphoreGive(myMutex01Handle);}
       osDelay(1);
  }
  /* USER CODE END USART_TX */
}

/* USER CODE BEGIN Header_LED_Flash_2 */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED_Flash_2 */
void LED_Flash_2(void *argument)
{
  /* USER CODE BEGIN LED_Flash_2 */
  /* Infinite loop */
  for(;;)
  {

      uint32_t x=xEventGroupWaitBits(myEvent01Handle ,0x01,pdTRUE,pdTRUE,portMAX_DELAY);
          HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_4);


    osDelay(1);
  }
  /* USER CODE END LED_Flash_2 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    BaseType_t *pxHigherPriorityTaskWoken;
    if(huart==&huart1){
        //HAL_UART_Receive_IT(&huart1,);
       static uint8_t uart;
        //  printf("%c\n",uart);
        xQueueSendFromISR(USAR_RXHandle,&uart,pxHigherPriorityTaskWoken);
        HAL_UART_Receive_IT(&huart1,&uart,1);
        xSemaphoreGiveFromISR(UARST_RXHandle,pxHigherPriorityTaskWoken);
    }
}
/* USER CODE END Application */

