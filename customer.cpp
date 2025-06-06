//
// Created by 86189 on 25-4-10.
//
// customer.cpp
#include "customer.h"
#include "shop.h"
#include "order.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include <iostream>
#include <stdexcept>

std::vector<Order> Customer::orders;

// void Customer::loadOrdersFromFile() {
//     std::ifstream infile("orders.txt");
//     if (infile.is_open()) {
//         std::string line;
//         while (std::getline(infile, line)) {
//             try {
//                 std::string orderId = line;
//
//                 // ��ȡ��Ʒ����
//                 std::getline(infile, line);
//                 if (line.empty()) throw std::invalid_argument("��Ʒ����Ϊ��");
//                 int itemCount = std::stoi(line);
//
//                 std::vector<CartItem> items;
//                 for (int i = 0; i < itemCount; ++i) {
//                     // ��ȡ��Ʒ��Ϣ
//                     std::getline(infile, line);
//                     std::stringstream ss(line);
//                     std::string productName;
//                     double price;
//                     int quantity;
//
//                     // ������Ʒ���ơ��۸�����
//                     if (!std::getline(ss, productName, ',')) throw std::invalid_argument("��Ʒ������Ч");
//                     if (!(ss >> price)) throw std::invalid_argument("��Ʒ�۸���Ч");
//                     ss.ignore();
//                     if (!(ss >> quantity)) throw std::invalid_argument("��Ʒ������Ч");
//
//                     Product product(productName, price, 0);
//                     items.emplace_back(product, quantity);
//                 }
//
//                 // ��ȡʱ���
//                 std::getline(infile, line);
//                 if (line.empty()) throw std::invalid_argument("ʱ���Ϊ��");
//                 std::time_t purchaseTime = std::stol(line);
//
//                 // ��ȡ�ܼ�
//                 std::getline(infile, line);
//                 if (line.empty()) throw std::invalid_argument("�ܼ�Ϊ��");
//                 double totalPrice = std::stod(line);
//
//                 // ��ȡ�������ܼ�
//                 std::getline(infile, line);
//                 if (line.empty()) throw std::invalid_argument("�������ܼ�Ϊ��");
//                 double promotedTotalPrice = std::stod(line);
//
//                 // ��ȡ��ַ
//                 std::getline(infile, line);
//                 std::string shippingAddress = line;
//
//                 // ��ȡ����״̬
//                 std::getline(infile, line);
//                 if (line.empty()) throw std::invalid_argument("����״̬Ϊ��");
//                 int statusValue = std::stoi(line);
//                 if (statusValue < 0 || statusValue > 2) throw std::invalid_argument("����״̬��Ч");
//                 OrderStatus status = static_cast<OrderStatus>(statusValue);
//
//                 // ��������
//                 Order order(orderId, items, shippingAddress);
//                 order.purchaseTime = purchaseTime;
//                 order.totalPrice = totalPrice;
//                 order.promotedTotalPrice = promotedTotalPrice; // ���ش������ܼ�
//                 order.status = status;
//                 orders.push_back(order);
//
//             } catch (const std::invalid_argument& e) {
//                 std::cerr << "��������ʧ��: " << e.what() << std::endl;
//                 // �����𻵵Ķ�������
//                 infile.close();
//                 return;
//             }
//         }
//         infile.close();
//     }
//     if (!infile.is_open()) {
//         std::cerr << "����: �޷����ļ� 'orders.txt' ���ж�ȡ��" << std::endl;
//     }
//
// }
// ���涩�����ļ�

void Customer::loadOrdersFromFile() {
    // ������ж������ݣ�ȷ�����ص�����������
    orders.clear();

    std::ifstream infile("orders.txt");

    // ����ļ��Ƿ�ɹ���
    if (!infile.is_open()) {
        std::cerr << "����: �޷����ļ� 'orders.txt' ���ж�ȡ��" << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;
    int orderCount = 0;

    while (std::getline(infile, line)) {
        lineNumber++;

        try {
            // ��֤����ID�Ƿ���Ч
            if (line.empty()) {
                throw std::invalid_argument("����IDΪ��");
            }
            std::string orderId = line;

            // ��ȡ��Ʒ����
            if (!std::getline(infile, line)) {
                throw std::runtime_error("�ļ����������ȱ����Ʒ����");
            }
            lineNumber++;

            if (line.empty()) {
                throw std::invalid_argument("��Ʒ����Ϊ��");
            }

            int itemCount = std::stoi(line);

            // ��֤��Ʒ�����Ƿ����
            if (itemCount < 0) {
                throw std::invalid_argument("��Ʒ��������Ϊ����");
            }

            std::vector<CartItem> items;
            for (int i = 0; i < itemCount; ++i) {
                // ��ȡ��Ʒ��Ϣ
                if (!std::getline(infile, line)) {
                    throw std::runtime_error("�ļ����������ȱ����Ʒ��Ϣ");
                }
                lineNumber++;

                std::stringstream ss(line);
                std::string productName;
                double price;
                int quantity;

                // ������Ʒ���ơ��۸�����
                if (!std::getline(ss, productName, ',')) {
                    throw std::invalid_argument("��Ʒ������Ч");
                }

                if (productName.empty()) {
                    throw std::invalid_argument("��Ʒ����Ϊ��");
                }

                if (!(ss >> price)) {
                    throw std::invalid_argument("��Ʒ�۸���Ч");
                }

                if (price < 0) {
                    throw std::invalid_argument("��Ʒ�۸���Ϊ����");
                }

                ss.ignore(); // ���Զ���

                if (!(ss >> quantity)) {
                    throw std::invalid_argument("��Ʒ������Ч");
                }

                if (quantity <= 0) {
                    throw std::invalid_argument("��Ʒ�����������0");
                }

                // ����Ƿ��ж�����ַ�
                std::string remaining;
                if (std::getline(ss, remaining) && !remaining.empty()) {
                    throw std::invalid_argument("��Ʒ��Ϣ��ʽ���󣬰�����������");
                }

                Product product(productName, price, 0);
                items.emplace_back(product, quantity);
            }

            // ��ȡʱ���
            if (!std::getline(infile, line)) {
                throw std::runtime_error("�ļ����������ȱ��ʱ���");
            }
            lineNumber++;

            if (line.empty()) {
                throw std::invalid_argument("ʱ���Ϊ��");
            }

            // ת��ʱ���
            try {
                std::time_t purchaseTime = std::stol(line);

                // ��֤ʱ����Ƿ����
                if (purchaseTime < 0) {
                    throw std::invalid_argument("ʱ�������Ϊ����");
                }

                // ����Ƿ��ں���Χ�ڣ���ѡ��
                const std::time_t currentTime = std::time(nullptr);
                if (purchaseTime > currentTime) {
                    throw std::invalid_argument("ʱ����������ڵ�ǰʱ��");
                }
            } catch (const std::out_of_range&) {
                throw std::invalid_argument("ʱ���ֵ������Χ");
            }

            // ��ȡ�ܼ�
            if (!std::getline(infile, line)) {
                throw std::runtime_error("�ļ����������ȱ���ܼ�");
            }
            lineNumber++;

            if (line.empty()) {
                throw std::invalid_argument("�ܼ�Ϊ��");
            }

            double totalPrice = std::stod(line);

            // ��֤�ܼ��Ƿ����
            if (totalPrice < 0) {
                throw std::invalid_argument("�ܼ۲���Ϊ����");
            }

            // ��ȡ�������ܼ�
            if (!std::getline(infile, line)) {
                throw std::runtime_error("�ļ����������ȱ�ٴ������ܼ�");
            }
            lineNumber++;

            if (line.empty()) {
                throw std::invalid_argument("�������ܼ�Ϊ��");
            }

            double promotedTotalPrice = std::stod(line);

            // ��֤�������ܼ��Ƿ����
            if (promotedTotalPrice < 0) {
                throw std::invalid_argument("�������ܼ۲���Ϊ����");
            }

            if (promotedTotalPrice > totalPrice) {
                throw std::invalid_argument("�������ܼ۲��ܴ���ԭ��");
            }

            // ��ȡ��ַ
            if (!std::getline(infile, line)) {
                throw std::runtime_error("�ļ����������ȱ���ջ���ַ");
            }
            lineNumber++;

            std::string shippingAddress = line;

            // ��֤��ַ�Ƿ���Ч����ѡ��
            if (shippingAddress.empty()) {
                throw std::invalid_argument("�ջ���ַΪ��");
            }

            // ��ȡ����״̬
            if (!std::getline(infile, line)) {
                throw std::runtime_error("�ļ����������ȱ�ٶ���״̬");
            }
            lineNumber++;

            if (line.empty()) {
                throw std::invalid_argument("����״̬Ϊ��");
            }

            int statusValue = std::stoi(line);

            // ��֤����״̬�Ƿ���Ч
            if (statusValue < 0 || statusValue > 2) {
                throw std::invalid_argument("����״̬��Ч����ЧֵӦΪ0-2");
            }

            OrderStatus status = static_cast<OrderStatus>(statusValue);

            // ��������
            Order order(orderId, items, shippingAddress);
            order.purchaseTime = std::stol(line); // ������ʹ����ȷ��ʱ���
            order.totalPrice = totalPrice;
            order.promotedTotalPrice = promotedTotalPrice;
            order.status = status;
            orders.push_back(order);
            orderCount++;

        } catch (const std::invalid_argument& e) {
            std::cerr << "����: �� " << lineNumber << " �����ݸ�ʽ�쳣 - " << e.what() << std::endl;
            // �����𻵵Ķ������ݣ�����������һ������
            continue;
        } catch (const std::out_of_range& e) {
            std::cerr << "����: �� " << lineNumber << " �����ݳ�����Χ - " << e.what() << std::endl;
            continue;
        } catch (const std::runtime_error& e) {
            std::cerr << "����: �� " << lineNumber << " �ж�ȡʧ�� - " << e.what() << std::endl;
            break; // ���ش���ֹͣ����
        } catch (...) {
            std::cerr << "����: �� " << lineNumber << " �з���δ֪�쳣" << std::endl;
            break;
        }
    }

    infile.close();

    if (orderCount > 0) {
        std::cout << "�������سɹ��������� " << orderCount << " ��������" << std::endl;
    } else if (!orders.empty()) {
        std::cout << "����������ɣ������ж��������ڴ���" << std::endl;
    } else {
        std::cout << "�����ļ�Ϊ�ջ��޷������κ���Ч������" << std::endl;
    }
}

void Customer::saveOrdersToFile() {
    std::ofstream outfile("orders.txt");
    if (outfile.is_open()) {
        for (const auto& order : orders) {
            // ���涩��ID
            outfile << order.orderId << std::endl;

            // ������Ʒ����
            outfile << order.items.size() << std::endl;

            // ����ÿ����Ʒ
            for (const auto& item : order.items) {
                outfile << item.product.name << ","
                        << item.product.price << ","
                        << item.quantity << std::endl;
            }

            // ���湺��ʱ��
            outfile << order.purchaseTime << std::endl;

            // �����ܼ�
            outfile << order.totalPrice << std::endl;

            // ����������ܼ�
            outfile << order.promotedTotalPrice << std::endl;

            // �����ջ���ַ
            outfile << order.shippingAddress << std::endl;

            // ���涩��״̬
            outfile << static_cast<int>(order.status) << std::endl;
        }
        outfile.close();
    }
}

void Customer::registerCustomer(const std::string& acc, const std::string& pwd, const std::string& n) {
    account = acc;
    password = pwd;
    name = n;
    std::ofstream outfile("customers.txt", std::ios::app);
    if (outfile.is_open()) {
        outfile << account << "," << password << "," << name << std::endl;
        outfile.close();
    } else {
        std::cerr << "�޷����ļ�����˿���Ϣ" << std::endl;
    }
}

bool Customer::login(const std::string& inputAccount, const std::string& inputPassword) {
    std::ifstream infile("customers.txt");
    std::string line;
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string acc, pwd, n;
        if (std::getline(ss, acc, ',') && std::getline(ss, pwd, ',') && std::getline(ss, n, ',')) {
            if (acc == inputAccount && pwd == inputPassword) {
                account = acc;
                password = pwd;
                name = n;
                isLoggedIn = true;
                return true;
            }
        }
    }
    return false;
}

int Customer::changePassword(const std::string& targetAccount, const std::string& oldPassword, const std::string& newPassword) {
    std::ifstream infile("customers.txt");
    std::vector<std::string> lines;
    std::string line;
    bool found = false;
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string acc, pwd, name;
        if (std::getline(ss, acc, ',') && std::getline(ss, pwd, ',') && std::getline(ss, name)) {
            if (acc == targetAccount) {
                found = true;
                if (pwd == oldPassword) {
                    line = acc + "," + newPassword + "," + name;
                    lines.push_back(line);
                    continue;
                } else {
                    infile.close();
                    return 2; // �������
                }
            }
            lines.push_back(line);
        }
    }
    infile.close();

    if (!found) {
        return 1; // �˻���������
    }

    std::ofstream outfile("customers.txt");
    if (outfile.is_open()) {
        for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
            outfile << *it << std::endl;
        }
        outfile.close();
    } else {
        return -1; // �ļ�����ʧ��
    }
    return 0; // �޸ĳɹ�
}

void Customer::queryShoppingInfo() {
    std::ifstream infile("shopping.txt");
    if (infile.is_open()) {
        std::string line;
        while (std::getline(infile, line)) {
            std::cout << line << std::endl;
        }
        infile.close();
    } else {
        std::cerr << "�޷����ļ���ѯ������Ϣ" << std::endl;
    }
}

// void Customer::purchase(const std::string& shippingAddress) {
//     if (isLoggedIn) {
//         if (shoppingCart.purchase(shippingAddress)) {
//             // ������������ӵ��û������б�
//             std::stringstream ss;
//             std::time_t now = std::time(nullptr);
//             ss << "ORD" << std::put_time(std::localtime(&now), "%Y%m%d%H%M%S");
//             std::string orderId = ss.str();
//             Order order(orderId, shoppingCart.getItems(), shippingAddress);
//
//             // ��Ӷ�������̬�б����浽�ļ�
//             addOrder(order);
//             saveOrdersToFile();
//         }
//     } else {
//         std::cout << "��δ��¼�����ȵ�¼�ٽ��й���" << std::endl;
//     }
// }

void Customer::purchase(const std::string& shippingAddress, const Date& today) {
    if (isLoggedIn) {
        Order order = shoppingCart.purchase(shippingAddress);
        if (!order.orderId.empty()) {
            std::vector<Promotion*> promotions;
            for (const auto& item : order.items) {
                auto productPromotions = productManager.getPromotionsForProduct(item.product.name);
                promotions.insert(promotions.end(), productPromotions.begin(), productPromotions.end());
            }
            order.applyPromotions(promotions, today);

            addOrder(order);
            saveOrdersToFile(); // ���涩��
            std::cout << "���������ɹ���������: " << order.orderId << std::endl;
            std::cout << "ԭ��: " << order.totalPrice << " Ԫ" << std::endl;
            std::cout << "������۸�: " << order.promotedTotalPrice << " Ԫ" << std::endl;
        }
    } else {
        std::cout << "��δ��¼�����ȵ�¼�ٽ��й���" << std::endl;
    }
}

void Customer::addToCart(const std::string& productName, int quantity) {
    if (isLoggedIn) {
        std::vector<Product> products = productManager.queryProductByName(productName);
        if (!products.empty()) {
            shoppingCart.addToCart(products[0], quantity);
        } else {
            std::cout << "δ�ҵ���Ʒ " << productName << "��" << std::endl;
        }
    } else {
        std::cout << "����δ��¼�����ȵ�¼�ٲ������ﳵ��" << std::endl;
    }
}

void Customer::removeFromCart(const std::string& productName, int quantity) {
    if (isLoggedIn) {
        shoppingCart.removeFromCart(productName, quantity);
    } else {
        std::cout << "����δ��¼�����ȵ�¼�ٲ������ﳵ��" << std::endl;
    }
}

void Customer::modifyQuantity(const std::string& productName, int newQuantity) {
    if (isLoggedIn) {
        shoppingCart.modifyQuantity(productName, newQuantity);
    } else {
        std::cout << "����δ��¼�����ȵ�¼�ٲ������ﳵ��" << std::endl;
    }
}

void Customer::queryCartInfo() {
    if (isLoggedIn) {
        shoppingCart.queryCartInfo();
    } else {
        std::cout << "����δ��¼�����ȵ�¼�ٲ������ﳵ��" << std::endl;
    }
}

void Customer::queryOrders() {
    if (isLoggedIn) {
        if (orders.empty()) {
            std::cout << "��û���κζ�����" << std::endl;
        } else {
            for (const auto& order : orders) {
                std::cout << "�������: " << order.orderId << std::endl;
                std::cout << "�µ�ʱ��: " << std::ctime(&order.purchaseTime);
                std::cout << "ԭ��: " << order.totalPrice << " Ԫ" << std::endl;
                std::cout << "������۸�: " << order.promotedTotalPrice << " Ԫ" << std::endl;
                std::cout << "�ջ���ַ: " << order.shippingAddress << std::endl;
                std::cout << "����״̬: ";
                switch (order.status) {
                    case PENDING_SHIPMENT:
                        std::cout << "������" << std::endl;
                        break;
                    case SHIPPED:
                        std::cout << "�ѷ���" << std::endl;
                        break;
                    case RECEIVED:
                        std::cout << "���ջ�" << std::endl;
                        break;
                }
                std::cout << "������Ʒ��Ϣ:" << std::endl;
                for (const auto& item : order.items) {
                    std::cout << "  ��Ʒ����: " << item.product.name << " - �۸�: " << item.product.price << " Ԫ - ����: " << item.quantity << std::endl;
                }
                std::cout << "------------------------" << std::endl;
            }
        }
    } else {
        std::cout << "��δ��¼�����ȵ�¼�ٲ�ѯ������" << std::endl;
    }
}

bool Customer::modifyOrder(const std::string& orderId, const std::string& action, const std::string& newAddress) {
    if (isLoggedIn) {
        for (auto& order : orders) {
            if (order.orderId == orderId) {
                if (order.status == PENDING_SHIPMENT) {
                    if (action == "cancel") {
                        // ȡ��������������Ʒ���
                        for (const auto& item : order.items) {
                            productManager.increaseStock(item.product.name, item.quantity);
                        }
                        order.status = RECEIVED; // ���Ϊ��ȡ��
                        std::cout << "���� " << orderId << " ��ȡ����" << std::endl;
                        saveOrdersToFile(); // �����޸�
                        return true;
                    } else if (action == "change_address") {
                        order.shippingAddress = newAddress;
                        std::cout << "���� " << orderId << " ���ջ���ַ�Ѹ���Ϊ: " << newAddress << std::endl;
                        saveOrdersToFile(); // �����޸�
                        return true;
                    }
                } else {
                    std::cout << "ֻ�д�����״̬�Ķ��������޸ġ�" << std::endl;
                    return false;
                }
            }
        }
        std::cout << "δ�ҵ����� " << orderId << "��" << std::endl;
        return false;
    } else {
        std::cout << "��δ��¼�����ȵ�¼���޸Ķ�����" << std::endl;
        return false;
    }
}

bool Customer::deleteOrder(const std::string& orderId) {
    if (isLoggedIn) {
        for (auto it = orders.begin(); it != orders.end(); ++it) {
            if (it->orderId == orderId) {
                if (it->status == RECEIVED) {
                    orders.erase(it);
                    std::cout << "���� " << orderId << " ��ɾ����" << std::endl;
                    saveOrdersToFile(); // �����޸�
                    return true;
                } else {
                    std::cout << "ֻ�����ջ�״̬�Ķ�������ɾ����" << std::endl;
                    return false;
                }
            }
        }
        std::cout << "δ�ҵ����� " << orderId << "��" << std::endl;
        return false;
    } else {
        std::cout << "��δ��¼�����ȵ�¼��ɾ��������" << std::endl;
        return false;
    }
}

void Customer::addOrder(const Order& order) {
    orders.push_back(order);
}

void Customer::analyzePurchaseData() const {
    // �����洢ͳ������
    std::map<std::string, CategoryStats> categoryStats;

    // �������ж���
    for (const auto& order : orders) {
        // ��¼�ö�������ͳ�Ƶ���Ʒ�������ظ�����
        std::map<std::string, bool> countedProducts;

        // ���������е�ÿ����Ʒ
        for (const auto& item : order.items) {
            const std::string& category = item.product.category;
            const std::string& productName = item.product.name;
            double itemAmount = item.product.price * item.quantity;

            // �������ͳ������
            categoryStats[category].totalAmount += itemAmount;
            categoryStats[category].productAmounts[productName] += itemAmount;

            // ���¹���Ƶ�ʣ�ÿ��������ͬһ��Ʒֻ��һ�Σ�
            if (countedProducts.find(productName) == countedProducts.end()) {
                categoryStats[category].purchaseFrequency++;
                categoryStats[category].productFrequencies[productName]++;
                countedProducts[productName] = true;
            }
        }
    }

    // ���ͳ�ƽ��
    std::cout << "\n===== �˿͹������ݷ��� =====" << std::endl;

    if (categoryStats.empty()) {
        std::cout << "û���ҵ���ɵĶ����������ڷ�����" << std::endl;
        return;
    }

    // �����ܹ�����
    double totalPurchaseAmount = 0.0;
    for (const auto& pair : categoryStats) {
        totalPurchaseAmount += pair.second.totalAmount;
    }

    std::cout << "�ܹ�����: " << std::fixed << std::setprecision(2) << totalPurchaseAmount << "Ԫ"<<std::endl;
    std::cout << "��������: " << orders.size() << std::endl;
    std::cout << "-----------------------------" << std::endl;

    // ���ÿ������ͳ������
    for (const auto& categoryPair : categoryStats) {
        const std::string& category = categoryPair.first;
        const CategoryStats& stats = categoryPair.second;

        std::cout << "\n���: " << category << std::endl;
        std::cout << "  ������: " << std::fixed << std::setprecision(2) << stats.totalAmount<<"Ԫ"
                  << " (" << (totalPurchaseAmount > 0 ? stats.totalAmount / totalPurchaseAmount * 100 : 0)
                  << "%)" << std::endl;
        std::cout << "  ����Ƶ��: " << stats.purchaseFrequency << " ��" << std::endl;

        // ���������µ���Ʒͳ��
        std::cout << "  ��Ʒ��ϸ:" << std::endl;
        for (const auto& productPair : stats.productAmounts) {
            const std::string& productName = productPair.first;
            double productAmount = productPair.second;
            int productFrequency = stats.productFrequencies.at(productName);

            std::cout << "    - " << productName
                      << ": " << std::fixed << std::setprecision(2) << productAmount<<"Ԫ"
                      << " (���� " << productFrequency << " ��)" << std::endl;
        }
    }
}