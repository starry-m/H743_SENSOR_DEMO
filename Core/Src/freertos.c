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
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
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
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void TCPServerTask(void const * argument);

//#include "tcp_server_test.h"
#include "lwip/opt.h"
#include <lwip/sockets.h>
#include "lwip/sys.h"
#include "lwip/api.h"
#include "string.h"

#if LWIP_SOCKET	//需要开启Scoket才能使用

#define RECV_DATA         	(1024UL)
#define LOCAL_PORT 			(6133UL)
#define BACKLOG 			(5UL)/*最大监听数*/

#define LWIP_TCP_DEBUG_ENABLE    1

#if LWIP_TCP_DEBUG_ENABLE
   #define LWIP_TCP_DEBUG	printf
#else
  #define LWIP_TCP_DEBUG(...)
#endif


static void tcp_server_thread(void *arg)
{
  int sockfd = -1,connected; /*socket句柄和建立连接后的句柄*/
  struct sockaddr_in server_addr,client_addr;
  socklen_t sin_size;
  int recv_data_len;
  uint8_t recv_data[RECV_DATA];
  osDelay(1000);
  LWIP_TCP_DEBUG("tcp server port %d\n\n",LOCAL_PORT);

again:

  //创建scoket描述符 AF_INET 使用ipv4地址
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
      LWIP_TCP_DEBUG("Socket error\n");
	  close(sockfd);
	  vTaskDelay(100);
      goto again;
  }
  //
  server_addr.sin_family = AF_INET; 				//该属性表示接收本机或其他机器传输
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//本机IP
  server_addr.sin_port = htons(LOCAL_PORT);			//端口号
  memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
  {
      LWIP_TCP_DEBUG("Unable to bind\n");
	  close(sockfd);
	  vTaskDelay(100);
      goto again;
  }

  //设置最大监听数
  if (listen(sockfd, BACKLOG) == -1)
  {
      LWIP_TCP_DEBUG("Listen error\n");
	  close(sockfd);
	  vTaskDelay(100);
      goto again;
  }

  while(1)
  {
    sin_size = sizeof(struct sockaddr_in);

	//在这里阻塞知道接收到消息，参数分别是socket句柄，接收到的地址信息以及大小
    connected = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);

    LWIP_TCP_DEBUG("new client connected from (%s, %d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    int tcp_nodelay = 1;//don't delay send to coalesce packets
    setsockopt(connected,IPPROTO_TCP,TCP_NODELAY,(void *) &tcp_nodelay,sizeof(int));

    while(1)
    {
      recv_data_len = recv(connected, recv_data, RECV_DATA, 0);

      if (recv_data_len <= 0)
	  {
		   break;
	  }


      LWIP_TCP_DEBUG("recv %d len data\n",recv_data_len);
	  //发送内容
      write(connected,recv_data,recv_data_len);
    }

    if (connected >= 0)
	{
		 close(connected);
	}

    connected = -1;
  }

  if (sockfd >= 0)
  {
	 close(sockfd);
  }

}
#endif

void tcp_server_init(void)
{
#if LWIP_SOCKET
  sys_thread_new("tcpecho_thread", tcp_server_thread, NULL, 512, 4);
#endif
}


/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
//  osThreadDef(myTask02, TCPServerTask, osPriorityNormal, 0, 1024);
//  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);


  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  tcp_server_init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	  MX_MEMS_Process();
	  osDelay(5);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Header_TCPServerTask */
void TCPServerTask(void const * argument)
{
  /* USER CODE BEGIN TCPServerTask */
	osDelay(1000);

  struct netconn *conn, *newconn;
  err_t err;

  LWIP_UNUSED_ARG(argument);

  /* Create a new connection identifier. */
  /* Bind connection to well known port number 7. */
  conn = netconn_new(NETCONN_TCP);
  netconn_bind(conn, IP_ADDR_ANY, 5101);

  LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);

  /* Tell connection to go into listening mode. */
  netconn_listen(conn);

  /* Infinite loop */
  for(;;)
  {

    /* Grab new connection. */
//	  osDelay(1000);
	  printf("TCPServerTask\n");
    err = netconn_accept(conn, &newconn);

    /*printf("accepted new connection %p\n", newconn);*/
    /* Process the new connection. */
    if (err == ERR_OK) {
      struct netbuf *buf;
      void *data;
      uint8_t rcvbuf[64];
      u16_t len;

      while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {
        /*printf("Recved\n");*/
        do {
             netbuf_data(buf, &data, &len);
             err = netconn_write(newconn, data, len, NETCONN_COPY);
        } while (netbuf_next(buf) >= 0);
        netbuf_delete(buf);
      }
      /*printf("Got EOF, looping\n");*/
      /* Close connection and discard connection identifier. */
      netconn_close(newconn);
      netconn_delete(newconn);
    }

    osDelay(500);
  } // loop
  /* USER CODE END TCPServerTask */
}
/* USER CODE END Application */
