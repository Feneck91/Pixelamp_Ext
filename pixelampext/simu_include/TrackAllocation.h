/****************************************************************************************
 *
 * DESCRIPTIONS : Description and module use
 * ------------
 * Détection des fuites mémoire en Release.
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
    // L'includes suivant a été ajoutés sinon ça ne compile pas. Il y aura peut-être des news
    // qui échaperont au track allocations...
    #include <memory>

    // Warning indiquant que si une exception est lancée pendant l'allocation,
    // l'objet ne pourra pas être détruit...
    #pragma warning (disable:4291)

    // permet d'appeller le new avec des paramètres supplémentaires afin
    // de connaître qui a a fait l'allocation (nom du fichier, numéro de ligne).
    #undef DEBUG_NEW
    #undef new
    #define DEBUG_NEW       ::new(__FILE__,__LINE__)
    #define new DEBUG_NEW
#endif // _RELEASE_DEBUG

#endif // INCLUDE_SRCDOC_WIZARD_TRACK_ALLOCATION_H
