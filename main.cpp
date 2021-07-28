#include <iostream>
#include "include/patterns/visitor/visitor.h"
#include "include/patterns/visitor/visitor_example.h"
#include "include/patterns/singleton/singleton.h"
#include "include/patterns/fabric/abstract_fabric.h"

void visitorPatternExample();

void singletonPatternExample();

void abstractFactoryExample();

int main(int argc, char **argv) {
    visitorPatternExample();
    singletonPatternExample();
    abstractFactoryExample();
    return 0;
}

void abstractFactoryExample() {
    using AFactory = abstract_factory<tl<int, bool>>;
    using CFactory = concrete_factory<AFactory, tl<int, bool>>;

    CFactory concreteFactory;
    AFactory *abstractFactory = &concreteFactory;

    std::unique_ptr<int> a = abstractFactory->create<int>();
    std::unique_ptr<bool> b = abstractFactory->create<bool>();
    *a = 10;
    *b = 2 == 2;
    std::cout << *a << " " << *b << std::endl;
}

void singletonPatternExample() {
    //class for test singleton
    class A {
    public:
        A() : count(0) {}

        inline void inc() { ++count; }

        inline unsigned int get() const { return count; }

    private:
        unsigned int count;
    };
    A &first = singleton::Singleton<A>::Instance();
    first.inc();
    A &second = singleton::Singleton<A>::Instance();
    second.inc();
    singleton::Singleton<A>::Instance().inc();

    bool isInstanceEq = std::addressof(first) == std::addressof(second);
    std::cout << "Equal instances == " << std::boolalpha << isInstanceEq << std::endl;

    std::cout << "second.get() == " << second.get() << std::endl;
}

void visitorPatternExample() {
    VisitorDerived visitor;
    Visitable1 visitable1;
    Visitable2 visitable2;
    Visitable3 visitable3;
    visitable1.accept<VisitorDerived>(visitor);
    visitable2.accept<VisitorDerived>(visitor);
    visitable3.accept<VisitorDerived>(visitor);
    int x = 200;
    visitor.visit(x);
}
