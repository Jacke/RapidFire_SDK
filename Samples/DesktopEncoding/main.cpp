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

/////////////////////////////////////////////////////////////////////////////////////////
//
// Desktop encoding shows how to use RapidFire to grab the desktop and encode it to
// a H264 stream. The stream is dumped to a file.
// First a session is created which is configured to grab the desktop and to use 
// the AMF encoder (HW encoder).
// 
/////////////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>

#include <windows.h>

#include "RapidFire.h"

#define NUM_FRAMES 500

using namespace std;


int main(int argc, char** argv)
{
    RFStatus        rfStatus = RF_STATUS_OK;
    RFEncodeSession rfSession = nullptr;

    RFProperties props[] = { RF_ENCODER, static_cast<RFProperties>(RF_AMF),
                             RF_DESKTOP, static_cast<RFProperties>(1),
                             0 };

    rfStatus = rfCreateEncodeSession(&rfSession, props);

    if (rfStatus != RF_STATUS_OK)
    {
        cerr << "Failed to create desktop encoding session!" << endl;
        return -1;
    }

    cout << "Created desktop session" << endl;

    // Get dimension of primary display
    unsigned int uiStreamWidth  = GetSystemMetrics(SM_CXSCREEN);
    unsigned int uiStreamHeight = GetSystemMetrics(SM_CYSCREEN);

    // Make sure dimension does not exceed 1080p.
    // This is the max resolution supported by the H.264 level 4.2 used in the preset.
    if (uiStreamWidth > 1920)
    {
        uiStreamWidth = 1920;
    }

    if (uiStreamHeight > 1080)
    {
        uiStreamHeight = 1080;
    }

    cout << "   Stream width  " << uiStreamWidth << endl;
    cout << "   Stream height " << uiStreamHeight << endl;

    // Create encoder and define the size of the stream.
    // RF will scale the desktop to the screen size.
    rfStatus = rfCreateEncoder(rfSession, uiStreamWidth, uiStreamHeight, RF_PRESET_BALANCED);

    if (rfStatus != RF_STATUS_OK)
    {
        cerr << "Failed to create HW encoder!" << endl;
        rfDeleteEncodeSession(&rfSession);
        return -1;
    }

    cout << "Created encoder" << endl;

    fstream outFile;

    // Open a file to which the H.264 frames are dumped.
    // The file can be displayed using ffplay.
    outFile.open("./Desktop.h264", fstream::out | fstream::trunc | fstream::binary);

    unsigned int uiBitStreamSize = 0;
    void*        pBitStreamdata  = nullptr;

    cout << "Starting to encode " << NUM_FRAMES << " frames" << endl;

    for (int i = 0; i < NUM_FRAMES; ++i)
    {
        if (rfEncodeFrame(rfSession, 0) == RF_STATUS_OK)
        {
            // Check if encoded frame is ready
            if (rfGetEncodedFrame(rfSession, &uiBitStreamSize, &pBitStreamdata) == RF_STATUS_OK)
            {
                if (uiBitStreamSize > 0)
                {
                    outFile.write(static_cast<char*>(pBitStreamdata), uiBitStreamSize);
                }
            }
        }
    }

    rfDeleteEncodeSession(&rfSession);

    outFile.close();

    cout << "Dumped frames to file" << endl;

    return 0;
}