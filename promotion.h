//
// Created by 86189 on 25-5-25.
//
// promotion.h
#ifndef PROMOTION_H
#define PROMOTION_H

#include <string>
#include <ctime>

// 订单类前置声明
class Order;
// 商品类前置声明
class Product;

// 促销活动抽象基类
class Promotion {
protected:
    // std::time_t startTime;
    // std::time_t endTime;
    std::string productName;

public:
    // Promotion(const std::string& productName, std::time_t startTime, std::time_t endTime)
    //     : productName(productName), startTime(startTime), endTime(endTime) {}
    Promotion(const std::string& productName)
       : productName(productName) {}

    virtual ~Promotion() = default;

    // 判断促销活动是否在有效期内
    // bool isActive() const {
    //     std::time_t now = std::time(nullptr);
    //     return now >= startTime && now <= endTime;
    // }

    // 应用促销活动到订单
    virtual double apply(double currentTotal, const Order& order) const = 0;

    // 获取商品名称
    std::string getProductName() const {
        return productName;
    }

    // // 获取开始时间
    // std::time_t getStartTime() const {
    //     return startTime;
    // }
    //
    // // 获取结束时间
    // std::time_t getEndTime() const {
    //     return endTime;
    // }
};

// 限时折扣促销活动类
class DiscountPromotion : public Promotion {
private:
    double discount;

public:
    // 修改构造函数
    DiscountPromotion(const std::string& productName, double discount)
        : Promotion(productName), discount(discount) {}

    // 应用折扣到订单
    double apply(double currentTotal, const Order& order) const override;

    // 获取折扣率
    double getDiscount() const {
        return discount;
    }
};

// 满减促销活动类
class FullReductionPromotion : public Promotion {
private:
    double fullAmount;
    double reductionAmount;

public:
    // FullReductionPromotion(const std::string& productName, std::time_t startTime, std::time_t endTime, double fullAmount, double reductionAmount)
    //     : Promotion(productName, startTime, endTime), fullAmount(fullAmount), reductionAmount(reductionAmount) {}
    FullReductionPromotion(const std::string& productName, double fullAmount, double reductionAmount)
        : Promotion(productName), fullAmount(fullAmount), reductionAmount(reductionAmount) {}

    // 应用满减到订单
    double apply(double currentTotal, const Order& order) const override;

    // 获取满减门槛金额
    double getFullAmount() const {
        return fullAmount;
    }

    // 获取满减金额
    double getReductionAmount() const {
        return reductionAmount;
    }
};

#endif // PROMOTION_H