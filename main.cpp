#include <iostream>
#include "patterns/visitor/visitor.h"
#include "patterns/visitor/visitor_example.h"

void visitorPatternExample();

int main(int argc, char **argv) {
    visitorPatternExample();
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
