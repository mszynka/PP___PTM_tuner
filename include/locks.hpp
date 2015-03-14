#ifndef LOCKS_HPP
#define LOCKS_HPP

#include "error_handlers.hpp"

/**
 * @brief   Task synchronization tool
 *          Use when changing critical shared data
 */
class Lock
{
  private:
    /**
     * @brief   State of the lock
     */
    bool isLocked;

  public:
    /**
     * @brief   Standard constructor
     */
    Lock();
    
    /**
     * @brief   Checks is the lock is locked and if not locks it
     * @param   None
     * @retval  None
     */
    void aquire();

    /**
     * @brief   Releases previously aquired lock
     * @param   None
     * @retval  None
     */
    void release();

    /**
     * @brief   Checks if the lock object is locked
     * @param   None
     * @return  True if is locked, false otherwise.
     */
    bool check();
};

#endif // LOCKS_HPP