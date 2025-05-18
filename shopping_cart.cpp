//
// Created by 86189 on 25-4-10.
//
#include "order.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <ctime>
static double total = 0;
void ShoppingCart::addToCart(const Product& product, int quantity) {
    for (auto& item : items) {
        if (item.product.name == product.name) {
            std::cout << "��Ʒ " << product.name << " �Ѵ����ڹ��ﳵ�С��Ƿ���Ҫ���룿(y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                if (product.stock >= item.quantity + quantity) {
                    item.quantity += quantity;
                    std::cout << "��Ʒ " << product.name << " �Ĺ��������Ѹ���Ϊ " << item.quantity << "��" << std::endl;
                } else {
                    std::cout << "��Ʒ " << product.name << " ��治�㣬�޷����ӹ���������" << std::endl;
                }
            }
            return;
        }
    }
    if (product.stock >= quantity) {
        items.emplace_back(product, quantity);
        std::cout << "��Ʒ " << product.name << " �Ѽ��빺�ﳵ������Ϊ " << quantity << "��" << std::endl;
    } else {
        std::cout << "��Ʒ " << product.name << " ��治�㣬�޷����빺�ﳵ��" << std::endl;
    }
}

bool ShoppingCart::removeFromCart(const std::string& productName, int quantity) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->product.name == productName) {
            if (it->quantity <= quantity) {
                items.erase(it);
                std::cout << "��Ʒ " << productName << " �Ѵӹ��ﳵ��ɾ����" << std::endl;
            } else {
                it->quantity -= quantity;
                std::cout << "��Ʒ " << productName << " �Ĺ��������Ѽ���Ϊ " << it->quantity << "��" << std::endl;
            }
            return true;
        }
    }
    std::cout << "δ�ҵ���Ʒ " << productName << "���޷�ɾ����" << std::endl;
    return false;
}

bool ShoppingCart::modifyQuantity(const std::string& productName, int newQuantity) {
    for (auto& item : items) {
        if (item.product.name == productName) {
            if (newQuantity == 0) {
                std::cout << "�Ƿ�Ҫɾ����Ʒ " << productName << "��(y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    removeFromCart(productName, item.quantity);
                    return true;
                }
            } else if (item.product.stock >= newQuantity) {
                item.quantity = newQuantity;
                std::cout << "��Ʒ " << productName << " �Ĺ��������Ѹ���Ϊ " << newQuantity << "��" << std::endl;
                return true;
            } else {
                std::cout << "��Ʒ " << productName << " ��治�㣬�޷��޸Ĺ���������" << std::endl;
            }
            return false;
        }
    }
    std::cout << "δ�ҵ���Ʒ " << productName << "���޷��޸�������" << std::endl;
    return false;
}

void ShoppingCart::queryCartInfo() {
    if (items.empty()) {
        std::cout << "���ﳵΪ�ա�" << std::endl;
        return;
    }
    int totalQuantity = 0;
    double totalPrice = 0;
    std::cout << "���ﳵ�е���Ʒ��Ϣ��" << std::endl;
    for (const auto& item : items) {
        std::cout << "��Ʒ����: " << item.product.name << " - �۸�: " << item.product.price << " Ԫ - ����: " << item.quantity << std::endl;
        totalQuantity += item.quantity;
        totalPrice += item.product.price * item.quantity;
    }
    std::cout << "��Ʒ����: " << totalQuantity << "���ܼ۸�: " << totalPrice << " Ԫ��" << std::endl;
}

// bool ShoppingCart::purchase(const std::string& shippingAddress) {
//     if (items.empty()) {
//         std::cout << "���ﳵΪ�գ��޷����й���" << std::endl;
//         return false;
//     }
//
//     std::ofstream logFile("purchase_log.txt", std::ios::app);
//     if (logFile.is_open()) {
//         std::time_t now = std::time(nullptr);
//         char* dt = std::ctime(&now);
//         logFile << "����ʱ��: " << dt;
//         for (const auto& item : items) {
//             if (!productManager.decreaseStock(item.product.name, item.quantity)) {
//                 std::cout << "��Ʒ " << item.product.name << " ��治�㣬����ʧ�ܡ�" << std::endl;
//                 return false;
//             }
//             total += item.product.price * item.quantity;
//             logFile << "��Ʒ����: " << item.product.name << ", �۸�: " << item.product.price << ", ����: " << item.quantity << std::endl;
//         }
//         logFile << "�����ܶ�: " << total << std::endl;
//         total = 0;
//         logFile << "�ջ���ַ: " << shippingAddress << std::endl;
//         logFile << "------------------------" << std::endl;
//         logFile.close();
//     } else {
//         std::cerr << "�޷��򿪹����¼�ļ���" << std::endl;
//         return false;
//     }
//
//     // ���ɶ������
//     std::stringstream ss;
//     std::time_t now = std::time(nullptr);
//     ss << "ORD" << std::put_time(std::localtime(&now), "%Y%m%d%H%M%S");
//     std::string orderId = ss.str();
//
//     // ��������
//     Order order(orderId, items, shippingAddress);
//
//     std::cout << "�������ɳɹ����������: " << orderId << std::endl;
//     std::cout << "����ʱ��: " << std::ctime(&order.purchaseTime);
//     std::cout << "�����ܶ�: " << order.totalPrice << " Ԫ" << std::endl;
//     std::cout << "�ջ���ַ: " << order.shippingAddress << std::endl;
//
//     // ��չ��ﳵ
//     items.clear();
//     return true;
// }

Order ShoppingCart::purchase(const std::string& shippingAddress) {
    if (items.empty()) {
        std::cout << "���ﳵΪ�գ��޷����й���" << std::endl;
        return Order("", std::vector<CartItem>(), ""); // ������Ч����
    }

    // ����沢���ٿ��
    for (const auto& item : items) {
        if (!productManager.decreaseStock(item.product.name, item.quantity)) {
            std::cout << "��Ʒ " << item.product.name << " ��治�㣬����ʧ�ܡ�" << std::endl;
            return Order("", std::vector<CartItem>(), "");
        }
    }

    // ���ɶ���
    std::stringstream ss;
    std::time_t now = std::time(nullptr);
    ss << "ORD" << std::put_time(std::localtime(&now), "%Y%m%d%H%M%S");
    std::string orderId = ss.str();
    Order order(orderId, items, shippingAddress);

    // ��չ��ﳵ
    items.clear();

    return order;
}

const std::vector<CartItem>& ShoppingCart::getItems() const {
    return items; // ���ع��ﳵ��Ʒ�б�
}
