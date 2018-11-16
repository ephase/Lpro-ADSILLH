#include <sys/utsname.h>
#include <string.h>

int uname(struct utsname *buf)
{

    strcpy(buf->sysname,"Windows GNU/LINUX");
    strcpy(buf->nodename, "");
    strcpy(buf->release, "10.0.0.1");
    strcpy(buf->version, "10.2");
    strcpy(buf->machine, "Shittycorp processors inc.");

    return 1;
}
