//
// Created by 86189 on 25-4-10.
//
#include "shop.h"
#include <iostream>
#include <fstream>
#include <ctime>

void ShoppingCart::addToCart(const Product& product) {
    if (product.stock > 0) {
        items.push_back(product);
        std::cout << "商品 " << product.name << " 已加入购物车。" << std::endl;
    } else {
        std::cout << "商品 " << product.name << " 库存不足，无法加入购物车。" << std::endl;
    }
}

bool ShoppingCart::purchase() {
    if (items.empty()) {
        std::cout << "购物车为空，无法购买。" << std::endl;
        return false;
    }
    double total = 0;
    std::ofstream logFile("purchase_log.txt", std::ios::app);
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        char* dt = std::ctime(&now);
        logFile << "购买时间: " << dt;
        for (const auto& item : items) {
            if (!productManager.decreaseStock(item.name, 1)) {
                std::cout << "商品 " << item.name << " 库存不足，购买失败。" << std::endl;
                return false;
            }
            total += item.price;
            logFile << "商品名称: " << item.name << ", 价格: " << item.price << ", 数量: 1" << std::endl;
        }
        logFile << "总金额: " << total << std::endl;
        logFile << "------------------------" << std::endl;
        logFile.close();
    } else {
        std::cerr << "无法打开购买记录文件。" << std::endl;
        return false;
    }
    std::cout << "购买成功！总金额: " << total << " 元。" << std::endl;
    items.clear();
    return true;
}