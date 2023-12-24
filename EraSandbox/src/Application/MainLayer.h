#include "Layer.h"
#include "imgui.h"
#include <cstdint>
#include <vector>
#include <string>
#include "CombineInfoRoot.h"
#include "ImGui/ImGuiUtils.h"
using namespace EraTools;

class MainLayer : public EraTools::Layer
{
public:

	virtual void OnAttach() {

	}

	virtual void OnUIRender() override;
	

	void Render()
	{

	}

private:
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0.0f;
};