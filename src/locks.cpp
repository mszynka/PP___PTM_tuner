#include "locks.hpp"

Lock::Lock()
{
  this->isLocked = false;
}

void Lock::aquire()
{
  while(this->isLocked)
  {
    for(int i=0; i<9000000;i++){}
    if(this->isLocked)
    {
      mError_Handler(-1);
    }
  }
  if(!this->isLocked)
  {
    this->isLocked = true;
  }
}

void Lock::release()
{
  this->isLocked = false;
}

bool Lock::check()
{
  return this->isLocked;
}