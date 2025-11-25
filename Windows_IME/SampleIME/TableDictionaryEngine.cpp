// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include "Private.h"
#include "TableDictionaryEngine.h"
#include "DictionarySearch.h"
#include "log.h"
//+---------------------------------------------------------------------------
//
// CollectWord
//
//----------------------------------------------------------------------------

VOID CTableDictionaryEngine::CollectWord(_In_ CStringRange *pKeyCode, _Inout_ CSampleImeArray<CStringRange> *pWordStrings)
{
    CDictionaryResult* pdret = nullptr;
    CDictionarySearch dshSearch(_locale, _pDictionaryFile, pKeyCode);

    while (dshSearch.FindPhrase(&pdret))
    {
        for (UINT index = 0; index < pdret->_FindPhraseList.Count(); index++)
        {
            CStringRange* pPhrase = nullptr;
            pPhrase = pWordStrings->Append();
            if (pPhrase)
            {
                *pPhrase = *pdret->_FindPhraseList.GetAt(index);
            }
        }

        delete pdret;
        pdret = nullptr;
    }
}

VOID CTableDictionaryEngine::CollectWord(_In_ CStringRange *pKeyCode, _Inout_ CSampleImeArray<CCandidateListItem> *pItemList)
{
    CDictionaryResult* pdret = nullptr;
    CDictionarySearch dshSearch(_locale, _pDictionaryFile, pKeyCode);

    while (dshSearch.FindPhrase(&pdret))
    {
        for (UINT iIndex = 0; iIndex < pdret->_FindPhraseList.Count(); iIndex++)
        {
            CCandidateListItem* pLI = nullptr;
            pLI = pItemList->Append();
            if (pLI)
            {
                pLI->_ItemString.Set(*pdret->_FindPhraseList.GetAt(iIndex));
                pLI->_FindKeyCode.Set(pdret->_FindKeyCode.Get(), pdret->_FindKeyCode.GetLength());
            }
        }

        delete pdret;
        pdret = nullptr;
    }
}

//+---------------------------------------------------------------------------
//
// CollectWordForWildcard
//多增加_In_ CStringRange *onKeycode用來記錄最後一個輸入的按鍵
// 
//----------------------------------------------------------------------------

VOID CTableDictionaryEngine::CollectWordForWildcard(_In_ CStringRange *pKeyCode, _Inout_ CSampleImeArray<CCandidateListItem> *pItemList,_In_ CStringRange *onKeycode)
{
    CDictionaryResult* pdret = nullptr;
    CDictionaryResult* pdretSpecial = nullptr;
    CDictionarySearch dshSearch(_locale, _pDictionaryFile, pKeyCode);//搜尋原本的字
    CDictionarySearch dshSearchSpecial(_locale, _pDictionaryFile, onKeycode);//搜尋特殊符號

    switch (*onKeycode->Get())
    {
    case 114: // 'r'
    case 105: // 'i'
        // 根據有幾個特殊符號執行幾次
        if (dshSearchSpecial.FindPhraseForWildcard(&pdretSpecial))
        {
            CCandidateListItem* pLI_Special = pItemList->Append();
            if (pLI_Special)
            {
                pLI_Special->_ItemString.Set(*pdretSpecial->_FindPhraseList.GetAt(0));
                pLI_Special->_FindKeyCode.Set(pdretSpecial->_FindKeyCode.Get(), pdretSpecial->_FindKeyCode.GetLength());
            }
            delete pdretSpecial;
            pdretSpecial = nullptr;
        }
        break;
    case 117: // 'u'
        // 根據有幾個特殊符號執行幾次
        for (int i = 0; i < 2; i++)
        {
            if (dshSearchSpecial.FindPhraseForWildcard(&pdretSpecial))
            {
                CCandidateListItem* pLI_Special = pItemList->Append();
                if (pLI_Special)
                {
                    pLI_Special->_ItemString.Set(*pdretSpecial->_FindPhraseList.GetAt(0));
                    pLI_Special->_FindKeyCode.Set(pdretSpecial->_FindKeyCode.Get(), pdretSpecial->_FindKeyCode.GetLength());
                }
                delete pdretSpecial;
                pdretSpecial = nullptr;
            }
        }
        break;
    case 101: // 'e'
        // 根據有幾個特殊符號執行幾次
        for (int i = 0; i < 2; i++)
        {
            if (dshSearchSpecial.FindPhraseForWildcard(&pdretSpecial))
            {
                CCandidateListItem* pLI_Special = pItemList->Append();
                if (pLI_Special)
                {
                    pLI_Special->_ItemString.Set(*pdretSpecial->_FindPhraseList.GetAt(0));
                    pLI_Special->_FindKeyCode.Set(pdretSpecial->_FindKeyCode.Get(), pdretSpecial->_FindKeyCode.GetLength());
                }
                delete pdretSpecial;
                pdretSpecial = nullptr;
            }
        }
        break;

    case 700: // 02BC特殊符號
        // 根據有幾個特殊符號執行幾次
        
        for (int i = 0; i < 6; i++)
        {
            if (dshSearchSpecial.FindPhraseForWildcard(&pdretSpecial))
            {
                CCandidateListItem* pLI_Special = pItemList->Append();
                if (pLI_Special)
                {
                    pLI_Special->_ItemString.Set(*pdretSpecial->_FindPhraseList.GetAt(0));
                    pLI_Special->_FindKeyCode.Set(pdretSpecial->_FindKeyCode.Get(), pdretSpecial->_FindKeyCode.GetLength());
                }
                delete pdretSpecial;
                pdretSpecial = nullptr;
            }
        }
        break;
    }

    
    //從FindPhraseForWildcard改成FindConvertedStringForWildcard 這樣可以搜尋到所有符合的字詞
    while (dshSearch.FindConvertedStringForWildcard(&pdret))
    {
        UINT iIndex;
        if (*onKeycode->Get() == 700 && pdret->_FindKeyCode.GetLength()==1)
        {
            iIndex = 1;
        }
        else
        {
            iIndex = 0;
        }
        for  (iIndex ; iIndex < pdret->_FindPhraseList.Count(); iIndex++)
        {
            
            CCandidateListItem* pLI = nullptr;
            pLI = pItemList->Append();
            if (pLI)
            {
                pLI->_ItemString.Set(*pdret->_FindPhraseList.GetAt(iIndex));
                pLI->_FindKeyCode.Set(pdret->_FindKeyCode.Get(), pdret->_FindKeyCode.GetLength());
            }
        }

        delete pdret;
        pdret = nullptr;
    }
}

//+---------------------------------------------------------------------------
//
// CollectWordFromConvertedStringForWildcard
//
//----------------------------------------------------------------------------

VOID CTableDictionaryEngine::CollectWordFromConvertedStringForWildcard(_In_ CStringRange *pString, _Inout_ CSampleImeArray<CCandidateListItem> *pItemList)
{
    CDictionaryResult* pdret = nullptr;
    CDictionarySearch dshSearch(_locale, _pDictionaryFile, pString);
    while (dshSearch.FindConvertedStringForWildcard(&pdret)) // TAIL ALL CHAR MATCH
    {
        for (UINT index = 0; index < pdret->_FindPhraseList.Count(); index++)
        {
            CCandidateListItem* pLI = nullptr;
            pLI = pItemList->Append();
            if (pLI)
            {
                pLI->_ItemString.Set(*pdret->_FindPhraseList.GetAt(index));
                pLI->_FindKeyCode.Set(pdret->_FindKeyCode.Get(), pdret->_FindKeyCode.GetLength());
            }
        }

        delete pdret;
        pdret = nullptr;
    }
}

