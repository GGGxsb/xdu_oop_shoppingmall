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
//                 // 读取商品数量
//                 std::getline(infile, line);
//                 if (line.empty()) throw std::invalid_argument("商品数量为空");
//                 int itemCount = std::stoi(line);
//
//                 std::vector<CartItem> items;
//                 for (int i = 0; i < itemCount; ++i) {
//                     // 读取商品信息
//                     std::getline(infile, line);
//                     std::stringstream ss(line);
//                     std::string productName;
//                     double price;
//                     int quantity;
//
//                     // 解析商品名称、价格、数量
//                     if (!std::getline(ss, productName, ',')) throw std::invalid_argument("商品名称无效");
//                     if (!(ss >> price)) throw std::invalid_argument("商品价格无效");
//                     ss.ignore();
//                     if (!(ss >> quantity)) throw std::invalid_argument("商品数量无效");
//
//                     Product product(productName, price, 0);
//                     items.emplace_back(product, quantity);
//                 }
//
//                 // 读取时间戳
//                 std::getline(infile, line);
//                 if (line.empty()) throw std::invalid_argument("时间戳为空");
//                 std::time_t purchaseTime = std::stol(line);
//
//                 // 读取总价
//                 std::getline(infile, line);
//                 if (line.empty()) throw std::invalid_argument("总价为空");
//                 double totalPrice = std::stod(line);
//
//                 // 获取促销后总价
//                 std::getline(infile, line);
//                 if (line.empty()) throw std::invalid_argument("促销后总价为空");
//                 double promotedTotalPrice = std::stod(line);
//
//                 // 读取地址
//                 std::getline(infile, line);
//                 std::string shippingAddress = line;
//
//                 // 读取订单状态
//                 std::getline(infile, line);
//                 if (line.empty()) throw std::invalid_argument("订单状态为空");
//                 int statusValue = std::stoi(line);
//                 if (statusValue < 0 || statusValue > 2) throw std::invalid_argument("订单状态无效");
//                 OrderStatus status = static_cast<OrderStatus>(statusValue);
//
//                 // 创建订单
//                 Order order(orderId, items, shippingAddress);
//                 order.purchaseTime = purchaseTime;
//                 order.totalPrice = totalPrice;
//                 order.promotedTotalPrice = promotedTotalPrice; // 加载促销后总价
//                 order.status = status;
//                 orders.push_back(order);
//
//             } catch (const std::invalid_argument& e) {
//                 std::cerr << "订单解析失败: " << e.what() << std::endl;
//                 // 跳过损坏的订单数据
//                 infile.close();
//                 return;
//             }
//         }
//         infile.close();
//     }
//     if (!infile.is_open()) {
//         std::cerr << "错误: 无法打开文件 'orders.txt' 进行读取。" << std::endl;
//     }
//
// }
// 保存订单到文件

void Customer::loadOrdersFromFile() {
    // 清空现有订单数据，确保加载的是完整数据
    orders.clear();

    std::ifstream infile("orders.txt");

    // 检查文件是否成功打开
    if (!infile.is_open()) {
        std::cerr << "错误: 无法打开文件 'orders.txt' 进行读取。" << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;
    int orderCount = 0;

    while (std::getline(infile, line)) {
        lineNumber++;

        try {
            // 验证订单ID是否有效
            if (line.empty()) {
                throw std::invalid_argument("订单ID为空");
            }
            std::string orderId = line;

            // 读取商品数量
            if (!std::getline(infile, line)) {
                throw std::runtime_error("文件意外结束，缺少商品数量");
            }
            lineNumber++;

            if (line.empty()) {
                throw std::invalid_argument("商品数量为空");
            }

            int itemCount = std::stoi(line);

            // 验证商品数量是否合理
            if (itemCount < 0) {
                throw std::invalid_argument("商品数量不能为负数");
            }

            std::vector<CartItem> items;
            for (int i = 0; i < itemCount; ++i) {
                // 读取商品信息
                if (!std::getline(infile, line)) {
                    throw std::runtime_error("文件意外结束，缺少商品信息");
                }
                lineNumber++;

                std::stringstream ss(line);
                std::string productName;
                double price;
                int quantity;

                // 解析商品名称、价格、数量
                if (!std::getline(ss, productName, ',')) {
                    throw std::invalid_argument("商品名称无效");
                }

                if (productName.empty()) {
                    throw std::invalid_argument("商品名称为空");
                }

                if (!(ss >> price)) {
                    throw std::invalid_argument("商品价格无效");
                }

                if (price < 0) {
                    throw std::invalid_argument("商品价格不能为负数");
                }

                ss.ignore(); // 忽略逗号

                if (!(ss >> quantity)) {
                    throw std::invalid_argument("商品数量无效");
                }

                if (quantity <= 0) {
                    throw std::invalid_argument("商品数量必须大于0");
                }

                // 检查是否有多余的字符
                std::string remaining;
                if (std::getline(ss, remaining) && !remaining.empty()) {
                    throw std::invalid_argument("商品信息格式错误，包含多余内容");
                }

                Product product(productName, price, 0);
                items.emplace_back(product, quantity);
            }

            // 读取时间戳
            if (!std::getline(infile, line)) {
                throw std::runtime_error("文件意外结束，缺少时间戳");
            }
            lineNumber++;

            if (line.empty()) {
                throw std::invalid_argument("时间戳为空");
            }

            // 转换时间戳
            try {
                std::time_t purchaseTime = std::stol(line);

                // 验证时间戳是否合理
                if (purchaseTime < 0) {
                    throw std::invalid_argument("时间戳不能为负数");
                }

                // 检查是否在合理范围内（可选）
                const std::time_t currentTime = std::time(nullptr);
                if (purchaseTime > currentTime) {
                    throw std::invalid_argument("时间戳不能晚于当前时间");
                }
            } catch (const std::out_of_range&) {
                throw std::invalid_argument("时间戳值超出范围");
            }

            // 读取总价
            if (!std::getline(infile, line)) {
                throw std::runtime_error("文件意外结束，缺少总价");
            }
            lineNumber++;

            if (line.empty()) {
                throw std::invalid_argument("总价为空");
            }

            double totalPrice = std::stod(line);

            // 验证总价是否合理
            if (totalPrice < 0) {
                throw std::invalid_argument("总价不能为负数");
            }

            // 获取促销后总价
            if (!std::getline(infile, line)) {
                throw std::runtime_error("文件意外结束，缺少促销后总价");
            }
            lineNumber++;

            if (line.empty()) {
                throw std::invalid_argument("促销后总价为空");
            }

            double promotedTotalPrice = std::stod(line);

            // 验证促销后总价是否合理
            if (promotedTotalPrice < 0) {
                throw std::invalid_argument("促销后总价不能为负数");
            }

            if (promotedTotalPrice > totalPrice) {
                throw std::invalid_argument("促销后总价不能大于原价");
            }

            // 读取地址
            if (!std::getline(infile, line)) {
                throw std::runtime_error("文件意外结束，缺少收货地址");
            }
            lineNumber++;

            std::string shippingAddress = line;

            // 验证地址是否有效（可选）
            if (shippingAddress.empty()) {
                throw std::invalid_argument("收货地址为空");
            }

            // 读取订单状态
            if (!std::getline(infile, line)) {
                throw std::runtime_error("文件意外结束，缺少订单状态");
            }
            lineNumber++;

            if (line.empty()) {
                throw std::invalid_argument("订单状态为空");
            }

            int statusValue = std::stoi(line);

            // 验证订单状态是否有效
            if (statusValue < 0 || statusValue > 2) {
                throw std::invalid_argument("订单状态无效，有效值应为0-2");
            }

            OrderStatus status = static_cast<OrderStatus>(statusValue);

            // 创建订单
            Order order(orderId, items, shippingAddress);
            order.purchaseTime = std::stol(line); // 修正：使用正确的时间戳
            order.totalPrice = totalPrice;
            order.promotedTotalPrice = promotedTotalPrice;
            order.status = status;
            orders.push_back(order);
            orderCount++;

        } catch (const std::invalid_argument& e) {
            std::cerr << "错误: 第 " << lineNumber << " 行数据格式异常 - " << e.what() << std::endl;
            // 跳过损坏的订单数据，继续处理下一个订单
            continue;
        } catch (const std::out_of_range& e) {
            std::cerr << "错误: 第 " << lineNumber << " 行数据超出范围 - " << e.what() << std::endl;
            continue;
        } catch (const std::runtime_error& e) {
            std::cerr << "错误: 第 " << lineNumber << " 行读取失败 - " << e.what() << std::endl;
            break; // 严重错误，停止处理
        } catch (...) {
            std::cerr << "错误: 第 " << lineNumber << " 行发生未知异常" << std::endl;
            break;
        }
    }

    infile.close();

    if (orderCount > 0) {
        std::cout << "订单加载成功，共加载 " << orderCount << " 个订单。" << std::endl;
    } else if (!orders.empty()) {
        std::cout << "订单加载完成，但所有订单均存在错误。" << std::endl;
    } else {
        std::cout << "订单文件为空或无法加载任何有效订单。" << std::endl;
    }
}

void Customer::saveOrdersToFile() {
    std::ofstream outfile("orders.txt");
    if (outfile.is_open()) {
        for (const auto& order : orders) {
            // 保存订单ID
            outfile << order.orderId << std::endl;

            // 保存商品数量
            outfile << order.items.size() << std::endl;

            // 保存每个商品
            for (const auto& item : order.items) {
                outfile << item.product.name << ","
                        << item.product.price << ","
                        << item.quantity << std::endl;
            }

            // 保存购买时间
            outfile << order.purchaseTime << std::endl;

            // 保存总价
            outfile << order.totalPrice << std::endl;

            // 保存促销后总价
            outfile << order.promotedTotalPrice << std::endl;

            // 保存收货地址
            outfile << order.shippingAddress << std::endl;

            // 保存订单状态
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
        std::cerr << "无法打开文件保存顾客信息" << std::endl;
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
                    return 2; // 密码错误
                }
            }
            lines.push_back(line);
        }
    }
    infile.close();

    if (!found) {
        return 1; // 账户名不存在
    }

    std::ofstream outfile("customers.txt");
    if (outfile.is_open()) {
        for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
            outfile << *it << std::endl;
        }
        outfile.close();
    } else {
        return -1; // 文件操作失败
    }
    return 0; // 修改成功
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
        std::cerr << "无法打开文件查询购物信息" << std::endl;
    }
}

// void Customer::purchase(const std::string& shippingAddress) {
//     if (isLoggedIn) {
//         if (shoppingCart.purchase(shippingAddress)) {
//             // 创建订单并添加到用户订单列表
//             std::stringstream ss;
//             std::time_t now = std::time(nullptr);
//             ss << "ORD" << std::put_time(std::localtime(&now), "%Y%m%d%H%M%S");
//             std::string orderId = ss.str();
//             Order order(orderId, shoppingCart.getItems(), shippingAddress);
//
//             // 添加订单到静态列表并保存到文件
//             addOrder(order);
//             saveOrdersToFile();
//         }
//     } else {
//         std::cout << "您未登录，请先登录再进行购买" << std::endl;
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
            saveOrdersToFile(); // 保存订单
            std::cout << "订单创建成功，订单号: " << order.orderId << std::endl;
            std::cout << "原价: " << order.totalPrice << " 元" << std::endl;
            std::cout << "促销后价格: " << order.promotedTotalPrice << " 元" << std::endl;
        }
    } else {
        std::cout << "您未登录，请先登录再进行购买" << std::endl;
    }
}

void Customer::addToCart(const std::string& productName, int quantity) {
    if (isLoggedIn) {
        std::vector<Product> products = productManager.queryProductByName(productName);
        if (!products.empty()) {
            shoppingCart.addToCart(products[0], quantity);
        } else {
            std::cout << "未找到商品 " << productName << "。" << std::endl;
        }
    } else {
        std::cout << "您尚未登录，请先登录再操作购物车。" << std::endl;
    }
}

void Customer::removeFromCart(const std::string& productName, int quantity) {
    if (isLoggedIn) {
        shoppingCart.removeFromCart(productName, quantity);
    } else {
        std::cout << "您尚未登录，请先登录再操作购物车。" << std::endl;
    }
}

void Customer::modifyQuantity(const std::string& productName, int newQuantity) {
    if (isLoggedIn) {
        shoppingCart.modifyQuantity(productName, newQuantity);
    } else {
        std::cout << "您尚未登录，请先登录再操作购物车。" << std::endl;
    }
}

void Customer::queryCartInfo() {
    if (isLoggedIn) {
        shoppingCart.queryCartInfo();
    } else {
        std::cout << "您尚未登录，请先登录再操作购物车。" << std::endl;
    }
}

void Customer::queryOrders() {
    if (isLoggedIn) {
        if (orders.empty()) {
            std::cout << "您没有任何订单。" << std::endl;
        } else {
            for (const auto& order : orders) {
                std::cout << "订单编号: " << order.orderId << std::endl;
                std::cout << "下单时间: " << std::ctime(&order.purchaseTime);
                std::cout << "原价: " << order.totalPrice << " 元" << std::endl;
                std::cout << "促销后价格: " << order.promotedTotalPrice << " 元" << std::endl;
                std::cout << "收货地址: " << order.shippingAddress << std::endl;
                std::cout << "订单状态: ";
                switch (order.status) {
                    case PENDING_SHIPMENT:
                        std::cout << "待发货" << std::endl;
                        break;
                    case SHIPPED:
                        std::cout << "已发货" << std::endl;
                        break;
                    case RECEIVED:
                        std::cout << "已收货" << std::endl;
                        break;
                }
                std::cout << "订单商品信息:" << std::endl;
                for (const auto& item : order.items) {
                    std::cout << "  商品名称: " << item.product.name << " - 价格: " << item.product.price << " 元 - 数量: " << item.quantity << std::endl;
                }
                std::cout << "------------------------" << std::endl;
            }
        }
    } else {
        std::cout << "您未登录，请先登录再查询订单。" << std::endl;
    }
}

bool Customer::modifyOrder(const std::string& orderId, const std::string& action, const std::string& newAddress) {
    if (isLoggedIn) {
        for (auto& order : orders) {
            if (order.orderId == orderId) {
                if (order.status == PENDING_SHIPMENT) {
                    if (action == "cancel") {
                        // 取消订单，更新商品库存
                        for (const auto& item : order.items) {
                            productManager.increaseStock(item.product.name, item.quantity);
                        }
                        order.status = RECEIVED; // 标记为已取消
                        std::cout << "订单 " << orderId << " 已取消。" << std::endl;
                        saveOrdersToFile(); // 保存修改
                        return true;
                    } else if (action == "change_address") {
                        order.shippingAddress = newAddress;
                        std::cout << "订单 " << orderId << " 的收货地址已更新为: " << newAddress << std::endl;
                        saveOrdersToFile(); // 保存修改
                        return true;
                    }
                } else {
                    std::cout << "只有待发货状态的订单可以修改。" << std::endl;
                    return false;
                }
            }
        }
        std::cout << "未找到订单 " << orderId << "。" << std::endl;
        return false;
    } else {
        std::cout << "您未登录，请先登录再修改订单。" << std::endl;
        return false;
    }
}

bool Customer::deleteOrder(const std::string& orderId) {
    if (isLoggedIn) {
        for (auto it = orders.begin(); it != orders.end(); ++it) {
            if (it->orderId == orderId) {
                if (it->status == RECEIVED) {
                    orders.erase(it);
                    std::cout << "订单 " << orderId << " 已删除。" << std::endl;
                    saveOrdersToFile(); // 保存修改
                    return true;
                } else {
                    std::cout << "只有已收货状态的订单可以删除。" << std::endl;
                    return false;
                }
            }
        }
        std::cout << "未找到订单 " << orderId << "。" << std::endl;
        return false;
    } else {
        std::cout << "您未登录，请先登录再删除订单。" << std::endl;
        return false;
    }
}

void Customer::addOrder(const Order& order) {
    orders.push_back(order);
}

void Customer::analyzePurchaseData() const {
    // 按类别存储统计数据
    std::map<std::string, CategoryStats> categoryStats;

    // 遍历所有订单
    for (const auto& order : orders) {
        // 记录该订单中已统计的商品，避免重复计数
        std::map<std::string, bool> countedProducts;

        // 遍历订单中的每个商品
        for (const auto& item : order.items) {
            const std::string& category = item.product.category;
            const std::string& productName = item.product.name;
            double itemAmount = item.product.price * item.quantity;

            // 更新类别统计数据
            categoryStats[category].totalAmount += itemAmount;
            categoryStats[category].productAmounts[productName] += itemAmount;

            // 更新购买频率（每个订单中同一商品只计一次）
            if (countedProducts.find(productName) == countedProducts.end()) {
                categoryStats[category].purchaseFrequency++;
                categoryStats[category].productFrequencies[productName]++;
                countedProducts[productName] = true;
            }
        }
    }

    // 输出统计结果
    std::cout << "\n===== 顾客购买数据分析 =====" << std::endl;

    if (categoryStats.empty()) {
        std::cout << "没有找到完成的订单数据用于分析。" << std::endl;
        return;
    }

    // 计算总购买金额
    double totalPurchaseAmount = 0.0;
    for (const auto& pair : categoryStats) {
        totalPurchaseAmount += pair.second.totalAmount;
    }

    std::cout << "总购买金额: " << std::fixed << std::setprecision(2) << totalPurchaseAmount << "元"<<std::endl;
    std::cout << "订单总数: " << orders.size() << std::endl;
    std::cout << "-----------------------------" << std::endl;

    // 输出每个类别的统计数据
    for (const auto& categoryPair : categoryStats) {
        const std::string& category = categoryPair.first;
        const CategoryStats& stats = categoryPair.second;

        std::cout << "\n类别: " << category << std::endl;
        std::cout << "  购买金额: " << std::fixed << std::setprecision(2) << stats.totalAmount<<"元"
                  << " (" << (totalPurchaseAmount > 0 ? stats.totalAmount / totalPurchaseAmount * 100 : 0)
                  << "%)" << std::endl;
        std::cout << "  购买频度: " << stats.purchaseFrequency << " 次" << std::endl;

        // 输出该类别下的商品统计
        std::cout << "  商品明细:" << std::endl;
        for (const auto& productPair : stats.productAmounts) {
            const std::string& productName = productPair.first;
            double productAmount = productPair.second;
            int productFrequency = stats.productFrequencies.at(productName);

            std::cout << "    - " << productName
                      << ": " << std::fixed << std::setprecision(2) << productAmount<<"元"
                      << " (购买 " << productFrequency << " 次)" << std::endl;
        }
    }
}