#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// 1. Базовий абстрактний клас "Товар"
class Product {
protected:
    string name;
    double price;
    int quantity;

public:
    Product(string n, double p, int q) : name(n), price(p), quantity(q) {}
    virtual ~Product() {}

    // Чисто віртуальна функція (поліморфізм)
    virtual void displayInfo() const = 0; 

    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    // Метод видачі товару (зменшення залишку)
    bool dispense() {
        if (quantity > 0) {
            quantity--;
            return true;
        }
        return false;
    }
};

// 2. Клас-нащадок "Напій"
class Drink : public Product {
private:
    double volume; // Об'єм у літрах

public:
    Drink(string n, double p, int q, double v) : Product(n, p, q), volume(v) {}

    void displayInfo() const override {
        cout << left << setw(15) << name 
             << " (" << volume << " л) | Ціна: " << setw(5) << price 
             << " грн | Залишок: " << quantity << " шт." << endl;
    }
};

// 3. Клас-нащадок "Снек"
class Snack : public Product {
private:
    int weight; // Вага у грамах

public:
    Snack(string n, double p, int q, int w) : Product(n, p, q), weight(w) {}

    void displayInfo() const override {
        cout << left << setw(15) << name 
             << " (" << weight << " г)  | Ціна: " << setw(5) << price 
             << " грн | Залишок: " << quantity << " шт." << endl;
    }
};

// 4. Клас "Платіжна система"
class PaymentSystem {
private:
    double currentBalance;

public:
    PaymentSystem() : currentBalance(0.0) {}

    void insertMoney(double amount) {
        if (amount > 0) {
            currentBalance += amount;
            cout << "[Система] Внесено: " << amount << " грн. Поточний баланс: " << currentBalance << " грн.\n";
        }
    }

    double getBalance() const { return currentBalance; }

    bool processPayment(double amount) {
        if (currentBalance >= amount) {
            currentBalance -= amount;
            return true;
        }
        return false;
    }

    double returnChange() {
        double change = currentBalance;
        currentBalance = 0.0;
        return change;
    }
};

// 5. Головний клас "Торговий автомат"
class VendingMachine {
private:
    vector<Product*> inventory;
    PaymentSystem payment;

public:
    ~VendingMachine() {
        // Очищення пам'яті, щоб уникнути витоків
        for (auto p : inventory) {
            delete p;
        }
    }

    void addProduct(Product* p) {
        inventory.push_back(p);
    }

    void displayMenu() const {
        cout << "\n================ МЕНЮ АВТОМАТА ================\n";
        for (size_t i = 0; i < inventory.size(); ++i) {
            cout << "[" << i + 1 << "] ";
            inventory[i]->displayInfo();
        }
        cout << "===============================================\n";
    }

    void run() {
        int choice = -1;
        while (choice != 0) {
            displayMenu();
            cout << "\nВаш баланс: " << payment.getBalance() << " грн.\n";
            cout << "Оберіть дію:\n";
            cout << "1-9. Купити товар (введіть номер)\n";
            cout << "10. Внести кошти\n";
            cout << "0. Забрати решту та вийти\n";
            cout << "-> ";
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "[Помилка] Невірне введення. Спробуйте ще раз.\n";
                choice = -1;
                continue;
            }
        

            if (choice == 10) {
                double amount;
                cout << "Введіть суму поповнення (грн): ";
                cin >> amount;
                payment.insertMoney(amount);
            } 
            else if (choice > 0 && choice <= (int)inventory.size()) {
                int productIndex = choice - 1;
                Product* selectedProduct = inventory[productIndex];

                if (selectedProduct->getQuantity() == 0) {
                    cout << "[Помилка] Товар закінчився!\n";
                    continue;
                }

                if (payment.processPayment(selectedProduct->getPrice())) {
                    selectedProduct->dispense();
                    cout << "[Успіх] Ви придбали: " << selectedProduct->getName() << "!\n";
                } else {
                    cout << "[Помилка] Недостатньо коштів! Поповніть баланс.\n";
                }
            } 
            else if (choice == 0) {
                cout << "[Система] Видача решти: " << payment.returnChange() << " грн. Дякуємо за покупку!\n";
            } 
            else {
                cout << "[Помилка] Невідома команда.\n";
            }
        }
    }
};

int main() {
    // Налаштування кирилиці для консолі Windows (за потреби)
    system("chcp 65001 > nul");

    VendingMachine myMachine;

    // Наповнюємо автомат (поліморфізм у дії: кладемо нащадків у вектор базового класу)
    myMachine.addProduct(new Drink("Coca-Cola", 25.50, 5, 0.5));
    myMachine.addProduct(new Drink("Morshynska", 15.00, 10, 0.5));
    myMachine.addProduct(new Snack("Snickers", 30.00, 7, 50));
    myMachine.addProduct(new Snack("Lays Cheese", 45.00, 3, 120));

    // Запуск системи
    myMachine.run();

    return 0;
}