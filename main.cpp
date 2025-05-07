#include <iostream>
#include "administrator.h"
#include "customer.h"
#include "products.h"
#include "shop.h"

int main() {
    // 预定义管理员账号密码
    Administrator admin("admin", "admin123");

    ProductManager productManager;
    Customer customer(productManager);

    int choice;
    int exit = 0;
    bool customerLoggedIn = false;
    bool adminLoggedIn = false;

    while (!exit) {
        if (!adminLoggedIn && !customerLoggedIn) {
            std::cout << "1. 管理员登录\n"
                         "2. 顾客注册\n"
                         "3. 顾客登录\n"
                         "4. 精确查询商品信息\n"
                         "5. 模糊查询商品信息\n"
                         "6. 查看商品列表\n"
                         "7. 退出系统\n"
                         "请选择: ";
        } else if (adminLoggedIn) {
            std::cout << "1. 商品上架\n"
                         "2. 删除顾客账号\n"
                         "3. 重置顾客密码\n"
                         "4. 精确查询商品信息\n"
                         "5. 模糊查询商品信息\n"
                         "6. 删除商品信息\n"
                         "7. 退出管理员登录\n"
                         "8. 退出系统\n"
                         "请选择: ";
        } else if (customerLoggedIn) {
            std::cout << "1. 修改顾客密码\n"
                         "2. 精确查询商品信息\n"
                         "3. 模糊查询商品信息\n"
                         "4. 添加商品到购物车\n"
                         "5. 删除购物车中的商品\n"
                         "6. 修改购物车中商品的购买数量\n"
                         "7. 查询购物车内的商品信息\n"
                         "8. 购买购物车中的商品\n"
                         "9. 退出顾客登录\n"
                         "10. 退出系统\n"
                         "请选择: ";
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
                    std::cout << "请输入要修改密码的账号: ";
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
                    std::cout << "顾客注册成功" << std::endl;
                } else if (adminLoggedIn) {
                    std::string targetAccount;
                    std::cout << "请输入要删除的顾客账号: ";
                    std::cin >> targetAccount;
                    if (admin.deleteCustomer(targetAccount)) {
                        std::cout << "顾客账号删除成功" << std::endl;
                    } else {
                        std::cout << "未找到该顾客账号或删除失败" << std::endl;
                    }
                } else if (customerLoggedIn) {
                    std::string targetName;
                    std::cout << "请输入要精确查询的商品名称: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "未找到该商品。" << std::endl;
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
                    std::cout << "请输入顾客账号: ";
                    std::cin >> inputAccount;
                    std::cout << "请输入顾客密码: ";
                    std::cin >> inputPassword;
                    if (customer.login(inputAccount, inputPassword)) {
                        std::cout << "顾客登录成功" << std::endl;
                        customerLoggedIn = true;
                    } else {
                        std::cout << "顾客登录失败" << std::endl;
                    }
                } else if (adminLoggedIn) {
                    std::string targetAccount, oldPassword, newPassword;
                    std::cout << "请输入要重置密码的顾客账号: ";
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
                    std::cout << "请输入要模糊查询的关键字: ";
                    std::cin >> keyword;
                    auto foundProducts = productManager.queryProductByFuzzyName(keyword);
                    if (foundProducts.empty()) {
                        std::cout << "未找到包含该关键字的商品。" << std::endl;
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
                    std::cout << "请输入要精确查询的商品名称: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "未找到该商品。" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "商品名称: " << product.name << " - " << product.price << " 元 - 库存: " << product.stock << std::endl;
                        }
                    }
                } else if (adminLoggedIn) {
                    std::string targetName;
                    std::cout << "请输入要精确查询的商品名称: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "未找到该商品。" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "商品名称: " << product.name << " - " << product.price << " 元 - 库存: " << product.stock << std::endl;
                        }
                    }
                } else if (customerLoggedIn) {
                    std::string productName;
                    int quantity;
                    std::cout << "请输入要添加到购物车的商品名称: ";
                    std::cin >> productName;
                    std::cout << "请输入要添加的商品数量: ";
                    std::cin >> quantity;
                    customer.addToCart(productName, quantity);
                }
                break;
            }
            case 5: {
                if (customerLoggedIn) {
                    std::string productName;
                    int quantity;
                    std::cout << "请输入要从购物车中删除的商品名称: ";
                    std::cin >> productName;
                    std::cout << "请输入要删除的商品数量: ";
                    std::cin >> quantity;
                    customer.removeFromCart(productName, quantity);
                } else if (!adminLoggedIn && !customerLoggedIn) {
                    std::string keyword;
                    std::cout << "请输入要模糊查询的关键字: ";
                    std::cin >> keyword;
                    auto foundProducts = productManager.queryProductByFuzzyName(keyword);
                    if (foundProducts.empty()) {
                        std::cout << "未找到包含该关键字的商品。" << std::endl;
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
                    std::cout << "请输入要修改购买数量的商品名称: ";
                    std::cin >> productName;
                    std::cout << "请输入新的购买数量: ";
                    std::cin >> newQuantity;
                    customer.modifyQuantity(productName, newQuantity);
                } else if (!adminLoggedIn && !customerLoggedIn) {
                    const auto& products = productManager.getProducts();
                    if (products.empty()) {
                        std::cout << "暂无商品信息。" << std::endl;
                    } else {
                        for (size_t i = 0; i < products.size(); ++i) {
                            std::cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " 元 - 库存: " << products[i].stock << std::endl;
                        }
                    }
                } else if (adminLoggedIn) {
                    std::string targetName;
                    std::cout << "请输入要删除的商品名称: ";
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
                    std::cout << "退出管理员登录" << std::endl;
                    adminLoggedIn = false;
                }
                break;
            }
            case 8: {
                if (customerLoggedIn) {
                    customer.purchase();
                } else if (adminLoggedIn) {
                    std::cout << "退出系统" << std::endl;
                    exit = 1;
                }
                break;
            }
            case 9: {
                if (customerLoggedIn) {
                    std::cout << "退出顾客登录" << std::endl;
                    customerLoggedIn = false;
                }
                break;
            }
            case 10: {
                if (customerLoggedIn) {
                    std::cout << "退出系统" << std::endl;
                    exit = 1;
                }
                break;
            }
            default:
                std::cout << "无效选择" << std::endl;
        }
    }
    return 0;
}