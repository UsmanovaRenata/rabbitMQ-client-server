
#ifndef RABBITMQ_SERVER_CONFIGPARSER_H
#define RABBITMQ_SERVER_CONFIGPARSER_H
#include <string>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>

class ConfigParser {
public:
    ConfigParser() : filename("config.ini") {}
    std::string get(const char* section, const char*  key) const{
        try {
            boost::property_tree::ptree pt;
            boost::property_tree::ini_parser::read_ini(filename, pt);
            std::string combined = std::string(section) + '.' + key;
            std::string value = pt.get<std::string>(combined);
            return value;
        } catch (const boost::property_tree::ini_parser_error &e) {
            std::cerr << "Error reading config.ini: " << e.what() << std::endl;
            return "";
        }
    }
private:
    std::string filename;
};


#endif //RABBITMQ_SERVER_CONFIGPARSER_H
