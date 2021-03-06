# TasksEventMillis
TasksEventMillis uses [millis()](https://www.arduino.cc/en/Reference/Millis) as a time base, so it becomes possible to emulate simple threads or perform everyday tasks without using [delay()](https://www.arduino.cc/en/Reference/Delay), so the processor is releasing to perform other tasks in that interval. Compatible with Arduino-Uno, ESP8266 and ESP32 or any platform that supports `malloc` and `realloc` dynamic memory allocation have impementation of function [millis()](https://www.arduino.cc/en/Reference/Millis).

Compatible with the  [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org)

Inspiration Library [TimedEvent](https://github.com/renatoferreirarenatoferreira/ebl-arduino/tree/master/TimedEvent)

# API Reference

This chapter explains the available methods of TasksEventMillis library.

# Methods

## `add`

Add a new task and start it immediately.

```c++
bool add(void (*onEvent)(taskInfo *task), uint32_t intervalMillis, bool enabled = true);
```
- `void (*onEvent)(taskInfo *task)`: Called on `onEvent` events.
- `uint32_t intervalMillis`: Time interval in miliseconds.
- `bool enabled`: Start running. Defaults to `true`.

Return `bool` if task current create with success return `true`, if return `false` something it gave wrong in the task allocation.

#
## `setInterval`

Set a new interval for a given timer.

```c++
void setInterval(void (*func)(taskInfo *task), uint32_t intervalMillis);
```

- `void (*func)(taskInfo *task)`: Associated task ID.
- `uint32_t intervalMillis`: Time interval in miliseconds.

#
## `start`

Start an event task.

```c++
void start(void (*func)(taskInfo *task));
```
- `void (*func)(taskInfo *task)`: Associated task ID.

#
## `stop`

Stop an event task.

```c++
void stop(void (*func)(taskInfo *task));
```
- `void (*func)(taskInfo *task)`: Associated task ID.

#
## `reset`

Reset an event task.

```c++
void reset(void (*func)(taskInfo *task));
```
- `void (*func)(taskInfo *task)`: Associated task ID.

#
## `isEnabled`

The specified task is enabled.

```c++
bool isEnabled(void (*func)(taskInfo *task));
```
- `void (*func)(taskInfo *task)`: Associated task ID.

Return if task is enabled `true` else `false`.

#
## `getNumTasks`

Number of used tasks.

```c++
uint8_t getNumTasks();
```

Return `uint8_t` the number of used tasks.

#
## `handle`

Event handler process loop. Must be added to main `loop()` function.

```c++
void handle();
```
	
# Example

Multitasking task cooperative example for Arduino UNO, ESP8266 or ES32 see more here [examples](examples)

```c++
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
```
# License

The content is licensed under the MIT license. See [License File](LICENSE) for more information.