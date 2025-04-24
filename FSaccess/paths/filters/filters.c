#include <dirent.h>
#include <string.h>


int
filter_visible(const struct dirent* dir)
{
    if(dir->d_name[0] == '.'){
        return 0;
    }
    return 1;
}


int
filter_local(const struct dirent* dir)
{
    switch (strlen(dir->d_name))
    {
    case 1:
        if(dir->d_name[0] == '.'){
            return 0;
        }
        return 1;
    case 2:
        if(dir->d_name[0] == '.' && dir->d_name[1] == '.'){
            return 0;
        }
        return 1;
    }
    return 1;
}
