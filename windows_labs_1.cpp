#include <iostream>
#include <string>

#include "utils.cpp"
#include "windows.h"

using namespace std;

char askDriveLetter() {
    char c;
    cout << "Введите букву диска: ";
    cin >> c;
    return toupper(c);
}

bool isDriveExists(char c) {
    DWORD drives = GetLogicalDrives();
    short shift = c - 'A';
    drives = drives >> shift;
    return (drives % 2 == 1);
}

string _getDriveType(char c) {
    char d[3];
    d[0] = c;
    d[1] = ':';
    d[2] = '\\';
    d[3] = '\0';
    UINT type = GetDriveType(d);
    switch (type) {
        case DRIVE_UNKNOWN:
            return "неизвестный тип";
        case DRIVE_NO_ROOT_DIR:
            return "неправильный путь";
        case DRIVE_REMOVABLE:
            return "съёмный диск";
        case DRIVE_FIXED:
            return "фиксированный диск";
        case DRIVE_REMOTE:
            return "удалённый диск";
        case DRIVE_CDROM:
            return "CD-ROM диск";
        case DRIVE_RAMDISK:
            return "RAM диск";
    }
    return "Неизвестный тип";
}

/**
* docs [EN]: https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationa
* docs [RU]: http://rusproject.narod.ru/winapi/g/getvolumeinformation.html
*/
void printDriveFlags(DWORD FileSF) {
    cout << "Системные флаги: " << endl;
    if (FileSF & FILE_CASE_PRESERVED_NAMES) {
        cout << "\t"
             << "FILE_CASE_PRESERVED_NAMES" << endl;
    }
    if (FileSF & FILE_CASE_SENSITIVE_SEARCH) {
        cout << "\t"
             << "FILE_CASE_SENSITIVE_SEARCH" << endl;
    }
    if (FileSF & FILE_FILE_COMPRESSION) {
        cout << "\t"
             << "FILE_FILE_COMPRESSION" << endl;
    }
    if (FileSF & FILE_NAMED_STREAMS) {
        cout << "\t"
             << "FILE_NAMED_STREAMS" << endl;
    }
    if (FileSF & FILE_PERSISTENT_ACLS) {
        cout << "\t"
             << "FILE_PERSISTENT_ACLS" << endl;
    }
    if (FileSF & FILE_READ_ONLY_VOLUME) {
        cout << "\t"
             << "FILE_READ_ONLY_VOLUME" << endl;
    }
    if (FileSF & FILE_SEQUENTIAL_WRITE_ONCE) {
        cout << "\t"
             << "FILE_SEQUENTIAL_WRITE_ONCE" << endl;
    }
    if (FileSF & FILE_SUPPORTS_ENCRYPTION) {
        cout << "\t"
             << "FILE_SUPPORTS_ENCRYPTION" << endl;
    }
    if (FileSF & FILE_SUPPORTS_EXTENDED_ATTRIBUTES) {
        cout << "\t"
             << "FILE_SUPPORTS_EXTENDED_ATTRIBUTES" << endl;
    }
    if (FileSF & FILE_SUPPORTS_HARD_LINKS) {
        cout << "\t"
             << "FILE_SUPPORTS_HARD_LINKS" << endl;
    }
    if (FileSF & FILE_SUPPORTS_OBJECT_IDS) {
        cout << "\t"
             << "FILE_SUPPORTS_OBJECT_IDS" << endl;
    }
    if (FileSF & FILE_SUPPORTS_OPEN_BY_FILE_ID) {
        cout << "\t"
             << "FILE_SUPPORTS_OPEN_BY_FILE_ID" << endl;
    }
    if (FileSF & FILE_SUPPORTS_REPARSE_POINTS) {
        cout << "\t"
             << "FILE_SUPPORTS_REPARSE_POINTS" << endl;
    }
    if (FileSF & FILE_SUPPORTS_SPARSE_FILES) {
        cout << "\t"
             << "FILE_SUPPORTS_SPARSE_FILES" << endl;
    }
    if (FileSF & FILE_SUPPORTS_TRANSACTIONS) {
        cout << "\t"
             << "FILE_SUPPORTS_TRANSACTIONS" << endl;
    }
    if (FileSF & FILE_SUPPORTS_USN_JOURNAL) {
        cout << "\t"
             << "FILE_SUPPORTS_USN_JOURNAL" << endl;
    }
    if (FileSF & FILE_UNICODE_ON_DISK) {
        cout << "\t"
             << "FILE_UNICODE_ON_DISK" << endl;
    }
    if (FileSF & FILE_VOLUME_IS_COMPRESSED) {
        cout << "\t"
             << "FILE_VOLUME_IS_COMPRESSED" << endl;
    }
    if (FileSF & FILE_VOLUME_QUOTAS) {
        cout << "\t"
             << "FILE_VOLUME_QUOTAS" << endl;
    }
}

void printVolumeInformation(char c) {
    char d[3];
    d[0] = c;
    d[1] = ':';
    d[2] = '\\';
    d[3] = '\0';

    char NameBuffer[MAX_PATH];
    char SysNameBuffer[MAX_PATH];
    DWORD VSNumber;
    DWORD MCLength;
    DWORD FileSF;

    GetVolumeInformation(
        d,                       // in Буква диска
        NameBuffer,              // out Название диска в системе
        sizeof(NameBuffer),      // in Длина предыдущего параметра
        &VSNumber,               // out Серийный номер диска
        &MCLength,               // out Максимальная длина названия файла
        &FileSF,                 // out Системные флаги
        SysNameBuffer,           // out Название файловой системы
        sizeof(SysNameBuffer));  // in Длина предыдущего параметра

    cout << "Название диска: " << NameBuffer << endl;
    cout << "Файловая система: " << SysNameBuffer << endl;
    cout << "Серийный номер: " << VSNumber << endl;
    cout << "Максимальная длина названия файла: " << MCLength << endl;
    printDriveFlags(FileSF);
}

void printFreeSpace() {
    LPCTSTR lpDirectoryName;                   // имя диска(директории) [in]
    PULARGE_INTEGER lpFreeBytesAvailable;      // доступно для использования(байт) [out]
    PULARGE_INTEGER lpTotalNumberOfBytes;      // максимальный объём( в байтах ) [out]
    PULARGE_INTEGER lpTotalNumberOfFreeBytes;  // свободно на диске( в байтах ) [out]
}

void printDriveInfo(char c) {
    cout << endl;
    cout << c << ":\\" << endl;
    cout << "Тип: " << _getDriveType(c) << endl;
    printVolumeInformation(c);
    printFreeSpace();
}

int main(void) {
    while (true) {
        char c = askDriveLetter();
        if (isDriveExists(c)) {
            printDriveInfo(c);
        } else {
            cout << "Такого диска нет" << endl;
        }
        cout << endl;
    }
    return 0;
}