#pragma once
#include <functional>
#include <ui/Widget.h>

namespace spd::ui {
	class Slider : public Widget {
	public:
		static constexpr const char* DEFAULT_TAG = "unnamed slider";

		Slider(float minValue = 0.0f, float maxValue = 100.0f, float step = 1.f, const char* tag = DEFAULT_TAG);

		// callback setter
		Slider* OnChange(std::function<void(float)> callback);

		// getters and setters
		float GetValue() const { return m_value; }
		Slider* SetValue(float value) { m_value = value; return this; }

		float GetMin() const { return m_minValue; }
		Slider* SetMin(float minValue) { m_minValue = minValue; return this; }

		float GetMax() const { return m_maxValue; }
		Slider* SetMax(float maxValue) { m_maxValue = maxValue; return this; }

	protected:
		ImVec2 OnCalcSize() override;
		void OnRender() override;

	private:
		float m_value;
		float m_minValue;
		float m_maxValue;
		char m_stepFmt[8]{ 0 }; // step in %.f string format
		std::function<void(float)> m_onChangeCallback = nullptr;
	};

	template <typename... Args>
	std::unique_ptr<Slider> MakeSlider(Args&&... args) {
		return std::make_unique<Slider>(std::forward<Args>(args)...);
	}
}
