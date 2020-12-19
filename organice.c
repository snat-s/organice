/* organice -- organize your files on the command line in an easy way.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

struct Files_To_Be_Moved
{
    char Files_To_Be_Moved[256];
    char Type_Of_Extension[256];
    char path[256];
    //int number;
};
int flag;

int Number_Of_Files(DIR *folder, struct dirent *entry, int i);
void flag_number(char *argv[]);
void Files_Moved(int *files, int argc, char Array_Of_Files[][256], char *argv[], struct Files_To_Be_Moved Files_To_Be_Moved[]);
void Make_Array_Of_Files(DIR *folder, struct dirent *entry, int *i, char Array_Of_Files[][256]);
void Create_Directorie(int argc, struct Files_To_Be_Moved  Files_To_Be_Moved[], char *argv[], char cwd []);
void Move_Files(int argc,int files, struct Files_To_Be_Moved Files_To_Be_Moved[], char cwd[], char *argv[]);

int main(int argc, char *argv[])
{
    DIR *folder;
    int files = 0, i=0;
    folder = opendir(".");
    struct dirent *entry;
    flag_number(argv);
    files=Number_Of_Files(folder, entry, i);

    char Array_Of_Files[files][256], cwd[256];
    struct Files_To_Be_Moved Files_To_Be_Moved[files];
    //Files_To_Be_Moved[0].size=argc;

    folder = opendir(".");
    Make_Array_Of_Files(folder, entry, &i, Array_Of_Files);
    Files_Moved(&files,argc,Array_Of_Files,argv, Files_To_Be_Moved);
    getcwd(cwd,sizeof(cwd));
    Create_Directorie(argc,Files_To_Be_Moved, argv, cwd);
    Move_Files(argc,files,Files_To_Be_Moved, cwd, argv);
    //printf("\n%d\n", files);
    /* for(int k=0;k<9;k++) */
    /* { */
    /*   printf("%d. %s %s\n", k, Files_To_Be_Moved[k].Files_To_Be_Moved, Files_To_Be_Moved[k].Type_Of_Extension ); */
    /* } */

    return(0);
}

void flag_number(char *argv[])
{
    if(strcmp(argv[1],"-n")==0)
    {
        flag = 1;
    } else if(strcmp(argv[1],"-e")==0)
    {
        flag = 2;
    } else if(strcmp(argv[1],"-v")==0)
    {
        flag = 3;
    } else if(strcmp(argv[1],"-h")==0 || strcmp(argv[1],"--help")==0) {
        flag = 4;
    } else
    {
     perror("There has been an error, see --help to learn how organice works.");
     flag = 0;
    }

}
int Number_Of_Files(DIR *folder, struct dirent *entry, int i)
{
    if(folder == NULL)
    {
        perror("Unable to read directory");
    }
    while( (entry=readdir(folder)) )
    {
        i++;
    }
    return i-2;
}

void Make_Array_Of_Files(DIR *folder, struct dirent *entry, int *i, char Array_Of_Files[][256])
{
    int j=0;
    while( (entry=readdir(folder)) )
    {
        i++;
        if(!strcmp(entry->d_name,".")||!strcmp(entry->d_name,"..")){
        }else{
        strcpy(Array_Of_Files[j],entry->d_name);
        j++;
        }
    }
    *i=j;
    /*Close dir has to be moved to the last time you close the directorie.
     *For some reason the *i makes it work but I have to check why the folder variable changes
     *the files variable can be changed. */
    closedir(folder);
}

void Files_Moved(int *files, int argc, char Array_Of_Files[][256], char *argv[], struct Files_To_Be_Moved Files_To_Be_Moved[])
{
        int j=0;
        for(int k=2;k<argc;k++)
        {
            for(int i=0; i<*files;i++)
            {
                char *ret;
                ret = strstr(Array_Of_Files[i],argv[k]);
                if(ret!=NULL)
                {
                    strcpy(Files_To_Be_Moved[j].Files_To_Be_Moved,Array_Of_Files[i]);
                    strcpy(Files_To_Be_Moved[j].Type_Of_Extension,argv[k]);
                    //printf("%d %s %s\n", j, Files_To_Be_Moved[j].Type_Of_Extension,Files_To_Be_Moved[j].Files_To_Be_Moved);
                    j++;
                }
            }
        }
        *files=j;
}
void Create_Directorie(int argc, struct Files_To_Be_Moved  Files_To_Be_Moved[], char *argv[], char cwd [])
{
    if (flag==1)
    {
        for(int i=2, j=0;i<argc; i++, j++)
        {
            sprintf(Files_To_Be_Moved[j].path, "%s/%s/", cwd, argv[i]);
            mkdir(Files_To_Be_Moved[j].path, 0777);
        }
    }
    else if(flag==2)
    {
    const char point[2]=".";
    for(int i=2, j=0;i<argc; i++,j++)
    {
        char *ptr;
        ptr=strtok(argv[i],point);
        sprintf(Files_To_Be_Moved[j].path, "%s/%s/", cwd, ptr);
        mkdir(Files_To_Be_Moved[j].path, 0777);
    }
    }
   /*  Does not support hidden directories so right know creates the directorie.
    *  Later I might add the flag to create a hidden directorie. */
}

void Move_Files(int argc,int files, struct Files_To_Be_Moved Files_To_Be_Moved[], char cwd[], char *argv[])
{
    for(int j=2, k=0; j<argc; j++,k++)
    {
        for(int i=0;i<files;i++)
        {
            if(strcmp(Files_To_Be_Moved[i].Type_Of_Extension,argv[j])==0)
            {
            char old_path[256], new_path[256];
            sprintf(old_path,"%s/%s", cwd, Files_To_Be_Moved[i].Files_To_Be_Moved);
            sprintf(new_path,"%s%s", Files_To_Be_Moved[k].path,Files_To_Be_Moved[i].Files_To_Be_Moved);
            rename(old_path, new_path);
            //printf("%s\n %s\n-------------------\n", old_path,new_path);
            }
        }
    }
}
