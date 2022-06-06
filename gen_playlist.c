#include <errno.h>     /* errno */
#include <sys/types.h> /* closedir, opendir, readdir */
#include <stdio.h>     /* perror, printf */
#include <stdlib.h>    /* exit, getenv */
#include <string.h>    /* strcmp, strtok */
#include <dirent.h>    /* closedir, opendir, readdir */

int main() {
    char *playlist_dirpath;
    DIR *dirp;
    struct dirent *p;
    FILE *fp;
    char *output_filename = "playlist.html";

    if ((playlist_dirpath = getenv("PLAYLIST_DIRPATH")) == NULL) {
        perror("getenv");
        exit(EXIT_FAILURE);
    }
    if ((dirp = opendir(playlist_dirpath)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    if ((fp = fopen(output_filename, "w")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    errno = 0;

    fprintf(fp, "<html lang=\"ja\">\n");
    fprintf(fp, "  <head>\n");
    fprintf(fp, "    <meta charset=\"utf-8\">\n");
    fprintf(fp, "    <meta name=\"viewport\" content=\"width=device-width\">\n");
    fprintf(fp, "    <title>playlist</title>\n");
    fprintf(fp, "  </head>\n");
    fprintf(fp, "  <body>\n");
    while ((p = readdir(dirp)) != NULL) {
        if (!strcmp(p->d_name, ".")) continue;
        if (!strcmp(p->d_name, "..")) continue;
        if (!strcmp(p->d_name, ".DS_Store")) continue;
        char *audio_filename = strtok(p->d_name, ".");
        fprintf(fp, "    <figure>\n");
        fprintf(fp, "      <figcaption>%s</figcaption>\n", audio_filename);
        fprintf(fp, "      <audio\n");
        fprintf(fp, "        controls\n");
        fprintf(fp, "        loop\n");
        fprintf(fp, "        style=\"width: 400px;\"\n");
        fprintf(fp, "        src=\"file://%s/%s.webm\">\n", playlist_dirpath, audio_filename);
        fprintf(fp, "      </audio>\n");
        fprintf(fp, "    </figure>\n");
    }
    fprintf(fp, "  </body>\n");
    fprintf(fp, "</html\n");

    if (errno != 0) {
        perror("readdir");
        exit(EXIT_FAILURE);
    }
    if (closedir(dirp) != 0) {
        perror("closedir");
        exit(EXIT_FAILURE);
    }
    return 0;
}
