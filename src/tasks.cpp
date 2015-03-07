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