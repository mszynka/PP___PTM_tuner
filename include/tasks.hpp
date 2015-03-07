/*
	Number of tasks on task stack being managed by TaskManager
 */
#ifndef TASK_STACK_SIZE
#define TASK_STACK_SIZE 2
#endif

/*
	Timeout needed to switch from one task to another [in ms].
	Maximum burst time.

	WARNING!
	If too low some tasks may never end therefore it is mandatory to set this value reasonably high
 */
#ifndef TASK_SWITCH_TIMEOUT
#define TASK_SWITCH_TIMEOUT 100
#endif

#ifndef TASKS_HPP
#define TASKS_HPP

typedef void (*callback_t)(void);

class Task
{
	private:
		callback_t p_function;

	public:
		Task()
		{
			this->p_function = NULL;
		}

		~Task(){}

		void init(callback_t funct)
		{
			this->p_function = funct;
		}

		void exec()
		{
			if(this->p_function!=NULL)
				this->p_function();
			else
				Error_Handler();
		}
};

class TaskManager
{
	private:
		Task task_array[TASK_STACK_SIZE];

	public:
};

#endif