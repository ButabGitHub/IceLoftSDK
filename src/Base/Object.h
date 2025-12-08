#pragma once

class IObject {
public:
	virtual void Enter() = 0;
	virtual void Update(double Delta) = 0;
	virtual void Exit() = 0;
	virtual void ProcessInput() = 0;
	
protected:
	virtual void OnEnter();
	virtual void OnUpdate(double Delta);
	virtual void OnExit();
	virtual void OnInput();
};
