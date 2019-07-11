#include "../include/jfile.h"

int main(){

    bStream::CFileStream f("characterinfo", bStream::Endianess::Big, bStream::OpenMode::In);
    nlohmann::json jsonOut;

    RefurnisherCore::JMP::DecompileJMP(jsonOut, f);
    std::printf("Read JMP...\n");

    std::ofstream jsonFile;
    jsonFile.open("test.json", std::ios::openmode::_S_out);
    jsonFile << jsonOut;
    jsonFile.close();

    bStream::CFileStream f2("testinfo", bStream::Endianess::Big, bStream::OpenMode::Out);
    RefurnisherCore::JMP::CompileJMP(jsonOut, f2);
    
    

    return 0;
}