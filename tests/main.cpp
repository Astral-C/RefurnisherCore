#include "../include/jmp.h"

int main(int argc, char* argv[]){

    bStream::CFileStream f("test_files/characterinfo", bStream::Endianess::Big, bStream::OpenMode::In);
    nlohmann::json jsonOut;

    RefurnisherCore::JMP::DecompileJMP(jsonOut, f);

    std::ofstream jsonFile;
    jsonFile.open("test.json", std::ios::openmode::_S_out);
    jsonFile << jsonOut;
    jsonFile.close();

    return 0;
}
