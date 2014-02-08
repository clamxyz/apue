#include "../public.h"

#include <pwd.h>

extern int errno;

static void print_title(void);
static void print_entry(struct passwd *pwd);
int main(int argc, char *argv[])
{
	struct passwd *pwd;

	setpwent();
	print_title();
	while((pwd = getpwent()) != NULL)
	{
		print_entry(pwd);	
	}
	if (errno != 0)
		printf("getpwent error[%d][%s]\n", errno, strerror(errno));
	endpwent();

	return 0;
}

static void print_title(void)
{
	printf("name\tpassword\tuser id\tgroup id\treal name\thome directory\tshell program\n");
}

static void print_entry(struct passwd *pwd)
{
	printf("%s\t%s\t%d\t%d\t%s\t%s\t%s\n", pwd->pw_name, pwd->pw_passwd, pwd->pw_uid, pwd->pw_gid, pwd->pw_gecos, pwd->pw_dir, pwd->pw_shell);
}

