//
// Created by 86189 on 25-4-10.
//
// fileio.cpp
#include "fileio.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

void saveCustomerToFile(const std::string& account, const std::string& password, const std::string& name) {
    std::ofstream outfile("customers.txt", std::ios::app);
    if (outfile.is_open()) {
        outfile << account << "," << password << "," << name << std::endl;
        outfile.close();
    } else {
        std::cerr << "无法打开文件保存顾客信息" << std::endl;
    }
}

bool loadCustomerFromFile(const std::string& account, const std::string& password, std::string& name) {
    std::ifstream infile("customers.txt");
    std::string line;
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string acc, pwd, n;
        if (std::getline(ss, acc, ',') && std::getline(ss, pwd, ',') && std::getline(ss, n, ',')) {
            if (acc == account && pwd == password) {
                name = n;
                return true;
            }
        }
    }
    return false;
}

bool updateCustomerPassword(const std::string& targetAccount, const std::string& newPassword) {
    std::ifstream infile("customers.txt");
    std::vector<std::string> lines;
    std::string line;
    bool found = false;
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string acc, pwd, n, c;
        if (std::getline(ss, acc, ',') && std::getline(ss, pwd, ',') && std::getline(ss, n, ',') && std::getline(ss, c)) {
            if (acc == targetAccount) {
                line = acc + "," + newPassword + "," + n + "," + c;
                found = true;
            }
        }
        lines.push_back(line);
    }
    infile.close();

    std::ofstream outfile("customers.txt");
    if (outfile.is_open()) {
        for (const auto& l : lines) {
            outfile << l << std::endl;
        }
        outfile.close();
    } else {
        std::cerr << "无法打开文件更新顾客信息" << std::endl;
    }
    return found;
}