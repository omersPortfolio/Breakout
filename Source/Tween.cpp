#include "GamePCH.h"

#include "Tween.h"

Tween::Tween(InterpFunction function, float& value, float startValue, float endValue, float duration, float startTime)
	: m_Value(value)
{
	m_StartValue = startValue;
	m_Value = startValue;
	m_EndValue = endValue;
	m_Duration = duration;
	m_InterpFunction = function;
	m_ElapsedTime = startTime;

	m_IsRunning = false;
	m_IsFinished = false;
}

void Tween::Update(float deltaTime)
{
	if (m_IsRunning)
	{
		m_ElapsedTime += deltaTime;

		if (m_ElapsedTime >= 0)
		{
			float percentage = m_ElapsedTime / m_Duration;
			if (percentage >= 1)
			{
				m_IsRunning = false;
				m_IsFinished = true;
				percentage = 1;
			}
			m_Value = m_StartValue + (m_EndValue - m_StartValue) * m_InterpFunction(percentage);
		}
	}
}

void Tween::Start()
{
	m_IsRunning = true;
	m_IsFinished = false;
	m_ElapsedTime = 0.0f;
	m_Value = m_StartValue;
}

void Tween::Reset()
{
	m_Value = m_StartValue;
	m_IsRunning = false;
	m_IsFinished = false;
	m_ElapsedTime = 0.0f;
}

void Tween::Restart()
{
	Reset();
	Start();
}

void Tween::Reverse()
{
	m_ElapsedTime = 0.0f;
	m_EndValue = m_StartValue;
	Start();
	
}

void Tween::StartAndReverse()
{
	Start();
	if (m_IsFinished)
		Reverse();
}

bool Tween::IsRunning()
{
	return m_IsRunning;
}

bool Tween::IsFinished()
{
	return m_IsFinished;
}

bool Tween::IsReverse()
{
	return m_IsReverse;
}

float Tween::GetRemainingTime()
{
	return m_Duration - m_ElapsedTime;
}
