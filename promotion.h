//
// Created by 86189 on 25-5-25.
//
// promotion.h
#ifndef PROMOTION_H
#define PROMOTION_H

#include <string>
#include <ctime>
#include <iostream>

// 自定义日期结构体，只包含年月日
struct Date {
    int year;
    int month;
    int day;

    // 日期比较函数
    bool operator<=(const Date& other) const {
        if (year < other.year) return true;
        if (year > other.year) return false;
        if (month < other.month) return true;
        if (month > other.month) return false;
        return day <= other.day;
    }

    bool operator>=(const Date& other) const {
        return other <= *this;
    }
    // 重载输入运算符
    friend std::istream& operator>>(std::istream& is, Date& date) {
        std::cout << "请输入年份: ";
        is >> date.year;
        std::cout << "请输入月份: ";
        is >> date.month;
        std::cout << "请输入日期: ";
        is >> date.day;

        // 简单验证日期有效性
        if (date.month < 1 || date.month > 12 || date.day < 1 || date.day > 31) {
            std::cerr << "警告: 输入的日期可能无效，请检查!" << std::endl;
        }

        return is;
    }

    // 重载输出运算符，方便打印日期
    friend std::ostream& operator<<(std::ostream& os, const Date& date) {
        os << date.year << "-" << date.month << "-" << date.day;
        return os;
    }
};

// 订单类前置声明
class Order;
// 商品类前置声明
class Product;

// 促销活动抽象基类
class Promotion {
protected:
    std::string productName;
    Date startTime;
    Date endTime;

public:
    Promotion(const std::string& productName, const Date& startTime, const Date& endTime)
        : productName(productName), startTime(startTime), endTime(endTime) {}

    virtual ~Promotion() = default;

    // 判断促销活动是否在有效期内
    bool isActive(const Date& today) const {
        return today >= startTime && today <= endTime;
    }

    // 应用促销活动到订单
    virtual double apply(double currentTotal, const Order& order, const Date& today) const = 0;

    // 获取商品名称
    std::string getProductName() const {
        return productName;
    }
};

// 限时折扣促销活动类
class DiscountPromotion : public Promotion {
private:
    double discount;

public:
    DiscountPromotion(const std::string& productName, double discount, const Date& startTime, const Date& endTime)
        : Promotion(productName, startTime, endTime), discount(discount) {}

    // 应用折扣到订单
    double apply(double currentTotal, const Order& order, const Date& today) const override;

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
    FullReductionPromotion(const std::string& productName, double fullAmount, double reductionAmount, const Date& startTime, const Date& endTime)
        : Promotion(productName, startTime, endTime), fullAmount(fullAmount), reductionAmount(reductionAmount) {}

    // 应用满减到订单
    double apply(double currentTotal, const Order& order, const Date& today) const override;

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