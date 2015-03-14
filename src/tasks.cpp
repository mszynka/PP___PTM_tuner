/* LEGACY CODE */

#include "tasks.hpp"

Task::Task()
{
	this->function_h = NULL;
}

Task::~Task(){}

void Task::init(callback_t funct)
{
  this->function_h = funct;
}

void Task::exec()
{
  if(this->function_h != NULL)
  {
    this->function_h();
  }
  else
    mError_Handler(0);
}

TaskManager::TaskManager()
{
  this->taskCounter = 0;
  this->currentTask = 0;
}

TaskManager::~TaskManager()
{
}

void TaskManager::add_task(callback_t funct)
{
  #if TASK_STACK_SIZE < 1
  #error Task stack size too small!
  #else
  this->task_array[this->taskCounter].init(funct);
  this->taskCounter++;
  #endif
}

void TaskManager::scheduler()
{
  while(true){
    this->task_array[this->currentTask].exec();
    if(this->currentTask < this->taskCounter-1) this->currentTask++;
    else this->currentTask = 0;
  }
}