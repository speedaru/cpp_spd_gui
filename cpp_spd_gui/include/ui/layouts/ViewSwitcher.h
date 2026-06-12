#pragma once
#include <unordered_map>
#include <string>
#include <ui/Container.h>

namespace spd::ui {

    class ViewSwitcher : public Container {
    public:
        static constexpr const char* DEFAULT_TAG = "unnamed view switcher";

        ViewSwitcher(const char* tag = DEFAULT_TAG) { m_tag = tag; }

        // calculate active view size + self size, calculate active view flex
        void Update() override;
        
        void Arrange(ImVec2 finalPosition) override;

        // register a named view into the layout
        template <typename T>
        T* AddView(const std::string& name, std::unique_ptr<T> view) {
			T* addedView = Add(std::move(view));
			m_views[name] = addedView;

			// switch to first view automatically
			if (!m_activeView) {
				m_activeView = { .name = name, .view = addedView, };
			}

			return addedView;
        }

        // swap to a registered view
        void SwitchTo(const std::string& name);

        // return the active view's name
        std::string GetActiveViewName() const { return m_activeView.name; }

    protected:
        ImVec2 OnCalcSize() override;
        void OnRender() override;

        void CalculateFlex();

    private:
        struct NamedView {
            std::string name{ "" };
            Widget* view{};

            operator bool() const { return view; }
            bool operator!() const { return !view; }
            Widget* operator->() { return view; }
        };

        std::unordered_map<std::string, Widget*> m_views;
        NamedView m_activeView;
    };

    template <typename... Args>
    std::unique_ptr<ViewSwitcher> MakeViewSwitcher(Args&&... args) {
        return std::make_unique<ViewSwitcher>(std::forward<Args>(args)...);
    }
}
