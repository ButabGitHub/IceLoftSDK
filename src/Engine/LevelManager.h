#pragma once

#include "Level/Level.h"

#include <type_traits>
#include <typeindex>

#include "Base/Object.h"

class LevelManager : public IObject {
public:

	void Enter() override;
	void Update(double Delta) override;
	void Exit() override;
	void ProcessInput() override;

	Level& GetCurrentLevel();

	template<typename T> void SwitchTo() {
		if (std::is_base_of<Level, T>)
			std::type_index LevelId = std::type_index
			_SwitchToImpl(LevelId);
	}

private:
	Level CurrentLevel;

	void _SwitchToIpml(std::type_index _Level);

};
