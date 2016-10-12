#include <stdio.h>
#include "typesafe.h"



NEWTYPE(cent_t, unsigned);
NEWTYPE(dollar_t, unsigned);

#define DOLLAR_2_CENT(dollar)       (TO_NT(cent_t, 100*FROM_NT(dollar)))

cent_t calc(cent_t amount) {
     // expecting 'amount' to semantically represents cents...
     return TO_NT(cent_t, FROM_NT(amount)*2);
}

int main(int argc, char* argv[]) {
    dollar_t amount = TO_NT(dollar_t, 50);  // or alternatively {50};
    calc(DOLLAR_2_CENT(amount));  // ok
    calc(amount);                 // raise warning
    return 0;
}
