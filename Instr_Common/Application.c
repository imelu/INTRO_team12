/**
 * \file
 * \brief Main application file
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This provides the main application entry point.
 */

#include "Platform.h"
#include "Application.h"
#include "Event.h"
#include "LED.h"
#include "WAIT1.h"
#include "CS1.h"
#include "KeyDebounce.h"
#include "CLS1.h"
#include "KIN1.h"
#if PL_CONFIG_HAS_KEYS
  #include "Keys.h"
#endif
#if PL_CONFIG_HAS_SHELL
  #include "CLS1.h"
  #include "Shell.h"
  #include "RTT1.h"
#endif
#if PL_CONFIG_HAS_BUZZER
  #include "Buzzer.h"
#endif
#if PL_CONFIG_HAS_RTOS
  #include "FRTOS1.h"
  #include "RTOS.h"
#endif
#if PL_CONFIG_HAS_QUADRATURE
  #include "Q4CLeft.h"
  #include "Q4CRight.h"
#endif
#if PL_CONFIG_HAS_MOTOR
  #include "Motor.h"
#endif
#if PL_CONFIG_BOARD_IS_ROBO_V2
  #include "PORT_PDD.h"
#endif
#if PL_CONFIG_HAS_LINE_FOLLOW
  #include "LineFollow.h"
#endif
#if PL_CONFIG_HAS_LCD_MENU
  #include "LCD.h"
#endif
#if PL_CONFIG_HAS_SNAKE_GAME
  #include "Snake.h"
#endif
#if PL_CONFIG_HAS_REFLECTANCE
  #include "Reflectance.h"
#endif
#if PL_CONFIG_HAS_MOTOR_TACHO
  #include "Tacho.h"
#endif
#if PL_CONFIG_HAS_LCD
  #include "Lcd.h"
#endif
#include "FRTOS1.h"

#if PL_CONFIG_HAS_EVENTS

static void BtnMsg(int btn, const char *msg) {
#if PL_CONFIG_HAS_SHELL
  #if PL_CONFIG_HAS_SHELL_QUEUE
    uint8_t buf[48];

    UTIL1_strcpy(buf, sizeof(buf), "Button pressed: ");
    UTIL1_strcat(buf, sizeof(buf), msg);
    UTIL1_strcat(buf, sizeof(buf), ": ");
    UTIL1_strcatNum32s(buf, sizeof(buf), btn);
    UTIL1_strcat(buf, sizeof(buf), "\r\n");
    SHELL_SendString(buf);
  #else
    CLS1_SendStr("Button pressed: ", CLS1_GetStdio()->stdOut);
    CLS1_SendStr(msg, CLS1_GetStdio()->stdOut);
    CLS1_SendStr(": ", CLS1_GetStdio()->stdOut);
    CLS1_SendNum32s(btn, CLS1_GetStdio()->stdOut);
    CLS1_SendStr("\r\n", CLS1_GetStdio()->stdOut);
  #endif
#endif
}

/*#if PL_CONFIG_HAS_MOTOR
static bool EnginesRunning = FALSE;

static void StartEngines(void) {
  if (!REF_IsCalibrated()) {
    SHELL_SendString("You need to calibrate the reflectance sensor first!\r\n");
    return;
  }
  if (REF_GetLineKind()==REF_LINE_STRAIGHT) {
    SHELL_SendString("Starting the engines!\r\n");
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), 20);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), 20);
    EnginesRunning = TRUE;
  } else {
    SHELL_SendString("Place the robot on a line first!\r\n");
  }
}
#endif*/
/*
#if PL_CONFIG_HAS_MOTOR
static void StopEngines(void) {
  MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), 0);
  MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), 0);
  //EnginesRunning = FALSE;
}
#endif*/

void APP_EventHandler(EVNT_Handle event) {
  /*! \todo handle events */
  switch(event) {
  case EVNT_STARTUP:
    {
      int i;
      for (i=0;i<5;i++) {
        LED1_Neg();
        WAIT1_Waitms(50);
      }
      LED1_Off();
#if PL_CONFIG_HAS_BUZZER
     BUZ_PlayTune(BUZ_TUNE_WELCOME);
#endif
    }
  case EVNT_LED_HEARTBEAT:
    LED1_Neg();
    break;

#if PL_CONFIG_NOF_KEYS>=1
  case EVNT_SW1_PRESSED:
    BtnMsg(1, "pressed");
    if (REF_IsCalibrated()){
    	WAIT1_WaitOSms(1000);
    	LF_StartFollowing();
    }
#if PL_CONFIG_HAS_BUZZER
     BUZ_Beep(500, 500);
#endif
#if PL_CONFIG_HAS_MOTOR
     WAIT1_WaitOSms(500); /* give user time to remove the finger from the button */
     /*if (EnginesRunning) {
       StopEngines();
     } else {
       StartEngines();
     }*/
#endif
#if PL_CONFIG_HAS_LCD
     LCD_SetEvent(LCD_BTN_RIGHT);
#endif
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=1
  case EVNT_SW1_LPRESSED:
    BtnMsg(1, "lpressed");
#if PL_CONFIG_HAS_BUZZER
     BUZ_Beep(500, 500);
#endif
#if PL_CONFIG_HAS_REFLECTANCE
     WAIT1_WaitOSms(1000); /* give user time to remove the finger from the button */
     REF_CalibrateStartStop();
#endif
     break;
#endif

#if PL_CONFIG_NOF_KEYS>=1
  case EVNT_SW1_RELEASED:
     BtnMsg(1, "released");
#if PL_CONFIG_HAS_BUZZER
     BUZ_Beep(500, 500);
#endif
     break;
#endif


#if PL_CONFIG_NOF_KEYS>=2
  case EVNT_SW2_PRESSED:
     BtnMsg(2, "pressed!\r\n");
#if PL_CONFIG_HAS_LCD
     LCD_SetEvent(LCD_BTN_LEFT);
#endif
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=3
  case EVNT_SW3_PRESSED:
     BtnMsg(3, "pressed!\r\n");
#if PL_CONFIG_HAS_LCD
     LCD_SetEvent(LCD_BTN_DOWN);
#endif
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=4
  case EVNT_SW4_PRESSED:
     BtnMsg(4, "pressed!\r\n");
#if PL_CONFIG_HAS_LCD
     LCD_SetEvent(LCD_BTN_CENTER);
#endif
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=5
  case EVNT_SW5_PRESSED:
     BtnMsg(5, "pressed!\r\n");
#if PL_CONFIG_HAS_LCD
     LCD_SetEvent(LCD_BTN_UP);
#endif
     break;
#endif

#if PL_CONFIG_NOF_KEYS>=6
  case EVNT_SW6_PRESSED:
     BtnMsg(6, "pressed!\r\n");
#if PL_CONFIG_HAS_LCD
     LCD_SetEvent(LCD_SIDE_BTN_DOWN);
#endif
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=6
  case EVNT_SW6_LPRESSED:
     BtnMsg(6, "long pressed!\r\n");
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=6
  case EVNT_SW6_RELEASED:
     BtnMsg(6, "released!\r\n");
     break;
#endif

#if PL_CONFIG_NOF_KEYS>=7
  case EVNT_SW7_PRESSED:
     BtnMsg(7, "pressed!\r\n");
#if PL_CONFIG_HAS_LCD
     LCD_SetEvent(LCD_BTN_UP);
#endif
     break;
#endif
    default:
      break;
   } /* switch */
}
#endif /* PL_CONFIG_HAS_EVENTS */

#if PL_CONFIG_HAS_MOTOR /* currently only used for robots */
static const KIN1_UID RoboIDs[] = {
  /* 0: L20, V2 */ {{0x00,0x03,0x00,0x00,0x67,0xCD,0xB7,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 1: L21, V2 */ {{0x00,0x05,0x00,0x00,0x4E,0x45,0xB7,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 2: L4, V1  */ {{0x00,0x0B,0xFF,0xFF,0x4E,0x45,0xFF,0xFF,0x4E,0x45,0x27,0x99,0x10,0x02,0x00,0x24}},
  /* 3: L23, V2 */ {{0x00,0x0A,0x00,0x00,0x67,0xCD,0xB8,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 4: L11, V2 */ {{0x00,0x19,0x00,0x00,0x67,0xCD,0xB9,0x11,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 5: L5, V2 */  {{0x00,0x38,0x00,0x00,0x67,0xCD,0xB5,0x41,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 6: L3, V1 */  {{0x00,0x33,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x4E,0x45,0x27,0x99,0x10,0x02,0x00,0x0A}},
  /* 7: L1, V1 */  {{0x00,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x4E,0x45,0x27,0x99,0x10,0x02,0x00,0x25}},
  /* 8: L7 V1 */   {{0x00,0x20,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x4E,0x45,0x27,0x99,0x10,0x02,0x00,0x07}},
  /* 9: L12, V2 */ {{0x00,0x34,0x00,0x00,0x67,0xCD,0xB7,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x14}},
};
#endif

static void APP_AdoptToHardware(void) {
  KIN1_UID id;
  uint8_t res;

  res = KIN1_UIDGet(&id);
  if (res!=ERR_OK) {
    for(;;); /* error */
  }
#if PL_CONFIG_HAS_MOTOR
  if (KIN1_UIDSame(&id, &RoboIDs[0])) { /* L20 */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CRight_SwapPins(TRUE);
#endif
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* invert left motor */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), TRUE); /* invert left motor */
  } else if (KIN1_UIDSame(&id, &RoboIDs[1])) { /* V2 L21 */
    /* no change needed */
  } else if (KIN1_UIDSame(&id, &RoboIDs[2])) { /* V1 L4 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* revert left motor */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CLeft_SwapPins(TRUE);
    (void)Q4CRight_SwapPins(TRUE);
#endif
  } else if (KIN1_UIDSame(&id, &RoboIDs[3])) { /* L23 */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CRight_SwapPins(TRUE);
#endif
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* invert left motor */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), TRUE); /* invert left motor */
  } else if (KIN1_UIDSame(&id, &RoboIDs[4])) { /* L11 */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CRight_SwapPins(TRUE);
#endif
  } else if (KIN1_UIDSame(&id, &RoboIDs[5])) { /* L5, V2 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), TRUE); /* invert right motor */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CRight_SwapPins(TRUE);
#endif
  } else if (KIN1_UIDSame(&id, &RoboIDs[6])) { /* L3, V1 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* invert right motor */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CLeft_SwapPins(TRUE);
    (void)Q4CRight_SwapPins(TRUE);
#endif
  } else if (KIN1_UIDSame(&id, &RoboIDs[7])) { /* L1, V1 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* invert right motor */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CLeft_SwapPins(TRUE);
    (void)Q4CRight_SwapPins(TRUE);
#endif
  } else if (KIN1_UIDSame(&id, &RoboIDs[8])) { /* L7, V1 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* invert right motor */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CLeft_SwapPins(TRUE);
    (void)Q4CRight_SwapPins(FALSE);
#endif
  } else if (KIN1_UIDSame(&id, &RoboIDs[8])) { /* L12, V2 */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CRight_SwapPins(TRUE);
#endif
}
#endif

#if PL_CONFIG_HAS_QUADRATURE && PL_CONFIG_BOARD_IS_ROBO_V2
  /* pull-ups for Quadrature Encoder Pins */
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 10, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 10, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 11, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 11, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 16, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 16, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 17, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 17, PORT_PDD_PULL_ENABLE);
#endif
}

static void Blinky(void *param) {
  for(;;) {
    LED1_Neg();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

static void AppTask(void *param) {
#if PL_CONFIG_HAS_SHELL
  SHELL_SendString("Hello World!\r\n");
#endif
  EVNT_SetEvent(EVNT_STARTUP);
  for(;;) {
#if PL_CONFIG_HAS_KEYS
  #if PL_CONFIG_HAS_DEBOUNCE
    KEYDBNC_Process();
  #else
    KEY_Scan();
  #endif
#endif
    while(EVNT_HandleEvent(APP_EventHandler, TRUE)) {};
#if PL_CONFIG_HAS_MOTOR
    if (REF_GetLineKind()!=REF_LINE_STRAIGHT) {
      //StopEngines();
    }
    TACHO_CalcSpeed();
#endif
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void APP_Start(void) {
  //int cntr = 0;

  PL_Init();
  APP_AdoptToHardware();

  if (xTaskCreate(AppTask, "App", 500/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
    for(;;){} /* error */
  }
  if (xTaskCreate(Blinky, "Blinky", 300/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
    for(;;){} /* error */
  }
  vTaskStartScheduler(); /* does usually not return */
  for(;;) { /* just in case... */}
#if 0
  __asm volatile("cpsie i"); /* enable interrupts */

  EVNT_SetEvent(EVNT_STARTUP);
#if PL_CONFIG_HAS_SHELL
  SHELL_SendString("Hello World!\r\n");
#endif
  for(;;) {
#if PL_CONFIG_HAS_KEYS
  #if PL_CONFIG_HAS_DEBOUNCE
    KEYDBNC_Process();
  #else
    KEY_Scan();
  #endif
#endif
    EVNT_HandleEvent(APP_EventHandler, TRUE);
#if 1
    if (CLS1_GetStdio()->keyPressed()>0) {
      uint8_t ch;

      CLS1_GetStdio()->stdIn(&ch);
      if (ch!='\0') { /* just for safety */
        CLS1_GetStdio()->stdOut(ch);
      }
    }
#endif
#if 0
    LED1_Put((cntr%10)==0);
    cntr++;

    WAIT1_Waitms(100);

    LED2_On(); /* right LED on robot */
    WAIT1_Waitms(100);
    LED2_Off();
    WAIT1_Waitms(100);
    LED2_Neg();
#endif
    WAIT1_Waitms(5);
#if PL_CONFIG_HAS_LEDS && PL_CONFIG_NOF_LEDS>=3
    LED3_On();
    WAIT1_Waitms(100);
    LED3_Off();
#endif
    __asm("nop");
  }
#endif
}


