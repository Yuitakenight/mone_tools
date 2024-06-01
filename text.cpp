#include <iostream>
#include <string>
#include <fstream>
#include <codecvt>
#include <Windows.h>
#include <iomanip>
#include <vector>

std::locale& GetCVT_UTF8()
{
    static auto cvtUTF8 = std::locale
    (
        std::locale::empty(),
        new std::codecvt_utf8<wchar_t, 0x10ffff, std::codecvt_mode(std::consume_header | std::generate_header | std::little_endian)>
        );
    return cvtUTF8;
}

void StrTOWStr(std::string& strString, std::wstring& wsString, unsigned int uCodePage)
{
    int len = 0;
    wchar_t* buffer = nullptr;

    len = MultiByteToWideChar(uCodePage, NULL, strString.c_str(), static_cast<int>(strString.size()), NULL, NULL);
    buffer = new wchar_t[len + 1];
    MultiByteToWideChar(uCodePage, NULL, strString.c_str(), static_cast<int>(strString.size()), buffer, len);
    buffer[len] = L'\0';
    wsString = buffer;
}

void WStrtoSte(std::wstring& wsString, std::string& strString, unsigned int uCodePage)
{
    int len = 0;
    char* buffer = nullptr;

    len = WideCharToMultiByte(uCodePage, NULL, wsString.c_str(), static_cast<int>(wsString.size()), NULL, NULL, NULL, NULL);
    buffer = new char[len + 1];
    WideCharToMultiByte(uCodePage, NULL, wsString.c_str(), static_cast<int>(wsString.size()), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    strString = buffer;
    delete[] buffer;
}

void dump()
{
    std::wofstream woText("001_dump.txt");
    woText.imbue(GetCVT_UTF8());
    std::ifstream iScript("0001.txt");
    if (!iScript.is_open())
    {
        std::cout << "Error opening file." << std::endl;
        return;
    }
    size_t count = 1;
    std::wstring wLine;
    for (std::string Line; std::getline(iScript, Line); count++)
    {
        if (((unsigned char)Line[0] > 0x7F && (unsigned char)Line[1] != 0x93) || (Line.find("//") == 0))
        {
            if ((unsigned char)Line[0] == 0x81 && (unsigned char)Line[1] == 0x40)
            {
                StrTOWStr(Line, wLine, 932);
                woText << '\n' << L"cot:" << std::setw(0x8) << std::setfill(L'0') << count;
                woText << '\n' << wLine.substr(1) << '\n';
            }
            else
            {
                StrTOWStr(Line, wLine, 932);
                woText << wLine << '\n';
            }
        }
    }

    woText.flush();
    woText.close();
    iScript.close();
}

void Inset(std::string strScript)
{
    std::vector<std::string> vecScript;
    std::ifstream iScript(strScript);
    if (iScript.is_open())
    {
        for (std::string Line; std::getline(iScript, Line);)
        {
            vecScript.push_back(Line);
        }
        iScript.close();
    }


    unsigned int pos = 0;
    std::vector<std::wstring> veText;
    std::wifstream wiText(strScript + ".txt");
    std::wstring NextLine;
    std::wstring prevLine;
    wiText.imbue(GetCVT_UTF8());

    if (wiText.is_open())
    {
        for (std::wstring Line; std::getline(wiText, Line);)
        {
            if (Line.find(L"cot:") == 0)
            {
                swscanf_s(Line.c_str(), L"cot:%d", &pos);
                continue;
   
            }
            else if (Line.size() >= 2)
            {
                if (prevLine.find(L"cot:") == 0 && NextLine.size() < 2)
                {
                    pos = pos;
                    continue;
                }
                else
                {
                    pos = pos + 1;
                    continue;
                }
            }
            else
            {
                pos = pos;
                continue;


            }
            

        }

        wiText.close();
    }
    
}
            

int main()
{
    Inset("001_dump");
    dump();

    return 0;
}
