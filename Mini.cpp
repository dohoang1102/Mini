#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include ".\\XLDownload.h"
#include ".\\XLError.h"
#pragma comment(lib, ".\\XLDownload.lib")
int xunlei()
{
     // 1、初始化下载引擎
     if ( FALSE == XLInitDownloadEngine() )
     {
          printf("初始化下载引擎出错。\n");
          return 1;
     }

     // 2、启动新任务
     // PS：如果链接无法通过，请把工程属性“将wchar_t设置为内置类型        (/Zc:wchar_t)”设置为Yes
	 char  downfile[100]={0};
	 TCHAR Ldownfile[100]={0};
	 scanf("%s",downfile);
	 mbstowcs(Ldownfile,downfile,100);
     LONG   lTaskId = 0;
     DWORD  dwRet   = XLURLDownloadToFile(
                       _T("z:\\xmp.gif"), 
                       _T(downfile), 
                       _T(""), lTaskId);
     if ( XL_SUCCESS != dwRet )
     {
          XLUninitDownloadEngine();
          printf("创建新任务失败,错误代码：%d.\n", dwRet);
		  system("ping 127.0 -n 3 > NUL");
          return 1;
     }

     printf("开始下载文件\n");

     // 3、查询任务状态
     do 
     {
          ::Sleep(1000);

          ULONGLONG   ullFileSize = 0;
          ULONGLONG   ullRecvSize = 0;
          LONG        lStatus     = -1;

          dwRet = XLQueryTaskInfo(lTaskId, &lStatus, &ullFileSize, &ullRecvSize);
          if ( XL_SUCCESS==dwRet )
          {
               // 输出进度信息
               if ( 0 != ullFileSize )
               {
                    double  douProgress = (double)ullRecvSize/(double)ullFileSize;
                    douProgress *= 100.0;
                    printf("下载进度:%.2f%%\r", douProgress);
               }
               else
               {
                    printf("文件为空文件。\r");
               }

               if ( enumTaskStatus_Success==lStatus )
               {
                    printf("\n下载成功");
                    break;
               }

               if ( enumTaskStatus_Fail==lStatus )
               {
                    printf("\n下载失败");
                    break;
               }
          }
     } while( XL_SUCCESS==dwRet );

     // 4、无论是否下载成功，都必须调用XLStopTask
     XLStopTask(lTaskId);

     // 5、释放资源
     XLUninitDownloadEngine();
     return 0;
}
int main()
{
	system("ping 127.0 -n 1 > NUL");
	int a;

	printf("*************************************************\n");
	printf("欢迎使用修罗下载系统!\n\r1.迅雷下载\n\r2.退出系统");
	printf("\n*************************************************\n");
	scanf("%d",&a);
	switch(a)
	{
	case 1:
		xunlei();break;
	case 2:
		printf("本系统将在5秒后关闭");
		system("ping 127.0 -n 5 > NUL");
		exit(0);break;
	}
}
