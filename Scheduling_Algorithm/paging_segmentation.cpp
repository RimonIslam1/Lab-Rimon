#include <iostream>
#include <vector>
using namespace std;

// Paging Implementation
void paging() {
    cout << "\n--- Paging ---\n";
    int processSize, pageSize, numPages, logicalAddr, pageTableEntry;
    cout << "Enter Process/Program Size: ";
    cin >> processSize;
    cout << "Enter Page Size: ";
    cin >> pageSize;
    numPages = (processSize + pageSize - 1) / pageSize;

    vector<int> pageTable(numPages);
    cout << "Enter Frame Number for each Page (Page Table):\n";
    for (int i = 0; i < numPages; i++) {
        cout << "Page " << i << ": ";
        cin >> pageTable[i];
    }

    cout << "Enter Logical Address: ";
    cin >> logicalAddr;

    int pageNumber = logicalAddr / pageSize;
    int offset = logicalAddr % pageSize;

    if (pageNumber >= numPages) {
        cout << "Invalid logical address!\n";
        return;
    }

    int frameNumber = pageTable[pageNumber];
    int physicalAddr = frameNumber * pageSize + offset;

    cout << "Physical Address: " << physicalAddr << "\n";
}

// Segmentation Implementation
void segmentation() {
    cout << "\n--- Segmentation ---\n";
    int numSegments;
    cout << "Enter Number of Segments: ";
    cin >> numSegments;

    vector<int> segmentBase(numSegments), segmentLimit(numSegments);
    cout << "Enter Base and Limit for each Segment (Segment Table):\n";
    for (int i = 0; i < numSegments; i++) {
        cout << "Segment " << i << " Base: ";
        cin >> segmentBase[i];
        cout << "Segment " << i << " Limit: ";
        cin >> segmentLimit[i];
    }

    int segmentNumber, offset;
    cout << "Enter Logical Address (Segment Number and Offset):\n";
    cout << "Segment Number: ";
    cin >> segmentNumber;
    cout << "Offset: ";
    cin >> offset;

    if (segmentNumber >= numSegments || offset >= segmentLimit[segmentNumber]) {
        cout << "Invalid logical address!\n";
        return;
    }

    int physicalAddr = segmentBase[segmentNumber] + offset;
    cout << "Physical Address: " << physicalAddr << "\n";
}

int main() {
    int choice;
    do {
        cout << "\nChoose Memory Management Technique:\n";
        cout << "1. Paging\n";
        cout << "2. Segmentation\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: paging(); break;
            case 2: segmentation(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);
    return 0;
}