#ifndef MENU_H
#define MENU_H

#include "data.h"

#define MENU_SIZE       7
#define SAVED_DATA_NAME "default.dat"

typedef struct MenuItem MenuItem;
typedef void (*MenuFunction)(Data*, MenuItem*, void*);
typedef struct Menu Menu;

struct MenuItem
{
    char* name;
    char* msg;
    MenuFunction f;
};

void mainMenu(Data* d_, MenuItem* item, void* mParam);
void addNewMemberByLineMenu(Data* d, MenuItem* item, void* mParam);
void addMultipleNewMembersByLineMenu(Data* d, MenuItem* item, void* mParam);
void modifyNewMemberByLineMenu(Data* d, MenuItem* item, void* mParam);
void deleteMemberMenu(Data* d, MenuItem* item, void* mParam);
void listMembersMenu(Data* d, MenuItem* item, void* mParam);
void listMembersByRouteMenu(Data* d, MenuItem* item, void* mParam);

void saveData(Data* d, size_t size_d);
void loadData(Data* d);

int addMember(Data* d, Member* m);
int deleteMember(Data* d, char* mail);
int findMember(Data* d, char* mail);

void printHeaders();
int getRouteId(char* rStr);

int readLine(char* p);
void printLn(char* s);
// utf8 string hossza(strlen byte-ok számát adja vissza)
int utf8strlen(char* c);
void listMenuItems();
int handleException(int err);

#endif
