//
// Created by 86189 on 25-4-10.
//

#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include <vector>
#include "products.h"

 class Order;

// ���ﳵ�е���Ʒ����
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
public:
    ShoppingCart(ProductManager& pm) : productManager(pm) {}
    void addToCart(const Product& product, int quantity = 1);
    bool removeFromCart(const std::string& productName, int quantity = 1);
    bool modifyQuantity(const std::string& productName, int newQuantity);
    void queryCartInfo();
    // bool purchase(const std::string& shippingAddress);
    Order purchase(const std::string& shippingAddress);
    const std::vector<CartItem>& getItems() const;
};

#endif