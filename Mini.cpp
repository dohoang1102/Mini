#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include ".\\XLDownload.h"
#include ".\\XLError.h"
#pragma comment(lib, ".\\XLDownload.lib")
int xunlei()
{
     // 1����ʼ����������
     if ( FALSE == XLInitDownloadEngine() )
     {
          printf("��ʼ�������������\n");
          return 1;
     }

     // 2������������
     // PS����������޷�ͨ������ѹ������ԡ���wchar_t����Ϊ��������        (/Zc:wchar_t)������ΪYes
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
          printf("����������ʧ��,������룺%d.\n", dwRet);
		  system("ping 127.0 -n 3 > NUL");
          return 1;
     }

     printf("��ʼ�����ļ�\n");

     // 3����ѯ����״̬
     do 
     {
          ::Sleep(1000);

          ULONGLONG   ullFileSize = 0;
          ULONGLONG   ullRecvSize = 0;
          LONG        lStatus     = -1;

          dwRet = XLQueryTaskInfo(lTaskId, &lStatus, &ullFileSize, &ullRecvSize);
          if ( XL_SUCCESS==dwRet )
          {
               // ���������Ϣ
               if ( 0 != ullFileSize )
               {
                    double  douProgress = (double)ullRecvSize/(double)ullFileSize;
                    douProgress *= 100.0;
                    printf("���ؽ���:%.2f%%\r", douProgress);
               }
               else
               {
                    printf("�ļ�Ϊ���ļ���\r");
               }

               if ( enumTaskStatus_Success==lStatus )
               {
                    printf("\n���سɹ�");
                    break;
               }

               if ( enumTaskStatus_Fail==lStatus )
               {
                    printf("\n����ʧ��");
                    break;
               }
          }
     } while( XL_SUCCESS==dwRet );

     // 4�������Ƿ����سɹ������������XLStopTask
     XLStopTask(lTaskId);

     // 5���ͷ���Դ
     XLUninitDownloadEngine();
     return 0;
}
int main()
{
	system("ping 127.0 -n 1 > NUL");
	int a;

	printf("*************************************************\n");
	printf("��ӭʹ����������ϵͳ!\n\r1.Ѹ������\n\r2.�˳�ϵͳ");
	printf("\n*************************************************\n");
	scanf("%d",&a);
	switch(a)
	{
	case 1:
		xunlei();break;
	case 2:
		printf("��ϵͳ����5���ر�");
		system("ping 127.0 -n 5 > NUL");
		exit(0);break;
	}
}
