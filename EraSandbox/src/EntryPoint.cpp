#include <iostream>
#include <vector>

#include "BinaryManager/BinaryManager.h"
#include "DeserializeManager/DeserializeManager.h"

int main() {
	BinaryManager binaryManager;
	DeserializerManager deserializeManager;

	Enum_DeserializationStatus deserializationStatus = 
		deserializeManager.Deserialize(DeserializeSpec{
			.m_FileType = Enum_ContentType::JSON, .m_FilePath = "resources/target.json" });

	deserializeManager.DisplayScreen();

	//binaryManager.SetBinaryData(obj);
	//binaryManager.SaveToFile();
	//binaryManager.FetchFromFile();

}