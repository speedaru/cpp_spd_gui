#pragma once
#include <functional>

namespace spd::core::event_dispatcher {
	inline std::vector<std::function<void()>> g_eventQueue;

    // registers a callback to execute after the rendering stage
    template <typename F, typename... Args>
    void Defer(F&& func, Args&&... args) {
        g_eventQueue.emplace_back(
            [f = std::forward<F>(func),
            ...args = std::forward<Args>(args)]() mutable
            {
                std::invoke(f, args...);
            }
        );
    }

    // flushes and executes all callbacks
    void Flush();
}
