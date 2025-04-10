//
// Created by 86189 on 25-4-10.
//

#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include <vector>
#include "products.h"

class ShoppingCart {
private:
    std::vector<Product> items;
    ProductManager& productManager;
public:
    ShoppingCart(ProductManager& pm) : productManager(pm) {}
    void addToCart(const Product& product);
    bool purchase();
};

#endif