#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H
#include <cstdlib>
#include "Pointer.h"
#include "Exception.h"

namespace YzcLib {

template <typename T>
class SharedPointer : public Pointer<T>
{
protected:
    int *m_ref;
    void assign(const SharedPointer<T>& obj)
    {
        this->m_ref = obj.m_ref;
        this->pointer = obj.pointer;
        if( this->m_ref )
        {
            (*this->m_ref)++;
        }
    }
public:
    SharedPointer(T* p = NULL): m_ref(NULL)
    {
        if( p )
        {
            this->m_ref = static_cast<int*>(std::malloc(sizeof(int)));
            if( this->m_ref )
            {
                *(this->m_ref) = 1;
                this->pointer = p;
            }
            else
            {
                THROW_EXCEPTION(NotEnoughMemoryException, "No memory to creat sharedpointer object ...");
            }

        }
    }

    SharedPointer(const SharedPointer<T>& obj): Pointer<T>(NULL)
    {
        assign(obj);
    }

    SharedPointer<T>& operator =( const SharedPointer<T>& obj)
    {
        if(this != &obj)
        {
            Clear();
            assign(obj);
        }
        return *this;
    }

    void Clear()
    {
        T* toDel = this->pointer;
        int* ref = this->m_ref;

        this->pointer = NULL;
        this->m_ref = NULL;

        if(ref)
        {
            (*ref)--;
            if(*ref == 0)
            {
                free(ref);

                delete toDel;
            }
        }
    }

    ~SharedPointer()
    {
        Clear();
    }
};
template <typename T>
bool operator == (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return (l.Get() == r.Get());
}

template <typename T>
bool operator != (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return (l.Get() != r.Get());
}
}

#endif // SHAREDPOINTER_H