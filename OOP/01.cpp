// objects are entities in the real world
// class is like a blueprint of these entities
// avoids repetition and promotes reusability
// helps in organizing code better
#include <iostream>
#include <string>
using namespace std;
// access modifiers: public, private, protected
// public: can be accessed from anywhere
// private: can be accessed only within the class
// protected: can be accessed within the class and its derived classes
// default is private

class Teacher
{
private:
    int salary;

public:
    string name;
    string dept;
    string subject;

    // constructor== non parameterized constructor
    Teacher()
    {
        cout << "Constructor1 called" << endl;
        dept = "CSE";
        // this->dept = "CSE"; is same as dept = "CSE";
    }
    // constructor== parameterized constructor
    // constructor overloading: same name but different parameters:polymorphism
    Teacher(string name)
    {
        cout << "Constructor2 called" << endl;
        dept = "CSE";
        this->name = name;
        // this pointer: pointer to the current object
        // this->name = name; is same as *(this).name = name;
        // this->dept = "CSE"; is same as dept = "CSE";
    }
    // copy constructor: used to copy the values of an object to another object

    Teacher(Teacher &t)
    {
        cout << "Copy constructor called" << endl;
        name = t.name;
        dept = t.dept;
        subject = t.subject;
        salary = t.salary;
        // this->name = t.name; is same as name = t.name;
        // now instead of default copy constructor, this constructor will be called when we create an object like Teacher t3(t1);
    }
    void changeDept(string newDept)
    {
        dept = newDept;
    }
    void setSalary(int s)
    {
        salary = s;
    }
    int getSalary()
    {
        return salary;
    }
};

// example for shallow copy:
class StudentShallow
{
public:
    string name;
    double *cgpaptr;
    StudentShallow(string name, double cgpa)
    {
        this->name = name;
        this->cgpaptr = new double();
        *this->cgpaptr = cgpa;
    }
    // Default copy constructor (shallow copy) - commented out to show it's the default behavior
    StudentShallow(StudentShallow &s)
    {
        cout << "Shallow copy constructor called" << endl;
        name = s.name;
        cgpaptr = s.cgpaptr; // Both objects point to the same memory location
    }
    void getInfo()
    {
        cout << "Name: " << name << endl;
        cout << "CGPA: " << *cgpaptr << endl;
    }
    // Destructor to free dynamically allocated memory
    // compiler automatically calls the destructor when the object is destroyed, if not defined, it is automatically defined by the compiler
    ~StudentShallow()
    {
        delete cgpaptr; // only normal variables are deleted automatically, pointers need to be deleted manually
    }
};

// example for deep copy:
class StudentDeep
{
public:
    string name;
    double *cgpaptr;
    StudentDeep(string name, double cgpa)
    {
        this->name = name;
        // this->cgpaptr = new double(cgpa);
        // dynamically allocating memory for the cgpa pointer
        this->cgpaptr = new double();
        *this->cgpaptr = cgpa;
    }
    StudentDeep(StudentDeep &s)
    {
        cout << "Copy constructor (Deep copy) called" << endl;
        name = s.name;
        cgpaptr = new double(*s.cgpaptr);
        // dynamically allocating memory for the cgpa pointer - creates a new copy
    }
    void getInfo()
    {
        cout << "Name: " << name << endl;
        cout << "CGPA: " << *cgpaptr << endl;
    }
    // Destructor to free dynamically allocated memory
    ~StudentDeep()
    {
        delete cgpaptr;
    }
};

// inheritance:
class Person
{
public:
    string name;
    int age;
    Person(string name, int age)
    {
        cout << "Person constructor called" << endl;
        this->name = name;
        this->age = age;
        // runs first because it is a base class and constructor is called in order of inheritance
    }
    ~Person()
    {
        cout << "Person destructor called" << endl;
        // runs last because it is a base class and destructor is called in reverse order of inheritance
    }
};
class Student : public Person
{
    // name,age,rollno
public:
    int rollno;
    Student(string name, int age, int rollno) : Person(name, age)
    {
        cout << "Student constructor called" << endl;
        this->rollno = rollno;
    }
    void getInfo()
    {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Roll No: " << rollno << endl;
    }
    ~Student()
    {
        cout << "Student destructor called" << endl;
        // runs first because it is a derived class and destructor is called in reverse order of inheritance
    }
};
int main()

{
    // Teacher t1("aniket");
    // Teacher t2;
    // t1.name = "aniket";
    // t1.dept = "CSE";
    // t1.subject = "OOP";
    // // t1.salary = 100000; //private member cannot be accessed outside the class
    // t1.setSalary(100000);
    // cout << t1.getSalary() << endl;
    // t1.changeDept("CSE");
    // Teacher t3(t1); // copy constructor is called, stores same values as t1 in t3
    // cout << t1.name << endl;

    // example for shallow copy:
    // cout << "=== Shallow Copy Example ===" << endl;
    // StudentShallow s1("aniket", 9.5);
    // s1.getInfo();
    // StudentShallow s2(s1); // Uses default copy constructor (shallow copy)
    // *(s2.cgpaptr) = 9.8;
    // s1.getInfo(); // 9.8 - value DOES change because both s1 and s2 share the same cgpaptr
    // s2.getInfo(); // 9.8
    // cout << endl;

    // // example for deep copy:
    // cout << "=== Deep Copy Example ===" << endl;
    // StudentDeep s3("rahul", 9.5);
    // s3.getInfo();
    // StudentDeep s4(s3); // Uses custom copy constructor (deep copy)
    // *(s4.cgpaptr) = 9.8;
    // s3.getInfo(); // 9.5 - value doesn't change because we implemented deep copy in copy constructor
    // If we had shallow copy (default copy constructor), both s3 and s4 would share the same cgpaptr
    // and changing s4.cgpaptr would also change s3.cgpaptr to 9.8
    // But with deep copy, s3.cgpaptr and s4.cgpaptr point to different memory locations
    // s4.getInfo(); // 9.8
    Student s1("aniket", 20, 123);
    s1.getInfo();

    return 0;
}

// encapsulation: wrapping up of data and methods that work on data within a single unit called class and hiding the background details
// data/property+member functions=class
// data hiding: hiding the data from the outside world(we can use private access modifier to hide the data)

// data encapsulation: bundling of data with the methods that work on data
// data abstraction: showing only essential details and hiding the background details

//---
// constructors: special member functions that are used to initialize objects, invoked automatically when an object is created, used for initialization
// same name as the class name
// doesn't have a return type, only called once when an object is created
// memory allocation happens automatically when an object is created

//---
// shallow copy vs deep copy:
// shallow copy: copy the values of the object to another object (including pointers, so both point to same memory)
// deep copy: copy the values of the object to another object but also makes copies of any dynamically allocated memory that memory is pointing to

//---
// mode of inheritance:
// ┌──────────────┬───────────────────┬──────────────────────┬──────────────────┐
// │  Base Class  │  Derived Class    │   Derived Class      │  Derived Class   │
// │              │  Private Mode     │   Protected Mode     │  Public Mode     │
// ├──────────────┼───────────────────┼──────────────────────┼──────────────────┤
// │  Private     │  Not Inherited    │   Not Inherited      │  Not Inherited   │
// ├──────────────┼───────────────────┼──────────────────────┼──────────────────┤
// │  Protected   │  Private          │   Protected          │  Protected       │
// ├──────────────┼───────────────────┼──────────────────────┼──────────────────┤
// │  Public      │  Private          │   Protected          │  Public          │
// └──────────────┴───────────────────┴──────────────────────┴──────────────────┘
//
// Explanation:
// - Private members are NEVER inherited (not accessible in derived class)
// - When inheriting with PRIVATE mode: protected and public members become private
// - When inheriting with PROTECTED mode: protected stays protected, public becomes protected
// - When inheriting with PUBLIC mode: protected stays protected, public stays public

//---
// types of inheritance:
// 1. single inheritance: one base class, one derived class (A -> B)
//    class B : public A {
//        B() : A() { } // constructor calls base class constructor
//    };
// 2. multiple inheritance: multiple base classes, one derived class (A, B -> C)
//    class C : public A, public B {
//        C() : A(), B() { } // constructor calls both base class constructors
//    };
// 3. multi-level inheritance: chain of inheritance (A -> B -> C)
//    class B : public A {
//        B() : A() { }
//    };
//    class C : public B {
//        C() : B() { } // calls B's constructor, which calls A's constructor
//    };
// 4. hybrid inheritance: combination of multiple and multi-level inheritance
//    class B : public A {
//        B() : A() { }
//    };
//    class C : public A {
//        C() : A() { }
//    };
//    class D : public B, public C {
//        D() : B(), C() { } // calls both B and C constructors
//    };
// 5. hierarchical inheritance: one base class, multiple derived classes (A -> B, C, D)
//    class B : public A {
//        B() : A() { }
//    };
//    class C : public A {
//        C() : A() { }
//    };
//    class D : public A {
//        D() : A() { }
//    };

//---
// polymorphism: the ability of objects to take on different forms or behave in different ways depending on the context in which they are used, for eg non parameterized constructor and parameterized constructor of a class are different forms of the same constructor
// 1. compile time polymorphism: function overloading--constructor overloading
// 2. run time polymorphism: function overriding

// function overloading: same name but different parameters, example:
class A
{
public:
    void print()
    {
        cout << "print function" << endl;
    }
    // it gets decided at compile time which function to call based on the parameters passed based on the type of the parameters or no parameters passed
    void print(int x)
    {
        cout << "print function with int parameter" << endl;
    }
    void print(double x)
    {
        cout << "print function with double parameter" << endl;
    }
};
// operator overloading: same operator but different parameters, example:
class A
{
public:
    void operator+(int x)
    {
        cout << "operator+ function with int parameter" << endl;
    }
    void operator+(double x)
    {
        cout << "operator+ function with double parameter" << endl;
    }
};

// run time polymorphism: function overriding, example:
// function overriding: same name but different parameters, when parent and child both contain the same function with different implementaion.
class A
{
public:
    virtual void print()
    {
        cout << "print function" << endl;
    }
};
class B : public A
{
public:
    void print() // overrides the print function in the parent class
    {
        cout << "print function in B" << endl;
    }
};
// virtual functions: functions that are declared in the base class and overridden in the derived class, dynamic in nature, called at runtime.
class A
{
public:
    virtual void print()
    {
        cout << "print function" << endl;
    }
};
class B : public A
{
public:
    void print() // overrides the print function in the parent class
    {
        cout << "print function in B" << endl;
    }
};

//---
// abstraction: showing only essential details and hiding the background details, achieved through abstract classes and interfaces.
// abstract class: a class that contains at least one pure virtual function and cannot be instantiated, provides a base class for other classes to inherit from, can not be instantiated directly, example:
class AbstractClass
{
public:
    virtual void print() = 0; // pure virtual function, automatically makes the class abstract
    void show()
    {
        cout << "show function in AbstractClass" << endl;
    }
};
class DerivedClass : public AbstractClass
{
public:
    void print() // overrides the print function in the parent class
    {
        cout << "print function in DerivedClass" << endl;
    }
};

//---
// static keyword: used to create class-level members that are shared across all instances
// static variables: created and initialized once and shared by all objects of the class, exist for the lifetime of the program
class StaticVariableExample
{
public:
    static int count; // declaration
    StaticVariableExample()
    {
        count++;
    }
    static void displayCount() // static function
    {
        cout << "Count: " << count << endl;
    }
};
int StaticVariableExample::count = 0; // definition and initialization

// static functions: can be called without creating an object of the class, can only access static members
class StaticFunctionExample
{
public:
    static int value;
    static void setValue(int v) // static function example
    {
        value = v;
    }
    static int getValue()
    {
        return value;
    }
};
int StaticFunctionExample::value = 0;

// Note: static variables and functions are shared by all objects of the class
// They belong to the class itself, not to any particular object instance