#ifndef TASKS_HPP
#define TASKS_HPP

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

class Task
{
	private:
		static void (*funct)();

	public:
		Task();
		static void initialize(void funct);
};

class TaskManager
{
	private:
		Task* task_array;

	public:
};

#endif