//
// Created by 86189 on 25-5-25.
//
// promotion.h
#ifndef PROMOTION_H
#define PROMOTION_H

#include <string>
#include <ctime>
#include <iostream>

// �Զ������ڽṹ�壬ֻ����������
struct Date {
    int year;
    int month;
    int day;

    // ���ڱȽϺ���
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
    // �������������
    friend std::istream& operator>>(std::istream& is, Date& date) {
        std::cout << "���������: ";
        is >> date.year;
        std::cout << "�������·�: ";
        is >> date.month;
        std::cout << "����������: ";
        is >> date.day;

        // ����֤������Ч��
        if (date.month < 1 || date.month > 12 || date.day < 1 || date.day > 31) {
            std::cerr << "����: ��������ڿ�����Ч������!" << std::endl;
        }

        return is;
    }

    // �������������������ӡ����
    friend std::ostream& operator<<(std::ostream& os, const Date& date) {
        os << date.year << "-" << date.month << "-" << date.day;
        return os;
    }
};

// ������ǰ������
class Order;
// ��Ʒ��ǰ������
class Product;

// ������������
class Promotion {
protected:
    std::string productName;
    Date startTime;
    Date endTime;

public:
    Promotion(const std::string& productName, const Date& startTime, const Date& endTime)
        : productName(productName), startTime(startTime), endTime(endTime) {}

    virtual ~Promotion() = default;

    // �жϴ�����Ƿ�����Ч����
    bool isActive(const Date& today) const {
        return today >= startTime && today <= endTime;
    }

    // Ӧ�ô����������
    virtual double apply(double currentTotal, const Order& order, const Date& today) const = 0;

    // ��ȡ��Ʒ����
    std::string getProductName() const {
        return productName;
    }
};

// ��ʱ�ۿ۴������
class DiscountPromotion : public Promotion {
private:
    double discount;

public:
    DiscountPromotion(const std::string& productName, double discount, const Date& startTime, const Date& endTime)
        : Promotion(productName, startTime, endTime), discount(discount) {}

    // Ӧ���ۿ۵�����
    double apply(double currentTotal, const Order& order, const Date& today) const override;

    // ��ȡ�ۿ���
    double getDiscount() const {
        return discount;
    }
};

// �����������
class FullReductionPromotion : public Promotion {
private:
    double fullAmount;
    double reductionAmount;

public:
    FullReductionPromotion(const std::string& productName, double fullAmount, double reductionAmount, const Date& startTime, const Date& endTime)
        : Promotion(productName, startTime, endTime), fullAmount(fullAmount), reductionAmount(reductionAmount) {}

    // Ӧ������������
    double apply(double currentTotal, const Order& order, const Date& today) const override;

    // ��ȡ�����ż����
    double getFullAmount() const {
        return fullAmount;
    }

    // ��ȡ�������
    double getReductionAmount() const {
        return reductionAmount;
    }
};

#endif // PROMOTION_H