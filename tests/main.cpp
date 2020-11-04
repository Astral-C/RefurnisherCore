#include <chrono>
#include <iostream>
#include <filesystem>
#include "../include/jmp.h"

int main(int argc, char* argv[]){

	int64_t total_deserialization_time = 0;
	int64_t total_serialization_time = 0;
	int64_t total_time = 0;

	for(auto entry : std::filesystem::directory_iterator("./tests/test_files/jmp")){
		bStream::CFileStream inJmp(entry.path(), bStream::Endianess::Big, bStream::OpenMode::In);
		nlohmann::json jsonOut;

		auto start = std::chrono::high_resolution_clock::now();
		Refurnisher::JMP::DecompileJMP(jsonOut, inJmp);
		auto end = std::chrono::high_resolution_clock::now();
		auto deserializeTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

		bStream::CMemoryStream outJmp(1, bStream::Endianess::Big, bStream::OpenMode::Out);
		start = std::chrono::high_resolution_clock::now();
		Refurnisher::JMP::CompileJMP(jsonOut, outJmp);
		end = std::chrono::high_resolution_clock::now();
		auto serializeTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

		total_deserialization_time += deserializeTime;
		total_serialization_time += serializeTime;
		total_time += (deserializeTime + serializeTime);

		std::printf("%s Deserialization took %ldms\nJMP Serialization took %ldms\n", entry.path().filename().c_str(), deserializeTime, serializeTime);
	}

	std::printf("===Test Complete===\nTime measured in Milliseconds\nTotal Deserialization Time: %ldms\nTotal Serialization Time: %ldms\nTotal Time: %ldms\n", total_deserialization_time, total_serialization_time, total_time);

	return 0;
}
