#include <MessageHeader/MessageHeader.h>

MessageHeader::MessageHeader()
{
    header.version = 0;
    header.local = 0;
    header.numberInLocalization = 0;
    header.dataType = 0;
    header.data = 0;
    header.reserv = 0;
    header.extension = 0;
}

MessageHeader::~MessageHeader()
{
    //dtor
}

MessageHeader::MessageHeader(const MessageHeader& a)
{
    header.version = a.header.version;
    header.local = a.header.local ;
    header.numberInLocalization = a.header.numberInLocalization ;
    header.dataType = a.header.dataType ;
    header.data = a.header.data ;
    header.extension = a.header.extension ;
    header.reserv = a.header.reserv ;
}

void MessageHeader::operator=(const MessageHeader& a)
{
    header.version = a.header.version;
    header.local = a.header.local;
    header.numberInLocalization = a.header.numberInLocalization;
    header.dataType = a.header.dataType;
    header.data = a.header.data;
    header.extension = a.header.extension;
    header.reserv = a.header.reserv;
}

std::ostream& operator<<(std::ostream& stream, MessageHeader messege){
    stream << "VERSION : " << static_cast<int>(messege.getVersion()) <<std::endl;
    stream << "LOCAL : " << static_cast<int>(messege.getLocal()) <<std::endl;
    stream << "NUMBER IN LOCAL : " << static_cast<int>(messege.getNumberInLocalization()) <<std::endl;
    stream << "DATA TYPE : " << static_cast<int>(messege.getDataType()) <<std::endl;
    stream << "DATA : " << static_cast<int>(messege.getData()) <<std::endl;
    stream << "EXTENSION : " << static_cast<int>(messege.getExtension()) <<std::endl;
    stream << "RESERVE : " << static_cast<int>(messege.getReserve());
    return stream;
}
