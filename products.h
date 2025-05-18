//
// Created by 86189 on 25-4-10.
//
#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>

// 商品类
class Product {
public:
    std::string name;
    double price;
    int stock;

    Product(const std::string& n, double p, int s) : name(n), price(p), stock(s) {}
};

// 商品管理类
class ProductManager {
private:
    std::vector<Product> products;
    void saveToFile();
    void loadFromFile();
public:
    ProductManager();
    void addProduct(const std::string& name, double price, int stock);
    bool deleteProduct(const std::string& targetName);
    const std::vector<Product>& getProducts() const;
    bool decreaseStock(const std::string& productName, int quantity);
    std::vector<Product> queryProductByName(const std::string& targetName);
    std::vector<Product> queryProductByFuzzyName(const std::string& keyword);
    bool increaseStock(const std::string& productName, int quantity);
};

#endif