#pragma once
#include <map>
#include <cstring>
#include <algorithm>
#include "bstream.h"
#include "json.hpp"

namespace Refurnisher::JMP {

enum jType {
	JINT32 = 0,
	JSTRING = 1,
	JFLOAT = 2
};


//A jField is a field description used for reading and writing them later on
struct jField {
	uint32_t hash;
	uint32_t bitmask;
	uint16_t start;
	uint8_t shift;
	jType type;

	jField(bStream::CStream& stream){
		hash = stream.readUInt32();
		bitmask = stream.readUInt32();
		start = stream.readUInt16();
		shift = stream.readUInt8();
		type = (jType)stream.readUInt8();
	}
	jField(){}
};

// A jValue represents a value that corresponds to a field as defined in the field LUT
struct jValue {
	int32_t vInt;
	float vFlt;
	char vStr[32];

	jValue(bStream::CStream& stream, jField& f){
		switch (f.type) {
			case JINT32:
				vInt = (stream.readInt32() & f.bitmask) >> f.shift;
			break;
			case JSTRING:
				strncpy(vStr, stream.readString(32).c_str(), 32);
			break;
			case JFLOAT:
				vFlt = stream.readFloat();
			break;
			default:
			break;
		}
	}
	jValue(){}
};

void DecompileJMP(nlohmann::json& jsonData, bStream::CStream& fileData);
void CompileJMP(nlohmann::json& jsonData, bStream::CStream& fileData);

void to_json(nlohmann::json& json, const jValue& v);
void from_json(const nlohmann::json& json, jValue& v);

void to_json(nlohmann::json& json, const jField& v);
void from_json(const nlohmann::json& json, jField& v);

}
