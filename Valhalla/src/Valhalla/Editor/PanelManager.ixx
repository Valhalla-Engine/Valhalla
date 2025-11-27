export module Valhalla.Editor.PanelManager;

import Valhalla.Core.Ref;
import Valhalla.Core.Log;
import Valhalla.Core.Hash;
import Valhalla.Core.Events;

import Valhalla.Editor.EditorPanel;

import <array>;
import <unordered_map>;
import <utility>;

export namespace Valhalla
{
	struct PanelData
	{
		const char* ID = "";
		const char* Name = "";
		Ref<EditorPanel> Panel = nullptr;
		bool isOpen = false;
	};

	enum class PanelCategory
	{
		Edit, View, _COUNT
	};

	class PanelManager
	{
	public:
		PanelManager() = default;
		~PanelManager();

		PanelData* GetPanelData(uint32_t PanelID);
		const PanelData* GetPanelData(uint32_t PanelID) const;

		void RemovePanel(const char* strID);

		void OnImGuiRender();

		void OnEvent(Event& e);

		std::unordered_map<uint32_t, PanelData>& GetPanels(PanelCategory category) { return m_Panels[(size_t)category]; }
		const std::unordered_map<uint32_t, PanelData>& GetPanels(PanelCategory category) const { return m_Panels[(size_t)category]; }

	public:
		template<typename TPanel>
		Ref<TPanel> AddPanel(PanelCategory category, const PanelData& data)
		{
			static_assert(std::is_base_of<EditorPanel, TPanel>::value, "PanelManager::AddPanel requires TPanel to inherit from EditorPanel");
			
			auto& panelMap = m_Panels[(size_t)category];
			uint32_t id = Hash::GenerateFNVHash(data.ID);
			if (panelMap.find(id) != panelMap.end())
			{
				Log::CoreErrorTag("PanelManager", "A panel with the id '{0}' has already been added", data.ID);
				return nullptr;
			}

			panelMap[id] = data;
			return data.Panel.As<TPanel>();
		}

		template<typename TPanel, typename... TArgs>
		Ref<TPanel> AddPanel(PanelCategory category, const char* strID, bool isOpenByDefault, TArgs&&... args)
		{
			return AddPanel<TPanel>(category, PanelData{ strID, strID, Ref<TPanel>::Create(std::forward<TArgs>(args)...), isOpenByDefault });
		}	

		template<typename TPanel, typename... TArgs>
		Ref<TPanel> AddPanel(PanelCategory category, const char* strID, const char* displayName, bool isOpenByDefault, TArgs&&... args)
		{
			return AddPanel<TPanel>(category, PanelData{ strID, displayName, Ref<TPanel>::Create(std::forward<TArgs>(args)...), isOpenByDefault });
		}

		template<typename TPanel>
		Ref<TPanel> GetPanel(const char* strID)
		{
			static_assert(std::is_base_of<EditorPanel, TPanel>::value, "PanelManager::AddPanel requires TPanel to inherit from EditorPanel");

			uint32_t id = Hash::GenerateFNVHash(strID);

			for (const auto& panelMap : m_Panels)
			{
				if (panelMap.find(id) == panelMap.end())
					continue;

				return panelMap.at(id).Panel.As<TPanel>();
			}

			Log::CoreErrorTag("PanelManager", "Couldn't find panel with id '{0}'", strID);
			return nullptr;
		}

	private:
		std::array<std::unordered_map<uint32_t, PanelData>, (size_t)PanelCategory::_COUNT> m_Panels;
	};
}