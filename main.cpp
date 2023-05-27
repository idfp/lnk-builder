// CreateLink - Uses the Shell's IShellLink and IPersistFile interfaces 
//              to create and store a shortcut to the specified object. 
//
// Returns the result of calling the member functions of the interfaces. 
//
// Parameters:
// lpszPathObj  - Address of a buffer that contains the path of the object,
//                including the file name.
// lpszPathLink - Address of a buffer that contains the path where the 
//                Shell link is to be stored, including the file name.
// lpszDesc     - Address of a buffer that contains a description of the 
//                Shell link, stored in the Comment field of the link
//                properties.

// #include "stdafx.h"
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include "string"
#include "stdio.h"
// LPSTR curdir, LPSTR icon, int iconIndex
HRESULT CreateLink(LPSTR target, LPSTR args, LPSTR linkFile, LPSTR desc, int showmode) 
{ 
    printf("creating shortcut file...\n");
    HRESULT hres; 
    IShellLink* psl;   
    IPersistFile* ppf; 
    WCHAR wsz[MAX_PATH]; 
    int wideCharWritten;
    CoInitialize(NULL);
    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
    if (SUCCEEDED(hres)) 
    { 
        printf("successfully created instance...\n");

        // Set the path to the shortcut target and add the description. 
        psl->SetPath(target); 
        psl->SetArguments(args); 
        psl->SetDescription(desc); 
        psl->SetShowCmd(showmode); 
        // psl->SetWorkingDirectory(curdir);
        // psl->SetIconLocation(icon, iconIndex); 

        // Query IShellLink for the IPersistFile interface, used for saving the 
        // shortcut in persistent storage. 
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 

        if (SUCCEEDED(hres)) 
        { 
            printf("successfully query the interface...\n");

            // Ensure that the string is Unicode. 
            MultiByteToWideChar(CP_ACP, 0, linkFile, -1, wsz, MAX_PATH); 

            // Save the link by calling IPersistFile::Save.
            printf("saving file...");
            hres = ppf->Save(wsz, TRUE); 
            ppf->Release(); 
        } 
        psl->Release(); 
    } 
    return hres; 
}

int main(){
    LPSTR res = "D:\\workspace\\lnk-builder\\chrome.lnk";
    LPSTR desc = "whatever";
    LPSTR target = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
    LPSTR args = "-Nop -sta -noni -w hidden Start-Process calc.exe";
    CreateLink(target, args, res, desc, 7);
    return 0;
}