#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;
using namespace std::chrono;
// define Product class
class Product {
private:
    string name;
    string location;
    float price;
    chrono::system_clock::time_point dueDate;

public:
    Product(string name, string location, float price, chrono::system_clock::time_point dueDate)
            : name(name), location(location), price(price), dueDate(dueDate) {}

    string getName() { return name; }
    string getLocation() { return location; }
    float getPrice() { return price; }
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
    // open file for reading
    ifstream file("supermarket_data.txt");
    if (!file.is_open()) {
        cout << "Error: unable to open file" << endl;
        return 1;
    }

    // read each line from the file
    string line;
    while (getline(file, line)) {
        istringstream iss(line);

        // read product data from the line
        string name, location;
        float price;
        int dueDateDays;
        iss >> name >> location >> price >> dueDateDays;

        // calculate due date as current time plus number of days
        system_clock::time_point dueDate = system_clock::now() + (hours(24) * dueDateDays);

        // create product and add it to the supermarket
        Product product(name, location, price, dueDate);
        supermarket.Add(product, location);
    }

    supermarket.view();
    supermarket.viewContainerType();
    supermarket.viewAllInfo();


    return 0;

}