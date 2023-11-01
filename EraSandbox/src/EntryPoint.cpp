#include <iostream>
#include <vector>

#include "BinaryManager/BinaryManager.h"
#include "DeserializeManager/DeserializeManager.h"
#include "SerializeManager/SerializeManager.h"
#include "Application/Application.h"

#ifdef ERA_PLATFORM_WINDOWS

bool g_ApplicationRunning = true;

namespace EraTools {

	int Main(int argc, char** argv)
	{
		while (g_ApplicationRunning)
		{
			EraTools::Application* app = EraTools::CreateApplication(argc, argv);
			app->Run();
			delete app;
		}

		return 0;
	}

}

#ifdef ERA_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return EraTools::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return EraTools::Main(argc, argv);
}

#endif // ERA_DIST

#endif // ERA_PLATFORM_WINDOWS
//int main() {
//	BinaryManager binaryManager;
//	// JSON DATA 
//	DeserializerManager deserializeManager;
//	Enum_DeserializationStatus deserializationStatus = 
//		deserializeManager.Deserialize(DeserializeSpec{
//			.m_FileType = Enum_DeserializeContentType::JSON, .m_FilePath = "resources/target.json" });
//
//
//
//	std::cout << "---JSON DATA---" << std::endl;
//	deserializeManager.DisplayScreen();
//	
//	const std::vector<CombineInfo>& combineInfos= deserializeManager.GetCombineInfos();
//
//	SerializerManager serializeManager;
//	serializeManager.Serialize(SerializeSpec{ 
//		.m_FileType = Enum_SerizalizeContentType::BINARY,
//		.m_FilePath = "output/data2.bin",
//		.m_CombineInfos = &combineInfos });
//	
//
//	// BINARY DATA 
//	DeserializerManager deserializeManager2;
//	std::cout << "---BINARY DATA---" << std::endl;
//	// Binary Deserialize içinde display screen kýsmý var orasý ekrana bastýrýyor.
//	Enum_DeserializationStatus deserializationStatus2 = deserializeManager2.Deserialize(DeserializeSpec{ .m_FileType = Enum_DeserializeContentType::BINARY, .m_FilePath = "output/data.bin" });
//	
//	deserializeManager2.DisplayScreen(); // display Screen fonksiyonunu binary ve json için iki farklý hale getir.
//	
//
//	/*
//	To Do :
//	1 - Deserializer Manager - > BinaryDeserializer (DONE)
//	2 - SetBinaryData fonksiyonunu hiyerarþik þekilde düzenlemek
//	3 - SerializerManager 
//	*/
//
//}