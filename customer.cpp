//
// Created by 86189 on 25-4-10.
//
// customer.cpp
#include "customer.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

Customer::Customer() : isLoggedIn(false) {}

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
        for (const auto& l : lines) {
            outfile << l << std::endl;
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

void Customer::purchase() {
    if (isLoggedIn) {
        std::cout << "模拟购买成功！" << std::endl;
    } else {
        std::cout << "您尚未登录，请先登录再进行购买。" << std::endl;
    }
}