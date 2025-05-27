#include "products.h"
#include <fstream>
#include <sstream>
#include <iostream>

double Product::calculatePromotedPrice(const std::vector<Promotion*>& promotions) const {
    double price = this->price;
    for (auto promo : promotions) {
        if (auto discount = dynamic_cast<DiscountPromotion*>(promo)) {
            price *= discount->getDiscount();
        }
        // ��������������Ϊ�����ǻ��ڶ����ܼ۵�
    }
    return price;
}
ProductManager::ProductManager() {
    loadFromFile();
}

void ProductManager::addProduct(const std::string& name, double price, int stock) {
    products.emplace_back(name, price, stock);
    saveToFile();
}

const std::vector<Product>& ProductManager::getProducts() const {
    return products;
}

bool ProductManager::decreaseStock(const std::string& productName, int quantity) {
    for (auto& product : products) {
        if (product.name == productName) {
            if (product.stock >= quantity) {
                product.stock -= quantity;
                saveToFile();
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

void ProductManager::saveToFile() {
    std::ofstream outfile("products.txt");
    if (outfile.is_open()) {
        for (const auto& product : products) {
            outfile << product.name << "," << product.price << "," << product.stock << std::endl;
        }
        outfile.close();
    } else {
        std::cout << "�޷����ļ�������Ʒ��Ϣ" << std::endl;
    }
}

void ProductManager::loadFromFile() {
    std::ifstream infile("products.txt");
    if (infile.is_open()) {
        std::string line;
        while (std::getline(infile, line)) {
            std::stringstream ss(line);
            std::string name;
            double price;
            int stock;
            std::getline(ss, name, ',');
            ss >> price;
            ss.ignore();
            ss >> stock;
            products.emplace_back(name, price, stock);
        }
        infile.close();
    }
}
std::vector<Product> ProductManager::queryProductByName(const std::string& targetName)  {
    std::vector<Product> result;
    for (const auto& product : products) {
        if (product.name == targetName) {
            result.push_back(product);
        }
    }
    return result;
}
std::vector<Product> ProductManager::queryProductByFuzzyName(const std::string& keyword) {
    std::vector<Product> result;
    for (const auto& product : products) {
        if (product.name.find(keyword) != std::string::npos) {
            result.push_back(product);
        }
    }
    return result;
}
bool ProductManager::deleteProduct(const std::string& targetName) {
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->name == targetName) {
            products.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;

}
bool ProductManager::increaseStock(const std::string& productName, int quantity) {
    for (auto& product : products) {
        if (product.name == productName) {
            product.stock += quantity;
            saveToFile();
            return true;
        }
    }
    return false;
}
// ��������Ӵ����
void ProductManager::addPromotion(Promotion* promotion) {
    promotions.push_back(promotion);
}
// ��������ȡ��Ʒ�Ĵ����
std::vector<Promotion*> ProductManager::getPromotionsForProduct(const std::string& productName) const {
    std::vector<Promotion*> productPromotions;
    for (auto promotion : promotions) {
        if (promotion->getProductName() == productName
            //&& promotion->isActive()
            ) {
            productPromotions.push_back(promotion);
        }
    }
    return productPromotions;
}
const std::vector<Promotion*>& ProductManager::getPromotions() const {
    return promotions;
}
std::string ProductManager::getProductPromotionLabel(const std::string& productName) const {
    std::string label;
    auto promotions = getPromotionsForProduct(productName);

    for (auto promo : promotions) {
        if (auto discount = dynamic_cast<DiscountPromotion*>(promo)) {
            label += " [�ۿ�:" + std::to_string(static_cast<int>(discount->getDiscount() * 10)) + "��]";
        } else if (auto fullReduction = dynamic_cast<FullReductionPromotion*>(promo)) {
            label += " [��" + std::to_string(fullReduction->getFullAmount()) + "��" +
                     std::to_string(fullReduction->getReductionAmount()) + "]";
        }
    }

    return label;
}

void ProductManager::printProductInfo() {
    const auto& products = getProducts();
    std::cout << "��Ʒ�б�:" << std::endl;
    for (size_t i = 0; i < products.size(); ++i) {
        std::string label = getProductPromotionLabel(products[i].name);
        std::cout << i + 1 << ". " << products[i].name << " - ԭ��:" << products[i].price << " Ԫ - ���: " << products[i].stock << label << std::endl;
    }
}