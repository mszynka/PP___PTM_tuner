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
    //TODO: enable timer
    this->function_h();
    //TODO: disable timer
  }
  else
    mError_Handler(0);
}

TaskManager::TaskManager()
{
  this->taskCounter = 0;
}

TaskManager::~TaskManager()
{
}

void TaskManager::add_task(callback_t funct)
{
  #if TASK_STACK_SIZE <= 1
  #error Task stack size too small!
  #else
  this->task_array[this->taskCounter].init(funct);
  this->taskCounter++;  
  #endif
}

void TaskManager::scheduler()
{
  //TODO: write scheduler
}