#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <random>

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
    Product(string name, string location) : name(name), location(location){}
    Product(string name, string location, float price, chrono::system_clock::time_point dueDate)
            : name(name), location(location), price(price), dueDate(dueDate) {}

    string getName() { return name; }
    string getLocation() { return location; }
    float getPrice() { return price; }
    void setLocation(string newLocation) { location = newLocation; }
    chrono::system_clock::time_point getDueDate() { return dueDate; }
    void setDueDate(chrono::system_clock::time_point newDueDate) { dueDate = newDueDate; }
};

class Factory {
private:
    vector<Product> stock;

public:
    Factory() {}

    vector<Product> getStock() {
        return stock;
    }

    void generate(int numProducts) {
        string name;
        string location;
        float price;
        int dueDateDays;
        vector<Product> listRandomProducts;

        // Generate random products and store them in the stock
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> disRandomProduct(0, 7);
        uniform_real_distribution<> disPrice(1, 1000);
        uniform_int_distribution<> disDay(-5, 10);

        ifstream fileRandomNames("random_names.txt");
        if (!fileRandomNames.is_open()) {
            cout << "Error: unable to open file" << endl;
            return;
        }

        string line;

        while (getline(fileRandomNames, line))
        {
            istringstream iss(line);

            iss >> name >> location;

            Product product = Product(name, location);
            listRandomProducts.push_back(product);
        }

        for (int i = 0; i < numProducts; i++)
        {
            int counter = 0;
            int id = disRandomProduct(gen);

            for (Product product : listRandomProducts)
            {
                if (counter != id)
                {
                    counter++;
                    continue;
                }

                name = product.getName();
                location = product.getLocation();
                price = disPrice(gen);

                dueDateDays = disDay(gen);
                system_clock::time_point dueDate = system_clock::now() + (hours(24) * dueDateDays);

                Product p = Product(name, location, price, dueDate);
                stock.push_back(p);
            }

        }
    }
};


class Storage {
public:
    virtual void addProduct(Product product) = 0;
    virtual void removeProduct(Product product) = 0;
};

// define Shelf class
class Shelf : public Storage {
private:
    string type;
    vector<Product> productsVec;

public:
    Shelf(string type) : type(type) {}

    string getType() { return type; }
    vector<Product> getProducts() { return productsVec; }
    void addProduct(Product product) override
    {
        productsVec.push_back(product);
    }
    void removeProduct(Product product) override {
        for (int i = 0; i < productsVec.size(); i++) {
            if (productsVec[i].getName() == product.getName()) {
                productsVec.erase(productsVec.begin() + i);
            }
        }
    }
};

class Refrigerator : public Storage {
private:
    float temperature = 5;
    vector<Product> productsVec;
public:
    vector<Product> getProducts() { return productsVec; }
    float getTemperature() { return this->temperature; }
    void addProduct(Product product) override
    {
        productsVec.push_back(product);
    }
    void removeProduct(Product product) override {
        for (int i = 0; i < productsVec.size(); i++) {
            if (productsVec[i].getName() == product.getName()) {
                productsVec.erase(productsVec.begin() + i);
            }
        }
    }

    void setTemperature(float temperature) {
        // set temperature only if it's within the allowed range for a refrigerator
        if (temperature >= 0 && temperature <= 10) {
            this->temperature = temperature;
            // set the temperature for this refrigerator
            cout << "Setting temperature for refrigerator: " << temperature << endl;
        }
    }


};

class Freezer : public Storage {
private:
    float temperature = -25;
    vector<Product> productsVec;
public:
    vector<Product> getProducts() { return productsVec; }
    float getTemperature() { return this->temperature; }
    void addProduct(Product product) override
    {
        productsVec.push_back(product);
    }
    void removeProduct(Product product) override {
        for (int i = 0; i < productsVec.size(); i++) {
            if (productsVec[i].getName() == product.getName()) {
                productsVec.erase(productsVec.begin() + i);
            }
        }
    }

    void setTemperature(float temperature) {
        // set temperature only if it's within the allowed range for a freezer
        if (temperature <= -20) {
            this->temperature = temperature;
            // set the temperature for this freezer
            cout << "Setting temperature for freezer: " << temperature << endl;
        }
    }
};

// define Supermarket class
class Supermarket {
private:
    vector<Shelf> shelves;

public:
    Refrigerator refrigerator;
    Freezer freezer;

    void createFreezer()
    {
        refrigerator = Refrigerator();
    }

    void createRefrigerator()
    {
        freezer = Freezer();
    }

    void setTemperatureFreezer(float temperature)
    {
        refrigerator.setTemperature(temperature);
    }

    void setTemperatureRefrigerator(float temperature)
    {
        freezer.setTemperature(temperature);
    }

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
        vector<Product> productsFreezer = freezer.getProducts();
        for (Product& product : productsFreezer) {
            if (product.getDueDate() < chrono::system_clock::now()) {
                freezer.removeProduct(product);
            }
        }

        vector<Product> productsRefrigerator = refrigerator.getProducts();
        for (Product& product : productsRefrigerator) {
            if (product.getDueDate() < chrono::system_clock::now()) {
                refrigerator.removeProduct(product);
            }
        }

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

    void Take(string productName, string shelfType) {



        if (shelfType == "Freezer")
        {
            vector<Product> productsFreezer = freezer.getProducts();
            for (Product product : productsFreezer)
            {
                if (product.getName() == productName)
                {
                    freezer.removeProduct(product);
                    return;
                }
            }
        }

        if (shelfType == "Refrigerator")
        {
            vector<Product> productsRefrigerator = refrigerator.getProducts();
            for (Product product : productsRefrigerator)
            {
                if (product.getName() == productName)
                {
                    refrigerator.removeProduct(product);
                    return;
                }
            }
        }

        for (Shelf& shelf : shelves)
        {
            if (shelf.getType() == shelfType) {
                vector<Product> products = shelf.getProducts();
                for (Product product : products)
                {
                    if (product.getName() == productName)
                    {
                        shelf.removeProduct(product);
                        return;
                    }
                }

            }
        }
    }

    void Move(string productName, string fromShelfType, string toShelfType) {

        if (fromShelfType == "Freezer")
        {
            vector<Product> productsFreezer = freezer.getProducts();
            for (Product product : productsFreezer)
            {
                if (product.getName() == productName)
                {
                    product.setLocation(toShelfType);

                    if (toShelfType == "Refrigerator")
                    {
                        refrigerator.addProduct(product);
                    }
                    else
                    {
                        for (Shelf& shelfTo : shelves)
                        {
                            if (shelfTo.getType() == toShelfType)
                            {
                                shelfTo.addProduct(product);
                                break;
                            }
                        }
                    }
                }
            }
            Take(productName, fromShelfType);
        }

        if (fromShelfType == "Refrigerator")
        {
            vector<Product> productsRefrigerator = refrigerator.getProducts();
            for (Product product : productsRefrigerator)
            {
                if (product.getName() == productName)
                {
                    product.setLocation(toShelfType);

                    if (toShelfType == "Freezer")
                    {
                        freezer.addProduct(product);
                    }
                    else
                    {
                        for (Shelf& shelfTo : shelves)
                        {
                            if (shelfTo.getType() == toShelfType)
                            {
                                shelfTo.addProduct(product);
                                break;
                            }
                        }
                    }
                }
            }
            Take(productName, fromShelfType);
        }

        // then add the product to the "to" shelf
        for (Shelf& shelf : shelves)
        {
            if (shelf.getType() == fromShelfType)
            {
                vector<Product> products = shelf.getProducts();
                for (Product product : products)
                {
                    if (product.getName() == productName)
                    {
                        product.setLocation(toShelfType);

                        if (toShelfType == "Freezer")
                        {
                            freezer.addProduct(product);
                        }
                        else if (toShelfType == "Refrigerator")
                        {
                            refrigerator.addProduct(product);
                        }
                        else
                        {
                            for (Shelf& shelfTo : shelves)
                            {
                                if (shelfTo.getType() == toShelfType)
                                {
                                    shelfTo.addProduct(product);
                                    break;
                                }
                            }
                        }

                    }
                }
                Take(productName, fromShelfType);
            }
        }
    }

    void Add(Product product, string shelfType)
    {
        bool shelfExists = false;

        if (shelfType == "Freezer")
        {
            freezer.addProduct(product);
            return;
        }
        else if (shelfType == "Refrigerator")
        {
            refrigerator.addProduct(product);
            return;
        }

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

    void view()
    {
        bool exist = false;
        bool empty = true;

        cout << "Container type: Freezer" << endl;
        vector<Product> productsFreezer = freezer.getProducts();
        for (Product& product : productsFreezer)
        {
            empty = false;
            cout << "Product name: " << product.getName() << endl;
        }
        cout << "---------------------" << endl;
        if (empty) cout << "Freezer is empty!" << endl;

        empty = true;

        cout << "Container type: Refrigerator" << endl;
        vector<Product> productsRefrigerator = refrigerator.getProducts();
        for (Product& product : productsRefrigerator)
        {
            empty = false;
            cout << "Product name: " << product.getName() << endl;
        }
        cout << "---------------------" << endl;
        if (empty) cout << "Refrigerator is empty!" << endl;

        empty = true;

        for (Shelf& shelf : shelves)
        {
            exist = true;
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

    void viewContainerType() {
        cout << "Container type: Freezer" << endl;
        cout << "Container type: Refrigerator" << endl;

        for (Shelf& shelf : shelves) {
            cout << "Container type: " << shelf.getType() << endl;
        }
    }

    void viewContainerType(string name)
    {

        if (name == "Freezer")
        {
            bool empty = true;
            cout << "Container type: Freezer" << endl;
            vector<Product> productsFreezer = freezer.getProducts();
            for (Product& product : productsFreezer)
            {
                empty = false;
                cout << "Product name: " << product.getName() << endl;
            }

            if (empty) cout << "Container is empty!" << endl;
            cout << "---------------------" << endl;
            return;
        }

        if (name == "Refrigerator")
        {
            bool empty = true;
            cout << "Container type: Refrigerator" << endl;
            vector<Product> productsRefrigerator = refrigerator.getProducts();
            for (Product& product : productsRefrigerator)
            {
                empty = false;
                cout << "Product name: " << product.getName() << endl;
            }

            if (empty) cout << "Container is empty!" << endl;
            cout << "---------------------" << endl;
            return;
        }

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
        vector<Product> productsFreezer = freezer.getProducts();
        for (Product product : productsFreezer) {
            cout << "Product Name: " << product.getName() << endl;
            cout << "Location: " << product.getLocation() << endl;
            cout << "Price: $" << product.getPrice() << endl;
            time_t dueDate = chrono::system_clock::to_time_t(product.getDueDate());
            tm tm;
            localtime_r(&dueDate, &tm);
            stringstream dueDateStream;
            dueDateStream << put_time(&tm, "%c %Z");
            cout << "Due Date: " << dueDateStream.str() << endl;
            cout << endl;
        }

        vector<Product> productsRefrigerator = refrigerator.getProducts();
        for (Product product : productsRefrigerator) {
            cout << "Product Name: " << product.getName() << endl;
            cout << "Location: " << product.getLocation() << endl;
            cout << "Price: $" << product.getPrice() << endl;
            time_t dueDate = chrono::system_clock::to_time_t(product.getDueDate());
            tm tm;
            localtime_r(&dueDate, &tm);
            stringstream dueDateStream;
            dueDateStream << put_time(&tm, "%c %Z");
            cout << "Due Date: " << dueDateStream.str() << endl;
            cout << endl;
        }

        for (Shelf shelf : shelves)
        {
            vector<Product> products = shelf.getProducts();
            for (Product product : products) {
                cout << "Product Name: " << product.getName() << endl;
                cout << "Location: " << product.getLocation() << endl;
                cout << "Price: $" << product.getPrice() << endl;
                time_t dueDate = chrono::system_clock::to_time_t(product.getDueDate());
                tm tm;
                localtime_r(&dueDate, &tm);
                stringstream dueDateStream;
                dueDateStream << put_time(&tm, "%c %Z");
                cout << "Due Date: " << dueDateStream.str() << endl;
                cout << endl;
            }
        }
    }

};



int main() {
    Supermarket supermarket;
    supermarket.createFreezer();
    supermarket.createRefrigerator();
    // open file for reading
    ifstream file("supermarket_data.txt");
    if (!file.is_open()) {
        cout << "Error: unable to open file" << endl;
        return 1;
    }

    // read each line from the file
    string line;

    // read product data from the line
    string name, location;
    float price;
    int dueDateDays;

    while (getline(file, line)) {
        istringstream iss(line);

        iss >> name >> location >> price >> dueDateDays;

        // calculate due date as current time plus number of days
        system_clock::time_point dueDate = system_clock::now() + (hours(24) * dueDateDays);

        // create product and add it to the supermarket
        Product product(name, location, price, dueDate);
        supermarket.Add(product, location);
    }

    //supermarket.view();

    string command;
    cout << "Enter a command or type 'help' for a list of commands" << endl;
    while (true) {
        cout << "> ";
        cin >> command;

        if (command == "help") {
            cout << "Commands:" << endl;
            cout << "  view " << endl;
            cout << "  viewAllInfo " << endl;
            cout << "  add " << endl;
            cout << "  take " << endl;
            cout << "  move " << endl;
            cout << "  next " << endl;
            cout << "  exit - exit the program" << endl;
        }
        else if (command == "view") {
            supermarket.view();
        }
        else if (command == "viewAllInfo") {
            supermarket.viewAllInfo();
        }
        else if (command == "add") {
            cout << "Selecet the name: ";
            cin >> name;

            cout << "Select the location(Shelf, Freezer, Refrigerator): ";
            cin >> location;

            cout << "Select the price: ";
            cin >> price;

            cout << "Select the shelf life: ";
            cin >> dueDateDays;

            system_clock::time_point dueDate = system_clock::now() + (hours(24) * dueDateDays);
            Product product(name, location, price, dueDate);

            supermarket.Add(product, location);
        }
        else if (command == "take") {
            supermarket.viewAllInfo();

            string name, locationName;

            cout << "Select name of product which to remove from shelve or freezer or refrigerator: ";
            cin >> name;

            cout << "Select the shelve or freezer or refrigerator of product which need to remove: ";
            cin >> locationName;

            if (locationName == "Freezer")
            {
                vector<Product> productsFreezer = supermarket.freezer.getProducts();

                for (Product product : productsFreezer)
                {
                    if (product.getName() == name)
                    {
                        supermarket.Take(product.getName(), product.getLocation());
                    }
                }
            }
            else if (locationName == "Refrigerator")
            {
                vector<Product> productsRefrigerator = supermarket.refrigerator.getProducts();

                for (Product product : productsRefrigerator)
                {
                    if (product.getName() == name)
                    {
                        supermarket.Take(product.getName(), product.getLocation());
                    }
                }
            }
            else
            {
                for (Shelf shelf : supermarket.getShelves())
                {
                    if (shelf.getType() == locationName)
                    {
                        vector<Product> products = shelf.getProducts();

                        for (Product product : products)
                        {
                            if (product.getName() == name)
                            {
                                supermarket.Take(product.getName(), product.getLocation());
                            }
                        }
                    }
                }
            }

        }
        else if (command == "move") {

            supermarket.viewAllInfo();

            string name, locationName, toLocationName;

            cout << "Select NAME of product which to remove from shelve or freezer or refrigerator: ";
            cin >> name;

            cout << "Select the shelve or freezer or refrigerator of product which need to move FROM: ";
            cin >> locationName;

            cout << "Select the shelve or freezer or refrigerator of product which need to move TO: ";
            cin >> toLocationName;


            if (locationName == "Freezer")
            {
                vector<Product> productsFreezer = supermarket.freezer.getProducts();

                for (Product product : productsFreezer)
                {
                    if (product.getName() == name)
                    {
                        supermarket.Move(product.getName(), product.getLocation(), toLocationName);
                    }
                }
            }
            else if (locationName == "Refrigetor")
            {
                vector<Product> productsRefrigetor = supermarket.refrigerator.getProducts();

                for (Product product : productsRefrigetor)
                {
                    if (product.getName() == name)
                    {
                        supermarket.Move(product.getName(), product.getLocation(), toLocationName);
                    }
                }
            }
            else
            {
                for (Shelf shelf : supermarket.getShelves())
                {
                    if (shelf.getType() == locationName)
                    {
                        vector<Product> products = shelf.getProducts();

                        for (Product product : products)
                        {
                            if (product.getName() == name)
                            {
                                supermarket.Move(product.getName(), product.getLocation(), toLocationName);
                            }
                        }
                    }
                }
            }

        }
        else if (command == "next") {
            supermarket.nextDay();
        }
        else if (command == "exit") {
            break;
        }
        else {
            cout << "Error: unrecognized command" << endl;
        }
    }

    return 0;
}
