// The useful bits of about inheritance in C++

#include <memory>
#include <string>
#include <format>
#include <iostream>

class ABCVector
{
public:
    ABCVector(const std::string &name) : name_(name)
    {
    }

    // Virtual destructors:
    // When a class is to be used as a base class, its destructor should
    // always be marked as virtual. Else, if a base class pointer points
    // to a derived class instance, only the base class destructor will be
    // called upon destruction of said pointer.
    // Or, more compactly:
    // Use virtual destructors for classes with polymorphic deletion
    //
    // Pure virtual destructors:
    // Marking the destructor of a class as pure virtual(=0),
    // effectively makes the class into an ABC (Abstract Base Class),
    // meaning that it cannot be instantiated.
    // For a class to be an ABC, at least one of its methods has to
    // be pure virtual. If all other methods have a default implementation,
    // the destructor can be made pure, just for the sake of making
    // the class into an ABC.
    // However, despite being pure virtual, the ABC's destructor MUST be
    // defined outside of the class definition, as it will be called
    // by the derived classes.
    // More generally, any method that is marked as pure virtual (=0) can
    // also have a default implementation, e.g. if it provdes some common
    // functionality that maybe specialized.
    // See also https://stackoverflow.com/q/977543/29179464
    // and https://stackoverflow.com/q/34383516/29179464
    virtual ~ABCVector() = 0;

    virtual void info(const std::string &msg)
    {
        std::cout << std::format("[ABCVector]-[{}]: {}\n", name_, msg);
    }

protected:
    std::string name_{"None"};
};

// Pure virtual destructor definition
ABCVector::~ABCVector()
{
    info("Destructor");
}

class PVector : public ABCVector
{
public:
    PVector(const std::string &name) : ABCVector(name)
    {
    }

    ~PVector()
    {
        info("Destructor");
    }

    virtual void info(const std::string &msg) override
    {
        std::cout << std::format("[PVector]-[{}]: {}\n", name_, msg);
    }
};

class PXVector : public PVector
{
public:
    PXVector(const std::string &name) : PVector(name)
    {
    }

    // It is not necessary to define the destructor in
    // derived classes, except, of course, if some resource
    // must be freed by the subclass.
    // ~PXVector()
    // {
    //     info("Destructor");
    // }

    // Marking the method as final means that it cannot be overriden
    // by subclasses
    void info(const std::string &msg) override final
    {
        std::cout << std::format("[PXVector]-[{}]: {}\n", name_, msg);
    }

    int data[100]{};
};

int main()
{

    ABCVector *p_vector = new PXVector("F_BODY");

    p_vector->info("Data");

    // Base classes are destroyed in the reverse order
    // of the completion of their constructors.
    // (First constructed, last destructed)
    // Thus, for p_vector, the order in which
    // the destructors are called is:
    // 1) ~PXVector()
    // 2) ~PVector()
    // 3) ~ABCVector()
    delete p_vector;

    return 0;
}