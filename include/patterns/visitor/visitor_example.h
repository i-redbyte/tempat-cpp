#ifndef TEMPAT_CPP_VISITOR_EXAMPLE_H
#define TEMPAT_CPP_VISITOR_EXAMPLE_H

//Stub class for implementation examples
class Visitable1 : public BaseVisitable<Visitable1> {
};

//Stub class for implementation examples
class Visitable2 : public BaseVisitable<Visitable2> {
};

//Stub class for implementation examples
class Visitable3 : public BaseVisitable<Visitable3> {
};

class VisitorDerived : public BaseVisitor,
                       public Visitor<Visitable1>,
                       public Visitor<int>,
                       public Visitor<Visitable2>,
                       public Visitor<Visitable3> {
public:
    void visit(Visitable1 &classObject) override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << "First visit" << std::endl;
    }

    void visit(Visitable2 &classObject) override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << "Second visit" << std::endl;
    }

    void visit(Visitable3 &classObject) override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << "Third visit" << std::endl;
    }

    void visit(int &v) override {
        std::cout << "O-E-E!" << std::endl;
    }
};

#endif
