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
	// Binary Deserialize i�inde display screen k�sm� var oras� ekrana bast�r�yor.
	Enum_DeserializationStatus deserializationStatus2 = deserializeManager2.Deserialize(DeserializeSpec{ .m_FileType = Enum_ContentType::BINARY, .m_FilePath = "output/data.bin" });
	
	deserializeManager2.DisplayScreen(); // display Screen fonksiyonunu binary ve json i�in iki farkl� hale getir.
	

	/*
	To Do :
	1 - Deserializer Manager - > BinaryDeserializer (DONE)
	2 - SetBinaryData fonksiyonunu hiyerar�ik �ekilde d�zenlemek
	3 - SerializerManager 
	*/

}