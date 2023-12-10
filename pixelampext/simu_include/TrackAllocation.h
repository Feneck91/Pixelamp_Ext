/****************************************************************************************
 *
 * DESCRIPTIONS : Description and module use
 * ------------
 * D�tection des fuites m�moire en Release.
 * N'est actif que si _RELEASE_DEBUG est actif
 *
 ****************************************************************************************/

#ifndef INCLUDE_SRCDOC_WIZARD_TRACK_ALLOCATION_H
#define INCLUDE_SRCDOC_WIZARD_TRACK_ALLOCATION_H

typedef const char *            LPCSTR;

#ifdef _DEBUG
    void *                  OperatorNewPrivate(size_t nSize,const char* pszFileName,int iNumLine);
    void                    OperatorDeletePrivate(void* ptr,const char* pszFileName,int iNumLine);
    void * operator         new(size_t nSize,LPCSTR _pszFileName,int _iNumLine);
    void * operator         new[](size_t nSize);
    void * operator         new[](size_t nSize,LPCSTR _pszFileName,int _iNumLine);
    void operator           delete(void *_ptr);
    void operator           delete[](void *_ptr);
    void operator           delete(void* _ptr,LPCSTR _pszFileName,int _iNumLine);
    void operator           delete[](void* _ptr,LPCSTR _pszFileName,int _iNumLine);

    // Include STL (Standard Template Librairie)
    // Disable : Warning C4786: : identifier was truncated to '255' characters in the debug information
    #pragma warning(disable:4786)
    // L'includes suivant a �t� ajout�s sinon �a ne compile pas. Il y aura peut-�tre des news
    // qui �chaperont au track allocations...
    #include <memory>

    // Warning indiquant que si une exception est lanc�e pendant l'allocation,
    // l'objet ne pourra pas �tre d�truit...
    #pragma warning (disable:4291)

    // permet d'appeller le new avec des param�tres suppl�mentaires afin
    // de conna�tre qui a a fait l'allocation (nom du fichier, num�ro de ligne).
    #undef DEBUG_NEW
    #undef new
    #define DEBUG_NEW       ::new(__FILE__,__LINE__)
    #define new DEBUG_NEW
#endif // _RELEASE_DEBUG

#endif // INCLUDE_SRCDOC_WIZARD_TRACK_ALLOCATION_H
