#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
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

std::string server_date() {
	struct tm info;
	struct timeval time;
	char buf[29];
	gettimeofday(&time, NULL);
	std::string s = std::to_string(time.tv_sec);
	strptime(s.c_str(), " %s ", &info);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %X", &info);
	std::string str = buf;
	str += " GMT";
	return str;
}

std::string get_last_modified()
{
	struct tm info;
	char buf[29];
	struct stat st;
	stat("config", &st);
	std::string s = std::to_string(st.st_mtimespec.tv_sec);
	strptime(s.c_str(), " %s ", &info);
	strftime (buf, sizeof(buf), "%a, %d %b %Y %X", &info);
	std::string str = buf;
	str += " GMT";
	return str;
}

int main()
{
	std::cout << get_last_modified() << std::endl;
	std::cout << server_date();
//	struct stat buf;
//	stat("config", &buf);
//	std::cout << get_date(buf.st_mtimespec.tv_sec);
	return (0);
}
