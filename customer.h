
//
// Created by 86189 on 25-4-10.
//

// customer.hpp
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "shop.h"
#include "order.h"

class Customer {
private:
    std::string account;
    std::string password;
    std::string name;
    bool isLoggedIn;
    ShoppingCart shoppingCart;
    ProductManager& productManager;
    static std::vector<Order> orders;  // 静态订单列表

public:
    Customer(ProductManager& pm) : isLoggedIn(false), shoppingCart(pm), productManager(pm) {}
    Customer();
    void registerCustomer(const std::string& acc, const std::string& pwd, const std::string& n);
    bool login(const std::string& inputAccount, const std::string& inputPassword);
    int changePassword(const std::string& targetAccount, const std::string& oldPassword, const std::string& newPassword);
    void queryShoppingInfo();
    void purchase(const std::string& shippingAddress, const Date& today);
    //void addToCart(const std::string& productName);
    void addToCart(const std::string& productName, int quantity = 1);
    void removeFromCart(const std::string& productName, int quantity = 1);
    void modifyQuantity(const std::string& productName, int newQuantity);
    void queryCartInfo();
    void queryOrders();
    bool modifyOrder(const std::string& orderId, const std::string& action, const std::string& newAddress = "");
    bool deleteOrder(const std::string& orderId);
    void addOrder(const Order& order);
    static void saveOrdersToFile();    // 保存订单到文件
    static void loadOrdersFromFile();  // 从文件加载订单
    static std::vector<Order>& getOrders() {
        return orders;
    }
};

#endif