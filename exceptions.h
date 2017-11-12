#ifndef EXECPTIONS_H
#define EXECPTIONS_H

#define NOT_FOUND_MSG       "A jelentkező nem található"
#define MEMBER_EXISTS_MSG   "Ilyen jelentkező már létezik"
#define MAIL_EXISTS_MSG     "Ilyen e-mail cím már létezik, és ez az azonosító ugyebár"
#define INPUT_TOO_LONG_MSG  "Az input túl hosszú"
#define ROUTE_NOT_FOUND_MSG "\nIlyen út nincsen, amik közül választani lehet:\
                            \n1: Parlament\n2: Hősök tere\n3: Vár\n(Ezeket be kell gépelni a sikeres kiválasztáshoz.)\n" 

typedef enum {
    NOERR,
    NOT_FOUND,
    MEMBER_EXISTS,
    MAIL_EXISTS,
    INPUT_TOO_LONG,
    ROUTE_NOT_FOUND
} Error;

static char* errorMessages[] = {
    "",
    NOT_FOUND_MSG,
    MEMBER_EXISTS_MSG,
    MAIL_EXISTS_MSG,
    INPUT_TOO_LONG_MSG,
    ROUTE_NOT_FOUND_MSG
};

#endif
