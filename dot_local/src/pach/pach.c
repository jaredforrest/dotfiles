#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <gpgme.h>
#include <unistd.h>
#include <inttypes.h>

#include <ftw.h> /* for the file list */

#define XSTR(s) STR(s)
#define STR(s) #s

#define DIE(values...) ({PRINTERR(values); exit(1); })
#define PRINTERR(values...) ({const char *v[] = { values }; printerr(sizeof(v) / sizeof(*v), v); })

#define MAXFILESIZE 2048
#define GEN_PASSWORD_LENGTH 50
#define MAX_PASSWORD_LENGTH 255
#define MAXSHORTPATH 255

void die(int count, char **messages);
void printerr(int count, const char ** messages);
void chkgpgerr(gpgme_error_t);
bool yesno(void);
void reset_term_input(void);
void disable_term_input_flag(tcflag_t flags);
bool is_valid_char(char onechar);

void getpassword(const char * filename, char * buffer, size_t buffersize);
void setpassword(const char *filename, const char *password, size_t passwordlength);
void generate_password(char * password, size_t size);
void usage(void);

void pw_add(const char * filename, uintmax_t pw_length);
void pw_del(const char * filename);
void pw_show(const char * filename);
void pw_head(const char * filename);
void pw_list(void);
void pw_edit(const char * filename);

void print_directory_tree(const char *const);
int print_entry(const char*, const struct stat*, const int);

struct termios saved_attributes;

char * pachdir;

int
main(int argc, char *argv[]){
   
    pachdir = getenv ("PACH_DIR");

    if(!pachdir){
        DIE("Please set $PACH_DIR");
    }


    if(argc==3||argc==4)
    {
        char pachfile[MAXSHORTPATH];
        if(strlen(pachdir)+strlen(argv[2])+sizeof("/.gpg")>MAXSHORTPATH)
            DIE("Path too long");
        strcpy(pachfile, pachdir);
        strcat(pachfile, "/");
        strcat(pachfile, argv[2]);
        strcat(pachfile, ".gpg");

        switch(argv[1][0]) {
            case 'a':
                if(argc==3){
                    pw_add(pachfile, 0);
                }else{
                    pw_add(pachfile, strtoumax(argv[3], NULL, 10));
                }
                break;
            case 'd':
                pw_del(pachfile);
                break;
            case 's':
                pw_show(pachfile);
                break;
            case 'h':
                pw_head(pachfile);
                break;
            case 'e':
                pw_edit(pachfile);
                break;
            default :
                usage();
        }
    }else if(argc==2){
        switch(argv[1][0]) {
            case 'l':
                pw_list();
                break;
            default:
                usage();
        }
    }else{
        usage();
    }
    return(0);
}

void
pw_add(const char *filename, uintmax_t pw_length){
    char password[MAX_PASSWORD_LENGTH + 1] = "";
    char password2[MAX_PASSWORD_LENGTH + 1] = "";
    size_t passwordlength = (pw_length != 0) ? pw_length : GEN_PASSWORD_LENGTH;

    printf("Generate a password");
    if (yesno()){
        generate_password(password, GEN_PASSWORD_LENGTH);
    }
    else{
        disable_term_input_flag(ECHO);
        printf("Enter password");
        scanf("%" XSTR(MAX_PASSWORD_LENGTH) "s", password);
        printf("\nEnter password (again)");
        scanf("%" XSTR(MAX_PASSWORD_LENGTH) "s", password2);
        reset_term_input();
        if(strcmp(password,password2))
            DIE("\nPasswords do not match");
        putchar('\n');
        passwordlength = strlen(password);
    }
    setpassword(filename, password, passwordlength);
    printf("Saved %s to the store\n", filename);
}

void
pw_del(const char *filename){
    int status;

    printf("Delete pass file %s", filename);
    if(yesno()){
        status = remove(filename);
        if (status)PRINTERR("Unable to delete the file");
    }
}

void
pw_show(const char * filename){
    char buffer[MAXFILESIZE] = "";
    getpassword(filename, buffer, MAXFILESIZE);
    printf("%s", buffer);
}

void
pw_head(const char * filename){
    char buffer[MAXFILESIZE] = "";
    getpassword(filename, buffer, MAXFILESIZE);
    /*char *tail = */strtok(buffer, "\n");
    printf("%s\n", buffer);
}

void
pw_edit(const char * filename){

    char buffer[MAXFILESIZE] = "";

    char * editor = getenv("EDITOR");
    char temp[] = "/tmp/pach-edit.XXXXXX";
    char * cmd;
    int tempfd;
    ssize_t len;

    getpassword(filename, buffer, MAXFILESIZE);
    
    if(!editor)DIE("Error: Please set $EDITOR to edit files");

    tempfd = mkstemp(temp);
    if(tempfd<0)PRINTERR("Error: Couldn't create temp file");

    len = write(tempfd, buffer, strlen(buffer));
    if(len<0)PRINTERR("Error: Couldn't write to temp file");

    cmd = malloc((strlen(editor)+strlen(temp)+2));

    strcpy(cmd, editor);
    strcat(cmd, " ");
    strcat(cmd, temp);

    system(cmd);
    free(cmd);

    lseek(tempfd, 0, SEEK_SET);
    len = read(tempfd, buffer, MAXFILESIZE);
    if(len==-1)PRINTERR("Error: Couldn't read from temp file");
    len = close(tempfd);
    if(len==-1)PRINTERR("Error: Couldn't close temp file");
    len = remove(temp);
    if(len==-1)PRINTERR("Error: Couldn't delete temp file");

    setpassword(filename, buffer, strlen(buffer));
}

void
pw_list(){
    print_directory_tree(pachdir);
}

void
getpassword(const char * filename, char * buffer, size_t buffersize){
    gpgme_data_t encdata;
    gpgme_data_t plaindata;
    gpgme_ctx_t  ctx;
    gpgme_error_t err;
    ssize_t len;

    // Check file exists
    if( access( filename, F_OK ) != 0 ) {
         DIE(filename /*argv[2]*/, " is not in the password store.");
    }

    gpgme_check_version (NULL);
    
    /* Create the context object */
    err = gpgme_new(&ctx);
    chkgpgerr (err);

    gpgme_set_protocol (ctx, GPGME_PROTOCOL_OPENPGP);
/*    gpgme_set_ctx_flag (ctx, "no-symkey-cache", "1");*/

    /* Create the data objects */
    err = gpgme_data_new (&plaindata);
    chkgpgerr (err);
    err = gpgme_data_new_from_file (&encdata, filename, 1);
    chkgpgerr (err);

    err = gpgme_op_decrypt_start (ctx, encdata, plaindata);
    chkgpgerr (err);

    gpgme_wait (ctx, &err, 1);
    chkgpgerr (err);

    len = gpgme_data_seek (plaindata, 0, SEEK_SET);
    if(len<0)PRINTERR("Error setting buffer position");

    len = gpgme_data_read (plaindata, buffer, buffersize-1);
    if(len<0)PRINTERR("Error reading plaintext password");

    gpgme_data_release (encdata);
    gpgme_data_release (plaindata);
    gpgme_release (ctx);
    
}

void
setpassword(const char *filename, const char *password, size_t passwordlength){
    gpgme_data_t encdata;
    gpgme_data_t plaindata;
    gpgme_ctx_t  ctx;
    gpgme_error_t err;
    const mode_t mode = S_IRUSR | S_IWUSR;
    int pass_file = open(filename, O_WRONLY|O_CREAT, mode);
    char buffer[MAXFILESIZE];
    ssize_t len;

    gpgme_check_version (NULL);
    
    /* Create the context object */
    err = gpgme_new(&ctx);
    chkgpgerr (err);

    gpgme_set_protocol (ctx, GPGME_PROTOCOL_OPENPGP);
    //gpgme_set_ctx_flag (ctx, "no-symkey-cache", "1");

    /* Create the data objects */
    err = gpgme_data_new_from_mem (&plaindata, password, passwordlength, 0);
    chkgpgerr (err);
    err = gpgme_data_new (&encdata);
    chkgpgerr (err);

    err = gpgme_op_encrypt (ctx, NULL, GPGME_ENCRYPT_SYMMETRIC, plaindata, encdata);
    chkgpgerr (err);

    gpgme_data_seek(encdata, 0, SEEK_SET);
    len = gpgme_data_read (encdata, buffer, MAXFILESIZE);
    if(len<0)PRINTERR("Error reading encrypted data");
  
    // only it len > 0
    len = write(pass_file, buffer, (size_t)len);
    if(len<0)PRINTERR("Error writing data to file");

    gpgme_data_release(encdata);
    gpgme_data_release (plaindata);
    gpgme_release (ctx);
    close(pass_file);
}

/* size must be less that 255 */
void
generate_password(char * password, size_t size){
    char onechar;
    int dev_random = open("/dev/random", O_RDONLY);
    size_t n = 0;
    for (;n < size;) {
        ssize_t status = read(dev_random, &onechar, 1);
        if (status<0)PRINTERR("Unable to read from /dev/random");
        if(is_valid_char(onechar)){
            password[n] = (char) onechar;
            n++;
        }
     }
    close(dev_random);
}


void
usage() {
    printf("pach 1.0.0 - simple password manager.\n\n"
"=> [a]dd  [name] - Create a new password entry.\n"
/*"=> [c]opy [name] - Copy entry to the clipboard.\n"*/
"=> [d]el  [name] - Delete a password entry.\n"
"=> [l]ist        - List all entries.\n"
"=> [s]how [name] - Show all lines of password for an entry.\n"
"=> [t]ree        - List all entries in a tree.\n"
"=> [h]ead        - Show first line of a password from an entry.\n"
"=> [gd]own       - Pull changes from git directory\n"
"=> [gu]p         - Push changes to git directory\n");
}

void
reset_term_input (void)
{
    tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void
disable_term_input_flag (tcflag_t flags){
    struct termios tattr;

    if (!isatty (STDIN_FILENO)){
        PRINTERR("Not a terminal.");
    }

    /* Save the terminal attributes so we can restore them later. */
    tcgetattr (STDIN_FILENO, &saved_attributes);

    /* Set the funny terminal modes. */
    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(flags); /* Clear Flags */
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}

bool
is_valid_char(char onechar)
{
    return (onechar>=48&&onechar<=57)||
        (onechar>=64&&onechar<=90)||
        (onechar>=97&&onechar<=122)||
        (onechar=='#')||
        (onechar=='_')||
        (onechar==',')||
        (onechar=='+')||
        (onechar=='-');
}

bool
yesno()
{
    int c;
    printf(" [y/N]:");
    disable_term_input_flag(ICANON);
    c = getchar();
    reset_term_input();
    printf("\n");
    return c=='y'||c=='Y';
}

void
printerr(int count, const char ** messages)
{
  for( int i = 0; i < count; i++ ){
    fprintf(stderr, "%s", *messages++);
  }

  fprintf(stderr, "\n");

  perror("Error");
}

void
chkgpgerr(gpgme_error_t err){
    if (err!=GPG_ERR_NO_ERROR)
    {
    DIE("A gpg error has occured\n",
            (const char *)gpgme_strsource (err),
            ": ",
            (const char *)gpgme_strerror (err));
    }
}

_Noreturn void
die(int count, char ** messages)
{
  for( int i = 0; i < count; i++ ){
    fprintf(stderr, "%s", *messages++);
  }

  fprintf(stderr, "\n");
  exit(1);

}

int
print_entry(const char *filepath, const struct stat *info,
                const int typeflag)
{
    if (typeflag == FTW_F && ( strstr(filepath,".gpg")==filepath+strlen(filepath)-4) ){
        
        char shortpath[MAXSHORTPATH];
        strncpy(shortpath, filepath, MAXSHORTPATH-1);

        shortpath[strlen(filepath)-4] = '\0';
        printf("%s\n", shortpath + strlen(pachdir) + 1);
    }
    return 0;
}

void
print_directory_tree(const char *const dirpath)
{
    ftw(dirpath, print_entry, 15);
}
