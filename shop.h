//
// Created by 86189 on 25-4-10.
//

#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include <vector>
#include "products.h"

 class Order;

// 购物车中的商品项类
class CartItem {
public:
    Product product;
    int quantity;

    CartItem(const Product& p, int q) : product(p), quantity(q) {}
};

class ShoppingCart {
private:
    std::vector<CartItem> items;
    ProductManager& productManager;
    void loadCartFromFile();  // 从文件中加载购物车信息
    void saveCartToFile();    // 将购物车信息保存到文件中
public:
    ShoppingCart(ProductManager& pm) : productManager(pm) {
        loadCartFromFile();  // 构造函数中加载购物车信息
    }
    void addToCart(const Product& product, int quantity = 1);
    bool removeFromCart(const std::string& productName, int quantity = 1);
    bool modifyQuantity(const std::string& productName, int newQuantity);
    void queryCartInfo();
    // bool purchase(const std::string& shippingAddress);
    Order purchase(const std::string& shippingAddress);
    const std::vector<CartItem>& getItems() const;

};

#endif