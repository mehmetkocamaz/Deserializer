#pragma once
#include <iostream>
#include <fstream>
#include "SerializeManager.h"
#include "BinaryManager/BinaryManager.h"
#include "iostream"
//#include "EraEncryptor/EraEncryptor.h"
#include "Utils/Utils.h"

Enum_SerializationStatus SerializerManager::Serialize() {
	switch (m_SerializeSpecification.m_ContentType)
	{
	case Enum_SerizalizeContentType::JSON:
		GetSerializationStatusRef().push_back(Enum_SerializationStatus::SUCCESS);
		return JsonSerialize();
	case Enum_SerizalizeContentType::BINARY:
		GetSerializationStatusRef().push_back(Enum_SerializationStatus::SUCCESS);
		return BinarySerialize();
	default:
		GetSerializationStatusRef().push_back(Enum_SerializationStatus::UNSUPPORTED);
		return Enum_SerializationStatus::UNSUPPORTED;
	}
}

Enum_SerializationStatus SerializerManager::ProcessForSave(SaveOptions saveOptions)
{
	if (m_BinaryDataAsByteArray.empty())
	{
		GetSerializationStatusRef().push_back(Enum_SerializationStatus::EMPTY_BUFFER);
		return Enum_SerializationStatus::EMPTY_BUFFER;
	}

	if (saveOptions.m_SaveFlags & Enum_Save::E_Compress)
		if (!Utils::Compress(m_BinaryDataAsByteArray, m_CompressedData))
		{
			GetSerializationStatusRef().push_back(Enum_SerializationStatus::COMPRESS_FAIL);
			return Enum_SerializationStatus::COMPRESS_FAIL;
		}
	
	if (saveOptions.m_SaveFlags & Enum_Save::E_XorFilter)
		Utils::ApplyXorFilter(m_CompressedData, saveOptions.m_XorKey);

	return Save(saveOptions.m_FilePath);
}

Enum_SerializationStatus SerializerManager::JsonSerialize() {
	GetSerializationStatusRef().push_back(Enum_SerializationStatus::SUCCESS);
	return Enum_SerializationStatus::SUCCESS;
}

Enum_SerializationStatus SerializerManager::CheckForNone() {
	for (const auto& combineInfo : *m_SerializeSpecification.m_CombineInfos) {
		for(const auto& combineCriteria : combineInfo.GetCombineCriterias()) {
			
			for (const auto& targetRequirementInfo : combineCriteria.GetTargetRequirementInfo()) {
				if (targetRequirementInfo.m_RequirementType == Enum_Requirement::None)
				{
					GetSerializationStatusRef().push_back(Enum_SerializationStatus::TYPE_NONE);
					return Enum_SerializationStatus::TYPE_NONE;
				}
			}

			for(const auto& sourceCriteria : combineCriteria.GetSourceCriterias()) {
			
				for(const auto& costInfo : sourceCriteria.GetCostInfos()) {
					if (costInfo.m_CostType == Enum_Cost::None)
					{
						GetSerializationStatusRef().push_back(Enum_SerializationStatus::TYPE_NONE);
						return Enum_SerializationStatus::TYPE_NONE;
					}
				}
				
				for (const auto& sourceRequirementInfo : sourceCriteria.GetSourceRequirementInfos()) {
					if (sourceRequirementInfo.m_RequirementType == Enum_Requirement::None)
					{
						GetSerializationStatusRef().push_back(Enum_SerializationStatus::TYPE_NONE);
						return Enum_SerializationStatus::TYPE_NONE;
					}
				}
			}
		}
	}
	GetSerializationStatusRef().push_back(Enum_SerializationStatus::SUCCESS);
	return Enum_SerializationStatus::SUCCESS;
}

Enum_SerializationStatus SerializerManager::BinarySerialize() {

	m_BinaryData.push_back(m_SerializeSpecification.m_CombineInfos->size());
	for (const auto& combineInfo : *m_SerializeSpecification.m_CombineInfos) {
		m_BinaryData.push_back(combineInfo.GetTargetItemId());
		m_BinaryData.push_back(combineInfo.GetCombineCriterias().size());
		for (const auto& combineCriterias : combineInfo.GetCombineCriterias()) {
			m_BinaryData.push_back(combineCriterias.GetTargetRequirementInfo().size());
			for (const auto& targetRequirementInfo : combineCriterias.GetTargetRequirementInfo()) {
				m_BinaryData.push_back(static_cast<int32_t>(targetRequirementInfo.m_RequirementType));
				m_BinaryData.push_back(targetRequirementInfo.m_RequirementValue);
			}
			m_BinaryData.push_back(combineCriterias.GetSourceCriterias().size());
			for (const auto& sourceCriterias : combineCriterias.GetSourceCriterias()) {
				m_BinaryData.push_back(sourceCriterias.GetSourceItemId());
				m_BinaryData.push_back(sourceCriterias.GetCostInfos().size());
				for (const auto& sourceCostInfo : sourceCriterias.GetCostInfos()) {
					m_BinaryData.push_back(static_cast<int32_t>(sourceCostInfo.m_CostType));
					m_BinaryData.push_back(sourceCostInfo.m_CostValue);
				}
				m_BinaryData.push_back(sourceCriterias.GetProbabilityInfos().size());
				for (const auto& probabilityInfo : sourceCriterias.GetProbabilityInfos())
				{
					m_BinaryData.push_back(static_cast<int32_t>(probabilityInfo.m_ProbabilityType));
					m_BinaryData.push_back(probabilityInfo.m_ProbabilityValue);
				}
				m_BinaryData.push_back(sourceCriterias.GetSourceRequirementInfos().size());
				for (const auto& sourceRequirementInfo : sourceCriterias.GetSourceRequirementInfos()) {
					m_BinaryData.push_back(static_cast<int32_t>(sourceRequirementInfo.m_RequirementType));
					m_BinaryData.push_back(sourceRequirementInfo.m_RequirementValue);
				}
			}
		}
	}


	for (const auto& bitset : m_BinaryData) {
		for (int i = 0; i < 32; i += 8) {
			uint8_t byte = 0;
			for (int j = 0; j < 8; ++j) {
				byte |= (bitset[i + j] << j);
			}
			m_BinaryDataAsByteArray.push_back(byte);
		}
	}
	GetSerializationStatusRef().push_back(Enum_SerializationStatus::SUCCESS);
	return Enum_SerializationStatus::SUCCESS;

}


//Enum_SerializationStatus SerializerManager::EncryptionTest()
//{
//	CryptoPP::AutoSeededRandomPool rng;
//	CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
//	rng.GenerateBlock(key, key.size());
//	std::cout << key;
//	CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
//	rng.GenerateBlock(iv, iv.size());
//	std::cout << iv;
//
//	// Create an instance of AESEncryptor with the generated key and IV
//	EraEncryptor aesEncryptor(key, iv);
//
//
//
//	// Encrypt the data
//	std::vector<uint8_t> ciphertext = aesEncryptor.encrypt(m_CompressedData);
//
//	// Decrypt the data
//	std::vector<uint8_t> decryptedData = aesEncryptor.decrypt(ciphertext);
//
//	// Print the original, encrypted, and decrypted data
//	std::cout << "Original data: ";
//	for (uint8_t byte : m_CompressedData) {
//		std::cout << static_cast<int>(byte) << ' ';
//	}
//	std::cout << std::endl;
//
//	std::cout << "Encrypted data: ";
//	for (uint8_t byte : ciphertext) {
//		std::cout << static_cast<int>(byte) << ' ';
//	}
//	std::cout << std::endl;
//
//	std::cout << "Decrypted data: ";
//	for (uint8_t byte : decryptedData) {
//		std::cout << static_cast<int>(byte) << ' ';
//	}
//	std::cout << std::endl;
//
//	return Enum_SerializationStatus::SUCCESS;
//}



Enum_SerializationStatus SerializerManager::Save(std::filesystem::path filePath)
{
	std::ofstream binaryFile(filePath, std::ios::binary);

	if (!binaryFile.is_open())
	{
		std::cerr << "file cannot opened!!" << std::endl;
		GetSerializationStatusRef().push_back(Enum_SerializationStatus::OPEN_FILE_ERROR);
		return Enum_SerializationStatus::OPEN_FILE_ERROR;
	}
	try
	{
		uint32_t uncompressedSize = m_BinaryDataAsByteArray.size();
		binaryFile.write(reinterpret_cast<const char*>(&uncompressedSize), sizeof(uncompressedSize));
		binaryFile.write(reinterpret_cast<const char*>(m_CompressedData.data()), m_CompressedData.size());
		binaryFile.write(reinterpret_cast<const char*>(m_BinaryDataAsByteArray.data()), m_BinaryDataAsByteArray.size());

		binaryFile.close();
	}
	catch (const std::exception&)
	{
		GetSerializationStatusRef().push_back(Enum_SerializationStatus::FAIL);
		return Enum_SerializationStatus::FAIL;
	}
	GetSerializationStatusRef().push_back(Enum_SerializationStatus::SUCCESS);
	return Enum_SerializationStatus::SUCCESS;
}