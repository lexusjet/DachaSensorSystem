#include <SensorMessage/SensorMessage.h>

SensorMessage::SensorMessage()
{
    header.version = 0;
    header.location = 0;
    header.numberInLocation = 0;
    header.dataType = 0;
    header.data = 0;
    header.reserv = 0;
    header.extension = 0;
}

SensorMessage::~SensorMessage()
{

}

SensorMessage::SensorMessage(const SensorMessage& a)
{
    header.version = a.header.version;
    header.location = a.header.location ;
    header.numberInLocation = a.header.numberInLocation ;
    header.dataType = a.header.dataType ;
    header.data = a.header.data ;
    header.extension = a.header.extension ;
    header.reserv = a.header.reserv ;
}

void SensorMessage::operator=(const SensorMessage& a)
{
    header.version = a.header.version;
    header.location = a.header.location;
    header.numberInLocation = a.header.numberInLocation;
    header.dataType = a.header.dataType;
    header.data = a.header.data;
    header.extension = a.header.extension;
    header.reserv = a.header.reserv;
}
bool SensorMessage::operator==(const SensorMessage& other) const
{
    return header.version == other.header.version
            && header.location == other.header.location
            && header.numberInLocation == other.header.numberInLocation
            && header.dataType == other.header.dataType
            && header.data == other.header.data
            && header.extension == other.header.extension
            && header.reserv == other.header.reserv;

}

bool SensorMessage::operator!=(const SensorMessage& other) const
{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& stream, SensorMessage messege){
    stream << "VERSION : " << static_cast<int>(messege.getVersion()) <<std::endl;
    stream << "LOCAL : " << static_cast<int>(messege.getLocation()) <<std::endl;
    stream << "NUMBER IN LOCAL : " << static_cast<int>(messege.getNumberInLocation()) <<std::endl;
    stream << "DATA TYPE : " << static_cast<int>(messege.getDataType()) <<std::endl;
    stream << "DATA : " << static_cast<int>(messege.getData()) <<std::endl;
    stream << "EXTENSION : " << static_cast<int>(messege.getExtension()) <<std::endl;
    stream << "RESERVE : " << static_cast<int>(messege.getReserve());
    return stream;
}
