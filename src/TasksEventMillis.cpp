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

#include "TasksEventMillis.h"

TasksEventMillisClass::TasksEventMillisClass()
{
}

TasksEventMillisClass::~TasksEventMillisClass()
{
	free(tasks);
}

bool TasksEventMillisClass::add(void (*onEvent)(taskInfo *task), uint32_t intervalMillis, bool enabled)
{
	// Check max tasks
	if (_numTasks > TASKS_MAX_BUFFER - 1)
		return false;

	// Alloc dynamically memory
	if (_numTasks > 0)
		tasks = (taskInfo *)realloc(tasks, sizeof(taskInfo) * (_numTasks + 1));
	else
		tasks = (taskInfo *)malloc(sizeof(taskInfo));

	// Error allocation of memory
	if (tasks == NULL)
		return false;

	// Select pointer task in memory
	currentTask = tasks + _numTasks;

	// Set parameters
	currentTask->onEvent = onEvent;
	currentTask->intervalMillis = intervalMillis;
	currentTask->lastEventMillis = _MILLIS();
	currentTask->enabled = enabled;

	// Next index
	_numTasks++;

	return true;
}

bool TasksEventMillisClass::searchTaskId(void (*func)(taskInfo *task))
{
	for (uint8_t index = 0; index < _numTasks; index++)
	{
		// Select pointer task in memory
		currentTask = tasks + index;

		// Check if task id exist in list
		if (currentTask->onEvent == func)
			return true;
	}
	return false;
}

void TasksEventMillisClass::setInterval(void (*func)(taskInfo *task), uint32_t intervalMillis)
{
	if (searchTaskId(func))
		currentTask->intervalMillis = intervalMillis;
}

void TasksEventMillisClass::start(void (*func)(taskInfo *task))
{
	if (searchTaskId(func))
	{
		if (!currentTask->enabled)
			currentTask->lastEventMillis = _MILLIS();
		currentTask->enabled = true;
	}
}

void TasksEventMillisClass::stop(void (*func)(taskInfo *task))
{
	if (searchTaskId(func))
		currentTask->enabled = false;
}

void TasksEventMillisClass::reset(void (*func)(taskInfo *task))
{
	if (searchTaskId(func))
		currentTask->lastEventMillis = _MILLIS();
}

bool TasksEventMillisClass::isEnabled(void (*func)(taskInfo *task))
{
	if (searchTaskId(func))
		return currentTask->enabled;
	return false;
}

uint8_t TasksEventMillisClass::getNumTasks()
{
	return _numTasks;
}

void TasksEventMillisClass::handle()
{
	for (uint32_t index = 0; index < _numTasks; index++)
	{
		// Select pointer task in memory
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
