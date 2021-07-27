#ifndef TEMPAT_CPP_VISITOR_H
#define TEMPAT_CPP_VISITOR_H

class BaseVisitor {
public:
    virtual ~BaseVisitor() = default;;
};

template<class T>
class Visitor {
public:
    virtual void visit(T &) = 0;
};

template<class Visitable>
class BaseVisitable {
public:
    template<typename T>
    void accept(T &visitor) {
        visitor.visit(static_cast<Visitable &>(*this));
    }
};

#endif
