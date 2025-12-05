#include "LevelManager.h"

void LevelManager::Update(double Delta) {
	GetCurrentLevel().Update(Delta);
}

Level& LevelManager::GetCurrentLevel() {
	Level& LvRef = CurrentLevel;

	return LvRef;
}

void LevelManager::_SwitchToIpml(std::type_index _Level) {
	
}
