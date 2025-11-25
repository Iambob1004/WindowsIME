// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include "Private.h"
#include "Globals.h"
#include "SampleIME.h"
#include "CompositionProcessorEngine.h"
#include "LanguageBar.h"

BOOL CSampleIME::VerifySampleIMECLSID(_In_ REFCLSID clsid)
{
    if (IsEqualCLSID(clsid, Global::SampleIMECLSID))
    {
        return TRUE;
    }
    return FALSE;
}

//+---------------------------------------------------------------------------
//
// ITfActiveLanguageProfileNotifySink::OnActivated
//
// Sink called by the framework when changes activate language profile.
//----------------------------------------------------------------------------

STDAPI CSampleIME::OnActivated(_In_ REFCLSID clsid, _In_ REFGUID guidProfile, _In_ BOOL isActivated)
{
    guidProfile;

    if (FALSE == VerifySampleIMECLSID(clsid))
    {
        return S_OK;
    }

    if (isActivated)
    {
        _AddTextProcessorEngine();
    }

    if (nullptr == _pCompositionProcessorEngine)
    {
        return S_OK;
    }
    // ����J�k�}�Ү�
    if (isActivated)
    {
        _pCompositionProcessorEngine->ShowAllLanguageBarIcons();
        _pCompositionProcessorEngine->ConversionModeCompartmentUpdated(_pThreadMgr);

        // �]�wicon��Dictionary
        ChechLanguageIcon = true;
        Global::g_DictionaryFileName = L"43_Sakizaya.txt";

        if (_pCompositionProcessorEngine)
        {
            _pCompositionProcessorEngine->SetupDictionaryFile();
        }
        if (_pCompositionProcessorEngine && _pCompositionProcessorEngine->_pLanguageBar_IMEMode &&
            _pCompositionProcessorEngine->_pLanguageBar_IMEMode->_pLangBarItemSink)
        {
            _pCompositionProcessorEngine->_pLanguageBar_IMEMode->_pLangBarItemSink->OnUpdate(TF_LBI_STATUS | TF_LBI_ICON);
        }

        ITfDocumentMgr *pDocMgrFocus = nullptr;
        if (SUCCEEDED(_pThreadMgr->GetFocus(&pDocMgrFocus)) && pDocMgrFocus != nullptr)
        {
            _SetInputModeToChinese(pDocMgrFocus);
            pDocMgrFocus->Release();
        }
    }
    else
    {
        _DeleteCandidateList(FALSE, nullptr);

        _pCompositionProcessorEngine->HideAllLanguageBarIcons();
    }

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _InitActiveLanguageProfileNotifySink
//
// Advise a active language profile notify sink.
//----------------------------------------------------------------------------

BOOL CSampleIME::_InitActiveLanguageProfileNotifySink()
{
    ITfSource *pSource = nullptr;
    BOOL ret = FALSE;

    if (_pThreadMgr->QueryInterface(IID_ITfSource, (void **)&pSource) != S_OK)
    {
        return ret;
    }

    if (pSource->AdviseSink(IID_ITfActiveLanguageProfileNotifySink, (ITfActiveLanguageProfileNotifySink *)this, &_activeLanguageProfileNotifySinkCookie) != S_OK)
    {
        _activeLanguageProfileNotifySinkCookie = TF_INVALID_COOKIE;
        goto Exit;
    }

    ret = TRUE;

Exit:
    pSource->Release();
    return ret;
}

//+---------------------------------------------------------------------------
//
// _UninitActiveLanguageProfileNotifySink
//
// Unadvise a active language profile notify sink.  Assumes we have advised one already.
//----------------------------------------------------------------------------

void CSampleIME::_UninitActiveLanguageProfileNotifySink()
{
    ITfSource *pSource = nullptr;

    if (_activeLanguageProfileNotifySinkCookie == TF_INVALID_COOKIE)
    {
        return; // never Advised
    }

    if (_pThreadMgr->QueryInterface(IID_ITfSource, (void **)&pSource) == S_OK)
    {
        pSource->UnadviseSink(_activeLanguageProfileNotifySinkCookie);
        pSource->Release();
    }

    _activeLanguageProfileNotifySinkCookie = TF_INVALID_COOKIE;
}
