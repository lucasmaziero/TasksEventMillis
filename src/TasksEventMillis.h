/**************************************************************************
Tasks event scheduler based on time intervals
***************************************************************************
AUTHOR: Lucas Maziero - Electrical Engineer
EMAIL: lucas.mazie.ro@hotmail.com or lucasmaziero@foxiot.com.br
CITY: Santa Maria - Rio Grande do Sul - Brasil
FUNTATION: Fox IoT (www.foxiot.com.br)
***************************************************************************
Version: 1.0.1
Date: 23/09/2021
Modified: 12/11/2021
***************************************************************************
Code base: https://github.com/renatoferreirarenatoferreira/ebl-arduino/tree/master/TimedEvent
***************************************************************************
CHANGELOG:
* 23/09/2021 (1.0.0v):
    -> Added improvements and simplifications;
* 12/11/2021 (1.0.1v):
    -> Now taskId is the function itself;

***************************************************************************
Copyright(c) by: Lucas Maziero.
**************************************************************************/

#ifndef TasksEventMillis_h
#define TasksEventMillis_h

#include <Arduino.h>
#include <stdlib.h>

#define TASKS_MAX_BUFFER 15
#define _MILLIS() millis()

#define TASK_ADD_FAIL -1

#define TASK_IMMEDIATE 0
#define TASK_MILLISECOND 1UL
#define TASK_SECOND 1000UL
#define TASK_MINUTE 60000UL
#define TASK_HOUR 3600000UL

struct taskInfo
{
	void (*onEvent)(taskInfo *task);
	uint32_t intervalMillis;
	uint32_t lastEventMillis;
	bool enabled;
};

class TasksEventMillisClass
{
public:
	/**************************************************************************
	 Functions public
  	***************************************************************************/
	TasksEventMillisClass();
	virtual ~TasksEventMillisClass();
	bool add(void (*onEvent)(taskInfo *task), uint32_t intervalMillis, bool enabled = true);
	void setInterval(void (*func)(taskInfo *task), uint32_t intervalMillis);
	void start(void (*func)(taskInfo *task));
	void stop(void (*func)(taskInfo *task));
	void reset(void (*func)(taskInfo *task));
	bool isEnabled(void (*func)(taskInfo *task));
	uint8_t getNumTasks();
	void handle();

private:
	/**************************************************************************
	 Functions private
  	***************************************************************************/
	bool searchTaskId(void (*func)(taskInfo *task));

	/**************************************************************************
	 variables private
	***************************************************************************/
	uint8_t _numTasks = 0;
	taskInfo *tasks = NULL, *currentTask = NULL;
};

// Global instance
extern TasksEventMillisClass TasksEventMillis;

#endif /* TasksEventMillis_h */
