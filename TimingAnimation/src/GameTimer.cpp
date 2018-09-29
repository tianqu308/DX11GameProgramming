#include <windows.h>
#include "GameTimer.h"



GameTimer::GameTimer():
mSecondsPerCount(0.0),
mDeltaTime(-1.0),
mBaseTime(0),
mPausedTime(0),
mPrevTime(0),
mCurrTime(0),
mStopped(false)
{
  __int64 countsPerSec;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  mSecondsPerCount = countsPerSec;
}

GameTimer::~GameTimer()
{

}

float GameTimer::GameTime() const
{

}

float GameTimer::DeltaTime() const
{
  return (float)mDeltaTime;
}

void GameTimer::Reset()
{
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

  mBaseTime = currTime;
  mPrevTime = currTime;
  mStopTime = 0;
  mStopped = false;
}

void GameTimer::Start()
{

}

void GameTimer::Stop()
{

}

void GameTimer::Tick()
{
  if (mStopped)
  {
    mDeltaTime = 0.0;
    return;
  }

  //Get the time this frame
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  mCurrTime = currTime;

  //Time difference between this frame and the previous
  mDeltaTime = (mCurrTime - mPrevTime)* mSecondsPerCount;

  //prepare for next frame
  mPrevTime = mCurrTime;

  //Force nonnegatice. The DXSDK's CDXUTimer memtions that if the processor
  // goes into a power save mode or we get shuffled to another processor,
  // then mDeltaTime can be negatice.
  if (mDeltaTime < 0.0)
  {
    mDeltaTime = 0.0;
  }
}
