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

#include "TasksEventMillis.h"

TasksEventMillisClass::TasksEventMillisClass()
{

}

TasksEventMillisClass::~TasksEventMillisClass()
{
	free(tasks);
}

int8_t TasksEventMillisClass::add(void (*onEvent)(TasksInfo *task), uint32_t intervalMillis, bool enabled)
{
	// Check max tasks
	if (_count > TASKS_MAX_BUFFER - 1)
		return -1;

	// Alloc dynamically memory
	if (_count > 0)
		tasks = (TasksInfo *)realloc(tasks, sizeof(TasksInfo) * (_count + 1));
	else
		tasks = (TasksInfo *)malloc(sizeof(TasksInfo));

	// Error allocation of memory
	if (tasks == NULL)
		return -1;

	// Select pointer timer in memory
	currentTask = tasks + _count;

	// Set parameters
	currentTask->taskId = _count;
	currentTask->intervalMillis = intervalMillis;
	currentTask->onEvent = onEvent;
	currentTask->enabled = enabled;
	currentTask->lastEventMillis = _MILLIS();

	// Next index
	_count++;

	return currentTask->taskId;
}

void TasksEventMillisClass::setInterval(uint8_t id, uint32_t intervalMillis)
{
	currentTask = tasks + id;
	currentTask->intervalMillis = intervalMillis;
}

void TasksEventMillisClass::start(uint8_t id)
{
	currentTask = tasks + id;
	if (!currentTask->enabled)
		currentTask->lastEventMillis = _MILLIS();
	currentTask->enabled = true;
}

void TasksEventMillisClass::stop(uint8_t id)
{
	currentTask = tasks + id;
	currentTask->enabled = false;
}

void TasksEventMillisClass::reset(uint8_t id)
{
	currentTask = tasks + id;
	currentTask->lastEventMillis = _MILLIS();
}

bool TasksEventMillisClass::isEnabled(uint8_t id)
{
	currentTask = tasks + id;
	return currentTask->enabled;
}

uint8_t TasksEventMillisClass::getNumTasks()
{
	return _count;
}

void TasksEventMillisClass::handle()
{
	for (uint8_t index = 0; index < _count; index++)
	{
		// Select pointer timer in memory
		currentTask = tasks + index;

		if (currentTask->enabled)
		{
			uint32_t lastMillis = _MILLIS();

			// Check for millis() trigger event, with avoiding the overflow of millis() (rollover)
			// https://www.norwegiancreations.com/2018/10/arduino-tutorial-avoiding-the-overflow-issue-when-using-millis-and-micros
			if (lastMillis - currentTask->lastEventMillis > currentTask->intervalMillis)
			{
				currentTask->lastEventMillis = lastMillis;
				currentTask->onEvent(currentTask);
			}
		}
	}
}

TasksEventMillisClass TasksEventMillis;
