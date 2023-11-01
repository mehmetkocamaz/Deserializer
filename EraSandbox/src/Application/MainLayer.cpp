#include "MainLayer.h"

void MainLayer::OnUIRender(){
	ImGui::Begin("Editor");
	std::vector<CombineInfo>& combineInfos = CombineInfoRoot::Instance().m_CombineInfos;
	// Iterate over the tree nodes and create a tree node for each element
	for (int i = 0; i < combineInfos.size(); i++)
	{
		// Create a tree node with a dynamic label
		if (ImGui::TreeNode((std::string("Combine Info ") + std::to_string(i)).c_str()))
		{
			// This code will be executed when the tree node is expanded.
			// Add your tree node content here.
			ImGui::SetNextItemWidth(100);
			ImGui::InputScalar("Target Item Id", ImGuiDataType_U32, &combineInfos[i].GetTargetItemIdRef(), NULL, NULL, "%u");
			std::vector<CombineCriteria>& combineCriterias = combineInfos[i].GetCombineCriteriasRef();
			for (int i = 0; i < combineCriterias.size(); i++)
			{
				if (ImGui::TreeNode((std::string("Combine Criteria ") + std::to_string(i)).c_str()))
				{
					if (ImGui::TreeNode((std::string("Target Requirement Infos ").c_str())))
					{
						std::vector<RequirementInfo>& targetRequirementInfos = combineCriterias[i].GetTargetRequirementInfoRef();
						for (int i = 0; i < targetRequirementInfos.size(); i++)
						{
							if (ImGui::TreeNode((std::string("Target Requirement Info ") + std::to_string(i)).c_str()))
							{
								const char* requirement_names[] = { "Enchantment", "Combine", "Refine", "Socket" };

								int current_item = static_cast<int>(targetRequirementInfos[i].m_RequirementType);
								ImGui::SetNextItemWidth(150);
								if (ImGui::Combo("Requirement Type", &current_item, requirement_names, IM_ARRAYSIZE(requirement_names)))
								{
									// TODO: Handle case multiple select same instance 
									targetRequirementInfos[i].m_RequirementType = static_cast<Enum_Requirement>(current_item);
								}

								ImGui::SetNextItemWidth(150);
								ImGui::InputScalar("Cost", ImGuiDataType_U32, &targetRequirementInfos[i].m_RequirementValue, NULL, NULL, "%u");


								ImGuiUtils::ColoredButton(std::string("Delete Requirement Info ") + std::to_string(i), ImVec2(0, 0), ImGuiUtils::imDarkRed, [&]() {
									targetRequirementInfos.erase(targetRequirementInfos.begin() + i);
									});

								ImGui::TreePop();
							}
						}

						if (targetRequirementInfos.size() < 4) {
							ImGuiUtils::ColoredButton(std::string("Add Target Requirement Info ") + std::to_string(targetRequirementInfos.size()), ImVec2(0, 0), ImGuiUtils::imDarkGreen, [&]() {
								RequirementInfo requirementInfo;
							targetRequirementInfos.push_back(requirementInfo);
								});
						}
						ImGui::TreePop();
					}

					if (ImGui::TreeNode((std::string("Source Criterias").c_str())))
					{
						std::vector<SourceCriteria>& sourceCriterias = combineCriterias[i].GetSourceCriteriasRef();
						for (int i = 0; i < sourceCriterias.size(); i++)
						{
							if (ImGui::TreeNode((std::string("Source Criterias ") + std::to_string(i)).c_str()))
							{
								ImGui::SetNextItemWidth(100);
								ImGui::InputScalar("Source Item Id", ImGuiDataType_U32, &sourceCriterias[i].GetSourceItemIdRef(), NULL, NULL, "%u");

								if (ImGui::TreeNode((std::string("Cost Infos ").c_str())))
								{
									std::vector<CostInfo>& costInfos = sourceCriterias[i].GetCostInfosRef();
									for (int i = 0; i < costInfos.size(); i++)
									{
										if (ImGui::TreeNode((std::string("Cost Info ") + std::to_string(i)).c_str()))
										{
											const char* cost_names[] = { "Silver" ,"Billion","ContributionPoint","BloodPoint" };

											int current_item = static_cast<int>(costInfos[i].m_CostType);
											ImGui::SetNextItemWidth(180);
											if (ImGui::Combo("Requirement Type", &current_item, cost_names, IM_ARRAYSIZE(cost_names)))
											{
												// TODO: Handle case multiple select same instance 
												costInfos[i].m_CostType = static_cast<Enum_Cost>(current_item);
											}
											ImGui::SetNextItemWidth(100);
											ImGui::InputScalar("Cost", ImGuiDataType_U32, &costInfos[i].m_CostValue, NULL, NULL, "%u");

											ImGuiUtils::ColoredButton(std::string("Delete Cost Info ") + std::to_string(i), ImVec2(0, 0), ImGuiUtils::imDarkRed, [&]() {
												costInfos.erase(costInfos.begin() + i);
												});
											ImGui::TreePop();
										}

									}
									ImGui::TreePop();
									if (costInfos.size() < 4)
									{
										ImGuiUtils::ColoredButton(std::string("Add Cost Info") + std::to_string(costInfos.size()), ImVec2(0, 0), ImGuiUtils::imDarkGreen, [&]() {
											CostInfo costInfo;
										costInfos.push_back(costInfo);
											});
									}
								}
								ImGui::TreePop();

								

							}
						}

						ImGuiUtils::ColoredButton(std::string("Add Source Criterias") + std::to_string(sourceCriterias.size()), ImVec2(0, 0), ImGuiUtils::imDarkGreen, [&]() {
							SourceCriteria sourceCriteria;
						sourceCriterias.push_back(sourceCriteria);
							});

						ImGui::TreePop();

					}


					ImGuiUtils::ColoredButton(std::string("Delete Combine Criteria ") + std::to_string(i), ImVec2(0, 0), ImGuiUtils::imDarkRed, [&]() {
						combineCriterias.erase(combineCriterias.begin() + i);
						});

					ImGui::TreePop();
				}


			}

			if (combineCriterias.size() < 12) {
				ImGuiUtils::ColoredButton(std::string("Add Combine Criteria ") + std::to_string(combineCriterias.size()), ImVec2(0, 0), ImGuiUtils::imDarkGreen, [&]() {
					CombineCriteria combineCriteria;
				combineCriterias.push_back(combineCriteria);
					});
			}

			ImGuiUtils::ColoredButton(std::string("Delete Combine Info ") + std::to_string(i), ImVec2(0, 0), ImGuiUtils::imDarkRed, [&]() {
				combineInfos.erase(combineInfos.begin() + i);

				});
			// End the tree node
			ImGui::TreePop();
		}
	}


	ImGuiUtils::ColoredButton(std::string("Add Combine Info ") + std::to_string(combineInfos.size()), ImVec2(0, 0), ImGuiUtils::imDarkGreen, [&]() {
		CombineInfo combineInfo;
	combineInfos.push_back(combineInfo);
		});


	ImGui::End();
	ImGui::ShowDemoWindow();

	Render();
	}