//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "RFError.h"
#include "RFSession.h"


RFStatus RAPIDFIRE_API rfCreateEncodeSession(RFEncodeSession* session, RFProperties* properties)
{
    RFSession* pSession = nullptr;
    RFStatus rfStatus = createRFSession(&pSession, properties);

    if (rfStatus != RF_STATUS_OK)
    {
        *session = nullptr;
        return rfStatus;
    }

    // Create OpenCL context, compile OpenCL kernels
    rfStatus = pSession->createContext();
    if (rfStatus != RF_STATUS_OK)
    {
        delete pSession;
        *session = nullptr;
        return rfStatus;
    }

    *session = static_cast<RFEncodeSession>(pSession);

    return RF_STATUS_OK;
}


RFStatus RAPIDFIRE_API rfDeleteEncodeSession(RFEncodeSession* s)
{
    RFSession* pEncodeSession = reinterpret_cast<RFSession*>(*s);

    if (!pEncodeSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    delete pEncodeSession;
    *s = nullptr;
    return RF_STATUS_OK;
}


RFStatus RAPIDFIRE_API rfCreateEncoder(RFEncodeSession s, unsigned int uiWidth, unsigned int uiHeight, const RFEncodePreset p)
{
    RFSession* pSession = reinterpret_cast<RFSession*>(s);

    if (!pSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pSession->createEncoder(uiWidth, uiHeight, p);
}


RFStatus RAPIDFIRE_API rfCreateEncoder2(RFEncodeSession s, unsigned int uiWidth, unsigned int uiHeight, const RFProperties* properties)
{
    RFSession* pSession = reinterpret_cast<RFSession*>(s);

    if (!pSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pSession->createEncoder(uiWidth, uiHeight, properties);
}


RFStatus RAPIDFIRE_API rfSetEncodeParameter(RFEncodeSession s, const int param, RFProperties value)
{
    RFSession* pSession = reinterpret_cast<RFSession*>(s);

    if (!pSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pSession->setEncodeParameter(param, value);
}


RFStatus RAPIDFIRE_API rfGetEncodeParameter(RFEncodeSession s, const int param, RFProperties* value)
{
    RFSession* pSession = reinterpret_cast<RFSession*>(s);

    if (!pSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pSession->getEncodeParameter(param, *value);
}


RFStatus RAPIDFIRE_API rfRegisterRenderTarget(RFEncodeSession s, RFRenderTarget rt, unsigned int uiRTWidth, unsigned int uiRTHeight, unsigned int* idx)
{
    RFSession* pSession = reinterpret_cast<RFSession*>(s);

    if (!pSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    RFTexture rfTex;
    rfTex.rfRT = rt;

    return pSession->registerRenderTarget(rfTex, uiRTWidth, uiRTHeight, *idx);
}


RFStatus RAPIDFIRE_API rfRemoveRenderTarget(RFEncodeSession s, unsigned int idx)
{
    RFSession* pSession = reinterpret_cast<RFSession*>(s);

    if (!pSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pSession->removeRenderTarget(idx);
}


RFStatus RAPIDFIRE_API rfGetRenderTargetState(RFEncodeSession s, RFRenderTargetState* state, unsigned int idx)
{
    RFSession* pSession = reinterpret_cast<RFSession*>(s);

    if (!pSession)
    {
        *state = RF_STATE_INVALID;
        return RF_STATUS_INVALID_SESSION;
    }

    return pSession->getRenderTargetState(state, idx);
}


RFStatus RAPIDFIRE_API rfResizeSession(RFEncodeSession s, unsigned int uiWidth, unsigned int uiHeight)
{
    RFSession* pSession = reinterpret_cast<RFSession*>(s);

    if (!pSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pSession->resize(uiWidth, uiHeight);
}


RFStatus RAPIDFIRE_API rfEncodeFrame(RFEncodeSession s, unsigned int idx)
{
    RFSession* pEncodeSession = reinterpret_cast<RFSession*>(s);

    if (!pEncodeSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pEncodeSession->encodeFrame(idx);
}


RFStatus RAPIDFIRE_API rfGetEncodedFrame(RFEncodeSession session, unsigned int* uiSize, void** pBitStream)
{
    RFSession* pEncodeSession = reinterpret_cast<RFSession*>(session);

    if (!pEncodeSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pEncodeSession->getEncodedFrame(*uiSize, *pBitStream);
}


RFStatus RAPIDFIRE_API rfGetSourceFrame(RFEncodeSession session, unsigned int* uiSize, void** pBitStream)
{
    RFSession* pEncodeSession = reinterpret_cast<RFSession*>(session);

    if (!pEncodeSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pEncodeSession->getSourceFrame(*uiSize, *pBitStream);
}


RFStatus RAPIDFIRE_API rfGetMouseData(RFEncodeSession s, int iWaitForShapeChange, RFMouseData* md)
{
    RFSession* pEncodeSession = reinterpret_cast<RFSession*>(s);

    if (!pEncodeSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pEncodeSession->getMouseData(iWaitForShapeChange, *md);
}


RFStatus RAPIDFIRE_API rfReleaseEvent(RFEncodeSession s, RFNotification const rfNotification)
{
    RFSession* pEncodeSession = reinterpret_cast<RFSession*>(s);

    if (!pEncodeSession)
    {
        return RF_STATUS_INVALID_SESSION;
    }

    return pEncodeSession->releaseEvent(rfNotification);
}