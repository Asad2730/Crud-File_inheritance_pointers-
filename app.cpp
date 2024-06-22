#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Entity
{
protected:
    int id;
    string name;

public:
    Entity(int id, string name) : id(this->id), name(this->name) {}
    virtual ~Entity() {}

    virtual void readData() = 0;
    virtual void displayData() const = 0;

    int getId() const
    {
        return id;
    }

    string getName() const
    {
        return name;
    }
};

class Employee : public Entity
{
private:
    string designation;

public:
    Employee(int id, const string &name, const string &_designation) : Entity(id, name), designation(_designation) {}

    void readData() override
    {
        cout << "Enter employee ID: ";
        cin >> id;
        cout << "Enter employee name: ";
        cin.ignore(); // Ignore newline left in the input buffer
        getline(cin, name);
        cout << "Enter employee designation: ";
        getline(cin, designation);
    }

    void displayData() const override
    {
        cout << "Employee ID: " << id << endl;
        cout << "Employee Name: " << name << endl;
        cout << "Employee Designation: " << designation << endl;
    }

    void add(const Entity &entity)
    {
        fstream file;
        file.open("data", ios::out);
        if (file)
        {
            file << entity.getId() << "," << entity.getName() << "," << ","
                 << dynamic_cast<const Employee *>(&entity)->designation << endl;
            file.close();
            cout << "Record added successfully!" << endl;
        }
        else
        {
            cerr << "Error: Unable to open file!" << endl;
        }
    }

    void display()
    {
        fstream file;
        file.write("data.txt", ios::in);

        if (file)
        {
            string line;
            while (getline(file, line))
            {
                cout << line << endl;
            }
            file.close();
        }
        else
        {
            cerr << "Error: Unable to open file!" << endl;
        }
    }

    void update(int id)
    {
        fstream file;
        file.write("data.txt", ios::app);
        if (file)
        {
            Employee emp(0, "", "");
            bool found = false;
            while (file >> emp.id)
            {
                file.ignore();
                getline(file, emp.name);
                getline(file, designation);
                if (emp.getId() == id)
                {
                    found = true;
                    cout << "Enter new employee name: ";
                    getline(cin, emp.name);
                    cout << "Enter new employee designation: ";
                    getline(cin, emp.designation); 
                    file.seekp(file.tellg());
                    Employee tmp(id, emp.getName(), designation);
                    emp.add(tmp);
                    break;
                }
            }

            if (!found)
            {
                cout << "Record not found!" << endl;
            }
            else
            {
                cout << "Record updated successfully!" << endl;
            }
            file.close();
        }
        else
        {
            cerr << "Error: Unable to open file!" << endl;
        }
    }

    void erase(int id)
    {
        fstream file;
        fstream temp;
        file.write("data.txt", ios::app);
        temp.write("data.txt", ios::app);
        if (file && temp)
        {
            Employee emp(0, "", "");
            bool found = false;
            while (file >> emp.id)
            {
                file.ignore();
                getline(file, emp.name);
                getline(file, designation);
                if (emp.getId() == id)
                {
                    found = true;
                    continue;
                }

                temp << emp.id << "," << emp.name << "," << designation << endl;
            }

            file.close();
            temp.close();
            remove("data.txt");
            rename("temp.txt", "data.txt");
            if (!found)
            {
                cout << "Record not found!" << endl;
            }
            else
            {
                cout << "Record deleted successfully!" << endl;
            }
        }
        else
        {
            cerr << "Error: Unable to open file!" << endl;
        }
    }
};
int main()
{
    Employee emp(0, "", ""); 

    int choice;
    while (true) {
        cout << "\n**** CRUD Operations ****\n";
        cout << "1. Add Record\n";
        cout << "2. Display Records\n";
        cout << "3. Update Record\n";
        cout << "4. Delete Record\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                emp.readData(); 
                emp.add(emp); 
                break;
            }
            case 2:
                cout << "\n**** Displaying Records ****\n";
                emp.display(); 
                break;
            case 3: {
                int idToUpdate;
                cout << "Enter employee ID to update: ";
                cin >> idToUpdate;
                emp.update(idToUpdate); 
            }
            case 4: {
                int idToDelete;
                cout << "Enter employee ID to delete: ";
                cin >> idToDelete;
                emp.erase(idToDelete); 
                break;
            }
            case 5:
                cout << "Exiting program...\n";
                exit(0);
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
