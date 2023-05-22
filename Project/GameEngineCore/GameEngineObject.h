#pragma once


class GameEngineObject
{
	friend class GameEngineLevel;
	friend class GameEngineCore;

public:
	GameEngineObject();
	~GameEngineObject();

	GameEngineObject(const GameEngineObject& _Other) = delete;
	GameEngineObject(GameEngineObject&& _Other) noexcept = delete;
	GameEngineObject& operator=(const GameEngineObject& _Other) = delete;
	GameEngineObject& operator=(GameEngineObject&& _Other) noexcept = delete;

	virtual void Start() {}

	virtual void Update(float _Delta) {}

	virtual void Render() {}

	virtual void Release() {}

	void On()
	{
		IsUpdateValue = true;
	}

	void Off()
	{
		IsUpdateValue = false;
	}

	void Death()
	{
		IsDeathValue = true;
	}

	bool IsUpdate()
	{
		return true == IsUpdateValue && false == IsDeathValue;
	}

	virtual bool IsDeath() 
	{
		return IsDeathValue;
	}

	void SetOrder(int _Order)
	{
		Order = _Order;
	}

	float GetLiveTime()
	{
		return LiveTime;
	}

	void ResetLiveTime(float _DeltaTime)
	{
		LiveTime = 0.0f;
	}

protected:

private:
	float LiveTime = 0.0f;
	int Order = 0;
	bool IsUpdateValue = true;
	bool IsDeathValue = false;

	void AddLiveTime(float _DeltaTime)
	{
		LiveTime += _DeltaTime;
	}
};

