#include <chrono>
#include "../include/jmp.h"

int main(int argc, char* argv[]){

	bStream::CFileStream inJmp("test_files/jmp/characterinfo", bStream::Endianess::Big, bStream::OpenMode::In);
	nlohmann::json jsonOut;

	auto start = std::chrono::high_resolution_clock::now();
	Refurnisher::JMP::DecompileJMP(jsonOut, inJmp);
	auto end = std::chrono::high_resolution_clock::now();
	auto deserializeTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	bStream::CMemoryStream outJmp(1, bStream::Endianess::Big, bStream::OpenMode::Out);
	start = std::chrono::high_resolution_clock::now();
	Refurnisher::JMP::CompileJMP(jsonOut, outJmp);
	end = std::chrono::high_resolution_clock::now();
	auto serializeTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	std::printf("JMP Deserialization took %ldms\nJMP Serialization took %ldms\n", deserializeTime, serializeTime);

	return 0;
}
