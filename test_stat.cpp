#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
//дата для даты и для последнего редактирования
//struct stat {
//	dev_t         st_dev;      /* устройство */
//	ino_t         st_ino;      /* inode */
//	mode_t        st_mode;     /* режим доступа */
//	nlink_t       st_nlink;    /* количество жестких ссылок */
//	uid_t         st_uid;      /* идентификатор пользователя-владельца */
//	gid_t         st_gid;      /* идентификатор группы-владельца */
//	dev_t         st_rdev;     /* тип устройства */
//	/* (если это устройство) */
//	off_t         st_size;     /* общий размер в байтах */
//	blksize_t     st_blksize;  /* размер блока ввода-вывода */
//	/* в файловой системе */
//	blkcnt_t      st_blocks;   /* количество выделенных блоков */
//	time_t        st_atime;    /* время последнего доступа */
//	time_t        st_mtime;    /* время последней модификации */
//	time_t        st_ctime;    /* время последнего изменения */
//};
std::string get_date(time_t t){
	struct tm info;
	char buffer[64];

	std::string s = std::to_string(t);
	strptime(s.c_str(), " %s ", &info);
	strftime (buffer, sizeof(buffer), "%a, %d %b %Y %X %Z", &info);
	std::string str = buffer;
	return str;
}
int main()
{
	struct stat buf;
	stat("config", &buf);
	std::cout << get_date(buf.st_mtimespec.tv_sec);
	return (0);
}