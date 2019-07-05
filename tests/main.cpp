#include "../include/jfile.h"

int main(){

    bStream::CFileStream f("characterinfo", bStream::Endianess::Big, bStream::OpenMode::In);

    nlohmann::json jsonOut = RefurnisherCore::JMP::DecompileJMP(f);
    std::string jsonString;
    jsonOut.get_to(jsonString);

    std::ofstream jsonFile;
    jsonFile.open("test.json", std::ios::openmode::_S_out);
    jsonFile << jsonString;
    jsonFile.close();

    return 0;
}