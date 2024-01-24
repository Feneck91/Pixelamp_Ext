/****************************************************************************************
 *
 * DESCRIPTIONS : Description and module use
 * ------------
 * Détection des fuites mémoire en Release.
 *
 ****************************************************************************************/

#ifdef _DEBUG

#pragma once
#include <string>
#include <assert.h>

#define  _AFXDLL
#include <afxcoll.h>

#include "../simu_include/TrackAllocation.h"

// Obligatoire de retirer la définition du new : sinon, remplacé par la macro
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

/** Classe utilisée pour enregistrer tous les éléments alloués par l'opérateur new.
*
* @see
*
* @author Stéphane
* @version Name : GfxExtTools<br>
*          Révision : <b>1.0</b>
*/
struct CGfxExtToolsAllocationMem
{
    /** Pointeur vers la zone allouée.
    *
    */
    void*                   m_pAllocatedMem;

    /** Fichier a qui a créé cette allocation dynamique.
    *
    */
    const char *            m_pszFileName;

    /** Ligne du code source où l'allocation a été faite.
    *
    */
    int                     m_iNumLine;

    /** Taille de la zone allouée.
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

/** Liste de toutes les zones mémoire allouées.
*
* @see g_crtsecPrivateNewDelete
*/
tdListeTrackAllocation *                        g_pTrackAllocationList;

/** Flag global indiquant que le test d'allocation / Désallocation est terminé.
*
* Une fois la fonction de test des memory leaks effectuée, ce flag est positionné à true
* de sorte que chaque allocation / désallocation faite utilise ensuite les malloc / free
* classiques sans mémoriser les allocations dans la liste.
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
        { // Il y a des fuites mémoire
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

// Lorsque le premier new est créé, il test la variable globale pgFirstAllocationMem. Si elle est
// nulle, alors il alloue un buffer pour mémoriser cette variable et appel en suite la fonction
// atexit (dans fonction OperatorNewPrivate). Ceci permet d'appeller la fonction de test des fuites
// mémoire à la fin de l'application. Après que tous les process aient été arrétés, lorsque toutes les
// variables membres statiques des classes aient étées détruites.
// A ce moment, les fonctions qui gèrent la mémorisation des fuites mémoires sont désactivées. Le
// programme ne peut donc plus tracer les fuites mémoires... Dans les CString par exemple
// lors de l'affichage des messages....
// Normalement, il ne doit pas y avoir de fuite.... donc cette partie de code n'est pas utilisée.
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

    // Si c'est la première allocation, créer la liste qui va mémoriser les éléments
    if (g_pTrackAllocationList == nullptr)
    {
        atexit(OnExitApplication); // Fonction à appeler lorsque l'application se termine.
        g_pTrackAllocationList = ::new tdListeTrackAllocation;
    }

    // Allouer zone mémoire demandée par le programme
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
            OutputDebugStringA("Erreur lors de l'ajout de la zone allouée dans la liste\n");
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
        free(*itFind);                          // Destruction de la classe allouée
        g_pTrackAllocationList->erase(itFind);  // Suppression de la liste
    }
    // Sinon, ne rien faire, ce cas arrive très souvent
    // else
    // {
    //    ASSERT(FALSE,"Erreur, bloc allocation non trouvée");
    // }

    free(ptr);  // destruction zone allouée
}

#endif // _RELEASE_DEBUG
