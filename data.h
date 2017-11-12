#ifndef DATA_H
#define DATA_H

#include <string.h>
#include <time.h>
#include "separators.h"

#define PARLAMENT       "Parlament"
#define HOSOK_TERE      "Hősök tere"
#define VAR             "Vár"

#define MAX_INUPUT_SIZE 512
#define MAX_MEMBERS     512
#define MAX_NAME_SIZE   32
#define MAX_MAIL_SIZE   32
#define MAX_TEL_SIZE    16
#define MAX_ROUTE_SIZE  12
#define MAX_DISC_SIZE   12
#define MAX_DATE_SIZE   22
#define ROUTES          3
#define H_SIZE          6

#define SEPARATOR_CHARS ((H_SIZE - 1)*(strlen(COLOUMN_SEPARATOR)) + strlen(COLOUMN_BEGIN) + strlen(COLOUMN_END))
#define FIELD_CHARS (MAX_NAME_SIZE + MAX_MAIL_SIZE + MAX_TEL_SIZE + MAX_ROUTE_SIZE + MAX_DISC_SIZE + MAX_DATE_SIZE)
#define SEPARATOR_LINE (SEPARATOR_CHARS + FIELD_CHARS)

typedef struct tm tm;
typedef enum { false, true } bool;
typedef struct Member Member;
typedef struct Data Data;

typedef enum {
    NAME,
    MAIL,
    TEL,
    ROUTE,
    DISCOUNT,
    APPLY_DATE
} Header;

typedef enum {
    PARLAMENT_,
    HOSOK_TERE_,
    VAR_
} Route;

static char* route_to_string[] = {
    PARLAMENT,
    HOSOK_TERE,
    VAR
};

static int route_to_size[] = {
    sizeof(PARLAMENT),
    sizeof(HOSOK_TERE),
    sizeof(VAR)
};

struct Member {
    char name[MAX_NAME_SIZE];
    int name_s;
    char mail[MAX_MAIL_SIZE];
    int mail_s;
    char tel[MAX_TEL_SIZE];
    int tel_s;
    Route route;
    bool discount;
    tm apply_date;
};

struct Data {
    Member members[MAX_MEMBERS];
    int size;
};

#endif
