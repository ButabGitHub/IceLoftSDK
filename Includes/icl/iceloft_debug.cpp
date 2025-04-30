#include "icl_debug.h"
#include <ImGUI/imconfig.h>
#include <glfw3.h>

namespace icl {
    static bool showDebugWindow = false;

	void IclDebug::imguiShowDebug() {
        showDebugWindow = true;
	}
	
	void IclDebug::imguiHideDebug() {
        showDebugWindow = false;
	}

	/*void drawDebugWindow() {
		if (showDebugWindow) {
            
		}
	}*/
}
