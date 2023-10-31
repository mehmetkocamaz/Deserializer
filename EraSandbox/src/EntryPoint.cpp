#include <iostream>
#include <vector>

#include "BinaryManager/BinaryManager.h"
#include "DeserializeManager/DeserializeManager.h"
#include "SerializeManager/SerializeManager.h"

int main() {
	BinaryManager binaryManager;
	
	DeserializerManager deserializeManager;
	Enum_DeserializationStatus deserializationStatus = 
		deserializeManager.Deserialize(DeserializeSpec{
			.m_FileType = Enum_DeserializeContentType::JSON, .m_FilePath = "resources/example.json" });



	std::cout << "---JSON DATA---" << std::endl;
	deserializeManager.DisplayScreen();
	
	const std::vector<CombineInfo>& combineInfos= deserializeManager.GetCombineInfos();

	SerializerManager serializeManager;
	serializeManager.Serialize(SerializeSpec{ 
		.m_FileType = Enum_SerizalizeContentType::BINARY,
		.m_FilePath = "output/example.bin",
		.m_CombineInfos = &combineInfos });

	

	std::cout << "---BINARY DATA---" << std::endl;
	DeserializerManager deserializeManager2;
	Enum_DeserializationStatus deserializationStatus2 =
		deserializeManager2.Deserialize(DeserializeSpec{
		.m_FileType = Enum_DeserializeContentType::BINARY, .m_FilePath = "output/example.bin" });

	deserializeManager2.DisplayScreen();
	
	// BINARY DATA 
	//DeserializerManager deserializeManager2;
	//std::cout << "---BINARY DATA---" << std::endl;
	//// Binary Deserialize içinde display screen kýsmý var orasý ekrana bastýrýyor.
	//Enum_DeserializationStatus deserializationStatus2 = deserializeManager2.Deserialize(DeserializeSpec{ .m_FileType = Enum_ContentType::BINARY, .m_FilePath = "output/data.bin" });
	
	//deserializeManager2.DisplayScreen(); // display Screen fonksiyonunu binary ve json için iki farklý hale getir.
	

}