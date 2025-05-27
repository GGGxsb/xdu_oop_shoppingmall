//
// Created by 86189 on 25-5-25.
//
#include "promotion.h"
#include "order.h"
#include "products.h"

// 应用折扣到订单
double DiscountPromotion::apply(double currentTotal, const Order& order, const Date& today) const {
    if (!isActive(today)) {
        return currentTotal;
    }
    double productOriginalTotal = 0.0;
    // 计算目标商品的原始总金额
    for (const auto& item : order.items) {
        if (item.product.name == productName) {
            productOriginalTotal += item.product.price * item.quantity;
        }
    }
    // 折扣后的总金额 = 原始金额 * 折扣率
    double discountedProductTotal = productOriginalTotal * discount;
    // 当前总价 = 当前总价 - (原始金额 - 折扣后金额)
    return currentTotal - (productOriginalTotal - discountedProductTotal);
}

double FullReductionPromotion::apply(double currentTotal, const Order& order, const Date& today) const {
    if (!isActive(today)) {
        return currentTotal;
    }
    if (currentTotal >= fullAmount) {
        return currentTotal - reductionAmount;  // 满减基于当前总价
    }
    return currentTotal;
}