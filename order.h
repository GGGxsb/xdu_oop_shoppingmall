//
// Created by 86189 on 25-5-18.
//

#ifndef ORDER_H
#define ORDER_H
#include "shop.h"
#include <string>
#include <vector>
#include <ctime>

enum OrderStatus {
    PENDING_SHIPMENT, // 待发货
    SHIPPED,          // 已发货
    RECEIVED          // 已收货
};
// 订单类
class Order {
public:
    //static std ::vector<Order> orders;  // 添加 orders 容器
    std::string orderId;
    std::vector<CartItem> items;
    std::time_t purchaseTime;
    double totalPrice = 0;
    double promotedTotalPrice = 0; // 新增：保存促销后的总价
    std::string shippingAddress;
    OrderStatus status;


    Order(const std::string& id, const std::vector<CartItem>& cartItems, const std::string& address)
    : orderId(id), shippingAddress(address), status(PENDING_SHIPMENT) {
        // 深拷贝商品数据
        for (const auto& item : cartItems) {
            items.emplace_back(
                Product(item.product.name, item.product.price, 0), // 库存不保存到订单
                item.quantity
            );
        }
        // 计算总价
        totalPrice = 0;
        for (const auto& item : items) {
            totalPrice += item.product.price * item.quantity;
        }
        purchaseTime = std::time(nullptr);
    }
    void updateStatus(OrderStatus newStatus) {
        status = newStatus;
    }
    // 应用促销活动到订单（计算但不保存结果）
    double calculatePromotedPrice(const std::vector<Promotion*>& promotions) const;

    // 应用促销活动并保存结果
    void applyPromotions(const std::vector<Promotion*>& promotions);

};


#endif //ORDER_H
