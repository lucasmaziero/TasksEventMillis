/**************************************************************************
Tasks event scheduler based on time intervals
***************************************************************************
AUTHOR: Lucas Maziero - Electrical Engineer
EMAIL: lucas.mazie.ro@hotmail.com or lucasmaziero@foxiot.com.br
CITY: Santa Maria - Rio Grande do Sul - Brasil
FUNTATION: Fox IoT (www.foxiot.com.br)
***************************************************************************
Version: 1.0.0
Date: 23/09/2021
Modified: xx/xx/20xx
***************************************************************************
Code base: https://github.com/renatoferreirarenatoferreira/ebl-arduino/tree/master/TimedEvent
***************************************************************************
CHANGELOG:
* 22/09/2021 (1.0.0v):
    -> Added improvements and simplifications;

***************************************************************************
Copyright(c) by: Lucas Maziero.
**************************************************************************/

#ifndef TasksEventMillis_h
#define TasksEventMillis_h

#include <Arduino.h>

#define TASKS_MAX_BUFFER 15
#define _MILLIS() millis()

#define TASK_ADD_FAIL -1

#define TASK_IMMEDIATE 0
#define TASK_MILLISECOND 1UL
#define TASK_SECOND 1000UL
#define TASK_MINUTE 60000UL
#define TASK_HOUR 3600000UL

struct TasksInfo
{
	uint8_t taskId;
	bool enabled;
	uint32_t intervalMillis;
	uint32_t lastEventMillis;
	void (*onEvent)(TasksInfo *task);
};

class TasksEventMillisClass
{
public:
	/**************************************************************************
	 Functions public
  	***************************************************************************/
	TasksEventMillisClass();
	virtual ~TasksEventMillisClass();
	int8_t add(void (*onEvent)(TasksInfo *task), uint32_t intervalMillis, bool enabled = true);
	void setInterval(uint8_t id, uint32_t intervalMillis);
	void start(uint8_t id);
	void stop(uint8_t id);
	void reset(uint8_t id);
	bool isEnabled(uint8_t id);
	uint8_t getNumTasks();
	void handle();

private:
	/**************************************************************************
	 variables private
	***************************************************************************/
	uint8_t _count = 0;
	TasksInfo *tasks = NULL, *currentTask = NULL;
};

// Global instance
extern TasksEventMillisClass TasksEventMillis;

#endif  /* TasksEventMillis_h */
