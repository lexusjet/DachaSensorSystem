#pragma once

#include <string.h>
#include <cstdint>
#include <cstddef>
#include <string>
#include <iostream>

// Format of header
//      size: 16 bytes
//
//      1) 1 byte: version
//      2) 1 byte: localisation
//      3) 1 byte: number in localisation
//      4) 1 byte: data type
//      5) 4 byte: data
//      6) 4 byte: extension
//      7) 4 byte: reserv

struct Header{
    uint8_t version;
    uint8_t local;
    uint8_t numberInLocalization;
    uint8_t dataType;
    int32_t data;
    uint32_t extension;
    uint32_t reserv;
};

const size_t MESSEGE_SIZE = 16;
// =============================================================================
class MessageHeader
{
    public:
        MessageHeader();
        ~MessageHeader();
        MessageHeader(const MessageHeader&);
        void operator=(const MessageHeader&);

        void setMessageHeader(char *head[MESSEGE_SIZE]){ memcpy(str, head, sizeof(str));};
        void setMessageHeader(const char* head){ memcpy(str, head, sizeof(str));};


        size_t messegeSize() const {return sizeof(str);};
        void setVersion(uint8_t version){ header.version = version;};
        uint8_t getVersion() const {return header.version;};

        void setLocal(uint8_t local){ header.local = local;};
        uint8_t getLocal()const {return header.local;};

        void setNumberInLocalization(uint8_t a) { header.numberInLocalization = a;};
        uint8_t getNumberInLocalization()const {return header.numberInLocalization;};

        void setDataType(uint8_t type) {header.dataType = type;};
        uint8_t getDataType() const { return header.dataType;};

        void setData(int32_t data) {header.data = data;};
        int32_t getData() const { return header.data;};

        void setExtension(uint32_t extension) {header.extension = extension;};
        uint32_t getExtension() const {return header.extension;};

        void setReserv(uint32_t reserv) {header.reserv = reserv;};
        uint32_t getReserve() const {return header.reserv;};

    protected:
        union {
            char str[MESSEGE_SIZE];
            Header header;
        };
};

std::ostream& operator<<(std::ostream& stream, MessageHeader messege);
