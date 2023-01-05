#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;

// define Product class
class Product {
private:
    std::string name;
    std::string location;
    float price;
    bool isPerishable;
    chrono::system_clock::time_point dueDate;

public:
    Product(std::string name, std::string location, float price, bool isPerishable, std::chrono::system_clock::time_point dueDate)
            : name(name), location(location), price(price), isPerishable(isPerishable), dueDate(dueDate) {}

    std::string getName() { return name; }
    std::string getLocation() { return location; }
    float getPrice() { return price; }
    bool getIsPerishable() { return isPerishable; }
    void setLocation(std::string newLocation) { location = newLocation; }
    chrono::system_clock::time_point getDueDate() { return dueDate; }
    void setDueDate(std::chrono::system_clock::time_point newDueDate) { dueDate = newDueDate; }
};

// define Shelf class
class Shelf {
private:
    std::string type;
    std::vector<Product> productsVec;
public:
    Shelf(std::string type) : type(type) {}
    std::string getType() { return type; }
    std::vector<Product> getProducts() { return productsVec; }
    void addProduct(Product product)
    {
        productsVec.push_back(product);
    }
    void removeProduct(Product product) {
        for (int i = 0; i < productsVec.size(); i++) {
            if (productsVec[i].getName() == product.getName()) {
                productsVec.erase(productsVec.begin() + i);
            }
        }
    }
};
// define Supermarket class
class Supermarket {
private:
    std::vector<Shelf> shelves;
public:
    std::vector<Shelf> getShelves() { return shelves; }
    void addShelf(Shelf shelf)
    {
        shelves.push_back(shelf);
    }
    void removeShelf(Shelf shelf) {
        for (int i = 0; i < shelves.size(); i++) {
            if (shelves[i].getType() == shelf.getType()) {
                shelves.erase(shelves.begin() + i);
            }
        }
    }
    void nextDay() {
        for (Shelf& shelf : shelves) {
            // get the products on this shelf
            std::vector<Product> products = shelf.getProducts();

            // remove any expired products from the shelf
            for (Product& product : products) {
                if (product.getDueDate() < std::chrono::system_clock::now()) {
                    shelf.removeProduct(product);
                }
            }
        }
    }
};
int main() {
    Supermarket supermarket;
    // create some shelves
    Shelf dairyShelf("Dairy");
    Shelf produceShelf("Produce");
    Shelf frozenShelf("Frozen");

    // create some products
    system_clock::time_point dueDate = system_clock::now() + hours(24);
    system_clock::time_point dueDateNow = system_clock::now();
    Product milk("Milk", "Dairy", 3.99, true, dueDate);
    Product bread("Bread", "Bakery", 2.99, true, dueDateNow);
    Product bananas("Bananas", "Produce", 0.99, false, dueDateNow);
    Product iceCream("Ice Cream", "Frozen", 4.99, true, dueDateNow);

    // add products to their respective shelves
    dairyShelf.addProduct(milk);
    produceShelf.addProduct(bananas);
    frozenShelf.addProduct(iceCream);
    // add shelves to the supermarket
    supermarket.addShelf(dairyShelf);
    supermarket.addShelf(produceShelf);
    supermarket.addShelf(frozenShelf);

    // iterate through all products in the supermarket
    std::vector<Shelf> shelves = supermarket.getShelves();
    for (Shelf shelf : shelves) {
        std::vector<Product> products = shelf.getProducts();
        for (Product product : products) {
            std::cout << "Product Name: " << product.getName() << std::endl;
            std::cout << "Location: " << product.getLocation() << std::endl;
            std::cout << "Price: $" << product.getPrice() << std::endl;
            std::cout << "Is Perishable: " << product.getIsPerishable() << std::endl;
            std::time_t dueDate = std::chrono::system_clock::to_time_t(product.getDueDate());
            std::tm tm;
            localtime_s(&tm, &dueDate);
            std::stringstream dueDateStream;
            dueDateStream << std::put_time(&tm, "%c %Z");
            std::cout << "Due Date: " << dueDateStream.str() << std::endl;
            std::cout << std::endl;
        }
    }

    // simulate the passage of time
    std::cout << "One day has passed..." << std::endl << endl;
    supermarket.nextDay();
    cout << "====================" << endl;

    // iterate through all products in the supermarket again
    shelves = supermarket.getShelves();
    for (Shelf shelf : shelves) {
        std::vector<Product> products = shelf.getProducts();
        for (Product product : products) {
            std::cout << "Product Name: " << product.getName() << std::endl;
            std::cout << "Location: " << product.getLocation() << std::endl;
            std::cout << "Price: $" << product.getPrice() << std::endl;
            std::cout << "Is Perishable: " << product.getIsPerishable() << std::endl;

            std::time_t dueDate = std::chrono::system_clock::to_time_t(product.getDueDate());
            std::tm tm;
            localtime_s(&tm, &dueDate);
            std::stringstream dueDateStream;
            dueDateStream << std::put_time(&tm, "%c %Z");
            std::cout << "Due Date: " << dueDateStream.str() << std::endl;

            std::cout << std::endl;
        }
    }

    return 0;
}