#include <iostream>
#include "administrator.h"
#include "customer.h"
#include "products.h"
#include "shop.h"

int main() {
    // Ԥ�����Ա�˺�����
    Administrator admin("admin", "admin123");

    Customer customer;
    ProductManager productManager;
    ShoppingCart shoppingCart(productManager);

    int choice;
    int exit = 0;
    bool customerLoggedIn = false;
    bool adminLoggedIn = false;

    while (!exit) {
        if (!adminLoggedIn && !customerLoggedIn) {
            std::cout << "1. ����Ա��¼\n"
                         "2. �˿�ע��\n"
                         "3. �˿͵�¼\n"
                         "4. ��ȷ��ѯ��Ʒ��Ϣ\n"
                         "5. ģ����ѯ��Ʒ��Ϣ\n"
                         "6. �鿴��Ʒ�б�\n"
                         "7. �˳�ϵͳ\n"
                         "��ѡ��: ";
        } else if (adminLoggedIn) {
            std::cout << "1. ��Ʒ�ϼ�\n"
                         "2. ɾ���˿��˺�\n"
                         "3. ���ù˿�����\n"
                         "4. ��ȷ��ѯ��Ʒ��Ϣ\n"
                         "5. ģ����ѯ��Ʒ��Ϣ\n"
                         "6. ɾ����Ʒ��Ϣ\n"
                         "7. �˳�����Ա����\n"
                         "8. �˳�ϵͳ\n"
                         "��ѡ��: ";
        } else if (customerLoggedIn) {
            std::cout << "1. �޸Ĺ˿�����\n"
                         "2. ��ȷ��ѯ��Ʒ��Ϣ\n"
                         "3. ģ����ѯ��Ʒ��Ϣ\n"
                         "4. ����Ʒ���빺�ﳵ\n"
                         "5. �����ﳵ��Ʒ\n"
                         "6. �˳��˿͵�¼\n"
                         "7. �˳�ϵͳ\n"
                         "��ѡ��: ";
        }

        std::cin >> choice;
        switch (choice) {
            case 1: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::string inputAccount, inputPassword;
                    std::cout << "���������Ա�˺�: ";
                    std::cin >> inputAccount;
                    std::cout << "���������Ա����: ";
                    std::cin >> inputPassword;
                    if (admin.login(inputAccount, inputPassword)) {
                        std::cout << "����Ա��¼�ɹ�" << std::endl;
                        adminLoggedIn = true;
                    } else {
                        std::cout << "����Ա��¼ʧ��" << std::endl;
                    }
                } else if (adminLoggedIn) {
                    std::string name;
                    double price;
                    int stock;
                    std::cout << "��������Ʒ����: ";
                    std::cin >> name;
                    std::cout << "��������Ʒ�۸�: ";
                    std::cin >> price;
                    std::cout << "��������Ʒ���: ";
                    std::cin >> stock;
                    productManager.addProduct(name, price, stock);
                } else if (customerLoggedIn) {
                    std::string targetAccount, oldPassword, newPassword;
                    std::cout << "������Ҫ�޸�������˻�: ";
                    std::cin >> targetAccount;
                    std::cout << "�����������: ";
                    std::cin >> oldPassword;
                    std::cout << "������������: ";
                    std::cin >> newPassword;
                    int result = customer.changePassword(targetAccount, oldPassword, newPassword);
                    switch (result) {
                        case 0:
                            std::cout << "�����޸ĳɹ�" << std::endl;
                            break;
                        case 1:
                            std::cout << "�˻��������ڣ������޸�ʧ��" << std::endl;
                            break;
                        case 2:
                            std::cout << "��������������޸�ʧ��" << std::endl;
                            break;
                        case -1:
                            std::cout << "�ļ�����ʧ�ܣ������޸�ʧ��" << std::endl;
                            break;
                    }
                }
                break;
            }
            case 2: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::string acc, pwd, n;
                    std::cout << "������ע���˺�: ";
                    std::cin >> acc;
                    std::cout << "������ע������: ";
                    std::cin >> pwd;
                    std::cout << "����������: ";
                    std::cin >> n;
                    customer.registerCustomer(acc, pwd, n);
                    std::cout << "�˿�ע��ɹ�" << std::endl;
                } else if (adminLoggedIn) {
                    std::string targetAccount;
                    std::cout << "������Ҫɾ���Ĺ˿��˺�: ";
                    std::cin >> targetAccount;
                    if (admin.deleteCustomer(targetAccount)) {
                        std::cout << "�˿��˺�ɾ���ɹ�" << std::endl;
                    } else {
                        std::cout << "δ�ҵ��ù˿��˺Ż�ɾ��ʧ��" << std::endl;
                    }
                } else if (customerLoggedIn) {
                    std::string targetName;
                    std::cout << "������Ҫ��ȷ��ѯ����Ʒ����: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "δ�ҵ�����Ʒ��" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "��Ʒ����: " << product.name << " - " << product.price << " Ԫ�����: " << product.stock << std::endl;
                        }
                    }
                }
                break;
            }
            case 3: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::string inputAccount, inputPassword;
                    std::cout << "������˿��˺�: ";
                    std::cin >> inputAccount;
                    std::cout << "������˿�����: ";
                    std::cin >> inputPassword;
                    if (customer.login(inputAccount, inputPassword)) {
                        std::cout << "�˿͵�¼�ɹ�" << std::endl;
                        customerLoggedIn = true;
                    } else {
                        std::cout << "�˿͵�¼ʧ��" << std::endl;
                    }
                } else if (adminLoggedIn) {
                    std::string targetAccount, oldPassword, newPassword;
                    std::cout << "������Ҫ��������Ĺ˿��˺�: ";
                    std::cin >> targetAccount;
                    std::cout << "�����������: ";
                    std::cin >> oldPassword;
                    std::cout << "������������: ";
                    std::cin >> newPassword;
                    int result = admin.resetCustomerPassword(targetAccount, oldPassword, newPassword);
                    switch (result) {
                        case 0:
                            std::cout << "�������óɹ�" << std::endl;
                            break;
                        case 1:
                            std::cout << "�˻��������ڣ���������ʧ��" << std::endl;
                            break;
                        case 2:
                            std::cout << "�����������������ʧ��" << std::endl;
                            break;
                        case -1:
                            std::cout << "�ļ�����ʧ�ܣ���������ʧ��" << std::endl;
                            break;
                    }
                } else if (customerLoggedIn) {
                    std::string keyword;
                    std::cout << "������Ҫģ����ѯ�Ĺؼ���: ";
                    std::cin >> keyword;
                    auto foundProducts = productManager.queryProductByFuzzyName(keyword);
                    if (foundProducts.empty()) {
                        std::cout << "δ�ҵ������ùؼ��ֵ���Ʒ��" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "��Ʒ����: " << product.name << " - " << product.price << " Ԫ�����: " << product.stock << std::endl;
                        }
                    }
                }
                break;
            }
            case 4: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::string targetName;
                    std::cout << "������Ҫ��ȷ��ѯ����Ʒ����: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "δ�ҵ�����Ʒ��" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "��Ʒ����: " << product.name << " - " << product.price << " Ԫ�����: " << product.stock << std::endl;
                        }
                    }
                } else if (adminLoggedIn) {
                    std::string targetName;
                    std::cout << "������Ҫ��ȷ��ѯ����Ʒ����: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "δ�ҵ�����Ʒ��" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "��Ʒ����: " << product.name << " - " << product.price << " Ԫ�����: " << product.stock << std::endl;
                        }
                    }
                } else if (customerLoggedIn) {
                    const auto& products = productManager.getProducts();
                    if (products.empty()) {
                        std::cout << "������Ʒ�ɹ�ѡ��" << std::endl;
                    } else {
                        for (size_t i = 0; i < products.size(); ++i) {
                            std::cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " Ԫ�����: " << products[i].stock << std::endl;
                        }
                        int productIndex;
                        std::cout << "������Ҫ���빺�ﳵ����Ʒ���: ";
                        std::cin >> productIndex;
                        if (productIndex >= 1 && productIndex <= static_cast<int>(products.size())) {
                            shoppingCart.addToCart(products[productIndex - 1]);
                        } else {
                            std::cout << "��Ч����Ʒ��š�" << std::endl;
                        }
                    }
                }
                break;
            }
            case 5: {
                std::string keyword;
                std::cout << "������Ҫģ����ѯ�Ĺؼ���: ";
                std::cin >> keyword;
                auto foundProducts = productManager.queryProductByFuzzyName(keyword);
                if (foundProducts.empty()) {
                    std::cout << "δ�ҵ������ùؼ��ֵ���Ʒ��" << std::endl;
                } else {
                    for (const auto& product : foundProducts) {
                        std::cout << "��Ʒ����: " << product.name << " - " << product.price << " Ԫ�����: " << product.stock << std::endl;
                    }
                }
                break;
            }
            case 6: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    const auto& products = productManager.getProducts();
                    if (products.empty()) {
                        std::cout << "������Ʒ��Ϣ��" << std::endl;
                    } else {
                        for (size_t i = 0; i < products.size(); ++i) {
                            std::cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " Ԫ�����: " << products[i].stock << std::endl;
                        }
                    }
                } else if (adminLoggedIn) {
                    std::string targetName;
                    std::cout << "������Ҫɾ������Ʒ����: ";
                    std::cin >> targetName;
                    if (productManager.deleteProduct(targetName)) {
                        std::cout << "��Ʒɾ���ɹ�" << std::endl;
                    } else {
                        std::cout << "δ�ҵ�����Ʒ��ɾ��ʧ��" << std::endl;
                    }
                } else if (customerLoggedIn) {
                    std::cout << "�˳��˿͵�¼" << std::endl;
                    customerLoggedIn = false;
                }
                break;
            }
            case 7: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::cout << "���˳�ϵͳ" << std::endl;
                    exit = 1;
                } else if (adminLoggedIn) {
                    std::cout << "�˳�����Ա����" << std::endl;
                    adminLoggedIn = false;
                } else if (customerLoggedIn) {
                    std::cout << "���˳�ϵͳ" << std::endl;
                    exit = 1;
                }
                break;
            }
            case 8: {
                if (adminLoggedIn) {
                    std::cout << "���˳�ϵͳ" << std::endl;
                    exit = 1;
                }
                break;
            }
            default:
                std::cout << "��Чѡ��" << std::endl;
        }
    }
    return 0;
}
