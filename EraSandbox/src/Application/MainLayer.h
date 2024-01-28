#include "Layer.h"
#include "imgui.h"
#include <cstdint>
#include <vector>
#include <string>
#include "CombineInfoRoot.h"
#include "ImGui/ImGuiUtils.h"
#include <thread>
//#include "ImGui/SaveOptionsRender.h"

using namespace EraTools;

class MainLayer : public EraTools::Layer
{
public:

	bool GetAutoSaveCheck() const { return  m_AutoSaveCheck; }
	bool& GetAutoSaveCheckRef() { return m_AutoSaveCheck; }

	bool& GetModifyInfoRef() { return m_IsModified; }

	virtual void OnAttach() override;

	virtual void OnUIRender() override;
	

	void Render()
	{

	}

private:
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0.0f;
	bool m_AutoSaveCheck = false;
	std::vector<CombineInfo> m_CombineCriterias;
	bool m_IsModified = false;
};