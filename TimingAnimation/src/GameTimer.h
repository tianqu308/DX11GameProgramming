#pragma once

class GameTimer
{
public:
  GameTimer();
  virtual ~GameTimer();

  float GameTime()const;// in seconds
  float DeltaTime()const; // in seconds

  void Reset();// Call before message loop
  void Start(); // call when unpaused
  void Stop(); // call when paused
  void Tick(); // call every frame

private:
  double mSecondsPerCount;
  double mDeltaTime;

  __int64 mBaseTime;
  __int64 mPausedTime;
  __int64 mStopTime;
  __int64 mPrevTime;
  __int64 mCurrTime;

  bool mStopped;
};
