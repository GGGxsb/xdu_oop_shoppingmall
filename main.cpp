#include <iostream>
#include <thread>
#include <ctime>
#include <chrono>
#include "administrator.h"
#include "customer.h"
#include "products.h"
#include "order.h"

// 自动检查订单状态并转换的函数
void autoUpdateOrderStatus(std::vector<Order>& orders, int seconds) {
    std::time_t now = std::time(nullptr);
    for (auto& order : orders) {
        if (order.status == PENDING_SHIPMENT && (now - order.purchaseTime) >= seconds) {
            order.status = SHIPPED;
            std::cout << "订单 " << order.orderId << " 已自动转换为已发货状态。" << std::endl;
        }
    }
}

int main() {

    Customer::loadOrdersFromFile();
    const auto& orders = Customer::getOrders();

    // 预定义管理员账号密码
    Administrator admin("admin", "admin123");

    ProductManager productManager;
    Customer customer(productManager);

    int choice;
    int exit = 0;
    bool customerLoggedIn = false;
    bool adminLoggedIn = false;

    // 启动一个线程来处理自动状态转换
    std::thread autoUpdateThread([&]() {
        const int checkInterval = 100000;
        while (true) {
            autoUpdateOrderStatus(Customer::getOrders(), checkInterval);
            std::this_thread::sleep_for(std::chrono::seconds(checkInterval));
        }
    });
    autoUpdateThread.detach();

    while (!exit) {
        if (!adminLoggedIn && !customerLoggedIn) {
            std::cout << "1. 管理员登录\n"
                      << "2. 用户注册\n"
                      << "3. 用户登录\n"
                      << "4. 精确查询商品信息\n"
                      << "5. 模糊查询商品信息\n"
                      << "6. 查看商品列表\n"
                      << "7. 退出系统\n"
                      << "请选择: " << std::endl;
        } else if (adminLoggedIn) {
            std::cout << "1. 商品上架\n"
                      << "2. 删除用户账号\n"
                      << "3. 重置用户密码\n"
                      << "4. 精确查询商品信息\n"
                      << "5. 模糊查询商品信息\n"
                      << "6. 删除商品信息\n"
                      << "7. 手动设置订单状态\n"
                      << "8. 退出管理员登录\n"
                      << "9. 退出系统\n"
                      << "请选择: ";
        } else if (customerLoggedIn) {
            std::cout << "1. 修改用户密码\n"
                      << "2. 精确查询商品信息\n"
                      << "3. 模糊查询商品信息\n"
                      << "4. 添加商品到购物车\n"
                      << "5. 删除购物车中的商品\n"
                      << "6. 修改购物车中商品的购买数量\n"
                      << "7. 查询购物车中的商品信息\n"
                      << "8. 结算购物车中的商品\n"
                      << "9. 查看商品列表\n"
                      << "10. 从商品列表选择商品购买\n"
                      << "11. **查询订单**\n"
                      << "12. **修改订单（取消或更改地址）**\n"
                      << "13. **删除已收货状态的订单**\n"
                      << "14. 退出用户登录\n"
                      << "15. 退出系统\n"
                      << "请选择: ";
        }

        std::cin >> choice;
        switch (choice) {
            case 1: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::string inputAccount, inputPassword;
                    std::cout << "请输入管理员账号: ";
                    std::cin >> inputAccount;
                    std::cout << "请输入管理员密码: ";
                    std::cin >> inputPassword;
                    if (admin.login(inputAccount, inputPassword)) {
                        std::cout << "管理员登录成功" << std::endl;
                        adminLoggedIn = true;
                    } else {
                        std::cout << "管理员登录失败" << std::endl;
                    }
                } else if (adminLoggedIn) {
                    std::string name;
                    double price;
                    int stock;
                    std::cout << "请输入商品名称: ";
                    std::cin >> name;
                    std::cout << "请输入商品价格: ";
                    std::cin >> price;
                    std::cout << "请输入商品库存: ";
                    std::cin >> stock;
                    productManager.addProduct(name, price, stock);
                } else if (customerLoggedIn) {
                    std::string targetAccount, oldPassword, newPassword;
                    std::cout << "请输入需要修改密码的用户账号: ";
                    std::cin >> targetAccount;
                    std::cout << "请输入旧密码: ";
                    std::cin >> oldPassword;
                    std::cout << "请输入新密码: ";
                    std::cin >> newPassword;
                    int result = customer.changePassword(targetAccount, oldPassword, newPassword);
                    switch (result) {
                        case 0:
                            std::cout << "密码修改成功" << std::endl;
                            break;
                        case 1:
                            std::cout << "账号不存在，密码修改失败" << std::endl;
                            break;
                        case 2:
                            std::cout << "旧密码错误，密码修改失败" << std::endl;
                            break;
                        case -1:
                            std::cout << "文件操作失败，密码修改失败" << std::endl;
                            break;
                    }
                }
                break;
            }
            case 2: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::string acc, pwd, n;
                    std::cout << "请输入注册账号: ";
                    std::cin >> acc;
                    std::cout << "请输入注册密码: ";
                    std::cin >> pwd;
                    std::cout << "请输入姓名: ";
                    std::cin >> n;
                    customer.registerCustomer(acc, pwd, n);
                    std::cout << "用户注册成功" << std::endl;
                } else if (adminLoggedIn) {
                    std::string targetAccount;
                    std::cout << "请输入需要删除的用户账号: ";
                    std::cin >> targetAccount;
                    if (admin.deleteCustomer(targetAccount)) {
                        std::cout << "用户账号删除成功" << std::endl;
                    } else {
                        std::cout << "未找到该用户账号或删除失败" << std::endl;
                    }
                } else if (customerLoggedIn) {
                    std::string targetName;
                    std::cout << "请输入需要精确查询的商品名称: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "未找到该商品" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "商品名称: " << product.name << " - " << product.price << " 元 - 库存: " << product.stock << std::endl;
                        }
                    }
                }
                break;
            }
            case 3: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::string inputAccount, inputPassword;
                    std::cout << "请输入用户账号: ";
                    std::cin >> inputAccount;
                    std::cout << "请输入用户密码: ";
                    std::cin >> inputPassword;
                    if (customer.login(inputAccount, inputPassword)) {
                        std::cout << "用户登录成功" << std::endl;
                        customerLoggedIn = true;
                    } else {
                        std::cout << "用户登录失败" << std::endl;
                    }
                } else if (adminLoggedIn) {
                    std::string targetAccount, oldPassword, newPassword;
                    std::cout << "请输入需要重置密码的用户账号: ";
                    std::cin >> targetAccount;
                    std::cout << "请输入旧密码: ";
                    std::cin >> oldPassword;
                    std::cout << "请输入新密码: ";
                    std::cin >> newPassword;
                    int result = admin.resetCustomerPassword(targetAccount, oldPassword, newPassword);
                    switch (result) {
                        case 0:
                            std::cout << "密码重置成功" << std::endl;
                            break;
                        case 1:
                            std::cout << "账号不存在，密码重置失败" << std::endl;
                            break;
                        case 2:
                            std::cout << "旧密码错误，密码重置失败" << std::endl;
                            break;
                        case -1:
                            std::cout << "文件操作失败，密码重置失败" << std::endl;
                            break;
                    }
                } else if (customerLoggedIn) {
                    std::string keyword;
                    std::cout << "请输入需要模糊查询的关键字: ";
                    std::cin >> keyword;
                    auto foundProducts = productManager.queryProductByFuzzyName(keyword);
                    if (foundProducts.empty()) {
                        std::cout << "未找到包含该关键字的商品" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "商品名称: " << product.name << " - " << product.price << " 元 - 库存: " << product.stock << std::endl;
                        }
                    }
                }
                break;
            }
            case 4: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::string targetName;
                    std::cout << "请输入需要精确查询的商品名称: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "未找到该商品" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "商品名称: " << product.name << " - " << product.price << " 元 - 库存: " << product.stock << std::endl;
                        }
                    }
                } else if (adminLoggedIn) {
                    std::string targetName;
                    std::cout << "请输入需要精确查询的商品名称: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "未找到该商品" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "商品名称: " << product.name << " - " << product.price << " 元 - 库存: " << product.stock << std::endl;
                        }
                    }
                } else if (customerLoggedIn) {
                    std::string productName;
                    int quantity;
                    std::cout << "请输入需要添加到购物车的商品名称: ";
                    std::cin >> productName;
                    std::cout << "请输入需要添加的商品数量: ";
                    std::cin >> quantity;
                    customer.addToCart(productName, quantity);
                }
                break;
            }
            case 5: {
                if (customerLoggedIn) {
                    std::string productName;
                    int quantity;
                    std::cout << "请输入需要从购物车中删除的商品名称: ";
                    std::cin >> productName;
                    std::cout << "请输入需要删除的商品数量: ";
                    std::cin >> quantity;
                    customer.removeFromCart(productName, quantity);
                } else if (!adminLoggedIn && !customerLoggedIn) {
                    std::string keyword;
                    std::cout << "请输入需要模糊查询的关键字: ";
                    std::cin >> keyword;
                    auto foundProducts = productManager.queryProductByFuzzyName(keyword);
                    if (foundProducts.empty()) {
                        std::cout << "未找到包含该关键字的商品" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "商品名称: " << product.name << " - " << product.price << " 元 - 库存: " << product.stock << std::endl;
                        }
                    }
                }
                break;
            }
            case 6: {
                if (customerLoggedIn) {
                    std::string productName;
                    int newQuantity;
                    std::cout << "请输入需要修改购买数量的商品名称: ";
                    std::cin >> productName;
                    std::cout << "请输入新的购买数量: ";
                    std::cin >> newQuantity;
                    customer.modifyQuantity(productName, newQuantity);
                } else if (!adminLoggedIn && !customerLoggedIn) {
                    const auto& products = productManager.getProducts();
                    if (products.empty()) {
                        std::cout << "暂无商品信息" << std::endl;
                    } else {
                        for (size_t i = 0; i < products.size(); ++i) {
                            std::cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " 元 - 库存: " << products[i].stock << std::endl;
                        }
                    }
                } else if (adminLoggedIn) {
                    std::string targetName;
                    std::cout << "请输入需要删除的商品名称: ";
                    std::cin >> targetName;
                    if (productManager.deleteProduct(targetName)) {
                        std::cout << "商品删除成功" << std::endl;
                    } else {
                        std::cout << "未找到该商品或删除失败" << std::endl;
                    }
                }
                break;
            }
            case 7: {
                if (customerLoggedIn) {
                    customer.queryCartInfo();
                } else if (!adminLoggedIn && !customerLoggedIn) {
                    std::cout << "退出系统" << std::endl;
                    exit = 1;
                } else if (adminLoggedIn) {
                    std::string orderId;
                    int newStatus;
                    std::cout << "请输入需要设置状态的订单编号: ";
                    std::cin >> orderId;
                    std::cout << "请选择新的订单状态 (0: 待发货, 1: 已发货, 2: 已签收): ";
                    std::cin >> newStatus;
                    if (newStatus < 0 || newStatus > 2) {
                        std::cout << "无效的订单状态" << std::endl;
                    } else {
                        for (auto& order : Customer::getOrders()) {
                            if (order.orderId == orderId) {
                                if (admin.setOrderStatus(order, static_cast<OrderStatus>(newStatus))) {
                                    std::cout << "订单 " << orderId << " 状态已更新为 ";
                                    switch (newStatus) {
                                        case 0:
                                            std::cout << "待发货" << std::endl;
                                            break;
                                        case 1:
                                            std::cout << "已发货" << std::endl;
                                            break;
                                        case 2:
                                            std::cout << "已签收" << std::endl;
                                            break;
                                    }
                                    Customer::saveOrdersToFile();
                                } else {
                                    std::cout << "无法将订单状态设置为指定状态" << std::endl;
                                }
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case 8: {
                if (customerLoggedIn) {
                    std::string addr;
                    std::cout << "请输入收货地址: ";
                    std::cin.ignore(); // 清除缓冲区
                    std::getline(std::cin, addr);
                    customer.purchase(addr);
                    std::cout << "订单创建成功" << std::endl;
                } else if (adminLoggedIn) {
                    std::cout << "退出管理员登录" << std::endl;
                    adminLoggedIn = false;
                }
                break;
            }
            case 9: {
                if (customerLoggedIn) {
                    const auto& products = productManager.getProducts();
                    if (products.empty()) {
                        std::cout << "暂无商品信息" << std::endl;
                    } else {
                        for (size_t i = 0; i < products.size(); ++i) {
                            std::cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " 元 - 库存: " << products[i].stock << std::endl;
                        }
                    }
                } else if (adminLoggedIn) {
                    // 退出系统前保存订单
                    Customer::saveOrdersToFile();
                    std::cout << "订单信息已保存，系统退出" << std::endl;
                    exit = 1;
                }
                break;
            }
            case 10: {
                std::string shippingAddress;
                if (customerLoggedIn) {
                    const auto& products = productManager.getProducts();
                    if (products.empty()) {
                        std::cout << "暂无商品信息，无法进行购买" << std::endl;
                    } else {
                        std::cout << "商品列表如下:" << std::endl;
                        for (size_t i = 0; i < products.size(); ++i) {
                            std::cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " 元 - 库存: " << products[i].stock << std::endl;
                        }
                        int productIndex;
                        int quantity;
                        std::cout << "请输入需要购买的商品序号: ";
                        std::cin >> productIndex;
                        if (productIndex > 0 && productIndex <= static_cast<int>(products.size())) {
                            std::cout << "请输入需要购买的商品数量: ";
                            std::cin >> quantity;
                            const Product& selectedProduct = products[productIndex - 1];
                            if (selectedProduct.stock >= quantity) {
                                customer.addToCart(selectedProduct.name, quantity);
                                std::cout << "请输入收货地址: " << std::endl;
                                std::cin >> shippingAddress;
                                customer.purchase(shippingAddress);
                            } else {
                                std::cout << "商品库存不足，无法进行购买" << std::endl;
                            }
                        } else {
                            std::cout << "无效的商品序号" << std::endl;
                        }
                    }
                }
                break;
            }
            case 11: {
                // **查询订单**
                if (customerLoggedIn) {
                    customer.queryOrders();
                }
                break;
            }
            case 12: {
                // **修改订单（取消或更改地址）**
                if (customerLoggedIn) {
                    std::string orderId, action, newAddress;
                    std::cout << "请输入需要修改的订单编号: ";
                    std::cin >> orderId;
                    std::cout << "请选择操作 (1: 取消订单, 2: 更改收货地址): ";
                    int op;
                    std::cin >> op;
                    if (op == 1) {
                        action = "cancel";
                        if (customer.modifyOrder(orderId, action)) {
                            std::cout << "订单取消成功，商品已返还" << std::endl;
                        }
                    } else if (op == 2) {
                        action = "change_address";
                        std::cout << "请输入新的收货地址: ";
                        std::cin.ignore(); // 清除缓冲区
                        std::getline(std::cin, newAddress);
                        if (customer.modifyOrder(orderId, action, newAddress)) {
                            std::cout << "收货地址修改成功" << std::endl;
                        }
                    } else {
                        std::cout << "无效的操作" << std::endl;
                    }
                }
                break;
            }
            case 13: {
                // **删除已收货状态的订单**
                if (customerLoggedIn) {
                    std::string orderId;
                    std::cout << "请输入需要删除的订单编号: ";
                    std::cin >> orderId;
                    if (customer.deleteOrder(orderId)) {
                        std::cout << "订单删除成功" << std::endl;
                    }
                }
                break;
            }
            case 14: {
                // 退出用户登录
                if (customerLoggedIn) {
                    customerLoggedIn = false;
                    std::cout << "已退出用户登录" << std::endl;
                }
                break;
            }
            case 15: {
                // 退出系统前保存订单
                if (customerLoggedIn || adminLoggedIn) {
                    Customer::saveOrdersToFile();
                    std::cout << "订单信息已保存，系统退出" << std::endl;
                }
                exit = 1;
                break;
            }
            default:
                std::cout << "无效选择，请重新输入" << std::endl;
        }
    }
    return 0;
}