#include "jmp.h"
#include <iostream>

namespace Refurnisher::JMP {

    void DecompileJMP(nlohmann::json& jsonData, bStream::CStream& fileData){

        int32_t m_entryCount = fileData.readInt32();
        int32_t m_fieldCount = fileData.readInt32();
        uint32_t m_entryOffset = fileData.readUInt32();
        int32_t m_entrySize = fileData.readInt32();

        std::vector<jField> m_fields;

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
    }

    void CompileJMP(nlohmann::json& jsonData, bStream::CStream& out){

        std::vector<jValue> values;
        std::vector<jField> fields;

        int32_t m_entryCount = jsonData.at("entryCount");
        int32_t m_fieldCount = jsonData.at("fieldCount");
        uint32_t m_entryOffset = jsonData.at("entryOffset");
        int32_t m_entrySize = jsonData.at("entrySize");

        //Write JMP Header
        out.writeInt32(m_entryCount);
        out.writeInt32(m_fieldCount);
        out.writeUInt32(m_entryOffset);
        out.writeInt32(m_entrySize);
        std::cout << jsonData.at("fields").is_array() << std::endl;
        std::cout << jsonData.at("fields")[0].at("hash") << std::endl;
        //Write field LUT and populate fields array
        for(auto& field : jsonData.at("fields")){
            out.writeUInt32(field.at("hash"));
            out.writeUInt32(field.at("bitmask"));
            out.writeUInt32(field.at("start"));
            out.writeUInt32(field.at("shift"));
            out.writeUInt32(field.at("type"));
            fields.push_back(static_cast<jField>(field));
        }

        for(auto& value : jsonData.at("values")){
            if(!value.is_string()){
                values.push_back(static_cast<jValue>(value));
            }
        }

        //Write Entries
        //Bitmask buffer needed for generating bitmasks for fields that require them. Clears on every entry.
        std::map<uint16_t, uint32_t> bitmaskBuffer;
        for(int x = 0; x < m_entryCount; x++){
            bitmaskBuffer.clear();
            for(int y = 0; y < m_fieldCount; y++){
                //TODO: Fix this up
                jValue curVal = values.at(x * y + m_fieldCount);
                jField curField = fields.at(y);
                out.seek(m_entryOffset + (m_entrySize * x) + curField.start);

                switch (curField.type) {
                case jType::JINT32:
                    if(curField.bitmask == 0xFFFFFFFFu){
                        out.writeInt32(curVal.vInt);
                    } else {
                        //If the bitmask buffer doesn't contain the current start, add it
                        if(bitmaskBuffer.count(curField.start) == 0){
                            bitmaskBuffer[curField.start] = 0u;
                        }
                        bitmaskBuffer.find(curField.start)->second |= ((uint32_t)curVal.vInt << curField.shift) & curField.bitmask;
                    }
                    break;
                case jType::JFLOAT:
                    out.writeFloat(curVal.vFlt);
                    break;
                case jType::JSTRING:
                    out.writeString(std::string(curVal.vStr));
                    break;
                }
            }
            std::map<uint16_t, uint32_t>::iterator it;
            for(it = bitmaskBuffer.begin(); it != bitmaskBuffer.end(); it++){
                out.seek(m_entryOffset + (m_entrySize * x) + it->first);
                out.writeUInt32(it->second);
            }
        }
    }

    void to_json(nlohmann::json& j, const jValue& v){
        j = nlohmann::json{
            {"jint", v.vInt},
            {"jstr", v.vStr},
            {"jfloat", v.vFlt}
        };
    }

    void from_json(const nlohmann::json& j, jValue& v){
        j.at("jint").get_to(v.vInt);
        j.at("jfloat").get_to(v.vFlt);
        //This is gross.
        strncpy(v.vStr, j.at("jstr").get<std::string>().c_str(), 32);
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
