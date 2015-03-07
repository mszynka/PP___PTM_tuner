/**
 * @brief Number of tasks on task stack being managed by TaskManager
 */
#ifndef TASK_STACK_SIZE
#define TASK_STACK_SIZE 2
#endif // TASK_STACK_SIZE

/**
 * @brief Timeout needed to switch from one task to another [in ms].
 *        Maximum burst time.
 *
 * WARNING!
 * If too low some tasks may never end therefore it is mandatory to set this value reasonably high
 */
#ifndef TASK_SWITCH_TIMEOUT
#define TASK_SWITCH_TIMEOUT 100
#endif // TASK_SWITCH_TIMEOUT

#ifndef TASKS_HPP
#define TASKS_HPP

#include "error_handlers.hpp"

/**
 * @brief  Callback-style pointer to function
 */
typedef void (*callback_t)(void);

/**
 * @brief  Object for handling tasks in system
 */
class Task
{
	private:
    /**
     * @brief Callable function pointer
     */
		callback_t function_h;

	public:
    /**
     * @brief   Default constructor resets pointer
     * @param   None
     */
		Task();

    /**
     * @brief   Default destructor
     */
		~Task();

    /**
     * @brief   Initializes callback pointer handler
     * @param   callback function pointer handler
     * @retval  None
     */
		void init(callback_t funct);

    /**
     * @brief   Executes funtions stored in callback handler or prints error if no pointer given
     * @param   None
     * @retval  None
     */
		void exec();
};

/**
 * @brief   Task manager stores, manages and executes all tasks in system.
 *          Only one task manager should be running on single board.
 */
class TaskManager
{
	private:
    /**
     * @brief   Task static array constains informations about every task running in system
     */
		Task task_array[TASK_STACK_SIZE];

	public:
};

#endif // TASKS_HPP