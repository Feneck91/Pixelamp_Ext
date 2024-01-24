/****************************************************************************************
 *
 * DESCRIPTIONS : Description and module use
 * ------------
 * D�tection des fuites m�moire en Release.
 *
 ****************************************************************************************/

#ifdef _DEBUG

#pragma once
#include <string>
#include <assert.h>

#define  _AFXDLL
#include <afxcoll.h>

#include "../simu_include/TrackAllocation.h"

// Obligatoire de retirer la d�finition du new : sinon, remplac� par la macro
// et le project ne peut pas compiler
#undef new
#undef delete
#include <set>

void* operator new(size_t nSize, LPCSTR _pszFileName, int _iNumLine)
{
    return OperatorNewPrivate(nSize, _pszFileName, _iNumLine);
}

void* operator         new[](size_t nSize)
{
    return OperatorNewPrivate(nSize, "Fichier inconnu (STL ?)", 0);
}

void* operator new[](size_t nSize, LPCSTR _pszFileName, int _iNumLine)
{
    return OperatorNewPrivate(nSize, _pszFileName, _iNumLine);
}

void operator delete(void* _ptr)
{
    OperatorDeletePrivate(_ptr, NULL, 0);
}
void operator delete[](void* _ptr)
{
    OperatorDeletePrivate(_ptr, NULL, 0);
}

void operator delete(void* _ptr, LPCSTR _pszFileName, int _iNumLine)
{
    OperatorDeletePrivate(_ptr, _pszFileName, _iNumLine);
}

void operator delete[](void* _ptr, LPCSTR _pszFileName, int _iNumLine)
{
    OperatorDeletePrivate(_ptr, _pszFileName, _iNumLine);
}

/** Classe utilis�e pour enregistrer tous les �l�ments allou�s par l'op�rateur new.
*
* @see
*
* @author St�phane
* @version Name : GfxExtTools<br>
*          R�vision : <b>1.0</b>
*/
struct CGfxExtToolsAllocationMem
{
    /** Pointeur vers la zone allou�e.
    *
    */
    void*                   m_pAllocatedMem;

    /** Fichier a qui a cr�� cette allocation dynamique.
    *
    */
    const char *            m_pszFileName;

    /** Ligne du code source o� l'allocation a �t� faite.
    *
    */
    int                     m_iNumLine;

    /** Taille de la zone allou�e.
    *
    */
    size_t                  m_stSizeAllocatedMem;
};

typedef std::set<CGfxExtToolsAllocationMem *>   tdListeTrackAllocation;

template<> struct std::less<CGfxExtToolsAllocationMem*>
{
    bool operator()(CGfxExtToolsAllocationMem* const & _X, CGfxExtToolsAllocationMem* const & _Y) const
    {
        return (_X->m_pAllocatedMem < _Y->m_pAllocatedMem);
    }
};

/** Liste de toutes les zones m�moire allou�es.
*
* @see g_crtsecPrivateNewDelete
*/
tdListeTrackAllocation *                        g_pTrackAllocationList;

/** Flag global indiquant que le test d'allocation / D�sallocation est termin�.
*
* Une fois la fonction de test des memory leaks effectu�e, ce flag est positionn� � true
* de sorte que chaque allocation / d�sallocation faite utilise ensuite les malloc / free
* classiques sans m�moriser les allocations dans la liste.
*/
bool                                            g_bEnd=false;

#undef new
#undef delete
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0)
    {
        assert(false); // throw std::runtime_error("Error during formatting."); }
    }
    auto size = static_cast<size_t>(size_s);
    char *pBuffer = nullptr;
    std::string strRet;
    pBuffer = new char[size];
    std::snprintf(pBuffer, size, format.c_str(), args ...);
    strRet = std::string(pBuffer, pBuffer + size - 1); // We don't want the '\0' inside
    delete []pBuffer;

    return strRet;
}

void CallDetectMemoryLeaks()
{
    g_bEnd=true;
    bool bMemoryLeaks=false;

    if (g_pTrackAllocationList)
    {
        if (g_pTrackAllocationList->size())
        {
            OutputDebugStringA("Memory leaks detected !!\n");
            bMemoryLeaks=true;
        }
        else
        {
            OutputDebugStringA("Memory cleaned perfectly.\n");
        }

        while (g_pTrackAllocationList->size())
        { // Il y a des fuites m�moire
            tdListeTrackAllocation::iterator it=g_pTrackAllocationList->begin();
            std::string string;

            string = string_format("Memory block not deleted ptr=%X (%d bytes). Allocated in %s (%d)\n",(*it)->m_pAllocatedMem,(*it)->m_stSizeAllocatedMem,(*it)->m_pszFileName,(*it)->m_iNumLine);
            OutputDebugStringA(string.c_str());
//            free((*it)->m_pAllocatedMem);
            ::delete *it;
            g_pTrackAllocationList->erase(it);
        }
    }
}

// Lorsque le premier new est cr��, il test la variable globale pgFirstAllocationMem. Si elle est
// nulle, alors il alloue un buffer pour m�moriser cette variable et appel en suite la fonction
// atexit (dans fonction OperatorNewPrivate). Ceci permet d'appeller la fonction de test des fuites
// m�moire � la fin de l'application. Apr�s que tous les process aient �t� arr�t�s, lorsque toutes les
// variables membres statiques des classes aient �t�es d�truites.
// A ce moment, les fonctions qui g�rent la m�morisation des fuites m�moires sont d�sactiv�es. Le
// programme ne peut donc plus tracer les fuites m�moires... Dans les CString par exemple
// lors de l'affichage des messages....
// Normalement, il ne doit pas y avoir de fuite.... donc cette partie de code n'est pas utilis�e.
void OnExitApplication()
{
    CallDetectMemoryLeaks();
}

void *OperatorNewPrivate(size_t nSize,const char* pszFileName,int iNumLine)
{
    if (g_bEnd)
    {
        return malloc(nSize);
    }

    // Si c'est la premi�re allocation, cr�er la liste qui va m�moriser les �l�ments
    if (g_pTrackAllocationList == nullptr)
    {
        atexit(OnExitApplication); // Fonction � appeler lorsque l'application se termine.
        g_pTrackAllocationList = ::new tdListeTrackAllocation;
    }

    // Allouer zone m�moire demand�e par le programme
    void *ptr=malloc(nSize);

    if (ptr)
    {
        CGfxExtToolsAllocationMem *pItemMem=(CGfxExtToolsAllocationMem *) malloc(sizeof(CGfxExtToolsAllocationMem));
        pItemMem->m_pAllocatedMem = ptr;
        pItemMem->m_stSizeAllocatedMem = nSize;
        pItemMem->m_pszFileName = pszFileName;
        pItemMem->m_iNumLine = iNumLine;
        if (!g_pTrackAllocationList->insert(pItemMem).second)
        {
            OutputDebugStringA("Erreur lors de l'ajout de la zone allou�e dans la liste\n");
        }
    }

    return ptr;
}

void OperatorDeletePrivate(void* ptr,const char* pszFileName,int iNumLine)
{
    if (g_bEnd)
    {
        free(ptr);
        return;
    }

    CGfxExtToolsAllocationMem localSearch;
    localSearch.m_pAllocatedMem=ptr;
    tdListeTrackAllocation::iterator itFind = g_pTrackAllocationList->find(&localSearch);

    if (itFind != g_pTrackAllocationList->end())
    {
        free(*itFind);                          // Destruction de la classe allou�e
        g_pTrackAllocationList->erase(itFind);  // Suppression de la liste
    }
    // Sinon, ne rien faire, ce cas arrive tr�s souvent
    // else
    // {
    //    ASSERT(FALSE,"Erreur, bloc allocation non trouv�e");
    // }

    free(ptr);  // destruction zone allou�e
}

#endif // _RELEASE_DEBUG
