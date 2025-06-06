
#include "administrator.h"
#include "customer.h"
#include <fstream>
#include <sstream>
#include <vector>

Administrator::Administrator(const std::string& acc, const std::string& pwd) : account(acc), password(pwd) {}

bool Administrator::login(const std::string& inputAccount, const std::string& inputPassword) {
    return account == inputAccount && password == inputPassword;
}

bool Administrator::deleteCustomer(const std::string& targetAccount) {
    std::ifstream infile("customers.txt");
    std::vector<std::string> lines;
    std::string line;
    bool found = false;
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string acc, pwd, name;
        if (std::getline(ss, acc, ',') && std::getline(ss, pwd, ',') && std::getline(ss, name)) {
            if (acc != targetAccount) {
                lines.push_back(line);
            } else {
                found = true;
            }
        }
    }
    infile.close();

    std::ofstream outfile("customers.txt");
    if (outfile.is_open()) {
        for (const auto& l : lines) {
            outfile << l << std::endl;
        }
        outfile.close();
    } else {
        return false;
    }
    return found;
}

int Administrator::resetCustomerPassword(const std::string& targetAccount, const std::string& oldPassword, const std::string& newPassword) {
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
        for (const auto& l : lines) {
            outfile << l << std::endl;
        }
        outfile.close();
    } else {
        return -1; // 文件操作失败
    }
    return 0; // 修改成功
}    

bool Administrator::setOrderStatus(Order& order, OrderStatus newStatus) {
    if (order.status < newStatus) {
        order.updateStatus(newStatus);
        Customer::saveOrdersToFile();
        return true;
    }
    return false;
}