/**
 * \file
 * \brief Real Time Operating System (RTOS) main program.
 * \author Erich Styger, erich.styger@hslu.ch
 */

#include "Platform.h"
#if PL_CONFIG_HAS_RTOS
#include "RTOS.h"
#include "FRTOS1.h"
#include "Application.h"
#include "LED.h"

#if 0
static void Blinky(void *param) {
  for(;;) {
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
#endif

void RTOS_Init(void) {
  /*! \todo Create tasks here */
#if 0
  if (xTaskCreate(Blinky, "notBlinky", 300/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
    for(;;){} /* error */
  }
#endif
}

void RTOS_Deinit(void) {
  /* nothing needed for now */
}

#endif /* PL_CONFIG_HAS_RTOS */
