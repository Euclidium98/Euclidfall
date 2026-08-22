#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <dirent.h>

int FilesInDir(const char *path);
char defaultstart[] = "#define";
FILE *fileout;
void sayh(char* location, char *name);
char answerthing[20];

int main(){
	fileout = fopen("objamount.h","w");
	
	
	//fputs("Awesome header file\n", fileout);
	sayh("Models","Hmodels");
	sayh("Graphics","Htextures");
	sayh("Models/Buildings","Hbuildings");
	sayh("Graphics/Sign","Hsigns");
	sayh("Graphics/UI","Hui");
	sayh("Graphics/Font","Hfonts");
	sayh("Graphics/Billboard","Hbillboards");
	sayh("Models/Enviroment","Henviroments");
	sayh("Models/Dungeon1","Hdungeon1");
	sayh("Models/Dungeon2","Hdungeon2");
	sayh("Models/Dungeon3","Hdungeon3");
	sayh("Models/Dungeon4","Hdungeon4");
	sayh("Models/Detail","Hdetail");
	sayh("Graphics/Animated","Hanimated");
	sayh("Models/Inside","Hinside");
	sayh("Graphics/FullScreen","Hfullscreen");
	
	fclose(fileout);
	return 0;
}

void sayh(char *location, char *name){
	//fputs(location, fileout);
	fputs("#define ",fileout);
	fputs(name,fileout);
	fputs(" ", fileout);
	sprintf(answerthing, "%d", FilesInDir(location));
	fputs(answerthing,fileout);
	fputs("\n", fileout);
	
}




int FilesInDir(const char *path) {
    DIR *dir;
    struct dirent *entry;
    int file_count = 0;

    // Open 
    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return -1; // Indicate an error
    }

    // Read from the directory
    while ((entry = readdir(dir)) != NULL) {
        // Skip current and parent directory
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        file_count++;
        
    }

    // Close
    closedir(dir);

    return file_count;
}