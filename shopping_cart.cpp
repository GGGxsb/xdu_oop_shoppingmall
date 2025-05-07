//
// Created by 86189 on 25-4-10.
//
#include "shop.h"
#include <iostream>
#include <fstream>
#include <ctime>

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
            if (!productManager.decreaseStock(item.product.name, item.quantity)) {
                std::cout << "商品 " << item.product.name << " 库存不足，购买失败。" << std::endl;
                return false;
            }
            total += item.product.price * item.quantity;
            logFile << "商品名称: " << item.product.name << ", 价格: " << item.product.price << ", 数量: " << item.quantity << std::endl;
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