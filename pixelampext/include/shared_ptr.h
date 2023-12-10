/////////////////////////////////////////////////////////////////////////////
// Name:        shared_ptr.h
// Purpose:     Implement shared pointers. Can be used into STL containers.
// Author:      Stéphane Château
// Modified by:
// Created:     14/01/2010
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <assert.h>

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

template <class T> class shared_ptr;

/////////////////////////////////////////
/** Class IRefCount.
*
* IRefCount is an interface for reference counting classes can implement it themselves,
* or shared_ptr will provide its internal implementation of IRefCount
*
* @see
*
* @author Stéphane Château
* @version Name : CAnsiPort<br>
*          Revision : <b>1.0</b>
*/
template <class T> class IRefCount
{
    friend class shared_ptr<T>;
protected:
    virtual                             ~IRefCount() {};
    virtual void                        IncRefCount() = 0;
    virtual void                        DecRefCount() = 0;
    virtual T*                          GetPtr() const = 0;
};

//============================================================
/** Class IRefCountImpl.
*
* IRefCountImpl is a standart implementation of IRefCount
* To use it just derive your class from it: class CMyObject : public IRefCountImpl<CMyObject> { ... };
* Reminder: implementing IRefCount is optional but it would reduce
* memory fragmentation.
*
* @see IRefCount<T>
*
* @author Stéphane Château
* @version Name : CAnsiPort<br>
*          Revision : <b>1.0</b>
*/
template <class T> class IRefCountImpl : public IRefCount<T>
{
private:
    int                                 m_iCounter;

protected:
    IRefCountImpl()
    {
        m_iCounter = 0;
    }

    IRefCountImpl(const IRefCountImpl<T>& _rMe)
    {   // Don't copy m_iCounter in copy constructor, it is another instance of this.
        // Counter must be set to 0.
        // Probleù when creating copy from an item pointer shared by more than on pointer, on copy item
        // destructor the counter is not 0 and assert fails
        m_iCounter = 0;
    }

    virtual ~IRefCountImpl()
    {
        // Pas de DestroyRef(); <- On est déjà en train de se détruire puisque GetPtr est this !!
        // Vérifie que l'on détruit un élément que lorsque son compteur est bien à 0
        // Si pas égal à 0 => des shared pointeurs pointent encore sur moi !!
        assert(m_iCounter == 0);
    }

protected:
    virtual void                        IncRefCount()
    {
        m_iCounter++;
    }

    virtual void                        DecRefCount()
    {
        m_iCounter--;
        if (m_iCounter <= 0)
        {
            DestroyRef();
        }
    }

    virtual T* GetPtr() const
    {
        return ((T*)this);
    }

    virtual void                        DestroyRef()
    {
        if (GetPtr() != nullptr)
        {
            // Vérifie que l'on détruit un élément que lorsque son compteur est bien à 0
            assert(m_iCounter == 0);
            DeletePtr();
        }
    }

    virtual void                        DeletePtr()
    {
        delete this;
    }
};

/** Class shared_ptr.
*
* Usage:
*
* ----------------------
* 1. In a program block
* ----------------------
* shared_ptr<CMyObject> ptr1(new CMyObject); // creates object 1
* shared_ptr<CMyObject> ptr2(new CMyObject); // creates object 2
* ptr1 = ptr2;             // destroys object 1
* ptr2 = nullptr;
* ptr1 = new CMyObject;    // creates object 3, destroys object 2
* ptr1->methodcall(...);
* CMyObject o1;
* // ptr1 = &o1;  // DON'T ! only memory allocated by new operator should be used
* CMyObject *o2 = new CMyObject;
* ptr1 = o2;
* //ptr2 = o2;  // DON'T ! unless CMyObject implements IRefCount
*               // try to use ptr1 = ptr2 instead, it's always safe;
* ----------------------
* 2. in a function call
* ----------------------
* void f(CMyObject *o) {...}
* ...
* shared_ptr<CMyObject> ptr(new CMyObject)
* f(ptr);
* ----------------------
* 3. As a return value
* ----------------------
* shared_ptr<CMyObject> f()
* {
*      shared_ptr<CMyObject> ptr(new CMyObject);
*      return ptr;
* }.
*
* @author Stéphane Château
* @version Name : CAnsiPort<br>
*          Revision : <b>1.0</b>
*/
template <class T> class shared_ptr
{
private:
    IRefCount<T>* m_pRefCount;

    /////////////////////////////////////////
    // __RefCounter
    // An internal implementation of IRefCount
    // for classes that don't implement it
    // shared_ptr will automatically choose between its internal and
    // class' implementation of IRefCount
    class __RefCounter : public IRefCountImpl<T>
    {
    private:
        T* m_ptr;

    protected:
        virtual T* GetPtr() const
        {
            return m_ptr;
        }

    public:
        __RefCounter(T* _ptr)
        {
            m_ptr = _ptr;
        }

        virtual ~__RefCounter()
        {   // Pas d'appel de DestroyRef() mais appel de celui de la classe de base
            IRefCountImpl<T>::DestroyRef();
        }

        virtual void                    DestroyRef()
        {   // Cette classe n'est crée que dans le Assign de la classe shared_ptr (la classe __RefCounter est privée)
            // Si la référence doit être détruit, this aussi (ils sont liés) - On fait alors un delete this, ceci appelera
            // ~__RefCounter() qui appelera le IRefCountImpl<T>::DestroyRef() de la classe de base, this will call DeletePtr()
            delete this;
        }

        virtual void                    DeletePtr()
        {
            delete GetPtr();
        }
    };

    // this method is called if T does not implement IRefCount
    void                                Assign(void* _ptr)
    {
        if (_ptr == nullptr)
        {
            Assign((IRefCount<T> *) nullptr);
        }
        else
        {
            Assign(new __RefCounter(static_cast<T*>(_ptr)));
        }
    }

    // this method is picked over Assign(void *ptr)
    // if T implements IRefCount.
    // This allows some memory usage optimization
    void                                Assign(IRefCount<T>* _pRefCount)
    {
        if (_pRefCount != nullptr)
        {
            _pRefCount->IncRefCount();
        }
        if (m_pRefCount != nullptr)
        {
            m_pRefCount->DecRefCount();
        }
        m_pRefCount = _pRefCount;
    }

public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    shared_ptr()
    {
        m_pRefCount = nullptr;
    }

    /// <summary>
    /// Constructor with pointer assignment.
    /// </summary>
    /// <param name="_ptr">Pointer of class T instance.</param>
    shared_ptr(T * _ptr)
    {
        m_pRefCount = nullptr;
        Assign(_ptr);
    }

    /// <summary>
    /// Move constructor.
    /// </summary>
    /// <param name="_rOther">Other class instance.</param>
    shared_ptr(shared_ptr && _rOther)
    {
        m_pRefCount = _rOther.m_pRefCount;
        _rOther.m_pRefCount = nullptr;
    }

    /// <summary>
    /// Copy constructor.
    /// </summary>
    /// <param name="_rOther">Other class instance.</param>
    shared_ptr(const shared_ptr & _rOther)
    {
        m_pRefCount = nullptr;
        Assign(_rOther.m_pRefCount);
    }

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~shared_ptr()
    {
        Assign((IRefCount<T> *) nullptr);
    }

    // get the contained pointer, not really needed but...
    T* GetPtr() const
    {
        if (m_pRefCount == nullptr)
        {
            return nullptr;
        }

        return m_pRefCount->GetPtr();
    }

    // To know if pointer is nullptr
    bool                                IsNull() const
    {
        return GetPtr() == nullptr;
    }

    // To know if pointer is not null
    bool                                IsNotNull() const
    {
        return GetPtr() != nullptr;
    }

    // assign another smart pointer
    shared_ptr& operator = (const shared_ptr& _rSmartPointer)
    {
        Assign(_rSmartPointer.m_pRefCount);
        return *this;
    }

    // assign pointer or NULL
    shared_ptr& operator = (T* _pSmartPointer)
    {
        Assign(_pSmartPointer);
        return *this;
    }
    // to access members of T
    T* operator ->()
    {
        assert(GetPtr() != nullptr);
        return GetPtr();
    }

    // to access members of T
    const T* operator ->() const
    {
        assert(GetPtr() != nullptr);
        return GetPtr();
    }

    // conversion to T* (for function calls)
    operator                            const T* () const
    {
        return GetPtr();
    }

    // conversion to T* (for function calls)
    operator T* ()
    {
        return GetPtr();
    }

    // utilities
    bool                                operator !()
    {
        return GetPtr() == nullptr;
    }
    bool                                operator ==(const shared_ptr & _rSmartPointer)
    {
        return GetPtr() == _rSmartPointer.GetPtr();
    }

    bool operator                       !=(const shared_ptr & _rSmartPointer)
    {
        return GetPtr() != _rSmartPointer.GetPtr();
    }
};
