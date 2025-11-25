// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include "Private.h"
#include "KeyHandlerEditSession.h"
#include "EditSession.h"
#include "SampleIME.h"
#include "CompositionProcessorEngine.h"
#include "KeyStateCategory.h"
#include "log.h"
//////////////////////////////////////////////////////////////////////
//
//    ITfEditSession
//        CEditSessionBase
// CKeyHandlerEditSession class
//
//////////////////////////////////////////////////////////////////////

//+---------------------------------------------------------------------------
//
// CKeyHandlerEditSession::DoEditSession
//
//----------------------------------------------------------------------------

STDAPI CKeyHandlerEditSession::DoEditSession(TfEditCookie ec)
{
    HRESULT hResult = S_OK;

    CKeyStateCategoryFactory* pKeyStateCategoryFactory = CKeyStateCategoryFactory::Instance();
    if ((_uCode == 222 || _uCode == 32) && _KeyState.Category == CATEGORY_INVOKE_COMPOSITION_EDIT_SESSION)
    {
        _KeyState.Category = CATEGORY_COMPOSING;
    }
    CKeyStateCategory* pKeyStateCategory = pKeyStateCategoryFactory->MakeKeyStateCategory(_KeyState.Category, _pTextService);
    if (pKeyStateCategory)
    {
        KeyHandlerEditSessionDTO keyHandlerEditSessioDTO(ec, _pContext, _uCode,_wch, _KeyState.Function);

        if (Word_checkedit == true)
        {
            hResult = pKeyStateCategory->KeyStateHandler(_KeyState.Function, keyHandlerEditSessioDTO);
            pKeyStateCategory->Release();
            pKeyStateCategoryFactory->Release();
        }
        else
        {
            pKeyStateCategory->Release();
            pKeyStateCategoryFactory->Release();
        }

    }

    return hResult;
}
