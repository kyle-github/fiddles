/* taken from Stack Overflow here:
http://stackoverflow.com/questions/36351496/type-safety-in-c
*/

#ifndef __TYPESAFE_H__
#define __TYPESAFE_H__ 1

#define NEWTYPE(nty,oty) typedef struct { oty v; } nty
#define FROM_NT(ntv)       ((ntv).v)
#define TO_NT(nty,val)     ((nty){(val)})  /* or better ((nty){ .v=(val)}) if C99 */

#endif

