#include "pch.h"
#include <core/event_dispatcher.h>

namespace spd::core::event_dispatcher {
	void Flush() {
		if (g_eventQueue.empty()) {
			return;
		}

		// capture event count before loop to avoid executing new defered events defered by another event
		size_t eventCount = g_eventQueue.size();
		for (size_t i{}; i < eventCount; i++) {
			g_eventQueue[i]();
		}

		g_eventQueue.clear();
	}
};
