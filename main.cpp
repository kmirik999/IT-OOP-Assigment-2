#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;
// define Product class
class Product {
private:
    string name;
    string location;
    float price;
    bool isPerishable;
    chrono::system_clock::time_point dueDate;

public:
    Product(string name, string location, float price, bool isPerishable, chrono::system_clock::time_point dueDate)
            : name(name), location(location), price(price), isPerishable(isPerishable), dueDate(dueDate) {}

    string getName() { return name; }
    string getLocation() { return location; }
    float getPrice() { return price; }
    bool getIsPerishable() { return isPerishable; }
    void setLocation(string newLocation) { location = newLocation; }
    chrono::system_clock::time_point getDueDate() { return dueDate; }
    void setDueDate(chrono::system_clock::time_point newDueDate) { dueDate = newDueDate; }
};

// define Shelf class
class Shelf {
private:
    string type;
    vector<Product> productsVec;

public:
    Shelf(string type) : type(type) {}

    string getType() { return type; }
    vector<Product> getProducts() { return productsVec; }
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
    vector<Shelf> shelves;

public:
    vector<Shelf> getShelves() { return shelves; }
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
    void nextDay()
    {
        for (Shelf& shelf : shelves) {
            // get the products on this shelf
            vector<Product> products = shelf.getProducts();

            // remove any expired products from the shelf
            for (Product& product : products) {
                if (product.getDueDate() < chrono::system_clock::now()) {
                    shelf.removeProduct(product);
                }
            }
        }
    }

    void Take(Product product, string shelfType) {
        for (Shelf& shelf : shelves) {
            if (shelf.getType() == shelfType) {
                shelf.removeProduct(product);
                break;
            }
        }
    }

    void view()
    {
        bool exist = false;
        for (Shelf& shelf : shelves)
        {
            exist = true;
            bool empty = true;
            cout << "Container type: " << shelf.getType() << endl;
            vector<Product> products = shelf.getProducts();
            for (Product& product : products)
            {
                empty = false;
                cout << "Product name: " << product.getName() << endl;
            }

            if (empty) cout << "Container is empty!" << endl;
            cout << "---------------------" << endl;

        }

        if (!exist) cout << "Container isn`t exist!" << endl;
    }

    void viewContainerType() {
        for (Shelf& shelf : shelves) {
            cout << "Container type: " << shelf.getType() << endl;
        }
    }

    void viewContainerType(string name)
    {
        bool exist = false;
        for (Shelf& shelf : shelves)
        {
            if (shelf.getType() == name)
            {
                exist = true;
                bool empty = true;
                cout << "Container type: " << shelf.getType() << endl;
                vector<Product> products = shelf.getProducts();
                for (Product& product : products)
                {
                    empty = false;
                    cout << "Product name: " << product.getName() << endl;
                }

                if (empty) cout << "Container is empty!" << endl;
                cout << "---------------------" << endl;
            }

        }

        if (!exist) cout << "Container isn`t exist!" << endl;
    }

    void viewAllInfo()
    {
        for (Shelf shelf : shelves) {
            vector<Product> products = shelf.getProducts();
            for (Product product : products) {
                cout << "Product Name: " << product.getName() << endl;
                cout << "Location: " << product.getLocation() << endl;
                cout << "Price: $" << product.getPrice() << endl;
                cout << "Is Perishable: " << product.getIsPerishable() << endl;
                time_t dueDate = chrono::system_clock::to_time_t(product.getDueDate());
                tm tm;
                localtime_s(&tm, &dueDate);
                stringstream dueDateStream;
                dueDateStream << put_time(&tm, "%c %Z");
                cout << "Due Date: " << dueDateStream.str() << endl;
                cout << endl;
            }
        }
    }


    void Move(Product product, string fromShelfType, string toShelfType) {
        // first remove the product from the "from" shelf
        Take(product, fromShelfType);

        // then add the product to the "to" shelf
        for (Shelf& shelf : shelves) {
            if (shelf.getType() == toShelfType) {
                product.setLocation(toShelfType);
                shelf.addProduct(product);
                break;
            }
        }
    }

    void Add(Product product, string shelfType)
    {
        bool shelfExists = false;

        // check if the shelf exists
        for (Shelf& shelf : shelves) {
            if (shelf.getType() == shelfType) {
                shelfExists = true;
                break;
            }
        }

        // if the shelf doesn't exist, create it and add the product to it
        if (!shelfExists) {
            Shelf newShelf(shelfType);
            newShelf.addProduct(product);
            shelves.push_back(newShelf);
        }
            // if the shelf exists, just add the product to it
        else {
            for (Shelf& shelf : shelves) {
                if (shelf.getType() == shelfType) {
                    shelf.addProduct(product);
                    break;
                }
            }
        }
    }

};



int main() {
    Supermarket supermarket;

    system_clock::time_point dueDateMilk = system_clock::now() + (hours(24) * 2);
    Product milk("Milk", "Dairy", 3.99, true, dueDateMilk);
    supermarket.Add(milk, "Dairy");

    system_clock::time_point dueDateBread = system_clock::now() + hours(24);
    Product bread("Bread", "Bakery", 2.99, true, dueDateBread);
    supermarket.Add(bread, "Bakery");

    system_clock::time_point dueDateBlackBread = system_clock::now() + (hours(24) * 2);
    Product blackBread("BlackBread", "Bakery", 2.99, true, dueDateBlackBread);
    supermarket.Add(blackBread, "Bakery");
    //// create some shelves
    //Shelf dairyShelf("Dairy");
    //Shelf produceShelf("Produce");
    //Shelf frozenShelf("Frozen");
    //Shelf bakeryShelf("Bakery");

    //// create some products
    //system_clock::time_point dueDateMilk = system_clock::now() + (hours(24) * 2);
    //Product milk("Milk", "Dairy", 3.99, true, dueDateMilk);

    //system_clock::time_point dueDateBread = system_clock::now() + hours(24);
    //Product bread("Bread", "Bakery", 2.99, true, dueDateBread);

    //system_clock::time_point dueDateBlackBread = system_clock::now() + (hours(24) * 2);
    //Product blackBread("BlackBread", "Bakery", 2.99, true, dueDateBlackBread);

    //system_clock::time_point dueDateBananas = system_clock::now() - hours(24);
    //Product bananas("Bananas", "Produce", 0.99, false, dueDateBananas);

    //system_clock::time_point dueDateIceCream = system_clock::now() + (hours(24) * 2);
    //Product iceCream("Ice Cream", "Frozen", 4.99, true, dueDateIceCream);

    //// add products to their respective shelves
    //dairyShelf.addProduct(milk);
    //bakeryShelf.addProduct(bread);
    //bakeryShelf.addProduct(blackBread);
    //produceShelf.addProduct(bananas);
    //frozenShelf.addProduct(iceCream);
    //

    //// add shelves to the supermarket
    //supermarket.addShelf(dairyShelf);
    //supermarket.addShelf(bakeryShelf);
    //supermarket.addShelf(produceShelf);
    //supermarket.addShelf(frozenShelf);

    supermarket.viewContainerType();
    cout << "====================" << endl;

    // iterate through all products in the supermarket
    supermarket.viewAllInfo();



    // simulate the passage of time
    cout << "One day has passed..." << endl << endl;
    supermarket.nextDay();
    cout << "====================" << endl;

    // iterate through all products in the supermarket again
    supermarket.viewAllInfo();
    //cout << "remove one..." << endl << endl;
    //supermarket.Take(milk, "Dairy");



    supermarket.viewContainerType();

    cout << endl;
    supermarket.viewContainerType("Bakery");

    cout << endl;
    supermarket.view();


    cout << "Move..." << endl;
    //supermarket.Move(milk, "Dairy", "Bakery");
    supermarket.view();

    cout << "Add additional..." << endl;
    system_clock::time_point dueDate = system_clock::now() + hours(24);
    Product newProduct("New Product", "Bakery", 10.99, false, dueDate);
    supermarket.Add(newProduct, "Bakery");

    supermarket.view();
    return 0;
}