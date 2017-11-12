#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "menu.h"
#include "data.h"
#include "exceptions.h"
#include "separators.h"

static MenuItem menu[MENU_SIZE] = {
    {
        "Főmenu",
        "Válasszon menüpontot\n",
        mainMenu
    },
    {
        "Jelentkező felvétele",
        "Adja meg az új jelentkező adatait soronként\n",
        addNewMemberByLineMenu
    },
    {
        "Több jelentkező felvétele",
        "Adja meg az adatait\n",
        addMultipleNewMembersByLineMenu
    },
    {
        "Jelentkező módosítása",
        "Adja meg a jelenzkező adatait\n",
        modifyNewMemberByLineMenu
    },
    {
        "Jelentkező törlése",
        "Adja meg a jelenzkező e-mail címét\n",
        deleteMemberMenu
    },
    {
        "Összes jelentkező listázása",
        "",
        listMembersMenu
    },
    {
        "Jelentkezők listázása útjuk szerint",
        "Adja meg az utat\n",
        listMembersByRouteMenu
    }
};

static char* headers[H_SIZE] = {
    "Név",
    "E-mail",
    "Telefonszám",
    "Útvonal",
    "Kedvezmény",
    "Jelentkezés ideje"
};

static int headers_s[H_SIZE] = {
    MAX_NAME_SIZE,
    MAX_MAIL_SIZE,
    MAX_TEL_SIZE,
    MAX_ROUTE_SIZE,
    MAX_DISC_SIZE,
    MAX_DATE_SIZE
};

static int data_s_sum;

void mainMenu(Data* d_, MenuItem* item, void* mParam)
{
    Data d;
    loadData(&d);

    int a = 5;
    for(int i=0; i<8; ++i)
    {
    printf("%d\n", a >> i);
    }

    int id = 5;
    char b[32];

    listMenuItems();
    write(1, "> ", 2);
    readLine(b);
    id = *b - '0';
    while(id != 0 && id != 53)
    {
        write(1, "\n", 1);

        printLn(menu[id].msg);
        menu[id].f(&d, NULL, NULL);
        write(1, "\n", 1);

        listMenuItems();
        write(1, "> ", 2);
        readLine(b);
        id = *b - '0';

    }
    if(id == 53)
    {
        saveData(&d,sizeof(d));
    }
}

void addNewMemberByLineMenu(Data* d, MenuItem* item, void* mParam)
{
    Member m;

    printLn("Név:\n");
    m.name_s = readLine(m.name);

    printLn("E-mail:\n");
    m.mail_s = readLine(m.mail);

    printLn("Tel:\n");
    m.tel_s = readLine(m.tel);

    char rBuf[512];
    printLn("Útvonal:\n");
    readLine(rBuf);
    int rId = getRouteId(rBuf);
    if(rId < 0) handleException(ROUTE_NOT_FOUND);
    m.route = rId;

    time_t t = time(NULL);
    m.apply_date = *localtime(&t);

    if((bool)mParam)
    {
        m.discount = true;
    }
    else
    {
        m.discount = false;
    }

    addMember(d, &m);
}

void addMultipleNewMembersByLineMenu(Data* d, MenuItem* item, void* mParam)
{
    addNewMemberByLineMenu(d, NULL, NULL);

    printLn("Adja meg hány embert hoz magával\n");
    char b[512];
    int l = readLine(b);
    int n = atoi(b);
    
    for(int i=0; i<n; ++i)
    {
        addNewMemberByLineMenu(d, NULL, ((void*)true));
    }
}

void modifyNewMemberByLineMenu(Data* d, MenuItem* item, void* mParam)
{
    printLn("Nem módosítandó mező esetén üres entert kell nyomni.\n");
    printLn("Módosítandó tag e-mail címe:\n");
    int ts;
    char tBuf[512];
    ts = readLine(tBuf);
    int id = findMember(d, tBuf);
    if(id < 0) handleException(NOT_FOUND);

    Member* om = &d->members[id];
    Member m = *om;

    printLn("Név:\n");
    m.name_s = readLine(m.name);
    if(m.name_s < 1)
    {
        memcpy(m.name,om->name,om->name_s);
        m.name_s = om->name_s;
    }

    printLn("E-mail:\n");
    m.mail_s = readLine(m.mail);
    if(findMember(d, m.mail) > 0) handleException(MAIL_EXISTS);
    if(m.mail_s < 1)
    {
        memcpy(m.mail,om->mail,om->mail_s);
        m.mail_s = om->mail_s;
    }

    printLn("Tel:\n");
    m.tel_s = readLine(m.tel);
    if(m.tel_s < 1)
    {
        memcpy(m.tel,om->tel,om->tel_s);
        m.tel_s = om->tel_s;
    }

    char rBuf[512];
    printLn("Útvonal:\n");
    int rs = readLine(rBuf);
    int rId = getRouteId(rBuf);
    m.route = rId;
    if(rs < 1 || rId < 0)
    {
        if(rId < 0) handleException(ROUTE_NOT_FOUND);
        m.route = om->route;
    }

    *om = m;
}

void deleteMemberMenu(Data* d, MenuItem* item, void* mParam)
{
    printLn("Törlendő tag e-mail címe:\n");
    char mail[MAX_MAIL_SIZE];
    readLine(mail);
    deleteMember(d, mail);
}

void listMembersMenu(Data* d, MenuItem* item, void* mParam)
{
    printHeaders();
    for(int i=0; i < d->size; ++i)
    {
        Member* m = &d->members[i];
        int rfill;

        rfill = MAX_NAME_SIZE - utf8strlen(m->name);
        write(1, COLOUMN_BEGIN, strlen(COLOUMN_BEGIN));
        write(1, m->name, m->name_s);
        write(1, WHITESPACES, rfill);
        write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));

        rfill = MAX_MAIL_SIZE - utf8strlen(m->mail);
        write(1, m->mail, m->mail_s);
        write(1, WHITESPACES, rfill);
        write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));

        rfill = MAX_TEL_SIZE - utf8strlen(m->tel);
        write(1, m->tel, m->tel_s);
        write(1, WHITESPACES, rfill);
        write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));

        char* r = route_to_string[m->route];
        rfill = MAX_ROUTE_SIZE - utf8strlen(r);
        write(1, r, route_to_size[m->route]);
        write(1, WHITESPACES, rfill);
        write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));

        if(m->discount)
        {
            rfill = MAX_DISC_SIZE - 3;
            write(1, "15%", 3);
            write(1, WHITESPACES, rfill);
        }
        else
        {
            rfill = MAX_DISC_SIZE - 2;
            write(1, "0%", 2);
            write(1, WHITESPACES, rfill);
        }
        write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));

        char cdate[MAX_DATE_SIZE];
        tm* t = &m->apply_date;

        sprintf(cdate, "%d-%d-%d_%d-%d-%d",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec
        );

        rfill = MAX_DATE_SIZE - utf8strlen(cdate);
        write(1, cdate, 19);
        write(1, WHITESPACES, rfill);
        write(1, COLOUMN_END, strlen(COLOUMN_END));
        write(1, "\n", 1);
    }

    write(1, DASHES, SEPARATOR_LINE);
    write(1, "\n", 1);
}

void listMembersByRouteMenu(Data* d, MenuItem* item, void* mParam)
{
    Route r;
    char tmpr[MAX_ROUTE_SIZE];
    readLine(tmpr);
    int id = getRouteId(tmpr);
    if(id > 0) r = (Route)id;
    else
    {
        handleException(ROUTE_NOT_FOUND);
        return;
    }

    printHeaders();
    for(int i=0; i < d->size; ++i)
    {
        Member* m = &d->members[i];
        if(m->route == r)
        {
            int rfill;

            rfill = MAX_NAME_SIZE - utf8strlen(m->name);
            write(1, COLOUMN_BEGIN, strlen(COLOUMN_BEGIN));
            write(1, m->name, m->name_s);
            write(1, WHITESPACES, rfill);
            write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));

            rfill = MAX_MAIL_SIZE - utf8strlen(m->mail);
            write(1, m->mail, m->mail_s);
            write(1, WHITESPACES, rfill);
            write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));

            rfill = MAX_TEL_SIZE - utf8strlen(m->tel);
            write(1, m->tel, m->tel_s);
            write(1, WHITESPACES, rfill);
            write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));

            char* r = route_to_string[m->route];
            rfill = MAX_ROUTE_SIZE - utf8strlen(r);
            write(1, r, route_to_size[m->route]);
            write(1, WHITESPACES, rfill);
            write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));

            if(m->discount)
            {
                rfill = MAX_DISC_SIZE - 3;
                write(1, "15%", 3);
                write(1, WHITESPACES, rfill);
            }
            else
            {
                rfill = MAX_DISC_SIZE - 2;
                write(1, "0%", 2);
                write(1, WHITESPACES, rfill);
            }
            write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));

            char cdate[MAX_DATE_SIZE];
            tm* t = &m->apply_date;

            sprintf(cdate, "%d-%d-%d_%d-%d-%d",
                t->tm_year + 1900,
                t->tm_mon + 1,
                t->tm_mday,
                t->tm_hour,
                t->tm_min,
                t->tm_sec
            );

            rfill = MAX_DATE_SIZE - utf8strlen(cdate);
            write(1, cdate, 19);
            write(1, WHITESPACES, rfill);
            write(1, COLOUMN_END, strlen(COLOUMN_END));
            write(1, "\n", 1);
        }
    }

    write(1, DASHES, SEPARATOR_LINE);
    write(1, "\n", 1);
}

void saveData(Data* d, size_t size_d)
{
    int fd = open(SAVED_DATA_NAME,O_WRONLY|O_CREAT|O_TRUNC,0644);
    write(fd, d, size_d);
    close(fd);
}

void loadData(Data* d)
{
    struct stat ldst;
    off_t ld_s;

    int fd = open(SAVED_DATA_NAME,O_RDONLY);
    fstat(fd, &ldst);
    ld_s = ldst.st_size;

    read(fd, d, ld_s);
    close(fd);
}

int addMember(Data* d, Member* m)
{
    if(findMember(d, m->mail) < 0)
    {
        d->members[d->size] = *m; 
        ++d->size;
    }
    else
    {
        handleException(MEMBER_EXISTS);
    }
}

int deleteMember(Data* d, char* mail)
{
    int id = findMember(d, mail);
    if(id < 0)
    {
        return id;
    }
    else
    {
        memcpy(&(d->members[id]), &(d->members[id + 1]), (d->size - id - 1)*sizeof(Member));
        --d->size;
        return 0;
    }
}

int findMember(Data* d, char* mail)
{
    for(int i=0; i<d->size; ++i)
    {
        char* p = d->members[i].mail;
        char* q = mail;
        for(; *p == *q && (*p != '\0' || *q != '\0'); ++p, ++q) { }
        if(*p == *q) return i;
    }
    return (-1);
}

void printHeaders()
{
    write(1, "\n", 1);
    write(1, DASHES, SEPARATOR_LINE);
    write(1, "\n", 1);

    write(1, COLOUMN_BEGIN, strlen(COLOUMN_BEGIN));
    for(int i=0; i<H_SIZE - 1; ++i)
    {
        int r_fill = headers_s[i] - utf8strlen(headers[i]);
        write(1, headers[i], strlen(headers[i]));
        write(1, WHITESPACES, r_fill);
        write(1, COLOUMN_SEPARATOR, strlen(COLOUMN_SEPARATOR));
    }
    int i = H_SIZE - 1;
    int r_fill = headers_s[i] - utf8strlen(headers[i]);
    write(1, headers[i], strlen(headers[i]));
    write(1, WHITESPACES, r_fill);
    write(1, COLOUMN_END, strlen(COLOUMN_END));

    write(1, "\n", 1);
    write(1, DASHES, SEPARATOR_LINE);
    write(1, "\n", 1);
}

int getRouteId(char* rStr)
{
    for(int i=0; i<ROUTES; ++i)
    {
        char* p = rStr;
        char* q = route_to_string[i];
        for(; *p == *q && (*p != '\0' || *q != '\0'); ++p, ++q)
        if(*p == *q) return i;
    }
    return (-1);
}

int readLine(char* p)
{
    int c = read(0, p, MAX_INUPUT_SIZE);
    p[c - 1] = '\0';
    return c - 1;
}

void printLn(char* s)
{
    write(1, s, strlen(s));
}

int utf8strlen(char* c)
{
    char* p = c;
    int s = 0;

    for(; *p != '\0'; ++p)
    {
        if((int)(*p) < 0)
        {
            int d = p[0] >> 4 & 0xf;

            ++s;
            p += (d & 0x1) + (d >> 1 & 0x1) + (d >> 2 & 0x1);
        }
        else
        {
            ++s;
        }
    }
    return s;
}

void listMenuItems()
{
    write(1, "\n", 1);
    for(int i=1; i<MENU_SIZE; ++i)
    {
        char head[3] = "e: ";
        head[0] = i + '0';

        write(1, head, 3);
        write(1, menu[i].name, strlen(menu[i].name));
        write(1, "\n", 1);
    }
    write(1, DASHES, strlen(menu[6].name));
    write(1, "\n", 1);
    printLn("0: Kilépés\n");
    printLn("e: Mentés és kilépés\n");
    write(1, "\n", 1);
}

int handleException(int err)
{
    printLn(errorMessages[err]);
    write(1, "\n", 1);
    return err;
}
