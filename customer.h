//
// Created by 86189 on 25-4-10.
//

//
// Created by 86189 on 25-4-10.
//

// customer.hpp
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "shop.h"

class Customer {
private:
    std::string account;
    std::string password;
    std::string name;
    bool isLoggedIn;
    ShoppingCart shoppingCart;
    ProductManager& productManager;
public:
    Customer(ProductManager& pm) : isLoggedIn(false), shoppingCart(pm), productManager(pm) {}
    Customer();
    void registerCustomer(const std::string& acc, const std::string& pwd, const std::string& n);
    bool login(const std::string& inputAccount, const std::string& inputPassword);
    int changePassword(const std::string& targetAccount, const std::string& oldPassword, const std::string& newPassword);
    void queryShoppingInfo();
    void purchase();
    //void addToCart(const std::string& productName);
    void addToCart(const std::string& productName, int quantity = 1);
    void removeFromCart(const std::string& productName, int quantity = 1);
    void modifyQuantity(const std::string& productName, int newQuantity);
    void queryCartInfo();
};

#endif