//
// Created by 86189 on 25-4-10.
//

// customer.hpp
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer {
private:
    std::string account;
    std::string password;
    std::string name;
    bool isLoggedIn;
public:
    Customer();
    void registerCustomer(const std::string& acc, const std::string& pwd, const std::string& n);
    bool login(const std::string& inputAccount, const std::string& inputPassword);
    int changePassword(const std::string& targetAccount, const std::string& oldPassword, const std::string& newPassword);
    void queryShoppingInfo();
    void purchase();
};

#endif
