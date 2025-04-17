#include "products.h"
#include <fstream>
#include <sstream>
#include <iostream>

ProductManager::ProductManager() {
    loadFromFile();
}

void ProductManager::addProduct(const std::string& name, double price, int stock) {
    products.emplace_back(name, price, stock);
    saveToFile();
}

const std::vector<Product>& ProductManager::getProducts() const {
    return products;
}

bool ProductManager::decreaseStock(const std::string& productName, int quantity) {
    for (auto& product : products) {
        if (product.name == productName) {
            if (product.stock >= quantity) {
                product.stock -= quantity;
                saveToFile();
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

void ProductManager::saveToFile() {
    std::ofstream outfile("products.txt");
    if (outfile.is_open()) {
        for (const auto& product : products) {
            outfile << product.name << "," << product.price << "," << product.stock << std::endl;
        }
        outfile.close();
    } else {
        std::cout << "无法打开文件保存商品信息" << std::endl;
    }
}

void ProductManager::loadFromFile() {
    std::ifstream infile("products.txt");
    if (infile.is_open()) {
        std::string line;
        while (std::getline(infile, line)) {
            std::stringstream ss(line);
            std::string name;
            double price;
            int stock;
            std::getline(ss, name, ',');
            ss >> price;
            ss.ignore();
            ss >> stock;
            products.emplace_back(name, price, stock);
        }
        infile.close();
    }
}
std::vector<Product> ProductManager::queryProductByName(const std::string& targetName)  {
    std::vector<Product> result;
    for (const auto& product : products) {
        if (product.name == targetName) {
            result.push_back(product);
        }
    }
    return result;
}
std::vector<Product> ProductManager::queryProductByFuzzyName(const std::string& keyword) {
    std::vector<Product> result;
    for (const auto& product : products) {
        if (product.name.find(keyword) != std::string::npos) {
            result.push_back(product);
        }
    }
    return result;
}
bool ProductManager::deleteProduct(const std::string& targetName) {
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->name == targetName) {
            products.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}