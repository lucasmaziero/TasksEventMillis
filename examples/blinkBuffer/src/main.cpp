/**************************************************************************
  SIMPLE TASKS BLINK LED WITH BUFFER
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
#include <CircularBuffer.h> // https://github.com/rlogiacco/CircularBuffer

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

#define LED_STATE_IDE 0
#define LED_STATE_ON 1
#define LED_STATE_OFF 2

// state blink LED
bool status = false;

/**************************************************************************
  INSTANCE OF OBJECTS
**************************************************************************/
// Create CircularBuffer Object
CircularBuffer<uint8_t, 10> ringbuffer;

/**************************************************************************
  PROTOTYPE OF FUNCTIONS
**************************************************************************/
void taskQueueSend(taskInfo *task);
uint8_t queueReceive(uint8_t valueDefault);
void queueSend(uint8_t value);
void queueDispatcher();

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
  TasksEventMillis.add(taskQueueSend, 0.5 * TASK_SECOND);

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
  TasksEventMillis.handle(); // TasksEventMillis handle
  queueDispatcher();         // FSM dispatcher
}

/**************************************************************************
  FUNCTION BLINK LED
**************************************************************************/
void taskQueueSend(taskInfo *task)
{
  queueSend(status ? LED_STATE_OFF : LED_STATE_ON);
  status = !status;
}

/**************************************************************************
  FUNCTIONS QUEUE CONTROLLER (FSM)
**************************************************************************/
uint8_t queueReceive(uint8_t valueDefault)
{
  // If buffer is empty and use "shift()" or "pop()" will crash the firmware (https://github.com/rlogiacco/CircularBuffer/blob/master/examples/Test/Test.ino#L180)
  return ((!ringbuffer.isEmpty()) ? ringbuffer.shift() : valueDefault);
}

void queueSend(uint8_t value)
{
  ringbuffer.push(value);
}

void queueDispatcher()
{
  // FSM queue select
  switch (queueReceive(LED_STATE_IDE))
  {
  case LED_STATE_ON: // LED (ON)
    digitalWrite(PIN_LED, HIGH);
    DebugPrintln("LED : ON");
    break;
  case LED_STATE_OFF: // LED (OFF)
    digitalWrite(PIN_LED, LOW);
    DebugPrintln("LED : OFF");
    break;
  default: // State IDE
    break;
  }
}