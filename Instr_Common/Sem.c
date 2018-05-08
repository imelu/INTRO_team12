/**
 * \file
 * \brief Semaphore usage
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module using semaphores.
 */

/**
 * \file
 * \brief Semaphore usage
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module using semaphores.
 */

#include "Platform.h" /* interface to the platform */
#if PL_CONFIG_HAS_SEMAPHORE
#include "FRTOS1.h"
#include "Sem.h"
#include "LED.h"
#include "CLS1.h"

//static xSemaphoreHandle sem = NULL;
static TaskHandle_t taskHndl;

static void vSlaveTask(void *pvParameters) {
  /*! \todo Implement functionality */
  xSemaphoreHandle sem = (xSemaphoreHandle)pvParameters;

  if (sem==NULL) {
    for(;;) {
      CLS1_SendStr("Outsch!!! Did nobody initialize the semaphore????!", CLS1_GetStdio()->stdErr);
      /* should not be NULL? */
    }
  }
  for(;;) {
    /*! \todo Implement functionality */
    if (xSemaphoreTake(sem, portMAX_DELAY)==pdPASS) { /* block on semaphore */
      LED1_Neg();
    }
  }
}

static void vMasterTask(void *pvParameters) {
  /*! \todo send semaphore from master task to slave task */
  xSemaphoreHandle sem = NULL;

  (void)pvParameters; /* parameter not used */
  sem = xSemaphoreCreateBinary();
  if (sem==NULL) { /* semaphore creation failed */
    CLS1_SendStr("Ups!!! Creating semaphore failed!", CLS1_GetStdio()->stdErr);
    for(;;){} /* error */
  }
  vQueueAddToRegistry(sem, "IPC_Sem");
  /* create slave task */
  if (xTaskCreate(vSlaveTask, "Slave", 400/sizeof(StackType_t), sem, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
    CLS1_SendStr("Dude!!! Did you allocate enough memory for the heap?!!", CLS1_GetStdio()->stdErr);
    for(;;){} /* error */
  }
  for(;;) {
    (void)xSemaphoreGive(sem); /* give control to other task */
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void SEM_Deinit(void) {
  /* \todo caution: would have to delete the semaphores too!!!! */
  vTaskDelete(taskHndl);
}

/*! \brief Initializes module */
void SEM_Init(void) {
  if (xTaskCreate(vMasterTask, "Master", 400/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+1, &taskHndl) != pdPASS) {
    for(;;){} /* error */
  }
}
#endif /* PL_CONFIG_HAS_SEMAPHORE */
