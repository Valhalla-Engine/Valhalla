#include "vhpch.h"
#include "PanelManager.h"

namespace Valhalla
{
	PanelManager::~PanelManager()
	{
		for (auto& map : m_Panels)
		{
			map.clear();
		}
	}

	PanelData* PanelManager::GetPanelData(uint32_t panelID)
	{
		for (auto& panelMap : m_Panels)
		{
			if (panelMap.find(panelID) == panelMap.end())
				continue;

			return &panelMap.at(panelID);
		}

		return nullptr;
	}

	const PanelData* PanelManager::GetPanelData(uint32_t panelID) const
	{
		for (auto& panelMap : m_Panels)
		{
			if (panelMap.find(panelID) == panelMap.end())
				continue;

			return &panelMap.at(panelID);
		}

		return nullptr;
	}

	void PanelManager::RemovePanel(const char* strID)
	{
		uint32_t id = Hash::GenerateFNVHash(strID);
		for (auto& map : m_Panels)
		{
			if (map.find(id) == map.end())
				continue;

			map.erase(id);
			return;
		}

		VH_CORE_ERROR_TAG("PanelManager", "Couldn't find panel with id '{0}'", id);
	}

	void PanelManager::OnImGuiRender()
	{
		for (auto& map : m_Panels)
		{
			for (auto& [id, panelData] : map)
			{
				bool closedThisFrame = false;
				if (panelData.isOpen)
				{
					panelData.Panel->OnImGuiRender(panelData.isOpen);
					closedThisFrame = !panelData.isOpen;
				}

				if (closedThisFrame)
				{
					// Do serialize
				}
			}
		}
	}

	void PanelManager::OnEvent(Event& e)
	{
		for (auto& map : m_Panels)
		{
			for (auto& [id, panelData] : map)
				panelData.Panel->OnEvent(e);
		}
	}
}