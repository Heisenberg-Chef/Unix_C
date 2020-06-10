##  前言
+   项目需求:目前需要实现基于客户机和服务器的模型的网络音频点播系统.
    +   本音频系统可以广泛应用在语音教室和公共广播的多种场所,该软件分为服务器和客户机2个部分,服务器运行在PC机上,客户端可运行在PC机或嵌入式设备上,服务器以多波的方式向局域网中所有客户机发送数据,客户机可以根据自己的选择来决定要接受的数据.
  +   广播/点播系统
    +   在server端,守护进程脱离终端!IO脱离终端,出错或者出现异常之后要有系统日志
+ 注意事项:
    1. 弃用root用户
    2. 重构
##  IO
z    +   优先调用标准IO可以屏蔽系统调用的函数.
    +   fopen unix-->open windows-->openfile这样移植性很好.
+   动态内存申请:malloc-->realloc
+   getline:#define _GUN_SOURCE 宏不一定必须有宏值,但是是对别的宏功能的一种封装,它可能存在于其他的文件之中.
    +   这个程序一直是在realloc一点点往外扩大空间.所以再初始化的时候把指针和linesize都安全初始化,在使用完了之后把这些内存都free()掉.这个api容易造成可控的内存泄漏.
    +   getline是GUN中的,不存在标准库中.
+   int fflush(FILE *stream):fflush()是一个C语言标准输入输出的库中的函数,功能是冲洗流中的信息,该函数通常用于处理磁盘文件,fflush会强制缓冲区的数据写入指定的stream中.
    +   如果指针指向一个输出流或者一个最近的一次操作不是输入的更新刘,输出刷新会创建任意未写入的数据给他将要写入的文件流和最近数据呗修改流,然后最后文件状态标记为更新的基础文件的时间戳
    +   对于打开已使用基础文件进行读取的流,如果文件尚未处于EOF,并且该文件是能够搜索的文件,则基础打开文件描述的文件偏移量设置为流的文件位置,并且任何并没独处的ungetc()或者ungetwc()退回到流上的字符都被丢失.
    +   如果stream是空的指针,则fflush将对上面定义行为的所有流执行此刷新操作.
    +   返回值0位成功
+   临时文件的问题:1)不冲突 2)及时销毁 
    +   char * tmpnam(char * s); ---> Create a name for a temporary file.这个api存在于标准io库中.这个api在并发编程的时候,可能会引发时间切片进行抢占.
    +   FILE \*tmpfile(void) ---funciton opens a unique temporary file which binary IO.这个函数产生一个不知道啥名字的文件(w+b)的形式,返回给我一个FILE\*,匿名文件.名字都没有,而且是原子操作,不会出现抢占异常.还可以使用fclose()对他进行关闭.
    +   ulimit -a :查看系统目前IO的数据量
+   异常处理的方案.
    +   perror --> print a system error message. void perror(const char * s);
    +   在<string.h>中有一个strerror,char *strerror(int errnum);
```
code:
        fprintf(stderr,"fopen() failed! errno = %d\n",errno);
        perror("fopen()");
        fprintf(stderr,"fopen() failed! %s\n",strerror(errno));
result:
fopen() failed! errno = 2
fopen(): No such file or directory
fopen() failed! No such file or directory
```
+   fopen()-->stream open functions.成功的话返回一个FILE指针,失败的话返回一个errno.
    +   栈区在函数调用完了就会释放.返回的指针不会再栈中.
    +   静态区 static,是在进程结束之前保留的.但是调用函数之后这个static是固定的,调用几次都只用一个static的结构体区域.
    +   堆,在开辟空间之后在对应的fclose()还可以free,如果函数的返回值有一个指针,并且有一个逆操作的-->一般都在堆区域
    +   mode中w w+ r r+所建立的属性都是**0666 & ~umask**这样创建的一个文件.
        +   umask负责一个用户创建一个文件时候的默认值,它与chmod的效果刚好相反,umask设置的是权限补码,而chmod设置的是文件的权限吗.profile中可以设置umask的值.
        +   umask的值越大,权限越低.
        +   umask:在登陆一个系统之后创建一个文件总是有一个默认的权限,这个
+   一个进程最大打开文件个数:在你打开一个进程时候已经有3个文件已经打开stdin stdout stderr,可以使用ulimit -n 来进行修改.centos默认是1024个.
+   getchar() is equivalent to getc(stdin).
    +   fgetc , getc 这个2函数是不一样的,getc是作为一个宏定义使用的,节省运行时间
+   putchar(c); is equivalent to putc(c,stdout).putc fputc  
    +   同样的putc也是宏定义的,fputc 是函数.这之中的f--> function.
+   fread,fwrite
    +   gets()-->不要用,不检查缓冲区的异常.char \*gets(char \*s),只约定了一个地址,输入缓冲区-->回车-->放入地址,这个地址没有划分空间大小,因此会出现一处问题
    +   char \* fgets(char\*s,int size,FILE \* stream);这个函数是安全的,可以指定大小.这个函数正常结束 size-1 或者遇到\n.
        +   在fgetc的时候读取一个字符串都会引入一个\n \0.
    +   size_t fread(void * ptr,size_t size,size_t nmemb,FILE * stream);
    +   size_t fwrite(const void *ptr,size_t size,size_t nmemb,FILE * stream);
    +   这2个操作都是2进制类型的操作,对于成块的数据操作的时候是非常好用的.但是对于数据的边界没有验证,如果中间有一个字节出现错误,那么所有的数据都会错位.
        +   当前数据量足够的时候,返回1.
        +   当文件当中只有5个字节的时候,不足够的时候,返回0会出现问题.
        +   建议当做单字节使用,如果文件错位,后面的都会错位.
+   sprintf(char *s,format *f,...) -->把数据格式化输出到一个string中
    +   atoi()--->在读取字符串时候不是数字的内容会自动截断.sprintf--->可以作为itoa(并不存在的)....
    +   int snprintf(char *str,size_t size,const char *format,...);防止sprintf越界.
+   fscanf(FILE) sscanf(s).....前者从流中读取,后者从字符串中读取.
+   int fseek(FILE \*stream,long offset,int whence); long ftell(FILE \*stream);void rewind(FILE)  0-2GB,但是现在的文件不是2GB能够承载的.
    +   int fseeko(FILE *stream,off_t offset,int whence); off_t 32 64都有可能----->On many architectures both off_t and long are 32-bit types,but compilation with #define _FILE_OFFSET_BITS 64 will turn off_t into a 64-bit type.
+   fflush():printf中如果不加入\n是不会刷新缓冲区的.
    +   If stream argument is NULL,fflush() flushess all open output steams.
    +   缓冲区的作用:在大多数情况下是好事,合并系统调用 -------     
        1.  行缓冲:换行时候刷新,强制刷新(标准输出是这样的,因为它是一个终端设备)
        2.  全缓冲:慢了的时候刷新,强制刷新(默认模式,只要不是终端设备)
        3.  无缓冲:立即刷新stderr
    +   setvbuf() --> int
     setvbuf(FILE *restrict stream, char *restrict buf, int type,
         size_t size);The setvbuf() function may be used to alter the buffering behavior of a
     stream.  The type argument must be one of the following three macros:

           _IONBF  unbuffered

           _IOLBF  line buffered

           _IOFBF  fully buffered
##  文件IO /系统调用IO
fd是在文件IO中贯穿始终的类型.
+   文件描述符的概念:文件描述符fd int 0 1 2 对于流的一种映射表,有限使用当前范围内最小的.文件描述符对应的FILE \* 结构体 是独立的,就算是打开同一个文件,不同的流吗,也是会相对独立的结构体.
+   open,close,write,read,lseek.标准IO中的都是根据上面5个系统调用IO函数实现的.
    +   int open(const char* pathname,int flags);
        +   RETURN:open() and creat() return a new file descriptor,or -1 if an error occured(文件描述符没有负数这么说,所以会返回-1)
        + flags是一个位选项, flags must include one of the following access modes : O_RDONLY,O_WRONLY or O_RDWR
        +   常用的有一些,O_DIRECT try to minimize cache effects of the I/O to and from this file.buf应该理解成为一个写的缓冲区,cache理解为一个读的缓冲区,不改变文件的修改时间,O_NONBLOCK不阻塞 
        +   **r -> O_RDONLY ,r+ -> O_RDWR , w -> O_WRONLY|O_CREAT|O_TRUNC , w+ -> O_RDWR|O_CREAT|O_TRUNC,0777** 位图的创建.
        +   open函数有两参数和三参数形式.在使用O_CREAT的时候,使用三参数,(重载 -> 定参,多参数 -> 不报错),只有在O_CREAT时候 第三个参数才有用!!!!!一晚上的教训!!!
    +   read():ssize_t read(int fd,void *buf,size_t count);
    +   write():ssize_t write(int fd,const void * buf,size_t count);
    +   lseek();off_t lseek(int fd,off_t offset,int whence);跟fseek()差不多.
+   文件IO与标准IO的区别:
    +   文件IO:是切换系统内核的实现方式
    +   标准IO:在缓冲区中缓存然后整体放入缓冲区的一种调用方式
    +   区别:文件IO响应速度快,标准IO吞吐量大
    +   面试:如何让一个程序变快?--->需要一份为2来作答,响应速度,想让吞吐量变大......从用户体验的角度是吞吐量大才会快,这样可以减少内核的切换频率.
    +   提醒:系统IO和标准IO不能混用.
    +   转换fileno,fopen进行2个IO转换.int fileno(FILE \*stream);FILE * fdopen(int fd);
+   文件共享:int truncate(int fd,off_t length);int ftruncate()
+   原子操作:不可分割的操作,解决的竞争和冲突
+   程序中的重定向:dup,dup2--->dup() duplicates an existing object descriptor and returns its value to
     +  dup :创建一个newfd，newfd指向oldfd的位置，并且dup返回的文件描述符总是取系统当前可用的最小整数值
     +  dup2 :将已有的一个newfd重定向到oldfd的位置，返回第一个不小于oldfd的整数值
     
     复制文件描述符.
+   同步:sync,fsync,fdatasync ------ fcntl(); ioctl();管家级别的函数:
    +   sync -- force completion of pending disk writes (flush cache)-->全局催促.全局刷新流.
    +   fsync -- synchronize a file's in-core state with that on disk-->刷一个文件流
    +   fdatasync--->同步一个文件,但是不刷亚数据
    +   fcntl():文件描述符相关的函数都来来自于这个函数-->fcntl() provides for control over descriptors.  The argument fildes is a descriptor to be operated on by cmd as follows.
    +   iocntl():设备相关的内容都由它来管/dev/....... 
+   虚拟目录:当前进程的文件描述符的信息,ls -al /dev/fd   看得是terminal打开的ls
##  文件系统
做一个myls,可以有一个系统的.-a -i -l
### 目录和文件
+   获取文件的属性:stat fstat lstat
    +   int fstat(int fildes, struct stat *buf);
    +   int stat(const char *restrict path, struct stat *restrict buf);
    +   int lstat(const char *restrict path, struct stat *restrict buf);
```
struct stat { /* when _DARWIN_FEATURE_64_BIT_INODE is NOT defined */
         dev_t    st_dev;    /* device inode resides on */
         ino_t    st_ino;    /* inode's number */ 
         mode_t   st_mode;   /* inode protection mode */
         nlink_t  st_nlink;  /* number of hard links to the file */
         uid_t    st_uid;    /* user-id of owner */
         gid_t    st_gid;    /* group-id of owner */
         dev_t    st_rdev;   /* device type, for special file inode */
         struct timespec st_atimespec;  /* time of last access */
         struct timespec st_mtimespec;  /* time of last data modification */
         struct timespec st_ctimespec;  /* time of last file status change */
         off_t    st_size;   /* file size, in bytes */
         quad_t   st_blocks; /* blocks allocated for file */
         u_long   st_blksize;/* optimal file sys I/O ops blocksize */
         u_long   st_flags;  /* user defined flags for file */
         u_long   st_gen;    /* file generation number */
     };

```
+   空洞文件:空洞文件特点就是offset大于实际的大小,也就是说一个文件两头有数据,中间是空的,\0来填充,系统不会傻到在磁盘上实际存放这么多东西的,第一是一种浪费,第二是一种威胁,因为黑客利用这个漏洞不断侵蚀磁盘资源,计算机就会崩溃.
    +   使用ls展现的是文件的逻辑大小,
    +   du展现的是文件的物理大小."文件系统使用了若干块以存放指向实际数据块的各个指针"。
    +   空洞文件作用很大，例如迅雷下载文件，在未下载完成时就已经占据了全部文件大小的空间，这时候就是空洞文件。下载时如果没有空洞文件，多线程下载时文件就都只能从一个地方写入，这就不是多线程了。如果有了空洞文件，可以从不同的地址写入，就完成了多线程的优势任务。
+   文件权限的更改/管理:在st_mode中,用于表现文件的类型,文件的访问权限,以及特殊权限位置
    +   文件类型:st_mode : dcp-lsp 目录|字符设备文件|块设备文件|常规文件|符号联结文件|sock文件|管道文件    --->   总共7种文件.
    +   UNIX系统提供了7个宏函数,来判断文件的类型.
        1.  S_ISREG(m) is it a regular file.
        2.  S_ISDIR(m) is directory?
        3.  S_ISCHR(m) character device?
        4.  S_ISBLK(m) block device?
        5.  S_ISFIFO(m) FIFO (named pippe)
        6.  S_ISLNK(m) symbolic link?
        7.  S_ISSOCK(m) socket?
    +   也可以用按照位图的值进行比较.
+   umask :0666 & ~umask 是这么一个公式,我的苹果电脑默认是022,002自动生成的是0644
    +   int chmod
    +   int fchmod  //自己去man吧.
+   粘住位: t位,给某一个可执行二进制执行的文件设置t位,在内存中保存它的一些信息,使它调用的更快.
+   文件系统:FAT,UFS(不开源,开源)文件系统:文件系统:文件或者数据的存储和管理
    +   FAT16/32 :静态存储的单链表<arr>,他这种承载能力有限,所以不能像UNIX一样使用文件的模式,而是使用了盘符模式<怕大文件>
        +   swap的交换分区是内存速度的1/200000倍.一些内存清理程序,就是把内存的数据挤到了swap/.360搞了一个进程,吃分区吃满了把其他的挤到swap中.
        +   磁盘清理也是改变链表的顺序
    +   UFS(开源):在最初的FFS文件系统设计中,为了使文件系统在遭到毁灭性打击之后,如硬盘放生整个磁道,整个盘面或者整个柱面损坏时候能够得以恢复,在文件系统初始化的时候,会将文件系统的重要数据复制到整个磁盘的多个位置,以便发生硬件损坏的时候能够读取,而UFS文件系统也保持这个优点<br>另外,为了提高运行效率,UFS文件系统与磁盘的结构也有着完美的结合,UFS将整个磁盘的所有逻辑柱面平均分配为若干个组,每个组成为一个柱面.在UFS内部就用柱面组队文件系统进行分段组织和管理,每个柱面组中都有文件系统关键数据结构的备份,所有文件在
        +   UFS文件系统在创建的时候
+   目录:硬链接,符号链接(软连接):
    +   硬链接:是一种关联,2个指针指向同一个空间.目录项的同义词(不能给分区建立,不能给目录建立)
    +   符号链接:ln -s (symbol) <--一个快捷方式 被记录在inode中.(可以跨分区,可以给目录建立)
    +   int link(const char * oldpath,const char * newpath);
    +   int unlink(const char * pathname);
    +   remove ---> rm 但是文件夹非空的是不能删除的
    +   rename -- change the name of location of a file.
+   utime.h:int utimes(const char *fielname,const struct utimbuf *time)
```
struct utimbuf{
    atime
    mtime
    };
```
+   目录创建和销毁:int chdir(const char * path);脱离控制终端,在umount之后必须切换运行环境,让进程一直在一个存在的环境中运行.
+   更改路径:int fchdir(int fd);
    +   chroot:假根技术,让一个进程在子目录跑,告诉进程当前的目录是一个根目录.顾名思义:change root directory(更改root目录),在linux系统中,系统默认的结构是以'/',经过chrootHi后,系统读取道德目录文件将不是在旧的系统根目录下,而是一个新的根下.
        1.  增加了系统的安全性,限制了用户的权利:经过chroot之后,在新的根下将访问不到旧系统的根目录结构和文件,这样就增强了系统的安全性,这个一般是在登陆(login)和使用chroot,以达到用户不能访问一些特定的文件.
        2.  建立一个与原系统隔离的系统目录结构,方便用户的开发:使用chroot之后,系统读取的是新的根下的目录和文件,这是一个原系统不相关的目录结构,在这个新的环境下,可以用来测试软件的静态编译以及与系统不相关的独立开发.
        3.  切换系统的根目录位置,引导Linux系统启动以及急救系统:chroot的作用就是切换系统的根位置,而这个作用最为明显的就是在系统初始化引导磁盘的处理过程中使用,从RAM磁盘切换到系统的根位置并执行真正的init.另外当系统出现一些问题时候也可以使用chroot来切换到一个临时的系统.
+   glob(3):搜索匹配函数,是我认为最方便的目录操作函数,包含在glob.h库中,操作结构体<pre>typedef struct {
             size_t gl_pathc;        /* count of total paths so far */
             int gl_matchc;          /* count of paths matching pattern */
             size_t gl_offs;         /* reserved at beginning of gl_pathv */
             int gl_flags;           /* returned flags */
             char **gl_pathv;        /* list of paths matching pattern */
     } glob_t;</pre>
+   目录流操作:
    +   opendir() :打开一个与给定的目录name相对应的目录流,并返回一个指向该目录流的指针,打开后,该目录指向了目录中的第一个目录项,RETURN VALUE,打开成功指向目录的流指针;打开失败返回NULL并且设置相应的errno
    +   closedir():函数关闭与指针dir相联系的目录流,关闭后,目录流描述符dir不再可用.
    +   readdir(3) :该函数返回一个指向dirent的结构体指针,该结构体代表了由dir指向的目录流中的下一个目录项;如果读到了EOF或者出现错误那么则返回NULL<pre>       struct dirent { 
             ino_t          d_ino;       /* inode number */ 
             off_t          d_off;       /* offset to the next dirent */ 
             unsigned short d_reclen;    /* length of this record */ 
             unsigned char  d_type;      /* type of file */ 
             char           d_name[256]; /* filename */ 
         };</pre>
    +   rewinddir()
    +   seekdir()
    +   telldir()
+   分析目录/读取目录内容:int getcwd -->get current working path.
+   一般使用递归,但是要设置一个递归上线,防止栈破裂.
### 系统数据文件和信息
```
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
     
struct passwd {
                   char    *pw_name;       /* user name */
                   char    *pw_passwd;     /* encrypted password */
                   uid_t   pw_uid;         /* user uid */
                   gid_t   pw_gid;         /* user gid */
                   time_t  pw_change;      /* password change time */
                   char    *pw_class;      /* user access class */
                   char    *pw_gecos;      /* Honeywell login info */
                   char    *pw_dir;        /* home directory */
                   char    *pw_shell;      /* default shell */
                   time_t  pw_expire;      /* account expiration */
                   int     pw_fields;      /* internal: fields filled in */
           };
```
+   /etc/passwd:在不同的OS中存储的信息是不同的因此需要标准的函数进行**和稀泥操作**
    +   getpwuid
    +   getpwnam

+   /etc/group :组信息.
    +   getgrgid()
    +   getgrnam();
+   /etc/shadow:macos没有shadow文件.在linux中,存储密码的.
    +   hash不是加密,是一种混淆算法. 哈希只能混淆但是不能解密,哈希加密的密码混淆的是相同的,可以用来撞库.(防备管理员监守自盗)
    +   加密:可以用穷举进行破解,口令随机校验.
        +   口令随机效验:2次成功才成功,登陆界面会让你重新输入一遍,他会随机效验.
        +   安全:攻击成本大于收益
        +   char * crypt(const char *key, const char *salt);salt = $ID$salt$ , 1 MD5 2a: 5 SHA-256 6 SHA-512
        +   getpass("...");s
+   时间戳:
    +   time();从内核中取出来一个时间戳,这个时间戳年份是从1900年开始的,月份是从0月开始的.
    +   localtime()
    +   gmtime() --> 格林威治时间
    +   mktime() --> 把一个越界结构体转变成一个时间戳,并且将越界部分计算完毕:The functions mktime() and timegm() convert the broken-out time (in the
     structure pointed to by *timeptr) into a time value with the same encod-
     ing as that of the values returned by the time(3) function
    +   strftime() --> 放在静态类型中,赶紧用,转化完了放一会可能就变了
+   有一些bug可以对客户说是特性,一层窗户纸的事儿.
### 进程环境
+   main函数:
    +   int main(int argc, char ** argv) ----> 
+   进程的终止:
    +   正常终止:
        1. 从main函数返回,
        2. exit().
        3. 调用\_exit或者\_EXIT ---> 系统调用
        4. 最后一个进程从启动例程返回.
        5. 最后一个线程调用了pthread_exit
    +   异常终止:
        1.  abort调用,弃用一个进程
        2.  接到一个信号并终止 cmd + c
        3.  最后一个线程对其取消请求作出相应.
    +   父进程看子进程的退出状态echo $?:
        +   void exit(int status);The C Standard (ISO/IEC 9899:1999 (``ISO C99'')) defines the values 0,EXIT_SUCCESS, and EXIT_FAILURE as possible values of status.  Cooperatingprocesses may use other values; in a program which might be called by amail transfer agent, the values described in sysexits(3) may be used to provide more information to the parent process.
        +   exit函数的参数的退出状态是 status & 0377 言外之意 只保留了第八位有限字符,所以exit返回值是-128 到 127的 255个数字
        +   _exit(),直接做退出,不做IO清理,abort
    +   atexit() and on_exit():钩子函数
        +   atexit注册一个函数,在进程正常终止之前被调用,释放没有释放的内容,类似**C++的析构函数**,把逆操作放在里面.
+   命令行参数的分析:
    +   getopt --> 跟python的那个getoptparse差不多
    +   getopt_long():直接把man手册的例子贴在下面吧
```
//  如有有冒号 可以使用全局变量
     extern char *optarg;
     extern int optind;
     extern int optopt;
     extern int opterr;
     extern int optreset;

     int
     getopt(int argc, char * const argv[], const char *optstring);
     
EXAMPLES
     #include <unistd.h>
     int bflag, ch, fd;

     bflag = 0;
     while ((ch = getopt(argc, argv, "bf:")) != -1) {
             switch (ch) {
             case 'b':
                     bflag = 1;
                     break;
             case 'f':
                     if ((fd = open(optarg, O_RDONLY, 0)) < 0) {
                             (void)fprintf(stderr,
                                 "myname: %s: %s\n", optarg, strerror(errno));
                             exit(1);
                     }
                     break;
             case '?':
             default:
                     usage();
             }
     }
     argc -= optind;
     argv += optind;
``` 
在const char * s传入需要代入的字符,如果有需要进行参数进行修饰的,加上一个冒号':s'
+   环境变量:使用export可以导出所有的环境变量[Key = Value]
    +   environ(char **) ,字符串数组.这是一个全局变量,extern char ** environ;
    +   getenv(const char *): 向函数中传入环境变量名字,它会返回环境变量的值.
    +   setenv(const char * name,const char * value,int overwrite);如果name存在如果不存在.
    +   putenv()
+   C程序的存储空间布局:
    +   x86:
        +   0-3G:user:0xc0000000
        +   3G-4G:kernel:0x08048000
        +   在i386上,由于历史原因,可执行文件的基本地址为0x8048000,这样做的原因是当栈堆直接放在可执行文件的下面时,允许在本文下方有相当大的堆栈,而仍保持在地址0x08000000上方,从而允许堆栈和可执行文件可以共存于同一个第二页表中.<br>如果栈溢出了,我们希望进程崩溃而不是覆盖其他的数据,因此栈应该有很大很大的空间,同时栈增长到这片空间的末端时,进程会崩溃,空指针解引用会导致进程崩溃,这是一个可以方便利用的特质,所以定义了除了第一页外,栈获得前128MB的地址空间,这意味着代码必须在128MB以后,所以地址类似于0x08000000.<br>据说,在NIX/i386上,0x08048000曾经是STACK_TOP(也就是说,堆栈从0x08048000附近向下增长到0)
    +   x64
        +   amd64使用更大的地址,存在一些技术限制,所以建议将2GB的地址空间专门用于代码和数据来提高效率,所以栈被移除了该范围.<br>0x40000(4MB)足够大,可以补货任何可能不正常的结构偏移(指针指向0对象,获取对象中成员变量),从而允许amd64运行较大的数据单元,但是有足够小不会浪费太多的那2GB地址
        +   0x400000(4MB)迄今为止最大的实际页面大小,并且目前所有其他虚拟内存的单元大小的公倍数.

|segment|含义|
|:---:|:---:|
|argument & environment|命令行和环境变量(环境表和环境字符串)|
|stack|栈|
|heap|堆|
|bss|未初始化数据段|
|data|初始化数据段|
|text|正文段(代码段)|

1.  栈(stack):由编译器自动分配释放,存放函数的参数值,局部变量的值等等(但是不包括static声明的变量,static意味着在数据段中存放变量).除此之外,在函数被调用时,栈用来传递参数和返回值.由于栈的先进后出的特点,所以栈特别方便是用来保存和回复数据.其操作方式类似于数据结构中的栈.
2.  堆(heap):堆是用于存放进程运行中被动态分配的内存段,它的大小并不固定,可以动态扩张或者缩小,当进程调用malloc/free等函数分配内存时候,新分配的内存就被动态添加到了堆上(堆被扩张)/释放的内存从堆中被提出(堆被缩减).堆一般由程序员分配释放,若程序员不进行释放,程序结束时候可能由OS回收.**它与数据结构的堆是两回事,分配方式类似于链表**.
3.  全局数据去(静态区)static:全局变量和静态变量的存储是放在一块的,初始化的全局变量和静态变量在一块区域,未初始化的全局变量和静态变量在相邻的另一块区域,另外文字常量区,常量字符串就是放在这里,程序接触后由OS系统.
    1.  BSS段(bss segment):通常是指哟领你刚来存放程序中**未初始化**的全局变量的一块内存区域,bss段属于静态内存分配.
    2.  数据段(data segment):通常是用来存存放**已初始化**的全局变量的一块内存区域,数据段属于静态内存分配.
4.  代码段(code segment):通常指用来存放程序执行代码的一块内存区域.这部分区域的大小在程序执行之前就已经确定了,并且内存区域通常属于只读.(某些框架也允许代码为可写,即可以修改程序),在代码段中,也有可能包含一些只读的常数变量,例如字符串常量等,程序段为程序代码在内存中的映射,一个程序可以在内存中有多个副本.
    +   使用pmap(shell)可以看所有映射的地址

+   库:
    +   动态库
    +   静态库
    +   手工装载库:内核中的库就是一些插件,如果能够加载则加载....
        +   dlopen()
        +   dlclose()
        +   dlerror()
        +   dlsym()
+   函数跳转
        +   getconf PAGE_SIZE ---->4096 4k一页,默认的.
        +   goto:不能执行跨函数的跳转
        +   catch---throw
        +   ret = setjmp(jmp_buf save) ---> 设置跳转点,ret为跳转函数进行跳转的时候带回来的信号值
        +   longjmp(jmp_buf,int val) ---> 跳转   可以安全的跳转 长跳转
+   资源的获取以及控制
    +   getrlimit();
    +   setrlimit();
### 线程与进程
#### 进程基本知识
1.  进程标识符pid.
    +   类型pid_t
    +   ps命令
    +   进程标识是顺次向下使用的
    +   getpid():获得当前进程的进程号
    +   getppid():获得父进程的进程号
2.  进程的产生
    +   fork():memory copy-->稍微有一些不一样其他都差不多
        +   duplicating意味着拷贝,克隆,一模一样等含义,
        +   fork后的父子进程的区别,fork的返回值不一样,pid不同,ppid也不同,未决信号和文件锁不继承,资源利用量清0.
        +   init:1号,是所有进程的祖先进程:
        +   **调度器**的调度策略来决定是那个进程先运行.
        +   **在fork之前刷新该刷新的流 fflush(NULL)**. 在终端上打印是\n一个begin 但是再重定向 > 不是行缓冲模式----> 是2个begin.
        +   wc -l 记录输出字节,行数等等.
        +   S:睡眠态(sleep):子进程还没执行完,父进程已经退出,所以系统会把这些没有父亲的进程收回来,给了init总进程.
        +   Z:僵尸态(zombie),不是很占内存,只有一个结构体还保存这退出状态.不会占很多的资源,但是pid没有释放.----> 父亲状态没有正常退出,子进程已经执行完了.
    +   vfork()[快废弃了]:fork成本特别高,vfork创建的内容,只能成功调用exec函数组中的某一个函数,其他的都是未定义行为.
        +   不过现在fork函数已经有加强,有一个写实拷贝技术,在进行fork的时候,我的子进程与父进程用的确实是一个公用数据块,如果是只读不写的模块父子进程都不会进行改变,就会进行拷贝并且写入(谁改谁拷贝).
3.  进程的消亡以及释放资源
    +   wait()
    +   waitpid()
    +   waitid()
    +   wait3()
    +   wait4()
    +   分快法分配:
    +   交叉分配法:
    +   池类算法:抢占式任务分配.
+   补充:进程的状态:
    +   R(TASK_RUNNING),可执行状态:只有在这个该状态下进程才可以在CPU上运行,而同一个时刻可能有多个进程处于执行态,这些进程的task\_struct结构体(进程控制块)被放入对应的CPU可执行序列中分别学则一个进程在一个CPU上面跑.*在很多的参考书上,正在CPU上运行的进程被定义为RUNNING状态,而将可执行但是尚未被调度的执行进程定义为READY状态,但是再LINUX下统一定义为TASK_RUNNING*
    +   S(TASK_INTERRUPTIBLE)可中断的睡眠态:处于这个状态的进程因为等待某某事件的发生(例如等待socket,等待信号量),而被挂起,这些进程的task\_struct结构被放入对应的事件的等待队列中,当这些事件发生时候(由外部中断触发,或者由其他进程出发),对应的等待结构中的一个或者多个进程将被唤醒.通过ps命令我么会看到,一般情况下,进程列表中的绝大对数进程都处于TASK_INTERRUPTIBLE状态(除非机器负载很高),毕竟CPU就4 - 8核,上百个进程必须来回切换这运行.
    +   D(TASK_UNINTERRUPTIBLE),不可中断的睡眠状态:与TASK_INTERRUPTIBLE状态类似,进程处于睡眠状态,但是此刻进程是不可中断的.<此刻的不可中断指的并不是CPU不相应外部硬件的终端,而是指进程不响应**异步信号**>.绝大多数情况下,进程处于睡眠状态时,总是应该能够响应异步信号.否则你将惊奇的发现kill -9竟然杀不死一个正在睡眠的进程.ps也看不到D状态的进程.<br>D状态存在的意义就在于,内核的某些处理流程是不可以被打断的,如果响应异步信号,程序的执行流程中就会被插入一段用于处理异步信号的流程,D状态就是搞一个原子态.使用vfork之后父进程将进入D状态,直到子进程调用exit或者exec
    +   T(TASK_STOPPED or TASK_TRACED).暂停状态或跟踪状态:向进程发送一个SIGSTOP信号,他就会因响应该信号而进入T状态(除非该进程本身处于TASK_UNINTERRUPTIBLE而不响应信号),SIGSTOP和SIGKILL信号一样,是非常强制的,不允许用户进程通过signal系列的系统调用重新设置对应的信号处理函数,向进程发送一个SIGCONT信号,可以让其从T --- > R状态
    +   Z(TASK_DEAD-EXIT_ZOMBIE),退出状态,进程成为僵尸进程:在进程突出的过程中,处于TASK_DEAD状态.在这个退出过程中,进程占有资源将被回收,除了**task_struct(以及极少数的资源)**,于是只剩下了一个task_struct的空壳,故称作僵尸.之所以保留task_struct是因为这个结构里面存储了程序的退出码以及一些统计信息,其父进程很可能会关注这些信息,比如在shell中使用$?变量就保存了前台进程的退出码,而这个退出码往往被作为if语句的判断条件.当然了,内核也可以将这些信息保存在别的地方,而task_struct结构释放掉,以节省一些时间,但是使用task_struct结构更为方便,因为在内核中已经建立了从Pid到task_struct的查找关系,还有进程间的父子进程的关系.<br>父进程通过使用wait系列的函数来等待某个某些子进程的退出,并获得它的退出信息.然后wait系列的系统调用混顺便将子进程的尸体也释放掉.子进程在退出的过程中,内核或给其父亲进程发送一个信号,通知父进程来"收尸",这个信号默认是SIGCHLD.如果父亲进程已经不存在了,那么就是给我们的NO.1进程init.让它来收尸
    +   X(TASK_DEAD-EXIT_DEAD),退出状态,进程即将被销毁:而进程在退出的过程中可能不会保留它的task_struct,比如这个进程是多线程程序中被detach过的进程.或者父进程通过设计SIGCHLD信号的handler为SIG_IGN显示忽略了SIGCHLD信号,此时进程将被置于EXIT_DEAD退出状态,这意味着接下来的代码立即就会将该进程彻底释放掉.所以EXIT_DEAD装备是机器短暂的,几乎不能被ps命令捕捉到.
+   文件权限管理:linux文件有三种权限:用户权限,群组权限,其他用户权限,非别为rwx
    +   u+s用户权限:执行chmod u+s 就是针对某个程序任何永不都有读写这个程序的权限,可以向root用户一样操作,这个指令只对程序有效,如果这个指令放在路径上是无效的.
    +   g+s群组权限:强制将次群组里的目录下文件编入此群组,无论哪个用户创建的文件
4.  exec函数族的使用:
    +   fork进行的是一个分支拷贝
    +   exec是创建一个新的任务,bash-->main 一个意思,我们的bash中并不存在main的代码,但是可以执行
    +   If any
    +    of the exec() functions returns,an error will have occurred. The return value is -1,and errno will be set to indicate the error.
+   shell -> exec -> wait:子进程会继承父进程的0 , 1 , 2设备,文件描述符也可以继承.可以通过文件进行交互,可以说UNIX世界就是fork exec wait搭建起来的.
+   对于自制的bash终端,当我们建立一个动态数组来存储串,使用int glob().GLOB_NOCHECK
+   当我们的机器开机,第一个进程是init进程,之后会产生一个fork+exec产生一个getit进程name: passwd -->exec-->login ---> checkpass 成功---> shell 切换到当前的用户,我的身份目前为止是固定了.
5.  u+x g+x:
+   u+s:让所有的用户都拥有所有人的权限,如果使用chown root 则其他用户也可以使用root权限运行这个文件.
+   g+s:群组权限出现s的情况，执行命令是chmod g+s，它的意思是强制将此群组里的目录下文件编入到此群组中，无论是哪个用户创建的文件。
+   **其他权限出现t的情况，命令是chmod o+t，意思是这个目录只有root和此目录的拥有者可以删除，其他用户全都不可以。**
+   getuid():return real user ID of calling process
+   geteuid():return effective user ID
+   getgid()
+   getegid()
+   setuid() :set the effective user ID of calling process
+   setgid():
+   setreuid():交换ruid euid
+   setregid():交换rgid egid
+   seteuid():更改有效用户ID
+   setegid():更改有效用户组ID
6.  解释器文件:./a.sh chmod u+x ....在装载这个文件的时候发现#!/bin/bash这个标记的时候,然后会装载一个脚本解释器,然后使用指定的解释器来解释所有指令.#!就是一个脚本的注释,shell一看到#!之后马上吧这个脚本解释器装载进来,然后再进行执行.如果#!/bin/cat
7.  system() ->(2,3,4的封装)   ---> system() executes a command specified in command by /bin/sh -c command
8.  进程会计:acct
9.  进程时间:time ./.....
10.  守护进程:开机之后自动启动,脱离控制终端,是一个会话的领导者,也是一个进程组的领导者,守护进程在httpd,qq服务器,wechat服务器,发挥作用,他不会在我登陆QQ的时候才启动.<br>会话:终端--->出现在早期,设备非常昂贵的时候,不可能一个人一台电脑,一个公司就有一台而已,多个人以终端的形式进行访问[例如在银行中,要求安全性特别高的单位才能看到终端],实际上一个终端的用户登录就是一个会话(session)
+   前台进程组:最多只能有一个.
    +   后台进程组:重定向 0 1 2 ---> log.txt
    +   setsid():The setsid function creates a new session.  The calling process is the
     session leader of the new session, is the process group leader of a new
     process group and has no controlling terminal.  The calling process is
     the only process in either the session or the process group.调用的进程如果不是一个会话的leader,则那么就会打开一个新的会话并把他做成这个会话的leader.
    +   getpgrp():
    +   int setpgid(pid_t pid,pid_t pgid)
    +   getpgid():当进程是会话的领头进程时setsid()调用失败并返回（-1）。setsid()调用成功后，返回新的会话的ID，调用setsid函数的进程成为新的会话的领头进程，并与其父进程的会话组和进程组脱离。
        +   open("/dev/null",o_rdwr)作用:在开始一个守护进程的时候都会这样做得到的解释就是(make sure we have stdin/stdout/stderr reserved),写入/dev/null的东西会被系统丢掉,就如同写注释一样,把012分配出去,以后再分配的时候不会将这3个流打开,以达到保护的目的.
由于会话对控制终端的独占性，进程同时与控制终端脱离。
11.  系统日志
+   syslogd:统一由一个守护进程,它来写系统日志.-->对系统的权限分离.
    +   openlog(char * ident,int opt,int facility); --- > 人物字段,选项,从哪里来
    +   syslog(int priority,char *format,...);   //可以当做printf来用.
    +   closelog()
    +   单个实例的进程:锁文件 /var/run/name.pid,每次开启这个进程,就会在这个目录下面生成一个锁文件.
    +   启动脚本文件:/etc/rc*.... 可以在这里面加上.
## 并发
信号与线程很少一块使用.不要大范围的混用信号和线程
### 信号来实现并发
Semaphore..

同步:电脑上的都是同步事件.

异步:什么时候到来我们不知道,不可打断的就是一条机器指令,单核机器中每个进程轮流使用机器的世界.
1.  查询法: 一直在扫描,异步时间发生的比较稠密.while 死循环 去读取状态位.开销太大了.
2.  通知法: 如果异步的时间发生的比较稀疏.当某一位触发事件之后,再进行一系列的操作.
####    信号的概念
信号是软件中断,信号的响应依赖于中断机制,底层来讲是中断来实现了软件的信号机制,可以理解为软件中断.
####    signal()
+   kill -l:可以查看所有的信号
```
HUP INT QUIT ILL TRAP ABRT EMT FPE KILL BUS SEGV SYS PIPE ALRM TERM URG STOP TSTP CONT CHLD TTIN TTOU IO XCPU XFSZ VTALRM PROF WINCH INFO USR1 USR2
34 - 64 realtime signal 实时信号
```
<ol>
<li>SIGHUP<br>
本信号在用户终端连接(正常或非正常)结束时发出, 通常是在终端的控制进程结束时, 通知同一session内的各个作业, 这时它们与控制终端不再关联。<br>
登录Linux时，系统会分配给登录用户一个终端(Session)。在这个终端运行的所有程序，包括前台进程组和后台进程组，一般都属于这个Session。当用户退出Linux登录时，前台进程组和后台有对终端输出的进程将会收到SIGHUP信号。这个信号的默认操作为终止进程，因此前台进程组和后台有终端输出的进程就会中止。不过可以捕获这个信号，比如wget能捕获SIGHUP信号，并忽略它，这样就算退出了Linux登录，wget也能继续下载。<br>
此外，对于与终端脱离关系的守护进程，这个信号用于通知它重新读取配置文件。</li>
<li>SIGINT<br>
程序终止(interrupt)信号, 在用户键入INTR字符(通常是Ctrl-C)时发出，用于通知前台进程组终止进程。</li>
<li>SIGQUIT<br>
和SIGINT类似, 但由QUIT字符(通常是Ctrl-)来控制. 进程在因收到SIGQUIT退出时会产生core文件, 在这个意义上类似于一个程序错误信号。</li>
<li>SIGILL<br>
执行了非法指令. 通常是因为可执行文件本身出现错误, 或者试图执行数据段. 堆栈溢出时也有可能产生这个信号。</li>
<li>SIGTRAP<br>
由断点指令或其它trap指令产生. 由debugger使用。</li>
<li>SIGABRT<br>
调用abort函数生成的信号。</li>
<li>SIGBUS<br>
非法地址, 包括内存地址对齐(alignment)出错。比如访5261问一个四个字长的整数, 但其地址不是4的倍数。它与SIGSEGV的区别在于后者是由于对合法存储地址的非法访问触发的(如访问不属于自己存储空间或只读存储空间)。</li>
<li>SIGFPE<br>
在发生致命的算术运算错误时发出. 不仅包括浮点运算错误, 还包括溢出及除数为0等其它所有的算术的错误。</li>
<li>SIGKILL<br>
用来立即结束程序的运行. 本信号不能被阻塞、处理和忽略。如果管理员发现某个进程终止不了，可尝试发送这个信号。</li>
<li>SIGUSR1<br>
留给用户使用</li>
<li>SIGSEGV<br>
试图访问未分配给自己的内存, 或试图往没有写权限的内存地址写数据.</li>
<li>SIGUSR2<br>
留给用户使用</li>
<li>SIGPIPE<br>
管道破裂。这个信号4102通常在进程间通信产生，比如采用FIFO(管道)通信的两个进程，读管道没打开或者意外终止就往管道写，写进程会收到SIGPIPE信号。此外用Socket通信的两个进程，写进程在写Socket的时候，读进程已经终止。</li>
<li>SIGALRM<br>
时钟定时信号, 计算的是实际的时间或时钟时间. alarm函数使用该信号.</li>
<li>SIGTERM<br>
程序结束(terminate)信号, 与SIGKILL不同的是该信号可以被阻塞和处理。通常用来要求程序自己正常退出，shell命令kill缺省产生这个信号。如果进程终止不了，我们才会尝试SIGKILL。</li>
<li>SIGCHLD<br>
子进程结束时1653, 父进程会收到这个信号。<br>
如果父进程没有处理这个信号，也没有等待(wait)子进程，子进程虽然终止，但是还会在内核进程表中占有表项，这时的子进程称为僵尸进程。这种情况我们应该避免(父进程或者忽略SIGCHILD信号，或者捕捉它，或者wait它派生的子进程，或者父进程先终止，这时子进程的终止自动由init进程来接管)。</li>
<li>SIGCONT<br>
让一个停止(stopped)的进程继续执行. 本信号不能被阻塞. 可以用一个handler来让程序在由stopped状态变为继续执行时完成特定的工作. 例如, 重新显示提示符</li>
<li>SIGSTOP<br>
停止(stopped)进程的执行. 注意它和terminate以及interrupt的区别:该进程还未结束, 只是暂停执行. 本信号不能被阻塞, 处理或忽略.</li>
<li>SIGTSTP<br>
停止进程的运行, 但该信号可以被处理和忽略. 用户键入SUSP字符时(通常是Ctrl-Z)发出这个信号</li>
<li>SIGTTIN<br>
当后台作业要从用户终端读数据时, 该作业中的所有进程会收到SIGTTIN信号. 缺省时这些进程会停止执行.</li>
<li>SIGTTOU<br>
类似于SIGTTIN, 但在写终端(或修改终端模式)时收到.</li>
<li>SIGURG<br>
有"紧急"数据或out-of-band数据到达socket时产生.</li>
<li>SIGXCPU<br>
超过CPU时间资源限制. 这个限制可以由getrlimit/setrlimit来读取/改变。</li>
<li>SIGXFSZ<br>
当进程企图扩大文件以至于超过文件大小资源限制。</li>
<li>SIGVTALRM<br>
虚拟时钟信号. 类似于SIGALRM, 但是计算的是该进程占用的CPU时间.</li>
<li>SIGPROF<br>
类似于SIGALRM/SIGVTALRM, 但包括该进程用的CPU时间以及系统调用的时间.</li>
<li>SIGWINCH<br>
窗口大小改变时发出.</li>
<li>SIGIO<br>
文件描述符准备就绪, 可以开始进行输入/输出操作.</li>
<li>SIGPWR<br>
Power failure</li>
<li>SIGSYS<br>
非法的系统调用。<br>
在以上列出的信号中，程序不可捕获、阻塞或忽略的信号有：SIGKILL,SIGSTOP<br>
不能恢复至默认动作的信号有：SIGILL,SIGTRAP<br>
默认会导致进程流产的信号有：SIGABRT,SIGBUS,SIGFPE,SIGILL,SIGIOT,SIGQUIT,SIGSEGV,SIGTRAP,SIGXCPU,SIGXFSZ<br>
默认会导致进程退出的信号有：SIGALRM,SIGHUP,SIGINT,SIGKILL,SIGPIPE,SIGPOLL,SIGPROF,SIGSYS,SIGTERM,SIGUSR1,SIGUSR2,SIGVTALRM<br>
默认会导致进程停止的信号有：SIGSTOP,SIGTSTP,SIGTTIN,SIGTTOU<br>
默认进程忽略的信号有：SIGCHLD,SIGPWR,SIGURG,SIGWINCH<br>
此外，SIGIO在SVR4是退出，在4.3BSD中是忽略；SIGCONT在进程挂起时是继续，否则是忽略，不能被阻塞</li>
</ol>
<li>SIGHUP<br>
本信号在用户终端连接(正常或非正常)结束时发出, 通常是在终端的控制进程结束时, 通知同一session内的各个作业, 这时它们与控制终端不再关联。<br>
登录Linux时，系统会分配给登录用户一个终端(Session)。在这个终端运行的所有程序，包括前台进程组和后台进程组，一般都属于这个Session。当用户退出Linux登录时，前台进程组和后台有对终端输出的进程将会收到SIGHUP信号。这个信号的默认操作为终止进程，因此前台进程组和后台有终端输出的进程就会中止。不过可以捕获这个信号，比如wget能捕获SIGHUP信号，并忽略它，这样就算退出了Linux登录，wget也能继续下载。<br>
此外，对于与终端脱离关系的守护进程，这个信号用于通知它重新读取配置文件。</li>
<li>SIGINT<br>
程序终止(interrupt)信号, 在用户键入INTR字符(通常是Ctrl-C)时发出，用于通知前台进程组终止进程。</li>
<li>SIGQUIT<br>
和SIGINT类似, 但由QUIT字符(通常是Ctrl-)来控制. 进程在因收到SIGQUIT退出时会产生core文件, 在这个意义上类似于一个程序错误信号。</li>
<li>SIGILL<br>
执行了非法指令. 通常是因为可执行文件本身出现错误, 或者试图执行数据段. 堆栈溢出时也有可能产生这个信号。</li>
<li>SIGTRAP<br>
由断点指令或其它trap指令产生. 由debugger使用。</li>
<li>SIGABRT<br>
调用abort函数生成的信号。</li>
<li>SIGBUS<br>
非法地址, 包括内存地址对齐(alignment)出错。比如访5261问一个四个字长的整数, 但其地址不是4的倍数。它与SIGSEGV的区别在于后者是由于对合法存储地址的非法访问触发的(如访问不属于自己存储空间或只读存储空间)。</li>
<li>SIGFPE<br>
在发生致命的算术运算错误时发出. 不仅包括浮点运算错误, 还包括溢出及除数为0等其它所有的算术的错误。</li>
<li>SIGKILL<br>
用来立即结束程序的运行. 本信号不能被阻塞、处理和忽略。如果管理员发现某个进程终止不了，可尝试发送这个信号。</li>
<li>SIGUSR1<br>
留给用户使用</li>
<li>SIGSEGV<br>
试图访问未分配给自己的内存, 或试图往没有写权限的内存地址写数据.</li>
<li>SIGUSR2<br>
留给用户使用</li>
<li>SIGPIPE<br>
管道破裂。这个信号4102通常在进程间通信产生，比如采用FIFO(管道)通信的两个进程，读管道没打开或者意外终止就往管道写，写进程会收到SIGPIPE信号。此外用Socket通信的两个进程，写进程在写Socket的时候，读进程已经终止。</li>
<li>SIGALRM<br>
时钟定时信号, 计算的是实际的时间或时钟时间. alarm函数使用该信号.</li>
<li>SIGTERM<br>
程序结束(terminate)信号, 与SIGKILL不同的是该信号可以被阻塞和处理。通常用来要求程序自己正常退出，shell命令kill缺省产生这个信号。如果进程终止不了，我们才会尝试SIGKILL。</li>
<li>SIGCHLD<br>
子进程结束时1653, 父进程会收到这个信号。<br>
如果父进程没有处理这个信号，也没有等待(wait)子进程，子进程虽然终止，但是还会在内核进程表中占有表项，这时的子进程称为僵尸进程。这种情况我们应该避免(父进程或者忽略SIGCHILD信号，或者捕捉它，或者wait它派生的子进程，或者父进程先终止，这时子进程的终止自动由init进程来接管)。</li>
<li>SIGCONT<br>
让一个停止(stopped)的进程继续执行. 本信号不能被阻塞. 可以用一个handler来让程序在由stopped状态变为继续执行时完成特定的工作. 例如, 重新显示提示符</li>
<li>SIGSTOP<br>
停止(stopped)进程的执行. 注意它和terminate以及interrupt的区别:该进程还未结束, 只是暂停执行. 本信号不能被阻塞, 处理或忽略.</li>
<li>SIGTSTP<br>
停止进程的运行, 但该信号可以被处理和忽略. 用户键入SUSP字符时(通常是Ctrl-Z)发出这个信号</li>
<li>SIGTTIN<br>
当后台作业要从用户终端读数据时, 该作业中的所有进程会收到SIGTTIN信号. 缺省时这些进程会停止执行.</li>
<li>SIGTTOU<br>
类似于SIGTTIN, 但在写终端(或修改终端模式)时收到.</li>
<li>SIGURG<br>
有"紧急"数据或out-of-band数据到达socket时产生.</li>
<li>SIGXCPU<br>
超过CPU时间资源限制. 这个限制可以由getrlimit/setrlimit来读取/改变。</li>
<li>SIGXFSZ<br>
当进程企图扩大文件以至于超过文件大小资源限制。</li>
<li>SIGVTALRM<br>
虚拟时钟信号. 类似于SIGALRM, 但是计算的是该进程占用的CPU时间.</li>
<li>SIGPROF<br>
类似于SIGALRM/SIGVTALRM, 但包括该进程用的CPU时间以及系统调用的时间.</li>
<li>SIGWINCH<br>
窗口大小改变时发出.</li>
<li>SIGIO<br>
文件描述符准备就绪, 可以开始进行输入/输出操作.</li>
<li>SIGPWR<br>
Power failure</li>
<li>SIGSYS<br>
非法的系统调用。<br>
在以上列出的信号中，程序不可捕获、阻塞或忽略的信号有：SIGKILL,SIGSTOP<br>
不能恢复至默认动作的信号有：SIGILL,SIGTRAP<br>
默认会导致进程流产的信号有：SIGABRT,SIGBUS,SIGFPE,SIGILL,SIGIOT,SIGQUIT,SIGSEGV,SIGTRAP,SIGXCPU,SIGXFSZ<br>
默认会导致进程退出的信号有：SIGALRM,SIGHUP,SIGINT,SIGKILL,SIGPIPE,SIGPOLL,SIGPROF,SIGSYS,SIGTERM,SIGUSR1,SIGUSR2,SIGVTALRM<br>
默认会导致进程停止的信号有：SIGSTOP,SIGTSTP,SIGTTIN,SIGTTOU<br>
默认进程忽略的信号有：SIGCHLD,SIGPWR,SIGURG,SIGWINCH<br>
此外，SIGIO在SVR4是退出，在4.3BSD中是忽略；SIGCONT在进程挂起时是继续，否则是忽略，不能被阻塞</li>
<ol>
<li>SIGHUP<br>
本信号在用户终端连接(正常或非正常)结束时发出, 通常是在终端的控制进程结束时, 通知同一session内的各个作业, 这时它们与控制终端不再关联。<br>
登录Linux时，系统会分配给登录用户一个终端(Session)。在这个终端运行的所有程序，包括前台进程组和后台进程组，一般都属于这个Session。当用户退出Linux登录时，前台进程组和后台有对终端输出的进程将会收到SIGHUP信号。这个信号的默认操作为终止进程，因此前台进程组和后台有终端输出的进程就会中止。不过可以捕获这个信号，比如wget能捕获SIGHUP信号，并忽略它，这样就算退出了Linux登录，wget也能继续下载。<br>
此外，对于与终端脱离关系的守护进程，这个信号用于通知它重新读取配置文件。</li>
<li>SIGINT<br>
程序终止(interrupt)信号, 在用户键入INTR字符(通常是Ctrl-C)时发出，用于通知前台进程组终止进程。</li>
<li>SIGQUIT<br>
和SIGINT类似, 但由QUIT字符(通常是Ctrl-)来控制. 进程在因收到SIGQUIT退出时会产生core文件, 在这个意义上类似于一个程序错误信号。</li>
<li>SIGILL<br>
执行了非法指令. 通常是因为可执行文件本身出现错误, 或者试图执行数据段. 堆栈溢出时也有可能产生这个信号。</li>
<li>SIGTRAP<br>
由断点指令或其它trap指令产生. 由debugger使用。</li>
<li>SIGABRT<br>
调用abort函数生成的信号。</li>
<li>SIGBUS<br>
非法地址, 包括内存地址对齐(alignment)出错。比如访5261问一个四个字长的整数, 但其地址不是4的倍数。它与SIGSEGV的区别在于后者是由于对合法存储地址的非法访问触发的(如访问不属于自己存储空间或只读存储空间)。</li>
<li>SIGFPE<br>
在发生致命的算术运算错误时发出. 不仅包括浮点运算错误, 还包括溢出及除数为0等其它所有的算术的错误。</li>
<li>SIGKILL<br>
用来立即结束程序的运行. 本信号不能被阻塞、处理和忽略。如果管理员发现某个进程终止不了，可尝试发送这个信号。</li>
<li>SIGUSR1<br>
留给用户使用</li>
<li>SIGSEGV<br>
试图访问未分配给自己的内存, 或试图往没有写权限的内存地址写数据.</li>
<li>SIGUSR2<br>
留给用户使用</li>
<li>SIGPIPE<br>
管道破裂。这个信号4102通常在进程间通信产生，比如采用FIFO(管道)通信的两个进程，读管道没打开或者意外终止就往管道写，写进程会收到SIGPIPE信号。此外用Socket通信的两个进程，写进程在写Socket的时候，读进程已经终止。</li>
<li>SIGALRM<br>
时钟定时信号, 计算的是实际的时间或时钟时间. alarm函数使用该信号.</li>
<li>SIGTERM<br>
程序结束(terminate)信号, 与SIGKILL不同的是该信号可以被阻塞和处理。通常用来要求程序自己正常退出，shell命令kill缺省产生这个信号。如果进程终止不了，我们才会尝试SIGKILL。</li>
<li>SIGCHLD<br>
子进程结束时1653, 父进程会收到这个信号。<br>
如果父进程没有处理这个信号，也没有等待(wait)子进程，子进程虽然终止，但是还会在内核进程表中占有表项，这时的子进程称为僵尸进程。这种情况我们应该避免(父进程或者忽略SIGCHILD信号，或者捕捉它，或者wait它派生的子进程，或者父进程先终止，这时子进程的终止自动由init进程来接管)。</li>
<li>SIGCONT<br>
让一个停止(stopped)的进程继续执行. 本信号不能被阻塞. 可以用一个handler来让程序在由stopped状态变为继续执行时完成特定的工作. 例如, 重新显示提示符</li>
<li>SIGSTOP<br>
停止(stopped)进程的执行. 注意它和terminate以及interrupt的区别:该进程还未结束, 只是暂停执行. 本信号不能被阻塞, 处理或忽略.</li>
<li>SIGTSTP<br>
停止进程的运行, 但该信号可以被处理和忽略. 用户键入SUSP字符时(通常是Ctrl-Z)发出这个信号</li>
<li>SIGTTIN<br>
当后台作业要从用户终端读数据时, 该作业中的所有进程会收到SIGTTIN信号. 缺省时这些进程会停止执行.</li>
<li>SIGTTOU<br>
类似于SIGTTIN, 但在写终端(或修改终端模式)时收到.</li>
<li>SIGURG<br>
有"紧急"数据或out-of-band数据到达socket时产生.</li>
<li>SIGXCPU<br>
超过CPU时间资源限制. 这个限制可以由getrlimit/setrlimit来读取/改变。</li>
<li>SIGXFSZ<br>
当进程企图扩大文件以至于超过文件大小资源限制。</li>
<li>SIGVTALRM<br>
虚拟时钟信号. 类似于SIGALRM, 但是计算的是该进程占用的CPU时间.</li>
<li>SIGPROF<br>
类似于SIGALRM/SIGVTALRM, 但包括该进程用的CPU时间以及系统调用的时间.</li>
<li>SIGWINCH<br>
窗口大小改变时发出.</li>
<li>SIGIO<br>
文件描述符准备就绪, 可以开始进行输入/输出操作.</li>
<li>SIGPWR<br>
Power failure</li>
<li>SIGSYS<br>
非法的系统调用。<br>
在以上列出的信号中，程序不可捕获、阻塞或忽略的信号有：SIGKILL,SIGSTOP<br>
不能恢复至默认动作的信号有：SIGILL,SIGTRAP<br>
默认会导致进程流产的信号有：SIGABRT,SIGBUS,SIGFPE,SIGILL,SIGIOT,SIGQUIT,SIGSEGV,SIGTRAP,SIGXCPU,SIGXFSZ<br>
默认会导致进程退出的信号有：SIGALRM,SIGHUP,SIGINT,SIGKILL,SIGPIPE,SIGPOLL,SIGPROF,SIGSYS,SIGTERM,SIGUSR1,SIGUSR2,SIGVTALRM<br>
默认会导致进程停止的信号有：SIGSTOP,SIGTSTP,SIGTTIN,SIGTTOU<br>
默认进程忽略的信号有：SIGCHLD,SIGPWR,SIGURG,SIGWINCH<br>
此外，SIGIO在SVR4是退出，在4.3BSD中是忽略；SIGCONT在进程挂起时是继续，否则是忽略，不能被阻塞</li>
</ol>

+ core 文件是程序的某一个现场,一般是错误的现场:做一个段错误,ulimit -c 1 可以产生一个core文件,可以在选项中加入gdb进行调试,

+   signal():sig_t signal(int sig,sig_t func);第一个为信号第二个为行为,返回之前动作的函数. 在man手册的第三章节中写了对于sig_t的宏定义,void (*signal(int sig,void (\*func)(int)))(int); --> 或者更好阅读的方式可以看到 .... typedef void (\*sig_t)(int).
    +   这种函数调用的暴露了C的一个大问题,名空间安排不善,也就是C++中的namespace,C中并没有,如果2个库都倒霉的有一个函数名字,那么就**炸**了
### 多线程来实现并发