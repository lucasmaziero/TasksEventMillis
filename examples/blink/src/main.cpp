/**************************************************************************
  SIMPLE TASKS BLINK LED AND PRINT UART
***************************************************************************
  AUTHOR: LUCAS MAZIERO - Electrical Engineer
  EMAIL: lucas.mazie.ro@hotmail.com or lucasmaziero@foxiot.com.br
  CITY: Santa Maria - Rio Grande do Sul - Brasil
  FOUNDATION: Fox IoT
**************************************************************************
  Version: 1.0
  Date: 23/10/2021
  Modified: xx/xx/20xx
*************************************************************************
 Copyright(c) by: Fox IoT.
**************************************************************************/
/**************************************************************************
  AUXILIARY LIBRARIES
**************************************************************************/
#include <Arduino.h>
#include <TasksEventMillis.h>

/**************************************************************************
  FIRMWARE VERSION x.x
**************************************************************************/
#define FIRMWARE_VERSION "1.0" // Firmware version

/**************************************************************************
  DEBUGGING
**************************************************************************/
#define DEBUG

#if defined(DEBUG)
#define DEBUG_UART Serial
#define DebugPrint(...) (DEBUG_UART.print(__VA_ARGS__))
#define DebugPrintln(...) (DEBUG_UART.println(__VA_ARGS__))
#define DebugBegin(...) (DEBUG_UART.begin(__VA_ARGS__))
#define DebugPrintHEX(data, length)       \
  {                                       \
    uint8_t *dataPointer = (data);        \
    for (uint16_t i = 0; i < length; i++) \
    {                                     \
      if (dataPointer[i] < 0x10)          \
        DebugPrint(F("0"));               \
      DebugPrint(dataPointer[i], HEX);    \
    }                                     \
  }
#else
#define DebugPrint(...)
#define DebugPrintln(...)
#define DebugBegin(...)
#define DebugPrintHEX(data, length)
#endif

/**************************************************************************
  VARIABLES AND DEFINITIONS
**************************************************************************/
// Pin mapping LED
#define PIN_LED 2

// Counter blink LED
uint32_t counter = 0;

/**************************************************************************
  INSTANCE OF OBJECTS
**************************************************************************/

/**************************************************************************
  PROTOTYPE OF FUNCTIONS
**************************************************************************/
void blink(taskInfo *task);
void timePrint(taskInfo *task);

/**************************************************************************
  FUNCTION SETUP
**************************************************************************/
void setup()
{
  // Set Pins
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  // Init Serial
  DebugBegin(115200);

  // Create TasksEventMillis
  TasksEventMillis.add(blink, 0.1 * TASK_SECOND);
  TasksEventMillis.add(timePrint, 1 * TASK_SECOND);

  DebugPrint("\nTask blink with ");
#if defined(ARDUINO_ARCH_ESP32)
  DebugPrint("ESP32");
#elif defined(ARDUINO_ARCH_ESP8266)
  DebugPrint("ESP8266");
#elif defined(ARDUINO_ARCH_AVR)
  DebugPrint("AVR");
#endif

  DebugPrintln(" v" FIRMWARE_VERSION);

  DebugPrint("Number of Tasks: " + String(TasksEventMillis.getNumTasks()));
  DebugPrintln();
}

/**************************************************************************
  FUNCTION LOOP
**************************************************************************/
void loop()
{
  TasksEventMillis.handle();
}

/**************************************************************************
  FUNCTION BLINK LED
**************************************************************************/
void blink(taskInfo *task)
{
  bool status = digitalRead(PIN_LED);
  digitalWrite(PIN_LED, !status);

  DebugPrint("LED(");
  DebugPrint(counter);
  DebugPrint("): ");
  DebugPrintln(status ? "OFF" : "ON");

  if (counter > 49)
  {
    task->enabled = false;
    //TasksEventMillis.stop(blink);
    digitalWrite(PIN_LED, LOW);
    DebugPrintln("Task with id " + String((uint32_t)task->onEvent, HEX) + " is stoped!");
  }
  counter++;
}

/**************************************************************************
  FUNCTION PRINT TIMER
**************************************************************************/
void timePrint(taskInfo *task)
{
  DebugPrint("Time: ");
  DebugPrint(millis() / 1000);
  DebugPrintln("s");
}