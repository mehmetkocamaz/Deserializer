#include <iostream>
#include <fstream>
#include <vector>
#include "BinaryManager.h"

int main() {
    BinaryManager binaryData;
    DeserializerManager obj;
    obj.Deserialize();
    obj.DisplayScreen();

    binaryData.SetBinaryData(obj);
    std::cout << "\n\nBinary Data : " << std::endl;
    for (const auto& item : binaryData.GetBinaryData()) {
        std::cout << item << std::endl;
    }
}