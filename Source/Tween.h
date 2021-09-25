#pragma once

#include "Easing.h"



class Tween
{
public:
	Tween(InterpFunction function, float& value, float startValue, float endValue, float duration, float startTime = 0.0f);

	void Update(float deltaTime);

	void Start();
	void Reset();
	void Restart();
	void Reverse();
	void StartAndReverse();

	bool IsRunning(); 
	bool IsFinished();
	bool IsReverse();
	float GetRemainingTime();

	void SetStartTime(float newTime)		{ m_ElapsedTime = m_StartValue; }
	void SetDuration(float newDuration)		{ m_Duration = newDuration;		}
	void SetStartValue(float newStartValue) { m_StartValue = newStartValue; }
	void SetEndValue(float newEndValue)		{ m_EndValue = newEndValue;		}
	void SetIsReverse(bool bReverse)		{ m_IsReverse = bReverse;		}
	

private:
	InterpFunction m_InterpFunction;

	float& m_Value;
	float m_StartValue;
	float m_EndValue;
	float m_Duration;
	float m_ElapsedTime;

	bool m_IsRunning;
	bool m_IsFinished;
	bool m_IsReverse;
};