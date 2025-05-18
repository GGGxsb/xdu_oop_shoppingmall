//
// Created by 86189 on 25-4-10.
//
#include "order.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <ctime>
static double total = 0;
void ShoppingCart::addToCart(const Product& product, int quantity) {
    for (auto& item : items) {
        if (item.product.name == product.name) {
            std::cout << "商品 " << product.name << " 已存在于购物车中。是否仍要加入？(y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                if (product.stock >= item.quantity + quantity) {
                    item.quantity += quantity;
                    std::cout << "商品 " << product.name << " 的购买数量已更新为 " << item.quantity << "。" << std::endl;
                } else {
                    std::cout << "商品 " << product.name << " 库存不足，无法增加购买数量。" << std::endl;
                }
            }
            return;
        }
    }
    if (product.stock >= quantity) {
        items.emplace_back(product, quantity);
        std::cout << "商品 " << product.name << " 已加入购物车，数量为 " << quantity << "。" << std::endl;
    } else {
        std::cout << "商品 " << product.name << " 库存不足，无法加入购物车。" << std::endl;
    }
}

bool ShoppingCart::removeFromCart(const std::string& productName, int quantity) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->product.name == productName) {
            if (it->quantity <= quantity) {
                items.erase(it);
                std::cout << "商品 " << productName << " 已从购物车中删除。" << std::endl;
            } else {
                it->quantity -= quantity;
                std::cout << "商品 " << productName << " 的购买数量已减少为 " << it->quantity << "。" << std::endl;
            }
            return true;
        }
    }
    std::cout << "未找到商品 " << productName << "，无法删除。" << std::endl;
    return false;
}

bool ShoppingCart::modifyQuantity(const std::string& productName, int newQuantity) {
    for (auto& item : items) {
        if (item.product.name == productName) {
            if (newQuantity == 0) {
                std::cout << "是否要删除商品 " << productName << "？(y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    removeFromCart(productName, item.quantity);
                    return true;
                }
            } else if (item.product.stock >= newQuantity) {
                item.quantity = newQuantity;
                std::cout << "商品 " << productName << " 的购买数量已更新为 " << newQuantity << "。" << std::endl;
                return true;
            } else {
                std::cout << "商品 " << productName << " 库存不足，无法修改购买数量。" << std::endl;
            }
            return false;
        }
    }
    std::cout << "未找到商品 " << productName << "，无法修改数量。" << std::endl;
    return false;
}

void ShoppingCart::queryCartInfo() {
    if (items.empty()) {
        std::cout << "购物车为空。" << std::endl;
        return;
    }
    int totalQuantity = 0;
    double totalPrice = 0;
    std::cout << "购物车中的商品信息：" << std::endl;
    for (const auto& item : items) {
        std::cout << "商品名称: " << item.product.name << " - 价格: " << item.product.price << " 元 - 数量: " << item.quantity << std::endl;
        totalQuantity += item.quantity;
        totalPrice += item.product.price * item.quantity;
    }
    std::cout << "商品总数: " << totalQuantity << "，总价格: " << totalPrice << " 元。" << std::endl;
}

// bool ShoppingCart::purchase(const std::string& shippingAddress) {
//     if (items.empty()) {
//         std::cout << "购物车为空，无法进行购买。" << std::endl;
//         return false;
//     }
//
//     std::ofstream logFile("purchase_log.txt", std::ios::app);
//     if (logFile.is_open()) {
//         std::time_t now = std::time(nullptr);
//         char* dt = std::ctime(&now);
//         logFile << "购买时间: " << dt;
//         for (const auto& item : items) {
//             if (!productManager.decreaseStock(item.product.name, item.quantity)) {
//                 std::cout << "商品 " << item.product.name << " 库存不足，购买失败。" << std::endl;
//                 return false;
//             }
//             total += item.product.price * item.quantity;
//             logFile << "商品名称: " << item.product.name << ", 价格: " << item.product.price << ", 数量: " << item.quantity << std::endl;
//         }
//         logFile << "订单总额: " << total << std::endl;
//         total = 0;
//         logFile << "收货地址: " << shippingAddress << std::endl;
//         logFile << "------------------------" << std::endl;
//         logFile.close();
//     } else {
//         std::cerr << "无法打开购买记录文件。" << std::endl;
//         return false;
//     }
//
//     // 生成订单编号
//     std::stringstream ss;
//     std::time_t now = std::time(nullptr);
//     ss << "ORD" << std::put_time(std::localtime(&now), "%Y%m%d%H%M%S");
//     std::string orderId = ss.str();
//
//     // 创建订单
//     Order order(orderId, items, shippingAddress);
//
//     std::cout << "订单生成成功，订单编号: " << orderId << std::endl;
//     std::cout << "购买时间: " << std::ctime(&order.purchaseTime);
//     std::cout << "订单总额: " << order.totalPrice << " 元" << std::endl;
//     std::cout << "收货地址: " << order.shippingAddress << std::endl;
//
//     // 清空购物车
//     items.clear();
//     return true;
// }

Order ShoppingCart::purchase(const std::string& shippingAddress) {
    if (items.empty()) {
        std::cout << "购物车为空，无法进行购买。" << std::endl;
        return Order("", std::vector<CartItem>(), ""); // 返回无效订单
    }

    // 检查库存并减少库存
    for (const auto& item : items) {
        if (!productManager.decreaseStock(item.product.name, item.quantity)) {
            std::cout << "商品 " << item.product.name << " 库存不足，购买失败。" << std::endl;
            return Order("", std::vector<CartItem>(), "");
        }
    }

    // 生成订单
    std::stringstream ss;
    std::time_t now = std::time(nullptr);
    ss << "ORD" << std::put_time(std::localtime(&now), "%Y%m%d%H%M%S");
    std::string orderId = ss.str();
    Order order(orderId, items, shippingAddress);

    // 清空购物车
    items.clear();

    return order;
}

const std::vector<CartItem>& ShoppingCart::getItems() const {
    return items; // 返回购物车商品列表
}
