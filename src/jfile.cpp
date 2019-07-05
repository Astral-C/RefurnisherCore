#include "jfile.h"
#include <iostream>

namespace RefurnisherCore::JMP {

    nlohmann::json DecompileJMP(bStream::CStream& fileData){

        int32_t m_entryCount = fileData.readInt32();
        int32_t m_fieldCount = fileData.readInt32();
        uint32_t m_entryOffset = fileData.readUInt32();
        int32_t m_entrySize = fileData.readInt32();

        std::vector<jField> m_fields;

        nlohmann::json jsonData;
        nlohmann::json jsonFields;
        nlohmann::json jsonValues;


        for(int i = 0; i < m_fieldCount; i++){
            m_fields.push_back(jField(fileData));
            jsonFields.push_back(m_fields.back());
        }

        for(int x = 0; x < m_entryCount; x++){
            for(int y = 0; y < m_fieldCount; y++){
                fileData.seek(m_entryOffset + (m_entrySize*x) + m_fields.at(y).start);
                jsonValues.push_back((jValue(fileData, m_fields.at(y))));
            }
        }

        jsonData["entryCount"] = m_entryCount;
        jsonData["fieldCount"] = m_fieldCount;
        jsonData["entryOffset"] = m_entryOffset;
        jsonData["entrySize"] = m_entrySize;
        jsonData["fields"] = jsonFields;
        jsonData["values"] = jsonValues;

        return jsonData;
    }

    /*
    bStream::CMemoryStream CompileJMP(nlohmann::json& jsonData){
        //Write JMP Header
        stream.writeInt32(m_entryCount);
        stream.writeInt32(m_fieldCount);
        stream.writeUInt32(m_entryOffset);
        stream.writeInt32(m_entrySize);

        //Write field LUT
        for(int f = 0; f < m_fieldCount; f++){
            stream.writeUInt32(m_fields.at(f).hash);
            stream.writeUInt32(m_fields.at(f).bitmask);
            stream.writeUInt16(m_fields.at(f).start);
            stream.writeUInt8(m_fields.at(f).shift);
            stream.writeUInt8((uint8_t)m_fields.at(f).type);
        }

        //Write Entries
        //Bitmask buffer needed for generating bitmasks for fields that require them. Clears on every entry.
        std::map<uint16_t, uint32_t> bitmaskBuffer;
        for(int x = 0; x < m_entryCount; x++){
            bitmaskBuffer.clear();
            for(int y = 0; y < m_fieldCount; y++){
                //TODO: Fix this up
                jValue curVal = getField(x, y);
                jField curField = getFieldDesc(y);
                stream.seek(m_entryOffset + (m_entrySize * x) + curField.start);

                switch (curField.type) {
                case jType::JINT32:
                    if(curField.bitmask == 0xFFFFFFFFu){
                        stream.writeInt32(curVal.vInt);
                    } else {
                        //If the bitmask buffer doesn't contain the current start, add it
                        if(bitmaskBuffer.count(curField.start) == 0){
                            bitmaskBuffer[curField.start] = 0u;
                        }
                        bitmaskBuffer.find(curField.start)->second |= ((uint32_t)curVal.vInt << curField.shift) & curField.bitmask;
                    }
                    break;
                case jType::JFLOAT:
                    stream.writeFloat(curVal.vFlt);
                    break;
                case jType::JSTRING:
                    stream.writeString(std::string(curVal.vStr));
                    break;
                }
            }
            std::map<uint16_t, uint32_t>::iterator it;
            for(it = bitmaskBuffer.begin(); it != bitmaskBuffer.end(); it++){
                stream.seek(m_entryOffset + (m_entrySize * x) + it->first);
                stream.writeUInt32(it->second);
            }
        }
    }*/

    void to_json(nlohmann::json& j, const jValue& v){
        j = nlohmann::json{
            {"int", v.vInt},
            {"str", v.vStr},
            {"float", v.vFlt}
        };
    }

    void from_json(const nlohmann::json& j, jValue& v){
        j.at("int").get_to(v.vInt);
        j.at("str").get_to(v.vStr); 
        j.at("float").get_to(v.vFlt);
    }

    void to_json(nlohmann::json& j, const jField& v){
        j = nlohmann::json{
            {"name", ""},
            {"hash", v.hash},
            {"bitmask", v.bitmask},
            {"start", v.start},
            {"shift", v.shift},
            {"type", (int)v.type}
        };
    }

    void from_json(const nlohmann::json& j, jField& v){
        j.at("hash").get_to(v.hash);
        j.at("bitmask").get_to(v.bitmask); 
        j.at("start").get_to(v.start);
        j.at("shift").get_to(v.shift);
        j.at("type").get_to(v.type);
    }

}