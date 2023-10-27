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
    binaryData.SaveToFile();
    binaryData.FetchFromFile();

}