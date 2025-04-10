//
// Created by 86189 on 25-4-10.
//

// administrator.hpp
#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <string>

class Administrator {
private:
    std::string account;
    std::string password;
public:
    Administrator(const std::string& acc, const std::string& pwd);
    bool login(const std::string& inputAccount, const std::string& inputPassword);
    bool deleteCustomer(const std::string& targetAccount);
    int resetCustomerPassword(const std::string& targetAccount, const std::string& oldPassword, const std::string& newPassword);
};

#endif
