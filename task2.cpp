#include <iostream>
#include <string>
#include <exception>
#include <fstream>

using namespace std;

/* =====================================================
   EXCEPTION CLASSES
   ===================================================== */

class InsufficientFundsException : public exception {
public:
    const char* what() const noexcept override {
        return "Error: Insufficient funds.";
    }
};

class InvalidReturnException : public exception {
public:
    const char* what() const noexcept override {
        return "Error: Invalid return date.";
    }
};

class ResourceNotAvailableException : public exception {
public:
    const char* what() const noexcept override {
        return "Error: Resource not available.";
    }
};

class BorrowLimitException : public exception {
public:
    const char* what() const noexcept override {
        return "Error: Borrow limit reached. Only 2 resources allowed.";
    }
};

class InvalidResourceOperationException : public exception {
public:
    const char* what() const noexcept override {
        return "Error: Invalid resource operation.";
    }
};

class InvalidReportException : public exception {
public:
    const char* what() const noexcept override {
        return "Error: Invalid report.";
    }
};


/* =====================================================
   RESOURCE BASE CLASS
   ===================================================== */

class Resource {
protected:
    int resourceID;
    string title;
    string category;
    bool isAvailable;
    int totalCopies;
    int availableCopies;
    int borrowCount;

public:

    Resource(int id, string t, string c, int copies)
        : resourceID(id),
          title(t),
          category(c),
          totalCopies(copies),
          availableCopies(copies),
          isAvailable(true),
          borrowCount(0) {}

    virtual ~Resource() {}

    virtual void displayInfo() const = 0;
    virtual void borrowResource() = 0;
    virtual void returnResource() = 0;

    bool checkAvailability() const {
        return availableCopies > 0;
    }

    int getID() const {
        return resourceID;
    }

    string getTitle() const {
        return title;
    }

    string getCategory() const {
        return category;
    }

    int getAvailableCopies() const {
        return availableCopies;
    }

    int getTotalCopies() const {
        return totalCopies;
    }

    virtual string getAuthor() const {
        return "";
    }

    virtual string getType() const = 0;

    virtual int getIssueNumber() const {
        return 0;
    }

    virtual string getISBN() const {
        return "";
    } 
    
};


/* =====================================================
   BOOK CLASS
   ===================================================== */

class Book : public Resource {
private:
    string author;
    string isbn;

public:

    Book(int id,
         string t,
         string c,
         string a,
         string i,
         int copies)

         : Resource(id, t, c, copies),
           author(a),
           isbn(i) {}

    void displayInfo() const override {

        cout << "\n========== BOOK ==========" << endl;
        cout << "ID: " << resourceID << endl;
        cout << "Title: " << title << endl;
        cout << "Category: " << category << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << isbn << endl;

        cout << "Available Copies: "
             << availableCopies
             << "/"
             << totalCopies
             << endl;

        cout << "Times Borrowed: "
             << borrowCount
             << endl;     

        if (availableCopies == 0)
            cout << "Status: Not Available" << endl;
        else
            cout << "Status: Available" << endl;
    }

    void borrowResource() override {

        if (!checkAvailability()) {
            throw ResourceNotAvailableException();
        }

        availableCopies--;
        borrowCount++;

        if (availableCopies == 0) {
            isAvailable = false;
        }
    }

    void returnResource() override {

        if (availableCopies >= totalCopies) {
            throw InvalidResourceOperationException();
        }

        availableCopies++;

        if (availableCopies > 0) {
            isAvailable = true;
        }
    }

    string getAuthor() const {
        return author;
    }

    string getType() const override {
        return "BOOK";
    }

    string getISBN() const override {
        return isbn;
    }

};

/* =====================================================
   MAGAZINE CLASS
   ===================================================== */

class Magazine : public Resource {
private:
    int issueNumber;

public:

    Magazine(int id,
             string t,
             string c,
             int issue,
             int copies)

             : Resource(id, t, c, copies),
               issueNumber(issue) {}

    void displayInfo() const override {

        cout << "\n========== MAGAZINE ==========" << endl;
        cout << "ID: " << resourceID << endl;
        cout << "Title: " << title << endl;
        cout << "Category: " << category << endl;
        cout << "Issue Number: " << issueNumber << endl;

        cout << "Available Copies: "
             << availableCopies
             << "/"
             << totalCopies
             << endl;

        cout << "Times Borrowed: "
             << borrowCount
             << endl;

        if (availableCopies == 0)
            cout << "Status: Not Available" << endl;
        else
            cout << "Status: Available" << endl;
    }

    void borrowResource() override {

        if (!checkAvailability()) {
            throw ResourceNotAvailableException();
        }

        availableCopies--;
        borrowCount++;

        if (availableCopies == 0) {
            isAvailable = false;
        }
    }

    void returnResource() override {

        if (availableCopies >= totalCopies) {
            throw InvalidResourceOperationException();
        }

        availableCopies++;

        if (availableCopies > 0) {
            isAvailable = true;
        }
    }

    string getType() const override {
        return "MAGAZINE";
    }

    int getIssueNumber() const override {
        return issueNumber;
    }

};


/* =====================================================
   PAYMENT CLASS
   ===================================================== */

class Payment {
public:

    static void deductBalance(double &balance,
                              double amount) {

        if (balance < amount) {
            throw InsufficientFundsException();
        }

        balance -= amount;
    }
};


/* =====================================================
   NOTIFICATION CLASS
   ===================================================== */

class Notification {
public:

    static void sendNotification(string msg) {

        cout << "Notification: "
             << msg
             << endl;
    }
};


/* =====================================================
   BORROW TRANSACTION CLASS
   ===================================================== */

class BorrowTransaction {
private:

    int transactionID;
    int issueDate;
    int dueDate;
    int returnDate;

    string resourceTitle;

    double fine;

public:

    BorrowTransaction(int tID = 0,
                      int iDate = 0,
                      int dDate = 0,
                      string title = "")

                      : transactionID(tID),
                        issueDate(iDate),
                        dueDate(dDate),
                        returnDate(0),
                        resourceTitle(title),
                        fine(0) {}

    void returnTransaction(int rDate,
                           double &balance) {

        if (rDate < issueDate) {
            throw InvalidReturnException();
        }

        returnDate = rDate;

        if (returnDate > dueDate) {

            int lateDays =
                returnDate - dueDate;

            fine = lateDays * 10;

            Payment::deductBalance(balance,
                                   fine);

            Notification::sendNotification(
                "Late return fine applied.");
        }

        else {

            Notification::sendNotification(
                "Resource returned on time.");
        }
    }

    void display() const {

        cout << "\n----- TRANSACTION -----"
             << endl;

        cout << "Transaction ID: "
             << transactionID
             << endl;

        cout << "Resource: "
             << resourceTitle
             << endl;

        cout << "Issue Date: "
             << issueDate
             << endl;

        cout << "Due Date: "
             << dueDate
             << endl;

        cout << "Return Date: "
             << returnDate
             << endl;

        cout << "Fine: "
             << fine
             << endl;
    }

    int getDueDate() const {
    return dueDate;
    }

    int getReturnDate() const {
    return returnDate;
    }

    string getResourceTitle() const {
    return resourceTitle;
    }

};


/* =====================================================
   ABSTRACT USER CLASS
   ===================================================== */

class User {
protected:

    int userID;

    string username;
    string password;

    string firstName;
    string lastName;
    string address;

    double balance;

public:

    User(int id = 0,
         string u = "",
         string p = "",
         string f = "",
         string l = "",
         string a = "",
         double b = 0)

         : userID(id),
           username(u),
           password(p),
           firstName(f),
           lastName(l),
           address(a),
           balance(b) {}

    virtual ~User() {}

    bool login(string u, string p) {

        return (u == username &&
                p == password);
    }

    virtual void searchResource() = 0;

    double getBalance() const {
        return balance;
    }

    void addBalance(double amount) {
        balance += amount;
    }
};


/* =====================================================
   CUSTOMER CLASS
   ===================================================== */

class Customer : public User {
private:

    Resource* borrowedResources[2];
    int borrowedCount;

    BorrowTransaction history[20];
    int historyCount;

    int dailyBorrowCount;
    int lastBorrowDay;

public:

    Customer(int id = 0,
         string u = "",
         string p = "",
         string f = "",
         string l = "",
         string a = "",
         double b = 1000)

         : User(id, u, p, f, l, a, b) {

        borrowedCount = 0;
        historyCount = 0;

        dailyBorrowCount = 0;
        lastBorrowDay = -1;
    }    

    string getUsername() const {
        return username;
    }

    string getPassword() const {
        return password;
    }

    string getFirstName() const {
        return firstName;
    }

    string getLastName() const {
        return lastName;
    }

    string getAddress() const {
        return address;
    }

    int getHistoryCount() const {
    return historyCount;
    }

    BorrowTransaction getTransaction(int index) const {
    return history[index];
    }


    void registerAccount() {

        cout << "\n========== REGISTER ACCOUNT =========="
             << endl;

        cout << "Enter Username: ";
        cin >> username;

        cout << "Enter Password: ";
        cin >> password;

        cout << "Enter First Name: ";
        cin >> firstName;

        cout << "Enter Last Name: ";
        cin >> lastName;

        cout << "Enter Address: ";
        cin >> address;

        balance = 1000;

        cout << "\nAccount Created Successfully."
             << endl;
    }

    void borrowResource(Resource* r) {

        try {
            int issueDate;

            cout << "Enter Issue Date (Day Number): ";
            cin >> issueDate;

            if (issueDate != lastBorrowDay) {
                dailyBorrowCount = 0;
                lastBorrowDay = issueDate;
            }

            if (dailyBorrowCount >= 2) {
                throw BorrowLimitException();
            }

            if (borrowedCount >= 2) {
                cout << "Maximum active borrow limit reached." << endl;
                return;
            }

            r->borrowResource();

            borrowedResources[borrowedCount++] = r;

            int dueDate = issueDate + 7;

            history[historyCount++] =
                BorrowTransaction(historyCount,
                              issueDate,
                              dueDate,
                              r->getTitle());

            dailyBorrowCount++;

            cout << "Borrow Successful." << endl;
        }
        catch (exception &e) {
            cout << e.what() << endl;
        }
    }

    void returnBorrowedResource(Resource* r) {

        try {

            bool found = false;

            for (int i = 0;
                 i < borrowedCount;
                 i++) {

                if (borrowedResources[i] == r) {

                    r->returnResource();

                    int returnDate;

                    cout << "Enter Return Date (Day Number): ";
                    cin >> returnDate;

                    history[i].returnTransaction(
                        returnDate,
                        balance);

                    for (int j = i;
                         j < borrowedCount - 1;
                         j++) {

                        borrowedResources[j] =
                            borrowedResources[j + 1];
                    }

                    borrowedCount--;

                    cout << "Return Successful."
                         << endl;

                    found = true;

                    break;
                }
            }

            if (!found) {

                cout << "This resource was not borrowed."
                     << endl;
            }
        }

        catch (exception &e) {
            cout << e.what() << endl;
        }
    }

    void viewBorrowHistory() {

        if (historyCount == 0) {

            cout << "No Borrowing History."
                 << endl;

            return;
        }

        for (int i = 0;
             i < historyCount;
             i++) {

            history[i].display();
        }
    }

    void searchResource() override {

        cout << "Customer Searching..."
             << endl;
    }

    void displayCustomerInfo() {

    cout << "\n========== CUSTOMER ==========" << endl;

    cout << "Username: " << username << endl;
    cout << "Name: " << firstName << " " << lastName << endl;
    cout << "Address: " << address << endl;
    cout << "Balance: " << balance << endl;

    cout << "Currently Borrowed Resources: " << borrowedCount << endl;

    for (int i = 0; i < borrowedCount; i++) {

        cout << "- "
             << borrowedResources[i]->getTitle()
             << endl;
    }

    cout << "\nBorrow History:" << endl;

    if (historyCount == 0) {
        cout << "No Borrow History." << endl;
    }

    for (int i = 0; i < historyCount; i++) {
        history[i].display();
    }
    }

};


/* =====================================================
   ADMIN CLASS
   ===================================================== */

class Admin : public User {
public:

    Admin(int id,
          string u,
          string p)

          : User(id,
                 u,
                 p,
                 "Admin",
                 "User",
                 "Library",
                 0) {}

    void addResource(Resource* arr[],
                     int &count,
                     Resource* r) {

        arr[count++] = r;

        cout << "Resource Added Successfully."
             << endl;
    }

    void removeResource(Resource* arr[],
                        int &count,
                        int id) {

        for (int i = 0;
             i < count;
             i++) {

            if (arr[i]->getID() == id) {

                delete arr[i];

                for (int j = i;
                     j < count - 1;
                     j++) {

                    arr[j] = arr[j + 1];
                }

                count--;

                cout << "Resource Removed."
                     << endl;

                return;
            }
        }

        cout << "Resource Not Found."
             << endl;
    }

    void generateReport(Resource* arr[],
                        int count) {

        if (count == 0) {
            throw InvalidReportException();
        }

        cout << "\n========== LIBRARY REPORT =========="
             << endl;

        for (int i = 0;
             i < count;
             i++) {

            arr[i]->displayInfo();
        }
    }

    void searchResource() override {

        cout << "Admin Searching..."
             << endl;
    }


};


/* =====================================================
   LIBRARY SYSTEM CLASS
   ===================================================== */

class LibrarySystem {
private:

    Resource* resources[20];
    int resourceCount;

    Customer customers[20];
    int customerCount;

    Admin admin;

public:

    LibrarySystem()
        : admin(99, "admin", "admin") {

        resourceCount = 0;
        customerCount = 0;

        loadResourcesFromFile();
        loadCustomerFromFile();

        if (resourceCount == 0) {

            resources[resourceCount++] =
                new Book(1,
                         "OOP",
                         "Programming",
                         "Gracie",
                         "111",
                         5);

            resources[resourceCount++] =
                new Book(2,
                         "C++ for Dummies",
                         "Education",
                         "Taylor",
                         "222",
                         6);

            resources[resourceCount++] =
                new Magazine(3,
                             "Tech Mine",
                             "Education",
                             101,
                             4);
        }
    }

    ~LibrarySystem() {

        saveResourcesToFile();
        saveCustomerToFile();

        for (int i = 0;
             i < resourceCount;
             i++) {

            delete resources[i];
        }
    }

    /* ===================== DISPLAY ===================== */

    void displayAvailableResources() {

        cout << "\n========== AVAILABLE RESOURCES =========="
             << endl;

        for (int i = 0;
             i < resourceCount;
             i++) {

            if (resources[i]->checkAvailability()) {

                resources[i]->displayInfo();
            }
        }
    }

    /* ===================== SEARCH ===================== */

    void searchByTitle(string title) {

        bool found = false;

        for (int i = 0;
             i < resourceCount;
             i++) {

            if (resources[i]->getTitle().find(title)
                != string::npos) {

                resources[i]->displayInfo();

                found = true;
            }
        }

        if (!found) {

            cout << "No Resource Found."
                 << endl;
        }
    }

    void searchByCategory(string cat) {

        bool found = false;

        for (int i = 0;
             i < resourceCount;
             i++) {

            if (resources[i]->getCategory().find(cat)
                != string::npos) {

                resources[i]->displayInfo();

                found = true;
            }
        }

        if (!found) {

            cout << "No Resource Found."
                 << endl;
        }
    }

    void searchByAuthor(string author) {

        bool found = false;

        for (int i = 0;
             i < resourceCount;
             i++) {

            string a =
                resources[i]->getAuthor();

            if (a != "" &&
                a.find(author) != string::npos) {

                resources[i]->displayInfo();

                found = true;
            }
        }

        if (!found) {

            cout << "No Resource Found."
                 << endl;
        }
    }

    void searchByID(int id) {

        bool found = false;

        for (int i = 0;
             i < resourceCount;
             i++) {

            if (resources[i]->getID() == id) {

                resources[i]->displayInfo();

                found = true;
            }
        }

        if (!found) {

            cout << "No Resource Found."
                 << endl;
        }
    }

    void generateCustomerReport() {

    cout << "\n========== CUSTOMER REPORT =========="
         << endl;

    if (customerCount == 0) {

        cout << "No Customers Registered." << endl;
        return;
    }

    for (int i = 0;
         i < customerCount;
         i++) {

        customers[i].displayCustomerInfo();
    }
    }

    void updateResource() {

    int id;

    cout << "Enter Resource ID to Update: ";
    cin >> id;

    for (int i = 0;
         i < resourceCount;
         i++) {

        if (resources[i]->getID() == id) {

            string newTitle;
            string newCategory;

            cin.ignore();

            cout << "Enter New Title: ";
            getline(cin, newTitle);

            cout << "Enter New Category: ";
            getline(cin, newCategory);

            int copies;

            cout << "Enter New Total Copies: ";
            cin >> copies;

            if (resources[i]->getType() == "BOOK") {

                string newAuthor;
                string newISBN;

                cin.ignore();

                cout << "Enter New Author: ";
                getline(cin, newAuthor);

                cout << "Enter New ISBN: ";
                getline(cin, newISBN);

                delete resources[i];

                resources[i] =
                    new Book(id,
                             newTitle,
                             newCategory,
                             newAuthor,
                             newISBN,
                             copies);
            }

            else if (resources[i]->getType() == "MAGAZINE") {

                int issue;

                cout << "Enter New Issue Number: ";
                cin >> issue;

                delete resources[i];

                resources[i] =
                    new Magazine(id,
                                 newTitle,
                                 newCategory,
                                 issue,
                                 copies);
            }

            cout << "Resource Updated Successfully."
                 << endl;

            return;
        }
    }

    cout << "Resource Not Found." << endl;
    }

    void generateIssuedOverdueReport() {

    int currentDay;

    cout << "Enter Current Day Number: ";
    cin >> currentDay;

    cout << "\n========== ISSUED / OVERDUE REPORT =========="
         << endl;

    bool found = false;

    for (int i = 0;
         i < customerCount;
         i++) {

        for (int j = 0;
             j < customers[i].getHistoryCount();
             j++) {

            BorrowTransaction t =
                customers[i].getTransaction(j);

            if (t.getReturnDate() == 0) {

                found = true;

                cout << "\nIssued Resource: "
                     << t.getResourceTitle()
                     << endl;

                cout << "Customer: "
                     << customers[i].getUsername()
                     << endl;

                cout << "Due Date: "
                     << t.getDueDate()
                     << endl;

                if (currentDay > t.getDueDate()) {

                    cout << "Status: OVERDUE"
                         << endl;
                }

                else {

                    cout << "Status: Currently Issued"
                         << endl;
                }
            }
        }
    }

    if (!found) {

        cout << "No Issued Resources Found."
             << endl;
    }
    }


    


    /* ===================== FILE HANDLING ===================== */

    void saveResourcesToFile() {
        ofstream fout("resources.txt");
        if (!fout) {
            cout << "File Error." << endl;
            return;
        }
        
        fout << resourceCount << endl;
        
        for (int i = 0; i < resourceCount; i++) {
            fout << resources[i]->getType() << endl;
            fout << resources[i]->getID() << endl;
            fout << resources[i]->getTitle() << endl;
            fout << resources[i]->getCategory() << endl;
            fout << resources[i]->getAvailableCopies() << endl;
            fout << resources[i]->getTotalCopies() << endl;

            if (resources[i]->getType() == "BOOK") {
                fout << resources[i]->getAuthor() << endl;
                fout << resources[i]->getISBN() << endl;
            }

            else if (resources[i]->getType() == "MAGAZINE") {
                fout << resources[i]->getIssueNumber() << endl;
            }
        }
        fout.close();
    }

    void loadResourcesFromFile() {
        ifstream fin("resources.txt");
        if (!fin){
            return;
        }    
        fin >> resourceCount;
        fin.ignore();
        
        for (int i = 0; i < resourceCount; i++) {
            string type;
            getline(fin, type);

            int id;
            int available;
            int total;
            string title;
            string category;

            fin >> id;
            fin.ignore();

            getline(fin, title);
            getline(fin, category);

            fin >> available;
            fin >> total;

            fin.ignore();

            if (type == "BOOK") {
                string author;
                string isbn;
                getline(fin, author);
                getline(fin, isbn);
                
                resources[i] = new Book(id,
                         title,
                         category,
                         author,
                         isbn,
                         total);
            }

            else if (type == "MAGAZINE") {
                int issue;

                fin >> issue;
                fin.ignore();

                resources[i] = new Magazine(id,
                             title,
                             category,
                             issue,
                             total);
            }

            while (resources[i]->getAvailableCopies()> available) {
                resources[i]->borrowResource();
            }
        }
        
        fin.close();
    }

    void saveCustomerToFile() {

        ofstream fout("customer.txt");

        if (!fout) {

            cout << "File Error."
                 << endl;

            return;
        }

        fout << customerCount << endl;

        for (int i = 0;
             i < customerCount;
             i++) {

            fout << customers[i].getUsername()
                 << endl;

            fout << customers[i].getPassword()
                 << endl;

            fout << customers[i].getFirstName()
                 << endl;

            fout << customers[i].getLastName()
                 << endl;

            fout << customers[i].getAddress()
                 << endl;

            fout << customers[i].getBalance()
                 << endl;
        }

        fout.close();
    }

    void loadCustomerFromFile() {

        ifstream fin("customer.txt");

        if (!fin)
            return;

        fin >> customerCount;

        fin.ignore();

        for (int i = 0;
             i < customerCount;
             i++) {

            string username;
            string password;
            string firstName;
            string lastName;
            string address;

            double balance;

            getline(fin, username);
            getline(fin, password);
            getline(fin, firstName);
            getline(fin, lastName);
            getline(fin, address);

            fin >> balance;
            fin.ignore();

            customers[i] =
                Customer(i + 1,
                         username,
                         password,
                         firstName,
                         lastName,
                         address,
                         balance);
        }

        fin.close();
    }

    /* ===================== CUSTOMER MENU ===================== */

    void customerMenu(Customer &customer) {

        int choice;

        do {

            cout << "\nCurrent Balance: "
                 << customer.getBalance()
                 << endl;

            cout << "\n=========== CUSTOMER MENU ==========="
                 << endl;

            cout << "1. View Resources" << endl;
            cout << "2. Borrow Resource" << endl;
            cout << "3. Return Resource" << endl;
            cout << "4. View Borrow History" << endl;
            cout << "5. Add Balance" << endl;
            cout << "6. Search By Title" << endl;
            cout << "7. Search By Category" << endl;
            cout << "8. Search By Author" << endl;
            cout << "9. Search By ID" << endl;
            cout << "0. Logout" << endl;

            cin >> choice;

            if (choice == 1) {

                displayAvailableResources();
            }

            else if (choice == 2) {

                int id;

                cout << "Enter Resource ID: ";
                cin >> id;

                for (int i = 0;
                     i < resourceCount;
                     i++) {

                    if (resources[i]->getID() == id) {

                        customer.borrowResource(
                            resources[i]);
                    }
                }
            }

            else if (choice == 3) {

                int id;

                cout << "Enter Resource ID: ";
                cin >> id;

                for (int i = 0;
                     i < resourceCount;
                     i++) {

                    if (resources[i]->getID() == id) {

                        customer.returnBorrowedResource(
                            resources[i]);
                    }
                }
            }

            else if (choice == 4) {

                customer.viewBorrowHistory();
            }

            else if (choice == 5) {

                double amount;

                cout << "Enter Amount: ";
                cin >> amount;

                customer.addBalance(amount);

                cout << "Balance Added Successfully."
                     << endl;
            }

            else if (choice == 6) {

                string title;

                cin.ignore();

                cout << "Enter Title: ";
                getline(cin, title);

                searchByTitle(title);
            }

            else if (choice == 7) {

                string category;

                cin.ignore();

                cout << "Enter Category: ";
                getline(cin, category);

                searchByCategory(category);
            }

            else if (choice == 8) {

                string author;

                cin.ignore();

                cout << "Enter Author: ";
                getline(cin, author);

                searchByAuthor(author);
            }

            else if (choice == 9) {

                int id;

                cout << "Enter ID: ";
                cin >> id;

                searchByID(id);
            }

        } while (choice != 0);
    }

    /* ===================== ADMIN MENU ===================== */

    void adminMenu() {

        int choice;

        do {

            cout << "\n=========== ADMIN MENU ==========="
                 << endl;

            cout << "1. Generate Report" << endl;
            cout << "2. Add Resource" << endl;
            cout << "3. Remove Resource" << endl;
            cout << "4. Generate Customer Report" << endl;
            cout << "5. Update Resource" << endl;
            cout << "6. Generate Issued/Overdue Report" << endl;
            cout << "0. Logout" << endl;

            cin >> choice;

            if (choice == 1) {

                try {

                    admin.generateReport(resources,
                                         resourceCount);
                }

                catch (exception &e) {

                    cout << e.what()
                         << endl;
                }
            }

            else if (choice == 2) {

                if (resourceCount >= 20) {

                    cout << "Resource Limit Reached."
                         << endl;

                    continue;
                }

                string title;
                string category;
                string author;
                string isbn;

                int copies;

                cin.ignore(1000, '\n');

                cout << "Title: ";
                getline(cin, title);

                cout << "Category: ";
                getline(cin, category);

                cout << "Author: ";
                getline(cin, author);

                cout << "ISBN: ";
                getline(cin, isbn);

                cout << "Copies: ";
                cin >> copies;

                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid Input." << endl;
                    continue;
                }

                int id = resourceCount + 1;

                resources[resourceCount] =
                    new Book(id,
                             title,
                             category,
                             author,
                             isbn,
                             copies);
                    resourceCount++;         

                cout << "Book Added Successfully."
                     << endl;
            }

            else if (choice == 3) {

                int id;

                cout << "Enter Resource ID: ";
                cin >> id;

                admin.removeResource(resources,
                                     resourceCount,
                                     id);
            }

            else if (choice == 4) {
                generateCustomerReport();
            }
            
            else if (choice == 5) {
                updateResource();
            }
            
            else if (choice == 6) {
                generateIssuedOverdueReport();
            }


        } while (choice != 0);
    }

    /* ===================== MAIN MENU ===================== */

    void mainMenu() {

        int choice;

        do {

            cout << "\n===== LIBRARY SYSTEM ====="
                 << endl;

            cout << "1. Register" << endl;
            cout << "2. Login" << endl;
            cout << "3. Admin Login" << endl;
            cout << "0. Exit" << endl;

            cin >> choice;

            if (choice == 1) {

                if (customerCount < 20) {

                    customers[customerCount]
                        .registerAccount();

                    customerCount++;
                }

                else {

                    cout << "Customer Limit Reached."
                         << endl;
                }
            }

            else if (choice == 2) {

                string u;
                string p;

                cout << "Username: ";
                cin >> u;

                cout << "Password: ";
                cin >> p;

                bool found = false;

                for (int i = 0;
                     i < customerCount;
                     i++) {

                    if (customers[i].login(u, p)) {

                        cout << "Login Successful."
                             << endl;

                        customerMenu(customers[i]);

                        found = true;

                        break;
                    }
                }

                if (!found) {

                    cout << "Invalid Username or Password."
                         << endl;
                }
            }

            else if (choice == 3) {

                string u;
                string p;

                cout << "Admin Username: ";
                cin >> u;

                cout << "Admin Password: ";
                cin >> p;

                if (admin.login(u, p)) {

                    cout << "Admin Login Successful."
                         << endl;

                    adminMenu();
                }

                else {

                    cout << "Invalid Admin Credentials."
                         << endl;
                }
            }

        } while (choice != 0);

        cout << "Saving Data..."
             << endl;
    }
};

/* =====================================================
   MAIN FUNCTION
   ===================================================== */

int main() {

    LibrarySystem system;

    system.mainMenu();

    return 0;
}