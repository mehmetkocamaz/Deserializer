#include <iostream>
#include <vector>

#include "BinaryManager/BinaryManager.h"
#include "DeserializeManager/DeserializeManager.h"

int main() {
	BinaryManager binaryManager;
	// JSON DATA 
	DeserializerManager deserializeManager;
	Enum_DeserializationStatus deserializationStatus = 
		deserializeManager.Deserialize(DeserializeSpec{
			.m_FileType = Enum_ContentType::JSON, .m_FilePath = "resources/target.json" });

	std::cout << "---JSON DATA---" << std::endl;
	deserializeManager.DisplayScreen();
	
	// BINARY DATA 
	DeserializerManager deserializeManager2;
	std::cout << "---BINARY DATA---" << std::endl;
	// Binary Deserialize içinde display screen kýsmý var orasý ekrana bastýrýyor.
	Enum_DeserializationStatus deserializationStatus2 = deserializeManager2.Deserialize(DeserializeSpec{ .m_FileType = Enum_ContentType::BINARY, .m_FilePath = "output/data.bin" });
	
	deserializeManager2.DisplayScreen(); // display Screen fonksiyonunu binary ve json için iki farklý hale getir.
	

	/*
	To Do :
	1 - Deserializer Manager - > BinaryDeserializer (DONE)
	2 - SetBinaryData fonksiyonunu hiyerarþik þekilde düzenlemek
	3 - SerializerManager 
	*/

}