#ifndef SAPPHIRE_MYSQLBASE_H
#define SAPPHIRE_MYSQLBASE_H

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <map>

namespace Mysql
{

enum mysqlOption
{
   MYSQL_OPT_CONNECT_TIMEOUT, MYSQL_OPT_COMPRESS, MYSQL_OPT_NAMED_PIPE,
   MYSQL_INIT_COMMAND, MYSQL_READ_DEFAULT_FILE, MYSQL_READ_DEFAULT_GROUP,
   MYSQL_SET_CHARSET_DIR, MYSQL_SET_CHARSET_NAME, MYSQL_OPT_LOCAL_INFILE,
   MYSQL_OPT_PROTOCOL, MYSQL_SHARED_MEMORY_BASE_NAME, MYSQL_OPT_READ_TIMEOUT,
   MYSQL_OPT_WRITE_TIMEOUT, MYSQL_OPT_USE_RESULT,
   MYSQL_OPT_USE_REMOTE_CONNECTION, MYSQL_OPT_USE_EMBEDDED_CONNECTION,
   MYSQL_OPT_GUESS_CONNECTION, MYSQL_SET_CLIENT_IP, MYSQL_SECURE_AUTH,
   MYSQL_REPORT_DATA_TRUNCATION, MYSQL_OPT_RECONNECT,
   MYSQL_OPT_SSL_VERIFY_SERVER_CERT
};

using optionMap = std::map< mysqlOption, std::string >;

class Connection;

class MySqlBase : public boost::enable_shared_from_this< MySqlBase >
{
public:
   MySqlBase();

   ~MySqlBase();

   boost::shared_ptr< Connection > connect( const std::string& hostName, const std::string& userName,
                                            const std::string& password, uint16_t port );
   boost::shared_ptr< Connection > connect( const std::string& hostName, const std::string& userName,
                                            const std::string& password, const optionMap& map, uint16_t port );

   std::string getVersionInfo();

private:
   MySqlBase( const MySqlBase& );
   void operator=( MySqlBase& );
};

}

#endif //SAPPHIRE_MYSQLBASE_H
