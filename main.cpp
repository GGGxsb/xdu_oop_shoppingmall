#include <iostream>
#include <thread>
#include <ctime>
#include <chrono>
#include "administrator.h"
#include "customer.h"
#include "products.h"
#include "order.h"

// �Զ���鶩��״̬��ת���ĺ���
void autoUpdateOrderStatus(std::vector<Order>& orders, int seconds) {
    std::time_t now = std::time(nullptr);
    for (auto& order : orders) {
        if (order.status == PENDING_SHIPMENT && (now - order.purchaseTime) >= seconds) {
            order.status = SHIPPED;
            std::cout << "���� " << order.orderId << " ���Զ�ת��Ϊ�ѷ���״̬��" << std::endl;
        }
    }
}

int main() {

    Customer::loadOrdersFromFile();
    const auto& orders = Customer::getOrders();

    // Ԥ�������Ա�˺�����
    Administrator admin("admin", "admin123");

    ProductManager productManager;
    Customer customer(productManager);

    int choice;
    int exit = 0;
    bool customerLoggedIn = false;
    bool adminLoggedIn = false;

    // ����һ���߳��������Զ�״̬ת��
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
            std::cout << "1. ����Ա��¼\n"
                      << "2. �û�ע��\n"
                      << "3. �û���¼\n"
                      << "4. ��ȷ��ѯ��Ʒ��Ϣ\n"
                      << "5. ģ����ѯ��Ʒ��Ϣ\n"
                      << "6. �鿴��Ʒ�б�\n"
                      << "7. �˳�ϵͳ\n"
                      << "��ѡ��: " << std::endl;
        } else if (adminLoggedIn) {
            std::cout << "1. ��Ʒ�ϼ�\n"
                      << "2. ɾ���û��˺�\n"
                      << "3. �����û�����\n"
                      << "4. ��ȷ��ѯ��Ʒ��Ϣ\n"
                      << "5. ģ����ѯ��Ʒ��Ϣ\n"
                      << "6. ɾ����Ʒ��Ϣ\n"
                      << "7. �ֶ����ö���״̬\n"
                      << "8. �˳�����Ա��¼\n"
                      << "9. �˳�ϵͳ\n"
                      << "��ѡ��: ";
        } else if (customerLoggedIn) {
            std::cout << "1. �޸��û�����\n"
                      << "2. ��ȷ��ѯ��Ʒ��Ϣ\n"
                      << "3. ģ����ѯ��Ʒ��Ϣ\n"
                      << "4. �����Ʒ�����ﳵ\n"
                      << "5. ɾ�����ﳵ�е���Ʒ\n"
                      << "6. �޸Ĺ��ﳵ����Ʒ�Ĺ�������\n"
                      << "7. ��ѯ���ﳵ�е���Ʒ��Ϣ\n"
                      << "8. ���㹺�ﳵ�е���Ʒ\n"
                      << "9. �鿴��Ʒ�б�\n"
                      << "10. ����Ʒ�б�ѡ����Ʒ����\n"
                      << "11. **��ѯ����**\n"
                      << "12. **�޸Ķ�����ȡ������ĵ�ַ��**\n"
                      << "13. **ɾ�����ջ�״̬�Ķ���**\n"
                      << "14. �˳��û���¼\n"
                      << "15. �˳�ϵͳ\n"
                      << "��ѡ��: ";
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
                    std::cout << "��������Ҫ�޸�������û��˺�: ";
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
                            std::cout << "�˺Ų����ڣ������޸�ʧ��" << std::endl;
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
                    std::cout << "�û�ע��ɹ�" << std::endl;
                } else if (adminLoggedIn) {
                    std::string targetAccount;
                    std::cout << "��������Ҫɾ�����û��˺�: ";
                    std::cin >> targetAccount;
                    if (admin.deleteCustomer(targetAccount)) {
                        std::cout << "�û��˺�ɾ���ɹ�" << std::endl;
                    } else {
                        std::cout << "δ�ҵ����û��˺Ż�ɾ��ʧ��" << std::endl;
                    }
                } else if (customerLoggedIn) {
                    std::string targetName;
                    std::cout << "��������Ҫ��ȷ��ѯ����Ʒ����: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "δ�ҵ�����Ʒ" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "��Ʒ����: " << product.name << " - " << product.price << " Ԫ - ���: " << product.stock << std::endl;
                        }
                    }
                }
                break;
            }
            case 3: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::string inputAccount, inputPassword;
                    std::cout << "�������û��˺�: ";
                    std::cin >> inputAccount;
                    std::cout << "�������û�����: ";
                    std::cin >> inputPassword;
                    if (customer.login(inputAccount, inputPassword)) {
                        std::cout << "�û���¼�ɹ�" << std::endl;
                        customerLoggedIn = true;
                    } else {
                        std::cout << "�û���¼ʧ��" << std::endl;
                    }
                } else if (adminLoggedIn) {
                    std::string targetAccount, oldPassword, newPassword;
                    std::cout << "��������Ҫ����������û��˺�: ";
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
                            std::cout << "�˺Ų����ڣ���������ʧ��" << std::endl;
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
                    std::cout << "��������Ҫģ����ѯ�Ĺؼ���: ";
                    std::cin >> keyword;
                    auto foundProducts = productManager.queryProductByFuzzyName(keyword);
                    if (foundProducts.empty()) {
                        std::cout << "δ�ҵ������ùؼ��ֵ���Ʒ" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "��Ʒ����: " << product.name << " - " << product.price << " Ԫ - ���: " << product.stock << std::endl;
                        }
                    }
                }
                break;
            }
            case 4: {
                if (!adminLoggedIn && !customerLoggedIn) {
                    std::string targetName;
                    std::cout << "��������Ҫ��ȷ��ѯ����Ʒ����: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "δ�ҵ�����Ʒ" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "��Ʒ����: " << product.name << " - " << product.price << " Ԫ - ���: " << product.stock << std::endl;
                        }
                    }
                } else if (adminLoggedIn) {
                    std::string targetName;
                    std::cout << "��������Ҫ��ȷ��ѯ����Ʒ����: ";
                    std::cin >> targetName;
                    auto foundProducts = productManager.queryProductByName(targetName);
                    if (foundProducts.empty()) {
                        std::cout << "δ�ҵ�����Ʒ" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "��Ʒ����: " << product.name << " - " << product.price << " Ԫ - ���: " << product.stock << std::endl;
                        }
                    }
                } else if (customerLoggedIn) {
                    std::string productName;
                    int quantity;
                    std::cout << "��������Ҫ��ӵ����ﳵ����Ʒ����: ";
                    std::cin >> productName;
                    std::cout << "��������Ҫ��ӵ���Ʒ����: ";
                    std::cin >> quantity;
                    customer.addToCart(productName, quantity);
                }
                break;
            }
            case 5: {
                if (customerLoggedIn) {
                    std::string productName;
                    int quantity;
                    std::cout << "��������Ҫ�ӹ��ﳵ��ɾ������Ʒ����: ";
                    std::cin >> productName;
                    std::cout << "��������Ҫɾ������Ʒ����: ";
                    std::cin >> quantity;
                    customer.removeFromCart(productName, quantity);
                } else if (!adminLoggedIn && !customerLoggedIn) {
                    std::string keyword;
                    std::cout << "��������Ҫģ����ѯ�Ĺؼ���: ";
                    std::cin >> keyword;
                    auto foundProducts = productManager.queryProductByFuzzyName(keyword);
                    if (foundProducts.empty()) {
                        std::cout << "δ�ҵ������ùؼ��ֵ���Ʒ" << std::endl;
                    } else {
                        for (const auto& product : foundProducts) {
                            std::cout << "��Ʒ����: " << product.name << " - " << product.price << " Ԫ - ���: " << product.stock << std::endl;
                        }
                    }
                }
                break;
            }
            case 6: {
                if (customerLoggedIn) {
                    std::string productName;
                    int newQuantity;
                    std::cout << "��������Ҫ�޸Ĺ�����������Ʒ����: ";
                    std::cin >> productName;
                    std::cout << "�������µĹ�������: ";
                    std::cin >> newQuantity;
                    customer.modifyQuantity(productName, newQuantity);
                } else if (!adminLoggedIn && !customerLoggedIn) {
                    const auto& products = productManager.getProducts();
                    if (products.empty()) {
                        std::cout << "������Ʒ��Ϣ" << std::endl;
                    } else {
                        for (size_t i = 0; i < products.size(); ++i) {
                            std::cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " Ԫ - ���: " << products[i].stock << std::endl;
                        }
                    }
                } else if (adminLoggedIn) {
                    std::string targetName;
                    std::cout << "��������Ҫɾ������Ʒ����: ";
                    std::cin >> targetName;
                    if (productManager.deleteProduct(targetName)) {
                        std::cout << "��Ʒɾ���ɹ�" << std::endl;
                    } else {
                        std::cout << "δ�ҵ�����Ʒ��ɾ��ʧ��" << std::endl;
                    }
                }
                break;
            }
            case 7: {
                if (customerLoggedIn) {
                    customer.queryCartInfo();
                } else if (!adminLoggedIn && !customerLoggedIn) {
                    std::cout << "�˳�ϵͳ" << std::endl;
                    exit = 1;
                } else if (adminLoggedIn) {
                    std::string orderId;
                    int newStatus;
                    std::cout << "��������Ҫ����״̬�Ķ������: ";
                    std::cin >> orderId;
                    std::cout << "��ѡ���µĶ���״̬ (0: ������, 1: �ѷ���, 2: ��ǩ��): ";
                    std::cin >> newStatus;
                    if (newStatus < 0 || newStatus > 2) {
                        std::cout << "��Ч�Ķ���״̬" << std::endl;
                    } else {
                        for (auto& order : Customer::getOrders()) {
                            if (order.orderId == orderId) {
                                if (admin.setOrderStatus(order, static_cast<OrderStatus>(newStatus))) {
                                    std::cout << "���� " << orderId << " ״̬�Ѹ���Ϊ ";
                                    switch (newStatus) {
                                        case 0:
                                            std::cout << "������" << std::endl;
                                            break;
                                        case 1:
                                            std::cout << "�ѷ���" << std::endl;
                                            break;
                                        case 2:
                                            std::cout << "��ǩ��" << std::endl;
                                            break;
                                    }
                                    Customer::saveOrdersToFile();
                                } else {
                                    std::cout << "�޷�������״̬����Ϊָ��״̬" << std::endl;
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
                    std::cout << "�������ջ���ַ: ";
                    std::cin.ignore(); // ���������
                    std::getline(std::cin, addr);
                    customer.purchase(addr);
                    std::cout << "���������ɹ�" << std::endl;
                } else if (adminLoggedIn) {
                    std::cout << "�˳�����Ա��¼" << std::endl;
                    adminLoggedIn = false;
                }
                break;
            }
            case 9: {
                if (customerLoggedIn) {
                    const auto& products = productManager.getProducts();
                    if (products.empty()) {
                        std::cout << "������Ʒ��Ϣ" << std::endl;
                    } else {
                        for (size_t i = 0; i < products.size(); ++i) {
                            std::cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " Ԫ - ���: " << products[i].stock << std::endl;
                        }
                    }
                } else if (adminLoggedIn) {
                    // �˳�ϵͳǰ���涩��
                    Customer::saveOrdersToFile();
                    std::cout << "������Ϣ�ѱ��棬ϵͳ�˳�" << std::endl;
                    exit = 1;
                }
                break;
            }
            case 10: {
                std::string shippingAddress;
                if (customerLoggedIn) {
                    const auto& products = productManager.getProducts();
                    if (products.empty()) {
                        std::cout << "������Ʒ��Ϣ���޷����й���" << std::endl;
                    } else {
                        std::cout << "��Ʒ�б�����:" << std::endl;
                        for (size_t i = 0; i < products.size(); ++i) {
                            std::cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " Ԫ - ���: " << products[i].stock << std::endl;
                        }
                        int productIndex;
                        int quantity;
                        std::cout << "��������Ҫ�������Ʒ���: ";
                        std::cin >> productIndex;
                        if (productIndex > 0 && productIndex <= static_cast<int>(products.size())) {
                            std::cout << "��������Ҫ�������Ʒ����: ";
                            std::cin >> quantity;
                            const Product& selectedProduct = products[productIndex - 1];
                            if (selectedProduct.stock >= quantity) {
                                customer.addToCart(selectedProduct.name, quantity);
                                std::cout << "�������ջ���ַ: " << std::endl;
                                std::cin >> shippingAddress;
                                customer.purchase(shippingAddress);
                            } else {
                                std::cout << "��Ʒ��治�㣬�޷����й���" << std::endl;
                            }
                        } else {
                            std::cout << "��Ч����Ʒ���" << std::endl;
                        }
                    }
                }
                break;
            }
            case 11: {
                // **��ѯ����**
                if (customerLoggedIn) {
                    customer.queryOrders();
                }
                break;
            }
            case 12: {
                // **�޸Ķ�����ȡ������ĵ�ַ��**
                if (customerLoggedIn) {
                    std::string orderId, action, newAddress;
                    std::cout << "��������Ҫ�޸ĵĶ������: ";
                    std::cin >> orderId;
                    std::cout << "��ѡ����� (1: ȡ������, 2: �����ջ���ַ): ";
                    int op;
                    std::cin >> op;
                    if (op == 1) {
                        action = "cancel";
                        if (customer.modifyOrder(orderId, action)) {
                            std::cout << "����ȡ���ɹ�����Ʒ�ѷ���" << std::endl;
                        }
                    } else if (op == 2) {
                        action = "change_address";
                        std::cout << "�������µ��ջ���ַ: ";
                        std::cin.ignore(); // ���������
                        std::getline(std::cin, newAddress);
                        if (customer.modifyOrder(orderId, action, newAddress)) {
                            std::cout << "�ջ���ַ�޸ĳɹ�" << std::endl;
                        }
                    } else {
                        std::cout << "��Ч�Ĳ���" << std::endl;
                    }
                }
                break;
            }
            case 13: {
                // **ɾ�����ջ�״̬�Ķ���**
                if (customerLoggedIn) {
                    std::string orderId;
                    std::cout << "��������Ҫɾ���Ķ������: ";
                    std::cin >> orderId;
                    if (customer.deleteOrder(orderId)) {
                        std::cout << "����ɾ���ɹ�" << std::endl;
                    }
                }
                break;
            }
            case 14: {
                // �˳��û���¼
                if (customerLoggedIn) {
                    customerLoggedIn = false;
                    std::cout << "���˳��û���¼" << std::endl;
                }
                break;
            }
            case 15: {
                // �˳�ϵͳǰ���涩��
                if (customerLoggedIn || adminLoggedIn) {
                    Customer::saveOrdersToFile();
                    std::cout << "������Ϣ�ѱ��棬ϵͳ�˳�" << std::endl;
                }
                exit = 1;
                break;
            }
            default:
                std::cout << "��Чѡ������������" << std::endl;
        }
    }
    return 0;
}