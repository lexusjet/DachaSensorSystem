
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <string>

class DatabaseInterface
{
    std::string name;
    sql::Driver * driver;


};