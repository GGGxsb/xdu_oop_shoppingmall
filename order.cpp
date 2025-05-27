//
// Created by 86189 on 25-5-25.
//
#include "order.h"

double Order::calculatePromotedPrice(const std::vector<Promotion*>& promotions,const Date& today) const {
    double currentTotal = totalPrice;  // 初始为原始总价
    for (auto promotion : promotions) {
        currentTotal = promotion->apply(currentTotal, *this,today);  // 依次应用促销
    }
    return currentTotal;
}

void Order::applyPromotions(const std::vector<Promotion*>& promotions,const Date& today) {
    promotedTotalPrice = calculatePromotedPrice(promotions,today);
}