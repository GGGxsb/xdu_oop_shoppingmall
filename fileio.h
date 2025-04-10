//
// Created by 86189 on 25-4-10.
//

// fileio.hpp
#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <string>

void saveCustomerToFile(const std::string& account, const std::string& password, const std::string& name);
bool loadCustomerFromFile(const std::string& account, const std::string& password, std::string& name);
bool updateCustomerPassword(const std::string& targetAccount, const std::string& newPassword);

#endif//FILEIO_H
