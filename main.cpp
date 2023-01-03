#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

// define Product class
class Product {
private:
    std::string name;
    std::string location;
    float price;
    bool isPerishable;
    time_t dueDate;

public:
    Product(std::string name, std::string location, float price, bool isPerishable, time_t dueDate)
            : name(name), location(location), price(price), isPerishable(isPerishable), dueDate(dueDate) {}

    std::string getName() { return name; }
    std::string getLocation() { return location; }
    float getPrice() { return price; }
    bool getIsPerishable() { return isPerishable; }
    void setLocation(std::string newLocation) { location = newLocation; }
    time_t getDueDate() { return dueDate; }
    void setDueDate(time_t newDueDate) { dueDate = newDueDate; }
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
        for (Shelf shelf : shelves) {
            std::vector<Product> products = shelf.getProducts();
            for (int i = 0; i < products.size(); i++) {
                if (products[i].getIsPerishable() && products[i].getDueDate() < time(0)) {
                    shelf.removeProduct(products[i]);
                    i--;
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
    Product milk("Milk", "Dairy", 3.99, true, time(0));
    Product bread("Bread", "Bakery", 2.99, true, time(0));
    Product bananas("Bananas", "Produce", 0.99, false, time(0));
    Product iceCream("Ice Cream", "Frozen", 4.99, true, time(0));

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
            time_t t = product.getDueDate();
            tm* now = localtime(&t);
            std::cout << "Current Date: " << now->tm_mday << '/' << (now->tm_mon + 1) << '/'
                      << (now->tm_year + 1900) << std::endl;
            //std::cout << "Due Date: " << ctime(product.getDueDate());
            //std::cout << "Due Date: " << product.getDueDate();
            std::cout << std::endl;
        }
    }
    return 0;
}
